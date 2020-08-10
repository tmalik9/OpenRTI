

#include "DebugNew.h"
#include "MomInteractions.h"
#include "MomManager.h"
#include "MomTypes.h"
#include "MomFederateMetrics.h"
#include "MomFederate.h"
#include <cassert>
#include "dprintf.h"

namespace OpenRTI {

MomInteractionHandler::MomInteractionHandler(std::shared_ptr<MomManager> mom) : mMomManager(mom)
{
  MomManager::InteractionClassAccessor request = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLArequest"];
  mRequestFederateParameter = request.parameters["HLAfederate"];
  MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"];
  mResponseFederateParameter = response.parameters["HLAfederate"];
}

FederateHandle MomInteractionHandler::getFederateHandleFromParameters(const ParameterHandleValueMap& requestParameters)
{
  HLAhandle handle = HLAhandle(FederateHandle());
  handle.decode(requestParameters.at(mRequestFederateParameter));
  return handle.getFederateHandle();
}

FederateHandle MomInteractionHandler::getFederateHandleFromResponse(const ParameterHandleValueMap& responseParameters)
{
  HLAhandle handle = HLAhandle(FederateHandle());
  handle.decode(responseParameters.at(mResponseFederateParameter));
  return handle.getFederateHandle();
}

std::shared_ptr<MomFederate> MomInteractionHandler::getFederateFromParameters(const ParameterHandleValueMap& requestParameters)
{
  HLAhandle handle = HLAhandle(FederateHandle());
  handle.decode(requestParameters.at(mRequestFederateParameter));
  if (handle.getFederateHandle().valid())
  {
    return mMomManager->getFederate(handle.getFederateHandle());
  }
  else
  {
    DebugPrintf("%s: invalid HLAfederate\n");
    return std::shared_ptr<MomFederate>();
  }
}

// default implementation - return one interaction per predefined response interaction
MomInteractionHandler::ResponseList MomInteractionHandler::operator()(const ParameterHandleValueMap& requestParameters)
{
  ResponseList responses;
  auto federate = getFederateFromParameters(requestParameters);
  if (federate == nullptr)
  {
    DebugPrintf("federate not known\n");
    return responses;
  }
  for (auto [interaction, pv] : mResponseInteractions)
  {
    for (auto& [parameter, value] : pv)
    {
      assert(parameter.valid());
      // NOTE: value is a reference to the in-place parameter value in the ParameterHandleValueMap
      value = mParameterGetters[parameter]();
    }
    responses.push_back(std::make_pair(interaction, pv));
  }
  return responses;
}


InteractionHandlerFederateRequestPublications::InteractionHandlerFederateRequestPublications(std::shared_ptr<MomManager> mom)
  : MomInteractionHandler(mom)
{
  MomManager::InteractionClassAccessor request = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLArequest"]["HLArequestPublications"];
  mRequestInteractionHandle = request;
  mRequestFederateParameter = request.parameters["HLAfederate"];
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportObjectClassPublication"];
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAnumberOfClasses"];
    mParameterGetters[p1] = [this]{ return mMomManager->getFederateMetrics()->getNumberOfPublishedClasses(); };
    _classHandleParameter = response.parameters["HLAobjectClass"];
    mParameterGetters[_classHandleParameter] = [] { return VariableLengthData(); };
    _attributeListParameter = response.parameters["HLAattributeList"];
    mParameterGetters[_attributeListParameter] = [] { return VariableLengthData(); };
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue },
      { _classHandleParameter, kEmptyValue },
      { _attributeListParameter, kEmptyValue}
    };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportInteractionPublication"];
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAinteractionClassList"];
    mParameterGetters[p1] = [this] { return mMomManager->getFederateMetrics()->getInteractionPublications(); };
    VariableLengthData empty;
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue }
    };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
}

InteractionHandlerFederateRequestPublications::ResponseList
InteractionHandlerFederateRequestPublications::operator()(const ParameterHandleValueMap& requestParameters)
{
  ResponseList responses;
  {
    // HLAreportObjectClassPublication: Need to iterate over all object class entries in MomFederateMetrics
    // and produce one response per object class.
    auto[interaction, pv] = mResponseInteractions[0];
    for (auto& [parameter, value] : pv)
    {
      assert(parameter.valid());
      value = mParameterGetters[parameter]();
    }
    auto& publications = mMomManager->getFederateMetrics()->getObjectClassPublications();
    for (auto [objectClassHandle, attributes] : publications)
    {
      pv[_classHandleParameter] = HLAhandle(objectClassHandle).encode();
      HLAvariableArray attrHandles = HLAvariableArray(HLAhandle(AttributeHandle()));
      for (auto attr : attributes)
      {
        attrHandles.addElement(HLAhandle(attr));
      }
      pv[_attributeListParameter] = attrHandles.encode();
      responses.push_back(std::make_pair(interaction, pv));
    }
  }
  {
    // HLAreportInteractionPublication - standard
    auto[interaction, pv] = mResponseInteractions[1];
    for (auto& [parameter, value] : pv)
    {
      assert(parameter.valid());
      value = mParameterGetters[parameter]();
    }
    responses.push_back(std::make_pair(interaction, pv));
  }
  return responses;
}

