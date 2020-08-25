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

#include "../../../src/rti1516e/HandleFactory.h"
#include "../../../src/OpenRTI/DebugNew.h"

#include <RTI1516ETestLib.h>

#include "Rand.h"
#include <crtdbg.h>

constexpr bool gDebugPrint = false;
constexpr int doIterations = 5;

static void DumpHeap()
{
  _CrtDumpMemoryLeaks();
}

static int initHeapDump = atexit( DumpHeap);

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
  std::unique_ptr<rti1516e::DataElement> createDataElement(OpenRTI::Rand& rand) const;
};

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

std::unique_ptr<rti1516e::DataElement>
DataType::createDataElement(OpenRTI::Rand& rand) const
{
  switch (type)
  {
    case HLAASCIIchar:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAASCIIchar(static_cast<char>(rand.get())));
    case HLAunicodeChar:
      // under linux wchar_t is 32 bits but unicode char is only 16 bits ...
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAunicodeChar(0xffff & rand.get()));
    case HLAboolean:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAboolean(rand.get()));
    case HLAbyte:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAbyte(static_cast<unsigned char>(rand.get())));
    case HLAoctet:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAoctet(static_cast<unsigned char>(rand.get())));
    case HLAoctetPairBE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAoctetPairBE(rti1516e::OctetPair(static_cast<unsigned char>(rand.get()), static_cast<unsigned char>(rand.get()))));
    case HLAoctetPairLE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAoctetPairLE(rti1516e::OctetPair(static_cast<unsigned char>(rand.get()), static_cast<unsigned char>(rand.get()))));
    case HLAinteger16LE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAinteger16LE(static_cast<rti1516e::Integer16>(rand.get())));
    case HLAinteger16BE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAinteger16BE(static_cast<rti1516e::Integer16>(rand.get())));
    case HLAinteger32BE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAinteger32BE(rand.get()));
    case HLAinteger32LE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAinteger32LE(rand.get()));
    case HLAinteger64BE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAinteger64BE(rti1516e::Integer64(rand.get()) | rti1516e::Integer64(rand.get()) << 32));
    case HLAinteger64LE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAinteger64LE(rti1516e::Integer64(rand.get()) | rti1516e::Integer64(rand.get()) << 32));
    case HLAfloat32BE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAfloat32BE(static_cast<float>(rand.get())));
    case HLAfloat32LE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAfloat32LE(static_cast<float>(rand.get())));
    case HLAfloat64BE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAfloat64BE(static_cast<double>(rand.get())));
    case HLAfloat64LE:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAfloat64LE(static_cast<double>(rand.get())));

    case HLAASCIIstring:
    {
      std::string s;
      s.resize(0xff & rand.get());
      for (std::size_t i = 0; i < s.size(); ++i)
        s[i] = static_cast<char>(rand.get());
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAASCIIstring(s));
    }
    case HLAunicodeString:
    {
      std::wstring s;
      s.resize(0xff & rand.get());
      for (std::size_t i = 0; i < s.size(); ++i)
        s[i] = 0xffff & static_cast<uint16_t>(rand.get());
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAunicodeString(s));
    }

    case HLAfixedArray:
    {
      std::unique_ptr<rti1516e::DataElement> dataElement(dataTypes[0]->createDataElement(rand));
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAfixedArray(*dataElement, count));
    }
    case HLAvariableArray:
    {
      std::unique_ptr<rti1516e::DataElement> dataElement(dataTypes[0]->createDataElement(rand));
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAvariableArray(*dataElement));
    }
    case HLAfixedRecord_Randomized:
    {
      std::unique_ptr<rti1516e::HLAfixedRecord> fixedRecord = std::unique_ptr<rti1516e::HLAfixedRecord>(new rti1516e::HLAfixedRecord());
      for (std::size_t i = 0; i < count; ++i)
      {
        std::unique_ptr<rti1516e::DataElement> dataElement(dataTypes[i]->createDataElement(rand));
        fixedRecord->appendElement(*dataElement);
      }
      return fixedRecord;
    }

    case HLAfixedRecord_WithElementPointers:
    {
      std::unique_ptr<rti1516e::HLAfixedRecord> fixedRecord = std::unique_ptr<rti1516e::HLAfixedRecord>(new rti1516e::HLAfixedRecord());
      for (std::size_t i = 0; i < count; ++i)
      {
        std::unique_ptr<rti1516e::DataElement> dataElement(dataTypes[i]->createDataElement(rand));
        fixedRecord->appendElement(*dataElement);
      }
      return fixedRecord;
    }

    case HLAfederateHandle:
    {
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAhandle(rti1516e::createFederateHandle(rand.get())));

    }
    case HLAobjectClassHandle:
    {
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAhandle(rti1516e::createObjectClassHandle(rand.get())));
    }
    case HLAobjectInstanceHandle:
    {
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAhandle(rti1516e::createObjectInstanceHandle(rand.get())));
    }
    case HLAattributeHandle:
    {
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAhandle(rti1516e::createAttributeHandle(rand.get())));
    }
    case HLAinteractionClassHandle:
    {
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAhandle(rti1516e::createInteractionClassHandle(rand.get())));
    }
    case HLAparameterHandle:
    {
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAhandle(rti1516e::createParameterHandle(rand.get())));
    }
    default:
      return std::unique_ptr<rti1516e::DataElement>(new rti1516e::HLAopaqueData);
  }
}

