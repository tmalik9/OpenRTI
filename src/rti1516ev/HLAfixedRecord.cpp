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
#include <RTI/encoding/HLAfixedRecord.h>

#include <RTI/VariableLengthData.h>
#include <RTI/encoding/DataElement.h>

#include <cstring>
#include <vector>
#include <sstream>
#include "Encoding.h"
#include "Export.h"

#define COMPRESS_HEADER

namespace rti1516ev
{

typedef std::vector<std::pair<DataElement*, bool>> DataElementVector;

class OPENRTI_LOCAL HLAfixedRecordImplementation : public HLAencodingImplementationBase {
public:
  HLAfixedRecordImplementation(uint32_t version)
    : _version(version)
  {
  }
  HLAfixedRecordImplementation(const HLAfixedRecordImplementation& rhs)
    : _version(rhs._version)
  {
    _dataElementVector.reserve(rhs._dataElementVector.size());
    for (auto& element : rhs._dataElementVector)
      _dataElementVector.push_back(std::make_pair(element.first->clone().release(), true));
    _dataElementVersions = rhs._dataElementVersions;
  }
  ~HLAfixedRecordImplementation() noexcept
  {
    for (auto& it_dataElement_owner : _dataElementVector) {
      if (it_dataElement_owner.second) delete it_dataElement_owner.first;
      it_dataElement_owner.first = nullptr;
    }
  }

  size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const override
  {
#ifdef _DEBUG
    if (bufferSize < offset + getEncodedLength())
      throw EncoderException(L"buffer to small: bufferSize=" + std::to_wstring(bufferSize) + L" offset=" + std::to_wstring(offset) + L" encodedLength=" + std::to_wstring(getEncodedLength()));
#endif
    uint32_t byteLength = static_cast<uint32_t>(getEncodedLength());
#ifdef COMPRESS_HEADER
    offset = encodeIntoBE32Compressed(buffer, bufferSize, offset, static_cast<uint32_t>(byteLength));
    offset = encodeIntoBE32Compressed(buffer, bufferSize, offset, static_cast<uint32_t>(_version));
#else
    offset = encodeIntoBE32(buffer, bufferSize, offset, static_cast<uint32_t>(byteLength));
    offset = encodeIntoBE32(buffer, bufferSize, offset, static_cast<uint32_t>(_version));
#endif
    for (DataElementVector::const_iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
      offset = i->first->encodeInto(buffer, bufferSize, offset);
    }
    return offset;
  }

  size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t startIndex) override
  {
    uint32_t byteLength;
    size_t index = startIndex;
    try {
#ifdef COMPRESS_HEADER
      index = decodeFromBE32Compressed(buffer, bufferSize, startIndex, byteLength);
#else
      index = decodeFromBE32(buffer, bufferSize, startIndex, byteLength);
#endif
    }
    catch (const EncoderException& e)
    {
      std::wostringstream msg;
      msg << e.what() + L" while decoding fixed record byte length";
      throw EncoderException(msg.str());
    }
    try {
#ifdef COMPRESS_HEADER
      index = decodeFromBE32Compressed(buffer, bufferSize, index, _version);
#else
      index = decodeFromBE32(buffer, bufferSize, index, _version);
#endif
    }
    catch (const EncoderException& e)
    {
      std::wostringstream msg;
      msg << e.what() + L" while decoding fixed record version";
      throw EncoderException(msg.str());
    }
    size_t end = startIndex + byteLength;
    int count=0;
    // NOTE: we decode only as far as the byteLength tells us to do, AND only for the data elements we know of.
    // The first rule catches the case that the received record's version is smaller than that we know of, and contains fewer fields.
    // The second rule lets us skip all fields we don't know of, because the record's version is higher than what we know.
    for (DataElementVector::iterator i = _dataElementVector.begin(); i != _dataElementVector.end() && index < end; ++i, count++) {
      try {
        index = i->first->decodeFrom(buffer, bufferSize, index);
      }
      catch (const EncoderException& e)
      {
        std::wostringstream msg;
        msg << e.what() + L" while decoding fixed record field #" << count;
        throw EncoderException(msg.str());
      }
    }
    return index;
  }

