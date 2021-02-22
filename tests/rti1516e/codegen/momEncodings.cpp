
// Code automatically generated from D:\vfs\CANoe\release\15\Projects_Source\OpenRTI\tests\rti1516e\codegen\mom.xml, do not edit

#pragma once

#include <vector>
#include <cstring>

#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAfixedRecord.h"
#include "RTI/encoding/HLAfixedArray.h"
#include "RTI/encoding/HLAvariableArray.h"
#include "RTI/encoding/HLAhandle.h"
#include "momEncodings.h"

namespace OpenRTI {
namespace Mom {

class HLAinteractionSubscriptionEncoding;
class HLAobjectClassBasedCountEncoding;
class HLAinteractionCountEncoding;
class HLAsynchPointFederateEncoding;

// fixed array of HLAbyte
HLAtokenEncoding::HLAtokenEncoding()
  : HLAfixedArray(rti1516e::HLAbyte(), 0)
{
}
// String whose legal value shall be a name from any row in the OMT transportation table (IEEE Std
//                1516.2-2010)
//             
// variable array of HLAunicodeChar
HLAtransportationNameEncoding::HLAtransportationNameEncoding()
  : HLAvariableArray(rti1516e::HLAunicodeChar())
  , mData()
{
}
HLAtransportationNameEncoding::HLAtransportationNameEncoding(const std::vector<wchar_t>& data)
  : HLAvariableArray(rti1516e::HLAunicodeChar())
  , mData(data)
{
  update();
}
void HLAtransportationNameEncoding::set(const std::vector<wchar_t>& data)
{
  if (mData.size() != data.size()) {
    mData = data;
    update();
  } else {
    memcpy(mData.data(), data.data(), mData.size());
  }
}
void HLAtransportationNameEncoding::set(const wchar_t* data, size_t size)
{
  if (mData.size() != size) {
    mData.resize(size);
    update();
  }
  memcpy(mData.data(), data, size);
}
const std::vector<wchar_t>& HLAtransportationNameEncoding::get() const { return mData; }
void HLAtransportationNameEncoding::resize(size_t size)
{
  mData.resize(size);
  update();
}
// resize buffer and encoder array before actually decoding 
size_t HLAtransportationNameEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
void HLAtransportationNameEncoding::update()
{
  size_t size = mData.size();
  mEncoding.resize(size);
  for (size_t i=0; i<size; i++)
  {
    mEncoding[i].setDataPointer(&mData.data()[i]);
    if (i < HLAvariableArray::size()) {
      setElementPointer(i, &mEncoding[i]);
    } else {
      addElementPointer(&mEncoding[i]);
    }
  }
}

// String whose legal value shall be a name from any row in the OMT update rate table (IEEE Std
//                1516.2-2010)
//             
// variable array of HLAunicodeChar
HLAupdateRateNameEncoding::HLAupdateRateNameEncoding()
  : HLAvariableArray(rti1516e::HLAunicodeChar())
  , mData()
{
}
HLAupdateRateNameEncoding::HLAupdateRateNameEncoding(const std::vector<wchar_t>& data)
  : HLAvariableArray(rti1516e::HLAunicodeChar())
  , mData(data)
{
  update();
}
void HLAupdateRateNameEncoding::set(const std::vector<wchar_t>& data)
{
  if (mData.size() != data.size()) {
    mData = data;
    update();
  } else {
    memcpy(mData.data(), data.data(), mData.size());
  }
}
void HLAupdateRateNameEncoding::set(const wchar_t* data, size_t size)
{
  if (mData.size() != size) {
    mData.resize(size);
    update();
  }
  memcpy(mData.data(), data, size);
}
const std::vector<wchar_t>& HLAupdateRateNameEncoding::get() const { return mData; }
void HLAupdateRateNameEncoding::resize(size_t size)
{
  mData.resize(size);
  update();
}
// resize buffer and encoder array before actually decoding 
size_t HLAupdateRateNameEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
void HLAupdateRateNameEncoding::update()
{
  size_t size = mData.size();
  mEncoding.resize(size);
  for (size_t i=0; i<size; i++)
  {
    mEncoding[i].setDataPointer(&mData.data()[i]);
    if (i < HLAvariableArray::size()) {
      setElementPointer(i, &mEncoding[i]);
    } else {
      addElementPointer(&mEncoding[i]);
    }
  }
}

// An encoded logical time. An empty array shall indicate that the values is not defined
//             
// variable array of HLAbyte
HLAlogicalTimeEncoding::HLAlogicalTimeEncoding()
  : HLAvariableArray(rti1516e::HLAbyte())
  , mData()
{
}
HLAlogicalTimeEncoding::HLAlogicalTimeEncoding(const std::vector<uint8_t>& data)
  : HLAvariableArray(rti1516e::HLAbyte())
  , mData(data)
{
  update();
}
void HLAlogicalTimeEncoding::set(const std::vector<uint8_t>& data)
{
  if (mData.size() != data.size()) {
    mData = data;
    update();
  } else {
    memcpy(mData.data(), data.data(), mData.size());
  }
}
void HLAlogicalTimeEncoding::set(const uint8_t* data, size_t size)
{
  if (mData.size() != size) {
    mData.resize(size);
    update();
  }
  memcpy(mData.data(), data, size);
}
const std::vector<uint8_t>& HLAlogicalTimeEncoding::get() const { return mData; }
void HLAlogicalTimeEncoding::resize(size_t size)
{
  mData.resize(size);
  update();
}
// resize buffer and encoder array before actually decoding 
size_t HLAlogicalTimeEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
void HLAlogicalTimeEncoding::update()
{
  size_t size = mData.size();
  mEncoding.resize(size);
  for (size_t i=0; i<size; i++)
  {
    mEncoding[i].setDataPointer(&mData.data()[i]);
    if (i < HLAvariableArray::size()) {
      setElementPointer(i, &mEncoding[i]);
    } else {
      addElementPointer(&mEncoding[i]);
    }
  }
}

// An encoded logical time interval. An empty array shall indicate that the values is not defined
//             
// variable array of HLAbyte
HLAtimeIntervalEncoding::HLAtimeIntervalEncoding()
  : HLAvariableArray(rti1516e::HLAbyte())
  , mData()
{
}
HLAtimeIntervalEncoding::HLAtimeIntervalEncoding(const std::vector<uint8_t>& data)
  : HLAvariableArray(rti1516e::HLAbyte())
  , mData(data)
{
  update();
}
void HLAtimeIntervalEncoding::set(const std::vector<uint8_t>& data)
{
  if (mData.size() != data.size()) {
    mData = data;
    update();
  } else {
    memcpy(mData.data(), data.data(), mData.size());
  }
}
void HLAtimeIntervalEncoding::set(const uint8_t* data, size_t size)
{
  if (mData.size() != size) {
    mData.resize(size);
    update();
  }
  memcpy(mData.data(), data, size);
}
const std::vector<uint8_t>& HLAtimeIntervalEncoding::get() const { return mData; }
void HLAtimeIntervalEncoding::resize(size_t size)
{
  mData.resize(size);
  update();
}
// resize buffer and encoder array before actually decoding 
size_t HLAtimeIntervalEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
void HLAtimeIntervalEncoding::update()
{
  size_t size = mData.size();
  mEncoding.resize(size);
  for (size_t i=0; i<size; i++)
  {
    mEncoding[i].setDataPointer(&mData.data()[i]);
    if (i < HLAvariableArray::size()) {
      setElementPointer(i, &mEncoding[i]);
    } else {
      addElementPointer(&mEncoding[i]);
    }
  }
}

// List of encoded handles
// variable array of HLAhandle
HLAhandleListEncoding::HLAhandleListEncoding()
  : HLAvariableArray(rti1516e::HLAhandle())
{
}
HLAhandleListEncoding::HLAhandleListEncoding(const std::vector<rti1516e::HLAhandle>& data)
  : HLAvariableArray(rti1516e::HLAhandle())
{
  set(data);
}
void HLAhandleListEncoding::set(const std::vector<rti1516e::HLAhandle>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElement(static_cast<const rti1516e::HLAhandle&>(data[i]));
  }
}
void HLAhandleListEncoding::set(std::vector<rti1516e::HLAhandle>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElementPointer(static_cast<rti1516e::HLAhandle*>(&data[i]));
  }
}
void HLAhandleListEncoding::set(const rti1516e::HLAhandle* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElement(static_cast<const rti1516e::HLAhandle&>(data[i]));
  }
}
void HLAhandleListEncoding::set(rti1516e::HLAhandle* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElementPointer(static_cast<rti1516e::HLAhandle*>(&data[i]));
  }
}
// resize buffer and encoder array before actually decoding 
size_t HLAhandleListEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
// List of interaction subscription indicators
// variable array of HLAinteractionSubscription
HLAinteractionSubListEncoding::HLAinteractionSubListEncoding()
  : HLAvariableArray(HLAinteractionSubscriptionEncoding())
{
}
HLAinteractionSubListEncoding::HLAinteractionSubListEncoding(const std::vector<HLAinteractionSubscriptionEncoding>& data)
  : HLAvariableArray(HLAinteractionSubscriptionEncoding())
{
  set(data);
}
void HLAinteractionSubListEncoding::set(const std::vector<HLAinteractionSubscriptionEncoding>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElement(static_cast<const HLAinteractionSubscriptionEncoding&>(data[i]));
  }
}
void HLAinteractionSubListEncoding::set(std::vector<HLAinteractionSubscriptionEncoding>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElementPointer(static_cast<HLAinteractionSubscriptionEncoding*>(&data[i]));
  }
}
void HLAinteractionSubListEncoding::set(const HLAinteractionSubscriptionEncoding* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElement(static_cast<const HLAinteractionSubscriptionEncoding&>(data[i]));
  }
}
void HLAinteractionSubListEncoding::set(HLAinteractionSubscriptionEncoding* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElementPointer(static_cast<HLAinteractionSubscriptionEncoding*>(&data[i]));
  }
}
// resize buffer and encoder array before actually decoding 
size_t HLAinteractionSubListEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
// List of arguments
// variable array of HLAunicodeString
HLAargumentListEncoding::HLAargumentListEncoding()
  : HLAvariableArray(rti1516e::HLAunicodeString())
  , mData()
{
}
HLAargumentListEncoding::HLAargumentListEncoding(const std::vector<std::wstring>& data)
  : HLAvariableArray(rti1516e::HLAunicodeString())
  , mData(data)
{
  update();
}
void HLAargumentListEncoding::set(const std::vector<std::wstring>& data)
{
  if (mData.size() != data.size()) {
    mData = data;
    update();
  } else {
    memcpy(mData.data(), data.data(), mData.size());
  }
}
void HLAargumentListEncoding::set(const std::wstring* data, size_t size)
{
  if (mData.size() != size) {
    mData.resize(size);
    update();
  }
  memcpy(mData.data(), data, size);
}
const std::vector<std::wstring>& HLAargumentListEncoding::get() const { return mData; }
void HLAargumentListEncoding::resize(size_t size)
{
  mData.resize(size);
  update();
}
// resize buffer and encoder array before actually decoding 
size_t HLAargumentListEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
void HLAargumentListEncoding::update()
{
  size_t size = mData.size();
  mEncoding.resize(size);
  for (size_t i=0; i<size; i++)
  {
    mEncoding[i].setDataPointer(&mData.data()[i]);
    if (i < HLAvariableArray::size()) {
      setElementPointer(i, &mEncoding[i]);
    } else {
      addElementPointer(&mEncoding[i]);
    }
  }
}

