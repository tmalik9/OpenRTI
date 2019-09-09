
#include "RTI1516ESimpleTestLib.h"
#include <RTI/RTIambassadorFactory.h>
#include "RTI/time/HLAfloat64Time.h"
#include <iostream>
#include "dprintf.h"

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

/**
 * convert normal C++-Strings to C++-Wstrings
 * Wstrings are used by openrti
 */
std::wstring convertStringToWstring(const std::string& str)
{
  const std::ctype<wchar_t>& CType = std::use_facet<std::ctype<wchar_t> >(std::locale());
  std::vector<wchar_t> wideStringBuffer(str.length());
  CType.widen(str.data(), str.data() + str.length(), &wideStringBuffer[0]);
  return std::wstring(&wideStringBuffer[0], wideStringBuffer.size());
}

/**
 * convert variableLengthData to Wstring (deserialize)\n
 * for serialize function see ExampleCPPFederate class
 */
std::wstring variableLengthDataToWstring(const rti1516e::VariableLengthData& variableLengthData)
{
  if (!variableLengthData.size())
  {
    return std::wstring();
  }
  return std::wstring((const wchar_t*) variableLengthData.data(), variableLengthData.size() / sizeof(std::wstring::value_type));
}

rti1516e::VariableLengthData toVariableLengthData(const wchar_t* s)
{
  rti1516e::VariableLengthData variableLengthData;
  if (s)
  {
    variableLengthData.setData(s, wcslen(s) * sizeof(wchar_t));
  }
  return variableLengthData;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// class SimpleTestFederate   ////////////////////////////
///////////////////////////////////////////////////////////////////////////////


SimpleTestFederate::SimpleTestFederate()
{
  mHandle = std::make_unique<VRTNotificationHandle>();
}

SimpleTestFederate::~SimpleTestFederate()
{
}


void SimpleTestFederate::initialize(std::string address, std::string federateName, std::string fom, bool regulating, bool constrained)
{
  ///
  /// 1. create the RTIambassador
  ///
  rti1516e::RTIambassadorFactory factory;
  this->rtiamb = factory.createRTIambassador();
  /// create the federate ambassador
  try
  {
    /// create the federate ambassador and connect to RTI
    fedamb = std::make_unique<SimpleTestAmbassador>(this->rtiamb.get());
    rtiamb->connect(*fedamb, rti1516e::HLA_EVOKED,  convertStringToWstring(address));
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
    rtiamb->createFederationExecution(L"ExampleFederation", convertStringToWstring(fom), L"HLAfloat64Time");
    DebugPrintf("Created Federation\n");
  }
  catch (FederationExecutionAlreadyExists exists)
  {
    DebugPrintf("Didn't create federation, it already existed\n");
  }
  /////////////////////////////
  /// 3. join the federation
  /////////////////////////////
  rtiamb->joinFederationExecution(convertStringToWstring(federateName),
    convertStringToWstring("ExampleFederation"));
  std::cout << "Joined Federation as " << federateName << std::endl;
  /// initialize the handles - have to wait until we are joined
  initializeHandles();
  /////////////////////////////////
  /// 4. announce the sync point
  /////////////////////////////////
  /// announce a sync point to get everyone on the same page. if the point
  /// has already been registered, we'll get a callback saying it failed,
  /// but we don't care about that, as long as someone registered it
  rtiamb->registerFederationSynchronizationPoint(READY_TO_RUN, toVariableLengthData(L""));
  rtiamb->registerFederationSynchronizationPoint(ALL_DONE, toVariableLengthData(L""));
  std::cout << "SynchronizationPoint registered" << std::endl;
  while (!fedamb->isAnnouncedReadyToRun || !fedamb->isAnnouncedAllDone)
  {
    rtiamb->evokeCallback(12.0);
  }
  /// WAIT FOR USER TO KICK US OFF.\n
  /// So that there is time to add other federates, we will wait until the
  /// user hits enter before proceeding. That was, you have time to start
  /// other federates.
  waitForUser();
  ////////////////////////////////////////////////////////
  /// 5. achieve the point and wait for synchronization
  ////////////////////////////////////////////////////////
  /// tell the RTI we are ready to move past the sync point and then wait
  /// until the federation has synchronized on
  rtiamb->synchronizationPointAchieved(READY_TO_RUN);
  std::wcout << L"Achieved sync point: " << READY_TO_RUN << L", waiting for federation..." << std::endl;
  while (fedamb->isReadyToRun == false)
  {
    rtiamb->evokeCallback(12.0);
  }
  /////////////////////////////
  // 6. enable time policies
  /////////////////////////////
  if (regulating)
  {
    /////////////////////////////
    /// enable time regulation
    /////////////////////////////
    HLAfloat64Interval lookahead(fedamb->federateLookahead);
    rtiamb->enableTimeRegulation(lookahead);
    /// wait for callback
    while (fedamb->isRegulating == false)
    {
      rtiamb->evokeCallback(12.0);
    }
    DebugPrintf("Time Regulation Enabled\n");
  }
  if (constrained)
  {
    /////////////////////////////
    /// enable time constrained
    /////////////////////////////
    rtiamb->enableTimeConstrained();
    /// wait for callback
    while (fedamb->isConstrained == false)
    {
      rtiamb->evokeCallback(12.0);
    }
    DebugPrintf("Time Constrained Enabled\n");
  }
}

/**
 * This method will get all the relevant handle information from the RTIambassador
 */
void SimpleTestFederate::initializeHandles()
{
  ObjectClassHandle aHandle = rtiamb->getObjectClassHandle(L"HLAobjectRoot.A");
  ObjectClassHandle bHandle = rtiamb->getObjectClassHandle(L"HLAobjectRoot.B");
  mObjectClassHandles["HLAobjectRoot.A"] = aHandle;
  mObjectClassHandles["HLAobjectRoot.B"] = bHandle;
  //this->aHandle  = rtiamb->getObjectClassHandle(L"HLAobjectRoot.A");
  this->aaHandle = rtiamb->getAttributeHandle(aHandle, L"aa");
  this->abHandle = rtiamb->getAttributeHandle(aHandle, L"ab");
  this->acHandle = rtiamb->getAttributeHandle(aHandle, L"ac");
  //this->bHandle  = rtiamb->getObjectClassHandle(L"HLAobjectRoot.B");
  this->baHandle = rtiamb->getAttributeHandle(bHandle, L"ba");
  this->bbHandle = rtiamb->getAttributeHandle(bHandle, L"bb");
  this->bcHandle = rtiamb->getAttributeHandle(bHandle, L"bc");
  this->xHandle  = rtiamb->getInteractionClassHandle(L"HLAinteractionRoot.X");
  this->xaHandle = rtiamb->getParameterHandle(this->xHandle, convertStringToWstring("xa"));
  this->xbHandle = rtiamb->getParameterHandle(this->xHandle, convertStringToWstring("xb"));
}

void SimpleTestFederate::waitForUser()
{
  std::cout << " >>>>>>>>>> Press Enter to Continue <<<<<<<<<<" << std::endl;
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
  DebugPrintf("%s: timestep=%f time=%0.6f\n", __FUNCTION__, timestep,  fedamb->federateTime);
  /// request the advance
  fedamb->isAdvancing = true;
  HLAfloat64Time newTime = (fedamb->federateTime + timestep);
  rtiamb->timeAdvanceRequest(newTime);
  /// wait for the time advance to be granted. ticking will tell the
  /// LRC to start delivering callbacks to the federate
  while (fedamb->isAdvancing)
  {
    DWORD waitResult = WaitForSingleObject(mHandle->mHandle, 200);
    switch (waitResult)
    {
      case WAIT_OBJECT_0:
        DebugPrintf("%s: TID=%d: handle triggered\n", __FUNCTION__, ::GetCurrentThreadId());
        while (rtiamb->evokeCallback(0.001))
        {
          DebugPrintf("%s: TID=%d: callbacks evoked\n", __FUNCTION__, ::GetCurrentThreadId());
        }
        break;
      case WAIT_TIMEOUT:
        DebugPrintf("%s: TID=%d: timeout\n", __FUNCTION__, ::GetCurrentThreadId());
        //rtiamb->evokeCallback(1.0);
        break;
      case WAIT_FAILED:
        DebugPrintf("%s: TID=%d: failed\n", __FUNCTION__, ::GetCurrentThreadId());
        break;
      case WAIT_ABANDONED:
        DebugPrintf("%s: TID=%d: abandoned\n", __FUNCTION__, ::GetCurrentThreadId());
        break;
    }
  }
  DebugPrintf("%0.6f %s: new time=%f\n",  fedamb->federateTime, __FUNCTION__, fedamb->federateTime);
}

/**
 * This method will send out an interaction of the type InteractionRoot.X. Any
 * federates which are subscribed to it will receive a notification the next time
 * they tick(). Here we are passing only two of the three parameters we could be
 * passing, but we don't actually have to pass any at all!
 */
void SimpleTestFederate::sendInteraction()
{
  ////////////////////////////////////////////////
  /// create the necessary container and values
  ////////////////////////////////////////////////
  /// create the collection to store the values in
  ParameterHandleValueMap parameters;
  /// generate the new values
  wchar_t xaValue[16], xbValue[16];
  swprintf(xaValue, 16, L"xa:%f", getLbts());
  swprintf(xbValue, 16, L"xb:%f", getLbts());
  parameters[xaHandle] = toVariableLengthData(xaValue);
  parameters[xbHandle] = toVariableLengthData(xbValue);
  ///////////////////////////
  /// send the interaction
  ///////////////////////////
  //rtiamb->sendInteraction(xHandle, parameters, toVariableLengthData(L"hi!"));
  /// if you want to associate a particular timestamp with the
  /// interaction, you will have to supply it to the RTI. Here
  /// we send another interaction, this time with a timestamp:
  HLAfloat64Time time = fedamb->federateTime + fedamb->federateLookahead;
  rtiamb->sendInteraction(xHandle, parameters, toVariableLengthData(L"hi!"), time);
}

double SimpleTestFederate::getFederateTime() const
{
  return fedamb->federateTime;
}

double SimpleTestFederate::getLbts() const
{
  return (fedamb->federateTime + fedamb->federateLookahead);
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// class SimpleTestAmbassador ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SimpleTestAmbassador::SimpleTestAmbassador(RTIambassador* rtiamb)
{
  // initialize all the variable values
  this->rtiamb = rtiamb;
  this->federateTime      = 0.0;
  this->federateLookahead = 1.0;
  this->isRegulating  = false;
  this->isConstrained = false;
  this->isAdvancing   = false;
  this->isAnnouncedReadyToRun   = false;
  this->isAnnouncedAllDone   = false;
  this->isReadyToRun  = false;
  this->allDone  = false;
}

SimpleTestAmbassador::~SimpleTestAmbassador()
{
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// Synchronization Point Callbacks ///////////////////////
///////////////////////////////////////////////////////////////////////////////
void SimpleTestAmbassador::synchronizationPointRegistrationSucceeded(std::wstring const& label)
{
  std::wcout << L"Successfully registered sync point: " << label << std::endl;
}

void SimpleTestAmbassador::synchronizationPointRegistrationFailed(std::wstring const& label, SynchronizationPointFailureReason reason)
{
  std::wcout << L"Failed to register sync point: " << label << std::endl;
}

void SimpleTestAmbassador::announceSynchronizationPoint(std::wstring const& label, VariableLengthData const& theUserSuppliedTag)
{
  std::wcout << L"Synchronization point announced: " << label << std::endl;
  std::wstring compair = L"ReadyToRun";
  if (label ==  READY_TO_RUN)
  {
    this->isAnnouncedReadyToRun = true;
  }
  else if (label == ALL_DONE)
  {
    this->isAnnouncedAllDone = true;
  }
}

void SimpleTestAmbassador::federationSynchronized(
  std::wstring const& label,
  FederateHandleSet const& failedToSyncSet)
{
  std::wcout << L"Federation Synchronized: " << label << std::endl;
  if (label == READY_TO_RUN)
  {
    this->isReadyToRun = true;
  }
  else if (label == ALL_DONE)
  {
    this->allDone = true;
  }
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Time Callbacks ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SimpleTestAmbassador::timeRegulationEnabled(LogicalTime const& theFederateTime)
{
  this->isRegulating = true;
  this->federateTime = convertTime(theFederateTime);
}

void SimpleTestAmbassador::timeConstrainedEnabled(LogicalTime const& theFederateTime)
{
  this->isConstrained = true;
  this->federateTime = convertTime(theFederateTime);
}

void SimpleTestAmbassador::timeAdvanceGrant(LogicalTime const& theTime)
{
  this->isAdvancing = false;
  this->federateTime = convertTime(theTime);
  DebugPrintf("timeAdvanceGrant(%.9f)\n", this->federateTime);
}

//                                 //
// Reflect Attribute Value Methods //
//                                 //
void SimpleTestAmbassador::reflectAttributeValues(
  ObjectInstanceHandle theObject,
  AttributeHandleValueMap const& theAttributeValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  SupplementalReflectInfo theReflectInfo)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  std::wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass);
  std::wcout << ", sentOrder=" << sentOrder;
  std::wcout << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wstring strData(static_cast<const wchar_t*>(attr.second.data()), attr.second.size() / sizeof(wchar_t));
    std::wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr.first) << L" value=" << strData << std::endl;
  }
}

