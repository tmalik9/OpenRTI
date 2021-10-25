
#pragma once

#include <RTI/NullFederateAmbassador.h>
#include <RTI/RTIambassador.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <condition_variable>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace rti1516ev;

#define INITIALIZED L"Initialized"
#define READY_TO_RUN L"ReadyToRun"
#define ALL_DONE L"AllDone"

class ImmediateCallbackTestFederate;

double convertTime(LogicalTime const& theTime);

std::wstring to_wstring(const std::string& str);
std::string to_string(const std::wstring& str);

std::vector<std::string> split(const std::string& s, const char* c);
std::vector<std::wstring> split(const std::wstring& s, const wchar_t* c);
std::wstring variableLengthDataToWstring(const VariableLengthData& variableLengthData);
VariableLengthData toVariableLengthData(const wchar_t* s);


class ImmediateCallbackTestFederate : public NullFederateAmbassador
{
  public:
    std::unique_ptr<RTIambassador> mRtiAmb; /*!< RTI ambassador to comunicate with RTI */

    // public methods //
    //! Constructor
    ImmediateCallbackTestFederate();
    //! Destructor
    virtual ~ImmediateCallbackTestFederate();
    void join(const std::string& address, const std::string& federateName, const std::string& fom, const std::string& federationName, bool regulating, bool constrained);
    virtual void initializeSimulation() = 0;
    virtual void joined() = 0;
    virtual void cleanupSimulation() = 0;
    virtual void disconnect();
    void run(std::chrono::milliseconds stepDuration);
    virtual void step() = 0;
    void setDone() { _done = true; }

    void waitForUser(const std::string& tag);
    void advanceTime(double timestep);
    double getFederateTime() const;
    double getLbts() const;
  protected:
    std::chrono::milliseconds getStepDuration() const { return stepDuration; }

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

    void setPrintVerbose(bool newValue) { _printVerbose = newValue; }

  protected:
    std::wstring mFederationName;
    FederateHandle mFederateHandle;

    std::vector<ObjectInstanceHandle> mDiscoveredObjectInstances;

    double _federateTime;
    std::mutex _federateTimeMutex;
    double _federateLookahead;
    bool _isAdvancing;
    std::condition_variable _isAdvancingCv;
    std::mutex _isAdvancingMutex;
    std::chrono::milliseconds stepDuration;

    // time modes
    bool _isRegulating;
    bool _isConstrained;
    std::condition_variable _timeModeCv;
    std::mutex _timeModeMutex;

    // sync points
    bool _isAnnouncedInitialized;
    bool _isAnnouncedReadyToRun;
    bool _isAnnouncedAllDone;
    std::mutex _syncPointAnnounceMutex;
    std::condition_variable _syncPointAnnounceCv;
    bool _syncedInitialized;
    bool _syncedReadyToRun;
    bool _syncedAllDone;
    std::mutex _syncedMutex;
    std::condition_variable _syncedCv;

    bool _printVerbose = true;
    std::mutex _coutMutex;

    std::atomic_bool _done;
};
