/* -*-c++-*- OpenRTI - Copyright (C) 2009-2016 Mathias Froehlich
 *
 * This file is part of OpenRTI.
 *
 * OpenRTI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * OpenRTI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenRTI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <RTI/Handle.h>

#include "HandleImplementation.h"
#include "VariableLengthDataImplementation.h"

#include <string>
#include <sstream>

namespace rti1516e
{

template <typename HandleKind>
Handle<HandleKind>::Handle()
  : _impl(HandleImplementation<HandleKind>::create(HandleImplementation<HandleKind>::OpenRTIHandleClass()))
{
  HandleImplementation<HandleKind>::get(_impl);
}

template <typename HandleKind>
Handle<HandleKind>::~Handle()
{
  HandleImplementation<HandleKind>::putAndDelete(_impl);
}

template <typename HandleKind>
Handle<HandleKind>::Handle(Handle const& rhs)
  : _impl(rhs._impl)
{
  HandleImplementation<HandleKind>::get(_impl);
}

template <typename HandleKind>
Handle<HandleKind>&
Handle<HandleKind>::operator=(Handle const& rhs)
{
  if(_impl != rhs._impl)
  {
    HandleImplementation<HandleKind>::get(rhs._impl);
    HandleImplementation<HandleKind>::putAndDelete(_impl);
    _impl = rhs._impl;
  }
  return *this;
}

template <typename HandleKind>
bool
Handle<HandleKind>::isValid() const
{
  return HandleImplementation<HandleKind>::getHandle(_impl).valid();
}

template <typename HandleKind>
bool Handle<HandleKind>::operator==(Handle const& rhs) const
{
  return HandleImplementation<HandleKind>::getHandle(_impl) ==
         HandleImplementation<HandleKind>::getHandle(rhs._impl);
}
template <typename HandleKind>
bool Handle<HandleKind>::operator!=(Handle const& rhs) const
{
  return HandleImplementation<HandleKind>::getHandle(_impl) !=
         HandleImplementation<HandleKind>::getHandle(rhs._impl);
}
template <typename HandleKind>
bool Handle<HandleKind>::operator<(Handle const& rhs) const
{
  return HandleImplementation<HandleKind>::getHandle(_impl) <
         HandleImplementation<HandleKind>::getHandle(rhs._impl);
}

template <typename HandleKind>
long Handle<HandleKind>::hash() const
{
  return long(HandleImplementation<HandleKind>::getHandle(_impl));
}

template <typename HandleKind>
VariableLengthData
Handle<HandleKind>::encode() const
{
  HandleImplementation<HandleKind>::OpenRTIHandleClass handle;
  handle = HandleImplementation<HandleKind>::getHandle(_impl);
  size_t encodedLength = handle.getEncodedLength();
  OpenRTI::VariableLengthData data(encodedLength);
  handle.encode(data.data());
  return VariableLengthDataFriend::create(data);
}

template <typename HandleKind>
void
Handle<HandleKind>::encode(VariableLengthData& buffer) const
{
  HandleImplementation<HandleKind>::OpenRTIHandleClass handle;
  handle = HandleImplementation<HandleKind>::getHandle(_impl);
  size_t encodedLength = handle.getEncodedLength();
  OpenRTI::VariableLengthData data(encodedLength);
  handle.encode(data.data());
  buffer = VariableLengthDataFriend::create(data);
}

template <typename HandleKind>
size_t
Handle<HandleKind>::encode(void* buffer, size_t bufferSize) const
{
  HandleImplementation<HandleKind>::OpenRTIHandleClass handle;
  handle = HandleImplementation<HandleKind>::getHandle(_impl);
  size_t encodedLength = handle.getEncodedLength();
  if(bufferSize < encodedLength)
    throw CouldNotEncode(toString());
  handle.encode(buffer);
  return encodedLength;
}

template <typename HandleKind>
size_t
Handle<HandleKind>::encodedLength() const
{
  HandleImplementation<HandleKind>::OpenRTIHandleClass handle;
  handle = HandleImplementation<HandleKind>::getHandle(_impl);
  return handle.getEncodedLength();
}

template <typename HandleKind>
std::wstring Handle<HandleKind>::toString() const
{
  HandleImplementation<HandleKind>::OpenRTIHandleClass handle;
  handle = HandleImplementation<HandleKind>::getHandle(_impl);
  std::wstringstream stream;
  if(handle.valid())
    stream << typeid(HandleKind).name() << "(" <<handle.getHandle()<< ")";\
  else
    stream << typeid(HandleKind).name() << "(Invalid)";
  return stream.str();
}


template <typename HandleKind>
const HandleImplementation<HandleKind>*
Handle<HandleKind>::getImplementation() const
{
  return _impl;
}

template <typename HandleKind>
HandleImplementation<HandleKind>*
Handle<HandleKind>::getImplementation()
{
  if(!HandleImplementation<HandleKind>::useImplementationClass())
    return _impl;
  if(OpenRTI::Referenced::count(_impl) <= 1)
    return _impl;
  HandleImplementation<HandleKind>* impl;
  impl = new HandleImplementation<HandleKind>(*_impl);
  HandleImplementation<HandleKind>::putAndDelete(_impl);
  _impl = impl;
  HandleImplementation<HandleKind>::get(_impl);
  return _impl;
}

template <typename HandleKind>
Handle<HandleKind>::Handle(HandleImplementation<HandleKind>* impl) :
  _impl(impl)
{
  HandleImplementation<HandleKind>::get(_impl);
}

template <typename HandleKind>
Handle<HandleKind>::Handle(VariableLengthData const& encodedValue) :
  _impl(0)
{
  HandleImplementation<HandleKind>::OpenRTIHandleClass handle;
  if(encodedValue.size() < handle.getEncodedLength())
    throw OpenRTI::CouldNotDecode(QUOTE(HandleKind));
  handle.decode(encodedValue.data());
  _impl = HandleImplementation<HandleKind>::create(handle);
  HandleImplementation<HandleKind>::get(_impl);
}

template <typename HandleKind>
RTI_EXPORT std::wostream& 
operator<<(std::wostream& stream, Handle<HandleKind> const& handle)
{
  return stream << handle.toString();
}

//template class Handle<FederateHandleKind>;

#define IMPLEMENT_HANDLE_CLASS(HandleKind) \
template class Handle<HandleKind##Kind>;   \
template class HandleImplementation<HandleKind##Kind>;   \
template RTI_EXPORT std::wostream& operator<<(std::wostream& stream, Handle<HandleKind##Kind> const& handle);

IMPLEMENT_HANDLE_CLASS(FederateHandle)
IMPLEMENT_HANDLE_CLASS(ObjectClassHandle)
IMPLEMENT_HANDLE_CLASS(InteractionClassHandle)
IMPLEMENT_HANDLE_CLASS(ObjectInstanceHandle)
IMPLEMENT_HANDLE_CLASS(AttributeHandle)
IMPLEMENT_HANDLE_CLASS(ParameterHandle)
IMPLEMENT_HANDLE_CLASS(DimensionHandle)
IMPLEMENT_HANDLE_CLASS(RegionHandle)
IMPLEMENT_HANDLE_CLASS(MessageRetractionHandle)

}
