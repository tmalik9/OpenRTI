
// Code automatically generated from mom.xml, do not edit

#pragma once

#include <vector>
#include <cstring>

#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAfixedRecord.h"
#include "RTI/encoding/HLAfixedArray.h"
#include "RTI/encoding/HLAvariableArray.h"
#include "RTI/encoding/HLAhandle.h"
#include "RTI/encoding/HLAopaqueData.h"

#include "momDataTypes.h"

namespace OpenRTI {
namespace Mom {

class HLAinteractionSubscriptionEncoding;
class HLAobjectClassBasedCountEncoding;
class HLAinteractionCountEncoding;
class HLAsynchPointFederateEncoding;

// simple data type HLAcount
using HLAcountEncoding=rti1516ev::HLAinteger32BE;
// simple data type HLAseconds
using HLAsecondsEncoding=rti1516ev::HLAinteger32BE;
// simple data type HLAmsec
using HLAmsecEncoding=rti1516ev::HLAinteger32BE;
// simple data type HLAnormalizedFederateHandle
using HLAnormalizedFederateHandleEncoding=rti1516ev::HLAinteger32BE;
// simple data type HLAindex
using HLAindexEncoding=rti1516ev::HLAinteger32BE;
// simple data type HLAinteger64Time
using HLAinteger64TimeEncoding=rti1516ev::HLAinteger64BE;
// simple data type HLAfloat64Time
using HLAfloat64TimeEncoding=rti1516ev::HLAfloat64BE;
// enumerated data type HLAfederateState
using HLAfederateStateEncoding=rti1516ev::HLAinteger32BE;
// enumerated data type HLAtimeState
using HLAtimeStateEncoding=rti1516ev::HLAinteger32BE;
// enumerated data type HLAownership
using HLAownershipEncoding=rti1516ev::HLAinteger32BE;
// enumerated data type HLAresignAction
using HLAresignActionEncoding=rti1516ev::HLAinteger32BE;
// enumerated data type HLAorderType
using HLAorderTypeEncoding=rti1516ev::HLAinteger32BE;
// enumerated data type HLAswitch
using HLAswitchEncoding=rti1516ev::HLAinteger32BE;
// enumerated data type HLAsynchPointStatus
using HLAsynchPointStatusEncoding=rti1516ev::HLAinteger32BE;
// enumerated data type HLAnormalizedServiceGroup
using HLAnormalizedServiceGroupEncoding=rti1516ev::HLAinteger32BE;
// fixed array of simple type HLAbyte
class HLAtokenEncoding : public rti1516ev::HLAfixedArray
{
  public:
    HLAtokenEncoding();
    // cardinality is 0, no settable contents
  private:
}; // class HLAtoken
// simple data type HLAtransportationName
using HLAtransportationNameEncoding=rti1516ev::HLAunicodeString;
// simple data type HLAupdateRateName
using HLAupdateRateNameEncoding=rti1516ev::HLAunicodeString;
// An encoded logical time. An empty array shall indicate that the values is not defined
//             
// variable array of simple type HLAbyte
class HLAlogicalTimeEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAlogicalTimeEncoding();
    HLAlogicalTimeEncoding(const std::vector<uint8_t>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<uint8_t>& data);
    void set(const uint8_t* data, size_t size);
    using rti1516ev::HLAvariableArray::get;
    const std::vector<uint8_t>& get() const;
    void resize(size_t size);
    using rti1516ev::HLAvariableArray::decodeFrom;
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<uint8_t> mData;
    std::vector<rti1516ev::HLAbyte> mEncoding;
}; // class HLAlogicalTime

// An encoded logical time interval. An empty array shall indicate that the values is not defined
//             
// variable array of simple type HLAbyte
class HLAtimeIntervalEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAtimeIntervalEncoding();
    HLAtimeIntervalEncoding(const std::vector<uint8_t>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<uint8_t>& data);
    void set(const uint8_t* data, size_t size);
    using rti1516ev::HLAvariableArray::get;
    const std::vector<uint8_t>& get() const;
    void resize(size_t size);
    using rti1516ev::HLAvariableArray::decodeFrom;
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<uint8_t> mData;
    std::vector<rti1516ev::HLAbyte> mEncoding;
}; // class HLAtimeInterval

// List of encoded handles
// variable array of complex type HLAhandle
class HLAhandleListEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAhandleListEncoding();
    HLAhandleListEncoding(const std::vector<rti1516ev::HLAhandle>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<rti1516ev::HLAhandle>& data);
    void set(std::vector<rti1516ev::HLAhandle>& data);
    void set(const rti1516ev::HLAhandle* data, size_t size);
    void set(rti1516ev::HLAhandle* data, size_t size);
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
}; // class HLAhandleList