// List of counts of various items based on object class. In all MOM interactions that have a
//                parameter of datatype HLAobjectClassBased- Counts, if an HLAobjectClassBasedCount element of the
//                HLAobjectClassBasedCounts array would have a value (object class, 0), the HLAobjectClassBasedCount
//                element shall not be present in the HLAobjectClassBasedCounts array. In other words, only HLAobject-
//                ClassBasedCount elements that have positive counts shall be present in an HLAobjectClassBasedCounts
//                array. From this, it follows that if all object class counts have a zero value, then the HLAobjectClass-
//                BasedCounts array shall not have any elements in it; it shall be an empty HLAobjectClassBasedCounts
//                array.
//             
// variable array of HLAobjectClassBasedCount
HLAobjectClassBasedCountsEncoding::HLAobjectClassBasedCountsEncoding()
  : HLAvariableArray(HLAobjectClassBasedCountEncoding())
{
}
HLAobjectClassBasedCountsEncoding::HLAobjectClassBasedCountsEncoding(const std::vector<HLAobjectClassBasedCountEncoding>& data)
  : HLAvariableArray(HLAobjectClassBasedCountEncoding())
{
  set(data);
}
void HLAobjectClassBasedCountsEncoding::set(const std::vector<HLAobjectClassBasedCountEncoding>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElement(static_cast<const HLAobjectClassBasedCountEncoding&>(data[i]));
  }
}
void HLAobjectClassBasedCountsEncoding::set(std::vector<HLAobjectClassBasedCountEncoding>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElementPointer(static_cast<HLAobjectClassBasedCountEncoding*>(&data[i]));
  }
}
void HLAobjectClassBasedCountsEncoding::set(const HLAobjectClassBasedCountEncoding* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElement(static_cast<const HLAobjectClassBasedCountEncoding&>(data[i]));
  }
}
void HLAobjectClassBasedCountsEncoding::set(HLAobjectClassBasedCountEncoding* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElementPointer(static_cast<HLAobjectClassBasedCountEncoding*>(&data[i]));
  }
}
// resize buffer and encoder array before actually decoding 
size_t HLAobjectClassBasedCountsEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
// List of interaction counts. In all MOM interactions that have a parameter of datatype
//                HLAinteractionCounts, if an HLAinteractionCount element of the HLAinteractionCounts array would have a
//                value (interaction class, 0), the HLAinteractionCount element shall not be present in the
//                HLAinteractionCounts array. In other words, only HLAinteractionCount elements that have positive counts
//                shall be present in an HLAinteractionCounts array. From this, it follows that if all interaction class
//                counts have a zero value, then the HLAinteractionCounts array shall not have any elements in it; it shall
//                be an empty HLAinteractionCounts array.
//             
// variable array of HLAinteractionCount
HLAinteractionCountsEncoding::HLAinteractionCountsEncoding()
  : HLAvariableArray(HLAinteractionCountEncoding())
{
}
HLAinteractionCountsEncoding::HLAinteractionCountsEncoding(const std::vector<HLAinteractionCountEncoding>& data)
  : HLAvariableArray(HLAinteractionCountEncoding())
{
  set(data);
}
void HLAinteractionCountsEncoding::set(const std::vector<HLAinteractionCountEncoding>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElement(static_cast<const HLAinteractionCountEncoding&>(data[i]));
  }
}
void HLAinteractionCountsEncoding::set(std::vector<HLAinteractionCountEncoding>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElementPointer(static_cast<HLAinteractionCountEncoding*>(&data[i]));
  }
}
void HLAinteractionCountsEncoding::set(const HLAinteractionCountEncoding* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElement(static_cast<const HLAinteractionCountEncoding&>(data[i]));
  }
}
void HLAinteractionCountsEncoding::set(HLAinteractionCountEncoding* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElementPointer(static_cast<HLAinteractionCountEncoding*>(&data[i]));
  }
}
// resize buffer and encoder array before actually decoding 
size_t HLAinteractionCountsEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
// List of names of synchronization points.
// variable array of HLAunicodeString
HLAsynchPointListEncoding::HLAsynchPointListEncoding()
  : HLAvariableArray(rti1516e::HLAunicodeString())
  , mData()
{
}
HLAsynchPointListEncoding::HLAsynchPointListEncoding(const std::vector<std::wstring>& data)
  : HLAvariableArray(rti1516e::HLAunicodeString())
  , mData(data)
{
  update();
}
void HLAsynchPointListEncoding::set(const std::vector<std::wstring>& data)
{
  if (mData.size() != data.size()) {
    mData = data;
    update();
  } else {
    memcpy(mData.data(), data.data(), mData.size());
  }
}
void HLAsynchPointListEncoding::set(const std::wstring* data, size_t size)
{
  if (mData.size() != size) {
    mData.resize(size);
    update();
  }
  memcpy(mData.data(), data, size);
}
const std::vector<std::wstring>& HLAsynchPointListEncoding::get() const { return mData; }
void HLAsynchPointListEncoding::resize(size_t size)
{
  mData.resize(size);
  update();
}
// resize buffer and encoder array before actually decoding 
size_t HLAsynchPointListEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
void HLAsynchPointListEncoding::update()
{
  size_t size = mData.size();
  mEncoding.resize(size);
  for (size_t i=0; i<size; i++)
  {
    mEncoding[i].setDataPointer(&mData.data()[i]);
    if (i < HLAvariableArray::size()) {
      setElementPointer(i, &mEncoding[i]);
    } else {
      addElementPointer(&mEncoding[i]);
    }
  }
}