bool
equal(const rti1516e::DataElement& left, const rti1516e::DataElement& right);

bool
equal(const rti1516e::HLAASCIIchar& left, const rti1516e::HLAASCIIchar& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAunicodeChar& left, const rti1516e::HLAunicodeChar& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAboolean& left, const rti1516e::HLAboolean& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAbyte& left, const rti1516e::HLAbyte& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAoctet& left, const rti1516e::HLAoctet& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAoctetPairBE& left, const rti1516e::HLAoctetPairBE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAoctetPairLE& left, const rti1516e::HLAoctetPairLE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAinteger16BE& left, const rti1516e::HLAinteger16BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAinteger16LE& left, const rti1516e::HLAinteger16LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAinteger32BE& left, const rti1516e::HLAinteger32BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAinteger32LE& left, const rti1516e::HLAinteger32LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAinteger64BE& left, const rti1516e::HLAinteger64BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAinteger64LE& left, const rti1516e::HLAinteger64LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAfloat32BE& left, const rti1516e::HLAfloat32BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAfloat32LE& left, const rti1516e::HLAfloat32LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAfloat64BE& left, const rti1516e::HLAfloat64BE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAfloat64LE& left, const rti1516e::HLAfloat64LE& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAASCIIstring& left, const rti1516e::HLAASCIIstring& right)
{ return left.get() == right.get(); }
bool
equal(const rti1516e::HLAunicodeString& left, const rti1516e::HLAunicodeString& right)
{ return left.get() == right.get(); }

bool
equal(const rti1516e::HLAhandle& left, const rti1516e::HLAhandle& right)
{
  return   left.getFederateHandle() == right.getFederateHandle()
           && left.getObjectClassHandle() == right.getObjectClassHandle()
           && left.getObjectInstanceHandle() == right.getObjectInstanceHandle()
           && left.getAttributeHandle() == right.getAttributeHandle()
           && left.getInteractionClassHandle() == right.getInteractionClassHandle()
           && left.getParameterHandle() == right.getParameterHandle();
}

bool
equal(const rti1516e::HLAfixedArray& left, const rti1516e::HLAfixedArray& right)
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
equal(const rti1516e::HLAvariableArray& left, const rti1516e::HLAvariableArray& right)
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
equal(const rti1516e::HLAfixedRecord& left, const rti1516e::HLAfixedRecord& right)
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
equal(const rti1516e::HLAopaqueData& left, const rti1516e::HLAopaqueData& right)
{
  if (left.dataLength() != right.dataLength())
    return false;
  return 0 == std::memcmp(left.get(), right.get(), left.dataLength());
}

