/**
 * @file
 *
 * Copyright (C) 2013 Max Oberberger <max@oberbergers.de>
 *
 * Last modified: 2013 February 23, 20:05:02 by max
 *
 * This file is part of ba-hla.
 *
 * ba-hla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ba-hla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ba-hla.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "TimeRegulatingAmb.h"
#include <string.h>
#include <RTI/RTI1516fedTime.h>

//using namespace std;
using std::wcout;
using std::endl;
using namespace rti1516;

/**
 * NOTE: The main part of the functions in this file is to print something on stdout
 * to see whether the function is called or not.
 */
TimeRegulatingFedAmb::TimeRegulatingFedAmb(rti1516::RTIambassador* rtiamb)
{
  // initialize all the variable values
  this->rtiamb = rtiamb;
  this->federateTime      = 0.0;
  this->federateLookahead = 1.0;
  this->isRegulating  = false;
  this->isConstrained = false;
  this->isAdvancing   = false;
  this->isAnnouncedReadyToRun   = false;
  this->isAnnouncedAllDone   = false;
  this->isReadyToRun  = false;
  this->allDone  = false;
}

TimeRegulatingFedAmb::~TimeRegulatingFedAmb()
throw()
{
}

/**
 * convert Logical System time to Federate time which can be understood by RTI
 */
double TimeRegulatingFedAmb::convertTime(LogicalTime const& theTime)
{
  RTI1516fedTime castedTime = (RTI1516fedTime)theTime;
  return castedTime.getFedTime();
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// Synchronization Point Callbacks ///////////////////////
///////////////////////////////////////////////////////////////////////////////
void TimeRegulatingFedAmb::synchronizationPointRegistrationSucceeded(std::wstring const& label)
{
  wcout << L"Successfully registered sync point: " << label << endl;
}

void TimeRegulatingFedAmb::synchronizationPointRegistrationFailed(std::wstring const& label,  SynchronizationFailureReason reason)
{
  wcout << L"Failed to register sync point: " << label << endl;
}

void TimeRegulatingFedAmb::announceSynchronizationPoint(std::wstring const& label, VariableLengthData const& theUserSuppliedTag)
{
  wcout << L"Synchronization point announced: " << label << endl;
  std::wstring compair = L"ReadyToRun";
  if (label ==  READY_TO_RUN)
  {
    this->isAnnouncedReadyToRun = true;
  }
  else if (label == ALL_DONE)
  {
    this->isAnnouncedAllDone = true;
  }
}

void TimeRegulatingFedAmb::federationSynchronized(std::wstring const& label)
{
  wcout << L"Federation Synchronized: " << label << endl;
  if (label == READY_TO_RUN)
  {
    this->isReadyToRun = true;
  }
  else if (label == ALL_DONE)
  {
    this->allDone = true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Time Callbacks ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TimeRegulatingFedAmb::timeRegulationEnabled(LogicalTime const& theFederateTime)
{
  this->isRegulating = true;
  this->federateTime = convertTime(theFederateTime);
}

void TimeRegulatingFedAmb::timeConstrainedEnabled(LogicalTime const& theFederateTime)
{
  this->isConstrained = true;
  this->federateTime = convertTime(theFederateTime);
}

void TimeRegulatingFedAmb::timeAdvanceGrant(LogicalTime const& theTime)
{
  this->isAdvancing = false;
  this->federateTime = convertTime(theTime);
  printf("timeAdvanceGrant(%.9f)\n", this->federateTime);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////// Object Management Callbacks /////////////////////////
///////////////////////////////////////////////////////////////////////////////

//                         //
// Discover Object Methods //
//                         //
void TimeRegulatingFedAmb::discoverObjectInstance(ObjectInstanceHandle theObject,
  ObjectClassHandle theObjectClass,
  std::wstring const& theObjectInstanceName)
{
  wcout <<  __FUNCTIONW__ << L": "
    << " handle=" << theObject << L" instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", classHandle=" << rtiamb->getObjectClassName(theObjectClass)
    << L", name=" << theObjectInstanceName << endl;
}


void TimeRegulatingFedAmb::attributesInScope(ObjectInstanceHandle theObject,
  const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}


void TimeRegulatingFedAmb::attributesOutOfScope(ObjectInstanceHandle theObject,
  const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}

//                                 //
// Reflect Attribute Value Methods //
//                                 //
void TimeRegulatingFedAmb::reflectAttributeValues(ObjectInstanceHandle theObject,
  AttributeHandleValueMap const& theAttributeValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wstring strData(static_cast<const wchar_t*>(attr.second.data()), attr.second.size() / sizeof(wchar_t));
    wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr.first) << L" value=" << strData << std::endl;
  }
}

void TimeRegulatingFedAmb::reflectAttributeValues(ObjectInstanceHandle theObject,
  AttributeHandleValueMap const& theAttributeValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  RegionHandleSet const& theSentRegionHandleSet)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr.first) << std::endl;
  }
}


