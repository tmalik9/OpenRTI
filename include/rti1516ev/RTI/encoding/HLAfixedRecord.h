/***********************************************************************
   The IEEE hereby grants a general, royalty-free license to copy, distribute,
   display and make derivative works from this material, for all purposes,
   provided that any use of the material contains the following
   attribution: "Reprinted with permission from IEEE 1516.1(TM)-2010".
   Should you require additional information, contact the Manager, Standards
   Intellectual Property, IEEE Standards Association (stds-ipr@ieee.org).
***********************************************************************/
/***********************************************************************
   IEEE 1516.1 High Level Architecture Interface Specification C++ API
   File: HLAfixedRecord.h
***********************************************************************/
#ifndef RTI_HLAfixedRecord_h_
#define RTI_HLAfixedRecord_h_

#include <RTI/encoding/DataElement.h>
#include <RTI/encoding/EncodingConfig.h>
#include <RTI/SpecificConfig.h>

namespace rti1516ev {

// Forward Declarations
class VariableLengthData;
class HLAfixedRecordImplementation;
// Base class for all encoder implementations. Not part of the public API.
class HLAencodingImplementationBase;

// Interface for the HLAfixedRecord complex data element
class RTI_EXPORT HLAfixedRecord : public rti1516ev::DataElement
{
  public:

    // Default Constructor
    HLAfixedRecord (uint32_t version = 0);

    // Copy Constructor
    HLAfixedRecord (
      HLAfixedRecord const & rhs);

    // Destructor
    virtual ~HLAfixedRecord () noexcept;

    // Return a new copy of the DataElement
    virtual std::unique_ptr<DataElement> clone () const override;

    // Encode this element into a new VariableLengthData
    virtual VariableLengthData encode () const override;

    // Encode this element into an existing VariableLengthData
    virtual void encode (
      VariableLengthData& inData) const override;

    // Encode this element and append it to a buffer
    virtual void encodeInto (
      std::vector<Octet>& buffer) const override;

    // Encode this element into a preallocated buffer
    virtual size_t encodeInto (Octet* buffer, size_t bufferSize, size_t offset) const override;

    // Decode this element from the RTI's VariableLengthData.
    virtual void decode (
      VariableLengthData const & inData) override;

    // Decode this element starting at the index in the provided buffer
    virtual size_t decodeFrom (
      std::vector<Octet> const & buffer,
      size_t index) override;

    // Decode this element starting at the index in the provided buffer
    size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index) override;

    // Return the size in bytes of this record's encoding.
    virtual size_t getEncodedLength () const override;

    // Return the octet boundary of this element.
    virtual unsigned int getOctetBoundary () const override;

    // Return true if given element is same type as this; otherwise, false.
    virtual bool isSameTypeAs(
      DataElement const& inData ) const override;

    // Return true if given element is same type as the indexed element;
    // otherwise, false.
    virtual bool hasElementSameTypeAs(size_t index,
                                      DataElement const& inData ) const;

    // Return the number of elements in this fixed record.
    virtual size_t size () const;

    // Append a copy of the dataElement instance to this fixed record.
    virtual void appendElement (const DataElement& dataElement, uint32_t fieldVersion = 0);

    // Append the dataElement instance to this fixed record.
    // Null pointer results in an exception.
    virtual void appendElementPointer (DataElement* dataElement, uint32_t fieldVersion = 0);

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

    uint32_t getVersion() const;

  private:

    // Assignment Operator not allowed
    HLAfixedRecord& operator=(
      HLAfixedRecord const & rhs);

  protected:

    HLAfixedRecordImplementation* _impl;
};
}

#endif // RTI_HLAfixedRecord_h_

