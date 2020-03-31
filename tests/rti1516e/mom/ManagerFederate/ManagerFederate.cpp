
#include <iostream>

#include "ManagerFederate.h"

#include "RTI/RTIambassadorFactory.h"
#include "RTI/RTIambassador.h"
#include "RTI/LogicalTimeInterval.h"
#include "RTI/time/HLAfloat64Interval.h"
#include "RTI/time/HLAfloat64Time.h"

#include "dprintf.h"

using namespace rti1516e;
using std::cout;
using std::cin;
using std::wcout;
using std::endl;


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
/////////////////////// class ManagerFederate ////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const wchar_t *federationName=L"Management";
const wchar_t *federateName=L"Manager";

ManagerFederate::ManagerFederate() : NullFederateAmbassador()
{
  mHandle = std::make_unique<VRTNotificationHandle>();
  this->federateTime      = 0.0;
  this->federateLookahead = 1.0;
  this->isRegulating  = false;
  this->isConstrained = false;
  this->isAdvancing   = false;
}

ManagerFederate::~ManagerFederate()
{
}

/**
 * Everything will be handled here
 *
 * @param federateName How the federate is called in simulation
 * @return void
 */
void ManagerFederate::runFederate()
{
  mRtiAmb->setNotificationHandle(mHandle.get());
  DebugPrintf("%s: TID=%d: starting loop\n", __FUNCTION__, ::GetCurrentThreadId());
  while (true)
  {
    mRtiAmb->evokeCallback(1.0);
  }

  ////////////////////////////////////
  // resign from the federation
  ////////////////////////////////////
  mRtiAmb->resignFederationExecution(NO_ACTION);
  cout << "Resigned from Federation" << endl;
  ////////////////////////////////////////
  // 12. try and destroy the federation
  ////////////////////////////////////////
  // NOTE: we won't die if we can't do this because other federates
  //       remain. in that case we'll leave it for them to clean up
  try
  {
    mRtiAmb->destroyFederationExecution(federationName);
    cout << "Destroyed Federation" << endl;
  }
  catch (FederationExecutionDoesNotExist dne)
  {
    cout << "No need to destroy federation, it doesn't exist" << endl;
  }
  catch (FederatesCurrentlyJoined fcj)
  {
    cout << "Didn't destroy federation, federates still joined" << endl;
  }
  mRtiAmb->disconnect();
}

void ManagerFederate::initialize(std::string address, std::string fom)
{
  ///
  /// 1. create the RTIambassador
  ///
  rti1516e::RTIambassadorFactory factory;
  this->mRtiAmb = factory.createRTIambassador();
  /// create the federate ambassador
  try
  {
    /// create the federate ambassador and connect to RTI
    mRtiAmb->connect(*this, rti1516e::HLA_EVOKED,  convertStringToWstring(address));
    printf("%s: connected to rtinode at %s\n", __FUNCTION__, address.c_str());
  }
  catch (ConnectionFailed e)
  {
    printf("%s: could not connect to %s: %S\n", __FUNCTION__, address.c_str(), e.what().c_str());
    return;
  }
  /// list existing federations
  try
  {
    /// create the federate ambassador and connect to RTI
    mRtiAmb->listFederationExecutions();
  }
  catch (ConnectionFailed e)
  {
    printf("%s: could not connect to %s: %S\n", __FUNCTION__, address.c_str(), e.what().c_str());
    return;
  }
  while (!federationsReported)
    mRtiAmb->evokeCallback(1);
  ///
  /// 2. create and join to the federation
  /// NOTE: some other federate may have already created the federation,
  ///       in that case, we'll just try and join it
  ///
  try
  {
    mRtiAmb->createFederationExecution(federationName, L"D:/vfs/CANoe/release/13.0/Projects_Source/OpenRTI-MOM/tests/rti1516e/fddget/fdd-1.xml", L"HLAinteger64Time");
    DebugPrintf("Created Federation\n");
  }
  catch (FederationExecutionAlreadyExists exists)
  {
    DebugPrintf("Didn't create federation, it already existed\n");
  }
  /////////////////////////////
  /// 3. join the federation
  /////////////////////////////
  mRtiAmb->joinFederationExecution(federateName, federationName);
  std::cout << "Joined Federation as " << federateName << std::endl;
  /// initialize the handles - have to wait until we are joined
  initializeHandles();
}



