
#include "MomFederate.h"
#include "Mom.h"
#include "dprintf.h"

#pragma region class MomFederate
MomFederate::MomFederate(std::shared_ptr<Mom> mom, FederateHandle federateHandle, ObjectInstanceHandle federateObjectInstance)
{
  mFederateObjectInstance = federateObjectInstance;
  mInternalFederateHandle = federateHandle;

  // get handles
  Mom::ObjectClassAccessor federateObject = mom->objects[L"HLAmanager"][L"HLAfederate"];
  mFederateClassHandle = federateObject;

  // getter functions
  AttributeHandle federateHandleAttribute = federateObject.attributes[L"HLAfederateHandle"];
  mAttributeGetters[federateHandleAttribute] = [this] { return mFederateHandle.encode(); };
  mAttributeSetters[federateHandleAttribute] = [this](const VariableLengthData& data) { mFederateHandle.decode(data); };

  AttributeHandle federateNameAttribute = federateObject.attributes[L"HLAfederateName"];
  mAttributeGetters[federateNameAttribute] = [this]{ return mFederateName.encode(); };
  mAttributeSetters[federateNameAttribute] = [this](const VariableLengthData& data) { mFederateName.decode(data); };

  AttributeHandle federateTypeAttribute = federateObject.attributes[L"HLAfederateType"];
  mAttributeGetters[federateTypeAttribute] = [this]{ return mFederateType.encode(); };
  mAttributeSetters[federateTypeAttribute] = [this](const VariableLengthData& data) { mFederateType.decode(data); };

  AttributeHandle federateHostAttribute = federateObject.attributes[L"HLAfederateHost"];
  mAttributeGetters[federateHostAttribute] = [this]{ return mFederateHost.encode(); };
  mAttributeSetters[federateHostAttribute] = [this](const VariableLengthData& data) { mFederateHost.decode(data); };

  AttributeHandle rtiVersionAttribute = federateObject.attributes[L"HLARTIversion"];
  mAttributeGetters[rtiVersionAttribute] = [this]{ return mFederateRTIversion.encode(); };
  mAttributeSetters[rtiVersionAttribute] = [this](const VariableLengthData& data) { mFederateRTIversion.decode(data); };

  AttributeHandle timeConstrainedAttribute = federateObject.attributes[L"HLAtimeConstrained"];
  mAttributeGetters[timeConstrainedAttribute] = [this]{ return mTimeConstrained.encode(); };
  mAttributeSetters[timeConstrainedAttribute] = [this](const VariableLengthData& data) { mTimeConstrained.decode(data); };

  AttributeHandle timeRegulatingAttribute = federateObject.attributes[L"HLAtimeRegulating"];
  mAttributeGetters[timeRegulatingAttribute] = [this]{ return mTimeRegulating.encode(); };
  mAttributeSetters[timeRegulatingAttribute] = [this](const VariableLengthData& data) { mTimeRegulating.decode(data); };

  AttributeHandle asynchronousDeliveryAttribute = federateObject.attributes[L"HLAasynchronousDelivery"];
  mAttributeGetters[asynchronousDeliveryAttribute] = [this]{ return mAsynchronousDelivery.encode(); };
  mAttributeSetters[asynchronousDeliveryAttribute] = [this](const VariableLengthData& data) { mAsynchronousDelivery.decode(data); };

  AttributeHandle reflectionsReceivedAttribute = federateObject.attributes[L"HLAreflectionsReceived"];
  mAttributeGetters[reflectionsReceivedAttribute] = [this]{ return mReflectionsReceived.encode(); };
  mAttributeSetters[reflectionsReceivedAttribute] = [this](const VariableLengthData& data) { mReflectionsReceived.decode(data); };

  AttributeHandle updatesSentAttribute = federateObject.attributes[L"HLAupdatesSent"];
  mAttributeGetters[updatesSentAttribute] = [this]{ return mUpdatesSent.encode(); };
  mAttributeSetters[updatesSentAttribute] = [this](const VariableLengthData& data) { mUpdatesSent.decode(data); };

  AttributeHandle interactionsReceivedAttribute = federateObject.attributes[L"HLAinteractionsReceived"];
  mAttributeGetters[interactionsReceivedAttribute] = [this]{ return mInteractionsReceived.encode(); };
  mAttributeSetters[interactionsReceivedAttribute] = [this](const VariableLengthData& data) { mInteractionsReceived.decode(data); };

  AttributeHandle interactionsSentAttribute = federateObject.attributes[L"HLAinteractionsSent"];
  mAttributeGetters[interactionsSentAttribute] = [this]{ return mInteractionsSent.encode(); };
  mAttributeSetters[interactionsSentAttribute] = [this](const VariableLengthData& data) { mInteractionsSent.decode(data); };

  AttributeHandle objectInstancesThatCanBeDeletedAttribute = federateObject.attributes[L"HLAobjectInstancesThatCanBeDeleted"];
  mAttributeGetters[objectInstancesThatCanBeDeletedAttribute] = [this]{ return mObjectInstancesThatCanBeDeleted.encode(); };
  mAttributeSetters[objectInstancesThatCanBeDeletedAttribute] = [this](const VariableLengthData& data) { mObjectInstancesThatCanBeDeleted.decode(data); };

  AttributeHandle objectInstancesUpdatedAttribute = federateObject.attributes[L"HLAobjectInstancesUpdated"];
  mAttributeGetters[objectInstancesUpdatedAttribute] = [this]{ return mObjectInstancesUpdated.encode(); };
  mAttributeSetters[objectInstancesUpdatedAttribute] = [this](const VariableLengthData& data) { mObjectInstancesUpdated.decode(data); };

  AttributeHandle objectInstancesReflectedAttribute = federateObject.attributes[L"HLAobjectInstancesReflected"];
  mAttributeGetters[objectInstancesReflectedAttribute] = [this]{ return mObjectInstancesReflected.encode(); };
  mAttributeSetters[objectInstancesReflectedAttribute] = [this](const VariableLengthData& data) { mObjectInstancesReflected.decode(data); };

  AttributeHandle objectInstancesDeletedAttribute = federateObject.attributes[L"HLAobjectInstancesDeleted"];
  mAttributeGetters[objectInstancesDeletedAttribute] = [this]{ return mObjectInstancesDeleted.encode(); };
  mAttributeSetters[objectInstancesDeletedAttribute] = [this](const VariableLengthData& data) { mObjectInstancesDeleted.decode(data); };

  AttributeHandle objectInstancesRemovedAttribute = federateObject.attributes[L"HLAobjectInstancesRemoved"];
  mAttributeGetters[objectInstancesRemovedAttribute] = [this]{ return mObjectInstancesRemoved.encode(); };
  mAttributeSetters[objectInstancesRemovedAttribute] = [this](const VariableLengthData& data) { mObjectInstancesRemoved.decode(data); };

  AttributeHandle objectInstancesRegisteredAttribute = federateObject.attributes[L"HLAobjectInstancesRegistered"];
  mAttributeGetters[objectInstancesRegisteredAttribute] = [this]{ return mObjectInstancesRegistered.encode(); };
  mAttributeSetters[objectInstancesRegisteredAttribute] = [this](const VariableLengthData& data) { mObjectInstancesRegistered.decode(data); };

  AttributeHandle objectInstancesDiscoveredAttribute = federateObject.attributes[L"HLAobjectInstancesDiscovered"];
  mAttributeGetters[objectInstancesDiscoveredAttribute] = [this]{ return mObjectInstancesDiscovered.encode(); };
  mAttributeSetters[objectInstancesDiscoveredAttribute] = [this](const VariableLengthData& data) { mObjectInstancesDiscovered.decode(data); };

  AttributeHandle federateTimeGrantedTimeAttribute = federateObject.attributes[L"HLAtimeGrantedTime"];
  mAttributeGetters[federateTimeGrantedTimeAttribute] = [this]{ return mFederateTimeGrantedTime.encode(); };
  mAttributeSetters[federateTimeGrantedTimeAttribute] = [this](const VariableLengthData& data) { mFederateTimeGrantedTime.decode(data); };

  AttributeHandle federateTimeAdvancingTimeAttribute = federateObject.attributes[L"HLAtimeAdvancingTime"];
  mAttributeGetters[federateTimeAdvancingTimeAttribute] = [this]{ return mFederateTimeAdvancingTime.encode(); };
  mAttributeSetters[federateTimeAdvancingTimeAttribute] = [this](const VariableLengthData& data) { mFederateTimeAdvancingTime.decode(data); };
}

AttributeHandleValueMap MomFederate::GetAttributeValues(AttributeHandleSet requestedAttributes)
{

  AttributeHandleValueMap providedAttributes;
  for (auto attribute : requestedAttributes)
  {
    providedAttributes[attribute] = mAttributeGetters[attribute]();
  }
  return providedAttributes;
}


void MomFederate::SetAttributeValues(const AttributeHandleValueMap& attributes)
{
  for (auto [attributeHandle, attributeValue] : attributes)
  {
    if (mAttributeSetters.find(attributeHandle) != mAttributeSetters.end())
    {
      mAttributeSetters[attributeHandle](attributeValue);
    }
  }
}

AttributeHandleSet MomFederate::GetKnownAttributes() const
{
  AttributeHandleSet result;
  for (auto& entry : mAttributeGetters)
  {
    result.insert(entry.first);
  }
  return result;
}

#pragma endregion
