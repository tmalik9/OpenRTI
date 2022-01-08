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
#include <set>
#include "Encoding.h"
#include "Export.h"

namespace rti1516ev {

// common abstract interface for all array implementations
class HLAarrayImplementationBase : public HLAencodingImplementationBase
{
  public:
    virtual ~HLAarrayImplementationBase() {}
    virtual const DataElement* getPrototype() const = 0;
    virtual size_t getEncodedLength() const = 0;
    virtual unsigned int getOctetBoundary() const = 0;
    virtual bool isSameTypeAs(const HLAarrayImplementationBase& rhs) const = 0;
    virtual size_t size() const = 0;
    virtual void set(size_t index, const DataElement& dataElement) = 0;
    virtual void addElement(const DataElement& dataElement) = 0;
    virtual void setElementPointer(size_t index, DataElement* dataElement) = 0;
    virtual void addElementPointer(DataElement* dataElement) = 0;
    virtual const DataElement& get(size_t index) const = 0;

    virtual void resize(size_t length) = 0;
    virtual void clear() = 0;
};

// Implementation base for arrays of basic types.
// The actual data is held in an std::vector<SimpleDataType> (_dataVector),
// the encoders ('data elements') in another array of same size (_dataElementVector),
// and pointers to the actually used encoders in a third array (_dataElementPointerVector).
// The pointers in _dataElementPointerVector may point either into _dataElementVector or to 'outside' encoders.
// get/set always access the actual data through _dataElementPointerVector.
// encoding, decoding, getEncodedLength and clone must be implemented in a subclass (specific to fixed/variable array)
template<class BasicTypeEncoding>
class OPENRTI_LOCAL HLAarrayOfBasicTypeImplementation : public HLAarrayImplementationBase
{
  public:
    using SimpleDataType = typename BasicTypeEncoding::SimpleDataType;
    HLAarrayOfBasicTypeImplementation() {
      _protoType = std::unique_ptr<BasicTypeEncoding>(new BasicTypeEncoding());
    }
    HLAarrayOfBasicTypeImplementation(size_t length) {
      _protoType = std::unique_ptr<BasicTypeEncoding>(new BasicTypeEncoding());
      resize(length);
    }
    // deep copy, data values from rhs are copied into new encoder elements
    HLAarrayOfBasicTypeImplementation(const HLAarrayOfBasicTypeImplementation& rhs) {
      _protoType = std::unique_ptr<BasicTypeEncoding>(new BasicTypeEncoding());
      resize(rhs.size());
      for (size_t i=0;i<rhs.size();i++)
      {
        _dataVector[i] = rhs._dataElementPointerVector[i]->get();
      }
    }

    virtual ~HLAarrayOfBasicTypeImplementation() noexcept {
      _protoType.reset();
    }

    const DataElement* getPrototype() const override { return _protoType.get(); }

    unsigned int getOctetBoundary() const override {
      return _protoType->getOctetBoundary();
    }

    bool isSameTypeAs(const HLAarrayImplementationBase& rhs) const override {
      return _protoType->isSameTypeAs(*(rhs.getPrototype()));
    }

