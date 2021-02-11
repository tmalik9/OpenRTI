#include "Mom.h"
#include "MomFederate.h"
#include "MomFederation.h"
#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAvariableArray.h"
#include "RTI/encoding/HLAhandle.h"
#include "RTI1516ESimpleTestLib.h"
#include "dprintf.h"

#include <iostream>

#pragma region class Mom

void Mom::initialize()
{
  MomTreeNodeBuilder b(mRtiAmb);

  //
  // Objects
  //
  b.object(L"HLAobjectRoot")
    .object(L"HLAmanager")
      .object(L"HLAfederate")
        .attribute(L"HLAfederateHandle", L"HLAhandle")
        .attribute(L"HLAfederateName", L"HLAunicodeString")
        .attribute(L"HLAfederateType", L"HLAunicodeString")
        .attribute(L"HLAfederateHost", L"HLAunicodeString")
        .attribute(L"HLARTIversion", L"HLAunicodeString")
        .attribute(L"HLAFOMmoduleDesignatorList", L"HLAmoduleDesignatorList")
        .attribute(L"HLAtimeConstrained", L"HLAboolean")
        .attribute(L"HLAtimeRegulating", L"HLAboolean")
        .attribute(L"HLAasynchronousDelivery", L"HLAboolean")
        .attribute(L"HLAfederateState", L"HLAfederateState")
        .attribute(L"HLAtimeManagerState", L"HLAtimeState")
        .attribute(L"HLAlogicalTime", L"HLAlogicalTime")
        .attribute(L"HLAlookahead", L"HLAtimeInterval")
        .attribute(L"HLAGALT", L"HLAlogicalTime")
        .attribute(L"HLALITS", L"HLAlogicalTime")
        .attribute(L"HLAROlength", L"HLAcount")
        .attribute(L"HLATSOlength", L"HLAcount")
        .attribute(L"HLAreflectionsReceived", L"HLAcount")
        .attribute(L"HLAupdatesSent", L"HLAcount")
        .attribute(L"HLAinteractionsReceived", L"HLAcount")
        .attribute(L"HLAinteractionsSent", L"HLAcount")
        .attribute(L"HLAobjectInstancesThatCanBeDeleted", L"HLAcount")
        .attribute(L"HLAobjectInstancesUpdated", L"HLAcount")
        .attribute(L"HLAobjectInstancesReflected", L"HLAcount")
        .attribute(L"HLAobjectInstancesDeleted", L"HLAcount")
        .attribute(L"HLAobjectInstancesRemoved", L"HLAcount")
        .attribute(L"HLAobjectInstancesRegistered", L"HLAcount")
        .attribute(L"HLAobjectInstancesDiscovered", L"HLAcount")
        .attribute(L"HLAtimeGrantedTime", L"HLAmsec")
        .attribute(L"HLAtimeAdvancingTime", L"HLAmsec")
        .attribute(L"HLAconveyRegionDesignatorSets", L"HLAswitch")
        .attribute(L"HLAconveyProducingFederate", L"HLAswitch")
      .end()
      .object(L"HLAfederation")
        .attribute(L"HLAfederationName", L"HLAunicodeString")
        .attribute(L"HLAfederatesInFederation", L"HLAhandleList")
        .attribute(L"HLARTIversion", L"HLAunicodeString")
        .attribute(L"HLAMIMdesignator", L"HLAunicodeString")
        .attribute(L"HLAFOMmoduleDesignatorList", L"HLAmoduleDesignatorList")
        .attribute(L"HLAcurrentFDD", L"HLAunicodeString")
        .attribute(L"HLAtimeImplementationName", L"HLAunicodeString")
        .attribute(L"HLAlastSaveName", L"HLAunicodeString")
        .attribute(L"HLAlastSaveTime", L"HLAlogicalTime")
        .attribute(L"HLAnextSaveName", L"HLAunicodeString")
        .attribute(L"HLAnextSaveTime", L"HLAlogicalTime")
        //.attribute(L"HLAconveyRegionDesignatorSets", L"HLAswitch")
        .attribute(L"HLAautoProvide", L"HLAswitch")
      .end()
    .end();
  MomTreeNode* objectRoot = b.getContext();
  b.end();

  //
  // Interactions
  //
  b.interaction(L"HLAinteractionRoot")
    .interaction(L"HLAmanager")
      .interaction(L"HLAfederate")
        .parameter(L"HLAfederate", L"HLAhandle")
        .interaction(L"HLAadjust")
          .interaction(L"HLAsetTiming")
            .parameter(L"HLAreportPeriod", L"HLAseconds")
          .end()
          .interaction(L"HLAmodifyAttributeState")
            .parameter(L"HLAobjectInstance", L"HLAhandle")
            .parameter(L"HLAattribute", L"HLAhandle")
            .parameter(L"HLAattributeState", L"HLAownership")
          .end()
          .interaction(L"HLAsetServiceReporting")
            .parameter(L"HLAreportingState", L"HLAboolean")
          .end()
          .interaction(L"HLAsetExceptionReporting")
            .parameter(L"HLAreportingState", L"HLAboolean")
          .end()
          .interaction(L"HLAsetSwitches")
            //.parameter(L"HLAconveyRegionDesignatorSets", L"HLAswitch")
            //.parameter(L"HLAconveyProducingFederate", L"HLAswitch")
          .end()
        .end()
        .interaction(L"HLArequest")
          .interaction(L"HLArequestPublications").end()
          .interaction(L"HLArequestSubscriptions").end()
          .interaction(L"HLArequestObjectInstancesThatCanBeDeleted").end()
          .interaction(L"HLArequestObjectInstancesUpdated").end()
          .interaction(L"HLArequestObjectInstancesReflected").end()
          .interaction(L"HLArequestUpdatesSent").end()
          .interaction(L"HLArequestInteractionsSent").end()
          .interaction(L"HLArequestReflectionsReceived").end()
          .interaction(L"HLArequestInteractionsReceived").end()
          .interaction(L"HLArequestObjectInstanceInformation")
          .parameter(L"HLAobjectInstance", L"HLAhandle")
          .end()
          .interaction(L"HLArequestFOMmoduleData")
          .parameter(L"HLAFOMmoduleIndicator", L"HLAindex")
          .end()
        .end()
        .interaction(L"HLAreport")
          .interaction(L"HLAreportObjectClassPublication")
            .parameter(L"HLAnumberOfClasses", L"HLAcount")
            .parameter(L"HLAobjectClass", L"HLAhandle")
            .parameter(L"HLAattributeList", L"HLAhandleList")
          .end()
          .interaction(L"HLAreportInteractionPublication")
            .parameter(L"HLAinteractionClassList", L"HLAhandleList")
          .end()
          .interaction(L"HLAreportObjectClassSubscription")
            .parameter(L"HLAnumberOfClasses", L"HLAcount")
            .parameter(L"HLAobjectClass", L"HLAhandle")
            .parameter(L"HLAactive", L"HLAboolean")
            .parameter(L"HLAmaxUpdateRate", L"HLAupdateRateName")
            .parameter(L"HLAattributeList", L"HLAhandleList")
          .end()
          .interaction(L"HLAreportInteractionSubscription")
            .parameter(L"HLAinteractionClassList", L"HLAinteractionSubList")
          .end()
          .interaction(L"HLAreportObjectInstancesThatCanBeDeleted")
            .parameter(L"HLAobjectInstanceCounts", L"HLAobjectClassBasedCounts")
          .end()
          .interaction(L"HLAreportObjectInstancesUpdated")
            .parameter(L"HLAobjectInstanceCounts", L"HLAobjectClassBasedCounts")
          .end()
          .interaction(L"HLAreportObjectInstancesReflected")
            .parameter(L"HLAobjectInstanceCounts", L"HLAobjectClassBasedCounts")
          .end()
          .interaction(L"HLAreportUpdatesSent")
            .parameter(L"HLAtransportation", L"HLAtransportationName")
            .parameter(L"HLAupdateCounts", L"HLAobjectClassBasedCounts")
          .end()
          .interaction(L"HLAreportReflectionsReceived")
            .parameter(L"HLAtransportation", L"HLAtransportationName")
            .parameter(L"HLAreflectCounts", L"HLAobjectClassBasedCounts")
          .end()
          .interaction(L"HLAreportInteractionsSent")
            .parameter(L"HLAtransportation", L"HLAtransportationName")
            .parameter(L"HLAinteractionCounts", L"HLAinteractionCounts")
          .end()
          .interaction(L"HLAreportInteractionsReceived")
            .parameter(L"HLAtransportation", L"HLAtransportationName")
            .parameter(L"HLAinteractionCounts", L"HLAinteractionCounts")
          .end()
          .interaction(L"HLAreportObjectInstanceInformation")
            .parameter(L"HLAobjectInstance", L"HLAhandle")
            .parameter(L"HLAownedInstanceAttributeList", L"HLAhandleList")
            .parameter(L"HLAregisteredClass", L"HLAhandle")
            .parameter(L"HLAknownClass", L"HLAhandle")
          .end()
          //.interaction(L"Alert")
          //  .parameter(L"AlertSeverity", L"HLAunicodeString")
          //  .parameter(L"AlertDescription", L"HLAunicodeString")
          //  .parameter(L"AlertID", L"HLAunicodeString")
          //.end()
          .interaction(L"HLAreportException")
            .parameter(L"HLAservice", L"HLAunicodeString")
            .parameter(L"HLAexception", L"HLAunicodeString")
          .end()
          .interaction(L"HLAreportServiceInvocation")
            .parameter(L"HLAservice", L"HLAunicodeString")
            //.parameter(L"Initiator", L"HLAunicodeString")
            .parameter(L"HLAsuccessIndicator", L"HLAboolean")
            .parameter(L"HLAsuppliedArguments", L"HLAargumentList")
            //.parameter(L"SuppliedArgument1", L"HLAunicodeString")
            //.parameter(L"SuppliedArgument2", L"HLAunicodeString")
            //.parameter(L"SuppliedArgument3", L"HLAunicodeString")
            //.parameter(L"SuppliedArgument4", L"HLAunicodeString")
            //.parameter(L"SuppliedArgument5", L"HLAunicodeString")
            .parameter(L"HLAreturnedArguments", L"HLAargumentList")
            .parameter(L"HLAexception", L"HLAunicodeString")
            .parameter(L"HLAserialNumber", L"HLAcount")
          .end()
          .interaction(L"HLAreportMOMexception")
            .parameter(L"HLAservice", L"HLAunicodeString")
            .parameter(L"HLAexception", L"HLAunicodeString")
            .parameter(L"HLAparameterError", L"HLAboolean")
          .end()
          .interaction(L"HLAreportFederateLost")
            .parameter(L"HLAfederateName", L"HLAunicodeString")
            .parameter(L"HLAtimeStamp", L"HLAlogicalTime")
            .parameter(L"HLAfaultDescription", L"HLAunicodeString")
          .end()
          .interaction(L"HLAreportFOMmoduleData")
            .parameter(L"HLAFOMmoduleIndicator", L"HLAindex")
            .parameter(L"HLAFOMmoduleData", L"HLAunicodeString")
          .end()
        .end()
        .interaction(L"HLAservice")
          .interaction(L"HLAresignFederationExecution")
            .parameter(L"HLAresignAction", L"HLAresignAction")
          .end()
          .interaction(L"HLAsynchronizationPointAchieved")
            .parameter(L"HLAlabel", L"HLAunicodeString")
          .end()
          .interaction(L"HLAfederateSaveBegun").end()
          .interaction(L"HLAfederateSaveComplete")
            .parameter(L"HLAsuccessIndicator", L"HLAboolean")
          .end()
          .interaction(L"HLAfederateRestoreComplete")
            .parameter(L"HLAsuccessIndicator", L"HLAboolean")
          .end()
          .interaction(L"HLApublishObjectClassAttributes")
            .parameter(L"HLAobjectClass", L"HLAhandle")
            .parameter(L"HLAattributeList", L"HLAhandleList")
          .end()
          .interaction(L"HLAunpublishObjectClassAttributes")
            .parameter(L"HLAobjectClass", L"HLAhandle")
            .parameter(L"HLAattributeList", L"HLAhandleList")
          .end()
          .interaction(L"HLApublishInteractionClass")
            .parameter(L"HLAinteractionClass", L"HLAhandle")
          .end()
          .interaction(L"HLAunpublishInteractionClass")
            .parameter(L"HLAinteractionClass", L"HLAhandle")
          .end()
          .interaction(L"HLAsubscribeObjectClassAttributes")
            .parameter(L"HLAobjectClass", L"HLAhandle")
            .parameter(L"HLAattributeList", L"HLAhandleList")
            .parameter(L"HLAactive", L"HLAboolean")
          .end()
          .interaction(L"HLAunsubscribeObjectClassAttributes")
            .parameter(L"HLAobjectClass", L"HLAhandle")
            .parameter(L"HLAattributeList", L"HLAhandleList")
          .end()
          .interaction(L"HLAsubscribeInteractionClass")
            .parameter(L"HLAinteractionClass", L"HLAhandle")
            .parameter(L"HLAactive", L"HLAboolean")
          .end()
          .interaction(L"HLAunsubscribeInteractionClass")
            .parameter(L"HLAinteractionClass", L"HLAhandle")
          .end()
          .interaction(L"HLAdeleteObjectInstance")
            .parameter(L"HLAobjectInstance", L"HLAhandle")
            .parameter(L"HLAtag", L"HLAopaqueData")
            .parameter(L"HLAtimeStamp", L"HLAlogicalTime")
          .end()
          .interaction(L"HLAlocalDeleteObjectInstance")
            .parameter(L"HLAobjectInstance", L"HLAhandle")
          .end()
          .interaction(L"HLArequestAttributeTransportationTypeChange")
            .parameter(L"HLAobjectInstance", L"HLAhandle")
            .parameter(L"HLAattributeList", L"HLAhandleList")
            .parameter(L"HLAtransportation", L"HLAtransportationName")
          .end()
          .interaction(L"HLArequestInteractionTransportationTypeChange")
            .parameter(L"HLAinteractionClass", L"HLAhandle")
            .parameter(L"HLAtransportation", L"HLAtransportationName")
          .end()
          .interaction(L"HLAunconditionalAttributeOwnershipDivestiture")
            .parameter(L"HLAobjectInstance", L"HLAhandle")
            .parameter(L"HLAattributeList", L"HLAhandleList")
          .end()
          .interaction(L"HLAenableTimeRegulation")
            //.parameter(L"FederationTime", L"HLAlogicalTime")
            .parameter(L"HLAlookahead", L"HLAtimeInterval")
          .end()
          .interaction(L"HLAdisableTimeRegulation").end()
          .interaction(L"HLAenableTimeConstrained").end()
          .interaction(L"HLAdisableTimeConstrained").end()
          .interaction(L"HLAtimeAdvanceRequest")
            .parameter(L"HLAtimeStamp", L"HLAlogicalTime")
          .end()
          .interaction(L"HLAtimeAdvanceRequestAvailable")
            .parameter(L"HLAtimeStamp", L"HLAlogicalTime")
          .end()
          .interaction(L"HLAnextMessageRequest")
            .parameter(L"HLAtimeStamp", L"HLAlogicalTime")
          .end()
          .interaction(L"HLAnextMessageRequestAvailable")
            .parameter(L"HLAtimeStamp", L"HLAlogicalTime")
          .end()
          .interaction(L"HLAflushQueueRequest")
            .parameter(L"HLAtimeStamp", L"HLAlogicalTime")
          .end()
          .interaction(L"HLAenableAsynchronousDelivery").end()
          .interaction(L"HLAdisableAsynchronousDelivery").end()
          .interaction(L"HLAmodifyLookahead")
            .parameter(L"HLAlookahead", L"HLAtimeInterval")
          .end()
          .interaction(L"HLAchangeAttributeOrderType")
            .parameter(L"HLAobjectInstance", L"HLAhandle")
            .parameter(L"HLAattributeList", L"HLAhandleList")
            .parameter(L"HLAsendOrder", L"HLAorderType")
          .end()
          .interaction(L"HLAchangeInteractionOrderType")
            .parameter(L"HLAinteractionClass", L"HLAhandle")
            .parameter(L"HLAsendOrder", L"HLAorderType")
          .end()
        .end()
      .end()
      .interaction(L"HLAfederation")
        .interaction(L"HLAadjust")
          .interaction(L"HLAsetSwitches")
          .parameter(L"HLAautoProvide", L"HLAswitch")
          .end()
        .end()
        .interaction(L"HLArequest")
          .interaction(L"HLArequestSynchronizationPoints").end()
          .interaction(L"HLArequestSynchronizationPointStatus").end()
          .interaction(L"HLArequestFOMmoduleData")
            .parameter(L"HLAFOMmoduleIndicator", L"HLAindex")
          .end()
          .interaction(L"HLArequestMIMdata").end()
        .end()
        .interaction(L"HLAreport")
          .interaction(L"HLAreportSynchronizationPoints")
            .parameter(L"HLAsyncPoints", L"HLAsynchPointList")
          .end()
          .interaction(L"HLAreportSynchronizationPointStatus")
            .parameter(L"HLAsyncPointName", L"HLAunicodeString")
            .parameter(L"HLAsyncPointFederates", L"HLAsynchPointFederateList")
          .end()
          .interaction(L"HLAreportFOMmoduleData")
            .parameter(L"HLAFOMmoduleIndicator", L"HLAindex")
            .parameter(L"HLAFOMmoduleData", L"HLAunicodeString")
          .end()
          .interaction(L"HLAreportMIMdata")
            .parameter(L"HLAMIMdata", L"HLAunicodeString")
          .end()
        .end()
      .end()
    .end();

  MomTreeNode* interactionRoot = b.getContext();
  b.end();

  mMomHandleTree = new MomHandleTree(objectRoot, interactionRoot);
  objects = objectRoot;
  interactions = interactionRoot;

  mFederationObjectClass = objects[L"HLAmanager"][L"HLAfederation"];
  mFederateObjectClass = objects[L"HLAmanager"][L"HLAfederate"];
}