InteractionHandlerFederateRequestSubscriptions::InteractionHandlerFederateRequestSubscriptions(std::shared_ptr<MomManager> mom)
  : MomInteractionHandler(mom)
{
  MomManager::InteractionClassAccessor request = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLArequest"]["HLArequestSubscriptions"];
  mRequestInteractionHandle = request;
  mRequestFederateParameter = request.parameters["HLAfederate"];
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportObjectClassSubscription"];
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAnumberOfClasses"];
    mParameterGetters[p1] = [this]{ return mMomManager->getFederateMetrics()->getNumberOfSubscribedClasses(); };
    assert(p1.valid());
    _classHandleParameter = response.parameters["HLAobjectClass"];
    assert(_classHandleParameter.valid());
    mParameterGetters[_classHandleParameter] = [] { return VariableLengthData(); };
    _activeSubscriptionParameter = response.parameters["HLAactive"];
    assert(_activeSubscriptionParameter.valid());
    mParameterGetters[_activeSubscriptionParameter] = [] { return VariableLengthData(); };
    ParameterHandle p4 = response.parameters["HLAmaxUpdateRate"];
    assert(p4.valid());
    mParameterGetters[p4] = [] { return VariableLengthData(); };
    _attributeListParameter = response.parameters["HLAattributeList"];
    assert(_attributeListParameter.valid());
    mParameterGetters[_attributeListParameter] = [] { return VariableLengthData(); };
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue },
      { _classHandleParameter, kEmptyValue },
      { _activeSubscriptionParameter, kEmptyValue },
      { p4, kEmptyValue },
      { _attributeListParameter, kEmptyValue}
    };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportInteractionSubscription"];
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAinteractionClassList"];
    assert(p1.valid());
    mParameterGetters[p1] = [this] { return mMomManager->getFederateMetrics()->getInteractionSubscriptions(); };
    VariableLengthData empty;
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue }
    };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
}

MomInteractionHandler::ResponseList InteractionHandlerFederateRequestSubscriptions::operator()(const ParameterHandleValueMap& requestParameters)
{
  ResponseList responses;
  {
    // HLAreportObjectClassSubscription: Need to iterate over all object class entries in MomFederateMetrics
    // and produce one response per object class.
    auto[interaction, pv] = mResponseInteractions[0];
    for (auto& [parameter, value] : pv)
    {
      assert(parameter.valid());
      value = mParameterGetters[parameter]();
    }
    auto& subscriptions = mMomManager->getFederateMetrics()->getObjectClassSubscriptions();
    for (auto [objectClassHandle, subscription] : subscriptions)
    {
      auto& [attributes, active] = subscription;
      pv[_classHandleParameter] = HLAhandle(objectClassHandle).encode();
      HLAvariableArray attrHandles = HLAvariableArray(HLAhandle(AttributeHandle()));
      for (auto attr : attributes)
      {
        attrHandles.addElement(HLAhandle(attr));
      }
      pv[_activeSubscriptionParameter] = HLAboolean(active).encode();
      pv[_attributeListParameter] = attrHandles.encode();
      responses.push_back(std::make_pair(interaction, pv));
    }
  }
  {
    // HLAreportInteractionSubscription - standard
    auto[interaction, pv] = mResponseInteractions[1];
    for (auto& [parameter, value] : pv)
    {
      assert(parameter.valid());
      value = mParameterGetters[parameter]();
    }
    responses.push_back(std::make_pair(interaction, pv));
  }
  return responses;
}

InteractionHandlerFederateRequestObjectInstancesUpdated::InteractionHandlerFederateRequestObjectInstancesUpdated(std::shared_ptr<MomManager> mom)
  : MomInteractionHandler(mom)
{
  MomManager::InteractionClassAccessor request = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLArequest"]["HLArequestObjectInstancesUpdated"];
  mRequestInteractionHandle = request;
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportObjectInstancesUpdated"];
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAobjectInstanceCounts"];
    assert(p1.valid());
    mParameterGetters[p1] = [this] { return mMomManager->getFederateMetrics()->getObjectInstancesUpdated(); };
    VariableLengthData empty;
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue },
    };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
}

