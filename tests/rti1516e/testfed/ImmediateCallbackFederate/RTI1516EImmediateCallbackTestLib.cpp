
#include "RTI1516EImmediateCallbackTestLib.h"
#include <RTI/RTIambassadorFactory.h>
#include "RTI/time/HLAfloat64Time.h"
#include <iostream>
#include "dprintf.h"
#include <cassert>
#include <thread>
#include <chrono>

#include "RTI/encoding/BasicDataElements.h"

using namespace rti1516e;

///////////////////////////////////////////////////////////////////////////////
/////////////////////// helper functions //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


std::wostream& operator<<(std::wostream& os, OrderType orderType)
{
  switch (orderType)
  {
  case OrderType::RECEIVE:
    os << L"RECEIVE";
    break;
  case OrderType::TIMESTAMP:
    os << L"TIMESTAMP";
    break;
  default:
    os << L"<invalid OrderType>";
    break;
  }
  return os;
}

double convertTime(LogicalTime const& theTime)
{
  HLAfloat64Time castedTime = (HLAfloat64Time)theTime;
  return castedTime.getTime();
}

using std::to_string;
using std::to_wstring;

std::wstring to_wstring(const std::string& str)
{
  if (str.empty()) return std::wstring();
  const std::ctype<wchar_t>& CType = std::use_facet<std::ctype<wchar_t> >(std::locale());
  std::vector<wchar_t> wideStringBuffer(str.length());
  CType.widen(str.data(), str.data() + str.length(), &wideStringBuffer[0]);
  return std::wstring(&wideStringBuffer[0], wideStringBuffer.size());
}

std::string to_string(const std::wstring& str)
{
  if (str.empty()) return std::string();
  const std::ctype<wchar_t>& CType = std::use_facet<std::ctype<wchar_t> >(std::locale());
  std::vector<char> stringBuffer(str.length());
  CType.narrow(str.data(), str.data() + str.length(), '_', &stringBuffer[0]);
  return std::string(&stringBuffer[0], stringBuffer.size());
}

std::string to_string(OrderType orderType)
{
  switch (orderType)
  {
  case OrderType::RECEIVE:
    return "RECEIVE";
  case OrderType::TIMESTAMP:
    return "TIMESTAMP";
  default:
    return "<invalid OrderType>";
    break;
  }
}



std::vector<std::string>
split(const std::string& s, const char* c)
{
  std::vector<std::string> v;
  std::string::size_type p0 = 0;
  std::string::size_type p = s.find_first_of(c);
  while (p != std::string::npos) {
    v.push_back(s.substr(p0, p - p0));
    p0 = s.find_first_not_of(c, p);
    if (p0 == std::string::npos)
      return v;
    p = s.find_first_of(c, p0);
  }
  v.push_back(s.substr(p0, p - p0));
  return v;
}


std::vector<std::wstring>
split(const std::wstring& s, const wchar_t* c)
{
  std::vector<std::wstring> v;
  std::wstring::size_type p0 = 0;
  std::wstring::size_type p = s.find_first_of(c);
  while (p != std::wstring::npos) {
    v.push_back(s.substr(p0, p - p0));
    p0 = s.find_first_not_of(c, p);
    if (p0 == std::string::npos)
      return v;
    p = s.find_first_of(c, p0);
  }
  v.push_back(s.substr(p0, p - p0));
  return v;
}

/**
 * convert variableLengthData to Wstring (deserialize)\n
 * for serialize function see ExampleCPPFederate class
 */
std::wstring variableLengthDataToWstring(const rti1516e::VariableLengthData& variableLengthData)
{
  rti1516e::HLAunicodeString decoded;
  decoded.decode(variableLengthData);
  return decoded.get();
}

