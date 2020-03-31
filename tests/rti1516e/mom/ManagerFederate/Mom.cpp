#include "Mom.h"
#include "StringUtils.h"
#include "RTI/encoding/BasicDataElements.h"


#include <iostream>

void Mom::initialize(RTIambassador* rtiAmb)
{
  MomTreeNodeBuilder b(rtiAmb);

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
          .interaction(L"HLAreportMOMexception")       // TODO 1516e only?
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

  mMomHandleTree->publishLeafObjectClasses(rtiAmb, mMomHandleTree->objectRoot);
  mMomHandleTree->subscribeLeafObjectClasses(rtiAmb, mMomHandleTree->objectRoot);
  mMomHandleTree->publishLeafInteractionClasses(rtiAmb, mMomHandleTree->interactionRoot);
  mMomHandleTree->subscribeLeafInteractionClasses(rtiAmb, mMomHandleTree->interactionRoot);

}

void Mom::registerFederateObject(rti1516e::RTIambassador* rtiAmb, const std::wstring& federationName, const std::wstring& federateName)
{
  //ObjectClassHandle federationClass = rtiAmb->getObjectClassHandle(L"HLAobjectRoot.HLAmanager.HLAfederation");
  ObjectClassHandle federationClass = getObjects()[L"HLAmanager"][L"HLAfederation"];
  //ObjectClassHandle federateClass = rtiAmb->getObjectClassHandle(L"HLAobjectRoot.HLAmanager.HLAfederate");
  ObjectClassHandle federateClass = getObjects()[L"HLAmanager"][L"HLAfederate"];

  try
  {
    federationObject = rtiAmb->getObjectInstanceHandle(federationName);
  }
  catch (const ObjectInstanceNotKnown& e)
  {
    DebugPrintf("%s: %S (create our own)\n", __FUNCTION__, e.what().c_str());
    try
    {
      federationObject = rtiAmb->registerObjectInstance(federationClass, federationName, true);
    }
    catch (const ObjectInstanceNameInUse& e)
    {
      DebugPrintf("%s: %S\n", __FUNCTION__, e.what().c_str());
    }
    catch (const Exception& e)
    {
      DebugPrintf("%s: %S\n", __FUNCTION__, e.what().c_str());
      throw;
    }
  }

  try
  {
    federateObject = rtiAmb->registerObjectInstance(federateClass, federateName, true);
  }
  catch (const ObjectInstanceNameInUse& e)
  {
    DebugPrintf("%s: %S\n", __FUNCTION__, e.what().c_str());
  }
  catch (const Exception& e)
  {
    DebugPrintf("%s: %S\n", __FUNCTION__, e.what().c_str());
    throw;
  }
  AttributeHandleValueMap attributes;
  VariableLengthData tag;
  AttributeHandle h1 = getObjects()[L"HLAmanager"][L"HLAfederation"].attributes[L"HLAfederatesInFederation"];
  attributes[h1] = VariableLengthData();
  rtiAmb->updateAttributeValues(federateObject, attributes, tag);
}

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


Mom::MomHandleTree::MomHandleTree(MomTreeNode* objectRoot, MomTreeNode* interactionRoot)
{
  this->objectRoot = objectRoot;
  this->interactionRoot = interactionRoot;
}

std::vector<std::wstring> Mom::MomHandleTree::tokenizeName(std::wstring name)
{
  return OpenRTI::split(name, L".");
}

bool Mom::MomHandleTree::publishLeafInteractionClasses(RTIambassador* rtiAmb, MomTreeNode* root)
{
  if (root->type != MomType::Interaction) return false;
  bool publishedChild = false;
  // Recurse find over all child nodes
  for (MomTreeNode* child : root->children)
  {
    if (child->type == MomType::Interaction)
    {
      publishedChild = publishLeafInteractionClasses(rtiAmb, child);
    }
  }
  if (!publishedChild && root->interactionClassHandle.isValid())
  {
    rtiAmb->publishInteractionClass(root->interactionClassHandle);
    DebugPrintf("publishing interaction class %S\n",root->getQualifiedName().c_str());
  }
  return true;
}


