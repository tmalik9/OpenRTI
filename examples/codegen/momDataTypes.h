
// Code automatically generated by FOMCodeGen version 2.1.0.0 from mom.xml, do not edit

// Name: Standard MOM and Initialization Module (MIM) for HLA IEEE 1516-2010
// Version: 1.0
// Description: The MOM Object Classes and Interaction Classes of this object model may be extended.
// Purpose: Standard MOM and Initialization Module for HLA IEEE 1516-2010

#pragma once

#include <vector>
#include <ostream>
#include <cassert>
#include <new>
#include <utility>
#include <type_traits>

#include "RTI/encoding/HLAhandle.h"

namespace OpenRTI {
namespace Mom {

// define placement_new in case some other header has redefined operator new to sth. different (e.g. MS crtdbg)
#ifndef _PLACEMENT_NEW_DEFINED
#pragma push_macro("new")
#undef new
template <class X, typename ... Types>
inline X* placement_new(void* addr, Types&& ... args)
{
  return new (addr) X(std::forward<Types>(args)...);
}
#pragma pop_macro("new")
#define _PLACEMENT_NEW_DEFINED
#endif

// define our own simplified 'optional' types, similar to but independent of std::optional (since C++17) or boost::optional

struct nullopt_t
{
  enum class _Construct { _Token };
  explicit constexpr nullopt_t(_Construct) { }
};

/// Tag to disengage optional objects.
constexpr nullopt_t nullopt { nullopt_t::_Construct::_Token };

class bad_optional_access : public std::exception
{
public:
  virtual const char* what() const noexcept override { return ("Bad optional access"); }
};

// (empty) base template
template<typename T, bool=std::is_class<T>::value > struct optional {};

// Specialization for classes and structs used by-value, w. in-place copy-construction.
// This implementation is limited to copy-constructible and default-destructible classes!
template<typename T> struct optional<T, true>
{
  static_assert(!std::is_reference< T >::value, "reference is not supported as optional value");
  // constructors
  optional() noexcept : _valid(false) {}
  constexpr optional(nullopt_t) noexcept : _valid(false) {}
  optional(const optional& rhs): _valid(rhs._valid) { create(rhs); }
  optional(const T& w): _valid(true) { create(w); }
  template<typename Other>
  optional(const Other& w): _valid(true) { create(w); }
  template<typename Other>
  optional(const optional<Other>& rhs): _valid(rhs.has_value()) { if (rhs.has_value()) create(rhs.value()); }

  // destructor
  ~optional() { if(_valid) destroy(); }

  // check for value presence
  bool has_value() const { return _valid; }
  explicit operator bool() const { return _valid; }
  bool operator ! () const { return !_valid; }

  // assign
  optional& operator = (optional const& rhs) { return assign(rhs); }
  optional& operator = (T const& w) { return assign(w); }

  // unchecked access to value
  const T& operator * () const & { return *get(); }
  T& operator * () & { return *get(); }
  T const* operator -> () const { return get(); }
  T* operator -> () { return get(); }
  // checked access to value
  const T& value() const & {
    if (!_valid) {
      throw bad_optional_access{};
    }
    return *get();
  }
  T& value() & {
    if (!_valid) {
      throw bad_optional_access{};
    }
    return *get();
  }
private:
  template<typename Other>
  void create(const Other& w) { placement_new<T>(&_value, w); }
  void create(const optional& rhs) { if(_valid) create(*rhs.get()); }
  void destroy() { get()->~T(); }
  T const* get() const { assert(_valid && "no optional value"); return reinterpret_cast< const T* >(&_value); }
  T* get() { assert(_valid && "no optional value"); return reinterpret_cast< T* >(&_value); }
  void cleanup() { destroy(); _valid = false; }
  optional& assign(const T& w) {
    if(_valid) *get() = w;
    else create(w), _valid = true;
    return *this;
  }
  optional& assign(const optional& rhs) {
    if(rhs._valid) return assign(*rhs.get());
    if(!_valid) return *this;
    cleanup();
    return *this;
  }
  bool _valid;
	union {
    typename std::remove_cv<T>::type _value;
  };
};
// Specialization for const-ref parameters, implemented by holding a pointer to the original variable.
// Note that non-const references are *not* supported (but could be easily added)!
template<typename T> struct optional<const T&, false>
{
  // constructors
  optional() noexcept : _valid(false), _pointer() {}
  constexpr optional(nullopt_t) noexcept : _valid(false), _pointer() {}
  optional(const optional& rhs): _valid(rhs._valid), _pointer(rhs._pointer) { }
  optional(const T& rhs) noexcept : _valid(true), _pointer(&rhs) { }
  template<typename Other>
  optional(const Other& rhs): _valid(true), _pointer(&rhs) { }
  template<typename Other>
  optional(const optional<Other>& rhs): _valid(rhs.has_value()), _pointer(rhs.operator->()) { }

  // destructor
  ~optional() { }

  // check for value presence
  bool has_value() const { return _valid; }
  explicit operator bool() const { return _valid; }
  bool operator ! () const { return !_valid; }

  // assign
  optional& operator = (const optional& rhs) {
    _valid = rhs._valid;
    _pointer = rhs._pointer;
    return *this;
  }
  optional& operator = (const T& w) {
    _valid = true;
    _pointer = &w;
    return *this;
  }
  // unchecked access to value
  T const& operator * () const & { return *get(); }
  T const* operator -> () const & { return get(); }
  // checked access to value
  const T& value() const & {
    if (!_valid) {
      throw bad_optional_access{};
    }
    return *get();
  }
  T& value() & {
    if (!_valid) {
      throw bad_optional_access{};
    }
    return *get();
  }
private:
  const T* get() const {
    assert(_valid && "no optional value");
    return _pointer;
  }
  bool _valid;
  const T* _pointer = nullptr;
};
// Specialization for everything else, which includes fundamental types and pointer types.
template<typename T> struct optional<T, false>
{
  // constructors
  optional() noexcept : _valid(false), _value() {}
  constexpr optional(nullopt_t) noexcept : _valid(false), _value() {}
  optional(const optional& rhs): _valid(rhs._valid), _value(rhs._value) { }
  optional(const T& rhs) : _valid(true), _value(rhs) { }
  template<typename Other>
  optional(const Other& rhs): _valid(true), _value(rhs) { }
  template<typename Other>
  optional(const optional<Other>& rhs): _valid(rhs.has_value()), _value(rhs.operator*()) { }

  // destructor
  ~optional() { }

  // check for value presence
  bool has_value() const { return _valid; }
  explicit operator bool() const { return _valid; }
  bool operator ! () const { return !_valid; }

  // assign
  optional& operator = (const optional& rhs) {
    _valid = rhs._valid;
    _value = rhs._value;
    return *this;
  }
  optional& operator = (const T& w) {
    _valid = true;
    _value = w;
    return *this;
  }
  // unchecked access to value
  T const& operator * () const & { return _value; }
  T const* operator -> () const { return &_value; }
  // checked access to value
  const T& value() const & {
    if (!_valid) {
      throw bad_optional_access{};
    }
    return _value;
  }
  T& value() & {
    if (!_valid) {
      throw bad_optional_access{};
    }
    return _value;
  }
private:
  bool _valid;
  T _value;
};

class InvalidLogicalTime : public std::logic_error
{
  public:
    InvalidLogicalTime(const std::string& what_arg) : std::logic_error(what_arg) {}
};

class NotConnected : public std::logic_error
{
  public:
    NotConnected() : std::logic_error("not connected") {}
};

class NotInitialized : public std::logic_error
{
  public:
    NotInitialized() : std::logic_error("class registry not initialized") {}
};

// extended receive order type, includes 'interpolated' for application-generated timestamps
enum class OrderType { RECEIVE, TIMESTAMP, INTERPOLATED };

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, OrderType value)
{
  switch (value)
  {
    case OrderType::RECEIVE: os << "RECEIVE"; break;
    case OrderType::TIMESTAMP: os << "TIMESTAMP"; break;
    case OrderType::INTERPOLATED: os << "INTERPOLATED"; break;
  }
  return os;
}

inline std::string to_string(OrderType value)
{
  switch (value)
  {
    case OrderType::RECEIVE: return "RECEIVE";
    case OrderType::TIMESTAMP: return "TIMESTAMP";
    case OrderType::INTERPOLATED: return "INTERPOLATED";
  }
}

inline std::wstring to_wstring(OrderType value)
{
  switch (value)
  {
    case OrderType::RECEIVE: return L"RECEIVE";
    case OrderType::TIMESTAMP: return L"TIMESTAMP";
    case OrderType::INTERPOLATED: return L"INTERPOLATED";
  }
}
class HLAinteractionSubscription;
class HLAobjectClassBasedCount;
class HLAinteractionCount;
class HLAsynchPointFederate;
typedef int32_t HLAcount;

typedef int32_t HLAseconds;

typedef int32_t HLAmsec;

// The type of the normalized value of a federate handle as returned by the Normalize Federate
//                Handle service. The value is appropriate for defining the range of the HLAfederate dimension for regions
//                with this dimension.
//             
typedef int32_t HLAnormalizedFederateHandle;

typedef int32_t HLAindex;

// Standardized 64 bit integer time
typedef int64_t HLAinteger64Time;

// Standardized 64 bit float time
typedef double HLAfloat64Time;

// State of the federate
enum class HLAfederateState : int32_t
{
      ActiveFederate = 1,
      FederateSaveInProgress = 3,
      FederateRestoreInProgress = 5,
}; // enum HLAfederateState

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, HLAfederateState value)
{
  switch (value)
  {
    case HLAfederateState::ActiveFederate: os << "ActiveFederate"; break;
    case HLAfederateState::FederateSaveInProgress: os << "FederateSaveInProgress"; break;
    case HLAfederateState::FederateRestoreInProgress: os << "FederateRestoreInProgress"; break;
    default: os << "<invalid HLAfederateState>"; break;
  }
  return os;
}