bool
equal(const rti1516e::VariableLengthData& left, const rti1516e::VariableLengthData& right)
{
  if (left.size() != right.size())
    return false;
  return 0 == std::memcmp(left.data(), right.data(), left.size());
}

template<typename T>
bool
typedEqual(const rti1516e::DataElement& left, const rti1516e::DataElement& right)
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
equal(const rti1516e::DataElement& left, const rti1516e::DataElement& right)
{
  // this one throws std::bad_cast ... very often
  if (typedEqual<rti1516e::HLAASCIIchar>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAunicodeChar>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAboolean>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAbyte>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAoctet>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAoctetPairBE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAoctetPairLE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAinteger16BE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAinteger16LE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAinteger32BE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAinteger32LE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAinteger64BE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAinteger64LE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAfloat32BE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAfloat32LE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAfloat64BE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAfloat64LE>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAASCIIstring>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAunicodeString>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAunicodeString>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAhandle>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAfixedArray>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAvariableArray>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAfixedRecord>(left, right))
    return true;
  if (typedEqual<rti1516e::HLAopaqueData>(left, right))
    return true;
  return false;
}

bool testDataElementEncoding(const DataType& dataType)
{
  OpenRTI::Rand rand;
  for (unsigned i = 0; i < 100; ++i)
  {
    std::unique_ptr<rti1516e::DataElement> dataElement(dataType.createDataElement(rand));
    if (!dataElement.get())
    {
      std::cerr << "Failed to create a rti1516e::DataElement to test with!" << std::endl;
      return false;
    }
    //try
    //{
    //  dataElement->decode(rti1516e::VariableLengthData());
    //}
    //catch (const rti1516e::EncoderException& /*e*/)
    //{
    //  // Ok, shall happen here
    //}
    //catch (...)
    //{
    //  std::cerr << "Unexpected exception while decoding empty data!" << std::endl;
    //  return false;
    //}
    rti1516e::VariableLengthData variableLengthData;
    try
    {
      variableLengthData = dataElement->encode();
    }
    catch (const rti1516e::Exception& e)
    {
      std::cerr << "Unexpected exception while encoding: " << e << std::endl;
      return false;
    }
    catch (...)
    {
      std::cerr << "Unexpected exception while encoding!" << std::endl;
      return false;
    }

    std::unique_ptr<rti1516e::DataElement> dataElement2 = dataElement->clone();
    try
    {
      dataElement2->decode(variableLengthData);
    }
    catch (const rti1516e::Exception& e)
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

    rti1516e::VariableLengthData variableLengthData2;
    try
    {
      variableLengthData2 = dataElement->encode();
    }
    catch (...)
    {
      std::cerr << "Unexpected exception while encoding!" << std::endl;
      return false;
    }

    if (!equal(variableLengthData, variableLengthData2))
    {
      std::cerr << "Encoded data is not equal!" << std::endl;
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
operator<<(std::basic_ostream<char_type, traits_type>& os, const rti1516e::VariableLengthData& value)
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

bool testHandleEncodings(OpenRTI::RTI1516ESimpleAmbassador& ambassador, rti1516e::FederateHandle federateHandle)
{
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing " << federateHandle << std::endl;
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  rti1516e::HLAhandle encodedHandle = rti1516e::HLAhandle(federateHandle);
  // encoded data from HLAhandle
  rti1516e::VariableLengthData data1 = encodedHandle.encode();
  if (gDebugPrint) std::cout << "data1=" << data1 << std::endl;
  // encoded data from rti1516e::FederateHandle
  rti1516e::VariableLengthData data2 = federateHandle.encode();
  if (gDebugPrint) std::cout << "data2=" << data2 << std::endl;

  // check the basic sizes of the HLAhandle ...
  if (encodedHandle.getEncodedLength() != 16)
  {
    std::wcerr << L"unexpected retval from HLAhandle::getEncodedLength: " << encodedHandle.getEncodedLength() << std::endl;
    return false;
  }
  if (data1.size() != 16)
  {
    std::wcerr << L"unexpected encoded data size from HLAhandle: " << data1.size() << std::endl;
    return false;
  }
  // ... and the FederateHandle
  if (federateHandle.encodedLength() != 8)
  {
    std::wcerr << L"unexpected retval from rti1516e::FederateHandle::encodedLength: " << federateHandle.encodedLength() << std::endl;
    return false;
  }
  if (data2.size() != 8)
  {
    std::wcerr << L"unexpected encoded data size from rti1516e::FederateHandle::encode: " << data1.size() << std::endl;
    return false;
  }

  if (encodedHandle.getFederateHandle() != federateHandle)
  {
    std::wcerr << L"handle stored in HLAhandle differs from input: " << encodedHandle.getFederateHandle() << " != " << federateHandle << std::endl;
    return false;
  }

  // let the ambassador decode the data stored in HLAhandle (after the handle kind discriminator)
  rti1516e::VariableLengthData data3 = rti1516e::VariableLengthData(static_cast<const char*>(data1.data()) + 8, federateHandle.encodedLength());
  rti1516e::FederateHandle decodedObjectClass = ambassador.decodeFederateHandle(data3);

  if (decodedObjectClass != federateHandle)
  {
    std::wcerr << L"handle decoded by ambassador differs from input: " << federateHandle << L" != " << decodedObjectClass << std::endl;
    return false;
  }
  std::wcout << federateHandle << L" OK" << std::endl;
  return true;
}

bool testHandleEncodings(OpenRTI::RTI1516ESimpleAmbassador& ambassador, rti1516e::ObjectClassHandle objectClassHandle)
{
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing " << ambassador.getObjectClassName(objectClassHandle) << std::endl;
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  rti1516e::HLAhandle encodedHandle = rti1516e::HLAhandle(objectClassHandle);
  // encoded data from HLAhandle
  rti1516e::VariableLengthData data1 = encodedHandle.encode();
  if (gDebugPrint) std::cout << "data1=" << data1 << std::endl;
  // encoded data from rti1516e::ObjectClassHandle
  rti1516e::VariableLengthData data2 = objectClassHandle.encode();
  if (gDebugPrint) std::cout << "data2=" << data2 << std::endl;

  // check the basic sizes of the HLAhandle ...
  if (encodedHandle.getEncodedLength() != 16)
  {
    std::wcerr << L"unexpected retval from HLAhandle::getEncodedLength: " << encodedHandle.getEncodedLength() << std::endl;
    return false;
  }
  if (data1.size() != 16)
  {
    std::wcerr << L"unexpected encoded data size from HLAhandle: " << data1.size() << std::endl;
    return false;
  }
  // ... and the ObjectClassHandle
  if (objectClassHandle.encodedLength() != 8)
  {
    std::wcerr << L"unexpected retval from rti1516e::ObjectClassHandle::encodedLength: " << objectClassHandle.encodedLength() << std::endl;
    return false;
  }
  if (data2.size() != 8)
  {
    std::wcerr << L"unexpected encoded data size from rti1516e::ObjectClassHandle::encode: " << data1.size() << std::endl;
    return false;
  }

  if (encodedHandle.getObjectClassHandle() != objectClassHandle)
  {
    std::wcerr << L"handle stored in HLAhandle differs from input: " << encodedHandle.getObjectClassHandle() << " != " << objectClassHandle << std::endl;
    return false;
  }

  // let the ambassador decode the data stored in HLAhandle (after the handle kind discriminator)
  rti1516e::VariableLengthData data3 = rti1516e::VariableLengthData(static_cast<const char*>(data1.data()) + 8, objectClassHandle.encodedLength());
  rti1516e::ObjectClassHandle decodedObjectClass = ambassador.decodeObjectClassHandle(data3);

  if (decodedObjectClass != objectClassHandle)
  {
    std::wcerr << L"handle decoded by ambassador differs from input: " << objectClassHandle << L" != " << decodedObjectClass << std::endl;
    return false;
  }
  std::wcout << ambassador.getObjectClassName(objectClassHandle) << L" OK" << std::endl;
  return true;
}

bool testHandleEncodings(OpenRTI::RTI1516ESimpleAmbassador& ambassador, rti1516e::InteractionClassHandle interactionClassHandle)
{
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  if (gDebugPrint) std::wcout << L"testing interaction class" << ambassador.getInteractionClassName(interactionClassHandle)  << std::endl;
  if (gDebugPrint) std::cout << "============================================" << std::endl;
  rti1516e::HLAhandle encodedHandle = rti1516e::HLAhandle(interactionClassHandle);
  // encoded data from HLAhandle
  rti1516e::VariableLengthData data1 = encodedHandle.encode();
  if (gDebugPrint) std::cout << "data1=" << data1 << std::endl;
  // encoded data from rti1516e::InteractionClassHandle
  rti1516e::VariableLengthData data2 = interactionClassHandle.encode();
  if (gDebugPrint) std::cout << "data2=" << data2 << std::endl;

  // check the basic sizes of the HLAhandle ...
  if (encodedHandle.getEncodedLength() != 16)
  {
    std::wcerr << L"unexpected retval from HLAhandle::getEncodedLength: " << encodedHandle.getEncodedLength() << std::endl;
    return false;
  }
  if (data1.size() != 16)
  {
    std::wcerr << L"unexpected encoded data size from HLAhandle: " << data1.size() << std::endl;
    return false;
  }
  // ... and the InteractionClassHandle
  if (interactionClassHandle.encodedLength() != 8)
  {
    std::wcerr << L"unexpected retval from rti1516e::InteractionClassHandle::encodedLength: " << interactionClassHandle.encodedLength() << std::endl;
    return false;
  }
  if (data2.size() != 8)
  {
    std::wcerr << L"unexpected encoded data size from rti1516e::InteractionClassHandle::encode: " << data1.size() << std::endl;
    return false;
  }

  if (encodedHandle.getInteractionClassHandle() != interactionClassHandle)
  {
    std::wcerr << L"handle stored in HLAhandle differs from input: " << encodedHandle.getInteractionClassHandle() << " != " << interactionClassHandle << std::endl;
    return false;
  }

  // let the ambassador decode the data stored in HLAhandle (after the handle kind discriminator)
  rti1516e::VariableLengthData data3 = rti1516e::VariableLengthData(static_cast<const char*>(data1.data()) + 8, interactionClassHandle.encodedLength());
  rti1516e::InteractionClassHandle decodedInteractionClass = ambassador.decodeInteractionClassHandle(data3);

  if (decodedInteractionClass != interactionClassHandle)
  {
    std::wcerr << L"handle decoded by ambassador differs from input: " << interactionClassHandle << L" != " << decodedInteractionClass << std::endl;
    return false;
  }
  std::wcout << ambassador.getInteractionClassName(interactionClassHandle) << L" OK" << std::endl;
  return true;
}

void ProcessData(std::list<rti1516e::VariableLengthData>& theList, std::condition_variable& cv, std::mutex& m, std::atomic_bool& finish)
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
  std::list<rti1516e::VariableLengthData> theData;
  std::mutex m;
  std::condition_variable cv;
  std::atomic_bool finish = false;
  std::thread process(ProcessData, std::ref(theData), std::ref(cv), std::ref(m), std::ref(finish));

  for (int i = 0; i< 1000; i++)
  {
    auto hlaHandle = rti1516e::HLAhandle(rti1516e::createFederateHandle(1));
    rti1516e::VariableLengthData handleData = hlaHandle.encode();
    {
        std::lock_guard<std::mutex> lk(m);
        theData.push_back(handleData);
    }
    cv.notify_one();
    if (gDebugPrint) std::cout << handleData << std::endl;
    int32_t value = i;
    rti1516e::HLAinteger32LE encoding1(&value);
    rti1516e::VariableLengthData x = encoding1.encode();
    if (gDebugPrint) std::cout << x << std::endl;
    rti1516e::VariableLengthData y;
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
  rti1516e::HLAinteger32LE encoding1(&value);
  rti1516e::VariableLengthData data;
  encoding1.encode(data);
  rti1516e::HLAinteger32LE encoding2;
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
  rti1516e::HLAbyte encoder1[8];
  rti1516e::HLAbyte encoder2[8];
  rti1516e::HLAfixedArray arrayEncoder1(rti1516e::HLAbyte(), 8);
  rti1516e::HLAfixedArray arrayEncoder2(rti1516e::HLAbyte(), 8);
  for (int i=0;i<8;i++)
  {
    encoder1[i].setDataPointer(&data1.data()[i]);
    encoder2[i].setDataPointer(&data2.data()[i]);
    arrayEncoder1.setElementPointer(i, &encoder1[i]);
    arrayEncoder2.setElementPointer(i, &encoder2[i]);
  }
  rti1516e::VariableLengthData encodedData1 = arrayEncoder1.encode();
  if (gDebugPrint) std::cout << "encodedData1=" << encodedData1 << std::endl;
  rti1516e::VariableLengthData encodedData2 = arrayEncoder2.encode();
  if (gDebugPrint) std::cout << "encodedData2=" << encodedData2 << std::endl;

  // set up decoder, to decode back the data produced by the encoder, before
  rti1516e::HLAfixedArray arrayDecoder(rti1516e::HLAbyte(), 8);
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
    arrayEncoder1.set(0, rti1516e::HLAinteger32LE());
    std::cout << "HLAfixedArray(HLAbyte).set accepts HLAinteger32LE - bad" << std::endl;
    return false;
  }
  catch (const rti1516e::EncoderException& e)
  {
    if (gDebugPrint) std::cout << "Expected EncoderException: " << e << std::endl;
  }
  try {
    rti1516e::HLAinteger32LE enc;
    arrayEncoder1.setElementPointer(0, &enc);
    std::cout << "HLAfixedArray(HLAbyte).setElementPointer accepts HLAinteger32LE* - bad" << std::endl;
    return false;
  }
  catch (const rti1516e::EncoderException& e)
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
  rti1516e::HLAbyte encoder1[8];
  rti1516e::HLAbyte encoder2[8];
  rti1516e::HLAvariableArray arrayEncoder1{rti1516e::HLAbyte()};
  rti1516e::HLAvariableArray arrayEncoder2{rti1516e::HLAbyte()};
  for (int i=0;i<8;i++)
  {
    encoder1[i].setDataPointer(&data1.data()[i]);
    encoder2[i].setDataPointer(&data2.data()[i]);
    arrayEncoder1.addElementPointer(&encoder1[i]);
    arrayEncoder2.addElementPointer(&encoder2[i]);
  }
  rti1516e::VariableLengthData encodedData1 = arrayEncoder1.encode();
  if (gDebugPrint) std::cout << "encodedData1=" << encodedData1 << std::endl;
  rti1516e::VariableLengthData encodedData2 = arrayEncoder2.encode();
  if (gDebugPrint) std::cout << "encodedData2=" << encodedData2 << std::endl;

  // set up decoder, to decode back the data produced by the encoder, before
  rti1516e::HLAvariableArray arrayDecoder{rti1516e::HLAbyte()};
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
    arrayEncoder1.set(0, rti1516e::HLAinteger32LE());
    std::cout << "HLAvariableArray(HLAbyte).set accepts HLAinteger32LE - bad" << std::endl;
    return false;
  }
  catch (const rti1516e::EncoderException& e)
  {
    if (gDebugPrint) std::cout << "Expected EncoderException: " << e << std::endl;
  }
  try {
    rti1516e::HLAinteger32LE enc;
    arrayEncoder1.setElementPointer(0, &enc);
    std::cout << "HLAvariableArray(HLAbyte).setElementPointer accepts HLAinteger32LE* - bad" << std::endl;
    return false;
  }
  catch (const rti1516e::EncoderException& e)
  {
    if (gDebugPrint) std::cout << "Expected EncoderException: " << e << std::endl;
  }
  std::cout << "variable array to data pointer encoding OK" << std::endl;
  return true;
}

// test array with data elements given by 'borrowed' encoder pointers, each pointing to 'borrowed' (external) memory
bool testVariableArrayOfStringEncoding()
{
  std::string data [] = { "one", "two", "three", "four" };
  rti1516e::HLAvariableArray arrayEncoder1{rti1516e::HLAASCIIstring()};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.addElement(rti1516e::HLAASCIIstring(data[i]));
  }
  rti1516e::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516e::HLAvariableArray arrayDecoder{rti1516e::HLAASCIIstring()};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::cout << static_cast<const rti1516e::HLAASCIIstring&>(arrayEncoder1.get(i)).get() << std::endl;
    if (static_cast<const rti1516e::HLAASCIIstring&>(arrayEncoder1.get(i)).get() != data[i])
      return false;
  }
  std::cout << "variable array of strings OK" << std::endl;
  return true;
}