void TimeRegulatingFedAmb::provideAttributeValueUpdate(ObjectInstanceHandle theObject, AttributeHandleSet const& theAttributes,
  VariableLengthData const& theUserSuppliedTag)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributes)
  {
    wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}

/**
 * convert variableLengthData to Wstring (deserialize)\n
 * for serialize function see ExampleCPPFederate class
 */
std::wstring
TimeRegulatingFedAmb::variableLengthDataToWstring(const rti1516::VariableLengthData& variableLengthData)
{
  if (!variableLengthData.size())
  {
    return std::wstring();
  }
  return std::wstring((const wchar_t*) variableLengthData.data(), variableLengthData.size() / sizeof(std::wstring::value_type));
}

/**
 * receiveInteraction with some detailed output
 */
void TimeRegulatingFedAmb::receiveInteraction(InteractionClassHandle theInteraction, ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag, OrderType sentOrder, TransportationType theType)
{
  wcout << L"Interaction Received:";
  /// print the handle
  wcout << " handle=" << theInteraction;
  /// print the tag
  // std::wstring data = variableLengthDataToWstring(theUserSuppliedTag);
  //wcout << ", tag=\"" << data << "\"";
  /// print the attribute information
  wcout << ", parameterCount=" << theParameterValues.size();
  for (ParameterHandleValueMap::const_iterator i = theParameterValues.begin();
    i != theParameterValues.end(); i++)
  {
    /// print the parameter handle
    wcout << "\tparamHandle=" << i->first;
    /// print the parameter value
    std::wstring value = variableLengthDataToWstring(i->second);
    wcout << ", paramValue=\"" << value << L"\"" << std::endl;
  }
}

void TimeRegulatingFedAmb::receiveInteraction(InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  RegionHandleSet const& theSentRegionHandleSet)
{
  wcout << L"Interaction Received:" << endl;
}
void TimeRegulatingFedAmb::receiveInteraction(InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder)
{
  wcout << L"Interaction Received:" << endl;
}
void TimeRegulatingFedAmb::receiveInteraction(InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  RegionHandleSet const& theSentRegionHandleSet)
{
  wcout << L"Interaction Received:" << endl;
}

void TimeRegulatingFedAmb::receiveInteraction(InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  MessageRetractionHandle theHandle)
{
  wcout << L"Interaction Received:" << endl;
}

void TimeRegulatingFedAmb::receiveInteraction(InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  MessageRetractionHandle theHandle,
  RegionHandleSet const& theSentRegionHandleSet)
{
  wcout << L"Interaction Received:" << endl;
}

//                       //
// Remove Object Methods //
//                       //
void TimeRegulatingFedAmb::removeObjectInstance(ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder)
{
  wcout << L"Object Removed: handle=" << theObject << endl;
}

void TimeRegulatingFedAmb::removeObjectInstance(ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  LogicalTime const& theTime,
  OrderType receivedOrder)
{
  wcout << L"Object Removed: handle=" << theObject << endl;
}

void TimeRegulatingFedAmb::removeObjectInstance(ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  MessageRetractionHandle theHandle)
{
  wcout << L"Object Removed: handle=" << theObject << endl;
}
