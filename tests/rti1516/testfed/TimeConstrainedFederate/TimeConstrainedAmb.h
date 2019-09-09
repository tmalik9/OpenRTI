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

#ifndef EXAMPLEFEDAMB_H_
#define EXAMPLEFEDAMB_H_

#include <RTI/NullFederateAmbassador.h>
#include <RTI/RTIambassador.h>

using namespace rti1516;


#define READY_TO_RUN L"ReadyToRun"
#define ALL_DONE L"AllDone"

class TimeConstrainedFedAmb : public NullFederateAmbassador
{
public:
  // variables //
  double federateTime;
  double federateLookahead;

  bool isRegulating;
  bool isConstrained;
  bool isAdvancing;
  bool isAnnouncedReadyToRun;
  bool isAnnouncedAllDone;
  bool isReadyToRun;
  bool allDone;
  // methods //
  //! Constructor
  TimeConstrainedFedAmb(rti1516::RTIambassador* rtiamb);
  //! Destructor
  virtual ~TimeConstrainedFedAmb() throw();

  ///////////////////////////////////
  // synchronization point methods //
  ///////////////////////////////////
  virtual void synchronizationPointRegistrationSucceeded(std::wstring const& label);

  virtual void synchronizationPointRegistrationFailed(std::wstring const& label, SynchronizationFailureReason reason);

  virtual void announceSynchronizationPoint(std::wstring const& label, VariableLengthData const& theUserSuppliedTag);

  virtual void federationSynchronized(std::wstring const& label);

  //////////////////////////
  // time related methods //
  //////////////////////////
  virtual void timeRegulationEnabled(LogicalTime const& theFederateTime);

  virtual void timeConstrainedEnabled(LogicalTime const& theFederateTime);

  virtual void timeAdvanceGrant(LogicalTime const& theTime);

  ///////////////////////////////
  // object management methods //
  ///////////////////////////////
  virtual void discoverObjectInstance(ObjectInstanceHandle theObject,
                                      ObjectClassHandle theObjectClass,
                                      std::wstring const& theObjectInstanceName) override;

  virtual void attributesInScope(ObjectInstanceHandle, const AttributeHandleSet&) override;

  virtual void attributesOutOfScope(ObjectInstanceHandle, const AttributeHandleSet&) override;

  virtual void reflectAttributeValues(ObjectInstanceHandle theObject,
                                      AttributeHandleValueMap const& theAttributeValues,
                                      VariableLengthData const& theUserSuppliedTag,
                                      OrderType sentOrder,
                                      TransportationType theType) override;

  virtual void reflectAttributeValues(ObjectInstanceHandle theObject,
                                      AttributeHandleValueMap const& theAttributeValues,
                                      VariableLengthData const& theUserSuppliedTag,
                                      OrderType sentOrder,
                                      TransportationType theType,
                                      RegionHandleSet const& theSentRegionHandleSet) override;

  virtual void provideAttributeValueUpdate(ObjectInstanceHandle theObject,
                                           AttributeHandleSet const& theAttributes,
                                           VariableLengthData const& theUserSuppliedTag) override;

  virtual void receiveInteraction(InteractionClassHandle theInteraction, ParameterHandleValueMap const& theParameterValues,
                                  VariableLengthData const& theUserSuppliedTag, OrderType sentOrder, TransportationType theType);
  virtual void receiveInteraction(InteractionClassHandle theInteraction, ParameterHandleValueMap const& theParameterValues,
                                  VariableLengthData const& theUserSuppliedTag, OrderType sentOrder, TransportationType theType,
                                  RegionHandleSet const& theSentRegionHandleSet);
  virtual void receiveInteraction(InteractionClassHandle theInteraction, ParameterHandleValueMap const& theParameterValues,
                                  VariableLengthData const& theUserSuppliedTag, OrderType sentOrder, TransportationType theType, LogicalTime const& theTime,
                                  OrderType receivedOrder);
  virtual void receiveInteraction(InteractionClassHandle theInteraction, ParameterHandleValueMap const& theParameterValues,
                                  VariableLengthData const& theUserSuppliedTag, OrderType sentOrder, TransportationType theType, LogicalTime const& theTime,
                                  OrderType receivedOrder, RegionHandleSet const& theSentRegionHandleSet);
  virtual void receiveInteraction(InteractionClassHandle theInteraction, ParameterHandleValueMap const& theParameterValues,
                                  VariableLengthData const& theUserSuppliedTag, OrderType sentOrder, TransportationType theType, LogicalTime const& theTime,
                                  OrderType receivedOrder, MessageRetractionHandle theHandle);
  virtual void receiveInteraction(InteractionClassHandle theInteraction, ParameterHandleValueMap const& theParameterValues,
                                  VariableLengthData const& theUserSuppliedTag, OrderType sentOrder, TransportationType theType, LogicalTime const& theTime,
                                  OrderType receivedOrder, MessageRetractionHandle theHandle,
                                  RegionHandleSet const& theSentRegionHandleSet);

  virtual void removeObjectInstance(ObjectInstanceHandle theObject, VariableLengthData const& theUserSuppliedTag,
                                    OrderType sentOrder) ;


  virtual void removeObjectInstance(ObjectInstanceHandle theObject, VariableLengthData const& theUserSuppliedTag, OrderType sentOrder,
                                    LogicalTime const& theTime, OrderType receivedOrder);
  virtual void removeObjectInstance(ObjectInstanceHandle theObject, VariableLengthData const& theUserSuppliedTag, OrderType sentOrder,
                                    LogicalTime const& theTime, OrderType receivedOrder, MessageRetractionHandle theHandle);


  /////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////// Private Section ////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
private:
  double convertTime(LogicalTime const& theTime);
  std::wstring variableLengthDataToWstring(const rti1516::VariableLengthData& variableLengthData);

  rti1516::RTIambassador* rtiamb; /*!< RTI ambassador to comunicate with RTI */
};

#endif /*EXAMPLEFEDAMB_H_*/
