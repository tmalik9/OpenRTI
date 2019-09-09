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

#include <RTI/encoding/HLAvariantRecord.h>

#include <RTI/VariableLengthData.h>
#include <RTI/encoding/DataElement.h>

#include <cstring>
#include <list>

#include "Encoding.h"
#include "Export.h"

namespace rti1516e
{

class OPENRTI_LOCAL HLAvariantRecordImplementation {
public:
  HLAvariantRecordImplementation(DataElement const& discriminantPrototype) :
    _octetBoundary(0)
  {
  }
  HLAvariantRecordImplementation(const HLAvariantRecordImplementation& rhs) :
    _discriminant(rhs._discriminant->clone()),
    _variant(rhs._variant->clone()),
    _octetBoundary(0)
  {
  }
  ~HLAvariantRecordImplementation()
  {
  }

  void encodeInto(std::vector<Octet>& buffer)
  {
    /* FIXME */
  }

  size_t decodeFrom(std::vector<Octet> const & buffer, size_t index)
  {
    /* FIXME */
    return index;
  }

  size_t getEncodedLength()
  {
    /* FIXME */
    return 0;
  }

  unsigned getOctetBoundary()
  {
    if (_octetBoundary)
      return _octetBoundary;
    /*FIXME*/
    return _octetBoundary;
  }

  bool isSameTypeAs(HLAvariantRecordImplementation const& inData) const
  {
    return false; /*FIXME*/
  }

  bool isSameTypeAs(DataElement const& discriminant, DataElement const& inData) const
  {
    return false; /*FIXME*/
  }

  bool hasMatchingDiscriminantTypeAs(DataElement const& dataElement) const
  {
    return false; /*FIXME*/
  }

  void addVariant(const DataElement& discriminant, const DataElement& valuePrototype)
  {
    /*FIXME*/
  }

  void addVariantPointer(const DataElement& discriminant, DataElement* valuePtr)
  {
    /*FIXME*/
  }

  void setDiscriminant(const DataElement& discriminant)
  {
    /*FIXME*/
  }

  void setVariant(const DataElement& discriminant, DataElement const& value)
  {
    /*FIXME*/
  }

  void setVariantPointer(const DataElement& discriminant, DataElement* valuePtr)
  {
    /*FIXME*/
  }

  const DataElement& getDiscriminant() const
  {
    return *_discriminant;
  }

  const DataElement& getVariant() const
  {
    return *_variant;
  }

  std::auto_ptr<DataElement> _discriminant;
  std::auto_ptr<DataElement> _variant;
  unsigned _octetBoundary;
};

HLAvariantRecord::HLAvariantRecord(DataElement const& discriminantPrototype) :
    /*FIXME*/
  // _impl(new HLAvariantRecordImplementation(discriminantPrototype))
  _impl(0)
{
}

HLAvariantRecord::HLAvariantRecord(HLAvariantRecord const & rhs) :
    /*FIXME*/
  // _impl(new HLAvariantRecordImplementation(*rhs._impl))
  _impl(0)
{
}

HLAvariantRecord::~HLAvariantRecord()
{
  delete _impl;
  _impl = 0;
}

std::auto_ptr<DataElement>
HLAvariantRecord::clone () const
{
  return std::auto_ptr<rti1516e::DataElement>(new HLAvariantRecord(*this));
}

VariableLengthData
HLAvariantRecord::encode () const
  throw (EncoderException)
{
  VariableLengthData variableLengthData;
  encode(variableLengthData);
  return variableLengthData;
}

void
HLAvariantRecord::encode(VariableLengthData& inData) const
  throw (EncoderException)
{
  std::vector<Octet> buffer;
  buffer.reserve(getEncodedLength());
  encodeInto(buffer);
  inData.setData(&buffer.front(), buffer.size());
}

void
HLAvariantRecord::encodeInto(std::vector<Octet>& buffer) const
  throw (EncoderException)
{
  _impl->encodeInto(buffer);
}

void HLAvariantRecord::decode(VariableLengthData const & inData)
  throw (EncoderException)
{
  std::vector<Octet> buffer(inData.size());
  std::memcpy(&buffer.front(), inData.data(), inData.size());
  decodeFrom(buffer, 0);
}

size_t
HLAvariantRecord::decodeFrom(std::vector<Octet> const & buffer, size_t index)
  throw (EncoderException)
{
  return _impl->decodeFrom(buffer, index);
}

size_t
HLAvariantRecord::getEncodedLength() const
  throw (EncoderException)
{
  return _impl->getEncodedLength();
}

unsigned int
HLAvariantRecord::getOctetBoundary() const
{
  return _impl->getOctetBoundary();
}

bool
HLAvariantRecord::isSameTypeAs(DataElement const& inData) const
{
  const HLAvariantRecord* variantRecord = dynamic_cast<const HLAvariantRecord*>(&inData);
  if (!variantRecord)
    return false;
  return _impl->isSameTypeAs(*variantRecord->_impl);
}

bool
HLAvariantRecord::isSameTypeAs(DataElement const& discriminant, DataElement const& inData) const
  throw (EncoderException)
{
  return _impl->isSameTypeAs(discriminant, inData);
}

bool
HLAvariantRecord::hasMatchingDiscriminantTypeAs(DataElement const& dataElement) const
{
  return _impl->hasMatchingDiscriminantTypeAs(dataElement);
}

void
HLAvariantRecord::addVariant(const DataElement& discriminant, const DataElement& valuePrototype)
  throw (EncoderException)
{
  _impl->addVariant(discriminant, valuePrototype);
}

void
HLAvariantRecord::addVariantPointer(const DataElement& discriminant, DataElement* valuePtr)
  throw (EncoderException)
{
  _impl->addVariantPointer(discriminant, valuePtr);
}

void
HLAvariantRecord::setDiscriminant(const DataElement& discriminant)
  throw (EncoderException)
{
  _impl->setDiscriminant(discriminant);
}

void
HLAvariantRecord::setVariant(const DataElement& discriminant, DataElement const& value)
  throw (EncoderException)
{
  _impl->setVariant(discriminant, value);
}

void
HLAvariantRecord::setVariantPointer(const DataElement& discriminant, DataElement* valuePtr)
  throw (EncoderException)
{
  _impl->setVariantPointer(discriminant, valuePtr);
}

const DataElement&
HLAvariantRecord::getDiscriminant() const
{
  return _impl->getDiscriminant();
}

const DataElement&
HLAvariantRecord::getVariant() const
  throw (EncoderException)
{
  return _impl->getVariant();
}

}