// List of joined federates and the synchronization status of each.
// variable array of HLAsynchPointFederate
HLAsynchPointFederateListEncoding::HLAsynchPointFederateListEncoding()
  : HLAvariableArray(HLAsynchPointFederateEncoding())
{
}
HLAsynchPointFederateListEncoding::HLAsynchPointFederateListEncoding(const std::vector<HLAsynchPointFederateEncoding>& data)
  : HLAvariableArray(HLAsynchPointFederateEncoding())
{
  set(data);
}
void HLAsynchPointFederateListEncoding::set(const std::vector<HLAsynchPointFederateEncoding>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElement(static_cast<const HLAsynchPointFederateEncoding&>(data[i]));
  }
}
void HLAsynchPointFederateListEncoding::set(std::vector<HLAsynchPointFederateEncoding>& data)
{
  clear();
  for (size_t i=0; i < data.size();i++)
  {
    addElementPointer(static_cast<HLAsynchPointFederateEncoding*>(&data[i]));
  }
}
void HLAsynchPointFederateListEncoding::set(const HLAsynchPointFederateEncoding* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElement(static_cast<const HLAsynchPointFederateEncoding&>(data[i]));
  }
}
void HLAsynchPointFederateListEncoding::set(HLAsynchPointFederateEncoding* data, size_t size)
{
  clear();
  for (size_t i=0; i < size;i++)
  {
    addElementPointer(static_cast<HLAsynchPointFederateEncoding*>(&data[i]));
  }
}
// resize buffer and encoder array before actually decoding 
size_t HLAsynchPointFederateListEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
// List of designators of FOM modules.
// variable array of HLAunicodeString
HLAmoduleDesignatorListEncoding::HLAmoduleDesignatorListEncoding()
  : HLAvariableArray(rti1516e::HLAunicodeString())
  , mData()
{
}
HLAmoduleDesignatorListEncoding::HLAmoduleDesignatorListEncoding(const std::vector<std::wstring>& data)
  : HLAvariableArray(rti1516e::HLAunicodeString())
  , mData(data)
{
  update();
}
void HLAmoduleDesignatorListEncoding::set(const std::vector<std::wstring>& data)
{
  if (mData.size() != data.size()) {
    mData = data;
    update();
  } else {
    memcpy(mData.data(), data.data(), mData.size());
  }
}
void HLAmoduleDesignatorListEncoding::set(const std::wstring* data, size_t size)
{
  if (mData.size() != size) {
    mData.resize(size);
    update();
  }
  memcpy(mData.data(), data, size);
}
const std::vector<std::wstring>& HLAmoduleDesignatorListEncoding::get() const { return mData; }
void HLAmoduleDesignatorListEncoding::resize(size_t size)
{
  mData.resize(size);
  update();
}
// resize buffer and encoder array before actually decoding 
size_t HLAmoduleDesignatorListEncoding::decodeFrom(const rti1516e::Octet* buffer, size_t bufferSize, size_t index)
{
  size_t newSize = decodedSize(buffer, bufferSize, index);
  resize(newSize);
  return HLAvariableArray::decodeFrom(buffer, bufferSize, index);
}
void HLAmoduleDesignatorListEncoding::update()
{
  size_t size = mData.size();
  mEncoding.resize(size);
  for (size_t i=0; i<size; i++)
  {
    mEncoding[i].setDataPointer(&mData.data()[i]);
    if (i < HLAvariableArray::size()) {
      setElementPointer(i, &mEncoding[i]);
    } else {
      addElementPointer(&mEncoding[i]);
    }
  }
}