bool testFixedArrayOfStringEncoding()
{
  std::string data [] = { "one", "two", "three", "four" };
  rti1516e::HLAfixedArray arrayEncoder1{rti1516e::HLAASCIIstring(), 4};
  for (int i=0;i<4;i++)
  {
    arrayEncoder1.set(i, rti1516e::HLAASCIIstring(data[i]));
  }
  rti1516e::VariableLengthData encodedData1 = arrayEncoder1.encode();
  std::cout << "encodedData1=" << encodedData1 << std::endl;

  rti1516e::HLAfixedArray arrayDecoder{rti1516e::HLAASCIIstring(), 4};
  arrayDecoder.decode(encodedData1);
  for (int i=0;i<4;i++)
  {
    std::cout << static_cast<const rti1516e::HLAASCIIstring&>(arrayEncoder1.get(i)).get() << std::endl;
    if (static_cast<const rti1516e::HLAASCIIstring&>(arrayEncoder1.get(i)).get() != data[i])
      return false;
  }
  std::cout << "fixed array of strings OK" << std::endl;
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
  rti1516e::HLAbyte encoder1[8];
  rti1516e::HLAbyte encoder2[8];
  rti1516e::HLAfixedArray arrayEncoder1(rti1516e::HLAbyte(), 8);
  rti1516e::HLAfixedArray arrayEncoder2(rti1516e::HLAbyte(), 8);
  for (int i=0;i<8;i++)
  {
    encoder1[i].setDataPointer(&data1.data()[i]);
    encoder2[i].setDataPointer(&data2.data()[i]);
    arrayEncoder1.setElementPointer(i, &encoder1[i]);
    arrayEncoder2.setElementPointer(i, &encoder2[i]);
  }
  int i1 = 123;
  rti1516e::HLAinteger32LE intEncoder;
  intEncoder.setDataPointer(&i1);
  char c1 = 123;
  rti1516e::HLAASCIIchar charEncoder;
  charEncoder.setDataPointer(&c1);

  // now set up the fixed array
  rti1516e::HLAfixedRecord recordEncoder;
  recordEncoder.appendElementPointer(&intEncoder);
  recordEncoder.appendElementPointer(&charEncoder);
  recordEncoder.appendElementPointer(&arrayEncoder1);
  rti1516e::VariableLengthData encodedData1 = recordEncoder.encode();
  if (gDebugPrint) std::cout << "encodedData1=" << encodedData1 << std::endl;
  // now decode all from encodedData1:
  // prepare recordDecoder
  rti1516e::HLAfixedRecord recordDecoder;
  recordDecoder.appendElement(rti1516e::HLAinteger32LE());
  recordDecoder.appendElement(rti1516e::HLAASCIIchar());
  // prepare array decoder
  rti1516e::HLAfixedArray arrayDecoder(rti1516e::HLAbyte(), 8);
  std::vector<uint8_t> decodedArray{0, 0, 0, 0, 0, 0, 0, 0};
  rti1516e::HLAbyte decoder[8];
  for (int i=0;i<8;i++)
  {
    decoder[i].setDataPointer(&decodedArray.data()[i]);
    arrayDecoder.setElementPointer(i, &decoder[i]);
  }
  recordDecoder.appendElementPointer(&arrayDecoder);
  // decode
  recordDecoder.decode(encodedData1);
  // fields should now equal given data
  if (gDebugPrint) std::cout << "field1=" << static_cast<const rti1516e::HLAinteger32LE&>(recordDecoder.get(0)).get() << std::endl;
  if (gDebugPrint) std::cout << "field2=" << static_cast<int>(static_cast<const rti1516e::HLAASCIIchar&>(recordDecoder.get(1)).get()) << std::endl;
  if (gDebugPrint) std::cout << "field3=" << decodedArray << std::endl;
  const rti1516e::HLAfixedArray& array = dynamic_cast<const rti1516e::HLAfixedArray&>(recordDecoder.get(2));
  if (gDebugPrint) std::cout << "data from distinct HLAbytes:" << std::endl;
  for (int i=0;i<8;i++)
  {
    const rti1516e::HLAbyte& byteDecoder = dynamic_cast<const rti1516e::HLAbyte&>(array.get(i));
    if (gDebugPrint) std::cout << " " << static_cast<int>(byteDecoder.get());
  }
  if (gDebugPrint) std::cout << std::endl;
  std::cout << "fixed record to data pointer encoding OK" << std::endl;
  return true;
}