/**
 * This method will attempt to delete the object instance of the given
 * handle. We can only delete objects we created, or for which we own the
 * privilegeToDelete attribute.
 */
void ManagerFederate::deleteObject(ObjectInstanceHandle objectHandle)
{
  mRtiAmb->deleteObjectInstance(objectHandle, toVariableLengthData(L""));
}


/**
 * This method will get all the relevant handle information from the RTIambassador
 */
void ManagerFederate::initializeHandles()
{
  mom = new Mom(mRtiAmb.get());
  mom->registerFederateObject(mRtiAmb.get(), federateName, federationName);

  mFederationClassHandle = mRtiAmb->getObjectClassHandle(L"HLAobjectRoot.HLAmanager.HLAfederation");
  mFederateClassHandle = mRtiAmb->getObjectClassHandle(L"HLAobjectRoot.HLAmanager.HLAfederate");
  mFederationNameAttributeHandle = mRtiAmb->getAttributeHandle(mFederationClassHandle, L"HLAfederationName");
  mFederatesInFederationAttributeHandle = mRtiAmb->getAttributeHandle(mFederationClassHandle, L"HLAfederatesInFederation");

}

void ManagerFederate::waitForUser()
{
  std::cout << " >>>>>>>>>> Press Enter to Continue <<<<<<<<<<" << std::endl;
  std::string line;
  getline(std::cin, line);
}

double ManagerFederate::getFederateTime() const
{
  return federateTime;
}

double ManagerFederate::getLbts() const
{
  return (federateTime + federateLookahead);
}


void ManagerFederate::connectionLost(std::wstring const & faultDescription)
{
  std::wcout << __FUNCTIONW__ << std::endl;
}


void ManagerFederate::reportFederationExecutions(FederationExecutionInformationVector const & theFederationExecutionInformationList)
{
  std::wcout << __FUNCTIONW__ << std::endl;

  for (auto& entry : theFederationExecutionInformationList)
  {
    std::wcout << L"  " << entry.federationExecutionName << std::endl;
  }
  federationsReported = true;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// Synchronization Point Callbacks ///////////////////////
///////////////////////////////////////////////////////////////////////////////
void ManagerFederate::synchronizationPointRegistrationSucceeded(std::wstring const& label)
{
  std::wcout << L"Successfully registered sync point: " << label << std::endl;
}

void ManagerFederate::synchronizationPointRegistrationFailed(std::wstring const& label, SynchronizationPointFailureReason reason)
{
  std::wcout << L"Failed to register sync point: " << label << std::endl;
}

void ManagerFederate::federationSynchronized(
  std::wstring const& label,
  FederateHandleSet const& failedToSyncSet)
{
  std::wcout << L"Federation Synchronized: " << label << std::endl;
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Time Callbacks ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ManagerFederate::timeRegulationEnabled(LogicalTime const& theFederateTime)
{
  this->isRegulating = true;
  this->federateTime = convertTime(theFederateTime);
}

void ManagerFederate::timeConstrainedEnabled(LogicalTime const& theFederateTime)
{
  this->isConstrained = true;
  this->federateTime = convertTime(theFederateTime);
}

void ManagerFederate::timeAdvanceGrant(LogicalTime const& theTime)
{
  this->isAdvancing = false;
  this->federateTime = convertTime(theTime);
  DebugPrintf("timeAdvanceGrant(%.9f)\n", this->federateTime);
}

//                                 //
// Reflect Attribute Value Methods //
//                                 //
void ManagerFederate::reflectAttributeValues(
  ObjectInstanceHandle theObject,
  AttributeHandleValueMap const& theAttributeValues,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  TransportationType theType,
  SupplementalReflectInfo theReflectInfo)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::wcout << __FUNCTIONW__ << L": "
    << L"instance=" << mRtiAmb->getObjectInstanceName(theObject)
    << L", class=" << mRtiAmb->getObjectClassName(theObjectClass);
  std::wcout << ", sentOrder=" << sentOrder;
  std::wcout << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wstring strData(static_cast<const wchar_t*>(attr.second.data()), attr.second.size() / sizeof(wchar_t));
    std::wcout << L"    attribute=" << mRtiAmb->getAttributeName(theObjectClass, attr.first) << L" value=" << strData << std::endl;
  }
}

