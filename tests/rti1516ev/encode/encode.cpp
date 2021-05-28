/* -*-c++-*- OpenRTI - Copyright (C) 2017-2017 Mathias Froehlich
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

#include <algorithm>
#include <cstring>
#include <string>
#include <memory>

#include <iostream>

#include <RTI/VariableLengthData.h>
#include <RTI/encoding/BasicDataElements.h>
#include <RTI/encoding/HLAhandle.h>
#include <RTI/encoding/DataElement.h>
#include <RTI/encoding/EncodingExceptions.h>
#include <RTI/encoding/EncodingConfig.h>
#include <RTI/encoding/HLAfixedArray.h>
#include <RTI/encoding/HLAfixedRecord.h>
#include <RTI/encoding/HLAopaqueData.h>
#include <RTI/encoding/HLAvariableArray.h>
#include <RTI/encoding/HLAvariantRecord.h>

#include "../../../src/rti1516ev/HandleFactory.h"
#include "../../../src/OpenRTI/DebugNew.h"
#include "../../../src/rti1516ev/Encoding.h"

#include <RTI1516ETestLib.h>

#include "Rand.h"
#ifdef _WIN32
#include <crtdbg.h>
#endif

typedef rti1516ev::HLAinteger32BE HLAcount;

class OPENRTI_LOCAL HLAinteractionCount : public rti1516ev::HLAfixedRecord
{
  public:
    HLAinteractionCount() : rti1516ev::HLAfixedRecord(1)
    {
      HLAfixedRecord::appendElementPointer(&mClassHandle);
      HLAfixedRecord::appendElementPointer(&mCount);
    }

    HLAinteractionCount(rti1516ev::InteractionClassHandle classHandle, int32_t count)
      : mClassHandle(classHandle)
      , mCount(count)
    {
      HLAfixedRecord::appendElementPointer(&mClassHandle);
      HLAfixedRecord::appendElementPointer(&mCount);
    }
    HLAinteractionCount(const HLAinteractionCount& ref)
      : mClassHandle(ref.mClassHandle)
      , mCount(ref.mCount)
    {
      mClassHandle.set(ref.mClassHandle.getInteractionClassHandle());
      mCount.set(ref.mCount.get());
      HLAfixedRecord::appendElementPointer(&mClassHandle);
      HLAfixedRecord::appendElementPointer(&mCount);
    }

    std::unique_ptr<DataElement> clone() const override
    {
      return std::unique_ptr<DataElement>(new HLAinteractionCount(*this));
    }
    rti1516ev::InteractionClassHandle getClassHandle() const
    {
      return mClassHandle.getInteractionClassHandle();
    }

    int32_t getCount() const
    {
      return mCount.get();
    }

    void increment()
    {
      mCount.set(mCount.get() + 1);
    }
    void setCount(uint32_t count)
    {
      mCount.set(count);
    }
  private:
    void operator=(const HLAinteractionCount& ) = delete;

    //----------------------------------------------------------
    //                   INSTANCE VARIABLES
    //----------------------------------------------------------
    rti1516ev::HLAhandle mClassHandle;
    HLAcount mCount;
};


class OPENRTI_LOCAL HLAinteractionSubscription : public rti1516ev::HLAfixedRecord
{
  public:
    HLAinteractionSubscription(rti1516ev::InteractionClassHandle classHandle, bool active)
      : rti1516ev::HLAfixedRecord(1)
      , mClassHandle(classHandle)
      , mActive(active)
    {
      HLAfixedRecord::appendElementPointer(&mClassHandle);
      HLAfixedRecord::appendElementPointer(&mActive);
    }
    HLAinteractionSubscription(const HLAinteractionSubscription& ref)
      : mClassHandle(ref.mClassHandle)
      , mActive(ref.mActive)
    {
      mClassHandle.set(ref.mClassHandle.getInteractionClassHandle());
      mActive.set(ref.mActive.get());
      HLAfixedRecord::appendElementPointer(&mClassHandle);
      HLAfixedRecord::appendElementPointer(&mActive);
    }
    ~HLAinteractionSubscription()
    {
    }
    std::unique_ptr<DataElement> clone() const override
    {
      return std::unique_ptr<DataElement>(new HLAinteractionSubscription(*this));
    }
    rti1516ev::InteractionClassHandle getClassHandle() const
    {
      return mClassHandle.getInteractionClassHandle();
    }

    bool getActive() const
    {
      return mActive.get();
    }

    void setActive(bool active)
    {
      mActive.set(active);
    }
  private:
    void operator=(const HLAinteractionCount& ) = delete;

    //----------------------------------------------------------
    //                   INSTANCE VARIABLES
    //----------------------------------------------------------
    rti1516ev::HLAhandle mClassHandle;
    rti1516ev::HLAboolean mActive;
};


constexpr bool gDebugPrint = true;

#ifdef _WIN32
static void DumpHeap()
{
  _CrtDumpMemoryLeaks();
}
static int initHeapDump = atexit( DumpHeap);
#endif

struct DataType
{
  enum Type
  {
    // scalar data types
    HLAASCIIchar,
    HLAunicodeChar,
    HLAboolean,
    HLAbyte,
    HLAoctet,
    HLAoctetPairBE,
    HLAoctetPairLE,
    HLAinteger16LE,
    HLAinteger16BE,
    HLAinteger32BE,
    HLAinteger32LE,
    HLAinteger64BE,
    HLAinteger64LE,
    HLAfloat32BE,
    HLAfloat32LE,
    HLAfloat64BE,
    HLAfloat64LE,
    HLAfederateHandle,
    HLAobjectClassHandle,
    HLAobjectInstanceHandle,
    HLAattributeHandle,
    HLAinteractionClassHandle,
    HLAparameterHandle,

    // string data types
    HLAASCIIstring,
    HLAunicodeString,

    // aggregate data types
    HLAfixedArray,
    HLAvariableArray,
    HLAfixedRecord_Randomized,
    HLAfixedRecord_WithElementPointers
  };
  Type type;
  // depending on type this contains the nested data types
  std::size_t count;
  const DataType* dataTypes[8];

  std::size_t getAlignment() const;
  std::unique_ptr<rti1516ev::DataElement> createDataElement(OpenRTI::Rand& rand) const;
};

template<typename TInputIter>
static inline std::string make_hex_string(TInputIter first, TInputIter last, bool use_uppercase = true, bool insert_spaces = false)
{
  std::ostringstream ss;
  ss << std::hex << std::setfill('0');
  if (use_uppercase)
    ss << std::uppercase;
  while (first != last)
  {
    unsigned char currentByte = (*first++) & 0xff;
    ss << std::setw(2) << (static_cast<unsigned int>(currentByte) & 0x000000ff);
    if (insert_spaces && first != last)
      ss << " ";
  }
  return ss.str();
}

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, const std::vector<uint8_t>& value)
{
  if (value.size() == 0)
  {
    os << "<empty>";
  }
  else
  {
    os << "{sz=" << value.size() << " [" << make_hex_string(static_cast<const uint8_t*>(value.data()), static_cast<const uint8_t*>(value.data()) + value.size(), true, true) << "]}";
  }
  return os;
}

template<typename char_type, typename traits_type>
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& os, const rti1516ev::VariableLengthData& value)
{
  if (value.size() == 0)
  {
    os << "<empty>";
  }
  else
  {
    os << "{sz=" << value.size() << " [" << make_hex_string(static_cast<const uint8_t*>(value.data()), static_cast<const uint8_t*>(value.data()) + value.size(), true, true) << "]}";
  }
  return os;
}


std::string to_string(DataType::Type type)
{
  switch (type)
  {
    case DataType::HLAASCIIchar: return "HLAASCIIchar";
    case DataType::HLAunicodeChar: return "HLAunicodeChar";
    case DataType::HLAboolean: return "HLAboolean";
    case DataType::HLAbyte: return "HLAbyte";
    case DataType::HLAoctet: return "HLAoctet";
    case DataType::HLAoctetPairBE: return "HLAoctetPairBE";
    case DataType::HLAoctetPairLE: return "HLAoctetPairLE";
    case DataType::HLAinteger16LE: return "HLAinteger16LE";
    case DataType::HLAinteger16BE: return "HLAinteger16BE";
    case DataType::HLAinteger32BE: return "HLAinteger32BE";
    case DataType::HLAinteger32LE: return "HLAinteger32LE";
    case DataType::HLAinteger64BE: return "HLAinteger64BE";
    case DataType::HLAinteger64LE: return "HLAinteger64LE";
    case DataType::HLAfloat32BE: return "HLAfloat32BE";
    case DataType::HLAfloat32LE: return "HLAfloat32LE";
    case DataType::HLAfloat64BE: return "HLAfloat64BE";
    case DataType::HLAfloat64LE: return "HLAfloat64LE";
    case DataType::HLAASCIIstring: return "HLAASCIIstring";
    case DataType::HLAunicodeString: return "HLAunicodeString";
    case DataType::HLAfederateHandle: return "HLAfederateHandle";
    case DataType::HLAobjectClassHandle: return "HLAobjectClassHandle";
    case DataType::HLAobjectInstanceHandle: return "HLAobjectInstanceHandle";
    case DataType::HLAattributeHandle: return "HLAattributeHandle";
    case DataType::HLAinteractionClassHandle: return "HLAinteractionClassHandle";
    case DataType::HLAparameterHandle: return "HLAparameterHandle";
    case DataType::HLAfixedArray: return "HLAfixedArray";
    case DataType::HLAvariableArray: return "HLAvariableArray";
    case DataType::HLAfixedRecord_Randomized: return "HLAfixedRecord_Randomized";
    case DataType::HLAfixedRecord_WithElementPointers: return "HLAfixedRecord_WithElementPointers";

    default: return "unknown-type";
  }
}

std::size_t
DataType::getAlignment() const
{
  switch (type)
  {
    case HLAASCIIchar:
    case HLAunicodeChar:
    case HLAboolean:
    case HLAbyte:
    case HLAoctet:
    case HLAoctetPairBE:
    case HLAoctetPairLE:
    case HLAinteger16LE:
    case HLAinteger16BE:
    case HLAinteger32BE:
    case HLAinteger32LE:
    case HLAinteger64BE:
    case HLAinteger64LE:
    case HLAfloat32BE:
    case HLAfloat32LE:
    case HLAfloat64BE:
    case HLAfloat64LE:
      return count;

    case HLAASCIIstring:
    case HLAunicodeString:
      return 4;
    case HLAfederateHandle:
    case HLAobjectClassHandle:
    case HLAobjectInstanceHandle:
    case HLAattributeHandle:
    case HLAinteractionClassHandle:
    case HLAparameterHandle:
      return 4;

    case HLAfixedArray:
      return dataTypes[0]->getAlignment();
    case HLAvariableArray:
      return std::max(std::size_t(4), dataTypes[0]->getAlignment());
    case HLAfixedRecord_Randomized:
    {
      std::size_t alignment = 1;
      for (std::size_t i = 0; i < count; ++i)
        alignment = std::max(alignment, dataTypes[i]->getAlignment());
      return alignment;
    }

    case HLAfixedRecord_WithElementPointers:
    {
      std::size_t alignment = 1;
      for (std::size_t i = 0; i < count; ++i)
        alignment = std::max(alignment, dataTypes[i]->getAlignment());
      return alignment;
    }

    default:
      return 1;
  }
}

//template <typename T>
//struct MyRandom
//{
//  static inline T operator() {
//    return static_cast<T>(rand());
//  }
//};

//template <> struct MyRandom<std::string>
//{std::string operator(){std::string result;return result;}
//};

std::unique_ptr<rti1516ev::DataElement>
DataType::createDataElement(OpenRTI::Rand& rand) const
{
  switch (type)
  {
    case HLAASCIIchar:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAASCIIchar(static_cast<char>(rand.get())));
    case HLAunicodeChar:
      // under linux wchar_t is 32 bits but unicode char is only 16 bits ...
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAunicodeChar(0xffff & rand.get()));
    case HLAboolean:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAboolean(rand.get()));
    case HLAbyte:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAbyte(static_cast<unsigned char>(rand.get())));
    case HLAoctet:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAoctet(static_cast<unsigned char>(rand.get())));
    case HLAoctetPairBE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAoctetPairBE(rti1516ev::OctetPair(static_cast<unsigned char>(rand.get()), static_cast<unsigned char>(rand.get()))));
    case HLAoctetPairLE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAoctetPairLE(rti1516ev::OctetPair(static_cast<unsigned char>(rand.get()), static_cast<unsigned char>(rand.get()))));
    case HLAinteger16LE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAinteger16LE(static_cast<rti1516ev::Integer16>(rand.get())));
    case HLAinteger16BE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAinteger16BE(static_cast<rti1516ev::Integer16>(rand.get())));
    case HLAinteger32BE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAinteger32BE(rand.get()));
    case HLAinteger32LE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAinteger32LE(rand.get()));
    case HLAinteger64BE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAinteger64BE(rti1516ev::Integer64(rand.get()) | rti1516ev::Integer64(rand.get()) << 32));
    case HLAinteger64LE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAinteger64LE(rti1516ev::Integer64(rand.get()) | rti1516ev::Integer64(rand.get()) << 32));
    case HLAfloat32BE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAfloat32BE(static_cast<float>(rand.get())));
    case HLAfloat32LE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAfloat32LE(static_cast<float>(rand.get())));
    case HLAfloat64BE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAfloat64BE(static_cast<double>(rand.get())));
    case HLAfloat64LE:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAfloat64LE(static_cast<double>(rand.get())));

    case HLAASCIIstring:
    {
      std::string s;
      s.resize(0xff & rand.get());
      for (std::size_t i = 0; i < s.size(); ++i)
        s[i] = static_cast<char>(rand.get());
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAASCIIstring(s));
    }
    case HLAunicodeString:
    {
      std::wstring s;
      wchar_t chars[] = L"0123456789";
      s.resize(0xff & rand.get());
      for (std::size_t i = 0; i < s.size(); ++i)
        //s[i] = 0xffff & static_cast<uint16_t>(rand.get());
        s[i] = chars[i%10];
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAunicodeString(s));
    }

    case HLAfixedArray:
    {
      std::unique_ptr<rti1516ev::DataElement> dataElement(dataTypes[0]->createDataElement(rand));
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAfixedArray(*dataElement, count));
    }
    case HLAvariableArray:
    {
      std::unique_ptr<rti1516ev::DataElement> dataElement(dataTypes[0]->createDataElement(rand));
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAvariableArray(*dataElement));
    }
    case HLAfixedRecord_Randomized:
    {
      std::unique_ptr<rti1516ev::HLAfixedRecord> fixedRecord = std::unique_ptr<rti1516ev::HLAfixedRecord>(new rti1516ev::HLAfixedRecord());
      for (std::size_t i = 0; i < count; ++i)
      {
        std::unique_ptr<rti1516ev::DataElement> dataElement(dataTypes[i]->createDataElement(rand));
        fixedRecord->appendElement(*dataElement);
      }
      return fixedRecord;
    }

    case HLAfixedRecord_WithElementPointers:
    {
      std::unique_ptr<rti1516ev::HLAfixedRecord> fixedRecord = std::unique_ptr<rti1516ev::HLAfixedRecord>(new rti1516ev::HLAfixedRecord());
      for (std::size_t i = 0; i < count; ++i)
      {
        std::unique_ptr<rti1516ev::DataElement> dataElement(dataTypes[i]->createDataElement(rand));
        fixedRecord->appendElement(*dataElement);
      }
      return fixedRecord;
    }

    case HLAfederateHandle:
    {
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAhandle(rti1516ev::createFederateHandle(rand.get())));

    }
    case HLAobjectClassHandle:
    {
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAhandle(rti1516ev::createObjectClassHandle(rand.get())));
    }
    case HLAobjectInstanceHandle:
    {
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAhandle(rti1516ev::createObjectInstanceHandle(rand.get())));
    }
    case HLAattributeHandle:
    {
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAhandle(rti1516ev::createAttributeHandle(rand.get())));
    }
    case HLAinteractionClassHandle:
    {
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAhandle(rti1516ev::createInteractionClassHandle(rand.get())));
    }
    case HLAparameterHandle:
    {
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAhandle(rti1516ev::createParameterHandle(rand.get())));
    }
    default:
      return std::unique_ptr<rti1516ev::DataElement>(new rti1516ev::HLAopaqueData);
  }
}

bool
equal(const rti1516ev::DataElement& left, const rti1516ev::DataElement& right);

bool
equal(const rti1516ev::HLAASCIIchar& left, const rti1516ev::HLAASCIIchar& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAunicodeChar& left, const rti1516ev::HLAunicodeChar& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAboolean& left, const rti1516ev::HLAboolean& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAbyte& left, const rti1516ev::HLAbyte& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAoctet& left, const rti1516ev::HLAoctet& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAoctetPairBE& left, const rti1516ev::HLAoctetPairBE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAoctetPairLE& left, const rti1516ev::HLAoctetPairLE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAinteger16BE& left, const rti1516ev::HLAinteger16BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAinteger16LE& left, const rti1516ev::HLAinteger16LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAinteger32BE& left, const rti1516ev::HLAinteger32BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAinteger32LE& left, const rti1516ev::HLAinteger32LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAinteger64BE& left, const rti1516ev::HLAinteger64BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAinteger64LE& left, const rti1516ev::HLAinteger64LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAfloat32BE& left, const rti1516ev::HLAfloat32BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAfloat32LE& left, const rti1516ev::HLAfloat32LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAfloat64BE& left, const rti1516ev::HLAfloat64BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAfloat64LE& left, const rti1516ev::HLAfloat64LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAASCIIstring& left, const rti1516ev::HLAASCIIstring& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516ev::HLAunicodeString& left, const rti1516ev::HLAunicodeString& right)
{ return left.get() == right.get(); }

bool
equal(const rti1516ev::HLAhandle& left, const rti1516ev::HLAhandle& right)
{
  return   left.getFederateHandle() == right.getFederateHandle()
           && left.getObjectClassHandle() == right.getObjectClassHandle()
           && left.getObjectInstanceHandle() == right.getObjectInstanceHandle()
           && left.getAttributeHandle() == right.getAttributeHandle()
           && left.getInteractionClassHandle() == right.getInteractionClassHandle()
           && left.getParameterHandle() == right.getParameterHandle();
}

bool
equal(const rti1516ev::HLAfixedArray& left, const rti1516ev::HLAfixedArray& right)
{
  if (left.size() != right.size())
  {
    std::cerr << __FUNCTION__ << ": HLAfixedArray sizes differ: " << left.size() << " != " << right.size() << std::endl;
    return false;
  }
  for (std::size_t i = 0; i < left.size(); ++i)
  {
    if (!equal(left.get(i), right.get(i)))
    {
      std::cerr << __FUNCTION__ << ": " << i << "'th elements differ" << std::endl;
      return false;
    }
  }
  return true;
}
bool
equal(const rti1516ev::HLAvariableArray& left, const rti1516ev::HLAvariableArray& right)
{
  if (left.size() != right.size())
    return false;
  for (std::size_t i = 0; i < left.size(); ++i)
  {
    if (!equal(left.get(i), right.get(i)))
      return false;
  }
  return true;
}
bool
equal(const rti1516ev::HLAfixedRecord& left, const rti1516ev::HLAfixedRecord& right)
{
  if (left.size() != right.size())
    return false;
  for (std::size_t i = 0; i < left.size(); ++i)
  {
    if (!equal(left.get(i), right.get(i)))
      return false;
  }
  return true;
}

bool
equal(const rti1516ev::HLAopaqueData& left, const rti1516ev::HLAopaqueData& right)
{
  if (left.dataLength() != right.dataLength())
    return false;
  return 0 == std::memcmp(left.get().data(), right.get().data(), left.dataLength());
}

bool
equal(const rti1516ev::VariableLengthData& left, const rti1516ev::VariableLengthData& right)
{
  if (left.size() != right.size())
    return false;
  return 0 == std::memcmp(left.data(), right.data(), left.size());
}

size_t
diff(const rti1516ev::VariableLengthData& left, const rti1516ev::VariableLengthData& right)
{
  size_t compareBytes = std::min(left.size(), right.size());
  for (size_t i=0;i<compareBytes;i++)
  {
    if (static_cast<const uint8_t*>(left.data())[i] != static_cast<const uint8_t*>(right.data())[i])
    {
      return i;
    }
  }
  return compareBytes;
}

template<typename T>
bool
typedEqual(const rti1516ev::DataElement& left, const rti1516ev::DataElement& right)
{
  try
  {
    return equal(dynamic_cast<const T&>(left), dynamic_cast<const T&>(right));
  }
  catch (...)
  {
    return false;
  }
}

bool
equal(const rti1516ev::DataElement& left, const rti1516ev::DataElement& right)
{
  // this one throws std::bad_cast ... very often
  if (typedEqual<rti1516ev::HLAASCIIchar>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAunicodeChar>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAboolean>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAbyte>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAoctet>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAoctetPairBE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAoctetPairLE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAinteger16BE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAinteger16LE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAinteger32BE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAinteger32LE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAinteger64BE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAinteger64LE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAfloat32BE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAfloat32LE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAfloat64BE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAfloat64LE>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAASCIIstring>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAunicodeString>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAunicodeString>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAhandle>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAfixedArray>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAvariableArray>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAfixedRecord>(left, right))
    return true;
  if (typedEqual<rti1516ev::HLAopaqueData>(left, right))
    return true;
  return false;
}

void DumpDataElement(std::wostream& to, const rti1516ev::DataElement* dataElement, uint32_t indent = 0)
{
  to << std::wstring(indent * 2, L' ') << OpenRTI::to_wstring(typeid(*dataElement).name()) 
    << L" encodedLength=" << dataElement->getEncodedLength()
    << L" align=" << dataElement->getOctetBoundary();
  const rti1516ev::HLAfixedRecord* rec = dynamic_cast<const rti1516ev::HLAfixedRecord*>(dataElement);
  if (rec != nullptr)
  {
    to << std::endl;
    for (size_t i = 0; i < rec->size(); i++)
    {
      DumpDataElement(to, &(rec->get(i)), indent + 1);
    }
    return;
  }
  const rti1516ev::HLAvariableArray* va = dynamic_cast<const rti1516ev::HLAvariableArray*>(dataElement);
  if (va != nullptr)
  {
    to << " size=" << va->size() << std::endl;
    for (size_t i = 0; i < va->size(); i++)
    {
      DumpDataElement(to, &(va->get(i)), indent + 1);
    }
    return;
  }
  const rti1516ev::HLAfixedArray* fa = dynamic_cast<const rti1516ev::HLAfixedArray*>(dataElement);
  if (fa != nullptr)
  {
    to << " size=" << va->size() << std::endl;
    for (size_t i = 0; i < fa->size(); i++)
    {
      DumpDataElement(to, &(fa->get(i)), indent + 1);
    }
    return;
  }
  const rti1516ev::HLAunicodeString* s = dynamic_cast<const rti1516ev::HLAunicodeString*>(dataElement);
  if (s != nullptr)
  {
    std::wcerr << L" ==> " << s->get().size() << " \"" << s->get() << L"\"" << std::endl;
  }
  to << std::endl;
}

bool testDataElementEncoding(const DataType& dataType)
{
  OpenRTI::Rand rand;
  std::cerr << "testing " << to_string(dataType.type) << std::endl;
  for (unsigned i = 0; i < 100; ++i)
  {
    std::unique_ptr<rti1516ev::DataElement> dataElement(dataType.createDataElement(rand));
    if (!dataElement.get())
    {
      std::cerr << "Failed to create a rti1516ev::DataElement to test with!" << std::endl;
      return false;
    }
    //try
    //{
    //  dataElement->decode(rti1516ev::VariableLengthData());
    //}
    //catch (const rti1516ev::EncoderException& /*e*/)
    //{
    //  // Ok, shall happen here
    //}
    //catch (...)
    //{
    //  std::cerr << "Unexpected exception while decoding empty data!" << std::endl;
    //  return false;
    //}
    rti1516ev::VariableLengthData encodedData;
    try
    {
      encodedData = dataElement->encode();
    }
    catch (const rti1516ev::Exception& e)
    {
      std::cerr << "Unexpected exception while encoding: " << e << std::endl;
      return false;
    }
    catch (...)
    {
      std::cerr << "Unexpected exception while encoding!" << std::endl;
      return false;
    }
    if (dataElement->getEncodedLength() != encodedData.size())
    {
      std::cerr << "getEncodedLength != encodedData.size: " << dataElement->getEncodedLength() << " != " << encodedData.size() << std::endl;
      std::cerr << encodedData << std::endl;
    }
    std::unique_ptr<rti1516ev::DataElement> dataElement2 = dataElement->clone();
    try
    {
      dataElement2->decode(encodedData);
    }
    catch (const rti1516ev::Exception& e)
    {
      std::cerr << "Unexpected exception while encoding: " << e << std::endl;
      return false;
    }
    catch (...)
    {
      std::cerr << "Unexpected exception while decoding!" << std::endl;
      return false;
    }

    if (!equal(*dataElement, *dataElement2))
    {
      std::cerr << "Data elements are not equal!" << std::endl;
      return false;
    }

    rti1516ev::VariableLengthData variableLengthData2;
    try
    {
      variableLengthData2 = dataElement->encode();
    }
    catch (...)
    {
      std::cerr << "Unexpected exception while encoding!" << std::endl;
      return false;
    }

    if (!equal(encodedData, variableLengthData2))
    {
      size_t pos = diff(encodedData, variableLengthData2);
      std::cerr << to_string(dataType.type) << ": cloned encoded data is not equal to original encoded data at pos " << pos << ": " << std::endl;
      std::cerr << "original=" << encodedData << std::endl;
      DumpDataElement(std::wcerr, dataElement.get());
      std::cerr << "  cloned=" << variableLengthData2 << std::endl;
      DumpDataElement(std::wcerr, dataElement2.get());
      return false;
    }
  }
  std::cerr << to_string(dataType.type) << " OK" << std::endl;
  return true;
}