int
main(int argc, char* argv[])
{
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

  if (!testFixedArrayOfStringEncoding())
    return EXIT_FAILURE;

  if (!testVariableArrayOfStringEncoding())
    return EXIT_FAILURE;

  if (!testDataPointerEncoding())
    return EXIT_FAILURE;

  if (!testDataPointerFixedArrayEncoding())
    return EXIT_FAILURE;

  if (!testDataPointerVariableArrayEncoding())
    return EXIT_FAILURE;

  if (!testDataPointerFixedRecordEncoding())
    return EXIT_FAILURE;

  OpenRTI::RTI1516ESimpleAmbassador ambassador;
  ambassador.setUseDataUrlObjectModels(false);

  try
  {
    ambassador.connect(L"thread://");
  }
  catch (const rti1516e::Exception& e)
  {
    std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }
  std::wstring federationExecutionName = L"test";
  // create, must work
  try
  {
    ambassador.createFederationExecution(federationExecutionName, L"fdd-1.xml");
  }
  catch (const rti1516e::Exception& e)
  {
    std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }

  rti1516e::FederateHandle federateHandle;

  // join must work
  try
  {
    federateHandle = ambassador.joinFederationExecution(L"federate", federationExecutionName);
  }
  catch (const rti1516e::Exception& e)
  {
    std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }

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

  if (!testDataPointerEncoding())
    return EXIT_FAILURE;

  if (!testDataPointerFixedArrayEncoding())
    return EXIT_FAILURE;

  if (!testDataPointerFixedRecordEncoding())
    return EXIT_FAILURE;

  // now run the tests

  testHandleEncodings(ambassador, federateHandle);

  rti1516e::ObjectClassHandle objectClass1 = ambassador.getObjectClassHandle(L"HLAobjectRoot.ObjectClass1");
  testHandleEncodings(ambassador, objectClass1);
  rti1516e::ObjectClassHandle objectClass2 = ambassador.getObjectClassHandle(L"HLAobjectRoot.ObjectClass1.ObjectClass2");
  testHandleEncodings(ambassador, objectClass2);
  rti1516e::InteractionClassHandle interactionClass1 = ambassador.getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass1");
  testHandleEncodings(ambassador, interactionClass1);

  if (!testDataElementEncoding(HLAfederateHandleDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAobjectClassHandleDataType))
    return EXIT_FAILURE;
  if (!testDataElementEncoding(HLAobjectInstanceHandleDataType))
    return EXIT_FAILURE;
  Leaktest();

  try
  {
    ambassador.resignFederationExecution(rti1516e::NO_ACTION);
  }
  catch (const rti1516e::Exception& e)
  {
    std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
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
  catch (const rti1516e::Exception& e)
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
  catch (const rti1516e::Exception& e)
  {
    std::wcout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::wcout << L"Unknown Exception!" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
