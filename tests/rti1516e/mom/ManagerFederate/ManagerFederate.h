
#pragma once

//#include "Windows.h"
#include "RTI/RTIambassador.h"
#include "RTI/NullFederateAmbassador.h"
#include "Mom.h"
#include <atomic>

using namespace rti1516e;

/**
 * Example Federate implementation
 */
class ManagerFederate : public NullFederateAmbassador
{
  /*
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
    */
  public:

    // public methods //
    //! Constructor
    ManagerFederate();
    //! Destructor
    virtual ~ManagerFederate();
    //! Main Simulation Method
    void runFederate();

    void initialize(const std::string& address, const std::string& federationName, const std::string& timeImplName);

    bool federationsReported = false;

    // methods //
    // implementation of RTI::FederateAmbassador
    virtual void connectionLost(std::wstring const& faultDescription) override;

    // 4.8
    virtual void reportFederationExecutions(FederationExecutionInformationVector const& theFederationExecutionInformationList) override;

    ///////////////////////////////////
    // synchronization point methods //
    ///////////////////////////////////
    virtual void synchronizationPointRegistrationSucceeded(std::wstring const& label) override;

    virtual void synchronizationPointRegistrationFailed(std::wstring const& label,  SynchronizationPointFailureReason reason) override;

    void announceSynchronizationPoint(std::wstring const& label, VariableLengthData const& theUserSuppliedTag);
    virtual void federationSynchronized(
      std::wstring const& label,
      FederateHandleSet const& failedToSyncSet) override;


    virtual void reflectAttributeValues(
      ObjectInstanceHandle theObject,
      AttributeHandleValueMap const& theAttributeValues,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      TransportationType theType,
      SupplementalReflectInfo theReflectInfo) override;

    void decodeInteractionCounts(const VariableLengthData& value);
    void decodeObjectCounts(const VariableLengthData& value);
    void decodeFederateHandle(const VariableLengthData& value);
    void decodeCount(const VariableLengthData& value);
    void decodeInteractionHandleList(const VariableLengthData& value);
    void decodeInteractionSubscriptionList(const VariableLengthData& value);
    void decodeObjectClass(const VariableLengthData& value);
    void decodeAttributeList(const VariableLengthData& value);
    void decodeBoolean(const VariableLengthData& value);

    virtual void receiveInteraction(
      InteractionClassHandle theInteraction,
      ParameterHandleValueMap const& theParameterValues,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      TransportationType theType,
      SupplementalReceiveInfo theReceiveInfo) override;

    ///////////////////////////////
    // object management methods //
    ///////////////////////////////
    void discoverObjectInstance(
      ObjectInstanceHandle theObject,
      ObjectClassHandle theObjectClass,
      std::wstring const& theObjectInstanceName) override;

    void discoverObjectInstance(
      ObjectInstanceHandle theObject,
      ObjectClassHandle theObjectClass,
      std::wstring const& theObjectInstanceName,
      FederateHandle producingFederate) override;

    virtual void attributesInScope(ObjectInstanceHandle, const AttributeHandleSet&) override;

    virtual void attributesOutOfScope(ObjectInstanceHandle, const AttributeHandleSet&) override;

    virtual void removeObjectInstance(
      ObjectInstanceHandle theObject,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      SupplementalRemoveInfo theRemoveInfo) override;
    void setDone() { _done = true; }
  private:
    std::unique_ptr<RTIambassador> mRtiAmb; /*!< RTI ambassador to communicate with RTI */
    //std::unique_ptr<VRTNotificationHandle> mHandle;

    rti1516e::FederateHandle mFederateHandle;

    std::shared_ptr<Mom> mom;
    std::map<ParameterHandle, std::function<void(const VariableLengthData&)>> mDecoders;
    std::wstring mFederationName;
    std::atomic_bool _done = false;
};