inline std::string to_string(HLAfederateState value)
{
  switch (value)
  {
    case HLAfederateState::ActiveFederate: return "ActiveFederate";
    case HLAfederateState::FederateSaveInProgress: return "FederateSaveInProgress";
    case HLAfederateState::FederateRestoreInProgress: return "FederateRestoreInProgress";
    default: return "<invalid HLAfederateState>";
  }
}

inline std::wstring to_wstring(HLAfederateState value)
{
  switch (value)
  {
    case HLAfederateState::ActiveFederate: return L"ActiveFederate";
    case HLAfederateState::FederateSaveInProgress: return L"FederateSaveInProgress";
    case HLAfederateState::FederateRestoreInProgress: return L"FederateRestoreInProgress";
    default: return L"<invalid HLAfederateState>";
  }
}

// State of time advancement
enum class HLAtimeState : int32_t
{
      TimeGranted = 0,
      TimeAdvancing = 1,
}; // enum HLAtimeState

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, HLAtimeState value)
{
  switch (value)
  {
    case HLAtimeState::TimeGranted: os << "TimeGranted"; break;
    case HLAtimeState::TimeAdvancing: os << "TimeAdvancing"; break;
    default: os << "<invalid HLAtimeState>"; break;
  }
  return os;
}

inline std::string to_string(HLAtimeState value)
{
  switch (value)
  {
    case HLAtimeState::TimeGranted: return "TimeGranted";
    case HLAtimeState::TimeAdvancing: return "TimeAdvancing";
    default: return "<invalid HLAtimeState>";
  }
}

