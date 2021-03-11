
#pragma once

#include <RTI/NullFederateAmbassador.h>
#include <RTI/RTIambassador.h>
#include <atomic>
#include <chrono>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace rti1516e;

#define INITIALIZED L"Initialized"
#define READY_TO_RUN L"ReadyToRun"
#define ALL_DONE L"AllDone"

class SimpleTestFederate;

double convertTime(LogicalTime const& theTime);

std::wstring to_wstring(const std::string& str);
std::string to_string(const std::wstring& str);

std::vector<std::string> split(const std::string& s, const char* c);
std::vector<std::wstring> split(const std::wstring& s, const wchar_t* c);
std::wstring variableLengthDataToWstring(const VariableLengthData& variableLengthData);
VariableLengthData toVariableLengthData(const wchar_t* s);


class SimpleTestFederate : public NullFederateAmbassador
{
#ifdef _WIN32
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
#endif

  public:
    std::unique_ptr<RTIambassador> mRtiAmb; /*!< RTI ambassador to comunicate with RTI */

    // public methods //
    //! Constructor
    SimpleTestFederate();
    //! Destructor
    virtual ~SimpleTestFederate();
    void join(const std::string& address, const std::string& federateName, const std::string& fom, const std::string& federationName, bool regulating, bool constrained);
    virtual void initializeSimulation() = 0;
    virtual void cleanupSimulation() = 0;
    virtual void disconnect();
    void run(unsigned int milliSeconds);
    virtual void step() = 0;
    void setDone() { _done = true; }

    void waitForUser(const std::string& tag);
    void advanceTime(double timestep);
    double getFederateTime() const;
    double getLbts() const;

#ifdef _WIN32
    void setNotificationHandle();
#endif
    // implementation of RTI::FederateAmbassador
    ///////////////////////////////////
    // synchronization point methods //
    ///////////////////////////////////
    virtual void connectionLost(const std::wstring& faultDescription) override
    {
      std::cout << "connection lost:" << to_string(faultDescription) << std::endl;
      _done = true;
    }
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

protected:
#ifdef _WIN32
    std::unique_ptr<VRTNotificationHandle> mHandle;
#endif
    std::wstring mFederationName;
    FederateHandle mFederateHandle;

    std::vector<ObjectInstanceHandle> mDiscoveredObjectInstances;

    double federateTime;
    double federateLookahead;

    bool isRegulating;
    bool isConstrained;
    bool isAdvancing;
    bool isAnnouncedInitialized;
    bool isAnnouncedReadyToRun;
    bool isAnnouncedAllDone;
    bool _syncedInitialized;
    bool _syncedReadyToRun;
    bool _syncedAllDone;

    std::atomic_bool _done;
};
