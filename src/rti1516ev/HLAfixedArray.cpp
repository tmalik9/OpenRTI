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

namespace rti1516ev {

class HLAfixedArrayImplementationBase : public HLAencodingImplementationBase
{
  public:
    virtual ~HLAfixedArrayImplementationBase() {}
    virtual const DataElement* getPrototype() const = 0;
    virtual size_t getEncodedLength() const = 0;
    virtual unsigned int getOctetBoundary() const = 0;
    virtual bool isSameTypeAs(const HLAfixedArrayImplementationBase& rhs) const = 0;
    virtual size_t size() const = 0;
    virtual void set(size_t index, const DataElement& dataElement) = 0;
    virtual void setElementPointer(size_t index, DataElement* dataElement) = 0;
    virtual const DataElement& get(size_t index) const = 0;

    virtual void resize(size_t length) = 0;
    virtual void clear() = 0;
};

template<class BasicTypeEncoding>
class OPENRTI_LOCAL HLAfixedArrayOfBasicTypeImplementation : public HLAfixedArrayImplementationBase
{
  public:
    using SimpleDataType = typename BasicTypeEncoding::SimpleDataType;
    HLAfixedArrayOfBasicTypeImplementation(size_t length) {
      _protoType = std::unique_ptr<BasicTypeEncoding>(new BasicTypeEncoding());
      _dataVector.resize(length);
      _dataElementVector.resize(length);
      _dataElementPointerVector.resize(length);
      for (size_t i = 0; i < length; i++) {
        _dataElementVector[i].setDataPointer(&_dataVector[i]);
        _dataElementPointerVector[i] = &_dataElementVector[i];
      }
    }
    HLAfixedArrayOfBasicTypeImplementation(const HLAfixedArrayImplementation& rhs) {
      _protoType = std::unique_ptr<BasicTypeEncoding>(new BasicTypeEncoding());
      size_t length = rhs.size();
      _dataVector.resize(length);
      _dataElementVector.resize(length);
      _dataElementPointerVector.resize(length);
      for (size_t i = 0; i < length; i++) {
        _dataElementVector[i].setDataPointer(&_dataVector[i]);
        _dataElementPointerVector[i] = &_dataElementVector[i];
      }
    }
    virtual ~HLAfixedArrayOfBasicTypeImplementation() noexcept {
      _protoType.reset();
    }

    const DataElement* getPrototype() const { return _protoType.get(); }

    size_t getEncodedLength() const {
      return _protoType->getEncodedLength() * _dataElementVector.size();
    }

    size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const {
#ifdef _DEBUG
      if (bufferSize < offset + getEncodedLength())
        throw EncoderException(L"buffer to small: bufferSize=" + std::to_wstring(bufferSize) + L" offset=" + std::to_wstring(offset) + L" encodedLength=" + std::to_wstring(getEncodedLength()));
#endif
      for (size_t i = 0; i < _dataElementPointerVector.size(); ++i) {
        offset = _dataElementPointerVector[i]->encodeInto(buffer, bufferSize, offset);
      }
      return offset;
    }

    size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t offset) {
      for (size_t i = 0; i < _dataElementPointerVector.size(); ++i) {
        offset = _dataElementPointerVector[i]->decodeFrom(buffer, bufferSize, offset);
      }
      return offset;
    }

    HLAencodingImplementationBase* clone() const override {
      return new HLAfixedArrayOfBasicTypeImplementation(size());
    }

    unsigned int getOctetBoundary() const {
      return _protoType->getOctetBoundary();
    }

    bool isSameTypeAs(const HLAfixedArrayImplementationBase& rhs) const override {
      return _protoType->isSameTypeAs(*(rhs.getPrototype()));
    }

    size_t size() const {
      return _dataVector.size();
    }

    //void set(size_t index, SimpleDataType dataElement)
    //{
    //  if (_dataVector.size() <= index)
    //    throw EncoderException(L"HLAfixedArray::set(): Index out of range!");
    //  _dataVector[index] = dataElement;
    //  auto& element = _dataElementVector[index];
    //  element.first = &_dataVector[index];
    //  element.second = true;
    //}

