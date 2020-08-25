/* -*-c++-*- OpenRTI - Copyright (C) 2011-2016 Mathias Froehlich
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

#include "DebugNew.h"
#include "BasicDataElements.h"

#include "VariableLengthData.h"
#include "DataElement.h"

#include <cstring>
#include <limits>
#include <vector>

#include "Encoding.h"
#include "Export.h"
#include "Types.h"
//#include "VariableLengthDataImplementation.h"

#pragma warning(disable: 4018)

namespace OpenRTI
{

#define IMPLEMENT_ENCODING_HELPER_CLASS(EncodableDataType, SimpleDataType, encoder) \
                                                                        \
class OPENRTI_LOCAL EncodableDataType##Implementation {                 \
public:                                                                 \
  EncodableDataType##Implementation() :                                 \
    _value()                                                            \
  {                                                                     \
    _valuePointer = & _value;                                           \
  }                                                                     \
  EncodableDataType##Implementation(const SimpleDataType& value) :      \
    _value(value)                                                       \
  {                                                                     \
    _valuePointer = & _value;                                           \
  }                                                                     \
  EncodableDataType##Implementation(SimpleDataType* valuePointer) :     \
    _value()                                                            \
  {                                                                     \
    _valuePointer = valuePointer;                                       \
  }                                                                     \
  EncodableDataType##Implementation(const EncodableDataType##Implementation& rhs) : \
    _value(rhs._value)                                                  \
  {                                                                     \
    if (rhs._valuePointer != &rhs._value) {                             \
      _valuePointer = rhs._valuePointer;                                \
    } else {                                                            \
      _valuePointer = &_value;                                          \
    }                                                                   \
  }                                                                     \
  virtual ~EncodableDataType##Implementation()                          \
  { }                                                                   \
                                                                        \
  EncodableDataType##Implementation&                                    \
  operator=(const EncodableDataType##Implementation& rhs)               \
  { set(rhs.get()); return *this; }                                     \
                                                                        \
  encoder                                                               \
                                                                        \
  void set(const SimpleDataType& inData)                                \
  {                                                                     \
    *_valuePointer = inData;                                            \
  }                                                                     \
                                                                        \
  void setDataPointer(SimpleDataType* inData)                           \
  {                                                                     \
    _valuePointer = inData;                                             \
  }                                                                     \
                                                                        \
  const SimpleDataType& get() const                                     \
  {                                                                     \
    return *_valuePointer;                                              \
  }                                                                     \
                                                                        \
  size_t align(size_t offset, size_t alignment) const                   \
  { return (offset + alignment - 1) & ~(alignment - 1); }               \
                                                                        \
  void align(std::vector<Octet>& buffer, size_t alignment) const        \
  {                                                                     \
    while (buffer.size() % alignment)                                   \
      buffer.push_back(0);                                              \
  }                                                                     \
                                                                        \
  SimpleDataType _value;                                                \
  SimpleDataType* _valuePointer;                                        \
};                                                                      \
                                                                        \
EncodableDataType::EncodableDataType() :                                \
  _impl(new EncodableDataType##Implementation)                          \
{                                                                       \
}                                                                       \
                                                                        \
EncodableDataType::EncodableDataType(SimpleDataType const& inData) :    \
  _impl(new EncodableDataType##Implementation(inData))                  \
{                                                                       \
}                                                                       \
                                                                        \
EncodableDataType::EncodableDataType(SimpleDataType* inData) :          \
  _impl(new EncodableDataType##Implementation(inData))                  \
{                                                                       \
}                                                                       \
                                                                        \
EncodableDataType::EncodableDataType(EncodableDataType const& rhs) :    \
  _impl(new EncodableDataType##Implementation(*rhs._impl))              \
{                                                                       \
}                                                                       \
                                                                        \
EncodableDataType::~EncodableDataType()                                 \
{                                                                       \
  delete _impl;                                                         \
}                                                                       \
                                                                        \
EncodableDataType&                                                      \
EncodableDataType::operator=(EncodableDataType const& rhs)              \
{                                                                       \
  *_impl = *rhs._impl;                                                  \
  return *this;                                                         \
}                                                                       \
                                                                        \
std::unique_ptr<DataElement>                                            \
EncodableDataType::clone() const                                        \
{                                                                       \
  return std::unique_ptr<DataElement>(new EncodableDataType(*this));    \
}                                                                       \
                                                                        \
VariableLengthData                                                      \
EncodableDataType::encode() const                                       \
{                                                                       \
  VariableLengthData variableLengthData;                                \
  encode(variableLengthData);                                           \
  return variableLengthData;                                            \
}                                                                       \
                                                                        \
void                                                                    \
EncodableDataType::encode(VariableLengthData& inData) const             \
{                                                                       \
  _impl->encode(inData);                                                \
}                                                                       \
                                                                        \
void                                                                    \
EncodableDataType::encodeInto(std::vector<Octet>& buffer) const         \
{                                                                       \
  return _impl->encodeInto(buffer);                                     \
}                                                                       \
                                                                        \
size_t                                                                  \
EncodableDataType::encodeInto(                                          \
  Octet* buffer, size_t bufferSize, size_t offset) const                \
{                                                                       \
  return _impl->encodeInto(buffer, bufferSize, offset);                 \
}                                                                       \
                                                                        \
void                                                                    \
EncodableDataType::decode(VariableLengthData const & inData)            \
{                                                                       \
  _impl->decode(inData);                                                \
}                                                                       \
                                                                        \
size_t                                                                  \
EncodableDataType::decodeFrom(std::vector<Octet> const & buffer, size_t index) \
{                                                                       \
  return _impl->decodeFrom(buffer.data(), buffer.size(), index);        \
}                                                                       \
                                                                        \
size_t                                                                  \
EncodableDataType::decodeFrom(const Octet* buffer, size_t bufferSize, size_t index) \
{                                                                       \
  return _impl->decodeFrom(buffer, bufferSize, index);                   \
}                                                                       \
                                                                        \
size_t                                                                  \
EncodableDataType::getEncodedLength() const                             \
{                                                                       \
  return _impl->getEncodedLength();                                     \
}                                                                       \
                                                                        \
unsigned int                                                            \
EncodableDataType::getOctetBoundary () const                            \
{                                                                       \
  return _impl->getOctetBoundary();                                     \
}                                                                       \
                                                                        \
Integer64                                                               \
EncodableDataType::hash() const                                         \
{                                                                       \
  return _impl->hash();                                                 \
}                                                                       \
                                                                        \
void                                                                    \
EncodableDataType::setDataPointer(SimpleDataType* inData)               \
{                                                                       \
  _impl->setDataPointer(inData);                                        \
}                                                                       \
                                                                        \
void                                                                    \
EncodableDataType::set(SimpleDataType inData)                           \
{                                                                       \
  _impl->set(inData);                                                   \
}                                                                       \
                                                                        \
SimpleDataType                                                          \
EncodableDataType::get() const                                          \
{                                                                       \
  return _impl->get();                                                  \
}                                                                       \
                                                                        \
EncodableDataType&                                                      \
EncodableDataType::operator=(SimpleDataType rhs)                        \
{                                                                       \
  _impl->set(rhs);                                                      \
  return *this;                                                         \
}                                                                       \
                                                                        \
EncodableDataType::operator SimpleDataType() const                      \
{                                                                       \
  return _impl->get();                                                  \
}

// 8 bit values
IMPLEMENT_ENCODING_HELPER_CLASS(HLAASCIIchar, char,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getChar(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setChar(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  if (index >= bufferSize)
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = buffer[index];
  return index + 1;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  buffer.push_back(*_valuePointer);
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  *(buffer + offset) = *_valuePointer;
  return offset + 1;
}

size_t
getEncodedLength() const
{
  return 1;
}

unsigned int
getOctetBoundary() const
{
  return 1;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAboolean, bool,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = bool(inData.getChar(0));
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setChar(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  if (index >= bufferSize)
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = bool(buffer[index]);
  return index + 1;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  buffer.push_back(*_valuePointer);
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  *(buffer + offset) = *_valuePointer;
  return offset + 1;
}

size_t
getEncodedLength() const
{
  return 1;
}

unsigned int
getOctetBoundary() const
{
  return 1;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAbyte, Octet,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getInt8(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setInt8(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  if (index >= bufferSize)
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = buffer[index];
  return index + 1;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  buffer.push_back(*_valuePointer);
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  *(buffer + offset) = *_valuePointer;
  return offset + 1;
}

size_t
getEncodedLength() const
{
  return 1;
}

unsigned int
getOctetBoundary() const
{
  return 1;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAoctet, Octet,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getInt8(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setInt8(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  if (index >= bufferSize)
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = buffer[index];
  return index + 1;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  buffer.push_back(*_valuePointer);
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  *(buffer + offset) = *_valuePointer;
  return offset + 1;
}

size_t
getEncodedLength() const
{
  return 1;
}

unsigned int
getOctetBoundary() const
{
  return 1;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)

// > 8 bit values, fixed endianess
IMPLEMENT_ENCODING_HELPER_CLASS(HLAunicodeChar, wchar_t,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getUInt16BE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setUInt16BE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 2);
  if (bufferSize < index + 2)
    throw EncoderException("Insufficient buffer size for decoding!");
  uint16_t u;
  u = uint16_t(uint8_t(buffer[index])) << 8;
  u |= uint16_t(uint8_t(buffer[index + 1]));
  *_valuePointer = wchar_t(u);
  return index + 2;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 2);
  uint16_t u = uint16_t(*_valuePointer);
  buffer.push_back(uint8_t(u >> 8));
  buffer.push_back(uint8_t(u));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 2);
  return encodeIntoBE16(buffer, bufferSize, offset, uint16_t(*_valuePointer));
}

size_t
getEncodedLength() const
{
  return 2;
}

unsigned int
getOctetBoundary() const
{
  return 2;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)

// > 8 bit values, both endianess
IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger16BE, Integer16,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getInt16BE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setInt16BE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 2);
  if (bufferSize < index + 2)
    throw EncoderException("Insufficient buffer size for decoding!");
  uint16_t u;
  u = uint16_t(uint8_t(buffer[index])) << 8;
  u |= uint16_t(uint8_t(buffer[index + 1]));
  *_valuePointer = Integer16(u);
  return index + 2;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 2);
  uint16_t u = *_valuePointer;
  buffer.push_back(uint8_t(u >> 8));
  buffer.push_back(uint8_t(u));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 2);
  return encodeIntoBE16(buffer, bufferSize, offset, uint16_t(*_valuePointer));
}

size_t
getEncodedLength() const
{
  return 2;
}

unsigned int
getOctetBoundary() const
{
  return 2;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger16LE, Integer16,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getInt16LE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setInt16LE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 2);
  if (bufferSize < index + 2)
    throw EncoderException("Insufficient buffer size for decoding!");
  uint16_t u;
  u = uint16_t(uint8_t(buffer[index]));
  u |= uint16_t(uint8_t(buffer[index + 1])) << 8;
  *_valuePointer = Integer16(u);
  return index + 2;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 2);
  uint16_t u = uint16_t(*_valuePointer);
  buffer.push_back(uint8_t(u));
  buffer.push_back(uint8_t(u >> 8));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 2);
  return encodeIntoLE16(buffer, bufferSize, offset, uint16_t(*_valuePointer));
}

size_t
getEncodedLength() const
{
  return 2;
}

unsigned int
getOctetBoundary() const
{
  return 2;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger32BE, Integer32,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getInt32BE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setInt32BE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 4);
  if (bufferSize < index + 4)
    throw EncoderException("Insufficient buffer size for decoding!");
  uint32_t u;
  u = uint32_t(uint8_t(buffer[index])) << 24;
  u |= uint32_t(uint8_t(buffer[index + 1])) << 16;
  u |= uint32_t(uint8_t(buffer[index + 2])) << 8;
  u |= uint32_t(uint8_t(buffer[index + 3]));
  *_valuePointer = Integer32(u);
  return index + 4;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 4);
  uint32_t u = uint32_t(*_valuePointer);
  buffer.push_back(uint8_t(u >> 24));
  buffer.push_back(uint8_t(u >> 16));
  buffer.push_back(uint8_t(u >> 8));
  buffer.push_back(uint8_t(u));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 4);
  return encodeIntoBE32(buffer, bufferSize, offset, uint32_t(*_valuePointer));
}


size_t
getEncodedLength() const
{
  return 4;
}

unsigned int
getOctetBoundary() const
{
  return 4;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger32LE, Integer32,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getInt32LE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setInt32LE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 4);
  if (bufferSize < index + 4)
    throw EncoderException("Insufficient buffer size for decoding!");
  uint32_t u;
  u = uint32_t(uint8_t(buffer[index]));
  u |= uint32_t(uint8_t(buffer[index + 1])) << 8;
  u |= uint32_t(uint8_t(buffer[index + 2])) << 16;
  u |= uint32_t(uint8_t(buffer[index + 3])) << 24;
  *_valuePointer = Integer32(u);
  return index + 4;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 4);
  uint32_t u = uint32_t(*_valuePointer);
  buffer.push_back(uint8_t(u));
  buffer.push_back(uint8_t(u >> 8));
  buffer.push_back(uint8_t(u >> 16));
  buffer.push_back(uint8_t(u >> 24));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 4);
  return encodeIntoLE32(buffer, bufferSize, offset, uint32_t(*_valuePointer));
}

size_t
getEncodedLength() const
{
  return 4;
}

unsigned int
getOctetBoundary() const
{
  return 4;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger64BE, Integer64,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getInt64BE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setInt64BE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 8);
  if (bufferSize < index + 8)
    throw EncoderException("Insufficient buffer size for decoding!");
  uint64_t u;
  u = uint64_t(uint8_t(buffer[index])) << 56;
  u |= uint64_t(uint8_t(buffer[index + 1])) << 48;
  u |= uint64_t(uint8_t(buffer[index + 2])) << 40;
  u |= uint64_t(uint8_t(buffer[index + 3])) << 32;
  u |= uint64_t(uint8_t(buffer[index + 4])) << 24;
  u |= uint64_t(uint8_t(buffer[index + 5])) << 16;
  u |= uint64_t(uint8_t(buffer[index + 6])) << 8;
  u |= uint64_t(uint8_t(buffer[index + 7]));
  *_valuePointer = Integer64(u);
  return index + 8;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 8);
  uint64_t u = uint64_t(*_valuePointer);
  buffer.push_back(uint8_t(u >> 56));
  buffer.push_back(uint8_t(u >> 48));
  buffer.push_back(uint8_t(u >> 40));
  buffer.push_back(uint8_t(u >> 32));
  buffer.push_back(uint8_t(u >> 24));
  buffer.push_back(uint8_t(u >> 16));
  buffer.push_back(uint8_t(u >> 8));
  buffer.push_back(uint8_t(u));
}


size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 8);
  return encodeIntoBE64(buffer, bufferSize, offset, uint64_t(*_valuePointer));
}

size_t
getEncodedLength() const
{
  return 8;
}

unsigned int
getOctetBoundary() const
{
  return 8;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger64LE, Integer64,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getInt64LE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setInt64LE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 8);
  if (bufferSize < index + 8)
    throw EncoderException("Insufficient buffer size for decoding!");
  uint64_t u;
  u = uint64_t(uint8_t(buffer[index]));
  u |= uint64_t(uint8_t(buffer[index + 1])) << 8;
  u |= uint64_t(uint8_t(buffer[index + 2])) << 16;
  u |= uint64_t(uint8_t(buffer[index + 3])) << 24;
  u |= uint64_t(uint8_t(buffer[index + 4])) << 32;
  u |= uint64_t(uint8_t(buffer[index + 5])) << 40;
  u |= uint64_t(uint8_t(buffer[index + 6])) << 48;
  u |= uint64_t(uint8_t(buffer[index + 7])) << 56;
  *_valuePointer = Integer64(u);
  return index + 8;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 8);
  uint64_t u = uint64_t(*_valuePointer);
  buffer.push_back(uint8_t(u));
  buffer.push_back(uint8_t(u >> 8));
  buffer.push_back(uint8_t(u >> 16));
  buffer.push_back(uint8_t(u >> 24));
  buffer.push_back(uint8_t(u >> 32));
  buffer.push_back(uint8_t(u >> 40));
  buffer.push_back(uint8_t(u >> 48));
  buffer.push_back(uint8_t(u >> 56));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 8);
  return encodeIntoLE64(buffer, bufferSize, offset, uint64_t(*_valuePointer));
}

size_t
getEncodedLength() const
{
  return 8;
}

unsigned int
getOctetBoundary() const
{
  return 8;
}

Integer64 hash() const
{
  return Integer64(*_valuePointer);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAoctetPairBE, OctetPair,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  _valuePointer->first = inData.getChar(0);
  _valuePointer->second = inData.getChar(1);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setChar(_valuePointer->first, 0);
  inData.setChar(_valuePointer->second, 1);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 2);
  if (bufferSize < index + 2)
    throw EncoderException("Insufficient buffer size for decoding!");
  _valuePointer->first = buffer[index];
  _valuePointer->second = buffer[index + 1];
  return index + 2;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 2);
  buffer.push_back(_valuePointer->first);
  buffer.push_back(_valuePointer->second);
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 2);
  auto* p = buffer + offset;
  *p++ = _valuePointer->first;
  *p++ = _valuePointer->second;
  return offset + 2;
}


size_t
getEncodedLength() const
{
  return 2;
}

unsigned int
getOctetBoundary() const
{
  return 2;
}

Integer64 hash() const
{
  return (Integer64(_valuePointer->first) << 8) | Integer64(_valuePointer->second);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAoctetPairLE, OctetPair,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  _valuePointer->second = inData.getChar(0);
  _valuePointer->first = inData.getChar(1);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setChar(_valuePointer->second, 0);
  inData.setChar(_valuePointer->first, 1);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 2);
  if (bufferSize < index + 2)
    throw EncoderException("Insufficient buffer size for decoding!");
  _valuePointer->second = buffer[index];
  _valuePointer->first = buffer[index + 1];
  return index + 2;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 2);
  buffer.push_back(_valuePointer->second);
  buffer.push_back(_valuePointer->first);
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 2);
  auto* p = buffer + offset;
  *p++ = _valuePointer->second;
  *p++ = _valuePointer->first;
  return offset + 2;
}

size_t
getEncodedLength() const
{
  return 2;
}

unsigned int
getOctetBoundary() const
{
  return 2;
}

Integer64 hash() const
{
  return (Integer64(_valuePointer->first) << 8) | Integer64(_valuePointer->second);
}
)

// float, > 8 bit values, both endianess
IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat32BE, float,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getFloat32BE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setFloat32BE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 4);
  if (bufferSize < index + 4)
    throw EncoderException("Insufficient buffer size for decoding!");
  union {
    uint32_t u;
    float s;
  } u;
  u.u = uint32_t(uint8_t(buffer[index])) << 24;
  u.u |= uint32_t(uint8_t(buffer[index + 1])) << 16;
  u.u |= uint32_t(uint8_t(buffer[index + 2])) << 8;
  u.u |= uint32_t(uint8_t(buffer[index + 3]));
  *_valuePointer = u.s;
  return index + 4;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 4);
  union {
    uint32_t u;
    float s;
  } u;
  u.s = *_valuePointer;
  buffer.push_back(uint8_t(u.u >> 24));
  buffer.push_back(uint8_t(u.u >> 16));
  buffer.push_back(uint8_t(u.u >> 8));
  buffer.push_back(uint8_t(u.u));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 4);
  union {
    uint32_t u;
    float s;
  } u;
  u.s = *_valuePointer;
  return encodeIntoBE32(buffer, bufferSize, offset, u.u);
}


size_t
getEncodedLength() const
{
  return 4;
}

unsigned int
getOctetBoundary() const
{
  return 4;
}

Integer64 hash() const
{
  union {
    uint32_t u;
    float s;
  } u;
  u.s = *_valuePointer;
  return Integer64(u.u);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat32LE, float,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getFloat32LE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setFloat32LE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 4);
  if (bufferSize < index + 4)
    throw EncoderException("Insufficient buffer size for decoding!");
  union {
    uint32_t u;
    float s;
  } u;
  u.u = uint32_t(uint8_t(buffer[index]));
  u.u |= uint32_t(uint8_t(buffer[index + 1])) << 8;
  u.u |= uint32_t(uint8_t(buffer[index + 2])) << 16;
  u.u |= uint32_t(uint8_t(buffer[index + 3])) << 24;
  *_valuePointer = u.s;
  return index + 4;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 4);
  union {
    uint32_t u;
    float s;
  } u;
  u.s = *_valuePointer;
  buffer.push_back(uint8_t(u.u));
  buffer.push_back(uint8_t(u.u >> 8));
  buffer.push_back(uint8_t(u.u >> 16));
  buffer.push_back(uint8_t(u.u >> 24));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 4);
  union {
    uint32_t u;
    float s;
  } u;
  u.s = *_valuePointer;
  return encodeIntoLE32(buffer, bufferSize, offset, u.u);
}

size_t
getEncodedLength() const
{
  return 4;
}

unsigned int
getOctetBoundary() const
{
  return 4;
}

Integer64 hash() const
{
  union {
    uint32_t u;
    float s;
  } u;
  u.s = *_valuePointer;
  return Integer64(u.u);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat64BE, double,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getFloat64BE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setFloat64BE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 8);
  if (bufferSize < index + 8)
    throw EncoderException("Insufficient buffer size for decoding!");
  union {
    uint64_t u;
    double s;
  } u;
  u.u = uint64_t(uint8_t(buffer[index])) << 56;
  u.u |= uint64_t(uint8_t(buffer[index + 1])) << 48;
  u.u |= uint64_t(uint8_t(buffer[index + 2])) << 40;
  u.u |= uint64_t(uint8_t(buffer[index + 3])) << 32;
  u.u |= uint64_t(uint8_t(buffer[index + 4])) << 24;
  u.u |= uint64_t(uint8_t(buffer[index + 5])) << 16;
  u.u |= uint64_t(uint8_t(buffer[index + 6])) << 8;
  u.u |= uint64_t(uint8_t(buffer[index + 7]));
  *_valuePointer = u.s;
  return index + 8;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 8);
  union {
    uint64_t u;
    double s;
  } u;
  u.s = *_valuePointer;
  buffer.push_back(uint8_t(u.u >> 56));
  buffer.push_back(uint8_t(u.u >> 48));
  buffer.push_back(uint8_t(u.u >> 40));
  buffer.push_back(uint8_t(u.u >> 32));
  buffer.push_back(uint8_t(u.u >> 24));
  buffer.push_back(uint8_t(u.u >> 16));
  buffer.push_back(uint8_t(u.u >> 8));
  buffer.push_back(uint8_t(u.u));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 8);
  union {
    uint64_t u;
    double s;
  } u;
  u.s = *_valuePointer;
  return encodeIntoBE64(buffer, bufferSize, offset, u.u);
}


size_t
getEncodedLength() const
{
  return 8;
}

unsigned int
getOctetBoundary() const
{
  return 8;
}

Integer64 hash() const
{
  union {
    uint64_t u;
    double s;
  } u;
  u.s = *_valuePointer;
  return Integer64(u.u);
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat64LE, double,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() != getEncodedLength())
    throw EncoderException("Insufficient buffer size for decoding!");
  *_valuePointer = inData.getFloat64LE(0);
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  inData.resize(getEncodedLength());
  inData.setFloat64LE(*_valuePointer, 0);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 8);
  if (bufferSize < index + 8)
    throw EncoderException("Insufficient buffer size for decoding!");
  union {
    uint64_t u;
    double s;
  } u;
  u.u = uint64_t(uint8_t(buffer[index]));
  u.u |= uint64_t(uint8_t(buffer[index + 1])) << 8;
  u.u |= uint64_t(uint8_t(buffer[index + 2])) << 16;
  u.u |= uint64_t(uint8_t(buffer[index + 3])) << 24;
  u.u |= uint64_t(uint8_t(buffer[index + 4])) << 32;
  u.u |= uint64_t(uint8_t(buffer[index + 5])) << 40;
  u.u |= uint64_t(uint8_t(buffer[index + 6])) << 48;
  u.u |= uint64_t(uint8_t(buffer[index + 7])) << 56;
  *_valuePointer = u.s;
  return index + 8;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 8);
  union {
    uint64_t u;
    double s;
  } u;
  u.s = *_valuePointer;
  buffer.push_back(uint8_t(u.u));
  buffer.push_back(uint8_t(u.u >> 8));
  buffer.push_back(uint8_t(u.u >> 16));
  buffer.push_back(uint8_t(u.u >> 24));
  buffer.push_back(uint8_t(u.u >> 32));
  buffer.push_back(uint8_t(u.u >> 40));
  buffer.push_back(uint8_t(u.u >> 48));
  buffer.push_back(uint8_t(u.u >> 56));
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 8);
  union {
    uint64_t u;
    double s;
  } u;
  u.s = *_valuePointer;
  return encodeIntoLE64(buffer, bufferSize, offset, u.u);
}

size_t
getEncodedLength() const
{
  return 8;
}

unsigned int
getOctetBoundary() const
{
  return 8;
}

Integer64 hash() const
{
  union {
    uint64_t u;
    double s;
  } u;
  u.s = *_valuePointer;
  return Integer64(u.u);
}
)

// Array values
IMPLEMENT_ENCODING_HELPER_CLASS(HLAASCIIstring, std::string,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() < 4)
    throw EncoderException("Insufficient buffer size for decoding!");

  Integer32 length = inData.getInt32BE(0);
  if (length < 0)
    length = 0;

  if (inData.size() < static_cast<size_t>(4 + length))
    throw EncoderException("Insufficient buffer size for decoding!");
  _valuePointer->clear();
  _valuePointer->reserve(length);
  for (Integer32 i = 0; i < length; ++i)
    _valuePointer->push_back(inData.getChar(4 + i));
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  size_t length = _valuePointer->size();
  if (std::numeric_limits<Integer32>::max() < length)
    length = std::numeric_limits<Integer32>::max();
  inData.resize(length + 4);
  inData.setInt32BE(static_cast<int32_t>(length), 0);

  for (size_t i = 0; i < length; ++i)
    inData.setChar((*_valuePointer)[i], 4 + i);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 4);
  if (bufferSize < index + 4)
    throw EncoderException("Insufficient buffer size for decoding!");
  uint32_t u;
  u = uint32_t(uint8_t(buffer[index])) << 24;
  u |= uint32_t(uint8_t(buffer[index + 1])) << 16;
  u |= uint32_t(uint8_t(buffer[index + 2])) << 8;
  u |= uint32_t(uint8_t(buffer[index + 3]));
  Integer32 length = Integer32(u);
  if (length < 0)
    length = 0;
  index += 4;

  if (bufferSize < index + length)
    throw EncoderException("Insufficient buffer size for decoding!");
  _valuePointer->clear();
  _valuePointer->reserve(length);
  while (0 < length--)
    _valuePointer->push_back(buffer[index++]);

  return index;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 4);
  size_t length = _valuePointer->size();
  if (std::numeric_limits<Integer32>::max() < length)
    length = std::numeric_limits<Integer32>::max();
  int32_t u = static_cast<int32_t>(length);
  buffer.push_back(uint8_t(u >> 24));
  buffer.push_back(uint8_t(u >> 16));
  buffer.push_back(uint8_t(u >> 8));
  buffer.push_back(uint8_t(u));

  for (size_t i = 0; i < length; ++i)
    buffer.push_back((*_valuePointer)[i]);
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 4);
  size_t length = _valuePointer->size();
  if (std::numeric_limits<Integer32>::max() < length)
    length = std::numeric_limits<Integer32>::max();
  //int32_t u = static_cast<int32_t>(length);
  offset = encodeIntoBE32(buffer, bufferSize, offset, static_cast<int32_t>(length));
  memcpy(buffer + offset, _valuePointer->data(), static_cast<int32_t>(length));
  return offset + length;
}

size_t
getEncodedLength() const
{
  return 4 + _valuePointer->size();
}

unsigned int
getOctetBoundary() const
{
  return 4;
}

Integer64 hash() const
{
  Integer64 hash = 8191;
  for (std::string::const_iterator i = _valuePointer->begin(); i != _valuePointer->end(); ++i)
    hash = hash*Integer64(127) + Integer64(*i);
  return hash;
}
)
IMPLEMENT_ENCODING_HELPER_CLASS(HLAunicodeString, std::wstring,
void
decode(OpenRTI::VariableLengthData const & inData)
{
  if (inData.size() < 4)
    throw EncoderException("Insufficient buffer size for decoding!");

  Integer32 length = inData.getInt32BE(0);
  if (length < 0)
    length = 0;

  if (inData.size() < 4 + 2*length)
    throw EncoderException("Insufficient buffer size for decoding!");
  _valuePointer->clear();
  _valuePointer->reserve(length);
  for (Integer32 i = 0; i < length; ++i)
    _valuePointer->push_back(inData.getUInt16BE(4 + 2*i));
}

void
encode(OpenRTI::VariableLengthData& inData) const
{
  size_t length = _valuePointer->size();
  if (std::numeric_limits<Integer32>::max() < length)
    length = std::numeric_limits<Integer32>::max();
  inData.resize(2*length + 4);
  inData.setInt32BE(static_cast<int32_t>(length), 0);

  for (size_t i = 0; i < length; ++i)
    inData.setUInt16BE((*_valuePointer)[i], 4 + 2*i);
}

size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  index = align(index, 4);
  if (bufferSize < index + 4)
    throw EncoderException("Insufficient buffer size for decoding!");
  uint32_t encodedLength;
  encodedLength = uint32_t(uint8_t(buffer[index])) << 24;
  encodedLength |= uint32_t(uint8_t(buffer[index + 1])) << 16;
  encodedLength |= uint32_t(uint8_t(buffer[index + 2])) << 8;
  encodedLength |= uint32_t(uint8_t(buffer[index + 3]));
  Integer32 length = Integer32(encodedLength);
  if (length < 0)
    length = 0;
  index += 4;

  if (bufferSize < index + 2*length)
    throw EncoderException("Insufficient buffer size for decoding!");
  _valuePointer->clear();
  _valuePointer->reserve(length);
  while (0 < length--) {
    uint16_t encodedChar;
    encodedChar = uint16_t(uint8_t(buffer[index])) << 8;
    encodedChar |= uint16_t(uint8_t(buffer[index + 1]));
    _valuePointer->push_back(wchar_t(encodedChar));
    index += 2;
  }

  return index;
}

void encodeInto(std::vector<Octet>& buffer) const
{
  align(buffer, 4);
  size_t length = _valuePointer->size();
  if (std::numeric_limits<Integer32>::max() < length)
    length = std::numeric_limits<Integer32>::max();
  buffer.push_back(uint8_t(length >> 24));
  buffer.push_back(uint8_t(length >> 16));
  buffer.push_back(uint8_t(length >> 8));
  buffer.push_back(uint8_t(length));

  for (size_t i = 0; i < length; ++i) {
    uint16_t unicodeChar = uint16_t((*_valuePointer)[i]);
    buffer.push_back(uint8_t(unicodeChar >> 8));
    buffer.push_back(uint8_t(unicodeChar));
  }
}

size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  offset = align(offset, 4);
  size_t length = _valuePointer->size();
  if (std::numeric_limits<Integer32>::max() < length)
    length = std::numeric_limits<Integer32>::max();
  offset = encodeIntoBE32(buffer, bufferSize, offset, static_cast<uint32_t>(length));
  for (size_t i = 0; i < length; ++i) {
    uint16_t unicodeChar = uint16_t((*_valuePointer)[i]);
    offset = encodeIntoBE16(buffer, bufferSize, offset, unicodeChar);
    //buffer.push_back(uint8_t(unicodeChar >> 8));
    //buffer.push_back(uint8_t(unicodeChar));
  }
  return offset;
}

size_t
getEncodedLength() const
{
  return 4 + 2*_valuePointer->size();
}

unsigned int
getOctetBoundary() const
{
  return 4;
}

Integer64 hash() const
{
  Integer64 hash = 8191;
  for (std::wstring::const_iterator i = _valuePointer->begin(); i != _valuePointer->end(); ++i)
    hash = hash*Integer64(127) + Integer64(*i);
  return hash;
}
)

}