// NOTE: there is no way of getting hold of a federation handle, because the RTI ambassador uniquely belongs to a federation at all times
// One can only get a federate handle, valid in the joined federation
void Mom::OnFederationJoined(const std::wstring& federationName)
{
  while (!mFederationObject.isValid())
  {
    mRtiAmb->evokeCallback(0.1);
    try
    {
      mFederationObject = mRtiAmb->getObjectInstanceHandle(federationName);
    }
    catch (const ObjectInstanceNotKnown& e)
    {
      std::cout << __FUNCTION__ << ": Exception : " << e << std::endl;
    }
  }
  mFederation = std::make_shared<MomFederation>(shared_from_this(), mRtiAmb->getObjectInstanceName(mFederationObject), mFederationObject);
  AttributeHandleSet knownAttributes = mFederation->GetKnownAttributes();
  VariableLengthData tag;
  mRtiAmb->requestAttributeValueUpdate(mFederationObject, knownAttributes, tag);
}

std::shared_ptr<MomFederate> Mom::RegisterFederate(FederateHandle federateHandle, ObjectInstanceHandle federateObject)
{
  mKnownFederates.insert(federateHandle);
  mKnownFederateObjects.insert(federateObject);
  std::shared_ptr<MomFederate> federate = std::make_shared<MomFederate>(shared_from_this(), federateHandle, federateObject);
  federate->SetFederateHandle(federateHandle);
  federate->SetFederateName(mRtiAmb->getObjectInstanceName(federateObject));
  mFederation->AddFederate(federate);
  return federate;
}