    size_t size() const override {
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

    void set(size_t index, const DataElement& dataElement) override {
      if (!dataElement.isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAfixedArray::set(): Data type is not compatible!");
      auto* basicDataElement = dynamic_cast<const BasicTypeEncoding*>(&dataElement);
      if (basicDataElement == nullptr)
        throw EncoderException(L"HLAfixedArray::set(): Data type is not compatible!");
      if (_dataElementPointerVector.size() <= index)
        throw EncoderException(L"HLAfixedArray::set(): Index out of range!");
      _dataElementPointerVector[index]->set(basicDataElement->get());
    }

    void addElement(const DataElement& dataElement) override {
      if (!dataElement.isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAvariableArray::addElement(): Data type is not compatible!");
      auto* basicDataElement = dynamic_cast<const BasicTypeEncoding*>(&dataElement);
      if (basicDataElement == nullptr)
        throw EncoderException(L"HLAfixedArray::set(): Data type is not compatible!");
      auto clonedBasicDataElement = basicDataElement->clone();
      if (dynamic_cast<BasicTypeEncoding*>(clonedBasicDataElement.get()) == nullptr)
        throw EncoderException(L"HLAfixedArray::set(): Data type is not compatible!");
      _dataElementPointerVector.push_back(dynamic_cast<BasicTypeEncoding*>(clonedBasicDataElement.release()));
    }

    void setElementPointer(size_t index, DataElement* dataElement) override {
      if (!dataElement)
        throw EncoderException(L"HLAfixedArray::setElementPointer(): dataElement is zero!");
      BasicTypeEncoding* basicDataElement = dynamic_cast<BasicTypeEncoding*>(dataElement);
      if (basicDataElement == nullptr)
        throw EncoderException(L"HLAfixedArray::setElementPointer(): Data type is not compatible!");
      if (!basicDataElement->isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAfixedArray::setElementPointer(): Data type is not compatible!");
      if (_dataElementVector.size() <= index)
        throw EncoderException(L"HLAfixedArray::setElementPointer(): Index out of range!");
      _dataElementPointerVector[index] = basicDataElement;
    }

    void addElementPointer(DataElement* dataElement) override {
      if (!dataElement)
        throw EncoderException(L"HLAvariableArray::addElementPointer(): dataElement is zero!");
      BasicTypeEncoding* basicDataElement = dynamic_cast<BasicTypeEncoding*>(dataElement);
      if (basicDataElement == nullptr)
        throw EncoderException(L"HLAfixedArray::setElementPointer(): Data type is not compatible!");
      if (!dataElement->isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAvariableArray::addElementPointer(): Data type is not compatible!");
      resize(size() + 1);
      _dataElementPointerVector[size() - 1] = basicDataElement;
    }

    const DataElement& get(size_t index) const override {
      return *_dataElementPointerVector[index];
    }

    void resize(size_t length) override { 

      // remember which elements have been owned by this array (e.g. are actually are part of _dataVector)
      std::set<size_t> existingOwnedElements;
      if (_dataElementVector.size() > 0)
      {
        BasicTypeEncoding* first = &_dataElementVector[0];
        BasicTypeEncoding* last = &_dataElementVector[_dataElementVector.size() - 1];
        for (size_t i = 0; i < _dataElementPointerVector.size();i++)
        {
          // if _dataElementPointerVector[i] points into _dataElementVector, it is one of our own encoders in _dataElementVector.
          if (_dataElementPointerVector[i] > first && _dataElementPointerVector[i] < last)
            existingOwnedElements.insert(i);
        }
      }
      // now resize 
      _dataVector.resize(length);
      _dataElementVector.resize(length);
      _dataElementPointerVector.resize(length);
      // set the data pointers of the encoders in the (resized) _dataElementVector to the elements in the
      // (possibly reallocated) _dataVector
      for (size_t i = 0; i < length; i++) {
        // all of our own encoders shall point into our owned data
        _dataElementVector[i].setDataPointer(&_dataVector[i]);
        // if it is an already 'owned' data element, or a freshly allocated element,
        // set the encoder reference to the corresponding encoder owned by this array.
        if (_dataElementPointerVector[i] == nullptr || existingOwnedElements.find(i) != existingOwnedElements.end())
          _dataElementPointerVector[i] = &_dataElementVector[i];
      }
    }
    void clear() override {
      _dataElementPointerVector.clear();
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

// implementation base for arrays of compound (non-basic) types
// encoding, decoding, getEncodedLength and clone must be implemented in subclass (specific to fixed/variable array)
class OPENRTI_LOCAL HLAarrayImplementation : public HLAarrayImplementationBase
{
  public:
    HLAarrayImplementation(const DataElement& protoType) {
      _protoType = protoType.clone().release();
    }
    HLAarrayImplementation(const DataElement& protoType, size_t length) {
      _protoType = protoType.clone().release();
      _dataElementVector.resize(length);
      for (DataElementVector::iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
        i->first = _protoType->clone().release();
        i->second = true;
      }
    }
    HLAarrayImplementation(const HLAarrayImplementation& rhs) {
      _protoType = rhs._protoType->clone().release();
      _dataElementVector.resize(rhs._dataElementVector.size(), std::make_pair(nullptr, true));
      for (size_t i = 0; i < rhs._dataElementVector.size(); ++i) {
        _dataElementVector[i].first = rhs._dataElementVector[i].first->clone().release();
      }
    }
    virtual ~HLAarrayImplementation() noexcept {
      for (DataElementVector::iterator i = _dataElementVector.begin(); i != _dataElementVector.end(); ++i) {
        if (i->second) delete i->first;
        i->first = nullptr;
      }
      delete _protoType;
      _protoType = 0;
    }

    const DataElement* getPrototype() const override { return _protoType; }

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

    void addElement(const DataElement& dataElement) override {
      if (!dataElement.isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAvariableArray::addElement(): Data type is not compatible!");
      _dataElementVector.push_back({dataElement.clone().release(), true});
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

    void addElementPointer(DataElement* dataElement) override {
      if (!dataElement)
        throw EncoderException(L"HLAvariableArray::addElementPointer(): dataElement is zero!");
      if (!dataElement->isSameTypeAs(*_protoType))
        throw EncoderException(L"HLAvariableArray::addElementPointer(): Data type is not compatible!");
      _dataElementVector.push_back({dataElement, false});
    }

    const DataElement& get(size_t index) const override {
      if (_dataElementVector.size() <= index)
        throw EncoderException(L"HLAfixedArray::get(): Index out of range!");
      auto& element = _dataElementVector[index];
      if (element.first == nullptr)
        throw EncoderException(L"HLAfixedArray::get(): dataElement is zero!");
      return *(_dataElementVector[index].first);
    }

    bool isSameTypeAs(const HLAarrayImplementationBase& rhs) const override {
      auto* ref = dynamic_cast<const HLAarrayImplementation*>(&rhs);
      if (ref == nullptr)
      {
        return false;
      }
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
      size_t oldSize = _dataElementVector.size();
      if (length < oldSize)
      {
        for (size_t i = length; i < oldSize; i++)
        {
          delete _dataElementVector[i].first; _dataElementVector[i].first = 0;
        }
      }
      _dataElementVector.resize(length, std::make_pair(nullptr, true));
      if (length > oldSize)
      {
        for (size_t i = oldSize; i < length; i++)
        {
          _dataElementVector[i].first = _protoType->clone().release();
        }
      }
    }
    void clear() override {
      _dataElementVector.clear();
    }

    // bool 'second' specifies whether the DataElement pointed is owned by this instance.
    typedef std::vector<std::pair<DataElement*, bool>> DataElementVector;
    DataElementVector _dataElementVector;
    DataElement* _protoType;
};

} // namespace rti1516ev