static const DataType HLAASCIIcharDataType =
{
  DataType::HLAASCIIchar, 1,
};
static const DataType HLAunicodeCharDataType =
{
  DataType::HLAunicodeChar, 2,
};
static const DataType HLAbooleanDataType =
{
  DataType::HLAboolean, 1,
};
static const DataType HLAbyteDataType =
{
  DataType::HLAbyte, 1,
};
static const DataType HLAoctetDataType =
{
  DataType::HLAoctet, 1,
};
static const DataType HLAoctetPairBEDataType =
{
  DataType::HLAoctetPairBE, 2,
};
static const DataType HLAoctetPairLEDataType =
{
  DataType::HLAoctetPairLE, 2,
};
static const DataType HLAinteger16BEDataType =
{
  DataType::HLAinteger16BE, 2,
};
static const DataType HLAinteger16LEDataType =
{
  DataType::HLAinteger16LE, 2,
};
static const DataType HLAinteger32BEDataType =
{
  DataType::HLAinteger32BE, 4,
};
static const DataType HLAinteger32LEDataType =
{
  DataType::HLAinteger32LE, 4,
};
static const DataType HLAinteger64BEDataType =
{
  DataType::HLAinteger64BE, 8,
};
static const DataType HLAinteger64LEDataType =
{
  DataType::HLAinteger64LE, 8,
};
static const DataType HLAfloat32BEDataType =
{
  DataType::HLAfloat32BE, 4,
};
static const DataType HLAfloat32LEDataType =
{
  DataType::HLAfloat32LE, 4,
};
static const DataType HLAfloat64BEDataType =
{
  DataType::HLAfloat64BE, 8,
};
static const DataType HLAfloat64LEDataType =
{
  DataType::HLAfloat64LE, 8,
};

