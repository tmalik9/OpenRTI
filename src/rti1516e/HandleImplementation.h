/* -*-c++-*- OpenRTI - Copyright (C) 2009-2012 Mathias Froehlich
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

#ifndef HandleImplementation_h
#define HandleImplementation_h

#include <RTI/Handle.h>
#include "Handle.h"
#include "Referenced.h"

namespace rti1516e
{

template<typename HandleKind>
struct HandleImplementationBase
{
};

#define DECLARE_IMPL_BASE(HandleKind) \
template<> struct HandleImplementationBase<HandleKind##Kind> { typedef OpenRTI::HandleKind BaseHandleClass; };

DECLARE_IMPL_BASE(FederateHandle)
DECLARE_IMPL_BASE(ObjectClassHandle)
DECLARE_IMPL_BASE(InteractionClassHandle)
DECLARE_IMPL_BASE(ObjectInstanceHandle)
DECLARE_IMPL_BASE(AttributeHandle)
DECLARE_IMPL_BASE(ParameterHandle)
DECLARE_IMPL_BASE(DimensionHandle)
DECLARE_IMPL_BASE(RegionHandle)
DECLARE_IMPL_BASE(MessageRetractionHandle)

template<class HandleKind>
class OPENRTI_LOCAL HandleImplementation : public OpenRTI::Referenced
{
  public:
    typedef typename HandleImplementationBase<HandleKind>::BaseHandleClass OpenRTIHandleClass;

    HandleImplementation()
    {}
    HandleImplementation(const OpenRTIHandleClass& handle) :
      _handle(handle)
    {}

    static bool
    useImplementationClass()
    {
      return sizeof(HandleImplementation*) < sizeof(OpenRTIHandleClass);
    }

    static HandleImplementation*
    create(const OpenRTIHandleClass& handle)
    {
      union
      {
        HandleImplementation* ptr;
        OpenRTIHandleClass::value_type value;
      } u;
      if(useImplementationClass())
      {
        u.ptr = new HandleImplementation(handle);
      }
      else
      {
        u.ptr = 0;
        u.value = handle;
      }
      return u.ptr;
    }

    static void
    putAndDelete(HandleImplementation* impl)
    {
      if(!useImplementationClass())
        return;
      if(OpenRTI::Referenced::put(impl))
        return;
      delete impl;
    }
    static void
    get(HandleImplementation* impl)
    {
      if(!useImplementationClass())
        return;
      OpenRTI::Referenced::get(impl);
    }

    static OpenRTIHandleClass
    getHandle(HandleImplementation* ptr)
    {
      if(useImplementationClass())
      {
        return ptr->_handle;
      }
      else
      {
        union
        {
          HandleImplementation* ptr;
          OpenRTIHandleClass::value_type value;
        } u;
        u.ptr = ptr;
        return u.value;
      }
    }
    OpenRTIHandleClass _handle;
};

template<class HandleKind>
class OPENRTI_LOCAL HandleImplementationHelper
{
  typedef typename HandleImplementationBase<HandleKind>::BaseHandleClass OpenRTIHandleClass;
  typedef Handle<HandleKind> HandleClass;
  typedef HandleImplementation<HandleKind> HandleImplementationClass;
  typedef typename HandleImplementationBase<HandleKind>::BaseHandleClass OpenRTIHandleClass;
public:
  static HandleClass decode(rti1516e::VariableLengthData const& encodedValue)
  {
    return Handle<HandleKind>(encodedValue);
  }
  static void copy(OpenRTIHandleClass& dst, HandleClass const& src)
  {
    dst = HandleImplementationClass::getHandle(src._impl);
  }
  static void copy(HandleClass& dst, OpenRTIHandleClass const& src)
  {
    dst = HandleClass(HandleImplementationClass::create(src));
  }
};

#define DECLARE_HANDLE_CLASS(HandleKind) \
typedef HandleImplementation<HandleKind##Kind> HandleKind##Implementation; \
typedef HandleImplementationHelper<HandleKind##Kind> HandleKind##ImplementationHelper;


DECLARE_HANDLE_CLASS(FederateHandle)
DECLARE_HANDLE_CLASS(ObjectClassHandle)
DECLARE_HANDLE_CLASS(InteractionClassHandle)
DECLARE_HANDLE_CLASS(ObjectInstanceHandle)
DECLARE_HANDLE_CLASS(AttributeHandle)
DECLARE_HANDLE_CLASS(ParameterHandle)
DECLARE_HANDLE_CLASS(DimensionHandle)
DECLARE_HANDLE_CLASS(RegionHandle)
DECLARE_HANDLE_CLASS(MessageRetractionHandle)

#undef DECLARE_HANDLE_CLASS

}

#endif