// Interaction subscription information
HLAinteractionSubscriptionEncoding::HLAinteractionSubscriptionEncoding() : rti1516e::HLAfixedRecord(0)
{
  rti1516e::HLAfixedRecord::appendElementPointer(&mHLAinteractionClass, 0);
  rti1516e::HLAfixedRecord::appendElementPointer(&mHLAactive, 0);
}
HLAinteractionSubscriptionEncoding::~HLAinteractionSubscriptionEncoding()
{
}
uint32_t HLAinteractionSubscriptionEncoding::getVersion() const { return rti1516e::HLAfixedRecord::getVersion(); }
void HLAinteractionSubscriptionEncoding::SetHLAinteractionClass(rti1516e::HLAhandle value)
{
	mHLAinteractionClass = value;
}
rti1516e::HLAhandle HLAinteractionSubscriptionEncoding::GetHLAinteractionClass() const
{
  return mHLAinteractionClass;
}
bool HLAinteractionSubscriptionEncoding::IsHLAinteractionClassAvailable() const
{
  return true;
}
void HLAinteractionSubscriptionEncoding::SetHLAactive(bool value)
{
	mHLAactive.set(value);
}
bool HLAinteractionSubscriptionEncoding::GetHLAactive() const
{
  return mHLAactive.get();
}
bool HLAinteractionSubscriptionEncoding::IsHLAactiveAvailable() const
{
  return true;
}
// Object class and count of associated items
HLAobjectClassBasedCountEncoding::HLAobjectClassBasedCountEncoding() : rti1516e::HLAfixedRecord(0)
{
  rti1516e::HLAfixedRecord::appendElementPointer(&mHLAobjectClass, 0);
  rti1516e::HLAfixedRecord::appendElementPointer(&mHLAcount, 0);
}
HLAobjectClassBasedCountEncoding::~HLAobjectClassBasedCountEncoding()
{
}
uint32_t HLAobjectClassBasedCountEncoding::getVersion() const { return rti1516e::HLAfixedRecord::getVersion(); }
void HLAobjectClassBasedCountEncoding::SetHLAobjectClass(rti1516e::HLAhandle value)
{
	mHLAobjectClass = value;
}
rti1516e::HLAhandle HLAobjectClassBasedCountEncoding::GetHLAobjectClass() const
{
  return mHLAobjectClass;
}
bool HLAobjectClassBasedCountEncoding::IsHLAobjectClassAvailable() const
{
  return true;
}
void HLAobjectClassBasedCountEncoding::SetHLAcount(HLAcount value)
{
	mHLAcount.set(value);
}
HLAcount HLAobjectClassBasedCountEncoding::GetHLAcount() const
{
  return mHLAcount.get();
}
bool HLAobjectClassBasedCountEncoding::IsHLAcountAvailable() const
{
  return true;
}
// Count of interactions of a class
HLAinteractionCountEncoding::HLAinteractionCountEncoding() : rti1516e::HLAfixedRecord(0)
{
  rti1516e::HLAfixedRecord::appendElementPointer(&mHLAinteractionClass, 0);
  rti1516e::HLAfixedRecord::appendElementPointer(&mHLAinteractionCount, 0);
}
HLAinteractionCountEncoding::~HLAinteractionCountEncoding()
{
}
uint32_t HLAinteractionCountEncoding::getVersion() const { return rti1516e::HLAfixedRecord::getVersion(); }
void HLAinteractionCountEncoding::SetHLAinteractionClass(rti1516e::HLAhandle value)
{
	mHLAinteractionClass = value;
}
rti1516e::HLAhandle HLAinteractionCountEncoding::GetHLAinteractionClass() const
{
  return mHLAinteractionClass;
}
bool HLAinteractionCountEncoding::IsHLAinteractionClassAvailable() const
{
  return true;
}
void HLAinteractionCountEncoding::SetHLAinteractionCount(HLAcount value)
{
	mHLAinteractionCount.set(value);
}
HLAcount HLAinteractionCountEncoding::GetHLAinteractionCount() const
{
  return mHLAinteractionCount.get();
}
bool HLAinteractionCountEncoding::IsHLAinteractionCountAvailable() const
{
  return true;
}
// A particular joined federate and its synchronization point status
HLAsynchPointFederateEncoding::HLAsynchPointFederateEncoding() : rti1516e::HLAfixedRecord(0)
{
  rti1516e::HLAfixedRecord::appendElementPointer(&mHLAfederate, 0);
  rti1516e::HLAfixedRecord::appendElementPointer(&mHLAfederateSynchStatus, 0);
}
HLAsynchPointFederateEncoding::~HLAsynchPointFederateEncoding()
{
}
uint32_t HLAsynchPointFederateEncoding::getVersion() const { return rti1516e::HLAfixedRecord::getVersion(); }
void HLAsynchPointFederateEncoding::SetHLAfederate(rti1516e::HLAhandle value)
{
	mHLAfederate = value;
}
rti1516e::HLAhandle HLAsynchPointFederateEncoding::GetHLAfederate() const
{
  return mHLAfederate;
}
bool HLAsynchPointFederateEncoding::IsHLAfederateAvailable() const
{
  return true;
}
void HLAsynchPointFederateEncoding::SetHLAfederateSynchStatus(HLAsynchPointStatus value)
{
	mHLAfederateSynchStatus = value;
}
HLAsynchPointStatus HLAsynchPointFederateEncoding::GetHLAfederateSynchStatus() const
{
  return static_cast<HLAsynchPointStatus>(mHLAfederateSynchStatus.get());
}
bool HLAsynchPointFederateEncoding::IsHLAfederateSynchStatusAvailable() const
{
  return true;
}
} // namespace OpenRTI
} // namespace Mom

