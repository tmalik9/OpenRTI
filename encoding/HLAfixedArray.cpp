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

#include "HLAfixedArray.h"

#include "VariableLengthData.h"
#include "DataElement.h"

#include <cstring>
#include <vector>

#include "Encoding.h"
#include "Export.h"

namespace OpenRTI
{

class OPENRTI_LOCAL HLAfixedArrayImplementation {
public:
  HLAfixedArrayImplementation(const DataElement& protoType, size_t length)
  {
    _dataElementVector.resize(length, 0);
    for (DataElementVector::iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i)
      (*i) = protoType.clone().release();
  }
  HLAfixedArrayImplementation(const HLAfixedArrayImplementation& rhs)
  {
    _dataElementVector.resize(rhs._dataElementVector.size(), 0);
    for (size_t i = 0; i < rhs._dataElementVector.size(); ++i) {
      if (!rhs._dataElementVector[i])
        continue;
      _dataElementVector[i] = rhs._dataElementVector[i]->clone().release();
    }
  }
  virtual ~HLAfixedArrayImplementation()
  {
    for (DataElementVector::iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
      delete *i;
      *i = NULL;
    }
  }

  size_t getEncodedLength() const
  {
    if (_dataElementVector.empty())
      return 0;
    if (!_dataElementVector.front())
      return 0;
    return _dataElementVector.size()*_dataElementVector.front()->getEncodedLength();
  }

  void encodeInto(std::vector<Octet>& buffer) const
  {
    for (DataElementVector::const_iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
      if (!*i)
        throw EncoderException("HLAfixedArray::encodeInto(): dataElement is zero!");
      (*i)->encodeInto(buffer);
    }
  }

  size_t decodeFrom(std::vector<Octet> const & buffer, size_t index)
  {
    for (DataElementVector::const_iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
      if (!*i)
        throw EncoderException("HLAfixedArray::decodeFrom(): dataElement is zero!");
      index = (*i)->decodeFrom(buffer, index);
    }
    return index;
  }

  unsigned int getOctetBoundary() const
  {
    if (_dataElementVector.empty())
      return 1;
    return _dataElementVector.front()->getOctetBoundary();
  }

  size_t size () const
  { return _dataElementVector.size(); }

  void set(size_t index, const DataElement& dataElement)
  {
    if (_dataElementVector.size() <= index)
      throw EncoderException("HLAfixedArray::set(): Index out of range!");
    if (!dataElement.isSameTypeAs(*_dataElementVector[index]))
      throw EncoderException("HLAfixedArray::set(): Data type is not compatible!");
    delete _dataElementVector[index];
    _dataElementVector[index] = dataElement.clone().release();
  }

  void setElementPointer(size_t index, DataElement* dataElement)
  {
    if (_dataElementVector.size() <= index)
      throw EncoderException("HLAfixedArray::setElementPointer(): Index out of range!");
    if (!dataElement)
      throw EncoderException("HLAfixedArray::setElementPointer(): dataElement is zero!");
    if (!dataElement->isSameTypeAs(*_dataElementVector[index]))
      throw EncoderException("HLAfixedArray::setElementPointer(): Data type is not compatible!");
    delete _dataElementVector[index];
    _dataElementVector[index] = dataElement;
  }

  const DataElement& get(size_t index) const
  {
    if (_dataElementVector.size() <= index)
      throw EncoderException("HLAfixedArray::get(): Index out of range!");
    if (!_dataElementVector[index])
      throw EncoderException("HLAfixedArray::get(): dataElement is zero!");
    return *_dataElementVector[index];
  }

  bool isSameTypeAs(const HLAfixedArrayImplementation& rhs) const
  {
    if (_dataElementVector.empty()) {
      return rhs._dataElementVector.empty();
    } else {
      if (rhs._dataElementVector.empty())
        return false;
      return _dataElementVector.front()->isSameTypeAs(*rhs._dataElementVector.front());
    }
  }

  typedef std::vector<DataElement*> DataElementVector;
  DataElementVector _dataElementVector;
};

HLAfixedArray::HLAfixedArray(const DataElement& protoType, size_t length) :
  _impl(new HLAfixedArrayImplementation(protoType, length))
{
}

HLAfixedArray::HLAfixedArray(HLAfixedArray const & rhs) :
  _impl(new HLAfixedArrayImplementation(*rhs._impl))
{
}

HLAfixedArray::~HLAfixedArray()
{
  delete _impl;
  _impl = 0;
}

std::unique_ptr<DataElement>
HLAfixedArray::clone () const
{
  return std::unique_ptr<OpenRTI::DataElement>(new HLAfixedArray(*this));
}

VariableLengthData
HLAfixedArray::encode () const
{
  VariableLengthData variableLengthData;
  encode(variableLengthData);
  return variableLengthData;
}

void
HLAfixedArray::encode(VariableLengthData& inData) const
{
  std::vector<Octet> buffer;
  buffer.reserve(getEncodedLength());
  encodeInto(buffer);
  if (!buffer.empty())
    inData.setData(&buffer.front(), buffer.size());
}

void
HLAfixedArray::encodeInto(std::vector<Octet>& buffer) const
{
  _impl->encodeInto(buffer);
}

void HLAfixedArray::decode(VariableLengthData const & inData)
{
  std::vector<Octet> buffer(inData.size());
  if (!buffer.empty())
    std::memcpy(&buffer.front(), inData.data(), inData.size());
  decodeFrom(buffer, 0);
}

size_t
HLAfixedArray::decodeFrom(std::vector<Octet> const & buffer, size_t index)
{
  return _impl->decodeFrom(buffer, index);
}

size_t
HLAfixedArray::getEncodedLength() const
{
  return _impl->getEncodedLength();
}

unsigned int
HLAfixedArray::getOctetBoundary() const
{
  return _impl->getOctetBoundary();
}

bool
HLAfixedArray::isSameTypeAs(DataElement const& inData ) const
{
  if (!DataElement::isSameTypeAs(inData))
    return false;
  const HLAfixedArray* fixedArray = dynamic_cast<const HLAfixedArray*>(&inData);
  if (!fixedArray)
    return false;
  return _impl->isSameTypeAs(*fixedArray->_impl);
}

bool
HLAfixedArray::hasPrototypeSameTypeAs(DataElement const& dataElement) const
{
  const HLAfixedArray* fixedArray = dynamic_cast<const HLAfixedArray*>(&dataElement);
  if (!fixedArray)
    return false;
  return _impl->isSameTypeAs(*fixedArray->_impl);
}

size_t
HLAfixedArray::size () const
{
  return _impl->size();
}

void
HLAfixedArray::set(size_t index, const DataElement& dataElement)
{
  _impl->set(index, dataElement);
}

void
HLAfixedArray::setElementPointer(size_t index, DataElement* dataElement)
{
  _impl->setElementPointer(index, dataElement);
}

const DataElement&
HLAfixedArray::get(size_t index) const
{
  return _impl->get(index);
}

DataElement const&
HLAfixedArray::operator [](size_t index) const
{
  return _impl->get(index);
}

}