  HLAencodingImplementationBase* clone() const override
  {
    return new HLAfixedRecordImplementation(*this);
  }

  size_t getEncodedLength() const
  {
    // first get the bytes used by the record fields
    size_t length = 0;
    for (DataElementVector::const_iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
      length += i->first->getEncodedLength();
    }
    // get the bytes used by the length and version fields
#ifdef COMPRESS_HEADER
    uint8_t buffer[10];
    size_t offset = 0;
    offset = encodeIntoBE32Compressed(buffer, sizeof(buffer), offset, static_cast<uint32_t>(length));
    offset = encodeIntoBE32Compressed(buffer, sizeof(buffer), offset, _version);
    return offset + length;
#else
    return 2 * sizeof(uint32_t) + length;
#endif
  }

  unsigned int getOctetBoundary()
  {
    if (_dataElementVector.empty())
      return 1;
    return _dataElementVector.front().first->getOctetBoundary();
  }

  bool isSameTypeAs(const HLAfixedRecordImplementation& rhs) const
  {
    if (_dataElementVector.size() != rhs._dataElementVector.size())
      return false;
    for (DataElementVector::const_iterator i = _dataElementVector.begin(), j = rhs._dataElementVector.begin();
         i != _dataElementVector.end(); ++i, ++j) {
      if (!i->first->isSameTypeAs(*(j->first)))
        return false;
    }
    return true;
  }

  bool hasElementSameTypeAs(size_t index, DataElement const& inData) const
  {
    if (_dataElementVector.size() <= index)
      return false;
    return inData.isSameTypeAs(*(_dataElementVector[index].first));
  }

  size_t size() const
  { return _dataElementVector.size(); }

  void appendElement(const DataElement& dataElement, uint32_t fieldVersion = 0)
  {
    _dataElementVector.push_back(std::make_pair(dataElement.clone().release(), true));
    _dataElementVersions.push_back(fieldVersion);
  }

  void appendElementPointer(DataElement* dataElement, uint32_t fieldVersion = 0)
  {
    if (!dataElement)
      throw EncoderException(L"HLAfixedRecord::appendElementPointer: Null pointer given!");
    _dataElementVector.push_back(std::make_pair(dataElement, false));
    _dataElementVersions.push_back(fieldVersion);
}

  void set(size_t index, const DataElement& dataElement)
  {
    if (_dataElementVector.size() <= index)
      throw EncoderException(L"HLAfixedRecord::setElement(): Index out of range!");
    auto& element = _dataElementVector[index];
    if (!element.first->isSameTypeAs(dataElement))
      throw EncoderException(L"HLAfixedRecord::setElement(): Incompatible dataElements!");
    if (element.second) delete element.first;
    element.first = dataElement.clone().release();
    element.second = true;
  }

  void setElementPointer(size_t index, DataElement* dataElement)
  {
    if (!dataElement)
      throw EncoderException(L"HLAfixedRecord::setElementPointer(): Null pointer given!");
    if (_dataElementVector.size() <= index)
      throw EncoderException(L"HLAfixedRecord::setElementPointer(): Index out of range!");
    auto& element = _dataElementVector[index];
    if (!element.first->isSameTypeAs(*dataElement))
      throw EncoderException(L"HLAfixedRecord::setElementPointer(): Incompatible dataElements!");
    if (element.second) delete element.first;
    element.first = dataElement;
    element.second = false;
  }

  const DataElement& get(size_t index) const
  {
    if (_dataElementVector.size() <= index)
      throw EncoderException(L"HLAfixedRecord::get(size_t): Index out of range: size=" + std::to_wstring(_dataElementVector.size()) + L" <= index=" + std::to_wstring(index));
    if (_version < _dataElementVersions[index])
      throw EncoderException(L"HLAfixedRecord::get(size_t): Field not available in given version: record version=" + std::to_wstring(_version) + L" < field version=" + std::to_wstring(_dataElementVersions[index]));
    return *(_dataElementVector[index].first);
  }

