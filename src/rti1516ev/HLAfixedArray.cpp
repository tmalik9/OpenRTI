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
#include "RTI/encoding/HLAfixedArray.h"

#include "RTI/VariableLengthData.h"
#include "RTI/encoding/DataElement.h"
#include "RTI/encoding/BasicDataElements.h"
#include <cstring>
#include <vector>

#include "Encoding.h"
#include "Export.h"
#include "HLAarrayImplementation.h"

namespace rti1516ev {

template<class BasicTypeEncoding>
class OPENRTI_LOCAL HLAfixedArrayOfBasicTypeImplementation : public HLAarrayOfBasicTypeImplementation<BasicTypeEncoding>
{
  public:
    using baseclass=HLAarrayOfBasicTypeImplementation<BasicTypeEncoding>;
    HLAfixedArrayOfBasicTypeImplementation(size_t length) : baseclass(length) { }
    HLAfixedArrayOfBasicTypeImplementation(const HLAfixedArrayOfBasicTypeImplementation& rhs) : HLAarrayOfBasicTypeImplementation<BasicTypeEncoding>(rhs) {}
    ~HLAfixedArrayOfBasicTypeImplementation() noexcept {}
    size_t getEncodedLength() const override {
      return baseclass::_protoType->getEncodedLength() * baseclass::_dataElementVector.size();
    }

    size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const  override 
    {
#ifdef _DEBUG
      if (bufferSize < offset + getEncodedLength())
        throw EncoderException(L"buffer to small: bufferSize=" + std::to_wstring(bufferSize) + L" offset=" + std::to_wstring(offset) + L" encodedLength=" + std::to_wstring(getEncodedLength()));
#endif
      for (size_t i = 0; i < baseclass::_dataElementPointerVector.size(); ++i) {
        offset = baseclass::_dataElementPointerVector[i]->encodeInto(buffer, bufferSize, offset);
      }
      return offset;
    }

    size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t offset) override
    {
      for (size_t i = 0; i < baseclass::_dataElementPointerVector.size(); ++i) {
        offset = baseclass::_dataElementPointerVector[i]->decodeFrom(buffer, bufferSize, offset);
      }
      return offset;
    }

    HLAencodingImplementationBase* clone() const override {
      return new HLAfixedArrayOfBasicTypeImplementation(*this);
    }
};

class OPENRTI_LOCAL HLAfixedArrayImplementation : public HLAarrayImplementation
{
  public:
    HLAfixedArrayImplementation(const DataElement& protoType, size_t length) : HLAarrayImplementation(protoType, length) {}
    HLAfixedArrayImplementation(const HLAfixedArrayImplementation& rhs) : HLAarrayImplementation(rhs) { }
    virtual ~HLAfixedArrayImplementation() noexcept { }

    size_t getEncodedLength() const override {
      size_t length = 0;
      for (auto& element : _dataElementVector) {
        length += element.first->getEncodedLength();
      }
      return length;
    }

    size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const override
    {
#ifdef _DEBUG
      if (bufferSize < offset + getEncodedLength())
        throw EncoderException(L"buffer to small: bufferSize=" + std::to_wstring(bufferSize) + L" offset=" + std::to_wstring(offset) + L" encodedLength=" + std::to_wstring(getEncodedLength()));
#endif
      for (DataElementVector::const_iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
        if (i->first == nullptr)
          throw EncoderException(L"HLAfixedArray::encodeInto(): dataElement is zero!");
        offset = i->first->encodeInto(buffer, bufferSize, offset);
      }
      return offset;
    }

    size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index) override
    {
      for (DataElementVector::const_iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
        if (i->first == nullptr)
          throw EncoderException(L"HLAfixedArray::decodeFrom(): dataElement is zero!");
        index = i->first->decodeFrom(buffer, bufferSize, index);
      }
      return index;
    }

    HLAencodingImplementationBase* clone() const override {
      return new HLAfixedArrayImplementation(*_protoType, size());
    }
};

HLAfixedArray::HLAfixedArray(const DataElement& protoType, size_t length) : _impl(nullptr)
{
  if (typeid(protoType) == typeid(HLAoctet))
  {
    _impl = new HLAfixedArrayOfBasicTypeImplementation<HLAoctet>(length);
  }
  else if (typeid(protoType) == typeid(HLAbyte))
  {
    _impl = new HLAfixedArrayOfBasicTypeImplementation<HLAbyte>(length);
  }
  else if (typeid(protoType) == typeid(HLAinteger16LE))
  {
    _impl = new HLAfixedArrayOfBasicTypeImplementation<HLAinteger16LE>(length);
  }
  else if (typeid(protoType) == typeid(HLAinteger16BE))
  {
    _impl = new HLAfixedArrayOfBasicTypeImplementation<HLAinteger16BE>(length);
  }
  else if (typeid(protoType) == typeid(HLAinteger32LE))
  {
    _impl = new HLAfixedArrayOfBasicTypeImplementation<HLAinteger32LE>(length);
  }
  else if (typeid(protoType) == typeid(HLAinteger32BE))
  {
    _impl = new HLAfixedArrayOfBasicTypeImplementation<HLAinteger32BE>(length);
  }
  else if (typeid(protoType) == typeid(HLAinteger64LE))
  {
    _impl = new HLAfixedArrayOfBasicTypeImplementation<HLAinteger64LE>(length);
  }
  else if (typeid(protoType) == typeid(HLAinteger64BE))
  {
    _impl = new HLAfixedArrayOfBasicTypeImplementation<HLAinteger64BE>(length);
  }
  else
  {
    _impl = new HLAfixedArrayImplementation(protoType, length);
  }
}

HLAfixedArray::HLAfixedArray(HLAfixedArray const & rhs) :
  _impl(static_cast<HLAarrayImplementationBase*>(rhs._impl->clone()))
{
}

HLAfixedArray::~HLAfixedArray() noexcept
{
  delete _impl;
  _impl = 0;
}

std::unique_ptr<DataElement>
HLAfixedArray::clone () const
{
  return std::unique_ptr<DataElement>(new HLAfixedArray(*this));
}

VariableLengthData
HLAfixedArray::encode () const
{
  VariableLengthData variableLengthData;
  encode(variableLengthData);
  return variableLengthData;
}

void
HLAfixedArray::encode(VariableLengthData& outData) const
{
  outData.resize(getEncodedLength());
  _impl->encodeInto(static_cast<Octet*>(outData.data()), outData.size(), 0);
}

void
HLAfixedArray::encodeInto(std::vector<Octet>& buffer) const
{
  size_t offset = buffer.size();
  size_t encodedLength = getEncodedLength();
  buffer.resize(offset + encodedLength);
  _impl->encodeInto(static_cast<Octet*>(buffer.data()) + offset, encodedLength, offset);
}


size_t HLAfixedArray::encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const
{
  return _impl->encodeInto(buffer, bufferSize, offset);
}

void HLAfixedArray::decode(VariableLengthData const & inData)
{
  _impl->decodeFrom(static_cast<const Octet*>(inData.data()), inData.size(), 0);
}

size_t
HLAfixedArray::decodeFrom(std::vector<Octet> const & buffer, size_t index)
{
  return _impl->decodeFrom(buffer.data(), buffer.size(), index);
}


size_t HLAfixedArray::decodeFrom(const Octet* buffer, size_t bufferSize, size_t index)
{
  return _impl->decodeFrom(buffer, bufferSize, index);
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


void HLAfixedArray::resize(size_t length)
{
  _impl->resize(length);
}

void HLAfixedArray::clear()
{
  _impl->clear();
}


}