static const DataType HLAASCIIstringDataType =
{
  DataType::HLAASCIIstring, 8,
};
static const DataType HLAunicodeStringDataType =
{
  DataType::HLAunicodeString, 8,
};


static const DataType HLAfederateHandleDataType =
{
  DataType::HLAfederateHandle, 8,
};
static const DataType HLAobjectClassHandleDataType =
{
  DataType::HLAobjectClassHandle, 8,
};
static const DataType HLAobjectInstanceHandleDataType =
{
  DataType::HLAobjectInstanceHandle, 8,
};

static const DataType Vec3fDataType =
{
  DataType::HLAfixedArray, 3, { &HLAfloat32BEDataType, }
};
static const DataType Vec3dDataType =
{
  DataType::HLAfixedArray, 3, { &HLAfloat64BEDataType, }
};

static const DataType VecfDataType =
{
  DataType::HLAvariableArray, 17, { &HLAfloat32LEDataType, }
};
static const DataType VecdDataType =
{
  DataType::HLAvariableArray, 17, { &HLAfloat64LEDataType, }
};

static const DataType GeodfDataType =
{
  DataType::HLAfixedRecord_Randomized, 3, { &HLAfloat32LEDataType, &HLAfloat32LEDataType, &HLAfloat32LEDataType, }
};
static const DataType GeoddDataType =
{
  DataType::HLAfixedRecord_Randomized, 3, { &HLAfloat64LEDataType, &HLAfloat64LEDataType, &HLAfloat64LEDataType, }
};

static const DataType StructAlignDataType =
{
  DataType::HLAfixedRecord_Randomized, 2, { &HLAfloat32LEDataType, &HLAfloat64LEDataType, }
};
static const DataType StructAlign2DataType =
{
  DataType::HLAfixedRecord_Randomized, 3, { &HLAinteger16LEDataType, &StructAlignDataType, &HLAunicodeStringDataType, }
};

bool testFederateHandleEncodings(OpenRTI::RTI1516ESimpleAmbassador& ambassador, rti1516ev::FederateHandle federateHandle)
{
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing " << federateHandle << std::endl;
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  rti1516ev::HLAhandle encodedHandle = rti1516ev::HLAhandle(federateHandle);
  // encoded data from HLAhandle
  rti1516ev::VariableLengthData data1 = encodedHandle.encode();
  if (gDebugPrint) std::cout << "data1=" << data1 << std::endl;
  // encoded data from rti1516ev::FederateHandle
  rti1516ev::VariableLengthData data2 = federateHandle.encode();
  if (gDebugPrint) std::cout << "data2=" << data2 << std::endl;

  // check the basic sizes of the HLAhandle ...
  if (encodedHandle.getEncodedLength() != 12)
  {
    std::wcerr << L"unexpected retval from HLAhandle::getEncodedLength: " << encodedHandle.getEncodedLength() << std::endl;
    return false;
  }
  if (data1.size() != 12)
  {
    std::wcerr << L"unexpected encoded data size from HLAhandle: " << data1.size() << std::endl;
    return false;
  }
  // ... and the FederateHandle
  if (federateHandle.encodedLength() != 8)
  {
    std::wcerr << L"unexpected retval from rti1516ev::FederateHandle::encodedLength: " << federateHandle.encodedLength() << std::endl;
    return false;
  }
  if (data2.size() != 8)
  {
    std::wcerr << L"unexpected encoded data size from rti1516ev::FederateHandle::encode: " << data2.size() << std::endl;
    return false;
  }

  if (encodedHandle.getFederateHandle() != federateHandle)
  {
    std::wcerr << L"handle stored in HLAhandle differs from input: " << encodedHandle.getFederateHandle() << " != " << federateHandle << std::endl;
    return false;
  }

  // let the ambassador decode the data stored in HLAhandle (after the handle kind discriminator)
  rti1516ev::VariableLengthData data3 = rti1516ev::VariableLengthData(static_cast<const char*>(data1.data()) + 4, federateHandle.encodedLength());
  rti1516ev::FederateHandle decodedHandle = ambassador.decodeFederateHandle(data3);

  if (decodedHandle != federateHandle)
  {
    std::wcerr << L"handle decoded by ambassador differs from input: " << federateHandle << L" != " << decodedHandle << std::endl;
    return false;
  }
  std::wcout << federateHandle << L" OK" << std::endl;
  return true;
}