inline std::wstring to_wstring(HLAtimeState value)
{
  switch (value)
  {
    case HLAtimeState::TimeGranted: return L"TimeGranted";
    case HLAtimeState::TimeAdvancing: return L"TimeAdvancing";
    default: return L"<invalid HLAtimeState>";
  }
}

enum class HLAownership : int32_t
{
      Unowned = 0,
      Owned = 1,
}; // enum HLAownership

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, HLAownership value)
{
  switch (value)
  {
    case HLAownership::Unowned: os << "Unowned"; break;
    case HLAownership::Owned: os << "Owned"; break;
    default: os << "<invalid HLAownership>"; break;
  }
  return os;
}

inline std::string to_string(HLAownership value)
{
  switch (value)
  {
    case HLAownership::Unowned: return "Unowned";
    case HLAownership::Owned: return "Owned";
    default: return "<invalid HLAownership>";
  }
}

inline std::wstring to_wstring(HLAownership value)
{
  switch (value)
  {
    case HLAownership::Unowned: return L"Unowned";
    case HLAownership::Owned: return L"Owned";
    default: return L"<invalid HLAownership>";
  }
}

// Action to be performed by RTI in conjunction with resignation
enum class HLAresignAction : int32_t
{
      DivestOwnership = 1,
      DeleteObjectInstances = 2,
      CancelPendingAcquisitions = 3,
      DeleteObjectInstancesThenDivestOwnership = 4,
      CancelPendingAcquisitionsThenDeleteObjectInstancesThenDivestOwnership = 5,
      NoAction = 6,
}; // enum HLAresignAction

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, HLAresignAction value)
{
  switch (value)
  {
    case HLAresignAction::DivestOwnership: os << "DivestOwnership"; break;
    case HLAresignAction::DeleteObjectInstances: os << "DeleteObjectInstances"; break;
    case HLAresignAction::CancelPendingAcquisitions: os << "CancelPendingAcquisitions"; break;
    case HLAresignAction::DeleteObjectInstancesThenDivestOwnership: os << "DeleteObjectInstancesThenDivestOwnership"; break;
    case HLAresignAction::CancelPendingAcquisitionsThenDeleteObjectInstancesThenDivestOwnership: os << "CancelPendingAcquisitionsThenDeleteObjectInstancesThenDivestOwnership"; break;
    case HLAresignAction::NoAction: os << "NoAction"; break;
    default: os << "<invalid HLAresignAction>"; break;
  }
  return os;
}

