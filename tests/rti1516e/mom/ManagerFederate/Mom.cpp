#include "Mom.h"
#include "StringUtils.h"
#include <iostream>

void Mom::initialize(RTIambassador* rtiAmb)
{
  // Declaratively build the tree. The creator methods convert the provided names into a
  // {@link VersionedName} so that nodes can be resolved based on the naming conventions of all
  // HLA specification versions
  //
  // NOTE: Handles generated here will be consistent across all federations running in the RTI
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

  this->momTree = new MomHandleTree(objectRoot, interactionRoot);
  this->objectLookup = b.getObjectLookup();
  this->attributeLookup = b.getAttributeLookup();
  this->interactionLookup = b.getInteractionLookup();
  this->parameterLookup = b.getParameterLookup();
}

int Mom::getMomObjectClassHandle(std::wstring className)
{
  MomTreeNode* node = momTree->find(className, momTree->objectRoot);
  if (node != nullptr)
  {
    return node->getHandle();
  }
  else
  {
    return 0;
  }
}


int Mom::getMomAttributeHandle(int classHandle, std::wstring name)
{
  int attributeHandle = 0;

  MomTreeNode* objectNode = momTree->find(MomType::Object,
                                          classHandle,
                                          momTree->objectRoot);

  if (objectNode != nullptr)
  {
    MomTreeNode* attributeNode = momTree->find(name, objectNode);
    if (attributeNode != nullptr)
      attributeHandle = attributeNode->getHandle();
  }

  return attributeHandle;
}

std::wstring Mom::getMomAttributeName(rti1516e::AttributeHandle attributeHandle)
{
  std::wstring name;
  decltype(attributeLookup)::iterator iter = attributeLookup.find(attributeHandle);
  if (iter != attributeLookup.end())
  {
    MomTreeNode* attributeNode = iter->second;
    if (attributeNode != nullptr)
      name = attributeNode->getName();
  }
  return name;
}


int Mom::getMomInteractionHandle(std::wstring className)
{
  MomTreeNode* node = momTree->find(className, momTree->interactionRoot);
  if (node != nullptr)
  {
    return node->getHandle();
  }
  else
  {
    return 0;
  }
}


std::wstring Mom::getMomInteractionName(rti1516e::InteractionClassHandle handle, bool qualified)
{
  std::wstring name;
  decltype(interactionLookup)::iterator iter = interactionLookup.find(handle);
  if (iter != interactionLookup.end())
  {
    MomTreeNode* interactionNode = iter->second;

    if (interactionNode != nullptr)
    {
      if (qualified)
        name = interactionNode->getQualifiedName();
      else
        name = interactionNode->getName();
    }
  }
  return name;
}

std::wstring Mom::getMomParameterName(rti1516e::ParameterHandle parameterHandle)
{
  std::wstring name;
  decltype(parameterLookup)::iterator iter = parameterLookup.find(parameterHandle);
  if (iter != parameterLookup.end())
  {
    MomTreeNode* paramNode = iter->second;
    if (paramNode != nullptr)
      name = paramNode->getName();
  }
  return name;
}

std::wstring Mom::MomTreeNode::getQualifiedName()
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

Mom::MomTreeNode* Mom::MomHandleTree::find(std::wstring name, MomTreeNode* root)
{
  // Break the qualified name into tokens
  std::vector<std::wstring> nameTokens = tokenizeName(name);

  MomTreeNode* context = root;
  for (auto& matchName : nameTokens)
  {
    // The current token we are searching for
    //std::wstring matchName = nameTokens[i].toLowerCase();
    MomTreeNode* nextContext = nullptr;

    // Iterate over all child nodes in the current context, searching for the one that matches
    // the current token
    //std::list<MomTreeNode*>& children = context->getChildren();
    for (MomTreeNode* child : context->getChildren())
    {
      std::wstring childName = child->getName()/*.toLowerCase()*/;
      if (childName == matchName)
      {
        // This node matches the current token that we're looking for, so it becomes the
        // context for the next token
        nextContext = child;
        break;
      }
    }

    if (nextContext == nullptr)
    {
      // Next part in the chain wasn't found, so break from the loop, returning nullptr
      break;
    }
    else
    {
      context = nextContext;
    }
  }

  return context;
}


Mom::MomTreeNode* Mom::MomHandleTree::find(MomType type, int handle, MomTreeNode* root)
{
  // TODO This could be optimized by caching found results, or creating a lookup tree on
  // startup

  MomTreeNode* found = nullptr;
  if (root->type == type && root->handle == handle)
  {
    // This is the node!
    found = root;
  }
  else
  {
    // Recurse find over all child nodes
    for (MomTreeNode* child : root->children)
    {
      found = find(type, handle, child);
      if (found != nullptr)
        break;
    }
  }

  return found;
}

std::vector<std::wstring> Mom::MomHandleTree::tokenizeName(std::wstring name)
{
  return OpenRTI::split(name, L".");
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
    rti1516e::ObjectClassHandle handle = mRtiAmb->getObjectClassHandle(name);
    MomTreeNode* node = new MomTreeNode(name, MomType::Object, handle);

    this->addAndPush(node);
    objectLookup[handle] = node;
    return *this;
  }
  catch (const rti1516e::NameNotFound& e)
  {
    std::wcerr << L"object class name not found: " << name << L" : " << e.what() << std::endl;
    throw;
  }
  catch (const rti1516e::Exception& e)
  {
    std::wcerr << L"Exception: " << e.what() << std::endl;
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
    MomTreeNode* node = new MomTreeNode(name, MomType::Attribute, handle, datatype);
    this->add(node);
    this->attributeLookup[handle] = node;
    return *this;
  }
  catch (const rti1516e::NameNotFound& e)
  {
    std::wcerr << L"attribute name not found: " << name << L" : " << e.what() << std::endl;
    throw;
  }
  catch (const rti1516e::Exception& e)
  {
    std::wcerr << L"Exception: " << e.what() << std::endl;
  }
}

Mom::MomTreeNodeBuilder& Mom::MomTreeNodeBuilder::interaction(std::wstring name)
{
  try
  {
    rti1516e::InteractionClassHandle handle = mRtiAmb->getInteractionClassHandle(name);
    MomTreeNode* node = new MomTreeNode(name, MomType::Interaction, handle);

    this->addAndPush(node);
    this->interactionLookup[handle] = node;
    return *this;
  }
  catch (const rti1516e::NameNotFound& e)
  {
    std::wcerr << L"interaction class name not found: " << name << L" : " << e.what() << std::endl;
    throw;
  }
  catch (const rti1516e::Exception& e)
  {
    std::wcerr << L"Exception: " << e.what() << std::endl;
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
    rti1516e::ParameterHandle handle = mRtiAmb->getParameterHandle(mContextStack.front()->interactionClassHandle, name);
    MomTreeNode* node = new MomTreeNode(name, MomType::Parameter, handle, datatype);
    this->add(node);
    this->parameterLookup[handle] = node;
    return *this;
  }
  catch (const rti1516e::NameNotFound& e)
  {
    std::wcerr << L"parameter name \"" << name << "\" not found in interaction class " << mContextStack.front()->getQualifiedName() << L" : " << e.what() << std::endl;
    std::wcerr << L" in " << mRtiAmb->getInteractionClassName(mContextStack.front()->interactionClassHandle) << std::endl;
    throw;
  }
  catch (const rti1516e::Exception& e)
  {
    std::wcerr << L"Exception: " << e.what() << std::endl;
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