void Mom::discoverObjectInstance(ObjectInstanceHandle theObject,
  ObjectClassHandle theObjectClass,
  std::wstring const& theObjectInstanceName)
{
  std::cout <<  __FUNCTION__ << ": "
    << " handle=" << to_string(theObject.toString())
    << ", classHandle=" << to_string(mRtiAmb->getObjectClassName(theObjectClass))
    << ", name=" << to_string(theObjectInstanceName)
    << std::endl;
  if (theObjectClass == mFederateObjectClass)
  {
    auto federateName = mRtiAmb->getObjectInstanceName(theObject);
    FederateHandle federateHandle;
    try {
      federateHandle = mRtiAmb->getFederateHandle(federateName);
      std::cout << "discovered federate " << to_string(federateName) << std::endl;
    }
    catch (const NameNotFound&)
    {
      federateHandle = mRtiAmb->getFederateHandle(L"HLA" + federateName);
      std::cout << "discovered federate " << "HLA" + to_string(federateName) << std::endl;
    }
    assert(federateHandle.isValid());
    auto federate = RegisterFederate(federateHandle, theObject);
    //{
    //  // Trigger federate to send it's information
    //  std::cout << __FUNCTION__ << ": Trigger attribute value update for federate object " << to_string(federateName) << std::endl;
    //  AttributeHandleSet knownAttributes = federate->GetKnownAttributes();
    //  VariableLengthData tag;
    //  mRtiAmb->requestAttributeValueUpdate(theObject, knownAttributes, tag);
    //}

    // Trigger federation to update it's information
    //if (mFederationObject.isValid())
    //{
    //  auto federationName = mRtiAmb->getObjectInstanceName(mFederationObject);
    //  std::cout << __FUNCTION__ << ": Trigger attribute value update for federation object" << to_string(federationName) << std::endl;
    //  AttributeHandleSet knownAttributes = mFederation->GetKnownAttributes();
    //  VariableLengthData tag;
    //  mRtiAmb->requestAttributeValueUpdate(mFederationObject, knownAttributes, tag);
    //}
  }
  else if (theObjectClass == mFederationObjectClass)
  {
    std::cout << "discovered federation object" << std::endl;
    // Trigger federation to send it's information
    auto federationName = mRtiAmb->getObjectInstanceName(theObject);
    mFederationObject = theObject;
    mFederation = std::make_shared<MomFederation>(shared_from_this(), mRtiAmb->getObjectInstanceName(mFederationObject), mFederationObject);
    //AttributeHandleSet knownAttributes = mFederation->GetKnownAttributes();
    //VariableLengthData tag;
    //std::cout << __FUNCTION__ << ": Trigger attribute value update for federation object " << to_string(federationName) << std::endl;
    //mRtiAmb->requestAttributeValueUpdate(mFederationObject, knownAttributes, tag);
  }
}