inline std::string to_string(HLAresignAction value)
{
  switch (value)
  {
    case HLAresignAction::DivestOwnership: return "DivestOwnership";
    case HLAresignAction::DeleteObjectInstances: return "DeleteObjectInstances";
    case HLAresignAction::CancelPendingAcquisitions: return "CancelPendingAcquisitions";
    case HLAresignAction::DeleteObjectInstancesThenDivestOwnership: return "DeleteObjectInstancesThenDivestOwnership";
    case HLAresignAction::CancelPendingAcquisitionsThenDeleteObjectInstancesThenDivestOwnership: return "CancelPendingAcquisitionsThenDeleteObjectInstancesThenDivestOwnership";
    case HLAresignAction::NoAction: return "NoAction";
    default: return "<invalid HLAresignAction>";
  }
}

inline std::wstring to_wstring(HLAresignAction value)
{
  switch (value)
  {
    case HLAresignAction::DivestOwnership: return L"DivestOwnership";
    case HLAresignAction::DeleteObjectInstances: return L"DeleteObjectInstances";
    case HLAresignAction::CancelPendingAcquisitions: return L"CancelPendingAcquisitions";
    case HLAresignAction::DeleteObjectInstancesThenDivestOwnership: return L"DeleteObjectInstancesThenDivestOwnership";
    case HLAresignAction::CancelPendingAcquisitionsThenDeleteObjectInstancesThenDivestOwnership: return L"CancelPendingAcquisitionsThenDeleteObjectInstancesThenDivestOwnership";
    case HLAresignAction::NoAction: return L"NoAction";
    default: return L"<invalid HLAresignAction>";
  }
}

// Order type to be used for sending attributes or interactions
enum class HLAorderType : int32_t
{
      Receive = 0,
      TimeStamp = 1,
}; // enum HLAorderType

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, HLAorderType value)
{
  switch (value)
  {
    case HLAorderType::Receive: os << "Receive"; break;
    case HLAorderType::TimeStamp: os << "TimeStamp"; break;
    default: os << "<invalid HLAorderType>"; break;
  }
  return os;
}

inline std::string to_string(HLAorderType value)
{
  switch (value)
  {
    case HLAorderType::Receive: return "Receive";
    case HLAorderType::TimeStamp: return "TimeStamp";
    default: return "<invalid HLAorderType>";
  }
}

inline std::wstring to_wstring(HLAorderType value)
{
  switch (value)
  {
    case HLAorderType::Receive: return L"Receive";
    case HLAorderType::TimeStamp: return L"TimeStamp";
    default: return L"<invalid HLAorderType>";
  }
}

enum class HLAswitch : int32_t
{
      Enabled = 1,
      Disabled = 0,
}; // enum HLAswitch

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, HLAswitch value)
{
  switch (value)
  {
    case HLAswitch::Enabled: os << "Enabled"; break;
    case HLAswitch::Disabled: os << "Disabled"; break;
    default: os << "<invalid HLAswitch>"; break;
  }
  return os;
}