// List of interaction subscription indicators
// variable array of complex type HLAinteractionSubscription
class HLAinteractionSubListEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAinteractionSubListEncoding();
    HLAinteractionSubListEncoding(const std::vector<HLAinteractionSubscriptionEncoding>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<HLAinteractionSubscriptionEncoding>& data);
    void set(std::vector<HLAinteractionSubscriptionEncoding>& data);
    void set(const HLAinteractionSubscriptionEncoding* data, size_t size);
    void set(HLAinteractionSubscriptionEncoding* data, size_t size);
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
}; // class HLAinteractionSubList

// List of arguments
// variable array of simple type HLAunicodeString
class HLAargumentListEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAargumentListEncoding();
    HLAargumentListEncoding(const std::vector<std::wstring>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<std::wstring>& data);
    void set(const std::wstring* data, size_t size);
    using rti1516ev::HLAvariableArray::get;
    const std::vector<std::wstring>& get() const;
    void resize(size_t size);
    using rti1516ev::HLAvariableArray::decodeFrom;
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<std::wstring> mData;
    std::vector<rti1516ev::HLAunicodeString> mEncoding;
}; // class HLAargumentList

// List of counts of various items based on object class. In all MOM interactions that have a
//                parameter of datatype HLAobjectClassBased- Counts, if an HLAobjectClassBasedCount element of the
//                HLAobjectClassBasedCounts array would have a value (object class, 0), the HLAobjectClassBasedCount
//                element shall not be present in the HLAobjectClassBasedCounts array. In other words, only HLAobject-
//                ClassBasedCount elements that have positive counts shall be present in an HLAobjectClassBasedCounts
//                array. From this, it follows that if all object class counts have a zero value, then the HLAobjectClass-
//                BasedCounts array shall not have any elements in it; it shall be an empty HLAobjectClassBasedCounts
//                array.
//             
// variable array of complex type HLAobjectClassBasedCount
class HLAobjectClassBasedCountsEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAobjectClassBasedCountsEncoding();
    HLAobjectClassBasedCountsEncoding(const std::vector<HLAobjectClassBasedCountEncoding>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<HLAobjectClassBasedCountEncoding>& data);
    void set(std::vector<HLAobjectClassBasedCountEncoding>& data);
    void set(const HLAobjectClassBasedCountEncoding* data, size_t size);
    void set(HLAobjectClassBasedCountEncoding* data, size_t size);
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
}; // class HLAobjectClassBasedCounts

// List of interaction counts. In all MOM interactions that have a parameter of datatype
//                HLAinteractionCounts, if an HLAinteractionCount element of the HLAinteractionCounts array would have a
//                value (interaction class, 0), the HLAinteractionCount element shall not be present in the
//                HLAinteractionCounts array. In other words, only HLAinteractionCount elements that have positive counts
//                shall be present in an HLAinteractionCounts array. From this, it follows that if all interaction class
//                counts have a zero value, then the HLAinteractionCounts array shall not have any elements in it; it shall
//                be an empty HLAinteractionCounts array.
//             
// variable array of complex type HLAinteractionCount
class HLAinteractionCountsEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAinteractionCountsEncoding();
    HLAinteractionCountsEncoding(const std::vector<HLAinteractionCountEncoding>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<HLAinteractionCountEncoding>& data);
    void set(std::vector<HLAinteractionCountEncoding>& data);
    void set(const HLAinteractionCountEncoding* data, size_t size);
    void set(HLAinteractionCountEncoding* data, size_t size);
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
}; // class HLAinteractionCounts

// List of names of synchronization points.
// variable array of simple type HLAunicodeString
class HLAsynchPointListEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAsynchPointListEncoding();
    HLAsynchPointListEncoding(const std::vector<std::wstring>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<std::wstring>& data);
    void set(const std::wstring* data, size_t size);
    using rti1516ev::HLAvariableArray::get;
    const std::vector<std::wstring>& get() const;
    void resize(size_t size);
    using rti1516ev::HLAvariableArray::decodeFrom;
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<std::wstring> mData;
    std::vector<rti1516ev::HLAunicodeString> mEncoding;
}; // class HLAsynchPointList

// List of joined federates and the synchronization status of each.
// variable array of complex type HLAsynchPointFederate
class HLAsynchPointFederateListEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAsynchPointFederateListEncoding();
    HLAsynchPointFederateListEncoding(const std::vector<HLAsynchPointFederateEncoding>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<HLAsynchPointFederateEncoding>& data);
    void set(std::vector<HLAsynchPointFederateEncoding>& data);
    void set(const HLAsynchPointFederateEncoding* data, size_t size);
    void set(HLAsynchPointFederateEncoding* data, size_t size);
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
}; // class HLAsynchPointFederateList

