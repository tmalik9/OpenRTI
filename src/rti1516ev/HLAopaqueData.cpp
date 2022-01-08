/* -*-c++-*- OpenRTI - Copyright (C) 2011-2015 Mathias Froehlich
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
#include <RTI/encoding/HLAopaqueData.h>

#include <RTI/VariableLengthData.h>
#include <RTI/encoding/DataElement.h>

#include <cstring>
#include <vector>

#include "Encoding.h"
#include "Export.h"

namespace rti1516ev
{

class OPENRTI_LOCAL HLAopaqueDataImplementation : public HLAencodingImplementationBase {
public:
  using ImplType = std::vector<uint8_t>;

  HLAopaqueDataImplementation()
  {
  }
  HLAopaqueDataImplementation(const Octet* inData, size_t dataSize) :
    _buffer(inData, inData + dataSize)
  {
  }
  HLAopaqueDataImplementation(const std::vector<Octet>& inData) :
    _buffer(inData)
  {
  }
  HLAopaqueDataImplementation(Octet** inData, size_t bufferSize, size_t dataSize)
  {
    // FIXME no clue if this is right!!
    if (!dataSize) {
      _buffer.clear();
    } else {
      set(*inData, dataSize);
    }
    if (*inData)
      delete *inData;
    inData = 0;
  }
  HLAopaqueDataImplementation(const HLAopaqueDataImplementation& rhs) :
    _buffer(rhs._buffer)
  {
  }

  ~HLAopaqueDataImplementation() noexcept
  {
  }

  size_t decodedSize(const Octet* buffer, size_t bufferSize, size_t index)
  {
    if (bufferSize < index + 4)
      throw EncoderException(L"Insufficient buffer size for decoding!");
    uint32_t length;
    decodeFromBE32(buffer, bufferSize, index, length);
    return length;
  }

  size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const override
  {
#ifdef _DEBUG
    if (bufferSize < offset + getEncodedLength())
      throw EncoderException(L"buffer to small: bufferSize=" + std::to_wstring(bufferSize) + L" offset=" + std::to_wstring(offset) + L" encodedLength=" + std::to_wstring(getEncodedLength()));
#endif
    size_t length = _buffer.size();
    if (0xffffffffu < length)
      throw EncoderException(L"HLAopaqueData::encodeInto(): array size is too big to encode!");
    offset = encodeIntoBE32(buffer, bufferSize, offset, static_cast<uint32_t>(length));
    memcpy(buffer + offset, _buffer.data(), _buffer.size());
    return offset + _buffer.size();
  }

  size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index) override
  {
    if (bufferSize < index + 4u)
      throw EncoderException(L"Insufficient buffer size for decoding!");
    uint32_t length;
    index = decodeFromBE32(buffer, bufferSize, index, length);
    _buffer.resize(length);
    memcpy(_buffer.data(), buffer + index, length);
    return index + length;
  }

  HLAencodingImplementationBase* clone() const override
  {
    return new HLAopaqueDataImplementation(*this);
  }

  size_t getEncodedLength() const
  {
    return 4u + _buffer.size();
  }

  size_t size() const
  {
    return _buffer.size();
  }

  void setDataPointer(Octet** inData, size_t /*bufferSize*/, size_t dataSize)
  {
    // FIXME no clue if this is right!!
    if (!dataSize) {
      _buffer.clear();
    } else {
      set(*inData, dataSize);
    }
    //if (*inData)
    //  delete *inData;
    //inData = 0;
  }

  void set(const std::vector<uint8_t>& newValue)
  {
    _buffer = newValue;
  }

  void set(const Octet* inData, size_t dataSize)
  {
    _buffer.reserve(dataSize);
    if (!dataSize)
      return;
    _buffer.assign(inData, inData + dataSize);
  }

  const std::vector<Octet>& get() const
  {
    return _buffer;
  }

  std::vector<Octet> _buffer;
};

HLAopaqueData::HLAopaqueData() :
  _impl(new HLAopaqueDataImplementation())
{
}

HLAopaqueData::HLAopaqueData(const Octet* inData, size_t dataSize)
  : _impl(new HLAopaqueDataImplementation(inData, dataSize))
{
}

HLAopaqueData::HLAopaqueData(const std::vector<Octet>& inData)
  : _impl(new HLAopaqueDataImplementation(inData))
{
}

HLAopaqueData::HLAopaqueData(Octet** inData, size_t bufferSize, size_t dataSize) :
  _impl(new HLAopaqueDataImplementation(inData, bufferSize, dataSize))
{
}

HLAopaqueData::HLAopaqueData(HLAopaqueData const & rhs) :
  _impl(new HLAopaqueDataImplementation(*rhs._impl))
{
}

HLAopaqueData::~HLAopaqueData() noexcept
{
  delete _impl;
  _impl = 0;
}

std::unique_ptr<DataElement>
HLAopaqueData::clone () const
{
  return std::unique_ptr<DataElement>(new HLAopaqueData(*this));
}

VariableLengthData
HLAopaqueData::encode () const
{
  VariableLengthData variableLengthData;
  encode(variableLengthData);
  return variableLengthData;
}

void
HLAopaqueData::encode(VariableLengthData& outData) const
{
  outData.resize(getEncodedLength());
  _impl->encodeInto(static_cast<Octet*>(outData.data()), outData.size(), 0);
}

void
HLAopaqueData::encodeInto(std::vector<Octet>& buffer) const
{
  size_t offset = buffer.size();
  size_t encodedLength = getEncodedLength();
  buffer.resize(offset + encodedLength);
  _impl->encodeInto(static_cast<Octet*>(buffer.data()) + offset, encodedLength, offset);
}


size_t HLAopaqueData::encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  return _impl->encodeInto(buffer, bufferSize, offset);
}

void HLAopaqueData::decode(VariableLengthData const & inData)
{
  _impl->decodeFrom(static_cast<const Octet*>(inData.data()), inData.size(), 0);
}

size_t
HLAopaqueData::decodeFrom(std::vector<Octet> const & buffer, size_t index)
{
  return _impl->decodeFrom(buffer.data(), buffer.size(), index);
}


size_t HLAopaqueData::decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  return _impl->decodeFrom(buffer, bufferSize, index);
}

size_t
HLAopaqueData::getEncodedLength() const
{
  return _impl->getEncodedLength();
}

unsigned int
HLAopaqueData::getOctetBoundary() const
{
  return 1;
}

size_t
HLAopaqueData::bufferLength() const
{
  return _impl->_buffer.size();
}

size_t
HLAopaqueData::dataLength() const
{
  return _impl->_buffer.size();
}

void
HLAopaqueData::setDataPointer(Octet** inData, size_t bufferSize, size_t dataSize)
{
  _impl->setDataPointer(inData, bufferSize, dataSize);
}

void
HLAopaqueData::set(const Octet* inData, size_t dataSize)
{
  _impl->set(inData, dataSize);
}

void
HLAopaqueData::set(const std::vector<Octet>& inData)
{
  _impl->set(inData);
}

const std::vector<Octet>&
HLAopaqueData::get() const
{
  return _impl->get();
}

HLAopaqueData::operator const Octet*() const
{
  if (_impl->size() > 0)
  {
    return _impl->get().data();
  }
  else
  {
    return nullptr;
  }
}

}