void Mom::removeObjectInstance(ObjectInstanceHandle theObject)
{
  std::cout << "Object Removed: handle=" << to_string(theObject.toString()) << std::endl;
  auto objectIter = mFederatesByObject.find(theObject);
  if (objectIter != mFederatesByObject.end())
  {
    auto federate = objectIter->second;
    assert(mFederation != nullptr);
    mFederation->RemoveFederate(federate);
    mFederatesByObject.erase(objectIter);
    federate.reset();
  }
}

void Mom::reflectAttributeValues(ObjectInstanceHandle theObject,
                                 AttributeHandleValueMap const& theAttributeValues,
                                 VariableLengthData const& theUserSuppliedTag,
                                 OrderType sentOrder, TransportationType theType,
                                 SupplementalReflectInfo theReflectInfo)
{
  std::cout << ">>> " << __FUNCTION__ << std::endl;

  try
  {
    ObjectClassHandle theObjectClass = mRtiAmb->getKnownObjectClassHandle(theObject);
    if (theObjectClass == mFederationObjectClass)
    {
      mFederation->SetAttributeValues(theAttributeValues);
      std::cout << "HLAfederationName=" << to_string(mFederation->GetFederationName()) << std::endl;
      std::cout << "HLAfederatesInFederation=" << std::endl;
      auto federates = mFederation->GetFederatesInFederation();
      for (auto federate : federates)
      {
        std::cout << "\t" << to_string(federate.toString());
        if (federate.isValid())
        {
          try
          {
            std::cout << " " << to_string(mRtiAmb->getFederateName(federate));
          }
          catch (const Exception& e)
          {
            std::cout << " " << e;
          }
        }
        std::wcout << std::endl;
      }
      std::cout << "HLAMIMdesignator=" << to_string(mFederation->GetMIMdesignator()) << std::endl;
      auto designators = mFederation->GetFOMmoduleDesignatorList();
      std::cout << "HLAFOMmoduleDesignatorList=" << std::endl;
      for (auto designator : designators)
      {
        std::cout << "\t" << to_string(designator) << std::endl;
      }
      std::cout << "HLAMIMdesignator=" << to_string(mFederation->GetCurrentFDD()) << std::endl;
    }
    else if (theObjectClass == mFederateObjectClass)
    {
      if (mFederation != nullptr)
      {
        auto federate = mFederation->GetFederate(theObject);
        federate->SetAttributeValues(theAttributeValues);
        std::cout << "HLAfederateName=" << to_string(federate->GetFederateName()) << std::endl;
        std::cout << "HLAfederateHandle=" << to_string(federate->GetFederateHandle().toString()) << std::endl;
        std::cout << "HLAfederateType=" << to_string(federate->GetFederateType()) << std::endl;
        std::cout << "HLAfederateHost=" << to_string(federate->GetFederateHost()) << std::endl;

        std::cout << "HLAtimeConstrained=" << federate->GetTimeConstrained() << std::endl;
        std::cout << "HLAtimeregulating=" << federate->GetTimeRegulating() << std::endl;
        std::cout << "HLAupdatesSent=" << federate->GetUpdatesSent() << std::endl;
        std::cout << "HLAreflectionsReceived=" << federate->GetReflectionsReceived() << std::endl;
      }
    }
  }

  catch (const Exception& e)
  {
    std::cout << __FUNCTION__ << ": Exception : " << e << std::endl;
  }
  std::cout << "<<< " << __FUNCTION__ << std::endl;
}

