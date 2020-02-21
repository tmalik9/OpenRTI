
#pragma once

#include <RTI/NullFederateAmbassador.h>
#include <RTI/RTIambassador.h>
#include <windows.h>

using namespace rti1516e;

#define READY_TO_RUN L"ReadyToRun"
#define ALL_DONE L"AllDone"

class SimpleTestAmbassador;
class SimpleTestFederate;

double convertTime(LogicalTime const& theTime);
std::wstring convertStringToWstring(const std::string& str);
std::wstring variableLengthDataToWstring(const VariableLengthData& variableLengthData);
VariableLengthData toVariableLengthData(const wchar_t* s);


class SimpleTestFederate
{
    struct VRTNotificationHandle : public rti1516e::RTInotificationHandle
    {
      VRTNotificationHandle() {
        mHandle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
      }
      ~VRTNotificationHandle() {
        CloseHandle(mHandle);
      }
      virtual void Signal() override {
        ::SetEvent(mHandle);
      }
      VRTNotificationHandle(const VRTNotificationHandle&) = delete;
      void operator=(const VRTNotificationHandle&) = delete;
      HANDLE mHandle;
    };


  public:
    std::unique_ptr<RTIambassador> rtiamb; /*!< RTI ambassador to comunicate with RTI */
    std::unique_ptr<SimpleTestAmbassador> fedamb; /*!< Own Federate Ambassador implementation */

    // public methods //
    //! Constructor
    SimpleTestFederate();
    //! Destructor
    virtual ~SimpleTestFederate();
    void initialize(std::string address, std::string federateName, std::string fom, bool regulating, bool constrained);
    void initializeHandles();
    void waitForUser();
    void advanceTime(double timestep);
    void sendInteraction();
    double getFederateTime() const;
    double getLbts() const;

protected:
    /// fom handles
    std::map<std::string, ObjectClassHandle> mObjectClassHandles;
    std::map<std::string, std::map<std::string, AttributeHandle> > mAttributeHandles;
    //ObjectClassHandle      aHandle;
    AttributeHandle        aaHandle;
    AttributeHandle        abHandle;
    AttributeHandle        acHandle;
    //ObjectClassHandle      bHandle;
    AttributeHandle        baHandle;
    AttributeHandle        bbHandle;
    AttributeHandle        bcHandle;
    InteractionClassHandle xHandle;
    ParameterHandle        xaHandle;
    ParameterHandle        xbHandle;

    std::unique_ptr<VRTNotificationHandle> mHandle;

};

class SimpleTestAmbassador : public NullFederateAmbassador
{
  public:
    RTIambassador* rtiamb; /*!< RTI ambassador to comunicate with RTI */
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
    SimpleTestAmbassador(RTIambassador* rtiamb);
    //! Destructor
    virtual ~SimpleTestAmbassador();


    // implementation of RTI::FederateAmbassador
    ///////////////////////////////////
    // synchronization point methods //
    ///////////////////////////////////
    virtual void synchronizationPointRegistrationSucceeded(std::wstring const& label) override;

    virtual void synchronizationPointRegistrationFailed(std::wstring const& label,  SynchronizationPointFailureReason reason) override;

    virtual void announceSynchronizationPoint(std::wstring const& label, VariableLengthData const& theUserSuppliedTag) override;

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
    virtual void discoverObjectInstance(ObjectInstanceHandle theObject,
      ObjectClassHandle theObjectClass,
      std::wstring const& theObjectInstanceName) override;

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
};