bool Mom::MomHandleTree::subscribeLeafInteractionClasses(RTIambassador* rtiAmb, MomTreeNode* root)
{
  if (root->type != MomType::Interaction) return false;
  bool publishedChild = false;
  // Recurse find over all child nodes
  for (MomTreeNode* child : root->children)
  {
    if (child->type == MomType::Interaction)
    {
      publishedChild = subscribeLeafInteractionClasses(rtiAmb, child);
    }
  }
  if (!publishedChild && root->interactionClassHandle.isValid())
  {
    rtiAmb->subscribeInteractionClass(root->interactionClassHandle);
    DebugPrintf("subscribed interaction class %S\n", rtiAmb->getInteractionClassName(root->interactionClassHandle).c_str());
  }
  return true;
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

bool Mom::MomHandleTree::publishLeafObjectClasses(RTIambassador* rtiAmb, MomTreeNode* root)
{
  if (root->type != MomType::Object) return false;
  bool publishedChild = false;
  // Recurse find over all child nodes
  for (MomTreeNode* child : root->children)
  {
    if (child->type == MomType::Object)
    {
      publishedChild = publishLeafObjectClasses(rtiAmb, child);
    }
  }
  if (!publishedChild && root->objectClassHandle.isValid())
  {
    // we reached a leaf object class
    AttributeHandleSet attributes;
    getClassAttributes(attributes, root);
    rtiAmb->publishObjectClassAttributes(root->objectClassHandle, attributes);
    DebugPrintf("published object class %S (%d attributes)\n", root->getQualifiedName().c_str(), attributes.size());
  }
  return true;
}


bool Mom::MomHandleTree::subscribeLeafObjectClasses(RTIambassador* rtiAmb, MomTreeNode* root)
{
  if (root->type != MomType::Object) return false;
  bool publishedChild = false;
  // Recurse find over all child nodes
  for (MomTreeNode* child : root->children)
  {
    if (child->type == MomType::Object)
    {
      publishedChild = subscribeLeafObjectClasses(rtiAmb, child);
    }
  }
  if (!publishedChild && root->objectClassHandle.isValid())
  {
    // we reached a leaf object class
    AttributeHandleSet attributes;
    getClassAttributes(attributes, root);
    rtiAmb->subscribeObjectClassAttributes(root->objectClassHandle, attributes);
    DebugPrintf("subscribed object class %S (%d attributes)\n", root->getQualifiedName().c_str(), attributes.size());
  }
  return true;
}

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
    DebugPrintf("%s: object class %s not found: %s", __FUNCTION__, name.c_str(), e.what().c_str());
    throw;
  }
  catch (const rti1516e::Exception& e)
  {
    DebugPrintf("%s: Exception: %s", __FUNCTION__, e.what().c_str());
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
    DebugPrintf("%s: \"%S\" => %S\n", __FUNCTION__, name.c_str(), handle.toString().c_str());
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
    DebugPrintf("%s: Exception: %S", __FUNCTION__, e.what().c_str());
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
    DebugPrintf("%s: Exception: %S", __FUNCTION__, e.what().c_str());
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
    DebugPrintf("%s: Exception: %s", __FUNCTION__, e.what().c_str());
  }
}

IllegalStateException::IllegalStateException(std::string t) : std::exception(t.c_str())
{

}

IllegalStateException::IllegalStateException(std::wstring t) : std::exception(OpenRTI::ucsToUtf8(t).c_str())
{

}

IllegalArgumentException::IllegalArgumentException(std::string t) : std::exception(t.c_str())
{

}

IllegalArgumentException::IllegalArgumentException(std::wstring t) : std::exception(OpenRTI::ucsToUtf8(t).c_str())
{

}
