
// Code automatically generated by T4 template from D:\vfs\OpenRTI-codegen\tests\rti1516ev\codegen\\RTFederate.xml, do not edit
#pragma once

#include <vector>
#include <functional>

#include "RTI/encoding/HLAopaqueData.h"
#include "RTI/encoding/HLAhandle.h"
#include "RTFederateEncodings.h"

namespace NDistSimIB {
namespace NRTFederateEncoding {

class IHLAobjectRoot;
class IHLAobjectRootObjectClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe() = 0;
    virtual void Unsubscribe() = 0;
    virtual IHLAobjectRoot* GetObjectInstance(const std::wstring& instanceName) = 0;
    virtual IHLAobjectRoot* CreateObjectInstance(const std::wstring& instanceName) = 0;
};

class IHLAobjectRoot
{
  public:
    using AttributeBits = uint32_t;
    static const AttributeBits kNone = 0x0;

    IHLAobjectRoot() {}
    virtual ~IHLAobjectRoot() {}
    IHLAobjectRoot(const IHLAobjectRoot&) = delete;
    IHLAobjectRoot(IHLAobjectRoot&&) = delete;
    IHLAobjectRoot& operator=(const IHLAobjectRoot&) = delete;
    IHLAobjectRoot& operator=(IHLAobjectRoot&&) = delete;

    virtual std::wstring GetObjectInstanceName() const = 0;
    // attribute HLAprivilegeToDeleteObject : no data type
};

class ISystemVariable;
class ISystemVariableObjectClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe() = 0;
    virtual void Unsubscribe() = 0;
    virtual ISystemVariable* GetObjectInstance(const std::wstring& instanceName) = 0;
    virtual ISystemVariable* CreateObjectInstance(const std::wstring& instanceName) = 0;
};

class ISystemVariable : public IHLAobjectRoot
{
  public:
    static const AttributeBits kValueBit = 0x2;

    ISystemVariable() {}
    ~ISystemVariable() {}
    ISystemVariable(const ISystemVariable&) = delete;
    ISystemVariable(ISystemVariable&&) = delete;
    ISystemVariable& operator=(const ISystemVariable&) = delete;
    ISystemVariable& operator=(ISystemVariable&&) = delete;

    // attribute Value : HLAopaqueData
    virtual std::vector<uint8_t> GetValue() const = 0;
    virtual void SetValue(std::vector<uint8_t> newValue) = 0;
    virtual void UpdateAllAttributeValues() = 0;
    virtual void UpdateAllAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual void UpdateModifiedAttributeValues() = 0;
    virtual void UpdateModifiedAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual AttributeBits GetUpdatedAttributes() const = 0;
    using UpdateCallbackType = std::function<void(ISystemVariable*)>;
    virtual uint32_t RegisterUpdateCallback(UpdateCallbackType callback) = 0;
    virtual void UnregisterUpdateCallback(uint32_t callbackToken) = 0;
};

class IValueEntity;
class IValueEntityObjectClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe() = 0;
    virtual void Unsubscribe() = 0;
    virtual IValueEntity* GetObjectInstance(const std::wstring& instanceName) = 0;
    virtual IValueEntity* CreateObjectInstance(const std::wstring& instanceName) = 0;
};

class IValueEntity : public IHLAobjectRoot
{
  public:
    static const AttributeBits kValueBit = 0x2;

    IValueEntity() {}
    ~IValueEntity() {}
    IValueEntity(const IValueEntity&) = delete;
    IValueEntity(IValueEntity&&) = delete;
    IValueEntity& operator=(const IValueEntity&) = delete;
    IValueEntity& operator=(IValueEntity&&) = delete;

    // attribute Value : HLAopaqueData
    virtual std::vector<uint8_t> GetValue() const = 0;
    virtual void SetValue(std::vector<uint8_t> newValue) = 0;
    virtual void UpdateAllAttributeValues() = 0;
    virtual void UpdateAllAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual void UpdateModifiedAttributeValues() = 0;
    virtual void UpdateModifiedAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual AttributeBits GetUpdatedAttributes() const = 0;
    using UpdateCallbackType = std::function<void(IValueEntity*)>;
    virtual uint32_t RegisterUpdateCallback(UpdateCallbackType callback) = 0;
    virtual void UnregisterUpdateCallback(uint32_t callbackToken) = 0;
};