bool testObjectClassHandleEncodings(OpenRTI::RTI1516ESimpleAmbassador& ambassador, rti1516ev::ObjectClassHandle objectClassHandle)
{
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing " << ambassador.getObjectClassName(objectClassHandle) << std::endl;
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  rti1516ev::HLAhandle encodedHandle = rti1516ev::HLAhandle(objectClassHandle);
  // encoded data from HLAhandle
  rti1516ev::VariableLengthData data1;
  try
  {
    data1 = encodedHandle.encode();
    if (gDebugPrint) std::cout << "data1=" << data1 << std::endl;
  }
  catch (const rti1516ev::Exception& e)
  {
    std::cerr << "Unexpected exception while encoding: " << e << std::endl;
    return false;
  }
  catch (...)
  {
    std::cerr << "Unexpected exception while encoding!" << std::endl;
    return false;
  }
  // encoded data from rti1516ev::ObjectClassHandle
  rti1516ev::VariableLengthData data2;
  try
  {
    data2 = objectClassHandle.encode();
  }
  catch (const rti1516ev::Exception& e)
  {
    std::cerr << "Unexpected exception while encoding: " << e << std::endl;
    return false;
  }
  catch (...)
  {
    std::cerr << "Unexpected exception while encoding!" << std::endl;
    return false;
  }

  if (gDebugPrint) std::cout << "data2=" << data2 << std::endl;

  // check the basic sizes of the HLAhandle ...
  if (encodedHandle.getEncodedLength() != 12)
  {
    std::wcerr << L"unexpected retval from HLAhandle::getEncodedLength: " << encodedHandle.getEncodedLength() << std::endl;
    return false;
  }
  if (data1.size() != 12)
  {
    std::wcerr << L"unexpected encoded data size from HLAhandle: " << data1.size() << std::endl;
    return false;
  }
  // ... and the ObjectClassHandle
  if (objectClassHandle.encodedLength() != 8)
  {
    std::wcerr << L"unexpected retval from rti1516ev::ObjectClassHandle::encodedLength: " << objectClassHandle.encodedLength() << std::endl;
    return false;
  }
  if (data2.size() != 8)
  {
    std::wcerr << L"unexpected encoded data size from rti1516ev::ObjectClassHandle::encode: " << data1.size() << std::endl;
    return false;
  }

  if (encodedHandle.getObjectClassHandle() != objectClassHandle)
  {
    std::wcerr << L"handle stored in HLAhandle differs from input: " << encodedHandle.getObjectClassHandle() << " != " << objectClassHandle << std::endl;
    return false;
  }

  // let the ambassador decode the data stored in HLAhandle (after the handle kind discriminator)
  rti1516ev::VariableLengthData data3 = rti1516ev::VariableLengthData(static_cast<const char*>(data1.data()) + 4, objectClassHandle.encodedLength());
  rti1516ev::ObjectClassHandle decodedHandle = ambassador.decodeObjectClassHandle(data3);

  if (decodedHandle != objectClassHandle)
  {
    std::wcerr << L"handle decoded by ambassador differs from input: " << objectClassHandle << L" != " << decodedHandle << std::endl;
    return false;
  }
  std::wcout << ambassador.getObjectClassName(objectClassHandle) << L" OK" << std::endl;
  return true;
}

bool testInteractionClassHandleEncodings(OpenRTI::RTI1516ESimpleAmbassador& ambassador, rti1516ev::InteractionClassHandle interactionClassHandle)
{
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing interaction class" << ambassador.getInteractionClassName(interactionClassHandle)  << std::endl;
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  rti1516ev::HLAhandle encodedHandle = rti1516ev::HLAhandle(interactionClassHandle);
  // encoded data from HLAhandle
  rti1516ev::VariableLengthData data1 = encodedHandle.encode();
  if (gDebugPrint) std::cout << "data1=" << data1 << std::endl;
  // encoded data from rti1516ev::InteractionClassHandle
  rti1516ev::VariableLengthData data2 = interactionClassHandle.encode();
  if (gDebugPrint) std::cout << "data2=" << data2 << std::endl;

  // check the basic sizes of the HLAhandle ...
  if (encodedHandle.getEncodedLength() != 12)
  {
    std::wcerr << L"unexpected retval from HLAhandle::getEncodedLength: " << encodedHandle.getEncodedLength() << std::endl;
    return false;
  }
  if (data1.size() != 12)
  {
    std::wcerr << L"unexpected encoded data size from HLAhandle: " << data1.size() << std::endl;
    return false;
  }
  // ... and the InteractionClassHandle
  if (interactionClassHandle.encodedLength() != 8)
  {
    std::wcerr << L"unexpected retval from rti1516ev::InteractionClassHandle::encodedLength: " << interactionClassHandle.encodedLength() << std::endl;
    return false;
  }
  if (data2.size() != 8)
  {
    std::wcerr << L"unexpected encoded data size from rti1516ev::InteractionClassHandle::encode: " << data1.size() << std::endl;
    return false;
  }

  if (encodedHandle.getInteractionClassHandle() != interactionClassHandle)
  {
    std::wcerr << L"handle stored in HLAhandle differs from input: " << encodedHandle.getInteractionClassHandle() << " != " << interactionClassHandle << std::endl;
    return false;
  }

  // let the ambassador decode the data stored in HLAhandle (after the handle kind discriminator)
  rti1516ev::VariableLengthData data3 = rti1516ev::VariableLengthData(static_cast<const char*>(data1.data()) + 4, interactionClassHandle.encodedLength());
  rti1516ev::InteractionClassHandle decodedInteractionClass = ambassador.decodeInteractionClassHandle(data3);

  if (decodedInteractionClass != interactionClassHandle)
  {
    std::wcerr << L"handle decoded by ambassador differs from input: " << interactionClassHandle << L" != " << decodedInteractionClass << std::endl;
    return false;
  }
  std::wcout << ambassador.getInteractionClassName(interactionClassHandle) << L" OK" << std::endl;
  return true;
}

void ProcessData(std::list<rti1516ev::VariableLengthData>& theList, std::condition_variable& cv, std::mutex& m, std::atomic_bool& finish)
{
  while(!finish)
  {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [&theList]{return !theList.empty();});
    while (!theList.empty())
    {
      if (gDebugPrint) std::cout << "in ProcessData:" << theList.front() << std::endl;
      theList.pop_front();
    }
  }
}

void Leaktest()
{
  std::list<rti1516ev::VariableLengthData> theData;
  std::mutex m;
  std::condition_variable cv;
  std::atomic_bool finish{false};
  std::thread process(ProcessData, std::ref(theData), std::ref(cv), std::ref(m), std::ref(finish));

  for (int i = 0; i< 1000; i++)
  {
    auto hlaHandle = rti1516ev::HLAhandle(rti1516ev::createFederateHandle(1));
    rti1516ev::VariableLengthData handleData = hlaHandle.encode();
    {
        std::lock_guard<std::mutex> lk(m);
        theData.push_back(handleData);
    }
    cv.notify_one();
    if (gDebugPrint) std::cout << handleData << std::endl;
    int32_t value = i;
    rti1516ev::HLAinteger32LE encoding1(&value);
    rti1516ev::VariableLengthData x = encoding1.encode();
    if (gDebugPrint) std::cout << x << std::endl;
    rti1516ev::VariableLengthData y;
    y = x;
    {
      std::lock_guard<std::mutex> lk(m);
      theData.push_back(x);
    }
    cv.notify_one();
    {
      std::lock_guard<std::mutex> lk(m);
      theData.push_back(y);
    }
    cv.notify_one();
  }
  finish.store(true);
  process.join();
  assert(theData.empty());
}

