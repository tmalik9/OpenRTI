/* -*-c++-*- OpenRTI - Copyright (C) 2009-2012 Mathias Froehlich
 *
 * This file is part of OpenRTI.
 *
 * OpenRTI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * OpenRTI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenRTI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef RTIambassadorImplementation_h
#define RTIambassadorImplementation_h

#include <Export.h>
#include <RTI/RTIambassador.h>

namespace OpenRTI {

class OPENRTI_LOCAL RTIambassadorImplementation : public rti1516e::RTIambassador {
public:
  RTIambassadorImplementation() throw ();
  virtual ~RTIambassadorImplementation();

  virtual void
  connect(rti1516e::FederateAmbassador & federateAmbassador, rti1516e::CallbackModel theCallbackModel,
          std::wstring const & localSettingsDesignator);

  virtual void
  disconnect();

  virtual void
  createFederationExecution(std::wstring const & federationExecutionName,
                            std::wstring const & fomModule,
                            std::wstring const & logicalTimeImplementationName);

  virtual void
  createFederationExecution(std::wstring const & federationExecutionName,
                            std::vector<std::wstring> const & fomModules,
                            std::wstring const & logicalTimeImplementationName);

  virtual void
  createFederationExecutionWithMIM (std::wstring const & federationExecutionName,
                             std::vector<std::wstring> const & fomModules,
                             std::wstring const & mimModule,
                             std::wstring const & logicalTimeImplementationName);

  virtual void
  destroyFederationExecution(std::wstring const & federationExecutionName);

  virtual void
  listFederationExecutions();

  virtual rti1516e::FederateHandle
  joinFederationExecution(std::wstring const & federateType,
                          std::wstring const & federationExecutionName,
                          std::vector<std::wstring> const & additionalFomModules);

  virtual rti1516e::FederateHandle
  joinFederationExecution(std::wstring const & federateName,
                          std::wstring const & federateType,
                          std::wstring const & federationExecutionName,
                          std::vector<std::wstring> const & additionalFomModules);

  // 4.5
  virtual void
  resignFederationExecution(rti1516e::ResignAction rti1516ResignAction);

  // 4.6
  virtual void
  registerFederationSynchronizationPoint(std::wstring const & label,
                                         rti1516e::VariableLengthData const & rti1516Tag);

  virtual void
  registerFederationSynchronizationPoint(std::wstring const & label,
                                         rti1516e::VariableLengthData const & rti1516Tag,
                                         rti1516e::FederateHandleSet const & rti1516FederateHandleSet);

  // 4.9
  virtual void
    synchronizationPointAchieved(std::wstring const & label, bool);

  // 4.11
  virtual void
  requestFederationSave(std::wstring const & label);

  virtual void
  requestFederationSave(const std::wstring& label,
                        const rti1516e::LogicalTime& rti1516LogicalTime);

  // 4.13
  virtual void
  federateSaveBegun();

  // 4.14
  virtual void
  federateSaveComplete();

  virtual void
  federateSaveNotComplete();

  virtual void
  abortFederationSave();

  // 4.16
  virtual void
  queryFederationSaveStatus ();

  // 4.18
  virtual void
  requestFederationRestore(std::wstring const & label);

  // 4.22
  virtual void
  federateRestoreComplete();

  virtual void
  federateRestoreNotComplete();

  virtual void
  abortFederationRestore ();

  // 4.24
  virtual void
  queryFederationRestoreStatus();

  /////////////////////////////////////
  // Declaration Management Services //
  /////////////////////////////////////

  // 5.2
  virtual void
  publishObjectClassAttributes(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                               rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet);

  // 5.3
  virtual void
  unpublishObjectClass(rti1516e::ObjectClassHandle rti1516ObjectClassHandle);

  virtual void
  unpublishObjectClassAttributes(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                 rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet);

  // 5.4
  virtual void
  publishInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle);

  // 5.5
  virtual void
  unpublishInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle);

  // 5.6
  virtual void
  subscribeObjectClassAttributes(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                 rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                 bool active, std::wstring const & updateRateDesignator);

  // 5.7
  virtual void
  unsubscribeObjectClass(rti1516e::ObjectClassHandle rti1516ObjectClassHandle);

  virtual void
  unsubscribeObjectClassAttributes(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                   rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet);

  // 5.8
  virtual void
  subscribeInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                            bool active);

  // 5.9
  virtual void
  unsubscribeInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle);

  ////////////////////////////////
  // Object Management Services //
  ////////////////////////////////

  virtual void
  reserveObjectInstanceName(std::wstring const & objectInstanceName);

  virtual void
  releaseObjectInstanceName(std::wstring const & objectInstanceName);

  virtual void
  reserveMultipleObjectInstanceName(std::set<std::wstring> const & theObjectInstanceNames);

  virtual void
  releaseMultipleObjectInstanceName(std::set<std::wstring> const & theObjectInstanceNames);

  // 6.4
  virtual rti1516e::ObjectInstanceHandle
  registerObjectInstance(rti1516e::ObjectClassHandle rti1516ObjectClassHandle);

  virtual rti1516e::ObjectInstanceHandle
  registerObjectInstance(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                         std::wstring const & objectInstanceName) override;

  rti1516e::ObjectInstanceHandle
  registerObjectInstance(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                   std::wstring const & objectInstanceName, bool allowUnreservedObjectNames) override;
  // 6.6
  virtual void
  updateAttributeValues(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                        const rti1516e::AttributeHandleValueMap& rti1516AttributeValues,
                        const rti1516e::VariableLengthData& rti1516Tag);

  virtual rti1516e::MessageRetractionHandle
  updateAttributeValues(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                        const rti1516e::AttributeHandleValueMap& rti1516AttributeValues,
                        const rti1516e::VariableLengthData& rti1516Tag,
                        const rti1516e::LogicalTime& rti1516LogicalTime);

  // 6.8
  virtual void
  sendInteraction(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                  const rti1516e::ParameterHandleValueMap& rti1516ParameterValues,
                  const rti1516e::VariableLengthData& rti1516Tag);

  virtual rti1516e::MessageRetractionHandle
  sendInteraction(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                  const rti1516e::ParameterHandleValueMap& rti1516ParameterValues,
                  const rti1516e::VariableLengthData& rti1516Tag,
                  const rti1516e::LogicalTime& rti1516LogicalTime);

  // 6.10
  virtual void
  deleteObjectInstance(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                       const rti1516e::VariableLengthData& rti1516Tag);

  virtual rti1516e::MessageRetractionHandle
  deleteObjectInstance(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                       const rti1516e::VariableLengthData& rti1516Tag,
                       const rti1516e::LogicalTime& rti1516LogicalTime);

  // 6.12
  virtual void
  localDeleteObjectInstance(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle);

  // 6.13
  virtual void
  changeAttributeTransportationType(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                    rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                    rti1516e::TransportationType rti1516TransportationType);

  // 6.14
  virtual void
  changeInteractionTransportationType(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                                      rti1516e::TransportationType rti1516TransportationType);

  // 6.17
  virtual void
  requestAttributeValueUpdate(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                              rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                              rti1516e::VariableLengthData const & rti1516Tag);

  virtual void
  requestAttributeValueUpdate(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                              rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                              rti1516e::VariableLengthData const & rti1516Tag);

  virtual void
  requestAttributeTransportationTypeChange(rti1516e::ObjectInstanceHandle theObject,
                                           rti1516e::AttributeHandleSet const & theAttributes,
                                           rti1516e::TransportationType theType);

  virtual void
  queryAttributeTransportationType(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandle theAttribute);

  virtual void
  requestInteractionTransportationTypeChange(rti1516e::InteractionClassHandle theClass, rti1516e::TransportationType theType);

    virtual void
    queryInteractionTransportationType(rti1516e::FederateHandle theFederate, rti1516e::InteractionClassHandle theInteraction);

  ///////////////////////////////////
  // Ownership Management Services //
  ///////////////////////////////////
  // 7.2
  virtual void
  unconditionalAttributeOwnershipDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                             rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet);

  // 7.3
  virtual void
  negotiatedAttributeOwnershipDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                          rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                          rti1516e::VariableLengthData const & rti1516Tag);

  // 7.6
  virtual void
  confirmDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                     rti1516e::AttributeHandleSet const& rti1516AttributeHandleSet,
                     rti1516e::VariableLengthData const& rti1516Tag);

  // 7.8
  virtual void
  attributeOwnershipAcquisition(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                rti1516e::VariableLengthData const & rti1516Tag);

  // 7.9
  virtual void
  attributeOwnershipAcquisitionIfAvailable(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                           rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet);

  virtual void
  attributeOwnershipReleaseDenied(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & theAttributes);

  // 7.12
  virtual void
  attributeOwnershipDivestitureIfWanted(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                        rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                        rti1516e::AttributeHandleSet & rti1516DivestedAttributeSet);

  // 7.13
  virtual void
  cancelNegotiatedAttributeOwnershipDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet);

  // 7.14
  virtual void
  cancelAttributeOwnershipAcquisition(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                      rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet);

  // 7.16
  virtual void
  queryAttributeOwnership(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                          rti1516e::AttributeHandle rti1516AttributeHandle);

  // 7.18
  virtual bool
  isAttributeOwnedByFederate(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                             rti1516e::AttributeHandle rti1516AttributeHandle);

  //////////////////////////////
  // Time Management Services //
  //////////////////////////////

  // 8.2
  virtual void
  enableTimeRegulation(const rti1516e::LogicalTimeInterval& rti1516Lookahead);

  // 8.4
  virtual void
  disableTimeRegulation();

  // 8.5
  virtual void
  enableTimeConstrained();

  // 8.7
  virtual void
  disableTimeConstrained();

  // 8.8
  virtual void
  timeAdvanceRequest(const rti1516e::LogicalTime& logicalTime);

  // 8.9
  virtual void
  timeAdvanceRequestAvailable(const rti1516e::LogicalTime& logicalTime);

  // 8.10
  virtual void
  nextMessageRequest(const rti1516e::LogicalTime& logicalTime);

  // 8.11
  virtual void
  nextMessageRequestAvailable(const rti1516e::LogicalTime& logicalTime);

  // 8.12
  virtual void
  flushQueueRequest(const rti1516e::LogicalTime& logicalTime);

  // 8.14
  virtual void
  enableAsynchronousDelivery();

  // 8.15
  virtual void
  disableAsynchronousDelivery();

  // 8.16
  virtual bool
  queryGALT(rti1516e::LogicalTime& logicalTime);

  // 8.17
  virtual void
  queryLogicalTime(rti1516e::LogicalTime& logicalTime);

  // 8.18
  virtual bool
  queryLITS(rti1516e::LogicalTime& logicalTime);

  // 8.19
  virtual void
  modifyLookahead(const rti1516e::LogicalTimeInterval& lookahead);

  // 8.20
  virtual void
  queryLookahead(rti1516e::LogicalTimeInterval& lookahead);

  // 8.21
  virtual void
  retract(rti1516e::MessageRetractionHandle rti1516MessageRetractionHandle);

  // 8.23
  virtual void
  changeAttributeOrderType(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                           rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                           rti1516e::OrderType rti1516OrderType);

  // 8.24
  virtual void
  changeInteractionOrderType(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                             rti1516e::OrderType rti1516OrderType);

  //////////////////////////////////
  // Data Distribution Management //
  //////////////////////////////////

  // 9.2
  virtual rti1516e::RegionHandle
  createRegion(rti1516e::DimensionHandleSet const & rti1516DimensionHandleSet);

  // 9.3
  virtual void
  commitRegionModifications(rti1516e::RegionHandleSet const & rti1516RegionHandleSet);

  // 9.4
  virtual void
  deleteRegion(const rti1516e::RegionHandle& rti1516RegionHandle);

  // 9.5
  virtual rti1516e::ObjectInstanceHandle
  registerObjectInstanceWithRegions(rti1516e::ObjectClassHandle theClass,
                                    rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                                    theAttributeHandleSetRegionHandleSetPairVector);

  virtual rti1516e::ObjectInstanceHandle
  registerObjectInstanceWithRegions(rti1516e::ObjectClassHandle theClass,
                                    rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                                    theAttributeHandleSetRegionHandleSetPairVector,
                                    std::wstring const & theObjectInstanceName);

  // 9.6
  virtual void
  associateRegionsForUpdates(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                             rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                             theAttributeHandleSetRegionHandleSetPairVector);

  // 9.7
  virtual void
  unassociateRegionsForUpdates(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                               rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                               theAttributeHandleSetRegionHandleSetPairVector);

  virtual void
  subscribeObjectClassAttributesWithRegions(rti1516e::ObjectClassHandle theClass,
                                            rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                                            theAttributeHandleSetRegionHandleSetPairVector,
                                            bool active, std::wstring const & updateRateDesignator);

  // 9.9
  virtual void
  unsubscribeObjectClassAttributesWithRegions(rti1516e::ObjectClassHandle theClass,
                                              rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                                              theAttributeHandleSetRegionHandleSetPairVector);

  // 9.10
  virtual void
  subscribeInteractionClassWithRegions(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                                       rti1516e::RegionHandleSet const & rti1516RegionHandleSet,
                                       bool active);

  // 9.11
  virtual void
  unsubscribeInteractionClassWithRegions(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                                         rti1516e::RegionHandleSet const & rti1516RegionHandleSet);

  // 9.12
  virtual void
  sendInteractionWithRegions(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                             rti1516e::ParameterHandleValueMap const & rti1516ParameterHandleValueMap,
                             rti1516e::RegionHandleSet const & rti1516RegionHandleSet,
                             rti1516e::VariableLengthData const & rti1516Tag);

  virtual rti1516e::MessageRetractionHandle
  sendInteractionWithRegions(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                             rti1516e::ParameterHandleValueMap const & rti1516ParameterHandleValueMap,
                             rti1516e::RegionHandleSet const & rti1516RegionHandleSet,
                             rti1516e::VariableLengthData const & rti1516Tag,
                             rti1516e::LogicalTime const & rti1516LogicalTime);

  // 9.13
  virtual void
  requestAttributeValueUpdateWithRegions(rti1516e::ObjectClassHandle theClass,
                                         rti1516e::AttributeHandleSetRegionHandleSetPairVector const & theSet,
                                         rti1516e::VariableLengthData const & rti1516Tag);

  //////////////////////////
  // RTI Support Services //
  //////////////////////////

  virtual rti1516e::ResignAction
  getAutomaticResignDirective();

  virtual void
  setAutomaticResignDirective(rti1516e::ResignAction resignAction);

  virtual rti1516e::FederateHandle getFederateHandle(std::wstring const & theName);

  virtual std::wstring getFederateName(rti1516e::FederateHandle theHandle);

  // 10.2
  virtual rti1516e::ObjectClassHandle
  getObjectClassHandle(std::wstring const & name);

  // 10.3
  virtual std::wstring
  getObjectClassName(rti1516e::ObjectClassHandle rti1516ObjectClassHandle);

  // 10.4
  virtual rti1516e::AttributeHandle
  getAttributeHandle(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                     std::wstring const & attributeName);

  // 10.5
  virtual std::wstring
  getAttributeName(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                   rti1516e::AttributeHandle rti1516AttributeHandle);

  // 10.13
  virtual double
  getUpdateRateValue(std::wstring const & updateRateDesignator);

  // 10.14
  virtual double
  getUpdateRateValueForAttribute(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandle theAttribute);

  // 10.6
  virtual rti1516e::InteractionClassHandle
  getInteractionClassHandle(std::wstring const & name);

  // 10.7
  virtual std::wstring
  getInteractionClassName(rti1516e::InteractionClassHandle rti1516InteractionClassHandle);

  // 10.8
  virtual rti1516e::ParameterHandle
  getParameterHandle(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                     std::wstring const & parameterName);

  // 10.9
  virtual std::wstring
  getParameterName(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                   rti1516e::ParameterHandle rti1516ParameterHandle);

  // 10.10
  virtual rti1516e::ObjectInstanceHandle
  getObjectInstanceHandle(std::wstring const & name);

  // 10.11
  virtual std::wstring
  getObjectInstanceName(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle);

  // 10.12
  virtual rti1516e::DimensionHandle
  getDimensionHandle(std::wstring const & name);

  // 10.13
  virtual std::wstring
  getDimensionName(rti1516e::DimensionHandle rti1516DimensionHandle);

  // 10.14
  virtual unsigned long
  getDimensionUpperBound(rti1516e::DimensionHandle rti1516DimensionHandle);

  // 10.15
  virtual rti1516e::DimensionHandleSet
  getAvailableDimensionsForClassAttribute(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                          rti1516e::AttributeHandle rti1516AttributeHandle);

  // 10.16
  virtual rti1516e::ObjectClassHandle
  getKnownObjectClassHandle(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle);

  // 10.17
  virtual rti1516e::DimensionHandleSet
  getAvailableDimensionsForInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle);

  // 10.18
  virtual rti1516e::TransportationType
  getTransportationType(std::wstring const & transportationName);

  // 10.19
  virtual std::wstring
  getTransportationName(rti1516e::TransportationType transportationType);

  // 10.20
  virtual rti1516e::OrderType
  getOrderType(std::wstring const & orderName);

  // 10.21
  virtual std::wstring
  getOrderName(rti1516e::OrderType orderType);

  // 10.22
  virtual void enableObjectClassRelevanceAdvisorySwitch();

  // 10.23
  virtual void disableObjectClassRelevanceAdvisorySwitch();

  // 10.24
  virtual void enableAttributeRelevanceAdvisorySwitch ();

  // 10.25
  virtual void disableAttributeRelevanceAdvisorySwitch ();

  // 10.26
  virtual void enableAttributeScopeAdvisorySwitch ();

  // 10.27
  virtual void disableAttributeScopeAdvisorySwitch ();

  // 10.28
  virtual void enableInteractionRelevanceAdvisorySwitch ();

  // 10.29
  virtual void disableInteractionRelevanceAdvisorySwitch ();

  // 10.30
  virtual
  rti1516e::DimensionHandleSet getDimensionHandleSet(rti1516e::RegionHandle rti1516RegionHandle);

  // 10.31
  virtual rti1516e::RangeBounds
  getRangeBounds(rti1516e::RegionHandle theRegionHandle,
                 rti1516e::DimensionHandle theDimensionHandle);

  // 10.32
  virtual void
  setRangeBounds(rti1516e::RegionHandle theRegionHandle,
                 rti1516e::DimensionHandle theDimensionHandle,
                 rti1516e::RangeBounds const & theRangeBounds);

  // 10.33
  virtual unsigned long normalizeFederateHandle(rti1516e::FederateHandle rti1516FederateHandle);

  // 10.34
  virtual unsigned long normalizeServiceGroup(rti1516e::ServiceGroup rti1516ServiceGroup);

  // 10.37
  virtual bool evokeCallback(double approximateMinimumTimeInSeconds);

  // 10.38
  virtual bool evokeMultipleCallbacks(double approximateMinimumTimeInSeconds,
                                      double approximateMaximumTimeInSeconds);

  // 10.39
  virtual void enableCallbacks ();

  // 10.40
  virtual void disableCallbacks ();

  // Return instance of time factory being used by the federation
  virtual std::unique_ptr<rti1516e::LogicalTimeFactory> getTimeFactory() const;

  // Decode handles
  virtual rti1516e::FederateHandle decodeFederateHandle(rti1516e::VariableLengthData const & encodedValue) const;

  virtual rti1516e::ObjectClassHandle decodeObjectClassHandle(rti1516e::VariableLengthData const & encodedValue) const;

  virtual rti1516e::InteractionClassHandle decodeInteractionClassHandle(rti1516e::VariableLengthData const & encodedValue) const;

  virtual rti1516e::ObjectInstanceHandle decodeObjectInstanceHandle(rti1516e::VariableLengthData const & encodedValue) const;

  virtual rti1516e::AttributeHandle decodeAttributeHandle(rti1516e::VariableLengthData const & encodedValue) const;

  virtual rti1516e::ParameterHandle decodeParameterHandle(rti1516e::VariableLengthData const & encodedValue) const;

  virtual rti1516e::DimensionHandle decodeDimensionHandle(rti1516e::VariableLengthData const & encodedValue) const;

  virtual rti1516e::MessageRetractionHandle decodeMessageRetractionHandle(rti1516e::VariableLengthData const & encodedValue) const;

  virtual rti1516e::RegionHandle decodeRegionHandle(rti1516e::VariableLengthData const & encodedValue) const;

  // Vector extensions
  void setNotificationHandle(rti1516e::RTInotificationHandle* h) override;

  void StartNetworkStatistics(bool startTimer) override;
  void StopNetworkStatistics() override;
  void ResetNetworkStatistics() override;

  void unsubscribeObjectInstance(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle) override;
  void subscribeInteractionClassWithFilter(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                                           rti1516e::ParameterHandleValueMap filterValues, 
                                           bool active) override;

  void setInteractionClassDeliverToSelf(rti1516e::InteractionClassHandle interactionClassHandle, bool enable) override;
  void setObjectClassDeliverToSelf(rti1516e::ObjectClassHandle objectClassHandle, bool enable) override;

private:
  class RTI1516EAmbassadorInterface;
  RTI1516EAmbassadorInterface* _ambassadorInterface;
};

}

#endif
