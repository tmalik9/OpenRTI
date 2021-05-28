
#include "DebugNew.h"
#include "StringUtils.h"
#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAhandle.h"
#include "Encoding.h"
#include "HandleImplementation.h"
#include "VariableLengthDataImplementation.h"
#include "encoding/HLAhandle.h"
#include "StringUtils.h"

#include <cassert>

namespace rti1516ev {

class HLAhandle::Implementation : public HLAencodingImplementationBase
{
  public:
    Implementation(const Implementation& ref) : mInternalHandle(ref.mInternalHandle) {}
    Implementation(const OpenRTI::HLAhandle& ref) : mInternalHandle(ref) {}
    Implementation() : mInternalHandle() {}
    size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
    {
      return mInternalHandle.encodeInto(buffer, bufferSize, offset);
    }
    size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t offset)
    {
      return mInternalHandle.decodeFrom(buffer, bufferSize, offset);
    }
    HLAencodingImplementationBase* clone() const
    {
      return new Implementation(*this);
    }
    OpenRTI::HLAhandle mInternalHandle;
};

HLAhandle::HLAhandle() : _impl()
{
  _impl = std::unique_ptr<HLAhandle::Implementation>(new HLAhandle::Implementation());
}

HLAhandle::HLAhandle(const HLAhandle& ref) : _impl()
{
  _impl = std::unique_ptr<HLAhandle::Implementation>(new HLAhandle::Implementation(*ref._impl.get()));
}

HLAhandle::HLAhandle(const FederateHandle& handle) : _impl(std::unique_ptr<HLAhandle::Implementation>(new HLAhandle::Implementation()))
{
  set(handle);
}

HLAhandle::HLAhandle(const ObjectClassHandle& handle) : _impl(std::unique_ptr<HLAhandle::Implementation>(new HLAhandle::Implementation()))
{
  set(handle);
}

HLAhandle::HLAhandle(const ObjectInstanceHandle& handle) : _impl(std::unique_ptr<HLAhandle::Implementation>(new HLAhandle::Implementation()))
{
  set(handle);
}

HLAhandle::HLAhandle(const AttributeHandle& handle) : _impl(std::unique_ptr<HLAhandle::Implementation>(new HLAhandle::Implementation()))
{
  set(handle);
}


HLAhandle::HLAhandle(const InteractionClassHandle& handle) : _impl(std::unique_ptr<HLAhandle::Implementation>(new HLAhandle::Implementation()))
{
  set(handle);
}

HLAhandle::HLAhandle(const ParameterHandle& handle) : _impl(std::unique_ptr<HLAhandle::Implementation>(new HLAhandle::Implementation()))
{
  set(handle);
}

HLAhandle::~HLAhandle() noexcept
{
}

HLAhandle& HLAhandle::operator=(const HLAhandle& ref)
{
  _impl = std::unique_ptr<HLAhandle::Implementation>(new HLAhandle::Implementation(*ref._impl.get()));
  return *this;
}

FederateHandle HLAhandle::getFederateHandle() const
{
  FederateHandle rti1516eHandle;
  FederateHandleFriend::copy(rti1516eHandle,  _impl->mInternalHandle.getFederateHandle());
  return rti1516eHandle;
}

ObjectClassHandle HLAhandle::getObjectClassHandle() const
{
  ObjectClassHandle rti1516eHandle;
  ObjectClassHandleFriend::copy(rti1516eHandle,  _impl->mInternalHandle.getObjectClassHandle());
  return rti1516eHandle;
}

ObjectInstanceHandle HLAhandle::getObjectInstanceHandle() const
{
  ObjectInstanceHandle rti1516eHandle;
  ObjectInstanceHandleFriend::copy(rti1516eHandle,  _impl->mInternalHandle.getObjectInstanceHandle());
  return rti1516eHandle;
}

AttributeHandle HLAhandle::getAttributeHandle() const
{
  AttributeHandle rti1516eHandle;
  AttributeHandleFriend::copy(rti1516eHandle,  _impl->mInternalHandle.getAttributeHandle());
  return rti1516eHandle;
}