class IDOMemberSource;
class IDOMemberSourceObjectClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe() = 0;
    virtual void Unsubscribe() = 0;
    virtual IDOMemberSource* GetObjectInstance(const std::wstring& instanceName) = 0;
    virtual IDOMemberSource* CreateObjectInstance(const std::wstring& instanceName) = 0;
};

class IDOMemberSource : public IHLAobjectRoot
{
  public:
    static const AttributeBits kDOSourceMemberNameBit = 0x2;
    static const AttributeBits kDOSourceMemberConnectionTypeBit = 0x4;
    static const AttributeBits kDOSourceMemberDataBytesBit = 0x8;

    IDOMemberSource() {}
    ~IDOMemberSource() {}
    IDOMemberSource(const IDOMemberSource&) = delete;
    IDOMemberSource(IDOMemberSource&&) = delete;
    IDOMemberSource& operator=(const IDOMemberSource&) = delete;
    IDOMemberSource& operator=(IDOMemberSource&&) = delete;

    // attribute DOSourceMemberName : HLAASCIIstring
    virtual std::string GetDOSourceMemberName() const = 0;
    virtual void SetDOSourceMemberName(std::string newValue) = 0;
    // attribute DOSourceMemberConnectionType : HLAASCIIstring
    virtual std::string GetDOSourceMemberConnectionType() const = 0;
    virtual void SetDOSourceMemberConnectionType(std::string newValue) = 0;
    // attribute DOSourceMemberDataBytes : HLAopaqueData
    virtual std::vector<uint8_t> GetDOSourceMemberDataBytes() const = 0;
    virtual void SetDOSourceMemberDataBytes(std::vector<uint8_t> newValue) = 0;
    virtual void UpdateAllAttributeValues() = 0;
    virtual void UpdateAllAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual void UpdateModifiedAttributeValues() = 0;
    virtual void UpdateModifiedAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual AttributeBits GetUpdatedAttributes() const = 0;
    using UpdateCallbackType = std::function<void(IDOMemberSource*)>;
    virtual uint32_t RegisterUpdateCallback(UpdateCallbackType callback) = 0;
    virtual void UnregisterUpdateCallback(uint32_t callbackToken) = 0;
};

class IDOMemberTarget;
class IDOMemberTargetObjectClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe() = 0;
    virtual void Unsubscribe() = 0;
    virtual IDOMemberTarget* GetObjectInstance(const std::wstring& instanceName) = 0;
    virtual IDOMemberTarget* CreateObjectInstance(const std::wstring& instanceName) = 0;
};

class IDOMemberTarget : public IHLAobjectRoot
{
  public:
    static const AttributeBits kDOTargetMemberNameBit = 0x2;
    static const AttributeBits kDOTargetMemberConnectionTypeBit = 0x4;

    IDOMemberTarget() {}
    ~IDOMemberTarget() {}
    IDOMemberTarget(const IDOMemberTarget&) = delete;
    IDOMemberTarget(IDOMemberTarget&&) = delete;
    IDOMemberTarget& operator=(const IDOMemberTarget&) = delete;
    IDOMemberTarget& operator=(IDOMemberTarget&&) = delete;

    // attribute DOTargetMemberName : HLAASCIIstring
    virtual std::string GetDOTargetMemberName() const = 0;
    virtual void SetDOTargetMemberName(std::string newValue) = 0;
    // attribute DOTargetMemberConnectionType : HLAASCIIstring
    virtual std::string GetDOTargetMemberConnectionType() const = 0;
    virtual void SetDOTargetMemberConnectionType(std::string newValue) = 0;
    virtual void UpdateAllAttributeValues() = 0;
    virtual void UpdateAllAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual void UpdateModifiedAttributeValues() = 0;
    virtual void UpdateModifiedAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual AttributeBits GetUpdatedAttributes() const = 0;
    using UpdateCallbackType = std::function<void(IDOMemberTarget*)>;
    virtual uint32_t RegisterUpdateCallback(UpdateCallbackType callback) = 0;
    virtual void UnregisterUpdateCallback(uint32_t callbackToken) = 0;
};

class IBusManagement;
class IBusManagementObjectClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe() = 0;
    virtual void Unsubscribe() = 0;
    virtual IBusManagement* GetObjectInstance(const std::wstring& instanceName) = 0;
    virtual IBusManagement* CreateObjectInstance(const std::wstring& instanceName) = 0;
};

