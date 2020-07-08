
#include "MomFederation.h"
#include "MomFederate.h"
#include "Mom.h"
#include "dprintf.h"
#include <algorithm>

#pragma region class MomFederation
MomFederation::MomFederation(std::shared_ptr<Mom> mom, std::wstring federationName, ObjectInstanceHandle federationObjectInstance)
  : mFederatesInFederation(HLAhandle())
  , mFOMmoduleDesignatorList(HLAunicodeString())
{
  Mom::ObjectClassAccessor federationObject = mom->objects[L"HLAmanager"][L"HLAfederation"];
  mFederationClassHandle = federationObject;
  mFederationObjectInstance = federationObjectInstance;
  
  // plumb attributes with encoders
  AttributeHandle federationNameAttribute = federationObject.attributes[L"HLAfederationName"];
  mAttributeGetters[federationNameAttribute] = [this] { return mFederationName.encode(); };
  mAttributeSetters[federationNameAttribute] = [this](const VariableLengthData& data) { mFederationName.decode(data); };

  AttributeHandle federatesInFederationAttribute = federationObject.attributes[L"HLAfederatesInFederation"];
  mAttributeGetters[federatesInFederationAttribute] = [this] { return mFederatesInFederation.encode(); };
  mAttributeSetters[federatesInFederationAttribute] = [this](const VariableLengthData& data) { mFederatesInFederation.decode(data); };

  AttributeHandle rtiVersionAttribute = federationObject.attributes[L"HLARTIversion"];
  mAttributeGetters[rtiVersionAttribute] = [this] { return mRTIversion.encode(); };
  mAttributeSetters[rtiVersionAttribute] = [this](const VariableLengthData& data) { mRTIversion.decode(data); };

  AttributeHandle MIMdesignatorAttribute = federationObject.attributes[L"HLAMIMdesignator"];
  mAttributeGetters[MIMdesignatorAttribute] = [this] { return mMIMdesignator.encode(); };
  mAttributeSetters[MIMdesignatorAttribute] = [this](const VariableLengthData& data) { mMIMdesignator.decode(data); };

  AttributeHandle FOMmoduleDesignatorListAttribute = federationObject.attributes[L"HLAFOMmoduleDesignatorList"];
  mAttributeGetters[FOMmoduleDesignatorListAttribute] = [this] { return mFOMmoduleDesignatorList.encode(); };
  mAttributeSetters[FOMmoduleDesignatorListAttribute] = [this](const VariableLengthData& data) { mFOMmoduleDesignatorList.decode(data); };

  AttributeHandle currentFDDAttribute = federationObject.attributes[L"HLAcurrentFDD"];
  mAttributeGetters[currentFDDAttribute] = [this] { return mCurrentFDD.encode(); };
  mAttributeSetters[currentFDDAttribute] = [this](const VariableLengthData& data) { mCurrentFDD.decode(data); };

}


rti1516e::AttributeHandleValueMap MomFederation::GetAttributeValues(AttributeHandleSet requestedAttributes)
{
  AttributeHandleValueMap providedAttributes;
  for (auto attribute : requestedAttributes)
  {
    providedAttributes[attribute] = mAttributeGetters[attribute]();
  }
  return providedAttributes;
}


void MomFederation::SetAttributeValues(const AttributeHandleValueMap& attributes)
{
  for (auto[attributeHandle, attributeValue] : attributes)
  {
    if (mAttributeSetters.find(attributeHandle) != mAttributeSetters.end())
    {
      mAttributeSetters[attributeHandle](attributeValue);
    }
  }
}


AttributeHandleSet MomFederation::GetKnownAttributes() const
{
  AttributeHandleSet result;
  for (auto& entry : mAttributeGetters)
  {
    result.insert(entry.first);
  }
  return result;
}


void MomFederation::AddFederate(std::shared_ptr<MomFederate> federate)
{
  assert(federate->GetInternalFederateObjectInstance().isValid());
  assert(federate->GetInternalFederateHandle().isValid());
  mFederates.insert(federate);
  mFederatesByObject.insert(std::make_pair(federate->GetInternalFederateObjectInstance(), federate));
  mFederatesByHandle.insert(std::make_pair(federate->GetInternalFederateHandle(), federate));
}

void MomFederation::RemoveFederate(std::shared_ptr<MomFederate> federate)
{
  mFederates.erase(federate);
  mFederatesByObject.erase(federate->GetInternalFederateObjectInstance());
  mFederatesByHandle.erase(federate->GetInternalFederateHandle());
}

std::shared_ptr<MomFederate> MomFederation::GetFederate(ObjectInstanceHandle federateObjectInstance) const
{
  auto iter = mFederatesByObject.find(federateObjectInstance);
  if (iter != mFederatesByObject.end())
  {
    return iter->second;
  }
  return std::shared_ptr<MomFederate>();
}

std::shared_ptr<MomFederate> MomFederation::GetFederate(FederateHandle federateHandle) const
{
  auto iter = mFederatesByHandle.find(federateHandle);
  if (iter != mFederatesByHandle.end())
  {
    return iter->second;
  }
  return std::shared_ptr<MomFederate>();
}

std::vector<FederateHandle> MomFederation::GetFederatesInFederation() const
{
  std::vector<FederateHandle> result;
  result.reserve(mFederatesInFederation.size());
  for (size_t i = 0; i < mFederatesInFederation.size(); i++)
  {
    const HLAhandle& handle = static_cast<const HLAhandle&>(mFederatesInFederation.get(i));
    assert(handle.getFederateHandle().isValid());
    result.push_back(handle.getFederateHandle());
  }
  return result;
}

void MomFederation::SetFederatesInFederation(const std::vector<FederateHandle>& value)
{
  HLAvariableArray newValue = HLAvariableArray(HLAhandle(FederateHandle()));
  for (auto handle : value)
  {
    assert(handle.isValid());
    newValue.addElement(HLAhandle(handle));
  }
  mFederatesInFederation = newValue;
}

std::vector<std::wstring> MomFederation::GetFOMmoduleDesignatorList() const
{
  std::vector<std::wstring> result(mFOMmoduleDesignatorList.size());

  for (size_t index = 0; index < mFOMmoduleDesignatorList.size(); index++)
  {
    HLAunicodeString element = static_cast<const HLAunicodeString&>(mFOMmoduleDesignatorList.get(index));
    result[index] = element.get();
  }
  return result;
}

void MomFederation::SetFOMmoduleDesignatorList(const std::vector<std::wstring>& fomDesignatorList)
{
  HLAvariableArray newValue = HLAvariableArray(HLAunicodeString());
  for (auto fomDesignator : fomDesignatorList)
  {
    newValue.addElement(HLAunicodeString(fomDesignator));
  }
  mFOMmoduleDesignatorList = newValue;
}


#pragma endregion
