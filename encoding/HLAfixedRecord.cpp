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
#include "HLAfixedRecord.h"

#include "VariableLengthData.h"
#include "DataElement.h"

#include <cstring>
#include <vector>

#include "Encoding.h"
#include "Export.h"

namespace OpenRTI
{

typedef std::vector<std::pair<DataElement*, bool>> DataElementVector;

class OPENRTI_LOCAL HLAfixedRecordImplementation {
public:
  HLAfixedRecordImplementation() :
    _octetBoundary(0)
  {
  }
  HLAfixedRecordImplementation(const HLAfixedRecordImplementation& rhs) :
    _octetBoundary(0)
  {
    try {
    _dataElementVector.reserve(rhs._dataElementVector.size());
    for (auto& element : rhs._dataElementVector)
      _dataElementVector.push_back(std::make_pair(element.first->clone().release(), true));
    }
    catch(...)
    {
      DebugPrintf("%s failed\n", __FUNCTION__);
    }
  }
  ~HLAfixedRecordImplementation()
  {
    for (auto& it_dataElement_owner : _dataElementVector) {
      if (it_dataElement_owner.second) delete it_dataElement_owner.first;
      it_dataElement_owner.first = nullptr;
    }
  }

  size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset)
  {
#ifdef _DEBUG
    if (bufferSize < offset + getEncodedLength())
      throw EncoderException("buffer to small: bufferSize=" + std::to_string(bufferSize) + " offset=" + std::to_string(offset) + " encodedLength=" + std::to_string(getEncodedLength()));
#endif
    for (DataElementVector::const_iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
      offset = i->first->encodeInto(buffer, bufferSize, offset);
    }
    return offset;
  }

  size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
  {
    for (DataElementVector::iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
      index = i->first->decodeFrom(buffer, bufferSize, index);
    }
    return index;
  }

  size_t getEncodedLength() const
  {
    size_t length = 0;
    for (DataElementVector::const_iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
      length += i->first->getEncodedLength();
    }
    return length;
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

  void appendElement(const DataElement& dataElement)
  {
    _dataElementVector.push_back(std::make_pair(dataElement.clone().release(), true));
    _octetBoundary = 0;
  }

  void appendElementPointer(DataElement* dataElement)
  {
    if (!dataElement)
      throw EncoderException("HLAfixedRecord::appendElementPointer: Null pointer given!");
    _dataElementVector.push_back(std::make_pair(dataElement, false));
    _octetBoundary = 0;
  }

  void set(size_t index, const DataElement& dataElement)
  {
    if (_dataElementVector.size() <= index)
      throw EncoderException("HLAfixedRecord::setElement(): Index out of range!");
    auto& element = _dataElementVector[index];
    if (!element.first->isSameTypeAs(dataElement))
      throw EncoderException("HLAfixedRecord::setElement(): Incompatible dataElements!");
    if (element.second) delete element.first;
    element.first = dataElement.clone().release();
    element.second = true;
  }

  void setElementPointer(size_t index, DataElement* dataElement)
  {
    if (!dataElement)
      throw EncoderException("HLAfixedRecord::setElementPointer(): Null pointer given!");
    if (_dataElementVector.size() <= index)
      throw EncoderException("HLAfixedRecord::setElementPointer(): Index out of range!");
    auto& element = _dataElementVector[index];
    if (!element.first->isSameTypeAs(*dataElement))
      throw EncoderException("HLAfixedRecord::setElementPointer(): Incompatible dataElements!");
    if (element.second) delete element.first;
    element.first = dataElement;
    element.second = false;
  }

  const DataElement& get(size_t index) const
  {
    if (_dataElementVector.size() <= index)
      throw EncoderException("HLAfixedRecord::get(size_t): Index out of range!");
    return *(_dataElementVector[index].first);
  }

  const DataElement& arrayget(size_t index) const
  {
    if (_dataElementVector.size() <= index)
      throw EncoderException("HLAfixedRecord::operator[](size_t): Index out of range!");
    return *(_dataElementVector[index].first);
  }

  DataElementVector _dataElementVector;
  unsigned _octetBoundary;
};

HLAfixedRecord::HLAfixedRecord() :
  _impl(new HLAfixedRecordImplementation)
{
}

HLAfixedRecord::HLAfixedRecord(HLAfixedRecord const & rhs) :
  _impl(new HLAfixedRecordImplementation(*rhs._impl))
{
}

HLAfixedRecord::~HLAfixedRecord()
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
HLAfixedRecord::appendElement(const DataElement& dataElement)
{
  _impl->appendElement(dataElement);
}

void
HLAfixedRecord::appendElementPointer(DataElement* dataElement)
{
  _impl->appendElementPointer(dataElement);
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

DataElement const&
HLAfixedRecord::operator[](size_t index) const
{
  return _impl->arrayget(index);
}

}