class IBusManagement : public IHLAobjectRoot
{
  public:
    static const AttributeBits kNetworkIDBit = 0x2;

    IBusManagement() {}
    ~IBusManagement() {}
    IBusManagement(const IBusManagement&) = delete;
    IBusManagement(IBusManagement&&) = delete;
    IBusManagement& operator=(const IBusManagement&) = delete;
    IBusManagement& operator=(IBusManagement&&) = delete;

    // attribute NetworkID : HLAASCIIstring
    virtual std::string GetNetworkID() const = 0;
    virtual void SetNetworkID(std::string newValue) = 0;
    virtual void UpdateAllAttributeValues() = 0;
    virtual void UpdateAllAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual void UpdateModifiedAttributeValues() = 0;
    virtual void UpdateModifiedAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual AttributeBits GetUpdatedAttributes() const = 0;
};

class IBusManagementCan;
class IBusManagementCanObjectClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe() = 0;
    virtual void Unsubscribe() = 0;
    virtual IBusManagementCan* GetObjectInstance(const std::wstring& instanceName) = 0;
    virtual IBusManagementCan* CreateObjectInstance(const std::wstring& instanceName) = 0;
};

class IBusManagementCan : public IBusManagement
{
  public:
    static const AttributeBits kBusStateBit = 0x4;
    static const AttributeBits kTxErrorCountBit = 0x8;
    static const AttributeBits kRxErrorCountBit = 0x10;
    static const AttributeBits kSendMessagesAsRxBit = 0x20;

    IBusManagementCan() {}
    ~IBusManagementCan() {}
    IBusManagementCan(const IBusManagementCan&) = delete;
    IBusManagementCan(IBusManagementCan&&) = delete;
    IBusManagementCan& operator=(const IBusManagementCan&) = delete;
    IBusManagementCan& operator=(IBusManagementCan&&) = delete;

    // attribute BusState : CanBusState
    virtual CanBusState GetBusState() const = 0;
    virtual void SetBusState(CanBusState newValue) = 0;
    // attribute TxErrorCount : HLAinteger32LE
    virtual int32_t GetTxErrorCount() const = 0;
    virtual void SetTxErrorCount(int32_t newValue) = 0;
    // attribute RxErrorCount : HLAinteger32LE
    virtual int32_t GetRxErrorCount() const = 0;
    virtual void SetRxErrorCount(int32_t newValue) = 0;
    // attribute SendMessagesAsRx : HLAboolean
    virtual bool GetSendMessagesAsRx() const = 0;
    virtual void SetSendMessagesAsRx(bool newValue) = 0;
    using UpdateCallbackType = std::function<void(IBusManagementCan*)>;
    virtual uint32_t RegisterUpdateCallback(UpdateCallbackType callback) = 0;
    virtual void UnregisterUpdateCallback(uint32_t callbackToken) = 0;
};

class IBusController;
class IBusControllerObjectClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe() = 0;
    virtual void Unsubscribe() = 0;
    virtual IBusController* GetObjectInstance(const std::wstring& instanceName) = 0;
    virtual IBusController* CreateObjectInstance(const std::wstring& instanceName) = 0;
};

class IBusController : public IHLAobjectRoot
{
  public:
    static const AttributeBits kNetworkIDBit = 0x2;
    static const AttributeBits kDeviceIDBit = 0x4;

    IBusController() {}
    ~IBusController() {}
    IBusController(const IBusController&) = delete;
    IBusController(IBusController&&) = delete;
    IBusController& operator=(const IBusController&) = delete;
    IBusController& operator=(IBusController&&) = delete;

    // attribute NetworkID : HLAASCIIstring
    virtual std::string GetNetworkID() const = 0;
    virtual void SetNetworkID(std::string newValue) = 0;
    // attribute DeviceID : HLAASCIIstring
    virtual std::string GetDeviceID() const = 0;
    virtual void SetDeviceID(std::string newValue) = 0;
    virtual void UpdateAllAttributeValues() = 0;
    virtual void UpdateAllAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual void UpdateModifiedAttributeValues() = 0;
    virtual void UpdateModifiedAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual AttributeBits GetUpdatedAttributes() const = 0;
};

