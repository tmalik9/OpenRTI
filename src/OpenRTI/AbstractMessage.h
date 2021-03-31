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

#ifndef OpenRTI_AbstractMessage_h
#define OpenRTI_AbstractMessage_h

#include <iosfwd>
#include <list>
#include "Export.h"
#include "Handle.h"
#include "Referenced.h"
#include "VariableLengthData.h"

namespace OpenRTI {

class AbstractMessageDispatcher;

template<typename F>
class FunctorMessageDispatcher;
template<typename F>
class ConstFunctorMessageDispatcher;

template<typename T>
class SharedPtr;
namespace ServerModel {
class Federation;
class InteractionClass;
class ObjectClass;
}

class ParameterValue;
class OPENRTI_API AbstractMessage : public Referenced {
public:
  AbstractMessage() noexcept = default;
  AbstractMessage(const AbstractMessage&) = default;
  AbstractMessage(AbstractMessage&&) = delete;
  virtual ~AbstractMessage() noexcept = default;

  virtual const char* getTypeName() const = 0;
  virtual void out(std::ostream& os) const = 0;
  virtual void out(std::ostream& os, ServerModel::Federation* federation) const = 0;
  virtual void dispatch(const AbstractMessageDispatcher&) const = 0;
  virtual size_t messageSize() const;
  std::string toString() const;
  std::string toString(ServerModel::Federation* federation) const;
  // For testing of the transport implementation
  virtual bool operator==(const AbstractMessage&) const noexcept = 0;
  bool operator!=(const AbstractMessage& message) const noexcept
  { return ! operator==(message); }

  template<typename F>
  void dispatchFunctor(F& functor) const
  { dispatch(FunctorMessageDispatcher<F>(functor)); }
  template<typename F>
  void dispatchFunctor(const F& functor) const
  { dispatch(ConstFunctorMessageDispatcher<F>(functor)); }

  // Returns true if the message needs to be reliably sent or not.
  // The default implementation returns true. Interaction and attribute
  // update messages will provide a dynamic implementation for that.
  virtual bool getReliable() const noexcept;

  // Returns the object instance handle this message is targeting at.
  // The default implementation returns an invalid handle.
  // This is used to throw out messages for object instances that are already deleted.
  virtual ObjectInstanceHandle getObjectInstanceHandleForMessage() const noexcept;
};

inline std::ostream&
operator<<(std::ostream& os, const AbstractMessage& message)
{ message.out(os); return os; }

std::ostream& prettyprint(std::ostream& os, const InteractionClassHandle& value, ServerModel::Federation* federation);
std::ostream& prettyprint(std::ostream& os, const ParameterHandle& value, ServerModel::InteractionClass* interactionClass);

typedef std::list<SharedPtr<const AbstractMessage> > MessageList;

inline size_t byteSize(const int16_t&) { return sizeof(int16_t); }
inline size_t byteSize(const uint16_t&) { return sizeof(uint16_t); }
inline size_t byteSize(const int32_t&) { return sizeof(int32_t); }
inline size_t byteSize(const uint32_t&) { return sizeof(uint32_t); }
inline size_t byteSize(const int64_t&) { return sizeof(int64_t); }
inline size_t byteSize(const uint64_t&) { return sizeof(uint64_t); }
inline size_t byteSize(const double&) { return sizeof(double); }
inline size_t byteSize(const std::string& value) { return value.size() + sizeof(std::string::size_type); }
inline size_t byteSize(const VariableLengthData& value) { return value.size() + 3 * sizeof(size_t); }
inline size_t byteSize(const FederationHandle&) { return sizeof(FederationHandle); }
inline size_t byteSize(const FederateHandle&) { return sizeof(FederateHandle); }
inline size_t byteSize(const ObjectClassHandle&) { return sizeof(ObjectClassHandle); }
inline size_t byteSize(const ObjectInstanceHandle&) { return sizeof(ObjectInstanceHandle); }
inline size_t byteSize(const AttributeHandle&) { return sizeof(AttributeHandle); }
inline size_t byteSize(const InteractionClassHandle&) { return sizeof(InteractionClassHandle); }
inline size_t byteSize(const ParameterHandle&) { return sizeof(ParameterHandle); }
inline size_t byteSize(const RegionHandle&) { return sizeof(RegionHandle); }
inline size_t byteSize(const DimensionHandle&) { return sizeof(DimensionHandle); }
inline size_t byteSize(const ModuleHandle&) { return sizeof(ModuleHandle); }
//template<typename L, typename R> inline size_t byteSize(const std::pair<L, R>& value) 
//{
//  return byteSize(value.first) + byteSize(value.second);
//}
} // namespace OpenRTI

#endif
