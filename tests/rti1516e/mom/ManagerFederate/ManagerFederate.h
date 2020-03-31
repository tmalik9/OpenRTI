
#pragma once

#include "Windows.h"
#include "RTI/RTIambassador.h"
#include "RTI/NullFederateAmbassador.h"
#include "Mom.h"

using namespace rti1516e;

/**
 * Example Federate implementation
 */
class ManagerFederate : public NullFederateAmbassador
{
    struct VRTNotificationHandle : public rti1516e::RTInotificationHandle
    {
      VRTNotificationHandle()
      {
        mHandle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
      }
      ~VRTNotificationHandle()
      {
        CloseHandle(mHandle);
      }
      virtual void Signal() override
      {
        ::SetEvent(mHandle);
      }
      VRTNotificationHandle(const VRTNotificationHandle&) = delete;
      void operator=(const VRTNotificationHandle&) = delete;
      HANDLE mHandle;
    };

  public:

    // public methods //
    //! Constructor
    ManagerFederate();
    //! Destructor
    virtual ~ManagerFederate();
    //! Main Simulation Method
    void runFederate();

    void initialize(std::string address, std::string fom);
    void initializeHandles();
    void waitForUser();
    double getFederateTime() const;
    double getLbts() const;

    double federateTime;
    double federateLookahead;

    bool isRegulating;
    bool isConstrained;
    bool isAdvancing;
    bool federationsReported = false;

    // methods //
    // implementation of RTI::FederateAmbassador
    virtual void connectionLost (
        std::wstring const & faultDescription) override;

    // 4.8
    virtual void reportFederationExecutions (
        FederationExecutionInformationVector const &
        theFederationExecutionInformationList) override;

    ///////////////////////////////////
    // synchronization point methods //
    ///////////////////////////////////
    virtual void synchronizationPointRegistrationSucceeded(std::wstring const& label) override;

    virtual void synchronizationPointRegistrationFailed(std::wstring const& label,  SynchronizationPointFailureReason reason) override;

    virtual void federationSynchronized(
      std::wstring const& label,
      FederateHandleSet const& failedToSyncSet) override;


    //////////////////////////
    // time related methods //
    //////////////////////////
    virtual void timeRegulationEnabled(LogicalTime const& theFederateTime) override;

    virtual void timeConstrainedEnabled(LogicalTime const& theFederateTime) override;

    virtual void timeAdvanceGrant(LogicalTime const& theTime) override;

    virtual void reflectAttributeValues(
      ObjectInstanceHandle theObject,
      AttributeHandleValueMap const& theAttributeValues,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      TransportationType theType,
      SupplementalReflectInfo theReflectInfo) override;

    virtual void reflectAttributeValues(
      ObjectInstanceHandle theObject,
      AttributeHandleValueMap const& theAttributeValues,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      TransportationType theType,
      LogicalTime const& theTime,
      OrderType receivedOrder,
      SupplementalReflectInfo theReflectInfo) override;

    virtual void receiveInteraction(
      InteractionClassHandle theInteraction,
      ParameterHandleValueMap const& theParameterValues,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      TransportationType theType,
      SupplementalReceiveInfo theReceiveInfo) override;

    virtual void receiveInteraction(
      InteractionClassHandle theInteraction,
      ParameterHandleValueMap const& theParameterValues,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      TransportationType theType,
      LogicalTime const& theTime,
      OrderType receivedOrder,
      SupplementalReceiveInfo theReceiveInfo) override;

    virtual void receiveInteraction(
      InteractionClassHandle theInteraction,
      ParameterHandleValueMap const& theParameterValues,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      TransportationType theType,
      LogicalTime const& theTime,
      OrderType receivedOrder,
      MessageRetractionHandle theHandle,
      SupplementalReceiveInfo theReceiveInfo) override;

    ///////////////////////////////
    // object management methods //
    ///////////////////////////////
    void discoverObjectInstance (
      ObjectInstanceHandle theObject,
      ObjectClassHandle theObjectClass,
      std::wstring const& theObjectInstanceName) override;

    void discoverObjectInstance (
      ObjectInstanceHandle theObject,
      ObjectClassHandle theObjectClass,
      std::wstring const& theObjectInstanceName,
      FederateHandle producingFederate) override;

    virtual void attributesInScope(ObjectInstanceHandle, const AttributeHandleSet&) override;

    virtual void attributesOutOfScope(ObjectInstanceHandle, const AttributeHandleSet&) override;

    virtual void provideAttributeValueUpdate(
      ObjectInstanceHandle theObject,
      AttributeHandleSet const& theAttributes,
      VariableLengthData const& theUserSuppliedTag) override;


    virtual void removeObjectInstance(
      ObjectInstanceHandle theObject,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      SupplementalRemoveInfo theRemoveInfo) override;

    virtual void removeObjectInstance(
      ObjectInstanceHandle theObject,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      LogicalTime const& theTime,
      OrderType receivedOrder,
      SupplementalRemoveInfo theRemoveInfo) override;

    virtual void removeObjectInstance(
      ObjectInstanceHandle theObject,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      LogicalTime const& theTime,
      OrderType receivedOrder,
      MessageRetractionHandle theHandle,
      SupplementalRemoveInfo theRemoveInfo) override;

  private:
    std::unique_ptr<RTIambassador> mRtiAmb; /*!< RTI ambassador to comunicate with RTI */
    std::unique_ptr<VRTNotificationHandle> mHandle;


  private:
    //ObjectInstanceHandle registerObject(const wchar_t* className);
    void deleteObject(ObjectInstanceHandle objectHandle);

  private:
    Mom* mom;
    ObjectClassHandle mFederationClassHandle;
    AttributeHandle mFederationNameAttributeHandle;
    AttributeHandle mFederatesInFederationAttributeHandle;

    ObjectClassHandle mFederateClassHandle;
    AttributeHandle mFederateNameAttributeHandle;
    AttributeHandle mFederateHostAttributeHandle;
    AttributeHandle mFederateTimeConstrainedAttributeHandle;
    AttributeHandle mFederateTimeRegulatingAttributeHandle;
    AttributeHandle mFederateReflectionsReceivedAttributeHandle;
    AttributeHandle mFederateUpdatesSentAttributeHandle;
    AttributeHandle mFederateInteractionsReceivedAttributeHandle;
    AttributeHandle mFederateInteractionsSentAttributeHandle;
    AttributeHandle mFederateObjectInstancesRegisteredAttributeHandle;
    AttributeHandle mFederateTimeGrantedTimeAttributeHandle;
    AttributeHandle mFederateTimeAdvancingTimeAttributeHandle;
};