bool testDataPointerEncoding()
{
  if (gDebugPrint) std::cout << "=============================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing data pointer encoding" << std::endl;
  if (gDebugPrint) std::cout << "=============================" << std::endl;
  int32_t value = 123;
  rti1516ev::HLAinteger32LE encoding1(&value);
  rti1516ev::VariableLengthData data;
  encoding1.encode(data);
  rti1516ev::HLAinteger32LE encoding2;
  encoding2.decode(data);
  if (gDebugPrint) std::cout << "data(123)=" << data << std::endl;
  if (encoding2.get() != value)
  {
    std::wcerr << L"decoded value does not match: " << encoding2.get() << L" != " << value << std::endl;
    return false;
  }

  value = 321;
  encoding1.encode(data);
  encoding2.decode(data);
  if (gDebugPrint) std::cout << "data(321)=" << data << std::endl;

  if (encoding2.get() != value)
  {
    std::wcerr << L"decoded modified value does not match: " << encoding2.get() << L" != " << value << std::endl;
    return false;
  }
  return true;
}
// test array with data elements given by 'borrowed' encoder pointers, each pointing to 'borrowed' (external) memory
bool testDataPointerFixedArrayEncoding()
{
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing fixed array to data pointer encoding" << std::endl;
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  std::vector<uint8_t> data1{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  std::vector<uint8_t> data2{0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
  rti1516ev::HLAbyte encoder1[8];
  rti1516ev::HLAbyte encoder2[8];
  rti1516ev::HLAfixedArray arrayEncoder1(rti1516ev::HLAbyte(), 8);
  rti1516ev::HLAfixedArray arrayEncoder2(rti1516ev::HLAbyte(), 8);
  for (int i=0;i<8;i++)
  {
    encoder1[i].setDataPointer(&data1.data()[i]);
    encoder2[i].setDataPointer(&data2.data()[i]);
    arrayEncoder1.setElementPointer(i, &encoder1[i]);
    arrayEncoder2.setElementPointer(i, &encoder2[i]);
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  if (gDebugPrint) std::cout << "encodedData1=" << encodedData1 << std::endl;
  rti1516ev::VariableLengthData encodedData2 = arrayEncoder2.encode();
  if (gDebugPrint) std::cout << "encodedData2=" << encodedData2 << std::endl;

  // set up decoder, to decode back the data produced by the encoder, before
  rti1516ev::HLAfixedArray arrayDecoder(rti1516ev::HLAbyte(), 8);
  for (int i=0;i<8;i++)
  {
    arrayDecoder.setElementPointer(i, &encoder1[i]);
  }
  std::vector<uint8_t> copyOfData1 = data1;
  arrayDecoder.decode(encodedData2);
  if (gDebugPrint) std::cout << "deccodedData(encodedData2)=" << data1 << std::endl;
  // NOTE: data2's content has been decoded into data1, so both should be identical now
  if (data1 != data2)
  {
    std::cout << "data1 != copyOfData1" << std::endl;
    std::cout << "data1 = " << data1 << std::endl;
    std::cout << "copyOfData1 = " << copyOfData1 << std::endl;
  }

  // test encodeInto
  std::vector<uint8_t> buffer1;
  arrayEncoder1.encodeInto(buffer1);
  if (gDebugPrint) std::cout << "arrayEncoder1.encodeInto => " << buffer1 << std::endl;
  std::vector<uint8_t> buffer2;
  arrayEncoder2.encodeInto(buffer2);
  if (gDebugPrint) std::cout << "arrayEncoder2.encodeInto => " << buffer2 << std::endl;

  // check prototype safety
  try {
    arrayEncoder1.set(0, rti1516ev::HLAinteger32LE());
    std::cout << "HLAfixedArray(HLAbyte).set accepts HLAinteger32LE - bad" << std::endl;
    return false;
  }
  catch (const rti1516ev::EncoderException& e)
  {
    if (gDebugPrint) std::cout << "Expected EncoderException: " << e << std::endl;
  }
  try {
    rti1516ev::HLAinteger32LE enc;
    arrayEncoder1.setElementPointer(0, &enc);
    std::cout << "HLAfixedArray(HLAbyte).setElementPointer accepts HLAinteger32LE* - bad" << std::endl;
    return false;
  }
  catch (const rti1516ev::EncoderException& e)
  {
    if (gDebugPrint) std::cout << "Expected EncoderException: " << e << std::endl;
  }

  std::cout << "fixed array to data pointer encoding OK" << std::endl;
  return true;
}

// test array with data elements given by 'borrowed' encoder pointers, each pointing to 'borrowed' (external) memory
bool testDataPointerVariableArrayEncoding()
{
  if (gDebugPrint) std::cout << "===============================================" << std::endl;
  if (gDebugPrint) std::cout << "testing variable array to data pointer encoding" << std::endl;
  if (gDebugPrint) std::cout << "===============================================" << std::endl;
  std::vector<uint8_t> data1{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  std::vector<uint8_t> data2{0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
  rti1516ev::HLAbyte encoder1[8];
  rti1516ev::HLAbyte encoder2[8];
  rti1516ev::HLAvariableArray arrayEncoder1{rti1516ev::HLAbyte()};
  rti1516ev::HLAvariableArray arrayEncoder2{rti1516ev::HLAbyte()};
  for (int i=0;i<8;i++)
  {
    encoder1[i].setDataPointer(&data1.data()[i]);
    encoder2[i].setDataPointer(&data2.data()[i]);
    arrayEncoder1.addElementPointer(&encoder1[i]);
    arrayEncoder2.addElementPointer(&encoder2[i]);
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  if (gDebugPrint) std::cout << "encodedData1=" << encodedData1 << std::endl;
  rti1516ev::VariableLengthData encodedData2 = arrayEncoder2.encode();
  if (gDebugPrint) std::cout << "encodedData2=" << encodedData2 << std::endl;

  // set up decoder, to decode back the data produced by the encoder, before
  rti1516ev::HLAvariableArray arrayDecoder{rti1516ev::HLAbyte()};
  for (int i=0;i<8;i++)
  {
    arrayDecoder.addElementPointer(&encoder1[i]);
  }
  arrayDecoder.decode(encodedData2);
  if (gDebugPrint) std::cout << "deccodedData(encodedData2)=" << data1 << std::endl;
  if (gDebugPrint) std::cout << "deccodedData(encodedData2)=" << data1 << std::endl;
  // NOTE: data2's content has been decoded into data1, so both should be identical now
  if (data1 != data2)
  {
    std::cerr << "data1 != dat2" << std::endl;
    std::cerr << "data1 = " << data1 << std::endl;
    std::cerr << "data2 = " << data2 << std::endl;
  }

  // test encodeInto
  std::vector<uint8_t> buffer1;
  arrayEncoder1.encodeInto(buffer1);
  if (gDebugPrint) std::cout << "arrayEncoder1.encodeInto => " << buffer1 << std::endl;
  std::vector<uint8_t> buffer2;
  arrayEncoder2.encodeInto(buffer2);
  if (gDebugPrint) std::cout << "arrayEncoder2.encodeInto => " << buffer2 << std::endl;

  // check prototype safety
  try {
    arrayEncoder1.set(0, rti1516ev::HLAinteger32LE());
    std::cout << "HLAvariableArray(HLAbyte).set accepts HLAinteger32LE - bad" << std::endl;
    return false;
  }
  catch (const rti1516ev::EncoderException& e)
  {
    if (gDebugPrint) std::cout << "Expected EncoderException: " << e << std::endl;
  }
  try {
    rti1516ev::HLAinteger32LE enc;
    arrayEncoder1.setElementPointer(0, &enc);
    std::cout << "HLAvariableArray(HLAbyte).setElementPointer accepts HLAinteger32LE* - bad" << std::endl;
    return false;
  }
  catch (const rti1516ev::EncoderException& e)
  {
    if (gDebugPrint) std::cout << "Expected EncoderException: " << e << std::endl;
  }
  std::cout << "variable array to data pointer encoding OK" << std::endl;
  return true;
}

bool testFixedArrayOfASCIIStringEncoding()
{
  if (gDebugPrint) std::cout << "=============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing fixed array of ascii strings" << std::endl;
  if (gDebugPrint) std::cout << "=============================================" << std::endl;
  std::string data [] = { "one", "two", "three", "four" };
  rti1516ev::HLAfixedArray arrayEncoder1{rti1516ev::HLAASCIIstring(), 4};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.set(i, rti1516ev::HLAASCIIstring(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAfixedArray arrayDecoder{rti1516ev::HLAASCIIstring(), 4};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::cout << static_cast<const rti1516ev::HLAASCIIstring&>(arrayDecoder.get(i)).get() << std::endl;
    if (static_cast<const rti1516ev::HLAASCIIstring&>(arrayDecoder.get(i)).get() != data[i])
      return false;
  }
  std::cout << "fixed array of ascii strings OK" << std::endl;
  return true;
}


// test array with data elements given by 'borrowed' encoder pointers, each pointing to 'borrowed' (external) memory
bool testVariableArrayOfASCIIStringEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing variable array of ascii strings" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  std::string data [] = { "one", "two", "three", "four" };
  rti1516ev::HLAvariableArray arrayEncoder1{rti1516ev::HLAASCIIstring()};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.addElement(rti1516ev::HLAASCIIstring(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAvariableArray arrayDecoder{rti1516ev::HLAASCIIstring()};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::cout << static_cast<const rti1516ev::HLAASCIIstring&>(arrayEncoder1.get(i)).get() << std::endl;
    if (static_cast<const rti1516ev::HLAASCIIstring&>(arrayEncoder1.get(i)).get() != data[i])
      return false;
  }
  std::cout << "variable array of ascii strings OK" << std::endl;
  return true;
}

bool testFixedArrayOfUnicodeStringEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing fixed array of unicode strings" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  std::wstring data [] = { L"one", L"two", L"three", L"four" };
  rti1516ev::HLAfixedArray arrayEncoder1{rti1516ev::HLAunicodeString(), 4};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.set(i, rti1516ev::HLAunicodeString(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAfixedArray arrayDecoder{rti1516ev::HLAunicodeString(), 4};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::wcout << static_cast<const rti1516ev::HLAunicodeString&>(arrayEncoder1.get(i)).get() << std::endl;
    if (static_cast<const rti1516ev::HLAunicodeString&>(arrayEncoder1.get(i)).get() != data[i])
      return false;
  }
  std::cout << "fixed array of unicode strings OK" << std::endl;
  return true;
}

template<class BasicTypeEncoding>
bool testFixedArrayOfBasicTypeEncoding(typename BasicTypeEncoding::SimpleDataType data[4])
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing fixed array of " << typeid(BasicTypeEncoding).name() << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;

  std::cout << "encode from data owned by array/decode into data owned by array" << std::endl;
  rti1516ev::HLAfixedArray arrayEncoder1{ BasicTypeEncoding(), 4 };
  for (int i = 0; i < 4; i++)
  {
    arrayEncoder1.set(i, BasicTypeEncoding(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAfixedArray arrayDecoder1{ BasicTypeEncoding(), 4 };
  arrayDecoder1.decode(encodedData1);
  for (int i = 0; i < 4; i++)
  {
    std::wcout << "arrayDecoder1.get(" << i << ")).get()=" << std::hex << static_cast<const BasicTypeEncoding&>(arrayDecoder1.get(i)).get() << std::endl;
    if (static_cast<const BasicTypeEncoding&>(arrayDecoder1.get(i)).get() != data[i])
      return false;
  }

  std::cout << "encode from external data/decode into data owned by array" << std::endl;
  BasicTypeEncoding integerEncoder[4];
  rti1516ev::HLAfixedArray arrayEncoder2{ BasicTypeEncoding(), 4 };
  for (int i = 0; i < 4; i++)
  {
    integerEncoder[i].setDataPointer(&data[i]);
    arrayEncoder2.setElementPointer(i, &integerEncoder[i]);
  }
  rti1516ev::VariableLengthData encodedData2 = arrayEncoder1.encode();
  std::cout << "encodedData2=" << encodedData2 << std::endl;

  rti1516ev::HLAfixedArray arrayDecoder2{ BasicTypeEncoding(), 4 };
  arrayDecoder2.decode(encodedData2);
  for (int i = 0; i < 4; i++)
  {
    std::wcout << "arrayDecoder2.get(" << i << ").get()=" << std::hex << static_cast<const BasicTypeEncoding&>(arrayDecoder1.get(i)).get() << std::endl;
    if (static_cast<const BasicTypeEncoding&>(arrayDecoder2.get(i)).get() != data[i])
      return false;
  }

  std::cout << "decode into external data, using encoded data from last step" << std::endl;
  typename BasicTypeEncoding::SimpleDataType decodedData[] = { 0, 0, 0, 0 };
  BasicTypeEncoding integerDecoder[4];
  rti1516ev::HLAfixedArray arrayDecoder3{ BasicTypeEncoding(), 4 };
  for (int i = 0; i < 4; i++)
  {
    integerDecoder[i].setDataPointer(&decodedData[i]);
    arrayDecoder3.setElementPointer(i, &integerDecoder[i]);
  }
  arrayDecoder3.decode(encodedData2);
  for (int i = 0; i < 4; i++)
  {
    std::wcout << "decodedData[" << i << "]=" << std::hex << decodedData[i] << std::endl;
    if (decodedData[i] != data[i])
      return false;
  }
  std::cout << "fixed array of " << typeid(BasicTypeEncoding).name() << " OK" << std::endl;
  return true;
}

bool testVariableArrayOfUnicodeStringEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing variable array of unicode strings" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  std::wstring data [] = { L"one", L"two", L"three", L"four" };
  rti1516ev::HLAvariableArray arrayEncoder1{rti1516ev::HLAunicodeString()};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.addElement(rti1516ev::HLAunicodeString(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAvariableArray arrayDecoder{rti1516ev::HLAunicodeString()};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::wcout << static_cast<const rti1516ev::HLAunicodeString&>(arrayEncoder1.get(i)).get() << std::endl;
    if (static_cast<const rti1516ev::HLAunicodeString&>(arrayEncoder1.get(i)).get() != data[i])
      return false;
  }
  std::cout << "variable array of unicode strings OK" << std::endl;
  return true;
}

bool testFixedArrayOfFederateHandlesEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing fixed array of FederateHandles" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  rti1516ev::FederateHandle data [] = {
    rti1516ev::createFederateHandle(2), 
    rti1516ev::createFederateHandle(5), 
    rti1516ev::createFederateHandle(7), 
    rti1516ev::createFederateHandle(10)};
  rti1516ev::HLAfixedArray arrayEncoder1{rti1516ev::HLAhandle(rti1516ev::FederateHandle()), 4};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.set(i, rti1516ev::HLAhandle(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAfixedArray arrayDecoder{rti1516ev::HLAhandle(rti1516ev::FederateHandle()), 4};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::wcout << static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getFederateHandle().toString() << std::endl;
    if (static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getFederateHandle() != data[i])
      return false;
  }
  std::cout << "fixed array of federate handles OK" << std::endl;
  return true;
}

bool testVariableArrayOfFederateHandlesEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing variable array of FederateHandles" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  rti1516ev::FederateHandle data [] = {
    rti1516ev::createFederateHandle(2), 
    rti1516ev::createFederateHandle(5), 
    rti1516ev::createFederateHandle(7), 
    rti1516ev::createFederateHandle(10)};
  rti1516ev::HLAvariableArray arrayEncoder1{rti1516ev::HLAhandle(rti1516ev::FederateHandle())};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.addElement(rti1516ev::HLAhandle(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAvariableArray arrayDecoder{rti1516ev::HLAhandle(rti1516ev::FederateHandle())};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::wcout << static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getFederateHandle().toString() << std::endl;
    if (static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getFederateHandle() != data[i])
      return false;
  }
  std::cout << "variable array of federate handles OK" << std::endl;
  return true;
}

bool testVariableArrayOfObjectClassHandlesEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing variable array of ObjectClassHandles" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  rti1516ev::ObjectClassHandle data [] = {
    rti1516ev::createObjectClassHandle(2), 
    rti1516ev::createObjectClassHandle(5), 
    rti1516ev::createObjectClassHandle(7), 
    rti1516ev::createObjectClassHandle(10)};
  rti1516ev::HLAvariableArray arrayEncoder1{rti1516ev::HLAhandle(rti1516ev::ObjectClassHandle())};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.addElement(rti1516ev::HLAhandle(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAvariableArray arrayDecoder{rti1516ev::HLAhandle(rti1516ev::ObjectClassHandle())};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::wcout << static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getObjectClassHandle().toString() << std::endl;
    if (static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getObjectClassHandle() != data[i])
      return false;
  }
  std::cout << "variable array of object class handles OK" << std::endl;
  return true;
}

bool testFixedArrayOfAttributeHandlesEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing fixed array of AttributeHandles" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  rti1516ev::AttributeHandle data [] = {
    rti1516ev::createAttributeHandle(2), 
    rti1516ev::createAttributeHandle(5), 
    rti1516ev::createAttributeHandle(7), 
    rti1516ev::createAttributeHandle(10)};
  rti1516ev::HLAfixedArray arrayEncoder1{rti1516ev::HLAhandle(rti1516ev::AttributeHandle()), 4};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.set(i, rti1516ev::HLAhandle(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAfixedArray arrayDecoder{rti1516ev::HLAhandle(rti1516ev::AttributeHandle()), 4};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::wcout << static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getAttributeHandle().toString() << std::endl;
    if (static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getAttributeHandle() != data[i])
      return false;
  }
  std::cout << "fixed array of attribute handles OK" << std::endl;
  return true;
}

bool testVariableArrayOfAttributeHandlesEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing variable array of AttributeHandles" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  rti1516ev::AttributeHandle data [] = {
    rti1516ev::createAttributeHandle(2), 
    rti1516ev::createAttributeHandle(5), 
    rti1516ev::createAttributeHandle(7), 
    rti1516ev::createAttributeHandle(10)};
  rti1516ev::HLAvariableArray arrayEncoder1{rti1516ev::HLAhandle(rti1516ev::AttributeHandle())};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.addElement(rti1516ev::HLAhandle(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516ev::HLAvariableArray arrayDecoder{rti1516ev::HLAhandle(rti1516ev::AttributeHandle())};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::wcout << static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getAttributeHandle().toString() << std::endl;
    if (static_cast<const rti1516ev::HLAhandle&>(arrayEncoder1.get(i)).getAttributeHandle() != data[i])
      return false;
  }
  std::cout << "variable array of attribute handles OK" << std::endl;
  return true;
}

bool testFixedArrayOfInteractionClassHandlesEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing fixed array of InteractionClassHandles" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  rti1516ev::InteractionClassHandle data [] = {
    rti1516ev::createInteractionClassHandle(2), 
    rti1516ev::createInteractionClassHandle(5), 
    rti1516ev::createInteractionClassHandle(7), 
    rti1516ev::createInteractionClassHandle(10)};
  rti1516ev::HLAfixedArray arrayEncoder{rti1516ev::HLAhandle(rti1516ev::InteractionClassHandle()), 4};
  for (int i=0;i<4;i++)
  {
    arrayEncoder.set(i, rti1516ev::HLAhandle(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1;
  try
  {
    encodedData1 = arrayEncoder.encode();
    std::cout << "encodedData1=" << encodedData1 << std::endl;
  }
  catch (const rti1516ev::Exception& e)
  {
    std::cerr << "Unexpected exception while encoding: " << e << std::endl;
    return false;
  }
  catch (...)
  {
    std::cerr << "Unexpected exception while encoding!" << std::endl;
    return false;
  }

  rti1516ev::HLAfixedArray arrayDecoder{rti1516ev::HLAhandle(rti1516ev::InteractionClassHandle()), 4};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::wcout << L"decoded(" << i << L") = " << static_cast<const rti1516ev::HLAhandle&>(arrayEncoder.get(i)).getInteractionClassHandle().toString() << std::endl;
    if (static_cast<const rti1516ev::HLAhandle&>(arrayEncoder.get(i)).getInteractionClassHandle() != data[i])
      return false;
  }
  std::cout << "variable array of interaction class handles OK" << std::endl;
  return true;
}

bool testVariableArrayOfInteractionClassHandlesEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing varaiable array of InteractionClassHandles" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  rti1516ev::InteractionClassHandle data [] = {
    rti1516ev::createInteractionClassHandle(2), 
    rti1516ev::createInteractionClassHandle(5), 
    rti1516ev::createInteractionClassHandle(7), 
    rti1516ev::createInteractionClassHandle(10)};
  rti1516ev::HLAvariableArray arrayEncoder{rti1516ev::HLAhandle(rti1516ev::InteractionClassHandle())};
  for (int i=0;i<4;i++)
  {
    arrayEncoder.addElement(rti1516ev::HLAhandle(data[i]));
  }
  rti1516ev::VariableLengthData encodedData1;
  try
  {
    encodedData1 = arrayEncoder.encode();
    std::cout << "encodedData1=" << encodedData1 << std::endl;
  }
  catch (const rti1516ev::Exception& e)
  {
    std::cerr << "Unexpected exception while encoding: " << e << std::endl;
    return false;
  }
  catch (...)
  {
    std::cerr << "Unexpected exception while encoding!" << std::endl;
    return false;
  }

  rti1516ev::HLAvariableArray arrayDecoder{rti1516ev::HLAhandle(rti1516ev::InteractionClassHandle())};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::wcout << L"decoded(" << i << L") = " << static_cast<const rti1516ev::HLAhandle&>(arrayEncoder.get(i)).getInteractionClassHandle().toString() << std::endl;
    if (static_cast<const rti1516ev::HLAhandle&>(arrayEncoder.get(i)).getInteractionClassHandle() != data[i])
      return false;
  }
  std::cout << "variable array of interaction class handles OK" << std::endl;
  return true;
}

bool testVariableArrayOfInteractionSubscriptionsEncoding()
{
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing varaiable array of HLAinteractionSubscription" << std::endl;
  if (gDebugPrint) std::cout << "=======================================================" << std::endl;
  std::vector<HLAinteractionSubscription> data{
    HLAinteractionSubscription(rti1516ev::createInteractionClassHandle(2), true),
    HLAinteractionSubscription(rti1516ev::createInteractionClassHandle(5), false),
    HLAinteractionSubscription(rti1516ev::createInteractionClassHandle(7), true),
    HLAinteractionSubscription(rti1516ev::createInteractionClassHandle(10), false),
  };
  rti1516ev::HLAvariableArray arrayEncoder = rti1516ev::HLAvariableArray(HLAinteractionSubscription(rti1516ev::InteractionClassHandle(), rti1516ev::HLAboolean()));
  for (size_t i=0;i<data.size();i++)
  {
    arrayEncoder.addElement(data[i]);
  }
  rti1516ev::VariableLengthData encodedData1;
  std::cout << "arrayEncoder.getEncodedLength()=" << arrayEncoder.getEncodedLength() << std::endl;
  try
  {
    encodedData1 = arrayEncoder.encode();
    std::cout << "encodedData1=" << encodedData1 << std::endl;
  }
  catch (const rti1516ev::Exception& e)
  {
    std::cerr << "Unexpected exception while encoding: " << e << std::endl;
    return false;
  }
  catch (...)
  {
    std::cerr << "Unexpected exception while encoding!" << std::endl;
    return false;
  }
  rti1516ev::HLAvariableArray arrayDecoder{HLAinteractionSubscription(rti1516ev::InteractionClassHandle(), rti1516ev::HLAboolean())};
  try
  {
    arrayDecoder.decode(encodedData1);
  }
  catch (const rti1516ev::Exception& e)
  {
    std::cerr << "Unexpected exception while decoding: " << e << std::endl;
    return false;
  }
  catch (...)
  {
    std::cerr << "Unexpected exception while decoding!" << std::endl;
    return false;
  }
  if (arrayDecoder.size() != data.size())
  {
    std::cerr << "decoded different array size: " << arrayDecoder.size() << " != " << data.size() << std::endl;
    return false;
  }
  DumpDataElement(std::wcerr, &arrayDecoder);
  for (size_t i=0;i<data.size();i++)
  {
    const rti1516ev::HLAfixedRecord& element = static_cast<const rti1516ev::HLAfixedRecord&>(arrayDecoder.get(i));
    const rti1516ev::HLAhandle& first = static_cast<const rti1516ev::HLAhandle&>(element.get(0));
    const rti1516ev::HLAboolean& second = static_cast<const rti1516ev::HLAboolean&>(element.get(1));

    std::wcout << L"decoded(" << i << L") = " << first.getInteractionClassHandle().toString() << ", " << second.get() << std::endl;
    if (first.getInteractionClassHandle() != data[i].getClassHandle())
      return false;
    if (second != data[i].getActive())
      return false;
  }
  std::cout << "variable array of interaction subscriptions OK" << std::endl;
  return true;
}

bool testDataPointerFixedRecordEncoding()
{
  if (gDebugPrint) std::cout << "=============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing fixed record to data pointer encoding" << std::endl;
  if (gDebugPrint) std::cout << "=============================================" << std::endl;
  // first set up a fixed array, to be added to a fixed record
  std::vector<uint8_t> data1{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  std::vector<uint8_t> data2{0, 0, 0, 0, 0, 0, 0, 0};
  rti1516ev::HLAbyte encoder1[8];
  rti1516ev::HLAbyte encoder2[8];
  rti1516ev::HLAfixedArray arrayEncoder1(rti1516ev::HLAbyte(), 8);
  rti1516ev::HLAfixedArray arrayEncoder2(rti1516ev::HLAbyte(), 8);
  for (int i=0;i<8;i++)
  {
    encoder1[i].setDataPointer(&data1.data()[i]);
    encoder2[i].setDataPointer(&data2.data()[i]);
    arrayEncoder1.setElementPointer(i, &encoder1[i]);
    arrayEncoder2.setElementPointer(i, &encoder2[i]);
  }
  int i1 = 123;
  rti1516ev::HLAinteger32LE intEncoder;
  intEncoder.setDataPointer(&i1);
  char c1 = 123;
  rti1516ev::HLAASCIIchar charEncoder;
  charEncoder.setDataPointer(&c1);

  // now set up the fixed array
  rti1516ev::HLAfixedRecord recordEncoder;
  recordEncoder.appendElementPointer(&intEncoder);
  recordEncoder.appendElementPointer(&charEncoder);
  recordEncoder.appendElementPointer(&arrayEncoder1);
  rti1516ev::VariableLengthData encodedData1 = recordEncoder.encode();
  if (gDebugPrint) std::cout << "encodedData1=" << encodedData1 << std::endl;
  // now decode all from encodedData1:
  // prepare recordDecoder
  rti1516ev::HLAfixedRecord recordDecoder;
  recordDecoder.appendElement(rti1516ev::HLAinteger32LE());
  recordDecoder.appendElement(rti1516ev::HLAASCIIchar());
  // prepare array decoder
  rti1516ev::HLAfixedArray arrayDecoder(rti1516ev::HLAbyte(), 8);
  std::vector<uint8_t> decodedArray{0, 0, 0, 0, 0, 0, 0, 0};
  rti1516ev::HLAbyte decoder[8];
  for (int i=0;i<8;i++)
  {
    decoder[i].setDataPointer(&decodedArray.data()[i]);
    arrayDecoder.setElementPointer(i, &decoder[i]);
  }
  recordDecoder.appendElementPointer(&arrayDecoder);
  // decode
  recordDecoder.decode(encodedData1);
  // fields should now equal given data
  if (gDebugPrint) std::cout << "field1=" << static_cast<const rti1516ev::HLAinteger32LE&>(recordDecoder.get(0)).get() << std::endl;
  if (gDebugPrint) std::cout << "field2=" << static_cast<int>(static_cast<const rti1516ev::HLAASCIIchar&>(recordDecoder.get(1)).get()) << std::endl;
  if (gDebugPrint) std::cout << "field3=" << decodedArray << std::endl;
  const rti1516ev::HLAfixedArray& array = dynamic_cast<const rti1516ev::HLAfixedArray&>(recordDecoder.get(2));
  if (gDebugPrint) std::cout << "data from distinct HLAbytes:" << std::endl;
  for (int i=0;i<8;i++)
  {
    const rti1516ev::HLAbyte& byteDecoder = dynamic_cast<const rti1516ev::HLAbyte&>(array.get(i));
    if (gDebugPrint) std::cout << " " << static_cast<int>(byteDecoder.get());
  }
  if (gDebugPrint) std::cout << std::endl;
  std::cout << "fixed record to data pointer encoding OK" << std::endl;
  return true;
}

bool testFixedRecordWithVersionsEncoding()
{
  if (gDebugPrint) std::cout << "==============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing fixed record encodings with versions" << std::endl;
  if (gDebugPrint) std::cout << "==============================================" << std::endl;
  // first set up a fixed array, to be added to a fixed record
  std::vector<uint8_t> data1{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  std::vector<uint8_t> data2{0, 0, 0, 0, 0, 0, 0, 0};
  rti1516ev::HLAbyte byteEncoder1[8];
  rti1516ev::HLAbyte byteEncoder2[8];
  rti1516ev::HLAfixedArray arrayEncoder1(rti1516ev::HLAbyte(), 8);
  rti1516ev::HLAfixedArray arrayEncoder2(rti1516ev::HLAbyte(), 8);
  for (int i=0;i<8;i++)
  {
    byteEncoder1[i].setDataPointer(&data1.data()[i]);
    byteEncoder2[i].setDataPointer(&data2.data()[i]);
    arrayEncoder1.setElementPointer(i, &byteEncoder1[i]);
    arrayEncoder2.setElementPointer(i, &byteEncoder2[i]);
  }
  int i1 = 123;
  rti1516ev::HLAinteger32LE intEncoder;
  intEncoder.setDataPointer(&i1);
  char c1 = 123;
  rti1516ev::HLAASCIIchar charEncoder;
  charEncoder.setDataPointer(&c1);
  bool b1 = true;
  rti1516ev::HLAboolean boolEncoder;
  boolEncoder.setDataPointer(&b1);

  // now set up the fixed record encoder version 1 ...
  rti1516ev::HLAfixedRecord recordEncoderV1(1);
  recordEncoderV1.appendElementPointer(&intEncoder, 1);
  recordEncoderV1.appendElementPointer(&charEncoder, 1);
  recordEncoderV1.appendElementPointer(&arrayEncoder1, 1);
  rti1516ev::VariableLengthData encodedDataV1 = recordEncoderV1.encode();
  if (gDebugPrint) std::cout << "encodedDataV1=" << encodedDataV1 << std::endl;

  // ... and the fixed record encoder version 2
  rti1516ev::HLAfixedRecord recordEncoderV2(2);
  recordEncoderV2.appendElementPointer(&intEncoder, 1);
  recordEncoderV2.appendElementPointer(&charEncoder, 1);
  recordEncoderV2.appendElementPointer(&arrayEncoder1, 1);
  recordEncoderV2.appendElementPointer(&boolEncoder, 2);
  rti1516ev::VariableLengthData encodedDataV2 = recordEncoderV2.encode();
  if (gDebugPrint) std::cout << "encodedDataV2=" << encodedDataV2 << std::endl;

  // now decode all from encodedDataV1:
  // prepare recordDecoderV1
  rti1516ev::HLAfixedRecord recordDecoderV1(1);
  recordDecoderV1.appendElement(rti1516ev::HLAinteger32LE());
  recordDecoderV1.appendElement(rti1516ev::HLAASCIIchar());
  // prepare array decoder
  rti1516ev::HLAfixedArray arrayDecoder1(rti1516ev::HLAbyte(), 8);
  std::vector<uint8_t> decodedArray1{0, 0, 0, 0, 0, 0, 0, 0};
  rti1516ev::HLAbyte byteDecoder1[8];
  for (int i=0;i<8;i++)
  {
    byteDecoder1[i].setDataPointer(&decodedArray1.data()[i]);
    arrayDecoder1.setElementPointer(i, &byteDecoder1[i]);
  }
  recordDecoderV1.appendElementPointer(&arrayDecoder1);
  // decode with data from V1 encoder
  {
    recordDecoderV1.decode(encodedDataV1);
    // fields should now equal given data
    if (gDebugPrint) std::cout << "field1=" << static_cast<const rti1516ev::HLAinteger32LE&>(recordDecoderV1.get(0)).get() << std::endl;
    if (gDebugPrint) std::cout << "field2=" << static_cast<int>(static_cast<const rti1516ev::HLAASCIIchar&>(recordDecoderV1.get(1)).get()) << std::endl;
    if (gDebugPrint) std::cout << "field3=" << decodedArray1 << std::endl;
    const rti1516ev::HLAfixedArray& array1 = dynamic_cast<const rti1516ev::HLAfixedArray&>(recordDecoderV1.get(2));
    if (gDebugPrint) std::cout << "data from distinct HLAbytes:" << std::endl;
    for (int i=0;i<8;i++)
    {
      const rti1516ev::HLAbyte& byteDecoder = dynamic_cast<const rti1516ev::HLAbyte&>(array1.get(i));
      if (gDebugPrint) std::cout << " " << static_cast<int>(byteDecoder.get());
    }
    if (gDebugPrint) std::cout << std::endl;
  }
  // decode with data from V2 encoder
  {
    recordDecoderV1.decode(encodedDataV2);
    // fields should now equal given data
    if (gDebugPrint) std::cout << "field1=" << static_cast<const rti1516ev::HLAinteger32LE&>(recordDecoderV1.get(0)).get() << std::endl;
    if (gDebugPrint) std::cout << "field2=" << static_cast<int>(static_cast<const rti1516ev::HLAASCIIchar&>(recordDecoderV1.get(1)).get()) << std::endl;
    if (gDebugPrint) std::cout << "field3=" << decodedArray1 << std::endl;
    const rti1516ev::HLAfixedArray& array1 = dynamic_cast<const rti1516ev::HLAfixedArray&>(recordDecoderV1.get(2));
    if (gDebugPrint) std::cout << "data from distinct HLAbytes:" << std::endl;
    for (int i=0;i<8;i++)
    {
      const rti1516ev::HLAbyte& byteDecoder = dynamic_cast<const rti1516ev::HLAbyte&>(array1.get(i));
      if (gDebugPrint) std::cout << " " << static_cast<int>(byteDecoder.get());
    }
    if (gDebugPrint) std::cout << std::endl;
  }

  // prepare recordDecoderV2
  rti1516ev::HLAfixedRecord recordDecoderV2(2);
  recordDecoderV2.appendElement(rti1516ev::HLAinteger32LE(), 1);
  recordDecoderV2.appendElement(rti1516ev::HLAASCIIchar(), 1);
  // prepare array decoder
  rti1516ev::HLAfixedArray arrayDecoder2(rti1516ev::HLAbyte(), 8);
  std::vector<uint8_t> decodedArray2{0, 0, 0, 0, 0, 0, 0, 0};
  rti1516ev::HLAbyte byteDecoder2[8];
  for (int i=0;i<8;i++)
  {
    byteDecoder2[i].setDataPointer(&decodedArray2.data()[i]);
    arrayDecoder2.setElementPointer(i, &byteDecoder2[i]);
  }
  recordDecoderV2.appendElementPointer(&arrayDecoder2, 1);
  recordDecoderV2.appendElementPointer(&boolEncoder, 2);
  {
    // decode with data from V2 encoder (full match)
    recordDecoderV2.decode(encodedDataV2);
    // fields should now equal given data
    if (gDebugPrint) std::cout << "record version=" << recordDecoderV2.getVersion() << std::endl;
    if (gDebugPrint) std::cout << "field1=" << static_cast<const rti1516ev::HLAinteger32LE&>(recordDecoderV2.get(0)).get() << std::endl;
    if (gDebugPrint) std::cout << "field2=" << static_cast<int>(static_cast<const rti1516ev::HLAASCIIchar&>(recordDecoderV2.get(1)).get()) << std::endl;
    if (gDebugPrint) std::cout << "field3=" << decodedArray2 << std::endl;
    const rti1516ev::HLAfixedArray& array2 = dynamic_cast<const rti1516ev::HLAfixedArray&>(recordDecoderV2.get(2));
    if (gDebugPrint) std::cout << "data from distinct HLAbytes:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
      const rti1516ev::HLAbyte& byteDecoder = dynamic_cast<const rti1516ev::HLAbyte&>(array2.get(i));
      if (gDebugPrint) std::cout << " " << static_cast<int>(byteDecoder.get());
    }
    if (gDebugPrint) std::cout << std::endl;
    if (gDebugPrint) std::cout << "field4=" << static_cast<const rti1516ev::HLAboolean&>(recordDecoderV2.get(3)).get() << std::endl;
  }
  {
    // decode with data from V1 encoding (boolean field at end is missing)
    recordDecoderV2.decode(encodedDataV1);
    // fields should now equal given data
    if (gDebugPrint) std::cout << "record version=" << recordDecoderV2.getVersion() << std::endl;
    if (gDebugPrint) std::cout << "field1=" << static_cast<const rti1516ev::HLAinteger32LE&>(recordDecoderV2.get(0)).get() << std::endl;
    if (gDebugPrint) std::cout << "field2=" << static_cast<int>(static_cast<const rti1516ev::HLAASCIIchar&>(recordDecoderV2.get(1)).get()) << std::endl;
    if (gDebugPrint) std::cout << "field3=" << decodedArray2 << std::endl;
    const rti1516ev::HLAfixedArray& array2 = dynamic_cast<const rti1516ev::HLAfixedArray&>(recordDecoderV2.get(2));
    if (gDebugPrint) std::cout << "data from distinct HLAbytes:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
      const rti1516ev::HLAbyte& byteDecoder = dynamic_cast<const rti1516ev::HLAbyte&>(array2.get(i));
      if (gDebugPrint) std::cout << " " << static_cast<int>(byteDecoder.get());
    }
    if (gDebugPrint) std::cout << std::endl;
    try {
      const rti1516ev::HLAboolean& boolDecoder = static_cast<const rti1516ev::HLAboolean&>(recordDecoderV2.get(3));
      if (gDebugPrint) std::cout << "expected exception not thrown (value is " << boolDecoder.get() << ")" << std::endl;
      return false;
    }
    catch (const rti1516ev::EncoderException& e)
    {
      if (gDebugPrint) std::cout << "expected exception: " << e << std::endl;
    }

  }
  std::cout << "fixed record encodings with versions OK" << std::endl;
  return true;
}

bool testTightBEEncoding()
{
  std::vector<uint32_t> values{
    0x0001, 0x0010, 0x0100, 0x1000,
    0x7fff, 0x07ff, 0x007f, 0x0007,
    0x8000, 0x0800, 0x0080, 0x0008,
    0xffff, 0x0fff, 0x00ff, 0x000f,
    0x5555, 0x0505, 0x5050, 0x1234
  };
  uint8_t buffer[128];
  size_t bufferSize = sizeof(buffer);
  size_t offset = 0;
  for (size_t i=0;i<values.size();i++)
  {
    uint32_t value = values[i];
    offset = rti1516ev::encodeIntoBE32Compressed(buffer, bufferSize, offset, values[i]);
    std::cout << std::dec << i << " value=" << std::hex << value << " => buffer=" << make_hex_string(buffer, buffer + offset, true, true) << std::endl;
  }
  offset = 0;
  for (size_t i=0;i<values.size();i++)
  {
    uint32_t value = 0;
    offset = rti1516ev::decodeFromBE32Compressed(buffer, bufferSize, offset, value);
    std::cout << std::dec << i << " value=" << std::hex << value << std::endl;
    if (value != values[i])
    {
      return false;
    }
  }
  return true;
}

bool testOpaqueDataEncoding()
{
  std::vector<uint8_t> inputValues{
    0x01, 0x02, 0x03, 0x04,
    0x10, 0x20, 0x30, 0x40,
  };
  //uint8_t buffer[128];
  //size_t bufferSize = sizeof(buffer);
  //size_t offset = 0;
  rti1516ev::HLAopaqueData opaqueDataEncoder1;
  opaqueDataEncoder1.set(inputValues.data(), inputValues.size());
  rti1516ev::VariableLengthData encodedData1 = opaqueDataEncoder1.encode();
  rti1516ev::HLAopaqueData opaqueDataDecoder1;
  opaqueDataDecoder1.decode(encodedData1);
  if (gDebugPrint) std::cout << "values=" << inputValues << std::endl;
  if (gDebugPrint) std::cout << "encodedData1=" << encodedData1 << std::endl;
  std::vector<uint8_t> decodedValues;
  decodedValues = opaqueDataDecoder1.get();
  if (gDebugPrint) std::cout << "decodedValues=" << decodedValues << std::endl;
  if (decodedValues != inputValues)
  {
    std::cerr << "decoded data does not match initial array:" << std::endl;
    if (gDebugPrint) std::cout << "values=" << inputValues << std::endl;
    if (gDebugPrint) std::cout << "decodedValues=" << decodedValues << std::endl;
    return false;
  }

  return true;
}

int
main(int argc, char* argv[])
{
  OpenRTI::Options options(argc, argv);
  std::wstring fomModule = L"fdd-1.xml";
  std::wstring federationExecutionName = L"test";

  std::vector<std::wstring> args;
  while (options.next("F:O:")) {
    switch (options.getOptChar()) {
    case 'F':
      federationExecutionName = OpenRTI::localeToUcs(options.getArgument());
      break;
    case 'O':
      fomModule = OpenRTI::localeToUcs(options.getArgument());
      break;
    }
  }
  OpenRTI::RTI1516ESimpleAmbassador ambassador;
  ambassador.setUseDataUrlObjectModels(false);

  try
  {
    ambassador.connect(L"thread://");
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }
  // create, must work
  try
  {
    ambassador.createFederationExecution(federationExecutionName, fomModule);
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }

  rti1516ev::FederateHandle federateHandle;

  // join must work
  try
  {
    federateHandle = ambassador.joinFederationExecution(L"federate", federationExecutionName);
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }

  if (!testTightBEEncoding())
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAASCIIcharDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAunicodeCharDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAbooleanDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAbyteDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAoctetDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAoctetPairBEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAoctetPairLEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAinteger16BEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAinteger16LEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAinteger32BEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAinteger32LEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAinteger64BEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAinteger64LEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAfloat32BEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAfloat32LEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAfloat64BEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAfloat64LEDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAASCIIstringDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAunicodeStringDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAfederateHandleDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAobjectClassHandleDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAobjectInstanceHandleDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(Vec3fDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(Vec3dDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(VecfDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(VecdDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(GeodfDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(GeoddDataType))
    return EXIT_FAILURE;

  if (!testDataElementEncoding(StructAlignDataType))
    return EXIT_FAILURE;

  if (!testDataElementEncoding(StructAlign2DataType))
    return EXIT_FAILURE;

  uint8_t bytes[] = { 0x02, 0x08, 0x20, 0x80 };
  if (!testFixedArrayOfBasicTypeEncoding<rti1516ev::HLAoctet>(bytes))
    return EXIT_FAILURE;

  if (!testFixedArrayOfBasicTypeEncoding<rti1516ev::HLAbyte>(bytes))
    return EXIT_FAILURE;

  int16_t words[] = { 0x0202, 0x0808, 0x2020, 0x7fff };
  if (!testFixedArrayOfBasicTypeEncoding<rti1516ev::HLAinteger16LE>(words))
    return EXIT_FAILURE;

  if (!testFixedArrayOfBasicTypeEncoding<rti1516ev::HLAinteger16BE>(words))
    return EXIT_FAILURE;

  int32_t dwords[] = { 0x02020202, 0x08080808, 0x20202020, 0x7fffffff };
  if (!testFixedArrayOfBasicTypeEncoding<rti1516ev::HLAinteger32LE>(dwords))
    return EXIT_FAILURE;

  if (!testFixedArrayOfBasicTypeEncoding<rti1516ev::HLAinteger32BE>(dwords))
    return EXIT_FAILURE;

  int64_t qwords[] = { 0x0202020202020202, 0x0808080808080808, 0x2020202020202020, 0x7fffffffffffffff };
  if (!testFixedArrayOfBasicTypeEncoding<rti1516ev::HLAinteger64LE>(qwords))
    return EXIT_FAILURE;

  if (!testFixedArrayOfBasicTypeEncoding<rti1516ev::HLAinteger64BE>(qwords))
    return EXIT_FAILURE;

  if (!testFixedArrayOfUnicodeStringEncoding())
    return EXIT_FAILURE;

  if (!testFixedArrayOfASCIIStringEncoding())
    return EXIT_FAILURE;

  if (!testVariableArrayOfASCIIStringEncoding())
    return EXIT_FAILURE;

  if (!testFixedArrayOfUnicodeStringEncoding())
    return EXIT_FAILURE;

  if (!testVariableArrayOfUnicodeStringEncoding())
    return EXIT_FAILURE;
  if (!testDataPointerEncoding())
    return EXIT_FAILURE;

  if (!testDataPointerFixedArrayEncoding())
    return EXIT_FAILURE;

  if (!testDataPointerVariableArrayEncoding())
    return EXIT_FAILURE;

  if (!testDataPointerFixedRecordEncoding())
    return EXIT_FAILURE;

  if (!testFixedRecordWithVersionsEncoding())
    return EXIT_FAILURE;

  if (!testFixedArrayOfFederateHandlesEncoding())
    return EXIT_FAILURE;

  if (!testVariableArrayOfFederateHandlesEncoding())
    return EXIT_FAILURE;

  if (!testVariableArrayOfObjectClassHandlesEncoding())
    return EXIT_FAILURE;

  if (!testFixedArrayOfAttributeHandlesEncoding())
    return EXIT_FAILURE;

  if (!testVariableArrayOfAttributeHandlesEncoding())
    return EXIT_FAILURE;

  if (!testFixedArrayOfInteractionClassHandlesEncoding())
    return EXIT_FAILURE;

  if (!testVariableArrayOfInteractionClassHandlesEncoding())
    return EXIT_FAILURE;

  if (!testVariableArrayOfInteractionSubscriptionsEncoding())
    return EXIT_FAILURE;

  if (!testFederateHandleEncodings(ambassador, federateHandle))
    return EXIT_FAILURE;

  rti1516ev::ObjectClassHandle objectClass1 = ambassador.getObjectClassHandle(L"HLAobjectRoot.ObjectClass1");
  if (!testObjectClassHandleEncodings(ambassador, objectClass1))
    return EXIT_FAILURE;
  rti1516ev::ObjectClassHandle objectClass2 = ambassador.getObjectClassHandle(L"HLAobjectRoot.ObjectClass1.ObjectClass2");
  if (!testObjectClassHandleEncodings(ambassador, objectClass2))
    return EXIT_FAILURE;
  rti1516ev::InteractionClassHandle interactionClass1 = ambassador.getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass1");
  if (!testInteractionClassHandleEncodings(ambassador, interactionClass1))
    return EXIT_FAILURE;

  if (!testDataElementEncoding(HLAfederateHandleDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAobjectClassHandleDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAobjectInstanceHandleDataType))
    return EXIT_FAILURE;

  if (!testOpaqueDataEncoding())
    return EXIT_FAILURE;
  //Leaktest();

  try
  {
    ambassador.resignFederationExecution(rti1516ev::NO_ACTION);
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }

  // destroy, must work
  try
  {
    ambassador.destroyFederationExecution(federationExecutionName);
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }

  try
  {
    ambassador.shutdown();
  }
  catch (const rti1516ev::Exception& e)
  {
    std::wcout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }

  if (gDebugPrint) std::wcout << "all tests SUCCEEDED!" << std::endl;
  return EXIT_SUCCESS;
}