#pragma endregion

#pragma region class Mom::MomTreeNode

std::wstring Mom::MomTreeNode::getQualifiedName() const
{
  std::wstring qualifiedName = this->getName();
  if (this->parent != nullptr)
    qualifiedName = this->parent->getQualifiedName() + L"." + qualifiedName;

  return qualifiedName;
}


void Mom::MomTreeNode::addChild(MomTreeNode* node)
{
  if (node->parent == nullptr)
  {
    node->parent = this;
    this->children.push_back(node);
  }
  else
  {
    throw IllegalArgumentException(node->getName() + L" already has a parent");
  }
}
#pragma endregion

#pragma region class MomHandleTree

Mom::MomHandleTree::MomHandleTree(MomTreeNode* objectRoot, MomTreeNode* interactionRoot)
{
  this->objectRoot = objectRoot;
  this->interactionRoot = interactionRoot;
}

std::vector<std::wstring> Mom::MomHandleTree::tokenizeName(std::wstring name)
{
  return split(name, L".");
}

void Mom::MomHandleTree::getClassAttributes(AttributeHandleSet& attributes, MomTreeNode* objectClassNode)
{
  for (MomTreeNode* child : objectClassNode->children)
  {
    if (child->type == MomType::Attribute)
    {
      assert(child->attributeHandle.isValid());
      attributes.insert(child->attributeHandle);
    }
  }
  if (objectClassNode->parent != nullptr)
  {
    getClassAttributes(attributes, objectClassNode->parent);
  }
}