InteractionHandlerFederateRequestObjectInstancesReflected::InteractionHandlerFederateRequestObjectInstancesReflected(std::shared_ptr<MomManager> mom)
  : MomInteractionHandler(mom)
{
  MomManager::InteractionClassAccessor request = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLArequest"]["HLArequestObjectInstancesReflected"];
  mRequestInteractionHandle = request;
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportObjectInstancesReflected"];
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAobjectInstanceCounts"];
    assert(p1.valid());
    mParameterGetters[p1] = [this] { return mMomManager->getFederateMetrics()->getObjectInstancesReflected(); };
    VariableLengthData empty;
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue },
    };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
}

InteractionHandlerFederateRequestUpdatesSent::InteractionHandlerFederateRequestUpdatesSent(std::shared_ptr<MomManager> mom)
  : MomInteractionHandler(mom)
{
  MomManager::InteractionClassAccessor request = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLArequest"]["HLArequestUpdatesSent"];
  mRequestInteractionHandle = request;
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportUpdatesSent"];
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAtransportation"];
    assert(p1.valid());
    mParameterGetters[p1] = [this] { return HLAunicodeString(L"HLAreliable").encode(); };
    ParameterHandle p2 = response.parameters["HLAupdateCounts"];
    assert(p2.valid());
    mParameterGetters[p2] = [this] { return mMomManager->getFederateMetrics()->getObjectClassUpdatesSent(); };
    VariableLengthData empty;
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue },
      { p2, kEmptyValue },
    };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
}


InteractionHandlerFederateRequestReflectionsReceived::InteractionHandlerFederateRequestReflectionsReceived(std::shared_ptr<MomManager> mom)
  : MomInteractionHandler(mom)
{
  MomManager::InteractionClassAccessor request = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLArequest"]["HLArequestReflectionsReceived"];
  mRequestInteractionHandle = request;
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportReflectionsReceived"];
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAtransportation"];
    assert(p1.valid());
    mParameterGetters[p1] = [this] { return HLAunicodeString(L"HLAreliable").encode(); };
    ParameterHandle p2 = response.parameters["HLAreflectCounts"];
    assert(p2.valid());
    mParameterGetters[p2] = [this] { return mMomManager->getFederateMetrics()->getObjectClassReflectionsReceived(); };
    VariableLengthData empty;
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue },
      { p2, kEmptyValue },
    };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
}

InteractionHandlerFederateRequestInteractionsSent::InteractionHandlerFederateRequestInteractionsSent(std::shared_ptr<MomManager> mom)
  : MomInteractionHandler(mom)
{
  MomManager::InteractionClassAccessor request = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLArequest"]["HLArequestInteractionsSent"];
  mRequestInteractionHandle = request;
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportInteractionsSent"];
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAtransportation"];
    assert(p1.valid());
    mParameterGetters[p1] = [this] { return HLAunicodeString(L"HLAreliable").encode(); };
    ParameterHandle p2 = response.parameters["HLAinteractionCounts"];
    assert(p2.valid());
    mParameterGetters[p2] = [this] { return mMomManager->getFederateMetrics()->getInteractionsSent(); };
    VariableLengthData empty;
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue },
      { p2, kEmptyValue }
    };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
}

InteractionHandlerFederateRequestInteractionsReceived::InteractionHandlerFederateRequestInteractionsReceived(std::shared_ptr<MomManager> mom)
  : MomInteractionHandler(mom)
{
  MomManager::InteractionClassAccessor request = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLArequest"]["HLArequestInteractionsReceived"];
  mRequestInteractionHandle = request;
  assert(mRequestInteractionHandle.valid());
  {
    MomManager::InteractionClassAccessor response = mom->interactions["HLAinteractionRoot"]["HLAmanager"]["HLAfederate"]["HLAreport"]["HLAreportInteractionsReceived"];
    InteractionClassHandle reponseInteraction = response;
    assert(reponseInteraction.valid());
    mResponseFederateParameter = response.parameters["HLAfederate"];
    mParameterGetters[mResponseFederateParameter] = [this] { return mMomManager->getEncodedFederateHandle(); };
    ParameterHandle p1 = response.parameters["HLAtransportation"];
    assert(p1.valid());
    mParameterGetters[p1] = [this] { return HLAunicodeString(L"HLAreliable").encode(); };
    ParameterHandle p2 = response.parameters["HLAinteractionCounts"];
    assert(p2.valid());
    mParameterGetters[p2] = [this] { return mMomManager->getFederateMetrics()->getInteractionsReceived(); };
    VariableLengthData empty;
    ParameterHandleValueMap respParams{
      { mResponseFederateParameter, mom->getEncodedFederateHandle() },
      { p1, kEmptyValue },
      { p2, kEmptyValue }
      };
    mResponseInteractions.push_back(std::make_pair(response, respParams));
  }
}

}
