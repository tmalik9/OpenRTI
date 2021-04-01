
// Code automatically generated from D:\vfs\CANoe\release\15\Projects_Source\OpenRTI\tests\rti1516e\codegen\\mom.xml, do not edit

#pragma once

#include <vector>
#include <cstring>

#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAfixedRecord.h"
#include "RTI/encoding/HLAfixedArray.h"
#include "RTI/encoding/HLAvariableArray.h"
#include "RTI/encoding/HLAhandle.h"

#include "momDataTypes.h"

namespace OpenRTI {
namespace Mom {

class HLAinteractionSubscriptionEncoding;
class HLAobjectClassBasedCountEncoding;
class HLAinteractionCountEncoding;
class HLAsynchPointFederateEncoding;

// fixed array of simple type HLAbyte
class HLAtokenEncoding : public rti1516e::HLAfixedArray
{
  public:
    HLAtokenEncoding();
    // cardinality is 0, no settable contents
  private:
}; // class HLAtoken
// An encoded logical time. An empty array shall indicate that the values is not defined
//             
// variable array of simple type HLAbyte
class HLAlogicalTimeEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAlogicalTimeEncoding();
    HLAlogicalTimeEncoding(const std::vector<uint8_t>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<uint8_t>& data);
    void set(const uint8_t* data, size_t size);
    using rti1516e::HLAvariableArray::get;
    const std::vector<uint8_t>& get() const;
    void resize(size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<uint8_t> mData;
    std::vector<rti1516e::HLAbyte> mEncoding;
}; // class HLAlogicalTime

// An encoded logical time interval. An empty array shall indicate that the values is not defined
//             
// variable array of simple type HLAbyte
class HLAtimeIntervalEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAtimeIntervalEncoding();
    HLAtimeIntervalEncoding(const std::vector<uint8_t>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<uint8_t>& data);
    void set(const uint8_t* data, size_t size);
    using rti1516e::HLAvariableArray::get;
    const std::vector<uint8_t>& get() const;
    void resize(size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<uint8_t> mData;
    std::vector<rti1516e::HLAbyte> mEncoding;
}; // class HLAtimeInterval

// List of encoded handles
// variable array of complex type HLAhandle
class HLAhandleListEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAhandleListEncoding();
    HLAhandleListEncoding(const std::vector<rti1516e::HLAhandle>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<rti1516e::HLAhandle>& data);
    void set(std::vector<rti1516e::HLAhandle>& data);
    void set(const rti1516e::HLAhandle* data, size_t size);
    void set(rti1516e::HLAhandle* data, size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
}; // class HLAhandleList

// List of interaction subscription indicators
// variable array of complex type HLAinteractionSubscription
class HLAinteractionSubListEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAinteractionSubListEncoding();
    HLAinteractionSubListEncoding(const std::vector<HLAinteractionSubscriptionEncoding>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<HLAinteractionSubscriptionEncoding>& data);
    void set(std::vector<HLAinteractionSubscriptionEncoding>& data);
    void set(const HLAinteractionSubscriptionEncoding* data, size_t size);
    void set(HLAinteractionSubscriptionEncoding* data, size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
}; // class HLAinteractionSubList

// List of arguments
// variable array of simple type HLAunicodeString
class HLAargumentListEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAargumentListEncoding();
    HLAargumentListEncoding(const std::vector<std::wstring>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<std::wstring>& data);
    void set(const std::wstring* data, size_t size);
    using rti1516e::HLAvariableArray::get;
    const std::vector<std::wstring>& get() const;
    void resize(size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<std::wstring> mData;
    std::vector<rti1516e::HLAunicodeString> mEncoding;
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
class HLAobjectClassBasedCountsEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAobjectClassBasedCountsEncoding();
    HLAobjectClassBasedCountsEncoding(const std::vector<HLAobjectClassBasedCountEncoding>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<HLAobjectClassBasedCountEncoding>& data);
    void set(std::vector<HLAobjectClassBasedCountEncoding>& data);
    void set(const HLAobjectClassBasedCountEncoding* data, size_t size);
    void set(HLAobjectClassBasedCountEncoding* data, size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
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
class HLAinteractionCountsEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAinteractionCountsEncoding();
    HLAinteractionCountsEncoding(const std::vector<HLAinteractionCountEncoding>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<HLAinteractionCountEncoding>& data);
    void set(std::vector<HLAinteractionCountEncoding>& data);
    void set(const HLAinteractionCountEncoding* data, size_t size);
    void set(HLAinteractionCountEncoding* data, size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
}; // class HLAinteractionCounts

// List of names of synchronization points.
// variable array of simple type HLAunicodeString
class HLAsynchPointListEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAsynchPointListEncoding();
    HLAsynchPointListEncoding(const std::vector<std::wstring>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<std::wstring>& data);
    void set(const std::wstring* data, size_t size);
    using rti1516e::HLAvariableArray::get;
    const std::vector<std::wstring>& get() const;
    void resize(size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<std::wstring> mData;
    std::vector<rti1516e::HLAunicodeString> mEncoding;
}; // class HLAsynchPointList

// List of joined federates and the synchronization status of each.
// variable array of complex type HLAsynchPointFederate
class HLAsynchPointFederateListEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAsynchPointFederateListEncoding();
    HLAsynchPointFederateListEncoding(const std::vector<HLAsynchPointFederateEncoding>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<HLAsynchPointFederateEncoding>& data);
    void set(std::vector<HLAsynchPointFederateEncoding>& data);
    void set(const HLAsynchPointFederateEncoding* data, size_t size);
    void set(HLAsynchPointFederateEncoding* data, size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
}; // class HLAsynchPointFederateList

// List of designators of FOM modules.
// variable array of simple type HLAunicodeString
class HLAmoduleDesignatorListEncoding : public rti1516e::HLAvariableArray
{
  public:
    HLAmoduleDesignatorListEncoding();
    HLAmoduleDesignatorListEncoding(const std::vector<std::wstring>& data);
    using rti1516e::HLAvariableArray::set;
    void set(const std::vector<std::wstring>& data);
    void set(const std::wstring* data, size_t size);
    using rti1516e::HLAvariableArray::get;
    const std::vector<std::wstring>& get() const;
    void resize(size_t size);
    virtual size_t decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<std::wstring> mData;
    std::vector<rti1516e::HLAunicodeString> mEncoding;
}; // class HLAmoduleDesignatorList

// Interaction subscription information
class HLAinteractionSubscriptionEncoding : public HLAinteractionSubscription, public rti1516e::HLAfixedRecord
{
  public:
    HLAinteractionSubscriptionEncoding();
    virtual ~HLAinteractionSubscriptionEncoding();
    uint32_t getVersion() const override;
    void SetHLAinteractionClass(rti1516e::HLAhandle value) override;
    rti1516e::HLAhandle GetHLAinteractionClass() const override;
    bool IsHLAinteractionClassAvailable() const override;
    void SetHLAactive(bool value) override;
    bool GetHLAactive() const override;
    bool IsHLAactiveAvailable() const override;
  private:
    rti1516e::HLAhandle mHLAinteractionClass;
    rti1516e::HLAboolean mHLAactive;
}; // class HLAinteractionSubscriptionEncoding

// Test.
// fixed array of complex type HLAinteractionSubscription
class HLAinteractionSubscriptionFixedListEncoding : public rti1516e::HLAfixedArray
{
  public:
    HLAinteractionSubscriptionFixedListEncoding();
    HLAinteractionSubscriptionFixedListEncoding(const std::vector<HLAinteractionSubscriptionEncoding>& data);
    using rti1516e::HLAfixedArray::set;
    void set(const std::vector<HLAinteractionSubscriptionEncoding>& data);
    void set(const HLAinteractionSubscription* data, size_t size);
    // not yet supported: const std::vector<HLAinteractionSubscriptionEncoding>& get() const;
  private:
}; // class HLAinteractionSubscriptionFixedList
// Object class and count of associated items
class HLAobjectClassBasedCountEncoding : public HLAobjectClassBasedCount, public rti1516e::HLAfixedRecord
{
  public:
    HLAobjectClassBasedCountEncoding();
    virtual ~HLAobjectClassBasedCountEncoding();
    uint32_t getVersion() const override;
    void SetHLAobjectClass(rti1516e::HLAhandle value) override;
    rti1516e::HLAhandle GetHLAobjectClass() const override;
    bool IsHLAobjectClassAvailable() const override;
    void SetHLAcount(HLAcount value) override;
    HLAcount GetHLAcount() const override;
    bool IsHLAcountAvailable() const override;
  private:
    rti1516e::HLAhandle mHLAobjectClass;
    rti1516e::HLAinteger32BE mHLAcount;
}; // class HLAobjectClassBasedCountEncoding

// Count of interactions of a class
class HLAinteractionCountEncoding : public HLAinteractionCount, public rti1516e::HLAfixedRecord
{
  public:
    HLAinteractionCountEncoding();
    virtual ~HLAinteractionCountEncoding();
    uint32_t getVersion() const override;
    void SetHLAinteractionClass(rti1516e::HLAhandle value) override;
    rti1516e::HLAhandle GetHLAinteractionClass() const override;
    bool IsHLAinteractionClassAvailable() const override;
    void SetHLAinteractionCount(HLAcount value) override;
    HLAcount GetHLAinteractionCount() const override;
    bool IsHLAinteractionCountAvailable() const override;
  private:
    rti1516e::HLAhandle mHLAinteractionClass;
    rti1516e::HLAinteger32BE mHLAinteractionCount;
}; // class HLAinteractionCountEncoding

// A particular joined federate and its synchronization point status
class HLAsynchPointFederateEncoding : public HLAsynchPointFederate, public rti1516e::HLAfixedRecord
{
  public:
    HLAsynchPointFederateEncoding();
    virtual ~HLAsynchPointFederateEncoding();
    uint32_t getVersion() const override;
    void SetHLAfederate(rti1516e::HLAhandle value) override;
    rti1516e::HLAhandle GetHLAfederate() const override;
    bool IsHLAfederateAvailable() const override;
    void SetHLAfederateSynchStatus(HLAsynchPointStatus value) override;
    HLAsynchPointStatus GetHLAfederateSynchStatus() const override;
    bool IsHLAfederateSynchStatusAvailable() const override;
  private:
    rti1516e::HLAhandle mHLAfederate;
    rti1516e::HLAinteger32BE mHLAfederateSynchStatus;
}; // class HLAsynchPointFederateEncoding

} // namespace OpenRTI
} // namespace Mom