// List of designators of FOM modules.
// variable array of simple type HLAunicodeString
class HLAmoduleDesignatorListEncoding : public rti1516ev::HLAvariableArray
{
  public:
    HLAmoduleDesignatorListEncoding();
    HLAmoduleDesignatorListEncoding(const std::vector<std::wstring>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<std::wstring>& data);
    void set(const std::wstring* data, size_t size);
    using rti1516ev::HLAvariableArray::get;
    const std::vector<std::wstring>& get() const;
    void resize(size_t size);
    using rti1516ev::HLAvariableArray::decodeFrom;
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<std::wstring> mData;
    std::vector<rti1516ev::HLAunicodeString> mEncoding;
}; // class HLAmoduleDesignatorList

// Interaction subscription information
class HLAinteractionSubscriptionEncoding : public HLAinteractionSubscription, public rti1516ev::HLAfixedRecord
{
  public:
    HLAinteractionSubscriptionEncoding();
    virtual ~HLAinteractionSubscriptionEncoding();
    uint32_t getVersion() const override;
    void SetHLAinteractionClass(rti1516ev::HLAhandle value) override;
    rti1516ev::HLAhandle GetHLAinteractionClass() const override;
    bool IsHLAinteractionClassAvailable() const override;
    void SetHLAactive(bool value) override;
    bool GetHLAactive() const override;
    bool IsHLAactiveAvailable() const override;
  private:
    rti1516ev::HLAhandle mHLAinteractionClass;
    rti1516ev::HLAboolean mHLAactive;
}; // class HLAinteractionSubscriptionEncoding

// Test.
// fixed array of complex type HLAinteractionSubscription
class HLAinteractionSubscriptionFixedListEncoding : public rti1516ev::HLAfixedArray
{
  public:
    HLAinteractionSubscriptionFixedListEncoding();
    HLAinteractionSubscriptionFixedListEncoding(const std::vector<HLAinteractionSubscriptionEncoding>& data);
    using rti1516ev::HLAfixedArray::set;
    void set(const std::vector<HLAinteractionSubscriptionEncoding>& data);
    void set(const HLAinteractionSubscription* data, size_t size);
    // not yet supported: const std::vector<HLAinteractionSubscriptionEncoding>& get() const;
  private:
}; // class HLAinteractionSubscriptionFixedList
// Object class and count of associated items
class HLAobjectClassBasedCountEncoding : public HLAobjectClassBasedCount, public rti1516ev::HLAfixedRecord
{
  public:
    HLAobjectClassBasedCountEncoding();
    virtual ~HLAobjectClassBasedCountEncoding();
    uint32_t getVersion() const override;
    void SetHLAobjectClass(rti1516ev::HLAhandle value) override;
    rti1516ev::HLAhandle GetHLAobjectClass() const override;
    bool IsHLAobjectClassAvailable() const override;
    void SetHLAcount(HLAcount value) override;
    HLAcount GetHLAcount() const override;
    bool IsHLAcountAvailable() const override;
  private:
    rti1516ev::HLAhandle mHLAobjectClass;
    rti1516ev::HLAinteger32BE mHLAcount;
}; // class HLAobjectClassBasedCountEncoding

// Count of interactions of a class
class HLAinteractionCountEncoding : public HLAinteractionCount, public rti1516ev::HLAfixedRecord
{
  public:
    HLAinteractionCountEncoding();
    virtual ~HLAinteractionCountEncoding();
    uint32_t getVersion() const override;
    void SetHLAinteractionClass(rti1516ev::HLAhandle value) override;
    rti1516ev::HLAhandle GetHLAinteractionClass() const override;
    bool IsHLAinteractionClassAvailable() const override;
    void SetHLAinteractionCount(HLAcount value) override;
    HLAcount GetHLAinteractionCount() const override;
    bool IsHLAinteractionCountAvailable() const override;
  private:
    rti1516ev::HLAhandle mHLAinteractionClass;
    rti1516ev::HLAinteger32BE mHLAinteractionCount;
}; // class HLAinteractionCountEncoding

// A particular joined federate and its synchronization point status
class HLAsynchPointFederateEncoding : public HLAsynchPointFederate, public rti1516ev::HLAfixedRecord
{
  public:
    HLAsynchPointFederateEncoding();
    virtual ~HLAsynchPointFederateEncoding();
    uint32_t getVersion() const override;
    void SetHLAfederate(rti1516ev::HLAhandle value) override;
    rti1516ev::HLAhandle GetHLAfederate() const override;
    bool IsHLAfederateAvailable() const override;
    void SetHLAfederateSynchStatus(HLAsynchPointStatus value) override;
    HLAsynchPointStatus GetHLAfederateSynchStatus() const override;
    bool IsHLAfederateSynchStatusAvailable() const override;
  private:
    rti1516ev::HLAhandle mHLAfederate;
    rti1516ev::HLAinteger32BE mHLAfederateSynchStatus;
}; // class HLAsynchPointFederateEncoding

} // namespace OpenRTI
} // namespace Mom

