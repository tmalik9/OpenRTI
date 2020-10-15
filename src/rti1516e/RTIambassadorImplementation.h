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

// This implementation class just acts as a converter between RTI1516E types and OpenRTI internal types.
// It's only member is the RTI1516EAmbassadorInterface, which is a subclass of the actual implementation
// OpenRTI::Ambassador<RTI1516ETraits>. Unlucky naming, this class would've better been named
// RTI1516EAmbassadorInterface, while the ambassadorInterface member's class should have been named
// RTI1516EAmbassadorImplementation (and the member ambassadorImplementation, because it is just this).
// The implementation class also contains the application-provided _federateAmbassador.
class OPENRTI_LOCAL RTIambassadorImplementation final : public rti1516e::RTIambassador {
public:
  RTIambassadorImplementation();
  virtual ~RTIambassadorImplementation();

  void connect(rti1516e::FederateAmbassador & federateAmbassador, rti1516e::CallbackModel theCallbackModel,
          std::wstring const & localSettingsDesignator) override;

  void disconnect() override;

  void shutdown() override;

  void createFederationExecution(std::wstring const & federationExecutionName,
                            std::wstring const & fomModule,
                            std::wstring const & logicalTimeImplementationName) override;

  void createFederationExecution(std::wstring const & federationExecutionName,
                            std::vector<std::wstring> const & fomModules,
                            std::wstring const & logicalTimeImplementationName) override;

  void createFederationExecutionWithMIM (std::wstring const & federationExecutionName,
                             std::vector<std::wstring> const & fomModules,
                             std::wstring const & mimModule,
                             std::wstring const & logicalTimeImplementationName) override;

  void destroyFederationExecution(std::wstring const & federationExecutionName) override;

  void listFederationExecutions() override;

  rti1516e::FederateHandle
  joinFederationExecution(std::wstring const & federateType,
                          std::wstring const & federationExecutionName,
                          std::vector<std::wstring> const & additionalFomModules) override;

  rti1516e::FederateHandle
  joinFederationExecution(std::wstring const & federateName,
                          std::wstring const & federateType,
                          std::wstring const & federationExecutionName,
                          std::vector<std::wstring> const & additionalFomModules) override;

  // 4.5
  void resignFederationExecution(rti1516e::ResignAction rti1516ResignAction) override;

  // 4.6
  void registerFederationSynchronizationPoint(std::wstring const & label,
                                         rti1516e::VariableLengthData const & rti1516Tag) override;

  void registerFederationSynchronizationPoint(std::wstring const & label,
                                         rti1516e::VariableLengthData const & rti1516Tag,
                                         rti1516e::FederateHandleSet const & rti1516FederateHandleSet) override;

  // 4.9
  void   synchronizationPointAchieved(std::wstring const & label, bool) override;

  // 4.11
  void requestFederationSave(std::wstring const & label) override;

  void requestFederationSave(const std::wstring& label,
                        const rti1516e::LogicalTime& rti1516LogicalTime) override;

  // 4.13
  void federateSaveBegun() override;

  // 4.14
  void federateSaveComplete() override;

  void federateSaveNotComplete() override;

  void abortFederationSave() override;

  // 4.16
  void queryFederationSaveStatus () override;

  // 4.18
  void requestFederationRestore(std::wstring const & label) override;

  // 4.22
  void federateRestoreComplete() override;

  void federateRestoreNotComplete() override;

  void abortFederationRestore () override;

  // 4.24
  void queryFederationRestoreStatus() override;

  /////////////////////////////////////
  // Declaration Management Services //
  /////////////////////////////////////

  // 5.2
  void publishObjectClassAttributes(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                               rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet) override;

  // 5.3
  void unpublishObjectClass(rti1516e::ObjectClassHandle rti1516ObjectClassHandle) override;

  void unpublishObjectClassAttributes(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                 rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet) override;