class IBusControllerCan;
class IBusControllerCanObjectClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe() = 0;
    virtual void Unsubscribe() = 0;
    virtual IBusControllerCan* GetObjectInstance(const std::wstring& instanceName) = 0;
    virtual IBusControllerCan* CreateObjectInstance(const std::wstring& instanceName) = 0;
};

class IBusControllerCan : public IBusController
{
  public:
    static const AttributeBits kBaudRateBit = 0x8;
    static const AttributeBits kDataBaudRateBit = 0x10;
    static const AttributeBits kPreScalerBit = 0x20;
    static const AttributeBits kOperationModeBit = 0x40;
    static const AttributeBits kSync_SegBit = 0x80;
    static const AttributeBits kProp_SegBit = 0x100;
    static const AttributeBits kPhase_Seg1Bit = 0x200;
    static const AttributeBits kPhase_Seg2Bit = 0x400;
    static const AttributeBits kSamplingModeBit = 0x800;

    IBusControllerCan() {}
    ~IBusControllerCan() {}
    IBusControllerCan(const IBusControllerCan&) = delete;
    IBusControllerCan(IBusControllerCan&&) = delete;
    IBusControllerCan& operator=(const IBusControllerCan&) = delete;
    IBusControllerCan& operator=(IBusControllerCan&&) = delete;

    // attribute BaudRate : HLAinteger32LE
    virtual int32_t GetBaudRate() const = 0;
    virtual void SetBaudRate(int32_t newValue) = 0;
    // attribute DataBaudRate : HLAinteger32LE
    virtual int32_t GetDataBaudRate() const = 0;
    virtual void SetDataBaudRate(int32_t newValue) = 0;
    // attribute PreScaler : HLAinteger32LE
    virtual int32_t GetPreScaler() const = 0;
    virtual void SetPreScaler(int32_t newValue) = 0;
    // attribute OperationMode : CanOperationMode
    virtual CanOperationMode GetOperationMode() const = 0;
    virtual void SetOperationMode(CanOperationMode newValue) = 0;
    // attribute Sync_Seg : HLAinteger32LE
    virtual int32_t GetSync_Seg() const = 0;
    virtual void SetSync_Seg(int32_t newValue) = 0;
    // attribute Prop_Seg : HLAinteger32LE
    virtual int32_t GetProp_Seg() const = 0;
    virtual void SetProp_Seg(int32_t newValue) = 0;
    // attribute Phase_Seg1 : HLAinteger32LE
    virtual int32_t GetPhase_Seg1() const = 0;
    virtual void SetPhase_Seg1(int32_t newValue) = 0;
    // attribute Phase_Seg2 : HLAinteger32LE
    virtual int32_t GetPhase_Seg2() const = 0;
    virtual void SetPhase_Seg2(int32_t newValue) = 0;
    // attribute SamplingMode : CanSamplingMode
    virtual CanSamplingMode GetSamplingMode() const = 0;
    virtual void SetSamplingMode(CanSamplingMode newValue) = 0;
    using UpdateCallbackType = std::function<void(IBusControllerCan*)>;
    virtual uint32_t RegisterUpdateCallback(UpdateCallbackType callback) = 0;
    virtual void UnregisterUpdateCallback(uint32_t callbackToken) = 0;
};


class IClassRegistry
{
  public:
    virtual IHLAobjectRootObjectClass* getHLAobjectRootObjectClass() const = 0;
    virtual ISystemVariableObjectClass* getSystemVariableObjectClass() const = 0;
    virtual IValueEntityObjectClass* getValueEntityObjectClass() const = 0;
    virtual IDOMemberSourceObjectClass* getDOMemberSourceObjectClass() const = 0;
    virtual IDOMemberTargetObjectClass* getDOMemberTargetObjectClass() const = 0;
    virtual IBusManagementObjectClass* getBusManagementObjectClass() const = 0;
    virtual IBusManagementCanObjectClass* getBusManagementCanObjectClass() const = 0;
    virtual IBusControllerObjectClass* getBusControllerObjectClass() const = 0;
    virtual IBusControllerCanObjectClass* getBusControllerCanObjectClass() const = 0;
  protected:
    virtual ~IClassRegistry() {}
}; // class ClassRegistry

IClassRegistry* GetClassRegistry();

} // namespace NDistSimIB
} // namespace NRTFederateEncoding
