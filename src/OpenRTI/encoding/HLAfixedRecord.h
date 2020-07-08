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
   class HLAfixedRecordImplementation;

   // Interface for the HLAfixedRecord complex data element
   class OPENRTI_LOCAL HLAfixedRecord : public OpenRTI::DataElement
   {
   public:

      // Default Constructor
      HLAfixedRecord ();

      // Copy Constructor
      HLAfixedRecord (
         HLAfixedRecord const & rhs);

      // Destructor
      virtual ~HLAfixedRecord ();

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

      // Return the size in bytes of this record's encoding.
      virtual size_t getEncodedLength () const;

      // Return the octet boundary of this element.
      virtual unsigned int getOctetBoundary () const;

      // Return true if given element is same type as this; otherwise, false.
      virtual bool isSameTypeAs(
         DataElement const& inData ) const;

      // Return true if given element is same type as the indexed element;
      // otherwise, false.
      virtual bool hasElementSameTypeAs(size_t index,
         DataElement const& inData ) const;

      // Return the number of elements in this fixed record.
      virtual size_t size () const;

      // Append a copy of the dataElement instance to this fixed record.
      virtual void appendElement (const DataElement& dataElement);

      // Append the dataElement instance to this fixed record.
      // Null pointer results in an exception.
      virtual void appendElementPointer (DataElement* dataElement);

      // Sets the element at the given index to a copy of the given element instance
      // Element must match prototype of existing element at this index.
      virtual void set (size_t index,
         const DataElement& dataElement);

      // Sets the element at the given index to the given element instance
      // Element must match prototype of existing element at this index.
      // Null pointer results in an exception.
      virtual void setElementPointer (size_t index, DataElement* dataElement);

      // Return a const reference to the element at the specified index.
      // Must use set to change element.
      virtual const DataElement& get (
         size_t index) const;

      // Return a const reference to the element instance at the specified index.
      // Must use set to change element.
      DataElement const& operator [](size_t index) const;

   private:

      // Assignment Operator not allowed
      HLAfixedRecord& operator=(
         HLAfixedRecord const & rhs);

   protected:

      HLAfixedRecordImplementation* _impl;
   };
}

