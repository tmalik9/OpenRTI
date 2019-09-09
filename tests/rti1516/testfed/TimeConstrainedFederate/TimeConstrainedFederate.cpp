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
#include <memory.h>
#include "TimeConstrainedAmb.h"
#include "TimeConstrainedFederate.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>
#include <RTI/RTI1516fedTime.h>
#include <RTI/LogicalTimeInterval.h>

using namespace std;
using namespace rti1516;

TimeConstrainedFederate::TimeConstrainedFederate()
{
}

TimeConstrainedFederate::~TimeConstrainedFederate()
{
  if (this->fedamb)
  {
    delete this->fedamb;
  }
}

/**
 * convert normal C++-Strings to C++-Wstrings
 * Wstrings are used by openrti
 */
std::wstring
TimeConstrainedFederate::convertStringToWstring(const std::string& str)
{
  const std::ctype<wchar_t>& CType = std::use_facet<std::ctype<wchar_t> >(std::locale());
  std::vector<wchar_t> wideStringBuffer(str.length());
  CType.widen(str.data(), str.data() + str.length(), &wideStringBuffer[0]);
  return std::wstring(&wideStringBuffer[0], wideStringBuffer.size());
}

/**
 * Everything will be handled here
 *
 * @param federateName How the federate is called in simulation
 * @return void
 */
