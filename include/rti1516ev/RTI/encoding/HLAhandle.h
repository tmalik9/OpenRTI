#pragma once

#include <RTI/encoding/DataElement.h>
#include <RTI/encoding/EncodingConfig.h>
#include <RTI/SpecificConfig.h>
#include "RTI/Handle.h"

namespace rti1516ev {

class RTI_EXPORT HLAhandle : public DataElement
{
    enum HandleKind
    {
      eFederateHandle,
      eObjectClassHandle,
      eObjectInstanceHandle,
      eInteractionClassHandle,
      eAttributeHandle,
      eParameterHandle,
      eInvalid
    };
  public:
    HLAhandle();
    HLAhandle(const HLAhandle& ref);
    HLAhandle(const FederateHandle& handle);
    HLAhandle(const ObjectClassHandle& handle);
    HLAhandle(const ObjectInstanceHandle& handle);
    HLAhandle(const AttributeHandle& handle);
    HLAhandle(const InteractionClassHandle& handle);
    HLAhandle(const ParameterHandle& handle);
    ~HLAhandle() noexcept;
    HLAhandle& operator=(const HLAhandle& ref);

    FederateHandle getFederateHandle() const;
    ObjectClassHandle getObjectClassHandle() const;
    ObjectInstanceHandle getObjectInstanceHandle() const;
    AttributeHandle getAttributeHandle() const;
    InteractionClassHandle getInteractionClassHandle() const;
    ParameterHandle getParameterHandle() const;

    void set(HLAhandle handle);
    void set(FederateHandle handle);
    void set(ObjectClassHandle handle);
    void set(ObjectInstanceHandle handle);
    void set(AttributeHandle handle);
    void set(InteractionClassHandle handle);
    void set(ParameterHandle handle);

    std::unique_ptr<DataElement> clone() const override {
      return std::unique_ptr<DataElement>(new HLAhandle(*this));
    }

    VariableLengthData encode() const override;
    void encode(VariableLengthData& inData) const override;
    void encodeInto(std::vector<Octet>& buffer) const override;

    // Encode this element into a preallocated buffer
    virtual size_t encodeInto(Octet* buffer, size_t bufferSize, size_t offset) const override;

    void decode(VariableLengthData const & inData) override;
    size_t decodeFrom(std::vector<Octet> const & buffer, size_t index) override;
    size_t decodeFrom(const Octet* buffer, size_t bufferSize, size_t index) override;

    size_t getEncodedLength() const override;
    unsigned int getOctetBoundary() const override;

  protected:
    class Implementation;
    std::unique_ptr<Implementation> _impl;
};

class RTI_EXPORT HLAfederateHandle : public HLAhandle
{
  public:
    HLAfederateHandle() = default;
    HLAfederateHandle(const HLAfederateHandle& ref) = default;
    HLAfederateHandle(const FederateHandle& handle);
    ~HLAfederateHandle() noexcept = default;
    HLAfederateHandle& operator=(const HLAfederateHandle& ref);
    void set(FederateHandle handle) {
      HLAhandle::set(handle);
    }
    FederateHandle get() const {
      return HLAhandle::getFederateHandle();
    }
};

class RTI_EXPORT HLAobjectClassHandle : public HLAhandle
{
  public:
    HLAobjectClassHandle() = default;
    HLAobjectClassHandle(const HLAobjectClassHandle& ref) = default;
    HLAobjectClassHandle(const ObjectClassHandle& handle) : HLAhandle(handle) {}
    ~HLAobjectClassHandle() noexcept = default;
    HLAobjectClassHandle& operator=(const HLAobjectClassHandle& ref);
    void set(ObjectClassHandle handle) {
      HLAhandle::set(handle);
    }
    ObjectClassHandle get() const {
      return HLAhandle::getObjectClassHandle();
    }
};

class RTI_EXPORT HLAobjectInstanceHandle : public HLAhandle
{
  public:
    HLAobjectInstanceHandle() = default;
    HLAobjectInstanceHandle(const HLAobjectInstanceHandle& ref) = default;
    HLAobjectInstanceHandle(const ObjectInstanceHandle& handle) : HLAhandle(handle) {}
    ~HLAobjectInstanceHandle() noexcept = default;
    HLAobjectInstanceHandle& operator=(const HLAobjectInstanceHandle& ref);
    void set(ObjectInstanceHandle handle) {
      HLAhandle::set(handle);
    }
    ObjectInstanceHandle get() const {
      return HLAhandle::getObjectInstanceHandle();
    }
};

class RTI_EXPORT HLAattributeHandle : public HLAhandle
{
  public:
    HLAattributeHandle() = default;
    HLAattributeHandle(const HLAattributeHandle& ref) = default;
    HLAattributeHandle(const AttributeHandle& handle) : HLAhandle(handle) {}
    ~HLAattributeHandle() noexcept = default;
    HLAattributeHandle& operator=(const HLAattributeHandle& ref);
    void set(AttributeHandle handle) {
      HLAhandle::set(handle);
    }
    AttributeHandle get() const {
      return HLAhandle::getAttributeHandle();
    }
};

class RTI_EXPORT HLAinteractionClassHandle : public HLAhandle
{
public:
  HLAinteractionClassHandle() = default;
  HLAinteractionClassHandle(const HLAinteractionClassHandle& ref) = default;
  HLAinteractionClassHandle(const InteractionClassHandle& handle) : HLAhandle(handle) {}
  ~HLAinteractionClassHandle() noexcept = default;
  HLAinteractionClassHandle& operator=(const HLAinteractionClassHandle& ref);
  void set(InteractionClassHandle handle) {
    HLAhandle::set(handle);
  }
  InteractionClassHandle get() const {
    return HLAhandle::getInteractionClassHandle();
  }
};

class RTI_EXPORT HLAparameterHandle : public HLAhandle
{
public:
  HLAparameterHandle() = default;
  HLAparameterHandle(const HLAparameterHandle& ref) = default;
  HLAparameterHandle(const InteractionClassHandle& handle) : HLAhandle(handle) {}
  ~HLAparameterHandle() noexcept = default;
  HLAparameterHandle& operator=(const HLAparameterHandle& ref);
  void set(ParameterHandle handle) {
    HLAhandle::set(handle);
  }
  ParameterHandle get() const {
    return HLAhandle::getParameterHandle();
  }
};

}