void SimpleTestAmbassador::reflectAttributeValues(
  ObjectInstanceHandle theObject,
  AttributeHandleValueMap const& theAttributeValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  SupplementalReflectInfo theReflectInfo)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  std::wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass);
  std::wcout << ", theTime=" << theTime.toString();
  std::wcout << ", sentOrder=" << sentOrder;
  std::wcout << ", receivedOrder=" << receivedOrder;
  std::wcout << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr.first) << std::endl;
  }
}

/**
 * receiveInteraction with some detailed output
 */
void SimpleTestAmbassador::receiveInteraction(
  InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  SupplementalReceiveInfo theReceiveInfo)
{
  std::wcout << L"Interaction Received:";
  /// print the handle
  std::wcout << " handle=" << theInteraction;
  /// print the tag
  // std::wstring data = variableLengthDataToWstring(theUserSuppliedTag);
  //std::wcout << ", tag=\"" << data << "\"";
  /// print the attribute information
  std::wcout << ", parameterCount=" << theParameterValues.size();
  std::wcout << ", sentOrder=" << sentOrder << std::endl;
  for (ParameterHandleValueMap::const_iterator i = theParameterValues.begin();
    i != theParameterValues.end(); i++)
  {
    /// print the parameter handle
    std::wcout << "\tparamHandle=" << i->first;
    /// print the parameter value
    std::wstring value = variableLengthDataToWstring(i->second);
    std::wcout << ", paramValue=\"" << value << L"\"" << std::endl;
  }
}