void Mom::MomHandleTree::getClassAttributesValueVector(AttributeHandleValueMap& attributes, MomTreeNode* objectClassNode)
{
  for (MomTreeNode* child : objectClassNode->children)
  {
    if (child->type == MomType::Attribute)
    {
      assert(child->attributeHandle.isValid());
      attributes.insert(std::make_pair(child->attributeHandle, VariableLengthData()));
    }
  }
  if (objectClassNode->parent != nullptr)
  {
    getClassAttributesValueVector(attributes, objectClassNode->parent);
  }
}

#pragma endregion

#pragma region MomTreeNodeBuilder
// class MomTreeNodeBuilder
void Mom::MomTreeNodeBuilder::add(MomTreeNode* newNode)
{
  if (!this->mContextStack.empty())
  {
    MomTreeNode* currentContext = this->mContextStack.front();
    currentContext->addChild(newNode);
  }
  else
  {
    throw new IllegalStateException("no context to add to");
  }
}

void Mom::MomTreeNodeBuilder::addAndPush(MomTreeNode* newNode)
{
  if (!this->mContextStack.empty())
  {
    MomTreeNode* currentContext = this->mContextStack.front();
    currentContext->addChild(newNode);
  }

  this->mContextStack.push_front(newNode);
}

Mom::MomTreeNodeBuilder& Mom::MomTreeNodeBuilder::object(std::wstring name)
{
  try
  {
    std::wstring fqName;
    if (!mContextStack.empty())
      fqName = mContextStack.front()->getQualifiedName() + L"." + name;
    else
      fqName = name;
    rti1516e::ObjectClassHandle handle = mRtiAmb->getObjectClassHandle(fqName);
    if (!handle.isValid())
      throw NameNotFound(name.c_str());
    MomTreeNode* node = new MomTreeNode(name, MomType::Object, handle);

    this->addAndPush(node);
    return *this;
  }
  catch (const rti1516e::NameNotFound& e)
  {
    std::cout << __FUNCTION__ << "object class " << to_string(name) << " not found: " << e << std::endl;
    throw;
  }
  catch (const rti1516e::Exception& e)
  {
    std::cout << __FUNCTION__ << ": Exception : " << e << std::endl;
    throw;
  }
}