  uint32_t getVersion() const { return _version; }

  DataElementVector _dataElementVector;
  std::vector<uint32_t> _dataElementVersions;
  uint32_t _version;
};

HLAfixedRecord::HLAfixedRecord(uint32_t version)
 : _impl(new HLAfixedRecordImplementation(version))
{
}

HLAfixedRecord::HLAfixedRecord(HLAfixedRecord const & rhs) :
  _impl(new HLAfixedRecordImplementation(*rhs._impl))
{
}

HLAfixedRecord::~HLAfixedRecord() noexcept
{
  delete _impl;
  _impl = 0;
}

std::unique_ptr<DataElement>
HLAfixedRecord::clone() const
{
  return std::unique_ptr<DataElement>(new HLAfixedRecord(*this));
}

VariableLengthData
HLAfixedRecord::encode() const
{
  VariableLengthData variableLengthData;
  encode(variableLengthData);
  return variableLengthData;
}

void
HLAfixedRecord::encode(VariableLengthData& outData) const
{
  outData.resize(getEncodedLength());
  _impl->encodeInto(static_cast<Octet*>(outData.data()), outData.size(), 0);
}

void
HLAfixedRecord::encodeInto(std::vector<Octet>& buffer) const
{
  size_t offset = buffer.size();
  size_t encodedLength = getEncodedLength();
  buffer.resize(offset + encodedLength);
  _impl->encodeInto(static_cast<Octet*>(buffer.data()) + offset, encodedLength, offset);
}

size_t HLAfixedRecord::encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  return _impl->encodeInto(buffer, bufferSize, offset);
}

void
HLAfixedRecord::decode(VariableLengthData const& inData)
{
  _impl->decodeFrom(static_cast<const Octet*>(inData.data()), inData.size(), 0);
}

size_t
HLAfixedRecord::decodeFrom(std::vector<Octet> const& buffer, size_t index)
{
  return _impl->decodeFrom(buffer.data(), buffer.size(), index);
}


size_t HLAfixedRecord::decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  return _impl->decodeFrom(buffer, bufferSize, index);
}

size_t
HLAfixedRecord::getEncodedLength() const
{
  return _impl->getEncodedLength();
}

unsigned int
HLAfixedRecord::getOctetBoundary() const
{
  return _impl->getOctetBoundary();
}

bool
HLAfixedRecord::isSameTypeAs(DataElement const& inData ) const
{
  //if (!DataElement::isSameTypeAs(inData))
  //  return false;
  const HLAfixedRecord* fixedRecord = dynamic_cast<const HLAfixedRecord*>(&inData);
  if (!fixedRecord)
    return false;
  return _impl->isSameTypeAs(*fixedRecord->_impl);
}

bool
HLAfixedRecord::hasElementSameTypeAs(size_t index, DataElement const& inData) const
{
  return _impl->hasElementSameTypeAs(index, inData);
}

size_t
HLAfixedRecord::size() const
{
  return _impl->size();
}

void
HLAfixedRecord::appendElement(const DataElement& dataElement, uint32_t fieldVersion)
{
  _impl->appendElement(dataElement, fieldVersion);
}

void
HLAfixedRecord::appendElementPointer(DataElement* dataElement, uint32_t fieldVersion)
{
  _impl->appendElementPointer(dataElement, fieldVersion);
}

void
HLAfixedRecord::set(size_t index, const DataElement& dataElement)
{
  _impl->set(index, dataElement);
}

void
HLAfixedRecord::setElementPointer(size_t index, DataElement* dataElement)
{
  _impl->setElementPointer(index, dataElement);
}

const DataElement&
HLAfixedRecord::get(size_t index) const
{
  return _impl->get(index);
}

uint32_t HLAfixedRecord::getVersion() const
{
  return _impl->getVersion();
}

DataElement const&
HLAfixedRecord::operator[](size_t index) const
{
  return _impl->get(index);
}

}