void SimpleTestAmbassador::receiveInteraction(
  InteractionClassHandle theInteraction,
  ParameterHandleValueMap const& theParameterValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  SupplementalReceiveInfo theReceiveInfo)
{
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
  std::wcout << std::endl;
  for (ParameterHandleValueMap::const_iterator i = theParameterValues.begin();
    i != theParameterValues.end(); i++)
  {
    /// print the parameter handle
    std::wcout << "\tparamHandle=" << i->first;
    /// print the parameter value
    std::wstring value = variableLengthDataToWstring(i->second);
    std::wcout << ", paramValue=\"" << value << L"\"" << std::endl;
  }
}

void SimpleTestAmbassador::receiveInteraction(
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
  for (ParameterHandleValueMap::const_iterator i = theParameterValues.begin();
    i != theParameterValues.end(); i++)
  {
    /// print the parameter handle
    std::wcout << "\tparamHandle=" << i->first;
    /// print the parameter value
    std::wstring value = variableLengthDataToWstring(i->second);
    std::wcout << ", paramValue=\"" << value << L"\"" << std::endl;
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////// Object Management Callbacks /////////////////////////
///////////////////////////////////////////////////////////////////////////////

//                         //
// Discover Object Methods //
//                         //
void SimpleTestAmbassador::discoverObjectInstance(ObjectInstanceHandle theObject,
  ObjectClassHandle theObjectClass,
  std::wstring const& theObjectInstanceName)
{
  std::wcout <<  __FUNCTIONW__ << L": "
    << " handle=" << theObject << L" instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", classHandle=" << rtiamb->getObjectClassName(theObjectClass)
    << L", name=" << theObjectInstanceName << std::endl;
}


void SimpleTestAmbassador::attributesInScope(ObjectInstanceHandle theObject,
  const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  std::wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}


void SimpleTestAmbassador::attributesOutOfScope(ObjectInstanceHandle theObject,
  const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  std::wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}

void SimpleTestAmbassador::provideAttributeValueUpdate(ObjectInstanceHandle theObject, AttributeHandleSet const& theAttributes,
  VariableLengthData const& theUserSuppliedTag)
{
  ObjectClassHandle theObjectClass = rtiamb->getKnownObjectClassHandle(theObject);
  std::wcout << __FUNCTIONW__ << L": "
    << L"instance=" << rtiamb->getObjectInstanceName(theObject)
    << L", class=" << rtiamb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributes)
  {
    std::wcout << L"    attribute=" << rtiamb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}


//                       //
// Remove Object Methods //
//                       //
void SimpleTestAmbassador::removeObjectInstance(
  ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  SupplementalRemoveInfo theRemoveInfo)
{
  std::wcout << L"Object Removed: handle=" << theObject << std::endl;
}

void SimpleTestAmbassador::removeObjectInstance(
  ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  SupplementalRemoveInfo theRemoveInfo)
{
  std::wcout << L"Object Removed: handle=" << theObject << std::endl;
}

void SimpleTestAmbassador::removeObjectInstance(
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