inline std::string to_string(HLAswitch value)
{
  switch (value)
  {
    case HLAswitch::Enabled: return "Enabled";
    case HLAswitch::Disabled: return "Disabled";
    default: return "<invalid HLAswitch>";
  }
}

inline std::wstring to_wstring(HLAswitch value)
{
  switch (value)
  {
    case HLAswitch::Enabled: return L"Enabled";
    case HLAswitch::Disabled: return L"Disabled";
    default: return L"<invalid HLAswitch>";
  }
}

// Joined federate synchronization point status
enum class HLAsynchPointStatus : int32_t
{
      NoActivity = 0,
      AttemptingToRegisterSynchPoint = 1,
      MovingToSynchPoint = 2,
      WaitingForRestOfFederation = 3,
}; // enum HLAsynchPointStatus

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, HLAsynchPointStatus value)
{
  switch (value)
  {
    case HLAsynchPointStatus::NoActivity: os << "NoActivity"; break;
    case HLAsynchPointStatus::AttemptingToRegisterSynchPoint: os << "AttemptingToRegisterSynchPoint"; break;
    case HLAsynchPointStatus::MovingToSynchPoint: os << "MovingToSynchPoint"; break;
    case HLAsynchPointStatus::WaitingForRestOfFederation: os << "WaitingForRestOfFederation"; break;
    default: os << "<invalid HLAsynchPointStatus>"; break;
  }
  return os;
}

inline std::string to_string(HLAsynchPointStatus value)
{
  switch (value)
  {
    case HLAsynchPointStatus::NoActivity: return "NoActivity";
    case HLAsynchPointStatus::AttemptingToRegisterSynchPoint: return "AttemptingToRegisterSynchPoint";
    case HLAsynchPointStatus::MovingToSynchPoint: return "MovingToSynchPoint";
    case HLAsynchPointStatus::WaitingForRestOfFederation: return "WaitingForRestOfFederation";
    default: return "<invalid HLAsynchPointStatus>";
  }
}

inline std::wstring to_wstring(HLAsynchPointStatus value)
{
  switch (value)
  {
    case HLAsynchPointStatus::NoActivity: return L"NoActivity";
    case HLAsynchPointStatus::AttemptingToRegisterSynchPoint: return L"AttemptingToRegisterSynchPoint";
    case HLAsynchPointStatus::MovingToSynchPoint: return L"MovingToSynchPoint";
    case HLAsynchPointStatus::WaitingForRestOfFederation: return L"WaitingForRestOfFederation";
    default: return L"<invalid HLAsynchPointStatus>";
  }
}

// Service group identifier
enum class HLAnormalizedServiceGroup : int32_t
{
      FederationManagement = 0,
      DeclarationManagement = 1,
      ObjectManagement = 2,
      OwnershipManagement = 3,
      TimeManagement = 4,
      DataDistributionManagement = 5,
      SupportServices = 6,
}; // enum HLAnormalizedServiceGroup

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, HLAnormalizedServiceGroup value)
{
  switch (value)
  {
    case HLAnormalizedServiceGroup::FederationManagement: os << "FederationManagement"; break;
    case HLAnormalizedServiceGroup::DeclarationManagement: os << "DeclarationManagement"; break;
    case HLAnormalizedServiceGroup::ObjectManagement: os << "ObjectManagement"; break;
    case HLAnormalizedServiceGroup::OwnershipManagement: os << "OwnershipManagement"; break;
    case HLAnormalizedServiceGroup::TimeManagement: os << "TimeManagement"; break;
    case HLAnormalizedServiceGroup::DataDistributionManagement: os << "DataDistributionManagement"; break;
    case HLAnormalizedServiceGroup::SupportServices: os << "SupportServices"; break;
    default: os << "<invalid HLAnormalizedServiceGroup>"; break;
  }
  return os;
}

