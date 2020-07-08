
#include "StringUtils.h"
#include "BasicDataElements.h"
#include "HLAhandle.h"
#include "Handle.h"
#include "Encoding.h"

#include <cassert>
#include <memory>

namespace OpenRTI {

HLAhandle::HLAhandle()
  : mHandleKindEncoding(eInvalid)
  , mHandleValueEncoding()
{
}

HLAhandle::HLAhandle(const HLAhandle& ref)
  : mHandleKindEncoding(ref.mHandleKindEncoding)
  , mHandleValueEncoding(ref.mHandleValueEncoding)
{
}

HLAhandle::HLAhandle(const FederateHandle& handle)
  : mHandleKindEncoding(eFederateHandle)
  , mHandleValueEncoding(handle.getHandle())
{
}

HLAhandle::HLAhandle(const ObjectClassHandle& handle)
  : mHandleKindEncoding(eObjectClassHandle)
  , mHandleValueEncoding(handle.getHandle())
{
}

HLAhandle::HLAhandle(const ObjectInstanceHandle& handle)
  : mHandleKindEncoding(eObjectInstanceHandle)
  , mHandleValueEncoding(handle.getHandle())
{
}

HLAhandle::HLAhandle(const AttributeHandle& handle)
  : mHandleKindEncoding(eAttributeHandle)
  , mHandleValueEncoding(handle.getHandle())
{
}

HLAhandle::HLAhandle(const InteractionClassHandle& handle)
  : mHandleKindEncoding(eInteractionClassHandle)
  , mHandleValueEncoding(handle.getHandle())
{
}

HLAhandle::HLAhandle(const ParameterHandle& handle)
  : mHandleKindEncoding(eParameterHandle)
  , mHandleValueEncoding(handle.getHandle())
{
}

FederateHandle HLAhandle::getFederateHandle() const
{
  if (mHandleKindEncoding.get() == eFederateHandle)
  {
    return FederateHandle(static_cast<FederateHandle::value_type>(mHandleValueEncoding.get()));
  }
  return FederateHandle();
}

ObjectClassHandle HLAhandle::getObjectClassHandle() const
{
  if (mHandleKindEncoding.get() == eObjectClassHandle)
  {
    return ObjectClassHandle(static_cast<ObjectClassHandle::value_type>(mHandleValueEncoding.get()));
  }
  return ObjectClassHandle();
}

ObjectInstanceHandle HLAhandle::getObjectInstanceHandle() const
{
  if (mHandleKindEncoding.get() == eObjectInstanceHandle)
  {
    return ObjectInstanceHandle(static_cast<ObjectInstanceHandle::value_type>(mHandleValueEncoding.get()));
  }
  return ObjectInstanceHandle();
}

AttributeHandle HLAhandle::getAttributeHandle() const
{
  if (mHandleKindEncoding.get() == eAttributeHandle)
  {
    return AttributeHandle(static_cast<AttributeHandle::value_type>(mHandleValueEncoding.get()));
  }
  return AttributeHandle();
}

InteractionClassHandle HLAhandle::getInteractionClassHandle() const
{
  if (mHandleKindEncoding.get() == eInteractionClassHandle)
  {
    return InteractionClassHandle(static_cast<InteractionClassHandle::value_type>(mHandleValueEncoding.get()));
  }
  return InteractionClassHandle();
}

ParameterHandle HLAhandle::getParameterHandle() const
{
  if (mHandleKindEncoding.get() == eParameterHandle)
  {
    return ParameterHandle(static_cast<ParameterHandle::value_type>(mHandleValueEncoding.get()));
  }
  return ParameterHandle();
}

void HLAhandle::set(FederateHandle handle)
{
  mHandleKindEncoding.set(eFederateHandle);
  mHandleValueEncoding.set(handle.getHandle());
}

void HLAhandle::set(ObjectClassHandle handle)
{
  mHandleKindEncoding.set(eObjectClassHandle);
  mHandleValueEncoding.set(handle.getHandle());
}

void HLAhandle::set(ObjectInstanceHandle handle)
{
  mHandleKindEncoding.set(eObjectInstanceHandle);
  mHandleValueEncoding.set(handle.getHandle());
}

void HLAhandle::set(AttributeHandle handle)
{
  mHandleKindEncoding.set(eAttributeHandle);
  mHandleValueEncoding.set(handle.getHandle());
}

void HLAhandle::set(InteractionClassHandle handle)
{
  mHandleKindEncoding.set(eInteractionClassHandle);
  mHandleValueEncoding.set(handle.getHandle());
}

void HLAhandle::set(ParameterHandle handle)
{
  mHandleKindEncoding.set(eParameterHandle);
  mHandleValueEncoding.set(handle.getHandle());
}

std::unique_ptr<OpenRTI::DataElement> HLAhandle::clone() const
{
  return std::make_unique<HLAhandle>(*this);
}

VariableLengthData HLAhandle::encode() const
{
  VariableLengthData result;
  encode(result);
  return result;
}

void HLAhandle::encode(VariableLengthData& inData) const
{
  std::vector<Octet> buffer;
  buffer.reserve(getEncodedLength());
  encodeInto(buffer);
  inData.setData(buffer.data(), buffer.size());
}

void HLAhandle::encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, getOctetBoundary());
  align(buffer, mHandleKindEncoding.getOctetBoundary());
  mHandleKindEncoding.encodeInto(buffer);
  align(buffer, mHandleValueEncoding.getOctetBoundary());
  mHandleValueEncoding.encodeInto(buffer);
}

void HLAhandle::decode(VariableLengthData const& inData)
{
  std::vector<Octet> buffer;
  buffer.resize(inData.size());
  memcpy(buffer.data(), inData.data(), inData.size());
  decodeFrom(buffer, 0);
}

size_t HLAhandle::decodeFrom(std::vector<Octet> const& buffer, size_t index)
{
  index = mHandleKindEncoding.decodeFrom(buffer, index);
  index = mHandleValueEncoding.decodeFrom(buffer, index);
  return index;
}

size_t HLAhandle::getEncodedLength() const
{
  size_t length = 0;
  length = align(length, mHandleKindEncoding.getEncodedLength());
  length += mHandleKindEncoding.getEncodedLength();
  length = align(length, mHandleValueEncoding.getEncodedLength());
  length += mHandleValueEncoding.getEncodedLength();
  return length;
}

unsigned int HLAhandle::getOctetBoundary() const
{
  return mHandleValueEncoding.getOctetBoundary();
}

}