    void set(size_t index, const DataElement& dataElement) override
    {
      if (!dataElement.isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAfixedArray::set(): Data type is not compatible!");
      auto* basicDataElement = dynamic_cast<const BasicTypeEncoding*>(&dataElement);
      if (basicDataElement == nullptr)
        throw EncoderException(L"HLAfixedArray::set(): Data type is not compatible!");
      if (_dataVector.size() <= index)
        throw EncoderException(L"HLAfixedArray::set(): Index out of range!");
      *(_dataElementPointerVector[index]) = basicDataElement->get();
    }

    void setElementPointer(size_t index, DataElement* dataElement) override
    {
      if (!dataElement)
        throw EncoderException(L"HLAfixedArray::setElementPointer(): dataElement is zero!");
      BasicTypeEncoding* basicDataElement = dynamic_cast<BasicTypeEncoding*>(dataElement);
      if (basicDataElement == nullptr)
        throw EncoderException(L"HLAfixedArray::setElementPointer(): Data type is not compatible!");
      if (!basicDataElement->isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAfixedArray::setElementPointer(): Data type is not compatible!");
      if (_dataElementVector.size() <= index)
        throw EncoderException(L"HLAfixedArray::setElementPointer(): Index out of range!");
      //if (element.second) delete element.first;
      _dataElementPointerVector[index] = basicDataElement;
    }

    const DataElement& get(size_t index) const override
    {
      return *_dataElementPointerVector[index];
    }

    void resize(size_t length) {
      _dataElementVector.resize(length);
      _dataVector.resize(length);
    }

    void clear() {
      _dataElementVector.clear();
      _dataVector.clear();
    }

    // bool 'second' specifies whether the DataElement pointed is owned by this instance.
    typedef std::vector<BasicTypeEncoding> DataElementVector;
    DataElementVector _dataElementVector;
    typedef std::vector<BasicTypeEncoding*> DataElementPointerVector;
    DataElementPointerVector _dataElementPointerVector;
    std::vector<SimpleDataType> _dataVector;
    std::unique_ptr<BasicTypeEncoding> _protoType;
};

class OPENRTI_LOCAL HLAfixedArrayImplementation : public HLAfixedArrayImplementationBase
{
  public:
    HLAfixedArrayImplementation(const DataElement& protoType, size_t length) {
      _protoType = protoType.clone().release();
      _dataElementVector.resize(length);
      for (DataElementVector::iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
        i->first = _protoType->clone().release();
        i->second = true;
      }
    }
    HLAfixedArrayImplementation(const HLAfixedArrayImplementation& rhs) {
      _protoType = rhs._protoType->clone().release();
      _dataElementVector.resize(rhs._dataElementVector.size(), std::make_pair(nullptr, true));
      for (size_t i = 0; i < rhs._dataElementVector.size(); ++i) {
        _dataElementVector[i].first = rhs._dataElementVector[i].first->clone().release();
      }
    }
    virtual ~HLAfixedArrayImplementation() noexcept {
      for (DataElementVector::iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
        if (i->second) delete i->first;
        i->first = nullptr;
      }
      delete _protoType;
      _protoType = 0;
    }

    const DataElement* getPrototype() const { return _protoType; }

    size_t getEncodedLength() const override {
      size_t length = 0;
      for (auto& element : _dataElementVector) {
        length += element.first->getEncodedLength();
      }
      return length;
    }

    size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const override {
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

    size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index) override {
      for (DataElementVector::const_iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
        if (i->first == nullptr)
          throw EncoderException(L"HLAfixedArray::decodeFrom(): dataElement is zero!");
        index = i->first->decodeFrom(buffer, bufferSize, index);
      }
      return index;
    }

    unsigned int getOctetBoundary() const override {
      if (_dataElementVector.empty())
        return 1;
      return _dataElementVector.front().first->getOctetBoundary();
    }

    size_t size () const override { return _dataElementVector.size(); }

    void set(size_t index, const DataElement& dataElement) override {
      if (_dataElementVector.size() <= index)
        throw EncoderException(L"HLAfixedArray::set(): Index out of range!");
      auto& element = _dataElementVector[index];
      if (!dataElement.isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAfixedArray::set(): Data type is not compatible!");
      if (element.second) delete element.first;
      element.first = dataElement.clone().release();
      element.second = true;
    }

    void setElementPointer(size_t index, DataElement* dataElement) override {
      if (_dataElementVector.size() <= index)
        throw EncoderException(L"HLAfixedArray::setElementPointer(): Index out of range!");
      if (!dataElement)
        throw EncoderException(L"HLAfixedArray::setElementPointer(): dataElement is zero!");
      auto& element = _dataElementVector[index];
      if (!dataElement->isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAfixedArray::setElementPointer(): Data type is not compatible!");
      if (element.second) delete element.first;
      element.first = dataElement;
      element.second = false;
    }

    const DataElement& get(size_t index) const override {
      if (_dataElementVector.size() <= index)
        throw EncoderException(L"HLAfixedArray::get(): Index out of range!");
      auto& element = _dataElementVector[index];
      if (element.first == nullptr)
        throw EncoderException(L"HLAfixedArray::get(): dataElement is zero!");
      return *(_dataElementVector[index].first);
    }

    bool isSameTypeAs(const HLAfixedArrayImplementationBase& rhs) const {
      auto* ref = dynamic_cast<const HLAfixedArrayImplementation*>(&rhs);
      if (_dataElementVector.empty()) {
        return ref->_dataElementVector.empty();
      }
      else {
        if (ref->_dataElementVector.empty())
          return false;
        return _protoType->isSameTypeAs(*(ref->_dataElementVector.front().first));
      }
    }

    void resize(size_t length) override {
      _dataElementVector.resize(length);
    }
    void clear() override {
      _dataElementVector.clear();
    }

    HLAencodingImplementationBase* clone() const override {
      return new HLAfixedArrayImplementation(*_protoType, size());
    }
    // bool 'second' specifies whether the DataElement pointed is owned by this instance.
    typedef std::vector<std::pair<DataElement*, bool>> DataElementVector;
    DataElementVector _dataElementVector;
    DataElement* _protoType;
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
  _impl(static_cast<HLAfixedArrayImplementationBase*>(rhs._impl->clone()))
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