void ManagerFederate::reflectAttributeValues(
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
  std::wcout << __FUNCTIONW__ << L": "
    << L"instance=" << mRtiAmb->getObjectInstanceName(theObject)
    << L", class=" << mRtiAmb->getObjectClassName(theObjectClass);
  std::wcout << ", theTime=" << theTime.toString();
  std::wcout << ", sentOrder=" << sentOrder;
  std::wcout << ", receivedOrder=" << receivedOrder;
  std::wcout << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wcout << L"    attribute=" << mRtiAmb->getAttributeName(theObjectClass, attr.first) << std::endl;
  }
}

/**
 * receiveInteraction with some detailed output
 */
void ManagerFederate::receiveInteraction(
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

void ManagerFederate::receiveInteraction(
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

void ManagerFederate::receiveInteraction(
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
void ManagerFederate::discoverObjectInstance(ObjectInstanceHandle theObject,
  ObjectClassHandle theObjectClass,
  std::wstring const& theObjectInstanceName)
{
  std::wcout <<  __FUNCTIONW__ << L": "
    << " handle=" << theObject << L" instance=" << mRtiAmb->getObjectInstanceName(theObject)
    << L", classHandle=" << mRtiAmb->getObjectClassName(theObjectClass)
    << L", name=" << theObjectInstanceName
    << std::endl;
}


void ManagerFederate::discoverObjectInstance(ObjectInstanceHandle theObject, ObjectClassHandle theObjectClass, std::wstring const& theObjectInstanceName, FederateHandle producingFederate)
{
  std::wcout <<  __FUNCTIONW__ << L": "
    << " handle=" << theObject << L" instance=" << mRtiAmb->getObjectInstanceName(theObject)
    << L", classHandle=" << mRtiAmb->getObjectClassName(theObjectClass)
    << L", name=" << theObjectInstanceName
    << L", producingFederate=" << producingFederate.toString()
    << std::endl;
}

void ManagerFederate::attributesInScope(ObjectInstanceHandle theObject,
  const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::wcout << __FUNCTIONW__ << L": "
    << L"instance=" << mRtiAmb->getObjectInstanceName(theObject)
    << L", class=" << mRtiAmb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wcout << L"    attribute=" << mRtiAmb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}


void ManagerFederate::attributesOutOfScope(ObjectInstanceHandle theObject,
  const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::wcout << __FUNCTIONW__ << L": "
    << L"instance=" << mRtiAmb->getObjectInstanceName(theObject)
    << L", class=" << mRtiAmb->getObjectClassName(theObjectClass) << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::wcout << L"    attribute=" << mRtiAmb->getAttributeName(theObjectClass, attr) << std::endl;
  }
}

void ManagerFederate::provideAttributeValueUpdate(ObjectInstanceHandle theObject, AttributeHandleSet const& theAttributes,
  VariableLengthData const& theUserSuppliedTag)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::wcout << __FUNCTIONW__ << L": "
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
void ManagerFederate::removeObjectInstance(
  ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  SupplementalRemoveInfo theRemoveInfo)
{
  std::wcout << L"Object Removed: handle=" << theObject << std::endl;
}

void ManagerFederate::removeObjectInstance(
  ObjectInstanceHandle theObject,
  VariableLengthData const& theUserSuppliedTag,
  OrderType sentOrder,
  LogicalTime const& theTime,
  OrderType receivedOrder,
  SupplementalRemoveInfo theRemoveInfo)
{
  std::wcout << L"Object Removed: handle=" << theObject << std::endl;
}

void ManagerFederate::removeObjectInstance(
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