inline std::string to_string(HLAnormalizedServiceGroup value)
{
  switch (value)
  {
    case HLAnormalizedServiceGroup::FederationManagement: return "FederationManagement";
    case HLAnormalizedServiceGroup::DeclarationManagement: return "DeclarationManagement";
    case HLAnormalizedServiceGroup::ObjectManagement: return "ObjectManagement";
    case HLAnormalizedServiceGroup::OwnershipManagement: return "OwnershipManagement";
    case HLAnormalizedServiceGroup::TimeManagement: return "TimeManagement";
    case HLAnormalizedServiceGroup::DataDistributionManagement: return "DataDistributionManagement";
    case HLAnormalizedServiceGroup::SupportServices: return "SupportServices";
    default: return "<invalid HLAnormalizedServiceGroup>";
  }
}

inline std::wstring to_wstring(HLAnormalizedServiceGroup value)
{
  switch (value)
  {
    case HLAnormalizedServiceGroup::FederationManagement: return L"FederationManagement";
    case HLAnormalizedServiceGroup::DeclarationManagement: return L"DeclarationManagement";
    case HLAnormalizedServiceGroup::ObjectManagement: return L"ObjectManagement";
    case HLAnormalizedServiceGroup::OwnershipManagement: return L"OwnershipManagement";
    case HLAnormalizedServiceGroup::TimeManagement: return L"TimeManagement";
    case HLAnormalizedServiceGroup::DataDistributionManagement: return L"DataDistributionManagement";
    case HLAnormalizedServiceGroup::SupportServices: return L"SupportServices";
    default: return L"<invalid HLAnormalizedServiceGroup>";
  }
}

typedef std::wstring HLAtransportationName;

typedef std::wstring HLAupdateRateName;

// Interaction subscription information
class HLAinteractionSubscription
{
  public:
    virtual ~HLAinteractionSubscription() noexcept {}
    virtual uint32_t getVersion() const = 0;
    virtual void SetHLAinteractionClass(rti1516ev::HLAhandle value) = 0;
    virtual rti1516ev::HLAhandle GetHLAinteractionClass() const = 0;
    virtual bool IsHLAinteractionClassAvailable() const = 0;
    virtual void SetHLAactive(bool value) = 0;
    virtual bool GetHLAactive() const = 0;
    virtual bool IsHLAactiveAvailable() const = 0;

}; // class HLAinteractionSubscription

// Object class and count of associated items
class HLAobjectClassBasedCount
{
  public:
    virtual ~HLAobjectClassBasedCount() noexcept {}
    virtual uint32_t getVersion() const = 0;
    virtual void SetHLAobjectClass(rti1516ev::HLAhandle value) = 0;
    virtual rti1516ev::HLAhandle GetHLAobjectClass() const = 0;
    virtual bool IsHLAobjectClassAvailable() const = 0;
    virtual void SetHLAcount(HLAcount value) = 0;
    virtual HLAcount GetHLAcount() const = 0;
    virtual bool IsHLAcountAvailable() const = 0;

}; // class HLAobjectClassBasedCount

// Count of interactions of a class
class HLAinteractionCount
{
  public:
    virtual ~HLAinteractionCount() noexcept {}
    virtual uint32_t getVersion() const = 0;
    virtual void SetHLAinteractionClass(rti1516ev::HLAhandle value) = 0;
    virtual rti1516ev::HLAhandle GetHLAinteractionClass() const = 0;
    virtual bool IsHLAinteractionClassAvailable() const = 0;
    virtual void SetHLAinteractionCount(HLAcount value) = 0;
    virtual HLAcount GetHLAinteractionCount() const = 0;
    virtual bool IsHLAinteractionCountAvailable() const = 0;

}; // class HLAinteractionCount

// A particular joined federate and its synchronization point status
class HLAsynchPointFederate
{
  public:
    virtual ~HLAsynchPointFederate() noexcept {}
    virtual uint32_t getVersion() const = 0;
    virtual void SetHLAfederate(rti1516ev::HLAhandle value) = 0;
    virtual rti1516ev::HLAhandle GetHLAfederate() const = 0;
    virtual bool IsHLAfederateAvailable() const = 0;
    virtual void SetHLAfederateSynchStatus(HLAsynchPointStatus value) = 0;
    virtual HLAsynchPointStatus GetHLAfederateSynchStatus() const = 0;
    virtual bool IsHLAfederateSynchStatusAvailable() const = 0;

}; // class HLAsynchPointFederate


} // namespace OpenRTI
} // namespace Mom
