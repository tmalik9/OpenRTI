
#include "RTI1516ESimpleTestLib.h"
#include <RTI/RTIambassadorFactory.h>
#include "RTI/time/HLAfloat64Time.h"
#include <iostream>
#include "dprintf.h"
#include <cassert>
#include "RTI/encoding/BasicDataElements.h"
#include <thread>
#include <chrono>

using namespace rti1516ev;

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
std::wstring variableLengthDataToWstring(const rti1516ev::VariableLengthData& variableLengthData)
{
  rti1516ev::HLAunicodeString decoded;
  decoded.decode(variableLengthData);
  return decoded.get();
}

rti1516ev::VariableLengthData toVariableLengthData(const wchar_t* s)
{
  if (s)
  {
    return rti1516ev::HLAunicodeString(s).encode();
  }
  return rti1516ev::VariableLengthData();
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// class SimpleTestFederate   ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SimpleTestFederate::SimpleTestFederate()
  : _done(false)
{
#ifdef _WIN32
  mHandle = std::unique_ptr<VRTNotificationHandle>(new VRTNotificationHandle());
#endif

  federateTime      = 0.0;
  federateLookahead = 0.1;
  isRegulating  = false;
  isConstrained = false;
  isAdvancing   = false;
  isAnnouncedInitialized   = false;
  isAnnouncedReadyToRun   = false;
  isAnnouncedAllDone   = false;
  _syncedReadyToRun  = false;
  _syncedAllDone  = false;
}

SimpleTestFederate::~SimpleTestFederate()
{
}


void SimpleTestFederate::join(const std::string& address, const std::string& federateName, const std::string& fom, const std::string& federationName, bool regulating, bool constrained)
{
  mFederationName = to_wstring(federationName);
  ///
  /// 1. create the RTIambassador
  ///
  std::vector<std::wstring> fomModules;

  rti1516ev::RTIambassadorFactory factory;
  mRtiAmb = factory.createRTIambassador();
  mRtiAmb->setConnectWaitTimeout(1000);
  mRtiAmb->setOperationWaitTimeout(1000);
  /// create the federate ambassador
  try
  {
    /// create the federate ambassador and connect to RTI
    mRtiAmb->connect(*this, rti1516ev::HLA_EVOKED,  to_wstring(address));
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
    HLAfloat64Interval lookahead(federateLookahead);
    mRtiAmb->enableTimeRegulation(lookahead);
    /// wait for callback
    while (isRegulating == false && !_done)
    {
      mRtiAmb->evokeCallback(12.0);
    }
    printf("Time Regulation Enabled\n");
  }
  if (constrained)
  {
    /////////////////////////////
    /// enable time constrained
    /////////////////////////////
    mRtiAmb->enableTimeConstrained();
    /// wait for callback
    while (isConstrained == false && !_done)
    {
      mRtiAmb->evokeCallback(12.0);
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
  while (!isAnnouncedInitialized || !isAnnouncedReadyToRun || !isAnnouncedAllDone)
  {
    mRtiAmb->evokeCallback(12.0);
  }

  waitForUser("INITIALIZED");
  mRtiAmb->synchronizationPointAchieved(INITIALIZED);
  std::wcout << L"Achieved sync point: " << INITIALIZED << L", waiting for federation..." << std::endl;
  while (_syncedInitialized == false && !_done)
  {
    mRtiAmb->evokeCallback(12.0);
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
  while (_syncedReadyToRun == false && !_done)
  {
    mRtiAmb->evokeCallback(12.0);
  }
}


void SimpleTestFederate::run(std::chrono::milliseconds stepMs)
{
  //mRtiAmb->setNotificationHandle(mHandle.get());
  std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
  stepDuration = stepMs;
  while (!_done)
  {
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    auto realDuration = currentTime - lastTime;
    while (currentTime - lastTime < stepDuration)
    {
      std::chrono::duration<double> evokeSeconds = stepDuration - realDuration;
      //std::cout << "evoke(" << evokeSeconds.count() << ")" << std::endl;
      mRtiAmb->evokeCallback(evokeSeconds.count());
      currentTime = std::chrono::steady_clock::now();
    }
    step();
    lastTime = currentTime;
  }
}

void SimpleTestFederate::disconnect()
{
  //////////////////////////////////////
  // 10. delete the object we created
  //////////////////////////////////////
  mRtiAmb->synchronizationPointAchieved(ALL_DONE);
  while (_syncedAllDone == false)
  {
    mRtiAmb->evokeCallback(12.0);
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
void SimpleTestFederate::waitForUser(const std::string& tag)
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
void SimpleTestFederate::advanceTime(double timestep)
{
  //printf("%s: timestep=%f time=%0.6f\n", __FUNCTION__, timestep,  federateTime);
  /// request the advance
  isAdvancing = true;
  HLAfloat64Time newTime = (federateTime + timestep);
  mRtiAmb->timeAdvanceRequest(newTime);
  /// wait for the time advance to be granted. ticking will tell the
  /// LRC to start delivering callbacks to the federate
  while (isAdvancing && !_done)
  {
#ifdef _WIN32
    DWORD waitResult = WaitForSingleObject(mHandle->mHandle, 200);
    switch (waitResult)
    {
      case WAIT_OBJECT_0:
        //printf("%s: TID=%d: handle triggered\n", __FUNCTION__, ::GetCurrentThreadId());
        while (mRtiAmb->evokeCallback(0))
          ;
        break;
      case WAIT_TIMEOUT:
        printf("%s: TID=%d: timeout\n", __FUNCTION__, ::GetCurrentThreadId());
        //mRtiAmb->evokeCallback(1.0);
        break;
      case WAIT_FAILED:
        printf("%s: TID=%d: failed\n", __FUNCTION__, ::GetCurrentThreadId());
        break;
      case WAIT_ABANDONED:
        printf("%s: TID=%d: abandoned\n", __FUNCTION__, ::GetCurrentThreadId());
        break;
    }
#else
    mRtiAmb->evokeCallback(0.1);
#endif
  }
  //printf("%0.6f %s: new time=%f\n",  federateTime, __FUNCTION__, federateTime);
}

void SimpleTestFederate::nextMessageRequest(double requestTime)
{
  printf("%s: current=%f request=%0.6f\n", __FUNCTION__, federateTime, requestTime);
  /// request the advance
  isAdvancing = true;
  HLAfloat64Time newTime(requestTime);
  mRtiAmb->nextMessageRequest(newTime);
  federateTime = requestTime;
  /// wait for the time advance to be granted. ticking will tell the
  /// LRC to start delivering callbacks to the federate
  while (isAdvancing && !_done)
  {
#ifdef _WIN32
    DWORD waitResult = WaitForSingleObject(mHandle->mHandle, 200);
    switch (waitResult)
    {
    case WAIT_OBJECT_0:
      //printf("%s: TID=%d: handle triggered\n", __FUNCTION__, ::GetCurrentThreadId());
      while (mRtiAmb->evokeCallback(0))
        ;
      break;
    case WAIT_TIMEOUT:
      printf("%s: TID=%d: timeout\n", __FUNCTION__, ::GetCurrentThreadId());
      //mRtiAmb->evokeCallback(1.0);
      break;
    case WAIT_FAILED:
      printf("%s: TID=%d: failed\n", __FUNCTION__, ::GetCurrentThreadId());
      break;
    case WAIT_ABANDONED:
      printf("%s: TID=%d: abandoned\n", __FUNCTION__, ::GetCurrentThreadId());
      break;
    }
#else
    mRtiAmb->evokeCallback(0.1);
#endif
  }
  //printf("%0.6f %s: new time=%f\n",  federateTime, __FUNCTION__, federateTime);
}

double SimpleTestFederate::getFederateTime() const
{
  return federateTime;
}

double SimpleTestFederate::getLbts() const
{
  return (federateTime + federateLookahead);
}

#ifdef _WIN32
void SimpleTestFederate::setNotificationHandle()
{
  assert(mHandle != nullptr);
  mRtiAmb->setNotificationHandle(mHandle.get());
  printf("%s: TID=%d: starting loop\n", __FUNCTION__, ::GetCurrentThreadId());
}
#endif
///////////////////////////////////////////////////////////////////////////////
/////////////////////// Synchronization Point Callbacks ///////////////////////
///////////////////////////////////////////////////////////////////////////////
void SimpleTestFederate::synchronizationPointRegistrationSucceeded(std::wstring const& label)
{
  printf("Successfully registered sync point: %ls\n", label.c_str());
}

void SimpleTestFederate::synchronizationPointRegistrationFailed(std::wstring const& label, SynchronizationPointFailureReason reason)
{
  printf("Failed to register sync point: %ls\n", label.c_str());
}

void SimpleTestFederate::announceSynchronizationPoint(std::wstring const& label, VariableLengthData const& theUserSuppliedTag)
{
  printf("Synchronization point announced: %ls\n", label.c_str());
  if (label == INITIALIZED)
  {
    isAnnouncedInitialized = true;
  }
  else if (label == READY_TO_RUN)
  {
    isAnnouncedReadyToRun = true;
  }
  else if (label == ALL_DONE)
  {
    isAnnouncedAllDone = true;
  }
  else
  {
    mRtiAmb->synchronizationPointAchieved(label);
  }
}

void SimpleTestFederate::federationSynchronized(std::wstring const& label, FederateHandleSet const& failedToSyncSet)
{
  printf("Federation Synchronized: %ls\n", label.c_str());
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
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Time Callbacks ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SimpleTestFederate::timeRegulationEnabled(LogicalTime const& theFederateTime)
{
  isRegulating = true;
  federateTime = convertTime(theFederateTime);
  std::cout << "[TID=" << std::this_thread::get_id() << "] timeRegulationEnabled=" << to_string(theFederateTime.toString()) << std::endl;
}

void SimpleTestFederate::timeConstrainedEnabled(LogicalTime const& theFederateTime)
{
  isConstrained = true;
  federateTime = convertTime(theFederateTime);
  std::cout << "[TID=" << std::this_thread::get_id() << "] timeConstrainedEnabled=" << to_string(theFederateTime.toString()) << std::endl;
}

void SimpleTestFederate::timeAdvanceGrant(LogicalTime const& theTime)
{
  isAdvancing = false;
  federateTime = convertTime(theTime);
  std::cout << "[TID=" << std::this_thread::get_id() << "] timeAdvanceGrant=" << to_string(theTime.toString()) << std::endl;
}

//                                 //
// Reflect Attribute Value Methods //
//                                 //
void SimpleTestFederate::reflectAttributeValues(
  ObjectInstanceHandle theObject,
  AttributeHandleValueMap const& theAttributeValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  SupplementalReflectInfo theReflectInfo)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ":"
    << " instance=" << to_string(mRtiAmb->getObjectInstanceName(theObject))
    << " class=" << to_string(mRtiAmb->getObjectClassName(theObjectClass))
    << " sentOrder=" << to_string(sentOrder)
    << std::endl;
  if (_printVerbose)
  {
    for (auto attr : theAttributeValues)
    {
      std::wcout << L"    attribute=" << mRtiAmb->getAttributeName(theObjectClass, attr.first) << std::endl;
    }
  }
}

void SimpleTestFederate::reflectAttributeValues(
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
  std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ":"
    << " instance=" << to_string(mRtiAmb->getObjectInstanceName(theObject))
    << ", class=" << to_string(mRtiAmb->getObjectClassName(theObjectClass))
    << ", sentOrder=" << to_string(sentOrder)
    << ", receivedOrder=" << to_string(receivedOrder)
    << ", time=" << to_string(theTime.toString())
    << std::endl;
  if (_printVerbose)
  {
    for (auto attr : theAttributeValues)
    {
      std::wcout << L"    attribute=" << mRtiAmb->getAttributeName(theObjectClass, attr.first) << std::endl;
    }
  }
}

/**
 * receiveInteraction with some detailed output
 */
void SimpleTestFederate::receiveInteraction(
  InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  SupplementalReceiveInfo theReceiveInfo)
{
  std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ":"
    << " interactionClass=" << to_string(mRtiAmb->getInteractionClassName(theInteraction))
    << ", parameterCount=" << theParameterValues.size()
    << ", sentOrder=" << to_string(sentOrder)
    << std::endl;
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

void SimpleTestFederate::receiveInteraction(
  InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  SupplementalReceiveInfo theReceiveInfo)
{
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

void SimpleTestFederate::receiveInteraction(
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
  std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ":"
    << " interactionClass=" << to_string(mRtiAmb->getInteractionClassName(theInteraction))
    << ", parameterCount=" << theParameterValues.size()
    << ", sentOrder=" << to_string(sentOrder)
    << ", receivedOrder=" << to_string(receivedOrder)
    << ", time=" << to_string(theTime.toString())
    << ", theMessageRetractionHandle=" << to_string(theMessageRetractionHandle.toString())
    << std::endl;
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
void SimpleTestFederate::discoverObjectInstance(ObjectInstanceHandle theObject,
  ObjectClassHandle theObjectClass,
  std::wstring const& theObjectInstanceName)
{
  mDiscoveredObjectInstances.push_back(theObject);

  std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ":"
    << " instance=" << to_string(mRtiAmb->getObjectInstanceName(theObject))
    << " class=" << to_string(mRtiAmb->getObjectClassName(theObjectClass))
    << " theObjectInstanceName=" << to_string(theObjectInstanceName)
    << std::endl;
}


void SimpleTestFederate::attributesInScope(ObjectInstanceHandle theObject,
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


void SimpleTestFederate::attributesOutOfScope(ObjectInstanceHandle theObject,
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

void SimpleTestFederate::provideAttributeValueUpdate(ObjectInstanceHandle theObject, AttributeHandleSet const& theAttributes,
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
void SimpleTestFederate::removeObjectInstance(
  ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  SupplementalRemoveInfo theRemoveInfo)
{
  std::wcout << L"Object Removed: handle=" << theObject << std::endl;
}

void SimpleTestFederate::removeObjectInstance(
  ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  SupplementalRemoveInfo theRemoveInfo)
{
  std::wcout << L"Object Removed: handle=" << theObject << std::endl;
}

void SimpleTestFederate::removeObjectInstance(
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