void TimeConstrainedFederate::runFederate(std::string federateName, std::string fom, std::string address)
{
  ///
  /// 1. create the RTIambassador
  ///
  rti1516::RTIambassadorFactory* factory = new rti1516::RTIambassadorFactory();
  std::vector<std::wstring> args; //!< vector args contains all necessary rti parameters
  args.push_back(L"protocol=rti");
  args.push_back(L"address=" + convertStringToWstring(address));
  this->rtiamb = factory->createRTIambassador(args);
  ///
  /// 2. create and join to the federation
  /// NOTE: some other federate may have already created the federation,
  ///       in that case, we'll just try and join it
  ///
  try
  {
    rtiamb->createFederationExecution(L"ExampleFederation",
                                      convertStringToWstring(fom));
    cout << "Created Federation" << endl;
  }
  catch (FederationExecutionAlreadyExists exists)
  {
    cout << "Didn't create federation, it already existed" << endl;
  }
  /////////////////////////////
  /// 3. join the federation
  /////////////////////////////
  /// create the federate ambassador and join the federation
  this->fedamb = new TimeConstrainedFedAmb(this->rtiamb.get());
  rtiamb->joinFederationExecution(convertStringToWstring(federateName),
                                  convertStringToWstring("ExampleFederation"), *fedamb);
  cout << "Joined Federation as " << federateName << endl;
  /// initialize the handles - have to wait until we are joined
  initializeHandles();
  /////////////////////////////////
  /// 4. announce the sync point
  /////////////////////////////////
  /// announce a sync point to get everyone on the same page. if the point
  /// has already been registered, we'll get a callback saying it failed,
  /// but we don't care about that, as long as someone registered it
  rtiamb->registerFederationSynchronizationPoint( READY_TO_RUN, toVariableLengthData(L""));
  rtiamb->registerFederationSynchronizationPoint( ALL_DONE, toVariableLengthData(L""));
  cout << "SynchronizationPoint registered" << endl;
  while (!fedamb->isAnnouncedReadyToRun || !fedamb->isAnnouncedAllDone)
  {
    rtiamb->evokeCallback(12.0);
  }
  /// WAIT FOR USER TO KICK US OFF.\n
  /// So that there is time to add other federates, we will wait until the
  /// user hits enter before proceeding. That was, you have time to start
  /// other federates.
  waitForUser();
  ////////////////////////////////////////////////////////
  /// 5. achieve the point and wait for synchronization
  ////////////////////////////////////////////////////////
  /// tell the RTI we are ready to move past the sync point and then wait
  /// until the federation has synchronized on
  rtiamb->synchronizationPointAchieved(READY_TO_RUN);
  wcout << L"Achieved sync point: " << READY_TO_RUN << L", waiting for federation..." << endl;
  while (fedamb->isReadyToRun == false)
  {
    rtiamb->evokeCallback(12.0);
  }
  /////////////////////////////
  // 6. enable time policies
  /////////////////////////////

  /////////////////////////////
  /// enable time constrained
  /////////////////////////////
  rtiamb->enableTimeConstrained();
  /// wait for callback
  while (fedamb->isConstrained == false)
  {
    rtiamb->evokeCallback(12.0);
  }
  cout << "Time Policy Enabled" << endl;

  myPublishedObjectClass = rtiamb->getObjectClassHandle(L"HLAobjectRoot.B");

  //////////////////////////////
  // 7. publish and subscribe
  //////////////////////////////
  // in this section we tell the RTI of all the data we are going to
  // produce, and all the data we want to know about
  publishAndSubscribe();
  cout << "Published and Subscribed" << endl;
  /////////////////////////////////////
  // 8. register an object to update
  /////////////////////////////////////
  myPublishedObject = rtiamb->registerObjectInstance(myPublishedObjectClass);
  wcout << L"Registered Object, handle=" << myPublishedObject << L" class=" << rtiamb->getObjectClassName(rtiamb->getKnownObjectClassHandle(myPublishedObject)) << endl;
  ////////////////////////////////////
  // 9. do the main simulation loop
  ////////////////////////////////////
  // here is where we do the meat of our work. in each iteration, we will
  // update the attribute values of the object we registered, and will
  // send an interaction.
  for (int i = 0; i < 20; i++)
  {
    /// 9.1 update the attribute values of the instance
    updateAttributeValues(myPublishedObject);
    /// 9.2 send an interaction
    sendInteraction();
    /// 9.3 request a time advance and wait until we get it
    advanceTime(1.0);
    //cout << "Time Advanced to " << fedamb->federateTime << endl;
  }

  rtiamb->synchronizationPointAchieved(ALL_DONE);
  while (fedamb->allDone == false)
  {
    rtiamb->evokeCallback(12.0);
  }
  //////////////////////////////////////
  // 10. delete the object we created
  //////////////////////////////////////
  deleteObject(myPublishedObject);
  wcout << "Deleted Object, handle=" << myPublishedObject << endl;
  ////////////////////////////////////
  // 11. resign from the federation
  ////////////////////////////////////
  rtiamb->resignFederationExecution(NO_ACTION);
  cout << "Resigned from Federation" << endl;
  ////////////////////////////////////////
  // 12. try and destroy the federation
  ////////////////////////////////////////
  // NOTE: we won't die if we can't do this because other federates
  //       remain. in that case we'll leave it for them to clean up
  try
  {
    rtiamb->destroyFederationExecution(L"ExampleFederation");
    cout << "Destroyed Federation" << endl;
  }
  catch (FederationExecutionDoesNotExist dne)
  {
    cout << "No need to destroy federation, it doesn't exist" << endl;
  }
  catch (FederatesCurrentlyJoined fcj)
  {
    cout << "Didn't destroy federation, federates still joined" << endl;
  }
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Helper Methods ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**
 * This method will get all the relevant handle information from the RTIambassador
 */
void TimeConstrainedFederate::initializeHandles()
{
  this->aHandle  = rtiamb->getObjectClassHandle(L"HLAobjectRoot.A");
  this->aaHandle = rtiamb->getAttributeHandle(this->aHandle, L"aa");
  this->abHandle = rtiamb->getAttributeHandle(this->aHandle, L"ab");
  this->acHandle = rtiamb->getAttributeHandle(this->aHandle, L"ac");
  this->bHandle  = rtiamb->getObjectClassHandle(L"HLAobjectRoot.B");
  this->baHandle = rtiamb->getAttributeHandle(this->bHandle, L"ba");
  this->bbHandle = rtiamb->getAttributeHandle(this->bHandle, L"bb");
  this->bcHandle = rtiamb->getAttributeHandle(this->bHandle, L"bc");
  this->xHandle  = rtiamb->getInteractionClassHandle(L"HLAinteractionRoot.X");
  this->xaHandle = rtiamb->getParameterHandle(this->xHandle,
                   convertStringToWstring("xa"));
  this->xbHandle = rtiamb->getParameterHandle(this->xHandle,
                   convertStringToWstring("xb"));
}

/**
 * Blocks until the user hits enter
 */
void TimeConstrainedFederate::waitForUser()
{
  cout << " >>>>>>>>>> Press Enter to Continue <<<<<<<<<<" << endl;
  string line;
  getline(cin, line);
}


/**
 * This method will inform the RTI about the types of data that the federate will
 * be creating, and the types of data we are interested in hearing about as other
 * federates produce it.
 */
void TimeConstrainedFederate::publishAndSubscribe()
{
  /////////////////////////////////////////////
  /// publish all attributes of ObjectRoot.A
  /////////////////////////////////////////////
  /// before we can register instance of the object class ObjectRoot.A and
  /// update the values of the various attributes, we need to tell the RTI
  /// that we intend to publish this information\n
  /// package the information into a handle set
  AttributeHandleSet attributesOfA;
  AttributeHandleSet attributesOfB;
  attributesOfA.insert(this->aaHandle);
  attributesOfA.insert(this->abHandle);
  attributesOfA.insert(this->acHandle);

  attributesOfB.insert(this->baHandle);
  attributesOfB.insert(this->bbHandle);
  attributesOfB.insert(this->bcHandle);

  /// do the actual publication

  myPublishedObjectClass = this->bHandle;
  myPublishedAttributes = attributesOfB;
  rtiamb->publishObjectClassAttributes(myPublishedObjectClass, myPublishedAttributes);
  //////////////////////////////////////////////////
  /// subscribe to all attributes of ObjectRoot.A
  //////////////////////////////////////////////////
  /// we also want to hear about the same sort of information as it is
  /// created and altered in other federates, so we need to subscribe to it
  rtiamb->subscribeObjectClassAttributes(this->aHandle, attributesOfA, true);
  rtiamb->subscribeObjectClassAttributes(this->bHandle, attributesOfB, true);
  //////////////////////////////////////////////////////
  /// publish the interaction class InteractionRoot.X
  //////////////////////////////////////////////////////
  /// we want to send interactions of type InteractionRoot.X, so we need
  /// to tell the RTI that we're publishing it first. We don't need to
  /// inform it of the parameters, only the class, making it much simpler
  /// do the publication
  rtiamb->publishInteractionClass(this->xHandle);
  /////////////////////////////////////////////////////
  /// subscribe to the InteractionRoot.X interaction //
  /////////////////////////////////////////////////////
  /// we also want to receive other interaction of the same type that are
  /// sent out by other federates, so we have to subscribe to it first
  rtiamb->subscribeInteractionClass(this->xHandle);
}

/**
 * This method will register an instance of the class ObjectRoot.A and will
 * return the federation-wide unique handle for that instance. Later in the
 * simulation, we will update the attribute values for this instance
 */
//ObjectInstanceHandle ExampleCPPFederate::registerObject(ObjectClassHandle theObjectClass)
//{
//  myPublishedObject = rtiamb->registerObjectInstance(theObjectClass);
//  return myPublishedObject;
//}

/**
 * something like a serialize function.\n
 * The RTI does not know everything about datatypes. It just knows a byte
 * format. To get this openrti offers VariableLengthData.
 */
rti1516::VariableLengthData
TimeConstrainedFederate::toVariableLengthData(const wchar_t* s)
{
  rti1516::VariableLengthData variableLengthData;
  if (s)
  {
    variableLengthData.setData(s, (unsigned long)(wcslen(s) * sizeof(wchar_t)));
  }
  return variableLengthData;
}

/**
 * This method will update all the values of the given object instance. It will
 * set each of the values to be a string which is equal to the name of the
 * attribute plus the current time. eg "aa:10.0" if the time is 10.0.
 * <p/>
 * Note that we don't actually have to update all the attributes at once, we
 * could update them individually, in groups or not at all!
 */
void TimeConstrainedFederate::updateAttributeValues(ObjectInstanceHandle theObject)
{
  std::wcout << __FUNCTIONW__ << L": " << rtiamb->getObjectClassName(rtiamb->getKnownObjectClassHandle(theObject)) << std::endl;
  ////////////////////////////////////////////////
  /// create the necessary container and values
  ///////////////////////////////////////////////
  /// create the collection to store the values in, as you can see
  /// this is quite a lot of work
  AttributeHandleValueMap attributeValues;
  /// generate the new values
  for (auto attr : myPublishedAttributes)
  {
    wchar_t value[16];
    swprintf(value, 16, L"%s:%f", rtiamb->getAttributeName(myPublishedObjectClass, attr).c_str(), getLbts());
    attributeValues[attr] = toVariableLengthData(value);
  }
  ///////////////////////////
  /// do the actual update
  ///////////////////////////
  rtiamb->updateAttributeValues(myPublishedObject, attributeValues, toVariableLengthData(L"hi!"));
  /// note that if you want to associate a particular timestamp with the
  /// update. here we send another update, this time with a timestamp:
  //RTI1516fedTime time = fedamb->federateTime + fedamb->federateLookahead;
  //rtiamb->updateAttributeValues(myPublishedObject, attributeValues, toVariableLengthData(L"hi!"), time);
}

/**
 * This method will send out an interaction of the type InteractionRoot.X. Any
 * federates which are subscribed to it will receive a notification the next time
 * they tick(). Here we are passing only two of the three parameters we could be
 * passing, but we don't actually have to pass any at all!
 */
void TimeConstrainedFederate::sendInteraction()
{
  ////////////////////////////////////////////////
  /// create the necessary container and values
  ////////////////////////////////////////////////
  /// create the collection to store the values in
  ParameterHandleValueMap parameters;
  /// generate the new values
  wchar_t xaValue[16], xbValue[16];
  swprintf(xaValue, 16, L"xa:%f", getLbts());
  swprintf(xbValue, 16, L"xb:%f", getLbts());
  parameters[xaHandle] = toVariableLengthData(xaValue);
  parameters[xbHandle] = toVariableLengthData(xbValue);
  ///////////////////////////
  /// send the interaction
  ///////////////////////////
  rtiamb->sendInteraction(xHandle, parameters, toVariableLengthData(L"hi!"));
  /// if you want to associate a particular timestamp with the
  /// interaction, you will have to supply it to the RTI. Here
  /// we send another interaction, this time with a timestamp:
  RTI1516fedTime time = fedamb->federateTime + fedamb->federateLookahead;
  rtiamb->sendInteraction(xHandle, parameters, toVariableLengthData(L"hi!"), time);
}

/**
 * This method will request a time advance to the current time, plus the given
 * timestep. It will then wait until a notification of the time advance grant
 * has been received.
 */
void TimeConstrainedFederate::advanceTime(double timestep)
{
  wprintf(L"%0.6f %s: timestep=%f\n",  fedamb->federateTime, __FUNCTIONW__, timestep);
  /// request the advance
  fedamb->isAdvancing = true;
  RTI1516fedTime newTime = (fedamb->federateTime + timestep);
  rtiamb->timeAdvanceRequest(newTime);
  /// wait for the time advance to be granted. ticking will tell the
  /// LRC to start delivering callbacks to the federate
  while (fedamb->isAdvancing)
  {
    rtiamb->evokeCallback(12.0);
  }
  wprintf(L"%0.6f %s: new time=%f\n",  fedamb->federateTime, __FUNCTIONW__, fedamb->federateTime);
}

/**
 * This method will attempt to delete the object instance of the given
 * handle. We can only delete objects we created, or for which we own the
 * privilegeToDelete attribute.
 */
void TimeConstrainedFederate::deleteObject(ObjectInstanceHandle objectHandle)
{
  rtiamb->deleteObjectInstance(objectHandle, toVariableLengthData(L""));
}
