
#include "ManagerFederate.h"
#include "MomFederation.h"
#include "MomFederate.h"

#include "RTI/RTIambassadorFactory.h"
#include "RTI/RTIambassador.h"
#include "RTI/LogicalTimeInterval.h"
#include "RTI/time/HLAfloat64Interval.h"
#include "RTI/time/HLAfloat64Time.h"

#include "dprintf.h"
#include "RTI/encoding/HLAvariableArray.h"
#include "RTI/encoding/HLAhandle.h"
#include "RTI/encoding/HLAfixedRecord.h"
#include "RTI1516ESimpleTestLib.h"
#include "emptyFOM.inc"
#include <chrono>
#include <iostream>

using namespace rti1516e;
using std::cout;
using std::cin;
using std::endl;

std::ostream& operator<<(std::ostream& os, OrderType orderType)
{
  switch (orderType)
  {
    case OrderType::RECEIVE:
      os << "RECEIVE";
      break;
    case OrderType::TIMESTAMP:
      os << "TIMESTAMP";
      break;
    default:
      os << "<invalid OrderType>";
      break;
  }
  return os;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// class ManagerFederate ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

std::wstring requests[]{
  //L"HLArequestPublications",
  L"HLArequestSubscriptions",
  //L"HLArequestObjectInstancesUpdated",
  //L"HLArequestObjectInstancesReflected",
  L"HLArequestUpdatesSent",
  //L"HLArequestReflectionsReceived",
  //L"HLArequestInteractionsSent",
  //L"HLArequestInteractionsReceived",
};
size_t numRequests = sizeof(requests) / sizeof(requests[0]);

ManagerFederate::ManagerFederate() : NullFederateAmbassador()
{
  //mHandle = std::make_unique<VRTNotificationHandle>();
}

ManagerFederate::~ManagerFederate()
{
}

void ManagerFederate::initialize(const std::string& address, const std::string& federationName, const std::string& timeImplName)
{
  mFederationName = to_wstring(federationName);
  ///
  /// 1. create the RTIambassador
  ///
  rti1516e::RTIambassadorFactory factory;
  this->mRtiAmb = factory.createRTIambassador();
  /// create the federate ambassador
  try
  {
    /// create the federate ambassador and connect to RTI
    mRtiAmb->connect(*this, rti1516e::HLA_EVOKED,  to_wstring(address));
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

  if (mFederationName.empty()) mFederationName = L"MomTestFederation";

  ///
  /// 2. create and join to the federation
  /// NOTE: some other federate may have already created the federation,
  ///       in that case, we'll just try and join it
  ///
  try
  {
    std::string emptyFomModule = std::string("data:text/plain,") + emptyFOM;

    mRtiAmb->createFederationExecution(mFederationName, to_wstring(emptyFomModule), to_wstring(timeImplName));
    DebugPrintf("Created Federation\n");
  }
  catch (FederationExecutionAlreadyExists exists)
  {
    std::cout << "Didn't create federation, it already existed" << std::endl;
  }
  /////////////////////////////
  /// 3. join the federation
  /////////////////////////////
  mFederateHandle = mRtiAmb->joinFederationExecution(std::wstring(), L"ManagerFederate", mFederationName);
  std::cout << "Joined Federation " << to_string(mFederationName) << " as " << to_string(mRtiAmb->getFederateName(mFederateHandle)) << std::endl;
  /// initialize the handles - have to wait until we are joined
  mom = std::make_shared<Mom>(mRtiAmb.get());
  mRtiAmb->enableAsynchronousDelivery();
  Mom::InteractionClassAccessor requestInteractions = mom->interactions[L"HLAinteractionRoot"][L"HLAmanager"][L"HLAfederate"][L"HLArequest"];
  for (std::wstring requestName : requests)
  {
    mRtiAmb->publishInteractionClass(requestInteractions[requestName]);
  }

  Mom::InteractionClassAccessor reportInteractions = mom->interactions[L"HLAinteractionRoot"][L"HLAmanager"][L"HLAfederate"][L"HLAreport"];
  mRtiAmb->subscribeInteractionClass(reportInteractions[L"HLAreportInteractionsSent"]);
  mRtiAmb->subscribeInteractionClass(reportInteractions[L"HLAreportInteractionsReceived"]);

  mRtiAmb->subscribeInteractionClass(reportInteractions[L"HLAreportObjectClassPublication"]);
  mRtiAmb->subscribeInteractionClass(reportInteractions[L"HLAreportObjectClassSubscription"]);
  mRtiAmb->subscribeInteractionClass(reportInteractions[L"HLAreportInteractionPublication"]);
  mRtiAmb->subscribeInteractionClass(reportInteractions[L"HLAreportInteractionSubscription"]);

  mRtiAmb->subscribeInteractionClass(reportInteractions[L"HLAreportObjectInstancesUpdated"]);
  mRtiAmb->subscribeInteractionClass(reportInteractions[L"HLAreportObjectInstancesReflected"]);
  mRtiAmb->subscribeInteractionClass(reportInteractions[L"HLAreportUpdatesSent"]);

  Mom::ObjectClassAccessor federationClass = mom->objects[L"HLAmanager"][L"HLAfederation"];
  Mom::ObjectClassAccessor federateClass = mom->objects[L"HLAmanager"][L"HLAfederate"];
  mRtiAmb->subscribeObjectClassAttributes(federationClass, federationClass.attributes);
  mRtiAmb->subscribeObjectClassAttributes(federateClass, federateClass.attributes);

  // TODO: better derive the decoder directly from the datatype given in the MOM tree
  // WARNING: parameter handles from different interactions are thrown together as keys into a single map -
  // the standard does not guarantee that all are distinct/unique (but it works in the current OpenRTI implementation)
  mDecoders[reportInteractions.parameters[L"HLAfederate"]] = [this](const VariableLengthData& v) { decodeFederateHandle(v); };
  mDecoders[reportInteractions[L"HLAreportInteractionsSent"].parameters[L"HLAinteractionCounts"]] = [this](const VariableLengthData& v) { decodeInteractionCounts(v); };
  mDecoders[reportInteractions[L"HLAreportInteractionsReceived"].parameters[L"HLAinteractionCounts"]] = [this](const VariableLengthData& v) { decodeInteractionCounts(v); };
  mDecoders[reportInteractions[L"HLAreportObjectInstancesUpdated"].parameters[L"HLAobjectInstanceCounts"]] = [this](const VariableLengthData& v) { decodeObjectCounts(v); };
  mDecoders[reportInteractions[L"HLAreportObjectInstancesReflected"].parameters[L"HLAobjectInstanceCounts"]] = [this](const VariableLengthData& v) { decodeObjectCounts(v); };
  mDecoders[reportInteractions[L"HLAreportUpdatesSent"].parameters[L"HLAupdateCounts"]] = [this](const VariableLengthData& v) { decodeObjectCounts(v); };
  mDecoders[reportInteractions[L"HLAreportInteractionPublication"].parameters[L"HLAinteractionClassList"]] = [this](const VariableLengthData& v) { decodeInteractionHandleList(v); };
  mDecoders[reportInteractions[L"HLAreportInteractionSubscription"].parameters[L"HLAinteractionClassList"]] = [this](const VariableLengthData& v) { decodeInteractionSubscriptionList(v); };
  mDecoders[reportInteractions[L"HLAreportObjectClassSubscription"].parameters[L"HLAnumberOfClasses"]] = [this](const VariableLengthData& v) { decodeCount(v); };
  mDecoders[reportInteractions[L"HLAreportObjectClassSubscription"].parameters[L"HLAobjectClass"]] = [this](const VariableLengthData& v) { decodeObjectClass(v); };
  mDecoders[reportInteractions[L"HLAreportObjectClassSubscription"].parameters[L"HLAattributeList"]] = [this](const VariableLengthData& v) { decodeAttributeList(v); };
}

/**
 * Everything will be handled here
 *
 * @param federateName How the federate is called in simulation
 * @return void
 */
void ManagerFederate::runFederate()
{
  //mRtiAmb->setNotificationHandle(mHandle.get());
  std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
  int currentRequest = 0;
  while (!_done)
  {
    mRtiAmb->evokeCallback(0.1);
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    int64_t milliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    if (milliSeconds >= 1000)
    {
      Mom::InteractionClassAccessor ic = mom->interactions[L"HLAinteractionRoot"][L"HLAmanager"][L"HLAfederate"][L"HLArequest"][requests[currentRequest]];
      if ( mom->getFederation() != nullptr)
      {
        auto federates = mom->getFederation()->GetFederatesInFederation();
        for (auto federateHandle : federates)
        {
          if (federateHandle.isValid())
          {
            ObjectInstanceHandle federateObject = mom->getFederation()->GetFederate(federateHandle)->GetInternalFederateObjectInstance();
            AttributeHandleSet attributes = mom->getFederation()->GetFederate(federateHandle)->GetKnownAttributes();
            mRtiAmb->requestAttributeValueUpdate(federateObject, attributes, VariableLengthData());
            ParameterHandleValueMap pv;
            VariableLengthData tag;
            ParameterHandle federateParam = ic.parameters[L"HLAfederate"];
            pv[federateParam] = HLAhandle(federateHandle).encode();
            mRtiAmb->sendInteraction(ic, pv, tag);
            std::cout << "sent " << to_string(mRtiAmb->getInteractionClassName(ic)) << " federate=" << to_string(federateHandle.toString()) << std::endl;
          }
          else
          {
            std::cout << "invalid federate=" << to_string(federateHandle.toString()) << std::endl;
          }
        }
        currentRequest = (currentRequest + 1) % numRequests;
      }
      else
      {
        std::cout << "federation not yet present " << std::endl;
      }
      lastTime = currentTime;
    }
  }

  ////////////////////////////////////
  // resign from the federation
  ////////////////////////////////////
  mRtiAmb->resignFederationExecution(DELETE_OBJECTS);
  cout << "Resigned from Federation" << endl;
  ////////////////////////////////////////
  // 12. try and destroy the federation
  ////////////////////////////////////////
  // NOTE: we won't die if we can't do this because other federates
  //       remain. in that case we'll leave it for them to clean up
  try
  {
    mRtiAmb->destroyFederationExecution(mFederationName);
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
  while (true)
    if (!mRtiAmb->evokeCallback(1.0))
      break;
  mRtiAmb->disconnect();
}

void ManagerFederate::connectionLost(std::wstring const & faultDescription)
{
  std::cout << __FUNCTION__ << std::endl;
  exit(1);
}

void ManagerFederate::reportFederationExecutions(FederationExecutionInformationVector const & theFederationExecutionInformationList)
{
  std::cout << __FUNCTION__ << std::endl;

  for (auto& entry : theFederationExecutionInformationList)
  {
    if (mFederationName.empty())
    {
      mFederationName = entry.federationExecutionName;
    }
    std::cout << "  " << to_string(entry.federationExecutionName) << std::endl;
  }
  federationsReported = true;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// Synchronization Point Callbacks ///////////////////////
///////////////////////////////////////////////////////////////////////////////
void ManagerFederate::synchronizationPointRegistrationSucceeded(std::wstring const& label)
{
  std::cout << "Successfully registered sync point: " << to_string(label) << std::endl;
}

void ManagerFederate::synchronizationPointRegistrationFailed(std::wstring const& label, SynchronizationPointFailureReason reason)
{
  std::cout << "Failed to register sync point: " << to_string(label) << std::endl;
}

void ManagerFederate::announceSynchronizationPoint(std::wstring const& label, VariableLengthData const& theUserSuppliedTag)
{
  std::wcout << L"Synchronization point announced: " << label << std::endl;
  mRtiAmb->synchronizationPointAchieved(label);
}

void ManagerFederate::federationSynchronized(
  std::wstring const& label,
  FederateHandleSet const& failedToSyncSet)
{
  std::cout << "Federation Synchronized: " << to_string(label) << std::endl;
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
  mom->reflectAttributeValues(theObject, theAttributeValues, theUserSuppliedTag, sentOrder, theType, theReflectInfo);
}

void ManagerFederate::decodeInteractionCounts(const VariableLengthData& value)
{
  HLAfixedRecord icCountProto;
  icCountProto.appendElement(HLAhandle(InteractionClassHandle()));
  icCountProto.appendElement(HLAinteger32BE());
  HLAvariableArray array(icCountProto);
  array.decode(value);
  for (size_t i=0; i<array.size(); i++)
  {
    const HLAfixedRecord& rec = dynamic_cast<const HLAfixedRecord&>(array.get(i));
    const HLAhandle& ic = dynamic_cast<const HLAhandle&>(rec.get(0));
    const HLAinteger32BE& count = dynamic_cast<const HLAinteger32BE&>(rec.get(1));
    std::cout << std::endl << "\t\t" << "interaction=" << to_string(mRtiAmb->getInteractionClassName(ic.getInteractionClassHandle())) << " count=" << count.get();
  }
}

void ManagerFederate::decodeObjectCounts(const VariableLengthData& value)
{
  HLAfixedRecord icCountProto;
  icCountProto.appendElement(HLAhandle(InteractionClassHandle()));
  icCountProto.appendElement(HLAinteger32BE());
  HLAvariableArray array(icCountProto);
  array.decode(value);
  for (size_t i=0; i<array.size(); i++)
  {
    const HLAfixedRecord& rec = dynamic_cast<const HLAfixedRecord&>(array.get(i));
    const HLAhandle& ic = dynamic_cast<const HLAhandle&>(rec.get(0));
    const HLAinteger32BE& count = dynamic_cast<const HLAinteger32BE&>(rec.get(1));
    std::cout << std::endl << "\t\t" << "objectclass=" << to_string(mRtiAmb->getObjectClassName(ic.getObjectClassHandle())) << " count=" << count.get();
  }
}


void ManagerFederate::decodeFederateHandle(const VariableLengthData& value)
{
  HLAhandle handle;
  handle.decode(value);
  std::cout << " federate=" << to_string(handle.getFederateHandle().toString()) << " " << to_string(mRtiAmb->getFederateName(handle.getFederateHandle()));
}

void ManagerFederate::decodeCount(const VariableLengthData& value)
{
  HLAinteger32BE count;
  count.decode(value);
  std::cout << " count=" << count.get();
}


void ManagerFederate::decodeInteractionHandleList(const VariableLengthData& value)
{
  HLAvariableArray handleList = HLAvariableArray(HLAhandle(InteractionClassHandle()));
  handleList.decode(value);
  for (size_t i=0;i<handleList.size();i++)
  {
    const HLAhandle& handle = dynamic_cast<const HLAhandle&>(handleList.get(i));
    std::cout  << std::endl << "\t\t" << "interaction=" << to_string(mRtiAmb->getInteractionClassName(handle.getInteractionClassHandle()));
  }
}

void ManagerFederate::decodeInteractionSubscriptionList(const VariableLengthData& value)
{
  HLAfixedRecord icCountProto;
  icCountProto.appendElement(HLAhandle(InteractionClassHandle()));
  icCountProto.appendElement(HLAboolean());
  HLAvariableArray array(icCountProto);
  array.decode(value);
  for (size_t i = 0; i < array.size(); i++)
  {
    const HLAfixedRecord& rec = dynamic_cast<const HLAfixedRecord&>(array.get(i));
    const HLAhandle& ic = dynamic_cast<const HLAhandle&>(rec.get(0));
    const HLAboolean& active = dynamic_cast<const HLAboolean&>(rec.get(1));
    std::cout << std::endl << "\t\t" << "interaction=" << to_string(mRtiAmb->getInteractionClassName(ic.getInteractionClassHandle())) << " active=" << active.get();
  }
}

void ManagerFederate::decodeObjectClass(const VariableLengthData& value)
{
  HLAhandle handle;
  handle.decode(value);
  std::cout << " objectClass=" << to_string(handle.getObjectClassHandle().toString()) << " " << to_string(mRtiAmb->getObjectClassName(handle.getObjectClassHandle()));
}

void ManagerFederate::decodeAttributeList(const VariableLengthData& value)
{
  HLAvariableArray handleArray = HLAvariableArray(HLAhandle());
  handleArray.decode(value);
  for (size_t i = 0; i < handleArray.size(); i++)
  {
    const HLAhandle& handle = dynamic_cast<const HLAhandle&>(handleArray.get(i));
    std::cout << std::endl << "\t\t" << " attribute=" << to_string(handle.getAttributeHandle().toString());
  }
}


void ManagerFederate::decodeBoolean(const VariableLengthData& value)
{
  HLAboolean decodedValue;
  decodedValue.decode(value);
  std::cout << " bool=" << decodedValue.get() ? "true" : "false";
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
  std::cout << "Interaction Received: " << to_string(mRtiAmb->getInteractionClassName(theInteraction));
  std::cout << ", parameterCount=" << theParameterValues.size();
  std::cout << ", sentOrder=" << sentOrder << std::endl;
  for (auto [p, v] : theParameterValues)
  {
    /// print the parameter handle
    std::wstring paramName = mRtiAmb->getParameterName(theInteraction, p);
    std::cout << "\tparam=" << to_string(paramName) << " value=" << v.size() << " bytes";
    auto decoderIter = mDecoders.find(p);
    if (decoderIter != mDecoders.end())
    {
      (decoderIter->second)(v);
    }
    std::wcout << std::endl;
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
  mom->discoverObjectInstance(theObject, theObjectClass, theObjectInstanceName);
}


void ManagerFederate::discoverObjectInstance(ObjectInstanceHandle theObject, ObjectClassHandle theObjectClass, std::wstring const& theObjectInstanceName, FederateHandle producingFederate)
{
  std::cout <<  __FUNCTION__ << ": "
    << " handle=" << to_string(theObject.toString()) << " instance=" << to_string(mRtiAmb->getObjectInstanceName(theObject))
    << ", classHandle=" << to_string(mRtiAmb->getObjectClassName(theObjectClass))
    << ", name=" << to_string(theObjectInstanceName)
    << ", producingFederate=" << to_string(producingFederate.toString())
    << std::endl;
  mom->discoverObjectInstance(theObject, theObjectClass, theObjectInstanceName);
}

void ManagerFederate::attributesInScope(ObjectInstanceHandle theObject,
  const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::cout << __FUNCTION__ << ": "
    << "instance=" << to_string(mRtiAmb->getObjectInstanceName(theObject))
    << ", class=" << to_string(mRtiAmb->getObjectClassName(theObjectClass)) << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::cout << "    attribute=" << to_string(mRtiAmb->getAttributeName(theObjectClass, attr)) << std::endl;
  }
}


void ManagerFederate::attributesOutOfScope(ObjectInstanceHandle theObject,
  const AttributeHandleSet& theAttributeValues)
{
  ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
  std::cout << __FUNCTION__ << ": "
    << "instance=" << to_string(mRtiAmb->getObjectInstanceName(theObject))
    << ", class=" << to_string(mRtiAmb->getObjectClassName(theObjectClass)) << std::endl;
  for (auto attr : theAttributeValues)
  {
    std::cout << "    attribute=" << to_string(mRtiAmb->getAttributeName(theObjectClass, attr)) << std::endl;
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
  std::cout << "Object Removed: handle=" << to_string(theObject.toString()) << std::endl;
  mom->removeObjectInstance(theObject);
}