rti1516e::VariableLengthData toVariableLengthData(const wchar_t* s)
{
  if (s)
  {
    return rti1516e::HLAunicodeString(s).encode();
  }
  return rti1516e::VariableLengthData();
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// class ImmediateCallbackTestFederate   ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ImmediateCallbackTestFederate::ImmediateCallbackTestFederate()
  : _done(false)
{
  _federateTime      = 0.0;
  _federateLookahead = 1.0;
  _isRegulating  = false;
  _isConstrained = false;
  _isAdvancing   = false;
  _isAnnouncedInitialized   = false;
  _isAnnouncedReadyToRun   = false;
  _isAnnouncedAllDone   = false;
  _syncedReadyToRun  = false;
  _syncedAllDone  = false;

}

ImmediateCallbackTestFederate::~ImmediateCallbackTestFederate()
{
}


void ImmediateCallbackTestFederate::join(const std::string& address, const std::string& federateName, const std::string& fom, const std::string& federationName, bool regulating, bool constrained)
{
  mFederationName = to_wstring(federationName);
  ///
  /// 1. create the RTIambassador
  ///
  std::vector<std::wstring> fomModules;

  rti1516e::RTIambassadorFactory factory;
  mRtiAmb = factory.createRTIambassador();
  mRtiAmb->setConnectWaitTimeout(20000);
  mRtiAmb->setOperationWaitTimeout(20000);
  /// create the federate ambassador
  try
  {
    /// create the federate ambassador and connect to RTI
    mRtiAmb->connect(*this, rti1516e::HLA_IMMEDIATE,  to_wstring(address));
    printf("%s: connected to rtinode at %s\n", __FUNCTION__, address.c_str());
  }
  catch (ConnectionFailed e)
  {
    printf("%s: could not connect to %s: %S\n", __FUNCTION__, address.c_str(), e.what().c_str());
    return;
  }
  ///
  /// 2. create and join to the federation
  /// NOTE: some other federate may have already created the federation,
  ///       in that case, we'll just try and join it
  ///
  try
  {
    mRtiAmb->createFederationExecution(mFederationName, to_wstring(fom), L"HLAfloat64Time");
    printf("Created Federation\n");
  }
  catch (FederationExecutionAlreadyExists exists)
  {
    std::cout << "Didn't create federation, it already existed" << std::endl;
    fomModules.push_back(to_wstring(fom));
  }
  /////////////////////////////
  /// 3. join the federation
  /////////////////////////////
  if (federateName.empty())
  {
    mFederateHandle = mRtiAmb->joinFederationExecution(L"ExampleFederate", mFederationName, fomModules);
  }
  else
  {
    mFederateHandle = mRtiAmb->joinFederationExecution(to_wstring(federateName), L"ExampleFederate", mFederationName, fomModules);
  }
  std::wcout << L"Joined Federation as " << mRtiAmb->getFederateName(mFederateHandle) << std::endl;
  //waitForUser("Joined");
  /////////////////////////////
  // 6. enable time policies
  /////////////////////////////
  if (regulating)
  {
    /////////////////////////////
    /// enable time regulation
    /////////////////////////////
    HLAfloat64Interval lookahead(_federateLookahead);
    std::unique_lock<std::mutex> timeModeLock(_timeModeMutex);
    mRtiAmb->enableTimeRegulation(lookahead);
    /// wait for callback
    while (_isRegulating == false)
    {
      _timeModeCv.wait(timeModeLock);
    }
    printf("Time Regulation Enabled\n");
  }
  if (constrained)
  {
    /////////////////////////////
    /// enable time constrained
    /////////////////////////////
    std::unique_lock<std::mutex> timeModeLock(_timeModeMutex);
    mRtiAmb->enableTimeConstrained();
    /// wait for callback
    while (_isConstrained == false)
    {
      _timeModeCv.wait(timeModeLock);
    }
    printf("Time Constrained Enabled\n");
  }
  /////////////////////////////////
  /// 4. announce the sync point
  /////////////////////////////////
  /// announce a sync point to get everyone on the same page. if the point
  /// has already been registered, we'll get a callback saying it failed,
  /// but we don't care about that, as long as someone registered it
  mRtiAmb->registerFederationSynchronizationPoint(INITIALIZED, toVariableLengthData(L""));
  mRtiAmb->registerFederationSynchronizationPoint(READY_TO_RUN, toVariableLengthData(L""));
  mRtiAmb->registerFederationSynchronizationPoint(ALL_DONE, toVariableLengthData(L""));
  std::cout << "SynchronizationPoint registered" << std::endl;
  {
    std::unique_lock<std::mutex> announceLock(_syncPointAnnounceMutex);
    while (!(_isAnnouncedInitialized && _isAnnouncedReadyToRun && _isAnnouncedAllDone))
    {
      _syncPointAnnounceCv.wait(announceLock);
    }
  }

  waitForUser("INITIALIZED");
  mRtiAmb->synchronizationPointAchieved(INITIALIZED);
  std::wcout << L"Achieved sync point: " << INITIALIZED << L", waiting for federation..." << std::endl;
  {
    std::unique_lock<std::mutex> syncLockInitialized(_syncedMutex);
    while (_syncedInitialized == false)
    {
      _syncedCv.wait(syncLockInitialized);
    }
  }
  /// WAIT FOR USER TO KICK US OFF.\n
  /// So that there is time to add other federates, we will wait until the
  /// user hits enter before proceeding. That was, you have time to start
  /// other federates.
  initializeSimulation();
  ////////////////////////////////////////////////////////
  /// 5. achieve the point and wait for synchronization
  ////////////////////////////////////////////////////////
  /// tell the RTI we are ready to move past the sync point and then wait
  /// until the federation has synchronized on
  mRtiAmb->synchronizationPointAchieved(READY_TO_RUN);
  std::wcout << L"Achieved sync point: " << READY_TO_RUN << L", waiting for federation..." << std::endl;
  {
    std::unique_lock<std::mutex> syncLockReadyToRun(_syncedMutex);
    while (_syncedReadyToRun == false)
    {
      _syncedCv.wait(syncLockReadyToRun);
    }
  }
  joined();
}


void ImmediateCallbackTestFederate::run(std::chrono::milliseconds stepMs)
{
  stepDuration = stepMs;
  while (!_done)
  {
    std::this_thread::sleep_for(stepDuration);
    step();
  }
}

void ImmediateCallbackTestFederate::disconnect()
{
  //////////////////////////////////////
  // 10. delete the object we created
  //////////////////////////////////////
  mRtiAmb->synchronizationPointAchieved(ALL_DONE);
  {
    std::unique_lock<std::mutex> syncLockAllDone(_syncedMutex);
    while (_syncedAllDone == false)
    {
      _syncedCv.wait(syncLockAllDone);
    }
  }
  cleanupSimulation();

  ////////////////////////////////////
  // 11. resign from the federation
  ////////////////////////////////////
  mRtiAmb->resignFederationExecution(NO_ACTION);
  printf("Resigned from Federation\n");
  ////////////////////////////////////////
  // 12. try and destroy the federation
  ////////////////////////////////////////
  // NOTE: we won't die if we can't do this because other federates
  //       remain. in that case we'll leave it for them to clean up
  try
  {
    mRtiAmb->destroyFederationExecution(mFederationName);
    printf("Destroyed Federation");
  }
  catch (FederationExecutionDoesNotExist dne)
  {
    printf("No need to destroy federation, it doesn't exist");
  }
  catch (FederatesCurrentlyJoined fcj)
  {
    printf("Didn't destroy federation, federates still joined");
  }
  mRtiAmb->disconnect();
}
void ImmediateCallbackTestFederate::waitForUser(const std::string& tag)
{
  std::cout << " >>>>>>>>>> " << tag << ": Press Enter to Continue <<<<<<<<<<" << std::endl;
  std::string line;
  getline(std::cin, line);
}

/**
 * This method will request a time advance to the current time, plus the given
 * timestep. It will then wait until a notification of the time advance grant
 * has been received.
 */
void ImmediateCallbackTestFederate::advanceTime(double timestep)
{
  //printf("%s: timestep=%f time=%0.6f\n", __FUNCTION__, timestep,  federateTime);
  /// request the advance
  _isAdvancing = true;
  HLAfloat64Time newTime = (_federateTime + timestep);
  mRtiAmb->timeAdvanceRequest(newTime);
  // We lock the federate time until we get back into non-advancing state
  std::unique_lock<std::mutex> federateTimeLock(_federateTimeMutex);
  std::unique_lock<std::mutex> isAdvancingLock(_isAdvancingMutex);
  while (_isAdvancing)
  {
    _isAdvancingCv.wait(isAdvancingLock);
  }
}

double ImmediateCallbackTestFederate::getFederateTime() const
{
  return _federateTime;
}

double ImmediateCallbackTestFederate::getLbts() const
{
  return (_federateTime + _federateLookahead);
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// Synchronization Point Callbacks ///////////////////////
///////////////////////////////////////////////////////////////////////////////
void ImmediateCallbackTestFederate::synchronizationPointRegistrationSucceeded(std::wstring const& label)
{
  printf("Successfully registered sync point: %ls\n", label.c_str());
}

void ImmediateCallbackTestFederate::synchronizationPointRegistrationFailed(std::wstring const& label, SynchronizationPointFailureReason reason)
{
  printf("Failed to register sync point: %ls\n", label.c_str());
}

void ImmediateCallbackTestFederate::announceSynchronizationPoint(std::wstring const& label, VariableLengthData const& theUserSuppliedTag)
{
  printf("Synchronization point announced: %ls\n", label.c_str());
  std::unique_lock<std::mutex> announceLock(_syncPointAnnounceMutex);
  if (label == INITIALIZED)
  {
    _isAnnouncedInitialized = true;
  }
  else if (label == READY_TO_RUN)
  {
    _isAnnouncedReadyToRun = true;
  }
  else if (label == ALL_DONE)
  {
    _isAnnouncedAllDone = true;
  }
  else
  {
    mRtiAmb->synchronizationPointAchieved(label);
  }
  _syncPointAnnounceCv.notify_all();
}

void ImmediateCallbackTestFederate::federationSynchronized(std::wstring const& label, FederateHandleSet const& failedToSyncSet)
{
  printf("Federation Synchronized: %ls\n", label.c_str());
  std::unique_lock<std::mutex> syncedLock(_syncedMutex);
  if (label == INITIALIZED)
  {
    _syncedInitialized = true;
  }
  else if (label == READY_TO_RUN)
  {
    _syncedReadyToRun = true;
  }
  else if (label == ALL_DONE)
  {
    _syncedAllDone = true;
  }
  _syncedCv.notify_all();
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Time Callbacks ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ImmediateCallbackTestFederate::timeRegulationEnabled(LogicalTime const& theFederateTime)
{
  std::unique_lock<std::mutex> federateTimeLock(_federateTimeMutex);
  std::unique_lock<std::mutex> timeModeLock(_timeModeMutex);
  _federateTime = convertTime(theFederateTime);
  std::cout << "[TID=" << std::this_thread::get_id() << "] timeRegulationEnabled=" << to_string(theFederateTime.toString()) << std::endl;
  _isRegulating = true;
  _timeModeCv.notify_all();
}

void ImmediateCallbackTestFederate::timeConstrainedEnabled(LogicalTime const& theFederateTime)
{
  std::unique_lock<std::mutex> federateTimeLock(_federateTimeMutex);
  std::unique_lock<std::mutex> timeModeLock(_timeModeMutex);
  _federateTime = convertTime(theFederateTime);
  std::cout << "[TID=" << std::this_thread::get_id() << "] timeConstrainedEnabled=" << to_string(theFederateTime.toString()) << std::endl;
  _isConstrained = true;
  _timeModeCv.notify_all();
}

void ImmediateCallbackTestFederate::timeAdvanceGrant(LogicalTime const& theTime)
{
  // do NOT lock the _federateTime here: DON'T std::unique_lock<std::mutex> lock(_federateTimeMutex);
  std::unique_lock<std::mutex> lock(_isAdvancingMutex);
  _federateTime = convertTime(theTime);
  _coutMutex.lock();
    std::cout << "[TID=" << std::this_thread::get_id() << "] timeAdvanceGrant=" << to_string(theTime.toString()) << std::endl;
  _coutMutex.unlock();
  _isAdvancing = false;
  _isAdvancingCv.notify_all();
}

//                                 //
// Reflect Attribute Value Methods //
//                                 //
void ImmediateCallbackTestFederate::reflectAttributeValues(
  ObjectInstanceHandle theObject,
  AttributeHandleValueMap const& theAttributeValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  SupplementalReflectInfo theReflectInfo)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::unique_lock<std::mutex> lock(_coutMutex);
  std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ":"
            << " instance=" << to_string(mRtiAmb->getObjectInstanceName(theObject))
            << " class=" << to_string(mRtiAmb->getObjectClassName(theObjectClass))
            << " sentOrder=" << to_string(sentOrder)
            << std::endl;
  if (_printVerbose)
  {
    for (auto attr : theAttributeValues)
    {
      std::wstring strData(static_cast<const wchar_t*>(attr.second.data()), attr.second.size() / sizeof(wchar_t));
      printf("    attribute=%ls value=%ls", mRtiAmb->getAttributeName(theObjectClass, attr.first).c_str(), strData.c_str());
    }
  }
}

void ImmediateCallbackTestFederate::reflectAttributeValues(
  ObjectInstanceHandle theObject,
  AttributeHandleValueMap const& theAttributeValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  SupplementalReflectInfo theReflectInfo)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::unique_lock<std::mutex> lock(_coutMutex);
  std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ":"
            << " instance=" << to_string(mRtiAmb->getObjectInstanceName(theObject))
            << " class=" << to_string(mRtiAmb->getObjectClassName(theObjectClass))
            << " sentOrder=" << to_string(sentOrder)
            << " receivedOrder=" << to_string(receivedOrder)
            << " time=" << to_string(theTime.toString())
            << std::endl;
  if (_printVerbose)
  {
    for (auto attr : theAttributeValues)
    {
      printf("    attribute=%ls", mRtiAmb->getAttributeName(theObjectClass, attr.first).c_str());
    }
  }
}

/**
 * receiveInteraction with some detailed output
 */
void ImmediateCallbackTestFederate::receiveInteraction(
  InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  SupplementalReceiveInfo theReceiveInfo)
{
  std::unique_lock<std::mutex> lock(_coutMutex);
  std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ":"
            << " interactionClass=" << to_string(mRtiAmb->getInteractionClassName(theInteraction))
            << ", parameterCount=" << theParameterValues.size()
            << ", sentOrder=" << to_string(sentOrder)
            << std::endl;
  if (_printVerbose)
  {
    for (ParameterHandleValueMap::const_iterator i = theParameterValues.begin(); i != theParameterValues.end(); i++)
    {
      /// print the parameter handle
      std::wcout << L"\tparamHandle=" << i->first;
      std::wcout << L"\tdataType=" << mRtiAmb->getParameterDataType(theInteraction, i->first);
      /// print the parameter value
      //std::wstring value = variableLengthDataToWstring(i->second);
      //std::wcout << ", paramValue=\"" << value << L"\"" << std::endl;
      std::wcout << std::endl;
    }
  }
}

void ImmediateCallbackTestFederate::receiveInteraction(
  InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  SupplementalReceiveInfo theReceiveInfo)
{
  std::unique_lock<std::mutex> lock(_coutMutex);
  std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ":"
            << " interactionClass=" << to_string(mRtiAmb->getInteractionClassName(theInteraction))
            << ", parameterCount=" << theParameterValues.size()
            << ", sentOrder=" << to_string(sentOrder)
            << ", receivedOrder=" << to_string(receivedOrder)
            << ", time=" << to_string(theTime.toString())
            << std::endl;
  if (_printVerbose)
  {
    for (ParameterHandleValueMap::const_iterator i = theParameterValues.begin();
      i != theParameterValues.end(); i++)
    {
      /// print the parameter handle
      std::wcout << "\tparamHandle=" << i->first;
      std::wcout << L"\tdataType=" << mRtiAmb->getParameterDataType(theInteraction, i->first);
      /// print the parameter value
      //std::wstring value = variableLengthDataToWstring(i->second);
      //std::wcout << ", paramValue=\"" << value << L"\"" << std::endl;
      std::wcout << std::endl;
    }
  }
}

void ImmediateCallbackTestFederate::receiveInteraction(
  InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  MessageRetractionHandle theMessageRetractionHandle,
  SupplementalReceiveInfo theReceiveInfo)
{
  std::unique_lock<std::mutex> lock(_coutMutex);
  std::wcout << L"Interaction Received:";
  /// print the handle
  std::wcout << " handle=" << theInteraction;
  /// print the tag
  // std::wstring data = variableLengthDataToWstring(theUserSuppliedTag);
  //std::wcout << ", tag=\"" << data << "\"";
  /// print the attribute information
  std::wcout << ", parameterCount=" << theParameterValues.size();
  std::wcout << ", theTime=" << theTime.toString();
  std::wcout << ", sentOrder=" << sentOrder;
  std::wcout << ", receivedOrder=" << receivedOrder;
  std::wcout << ", theMessageRetractionHandle=" << theMessageRetractionHandle;
  std::wcout << std::endl;
  if (_printVerbose)
  {
    for (ParameterHandleValueMap::const_iterator i = theParameterValues.begin();
      i != theParameterValues.end(); i++)
    {
      /// print the parameter handle
      std::wcout << L"\tparamHandle=" << i->first;
      std::wcout << L"\tdataType=" << mRtiAmb->getParameterDataType(theInteraction, i->first);
      /// print the parameter value
      //std::wstring value = variableLengthDataToWstring(i->second);
      //std::wcout << ", paramValue=\"" << value << L"\"" << std::endl;
      std::wcout << std::endl;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////// Object Management Callbacks /////////////////////////
///////////////////////////////////////////////////////////////////////////////

//                         //
// Discover Object Methods //
//                         //
void ImmediateCallbackTestFederate::discoverObjectInstance(ObjectInstanceHandle theObject,
    ObjectClassHandle theObjectClass,
    std::wstring const& theObjectInstanceName)
{
  mDiscoveredObjectInstances.push_back(theObject);

  printf("%s: instanceHandle=%ls class=%ls name=%ls", __FUNCTION__,
         theObject.toString().c_str(),
         mRtiAmb->getObjectClassName(theObjectClass).c_str(),
         theObjectInstanceName.c_str());
}


void ImmediateCallbackTestFederate::attributesInScope(ObjectInstanceHandle theObject,
    const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::wcout << to_wstring(__FUNCTION__) << L": "
             << L"instance=" << mRtiAmb->getObjectInstanceName(theObject)
             << L", class=" << mRtiAmb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wcout << L"    attribute=" << mRtiAmb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}


void ImmediateCallbackTestFederate::attributesOutOfScope(ObjectInstanceHandle theObject,
    const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::wcout << to_wstring(__FUNCTION__) << L": "
             << L"instance=" << mRtiAmb->getObjectInstanceName(theObject)
             << L", class=" << mRtiAmb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wcout << L"    attribute=" << mRtiAmb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}

void ImmediateCallbackTestFederate::provideAttributeValueUpdate(ObjectInstanceHandle theObject, AttributeHandleSet const& theAttributes,
    VariableLengthData const& theUserSuppliedTag)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::wcout << to_wstring(__FUNCTION__) << L": "
             << L"instance=" << mRtiAmb->getObjectInstanceName(theObject)
             << L", class=" << mRtiAmb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributes)
  {
    std::wcout << L"    attribute=" << mRtiAmb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}


//                       //
// Remove Object Methods //
//                       //
void ImmediateCallbackTestFederate::removeObjectInstance(
  ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  SupplementalRemoveInfo theRemoveInfo)
{
  std::wcout << L"Object Removed: handle=" << theObject << std::endl;
}

void ImmediateCallbackTestFederate::removeObjectInstance(
  ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  SupplementalRemoveInfo theRemoveInfo)
{
  std::wcout << L"Object Removed: handle=" << theObject << std::endl;
}

void ImmediateCallbackTestFederate::removeObjectInstance(
  ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  MessageRetractionHandle theHandle,
  SupplementalRemoveInfo theRemoveInfo)
{
  std::wcout << L"Object Removed: handle=" << theObject << std::endl;
}