InteractionClassHandle HLAhandle::getInteractionClassHandle() const
{
  InteractionClassHandle rti1516eHandle;
  InteractionClassHandleFriend::copy(rti1516eHandle,  _impl->mInternalHandle.getInteractionClassHandle());
  return rti1516eHandle;
}

ParameterHandle HLAhandle::getParameterHandle() const
{
  ParameterHandle rti1516eHandle;
  ParameterHandleFriend::copy(rti1516eHandle,  _impl->mInternalHandle.getParameterHandle());
  return rti1516eHandle;
}

void HLAhandle::set(FederateHandle handle)
{
  OpenRTI::FederateHandle nativeHandle;
  FederateHandleFriend::copy(nativeHandle, handle);
  _impl->mInternalHandle.set(nativeHandle);
}

void HLAhandle::set(ObjectClassHandle handle)
{
  OpenRTI::ObjectClassHandle nativeHandle;
  ObjectClassHandleFriend::copy(nativeHandle, handle);
  _impl->mInternalHandle.set(nativeHandle);
}

void HLAhandle::set(ObjectInstanceHandle handle)
{
  OpenRTI::ObjectInstanceHandle nativeHandle;
  ObjectInstanceHandleFriend::copy(nativeHandle, handle);
  _impl->mInternalHandle.set(nativeHandle);
}

void HLAhandle::set(AttributeHandle handle)
{
  OpenRTI::AttributeHandle nativeHandle;
  AttributeHandleFriend::copy(nativeHandle, handle);
  _impl->mInternalHandle.set(nativeHandle);
}

void HLAhandle::set(InteractionClassHandle handle)
{
  OpenRTI::InteractionClassHandle nativeHandle;
  InteractionClassHandleFriend::copy(nativeHandle, handle);
  _impl->mInternalHandle.set(nativeHandle);
}

void HLAhandle::set(ParameterHandle handle)
{
  OpenRTI::ParameterHandle nativeHandle;
  ParameterHandleFriend::copy(nativeHandle, handle);
  _impl->mInternalHandle.set(nativeHandle);
}

VariableLengthData HLAhandle::encode() const
{
  VariableLengthData result;
  encode(result);
  return result;
}

void HLAhandle::encode(VariableLengthData& outData) const
{
  OpenRTI::VariableLengthData data;
  _impl->mInternalHandle.encode(data);
  outData = VariableLengthDataFriend::create(data);
}

void HLAhandle::encodeInto(std::vector<Octet>& buffer) const
{
  _impl->mInternalHandle.encodeInto(buffer);
}

size_t HLAhandle::encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  return _impl->encodeInto(buffer, bufferSize, offset);
}

void HLAhandle::decode(VariableLengthData const& inData)
{
  try
  {
    _impl->mInternalHandle.decode(OpenRTI::VariableLengthData(rti1516ev::VariableLengthDataFriend::readPointer(inData)));
  }
  catch (const OpenRTI::EncoderException& e)
  {
    throw rti1516ev::EncoderException(OpenRTI::utf8ToUcs(e.what()));
  }
}

size_t HLAhandle::decodeFrom(std::vector<Octet> const& buffer, size_t index)
{
  try
  {
    return _impl->mInternalHandle.decodeFrom(buffer, index);
  }
  catch (const OpenRTI::EncoderException& e)
  {
    throw rti1516ev::EncoderException(OpenRTI::utf8ToUcs(e.what()));
  }
}

size_t HLAhandle::decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  try
  {
    return _impl->decodeFrom(buffer, bufferSize, index);
  }
  catch (const OpenRTI::EncoderException& e)
  {
    throw rti1516ev::EncoderException(OpenRTI::utf8ToUcs(e.what()));
  }
}

size_t HLAhandle::getEncodedLength() const
{
  return _impl->mInternalHandle.getEncodedLength();
}

unsigned int HLAhandle::getOctetBoundary() const
{
  return _impl->mInternalHandle.getOctetBoundary();
}

}