/* -*-c++-*- OpenRTI - Copyright (C) 2009-2012 Mathias Froehlich
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

#pragma once

#include "OpenRTIConfig.h"
#include "DataElement.h"
#include "EncodingConfig.h"

namespace OpenRTI
{
   // Forward Declarations
   class VariableLengthData;
   class HLAvariantRecordImplementation;

   // Interface for the HLAvariantRecord complex data element
   class OPENRTI_LOCAL HLAvariantRecord : public OpenRTI::DataElement
   {
   public:

      // Constructor which accepts a prototype element for discriminants.
      // A clone of the given element acts to validate the discriminant type.
      explicit HLAvariantRecord (
         DataElement const& discriminantPrototype);

      // Copy Constructor
      HLAvariantRecord (
         HLAvariantRecord const & rhs);

      // Destructor
      virtual ~HLAvariantRecord ();

      // Return a new copy of the DataElement
      virtual std::unique_ptr<DataElement> clone () const;

      // Encode this element into a new VariableLengthData
      virtual VariableLengthData encode () const;

      // Encode this element into an existing VariableLengthData
      virtual void encode (
         VariableLengthData& inData) const;

      // Encode this element and append it to a buffer
      virtual void encodeInto (
         std::vector<Octet>& buffer) const;

      // Decode this element from the RTI's VariableLengthData.
      virtual void decode (
         VariableLengthData const & inData);

      // Decode this element starting at the index in the provided buffer
      virtual size_t decodeFrom (
         std::vector<Octet> const & buffer,
         size_t index);

      // Return the size in bytes of this element's encoding.
      virtual size_t getEncodedLength () const;

      // Return the octet boundary of this element.
      virtual unsigned int getOctetBoundary () const;

      // Return true if given element is same type as this; otherwise, false.
      virtual bool isSameTypeAs(
         DataElement const& inData ) const;

      // Return true if given element is same type as specified variant; otherwise, false.
      virtual bool isSameTypeAs(DataElement const& discriminant, 
         DataElement const& inData ) const;

      // Return true if given element matches prototype of this array.
      virtual bool hasMatchingDiscriminantTypeAs(DataElement const& dataElement ) const;

      // Add a new discriminant/variant pair: adds a mapping between the given
      // unique discriminant and a copy of the value element.
      // When encoding, the last discriminant specified (either by adding or setDescriminant)
      // determines the value to be encoded.
      // When decoding, the encoded discriminant will determine which variant is
      // used. The getDescriminant call indicates the variant data element returned
      // by getValue.
      // Discriminants must match prototype
      virtual void addVariant (
         const DataElement& discriminant,
         const DataElement& valuePrototype);

      // Add a new discriminant/variant pair: adds a mapping between the given
      // unique discriminant and the given value element.
      // When encoding, the last discriminant specified (either by adding or
      // setVariant, or setDescriminant) determines the value to be encoded.
      // When decoding, the encoded discriminant will determine which variant is
      // used. The getDescriminant call indicates the variant data element
      // returned by getValue.
      // Discriminants must match prototype
      // Caller is responsible for ensuring that the external memory is
      // valid for the lifetime of this object or until the variant for the
      // given discriminant acquires new memory through setVariantPointer.
      // Null pointer results in an exception.
      virtual void addVariantPointer (
         const DataElement& discriminant,
         DataElement* valuePtr);

      // Set the current value of the discriminant (specifies the type of the value)
      // Discriminants must match prototype
      virtual void setDiscriminant (
         const DataElement& discriminant);

      // Sets the variant with the given discriminant to a copy of the given value
      // Discriminant must match prototype and value must match its variant
      virtual void setVariant (
         const DataElement& discriminant,
         DataElement const& value);

      // Sets the variant with the given discriminant to the given value
      // Discriminant must match prototype and value must match its variant
      // Caller is responsible for ensuring that the external memory is
      // valid for the lifetime of this object or until the variant for the
      // given discriminant acquires new memory through this call.
      // Null pointer results in an exception.
      virtual void setVariantPointer (
         const DataElement& discriminant,
         DataElement* valuePtr);

      // Return a reference to the discriminant element
      virtual const DataElement& getDiscriminant () const;

      // Return a reference to the variant element.
      // Exception thrown if encoded discriminant is not mapped to a value.
      virtual const DataElement& getVariant() const;

   private:

      // Default constructor not allowed
      HLAvariantRecord ();

      // Assignment Operator not allowed
      HLAvariantRecord& operator=(
         HLAvariantRecord const & rhs);

   protected:

      HLAvariantRecordImplementation* _impl;
   };
}