Mom::MomTreeNodeBuilder& Mom::MomTreeNodeBuilder::attribute(std::wstring name, std::wstring datatype)
{
  if (mContextStack.size() == 0)
    throw IllegalStateException("attribute cannot be root node");

  if (mContextStack.front()->type != MomType::Object)
    throw IllegalStateException("attribute can only be added to an object");

  try
  {
    rti1516e::AttributeHandle handle = mRtiAmb->getAttributeHandle(mContextStack.front()->objectClassHandle, name);
    //DebugPrintf("%s: \"%S\" => %S\n", __FUNCTION__, name.c_str(), handle.toString().c_str());
    if (!handle.isValid())
      throw NameNotFound(name.c_str());
    MomTreeNode* node = new MomTreeNode(name, MomType::Attribute, handle, datatype);
    this->add(node);
    return *this;
  }
  catch (const rti1516e::NameNotFound& e)
  {
    DebugPrintf("%s: attribute %s not found: %S", __FUNCTION__, name.c_str(), e.what().c_str());
    throw;
  }
  catch (const rti1516e::Exception& e)
  {
    std::cout << __FUNCTION__ << ": Exception : " << e << std::endl;
    throw;
  }
}

Mom::MomTreeNodeBuilder& Mom::MomTreeNodeBuilder::interaction(std::wstring name)
{
  try
  {
    std::wstring fqName;
    if (!mContextStack.empty())
      fqName = mContextStack.front()->getQualifiedName() + L"." + name;
    else
      fqName = name;
    rti1516e::InteractionClassHandle handle = mRtiAmb->getInteractionClassHandle(name);
    if (!handle.isValid())
      throw NameNotFound(name.c_str());
    MomTreeNode* node = new MomTreeNode(name, MomType::Interaction, handle);

    this->addAndPush(node);
    return *this;
  }
  catch (const rti1516e::NameNotFound& e)
  {
    DebugPrintf("%s: interaction class %S not found: %S", __FUNCTION__, name.c_str(), e.what().c_str());
    throw;
  }
  catch (const rti1516e::Exception& e)
  {
    std::cout << __FUNCTION__ << ": Exception : " << e << std::endl;
    throw;
  }
}