  // 5.4
  void publishInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle) override;

  // 5.5
  void unpublishInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle) override;

  // 5.6
  void subscribeObjectClassAttributes(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                 rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                 bool active, std::wstring const & updateRateDesignator) override;

  // 5.7
  void unsubscribeObjectClass(rti1516e::ObjectClassHandle rti1516ObjectClassHandle) override;

  void unsubscribeObjectClassAttributes(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                   rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet) override;

  // 5.8
  void subscribeInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                            bool active) override;

  // 5.9
  void unsubscribeInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle) override;

  ////////////////////////////////
  // Object Management Services //
  ////////////////////////////////

  void reserveObjectInstanceName(std::wstring const & objectInstanceName) override;

  void releaseObjectInstanceName(std::wstring const & objectInstanceName) override;

  void reserveMultipleObjectInstanceName(std::set<std::wstring> const & theObjectInstanceNames) override;

  void releaseMultipleObjectInstanceName(std::set<std::wstring> const & theObjectInstanceNames) override;

  // 6.4
  rti1516e::ObjectInstanceHandle
  registerObjectInstance(rti1516e::ObjectClassHandle rti1516ObjectClassHandle) override;

  rti1516e::ObjectInstanceHandle
  registerObjectInstance(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                         std::wstring const & objectInstanceName) override;

  rti1516e::ObjectInstanceHandle
  registerObjectInstance(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                   std::wstring const & objectInstanceName, bool allowUnreservedObjectNames) override;
  // 6.6
  void updateAttributeValues(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                        const rti1516e::AttributeHandleValueMap& rti1516AttributeValues,
                        const rti1516e::VariableLengthData& rti1516Tag) override;

  virtual rti1516e::MessageRetractionHandle
  updateAttributeValues(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                        const rti1516e::AttributeHandleValueMap& rti1516AttributeValues,
                        const rti1516e::VariableLengthData& rti1516Tag,
                        const rti1516e::LogicalTime& rti1516LogicalTime) override;

  // 6.8
  void sendInteraction(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                  const rti1516e::ParameterHandleValueMap& rti1516ParameterValues,
                  const rti1516e::VariableLengthData& rti1516Tag) override;

  virtual rti1516e::MessageRetractionHandle
  sendInteraction(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                  const rti1516e::ParameterHandleValueMap& rti1516ParameterValues,
                  const rti1516e::VariableLengthData& rti1516Tag,
                  const rti1516e::LogicalTime& rti1516LogicalTime) override;

  // 6.10
  void deleteObjectInstance(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                       const rti1516e::VariableLengthData& rti1516Tag) override;

  virtual rti1516e::MessageRetractionHandle
  deleteObjectInstance(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                       const rti1516e::VariableLengthData& rti1516Tag,
                       const rti1516e::LogicalTime& rti1516LogicalTime) override;

  // 6.12
  void localDeleteObjectInstance(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle) override;

  //// 6.13
  //void //changeAttributeTransportationType(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
  //                                  rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
  //                                  rti1516e::TransportationType rti1516TransportationType) override;

  //// 6.14
  //void //changeInteractionTransportationType(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
  //                                    rti1516e::TransportationType rti1516TransportationType) override;

  // 6.17
  void requestAttributeValueUpdate(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                              rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                              rti1516e::VariableLengthData const & rti1516Tag) override;

  void requestAttributeValueUpdate(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                              rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                              rti1516e::VariableLengthData const & rti1516Tag) override;

  void requestAttributeTransportationTypeChange(rti1516e::ObjectInstanceHandle theObject,
                                           rti1516e::AttributeHandleSet const & theAttributes,
                                           rti1516e::TransportationType theType) override;

  void queryAttributeTransportationType(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandle theAttribute) override;

  void requestInteractionTransportationTypeChange(rti1516e::InteractionClassHandle theClass, rti1516e::TransportationType theType) override;

  void queryInteractionTransportationType(rti1516e::FederateHandle theFederate, rti1516e::InteractionClassHandle theInteraction) override;

  ///////////////////////////////////
  // Ownership Management Services //
  ///////////////////////////////////
  // 7.2
  void unconditionalAttributeOwnershipDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                             rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet) override;

  // 7.3
  void negotiatedAttributeOwnershipDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                          rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                          rti1516e::VariableLengthData const & rti1516Tag) override;

  // 7.6
  void confirmDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                     rti1516e::AttributeHandleSet const& rti1516AttributeHandleSet,
                     rti1516e::VariableLengthData const& rti1516Tag) override;

  // 7.8
  void attributeOwnershipAcquisition(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                rti1516e::VariableLengthData const & rti1516Tag) override;

  // 7.9
  void attributeOwnershipAcquisitionIfAvailable(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                           rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet) override;

  void attributeOwnershipReleaseDenied(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandleSet const & theAttributes) override;

  // 7.12
  void attributeOwnershipDivestitureIfWanted(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                        rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                        rti1516e::AttributeHandleSet & rti1516DivestedAttributeSet) override;

  // 7.13
  void cancelNegotiatedAttributeOwnershipDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet) override;

  // 7.14
  void cancelAttributeOwnershipAcquisition(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                      rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet) override;

  // 7.16
  void queryAttributeOwnership(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                          rti1516e::AttributeHandle rti1516AttributeHandle) override;

  // 7.18
  virtual bool
  isAttributeOwnedByFederate(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                             rti1516e::AttributeHandle rti1516AttributeHandle) override;

  //////////////////////////////
  // Time Management Services //
  //////////////////////////////

  // 8.2
  void enableTimeRegulation(const rti1516e::LogicalTimeInterval& rti1516Lookahead) override;

  // 8.4
  void disableTimeRegulation() override;

  // 8.5
  void enableTimeConstrained() override;

  // 8.7
  void disableTimeConstrained() override;

  // 8.8
  void timeAdvanceRequest(const rti1516e::LogicalTime& logicalTime) override;

  // 8.9
  void timeAdvanceRequestAvailable(const rti1516e::LogicalTime& logicalTime) override;

  // 8.10
  void nextMessageRequest(const rti1516e::LogicalTime& logicalTime) override;

  // 8.11
  void nextMessageRequestAvailable(const rti1516e::LogicalTime& logicalTime) override;

  // 8.12
  void flushQueueRequest(const rti1516e::LogicalTime& logicalTime) override;

  // 8.14
  void enableAsynchronousDelivery() override;

  // 8.15
  void disableAsynchronousDelivery() override;

  // 8.16
  virtual bool
  queryGALT(rti1516e::LogicalTime& logicalTime) override;

  // 8.17
  void queryLogicalTime(rti1516e::LogicalTime& logicalTime) override;

  // 8.18
  virtual bool
  queryLITS(rti1516e::LogicalTime& logicalTime) override;

  // 8.19
  void modifyLookahead(const rti1516e::LogicalTimeInterval& lookahead) override;

  // 8.20
  void queryLookahead(rti1516e::LogicalTimeInterval& lookahead) override;

  // 8.21
  void retract(rti1516e::MessageRetractionHandle rti1516MessageRetractionHandle) override;

  // 8.23
  void changeAttributeOrderType(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                           rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                           rti1516e::OrderType rti1516OrderType) override;

  // 8.24
  void changeInteractionOrderType(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                             rti1516e::OrderType rti1516OrderType) override;

  //////////////////////////////////
  // Data Distribution Management //
  //////////////////////////////////

  // 9.2
  virtual rti1516e::RegionHandle
  createRegion(rti1516e::DimensionHandleSet const & rti1516DimensionHandleSet) override;

  // 9.3
  void commitRegionModifications(rti1516e::RegionHandleSet const & rti1516RegionHandleSet) override;

  // 9.4
  void deleteRegion(const rti1516e::RegionHandle& rti1516RegionHandle) override;

  // 9.5
  virtual rti1516e::ObjectInstanceHandle
  registerObjectInstanceWithRegions(rti1516e::ObjectClassHandle theClass,
                                    rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                                    theAttributeHandleSetRegionHandleSetPairVector) override;

  virtual rti1516e::ObjectInstanceHandle
  registerObjectInstanceWithRegions(rti1516e::ObjectClassHandle theClass,
                                    rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                                    theAttributeHandleSetRegionHandleSetPairVector,
                                    std::wstring const & theObjectInstanceName) override;

  // 9.6
  void associateRegionsForUpdates(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                             rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                             theAttributeHandleSetRegionHandleSetPairVector) override;

  // 9.7
  void unassociateRegionsForUpdates(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                               rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                               theAttributeHandleSetRegionHandleSetPairVector) override;

  void subscribeObjectClassAttributesWithRegions(rti1516e::ObjectClassHandle theClass,
                                            rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                                            theAttributeHandleSetRegionHandleSetPairVector,
                                            bool active, std::wstring const & updateRateDesignator) override;

  // 9.9
  void unsubscribeObjectClassAttributesWithRegions(rti1516e::ObjectClassHandle theClass,
                                              rti1516e::AttributeHandleSetRegionHandleSetPairVector const &
                                              theAttributeHandleSetRegionHandleSetPairVector) override;

  // 9.10
  void subscribeInteractionClassWithRegions(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                                       rti1516e::RegionHandleSet const & rti1516RegionHandleSet,
                                       bool active) override;

  // 9.11
  void unsubscribeInteractionClassWithRegions(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                                         rti1516e::RegionHandleSet const & rti1516RegionHandleSet) override;

  // 9.12
  void sendInteractionWithRegions(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                             rti1516e::ParameterHandleValueMap const & rti1516ParameterHandleValueMap,
                             rti1516e::RegionHandleSet const & rti1516RegionHandleSet,
                             rti1516e::VariableLengthData const & rti1516Tag) override;

  virtual rti1516e::MessageRetractionHandle
  sendInteractionWithRegions(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                             rti1516e::ParameterHandleValueMap const & rti1516ParameterHandleValueMap,
                             rti1516e::RegionHandleSet const & rti1516RegionHandleSet,
                             rti1516e::VariableLengthData const & rti1516Tag,
                             rti1516e::LogicalTime const & rti1516LogicalTime) override;

  // 9.13
  void requestAttributeValueUpdateWithRegions(rti1516e::ObjectClassHandle theClass,
                                         rti1516e::AttributeHandleSetRegionHandleSetPairVector const & theSet,
                                         rti1516e::VariableLengthData const & rti1516Tag) override;

  //////////////////////////
  // RTI Support Services //
  //////////////////////////

  virtual rti1516e::ResignAction
  getAutomaticResignDirective() override;

  void setAutomaticResignDirective(rti1516e::ResignAction resignAction) override;

  virtual rti1516e::FederateHandle getFederateHandle(std::wstring const & theName) override;

  virtual std::wstring getFederateName(rti1516e::FederateHandle theHandle) override;

  // 10.2
  virtual rti1516e::ObjectClassHandle
  getObjectClassHandle(std::wstring const & name) override;

  // 10.3
  virtual std::wstring
  getObjectClassName(rti1516e::ObjectClassHandle rti1516ObjectClassHandle) override;

  // 10.4
  virtual rti1516e::AttributeHandle
  getAttributeHandle(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                     std::wstring const & attributeName) override;

  // 10.5
  virtual std::wstring
  getAttributeName(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                   rti1516e::AttributeHandle rti1516AttributeHandle) override;

  // 10.13
  virtual double
  getUpdateRateValue(std::wstring const & updateRateDesignator) override;

  // 10.14
  virtual double
  getUpdateRateValueForAttribute(rti1516e::ObjectInstanceHandle theObject, rti1516e::AttributeHandle theAttribute) override;

  // 10.6
  virtual rti1516e::InteractionClassHandle
  getInteractionClassHandle(std::wstring const & name) override;

  // 10.7
  virtual std::wstring
  getInteractionClassName(rti1516e::InteractionClassHandle rti1516InteractionClassHandle) override;

  // 10.8
  virtual rti1516e::ParameterHandle
  getParameterHandle(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                     std::wstring const & parameterName) override;

  // 10.9
  virtual std::wstring
  getParameterName(rti1516e::InteractionClassHandle rti1516InteractionClassHandle,
                   rti1516e::ParameterHandle rti1516ParameterHandle) override;

  // 10.10
  virtual rti1516e::ObjectInstanceHandle
  getObjectInstanceHandle(std::wstring const & name) override;

  // 10.11
  virtual std::wstring
  getObjectInstanceName(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle) override;

  // 10.12
  virtual rti1516e::DimensionHandle
  getDimensionHandle(std::wstring const & name) override;

  // 10.13
  virtual std::wstring
  getDimensionName(rti1516e::DimensionHandle rti1516DimensionHandle) override;

  // 10.14
  virtual unsigned long
  getDimensionUpperBound(rti1516e::DimensionHandle rti1516DimensionHandle) override;

  // 10.15
  virtual rti1516e::DimensionHandleSet
  getAvailableDimensionsForClassAttribute(rti1516e::ObjectClassHandle rti1516ObjectClassHandle,
                                          rti1516e::AttributeHandle rti1516AttributeHandle) override;

  // 10.16
  virtual rti1516e::ObjectClassHandle
  getKnownObjectClassHandle(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle) override;

  // 10.17
  virtual rti1516e::DimensionHandleSet
  getAvailableDimensionsForInteractionClass(rti1516e::InteractionClassHandle rti1516InteractionClassHandle) override;

  // 10.18
  virtual rti1516e::TransportationType
  getTransportationType(std::wstring const & transportationName) override;

  // 10.19
  virtual std::wstring
  getTransportationName(rti1516e::TransportationType transportationType) override;

  // 10.20
  virtual rti1516e::OrderType
  getOrderType(std::wstring const & orderName) override;

  // 10.21
  virtual std::wstring
  getOrderName(rti1516e::OrderType orderType) override;

  // 10.22
  virtual void enableObjectClassRelevanceAdvisorySwitch() override;

  // 10.23
  virtual void disableObjectClassRelevanceAdvisorySwitch() override;

  // 10.24
  virtual void enableAttributeRelevanceAdvisorySwitch () override;

  // 10.25
  virtual void disableAttributeRelevanceAdvisorySwitch () override;

  // 10.26
  virtual void enableAttributeScopeAdvisorySwitch () override;

  // 10.27
  virtual void disableAttributeScopeAdvisorySwitch () override;

  // 10.28
  virtual void enableInteractionRelevanceAdvisorySwitch () override;

  // 10.29
  virtual void disableInteractionRelevanceAdvisorySwitch () override;

  // 10.30
  virtual
  rti1516e::DimensionHandleSet getDimensionHandleSet(rti1516e::RegionHandle rti1516RegionHandle) override;

  // 10.31
  virtual rti1516e::RangeBounds
  getRangeBounds(rti1516e::RegionHandle theRegionHandle,
                 rti1516e::DimensionHandle theDimensionHandle) override;

  // 10.32
  void setRangeBounds(rti1516e::RegionHandle theRegionHandle,
                 rti1516e::DimensionHandle theDimensionHandle,
                 rti1516e::RangeBounds const & theRangeBounds) override;

  // 10.33
  virtual unsigned long normalizeFederateHandle(rti1516e::FederateHandle rti1516FederateHandle) override;

  // 10.34
  virtual unsigned long normalizeServiceGroup(rti1516e::ServiceGroup rti1516ServiceGroup) override;

  // 10.37
  virtual bool evokeCallback(double approximateMinimumTimeInSeconds) override;

  // 10.38
  virtual bool evokeMultipleCallbacks(double approximateMinimumTimeInSeconds,
                                      double approximateMaximumTimeInSeconds) override;

  // 10.39
  virtual void enableCallbacks () override;

  // 10.40
  virtual void disableCallbacks () override;

  // Return instance of time factory being used by the federation
  std::unique_ptr<rti1516e::LogicalTimeFactory> getTimeFactory() const override;

  // Decode handles
  virtual rti1516e::FederateHandle decodeFederateHandle(rti1516e::VariableLengthData const & encodedValue) const override;

  virtual rti1516e::ObjectClassHandle decodeObjectClassHandle(rti1516e::VariableLengthData const & encodedValue) const override;

  virtual rti1516e::InteractionClassHandle decodeInteractionClassHandle(rti1516e::VariableLengthData const & encodedValue) const override;

  virtual rti1516e::ObjectInstanceHandle decodeObjectInstanceHandle(rti1516e::VariableLengthData const & encodedValue) const override;

  virtual rti1516e::AttributeHandle decodeAttributeHandle(rti1516e::VariableLengthData const & encodedValue) const override;

  virtual rti1516e::ParameterHandle decodeParameterHandle(rti1516e::VariableLengthData const & encodedValue) const override;

  virtual rti1516e::DimensionHandle decodeDimensionHandle(rti1516e::VariableLengthData const & encodedValue) const override;

  virtual rti1516e::MessageRetractionHandle decodeMessageRetractionHandle(rti1516e::VariableLengthData const & encodedValue) const override;

  virtual rti1516e::RegionHandle decodeRegionHandle(rti1516e::VariableLengthData const & encodedValue) const override;

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
  void setConnectWaitTimeout(uint32_t timeoutMilliSeconds) override;
  void setOperationWaitTimeout(uint32_t timeoutMilliSeconds) override;
private:
  class RTI1516EAmbassadorInterface;
  RTI1516EAmbassadorInterface* _ambassadorInterface;
};

}

#endif