Mom::MomTreeNodeBuilder& Mom::MomTreeNodeBuilder::parameter(std::wstring name, std::wstring datatype)
{
  if (mContextStack.size() == 0)
    throw new IllegalStateException("parameter cannot be root node");

  if (mContextStack.front()->type != MomType::Interaction)
    throw new IllegalStateException("parameter can only be added to an interaction");

  try
  {
    InteractionClassHandle interactionClass = mContextStack.front()->interactionClassHandle;
    rti1516e::ParameterHandle handle = mRtiAmb->getParameterHandle(mContextStack.front()->interactionClassHandle, name);
    assert(handle.isValid());
    MomTreeNode* node = new MomTreeNode(name, MomType::Parameter, handle, datatype);
    this->add(node);
    return *this;
  }
  catch (const rti1516e::NameNotFound& e)
  {
    DebugPrintf("%s: parameter %s not found: %S", __FUNCTION__, name.c_str(), e.what().c_str());
    DebugPrintf("%s:  in interaction class %S\n", __FUNCTION__, mRtiAmb->getInteractionClassName(mContextStack.front()->interactionClassHandle).c_str());
    throw;
  }
  catch (const rti1516e::Exception& e)
  {
    std::cout << __FUNCTION__ << ": Exception : " << e << std::endl;
    throw;
  }
}
#pragma endregion

#pragma region tree builder exceptions
IllegalStateException::IllegalStateException(std::string t) : std::logic_error(t.c_str())
{

}

IllegalStateException::IllegalStateException(std::wstring t) : std::logic_error(to_string(t).c_str())
{

}

IllegalArgumentException::IllegalArgumentException(std::string t) : std::logic_error(t.c_str())
{

}

IllegalArgumentException::IllegalArgumentException(std::wstring t) : std::logic_error(to_string(t).c_str())
{

}

#pragma endregion
