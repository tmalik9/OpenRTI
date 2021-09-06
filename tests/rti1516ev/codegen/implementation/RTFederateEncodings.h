
// Code automatically generated by FOMCodeGen version 2.0.1.18 from RTFederate.xml, do not edit

#pragma once

#include <vector>
#include <cstring>

#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAfixedRecord.h"
#include "RTI/encoding/HLAfixedArray.h"
#include "RTI/encoding/HLAvariableArray.h"

#include "interface/RTFederateDataTypes.h"

namespace NDistSimIB {
namespace NRTFederateEncoding {



// simple data type VTimeNS
using VTimeNSEncoding=rti1516ev::HLAinteger64LE;
// simple data type VMessageId
using VMessageIdEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type BusType
class BusTypeEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    BusTypeEncoding() : rti1516ev::HLAinteger32LE() {}
    BusTypeEncoding(BusType value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    BusTypeEncoding(const BusTypeEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    BusType get() const { return static_cast<BusType>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(BusType value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type SimulatedFlag
class SimulatedFlagEncoding : public rti1516ev::HLAoctet
{
  public:
    SimulatedFlagEncoding() : rti1516ev::HLAoctet() {}
    SimulatedFlagEncoding(SimulatedFlag value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    SimulatedFlagEncoding(const SimulatedFlagEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    SimulatedFlag get() const { return static_cast<SimulatedFlag>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(SimulatedFlag value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type DirMask
class DirMaskEncoding : public rti1516ev::HLAoctet
{
  public:
    DirMaskEncoding() : rti1516ev::HLAoctet() {}
    DirMaskEncoding(DirMask value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    DirMaskEncoding(const DirMaskEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    DirMask get() const { return static_cast<DirMask>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(DirMask value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type MessageFlags
class MessageFlagsEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    MessageFlagsEncoding() : rti1516ev::HLAinteger32LE() {}
    MessageFlagsEncoding(MessageFlags value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    MessageFlagsEncoding(const MessageFlagsEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    MessageFlags get() const { return static_cast<MessageFlags>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(MessageFlags value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type LinkStatus
class LinkStatusEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    LinkStatusEncoding() : rti1516ev::HLAinteger32LE() {}
    LinkStatusEncoding(LinkStatus value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    LinkStatusEncoding(const LinkStatusEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    LinkStatus get() const { return static_cast<LinkStatus>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(LinkStatus value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type EthernetPhy
class EthernetPhyEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    EthernetPhyEncoding() : rti1516ev::HLAinteger32LE() {}
    EthernetPhyEncoding(EthernetPhy value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    EthernetPhyEncoding(const EthernetPhyEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    EthernetPhy get() const { return static_cast<EthernetPhy>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(EthernetPhy value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type Duplex
class DuplexEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    DuplexEncoding() : rti1516ev::HLAinteger32LE() {}
    DuplexEncoding(Duplex value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    DuplexEncoding(const DuplexEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    Duplex get() const { return static_cast<Duplex>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(Duplex value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type MdiType
class MdiTypeEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    MdiTypeEncoding() : rti1516ev::HLAinteger32LE() {}
    MdiTypeEncoding(MdiType value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    MdiTypeEncoding(const MdiTypeEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    MdiType get() const { return static_cast<MdiType>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(MdiType value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type Connector
class ConnectorEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    ConnectorEncoding() : rti1516ev::HLAinteger32LE() {}
    ConnectorEncoding(Connector value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    ConnectorEncoding(const ConnectorEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    Connector get() const { return static_cast<Connector>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(Connector value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type ClockMode
class ClockModeEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    ClockModeEncoding() : rti1516ev::HLAinteger32LE() {}
    ClockModeEncoding(ClockMode value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    ClockModeEncoding(const ClockModeEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    ClockMode get() const { return static_cast<ClockMode>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(ClockMode value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type BrPair
class BrPairEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    BrPairEncoding() : rti1516ev::HLAinteger32LE() {}
    BrPairEncoding(BrPair value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    BrPairEncoding(const BrPairEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    BrPair get() const { return static_cast<BrPair>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(BrPair value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type CanOperationMode
class CanOperationModeEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    CanOperationModeEncoding() : rti1516ev::HLAinteger32LE() {}
    CanOperationModeEncoding(CanOperationMode value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    CanOperationModeEncoding(const CanOperationModeEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    CanOperationMode get() const { return static_cast<CanOperationMode>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(CanOperationMode value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type CanSamplingMode
class CanSamplingModeEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    CanSamplingModeEncoding() : rti1516ev::HLAinteger32LE() {}
    CanSamplingModeEncoding(CanSamplingMode value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    CanSamplingModeEncoding(const CanSamplingModeEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    CanSamplingMode get() const { return static_cast<CanSamplingMode>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(CanSamplingMode value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type CanBusState
class CanBusStateEncoding : public rti1516ev::HLAinteger32LE
{
  public:
    CanBusStateEncoding() : rti1516ev::HLAinteger32LE() {}
    CanBusStateEncoding(CanBusState value) : rti1516ev::HLAinteger32LE(static_cast<int32_t>(value)) {}
    CanBusStateEncoding(const CanBusStateEncoding& ref) : rti1516ev::HLAinteger32LE(ref) {}
    using rti1516ev::HLAinteger32LE::get;
    CanBusState get() const { return static_cast<CanBusState>(rti1516ev::HLAinteger32LE::get()); }
    using rti1516ev::HLAinteger32LE::set;
    void set(CanBusState value) { rti1516ev::HLAinteger32LE::set(static_cast<int32_t>(value)); }
};

// enumerated data type FlexRayChannel
class FlexRayChannelEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRayChannelEncoding() : rti1516ev::HLAoctet() {}
    FlexRayChannelEncoding(FlexRayChannel value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRayChannelEncoding(const FlexRayChannelEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRayChannel get() const { return static_cast<FlexRayChannel>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRayChannel value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type FlexRaySymbolPattern
class FlexRaySymbolPatternEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRaySymbolPatternEncoding() : rti1516ev::HLAoctet() {}
    FlexRaySymbolPatternEncoding(FlexRaySymbolPattern value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRaySymbolPatternEncoding(const FlexRaySymbolPatternEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRaySymbolPattern get() const { return static_cast<FlexRaySymbolPattern>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRaySymbolPattern value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type FlexRayChiCommand
class FlexRayChiCommandEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRayChiCommandEncoding() : rti1516ev::HLAoctet() {}
    FlexRayChiCommandEncoding(FlexRayChiCommand value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRayChiCommandEncoding(const FlexRayChiCommandEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRayChiCommand get() const { return static_cast<FlexRayChiCommand>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRayChiCommand value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type FlexRayTransmissionMode
class FlexRayTransmissionModeEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRayTransmissionModeEncoding() : rti1516ev::HLAoctet() {}
    FlexRayTransmissionModeEncoding(FlexRayTransmissionMode value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRayTransmissionModeEncoding(const FlexRayTransmissionModeEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRayTransmissionMode get() const { return static_cast<FlexRayTransmissionMode>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRayTransmissionMode value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type FlexRayPocState
class FlexRayPocStateEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRayPocStateEncoding() : rti1516ev::HLAoctet() {}
    FlexRayPocStateEncoding(FlexRayPocState value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRayPocStateEncoding(const FlexRayPocStateEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRayPocState get() const { return static_cast<FlexRayPocState>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRayPocState value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type FlexRaySlotModeType
class FlexRaySlotModeTypeEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRaySlotModeTypeEncoding() : rti1516ev::HLAoctet() {}
    FlexRaySlotModeTypeEncoding(FlexRaySlotModeType value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRaySlotModeTypeEncoding(const FlexRaySlotModeTypeEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRaySlotModeType get() const { return static_cast<FlexRaySlotModeType>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRaySlotModeType value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type FlexRayErrorModeType
class FlexRayErrorModeTypeEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRayErrorModeTypeEncoding() : rti1516ev::HLAoctet() {}
    FlexRayErrorModeTypeEncoding(FlexRayErrorModeType value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRayErrorModeTypeEncoding(const FlexRayErrorModeTypeEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRayErrorModeType get() const { return static_cast<FlexRayErrorModeType>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRayErrorModeType value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type FlexRayStartupStateType
class FlexRayStartupStateTypeEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRayStartupStateTypeEncoding() : rti1516ev::HLAoctet() {}
    FlexRayStartupStateTypeEncoding(FlexRayStartupStateType value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRayStartupStateTypeEncoding(const FlexRayStartupStateTypeEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRayStartupStateType get() const { return static_cast<FlexRayStartupStateType>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRayStartupStateType value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type FlexRayWakeupStatusType
class FlexRayWakeupStatusTypeEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRayWakeupStatusTypeEncoding() : rti1516ev::HLAoctet() {}
    FlexRayWakeupStatusTypeEncoding(FlexRayWakeupStatusType value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRayWakeupStatusTypeEncoding(const FlexRayWakeupStatusTypeEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRayWakeupStatusType get() const { return static_cast<FlexRayWakeupStatusType>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRayWakeupStatusType value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// enumerated data type FlexRayClockPeriod
class FlexRayClockPeriodEncoding : public rti1516ev::HLAoctet
{
  public:
    FlexRayClockPeriodEncoding() : rti1516ev::HLAoctet() {}
    FlexRayClockPeriodEncoding(FlexRayClockPeriod value) : rti1516ev::HLAoctet(static_cast<uint8_t>(value)) {}
    FlexRayClockPeriodEncoding(const FlexRayClockPeriodEncoding& ref) : rti1516ev::HLAoctet(ref) {}
    using rti1516ev::HLAoctet::get;
    FlexRayClockPeriod get() const { return static_cast<FlexRayClockPeriod>(rti1516ev::HLAoctet::get()); }
    using rti1516ev::HLAoctet::set;
    void set(FlexRayClockPeriod value) { rti1516ev::HLAoctet::set(static_cast<uint8_t>(value)); }
};

// 
// Bytes carried in a CAN frame. Note that the actual number of bytes is carried in
// a distinct member field of the CANFrame.
//         
// fixed array of simple type HLAbyte
class CANMessageDataEncoding : public rti1516ev::HLAfixedArray
{
  public:
    CANMessageDataEncoding();
    CANMessageDataEncoding(const CANMessageDataEncoding& rhs);
    CANMessageDataEncoding(const std::vector<uint8_t>& data);
    using rti1516ev::HLAfixedArray::set;
    void set(const std::vector<uint8_t>& data);
    void set(const uint8_t* data, size_t size);
    using rti1516ev::HLAfixedArray::get;
    const std::vector<uint8_t>& get() const;
  private:
    std::vector<uint8_t> mData;
    rti1516ev::HLAbyte mEncoding[64];
}; // class CANMessageData
// 
// Bytes carried in a Ethernet Packet. The data is variable-sized and carries the
// actual number of bytes used.
//         
// variable array of simple type HLAbyte
class EthernetPacketDataEncoding : public rti1516ev::HLAvariableArray
{
  public:
    EthernetPacketDataEncoding();
    EthernetPacketDataEncoding(const EthernetPacketDataEncoding& rhs);
    EthernetPacketDataEncoding(const std::vector<uint8_t>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<uint8_t>& data);
    void set(const uint8_t* data, size_t size);
    using rti1516ev::HLAvariableArray::get;
    const std::vector<uint8_t>& get() const;
    void resize(size_t size);
    using rti1516ev::HLAvariableArray::decodeFrom;
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<uint8_t> mData;
    std::vector<rti1516ev::HLAbyte> mEncoding;
}; // class EthernetPacketData

// Bytes carried in a FlexRay Frame. The data is variable-sized and carries the actual number of bytes used.
// variable array of simple type HLAbyte
class FlexRayPayloadDataEncoding : public rti1516ev::HLAvariableArray
{
  public:
    FlexRayPayloadDataEncoding();
    FlexRayPayloadDataEncoding(const FlexRayPayloadDataEncoding& rhs);
    FlexRayPayloadDataEncoding(const std::vector<uint8_t>& data);
    using rti1516ev::HLAvariableArray::set;
    void set(const std::vector<uint8_t>& data);
    void set(const uint8_t* data, size_t size);
    using rti1516ev::HLAvariableArray::get;
    const std::vector<uint8_t>& get() const;
    void resize(size_t size);
    using rti1516ev::HLAvariableArray::decodeFrom;
    virtual size_t decodeFrom(const rti1516ev::Octet* buffer, size_t bufferSize, size_t index) override;
  private:
    void update();
    std::vector<uint8_t> mData;
    std::vector<rti1516ev::HLAbyte> mEncoding;
}; // class FlexRayPayloadData

// CAN Frame
class CANFrameEncoding : public CANFrame, public rti1516ev::HLAfixedRecord
{
  public:
    CANFrameEncoding();
    CANFrameEncoding(const CANFrameEncoding& ref);
    virtual ~CANFrameEncoding();
    uint32_t getVersion() const override;
    void SetClientIndex(int16_t value) override;
    int16_t GetClientIndex() const override;
    bool IsClientIndexAvailable() const override;
    void SetDir(DirMask value) override;
    DirMask GetDir() const override;
    bool IsDirAvailable() const override;
    void SetOriginalTimeStamp(VTimeNS value) override;
    VTimeNS GetOriginalTimeStamp() const override;
    bool IsOriginalTimeStampAvailable() const override;
    void SetSimulated(SimulatedFlag value) override;
    SimulatedFlag GetSimulated() const override;
    bool IsSimulatedAvailable() const override;
    void SetId(VMessageId value) override;
    VMessageId GetId() const override;
    bool IsIdAvailable() const override;
    void SetFlags(int32_t value) override;
    int32_t GetFlags() const override;
    bool IsFlagsAvailable() const override;
    void SetDataLength(uint8_t value) override;
    uint8_t GetDataLength() const override;
    bool IsDataLengthAvailable() const override;
    void SetData(const std::vector<uint8_t>& value) override;
    void SetData(const uint8_t* value, size_t size) override;
    const std::vector<uint8_t>& GetData() const override;
    bool IsDataAvailable() const override;
    void SetFrameLengthNS(int32_t value) override;
    int32_t GetFrameLengthNS() const override;
    bool IsFrameLengthNSAvailable() const override;
    void SetBitCount(int16_t value) override;
    int16_t GetBitCount() const override;
    bool IsBitCountAvailable() const override;
    CANFrameEncoding& operator=(const CANFrame& ref);
  private:
    rti1516ev::HLAinteger16LE mClientIndex;
    rti1516ev::HLAoctet mDir;
    rti1516ev::HLAinteger64LE mOriginalTimeStamp;
    rti1516ev::HLAoctet mSimulated;
    rti1516ev::HLAinteger32LE mId;
    rti1516ev::HLAinteger32LE mFlags;
    rti1516ev::HLAoctet mDataLength;
    CANMessageDataEncoding mData;
    rti1516ev::HLAinteger32LE mFrameLengthNS;
    rti1516ev::HLAinteger16LE mBitCount;
}; // class CANFrameEncoding

// CAN Error Frame
class CANErrorFrameEncoding : public CANErrorFrame, public rti1516ev::HLAfixedRecord
{
  public:
    CANErrorFrameEncoding();
    CANErrorFrameEncoding(const CANErrorFrameEncoding& ref);
    virtual ~CANErrorFrameEncoding();
    uint32_t getVersion() const override;
    void SetErrorBitPosition(int16_t value) override;
    int16_t GetErrorBitPosition() const override;
    bool IsErrorBitPositionAvailable() const override;
    void SetFrameLengthNS(int32_t value) override;
    int32_t GetFrameLengthNS() const override;
    bool IsFrameLengthNSAvailable() const override;
    void SetClientIndex(int16_t value) override;
    int16_t GetClientIndex() const override;
    bool IsClientIndexAvailable() const override;
    CANErrorFrameEncoding& operator=(const CANErrorFrame& ref);
  private:
    rti1516ev::HLAinteger16LE mErrorBitPosition;
    rti1516ev::HLAinteger32LE mFrameLengthNS;
    rti1516ev::HLAinteger16LE mClientIndex;
}; // class CANErrorFrameEncoding

// Ethernet Status Event
class EthernetStatusEncoding : public EthernetStatus, public rti1516ev::HLAfixedRecord
{
  public:
    EthernetStatusEncoding();
    EthernetStatusEncoding(const EthernetStatusEncoding& ref);
    virtual ~EthernetStatusEncoding();
    uint32_t getVersion() const override;
    void SetBusType(int16_t value) override;
    int16_t GetBusType() const override;
    bool IsBusTypeAvailable() const override;
    void SetHardwareChannel(int64_t value) override;
    int64_t GetHardwareChannel() const override;
    bool IsHardwareChannelAvailable() const override;
    void SetLinkStatus(LinkStatus value) override;
    LinkStatus GetLinkStatus() const override;
    bool IsLinkStatusAvailable() const override;
    void SetBitrate(int32_t value) override;
    int32_t GetBitrate() const override;
    bool IsBitrateAvailable() const override;
    void SetEthernetPhy(EthernetPhy value) override;
    EthernetPhy GetEthernetPhy() const override;
    bool IsEthernetPhyAvailable() const override;
    void SetDuplex(Duplex value) override;
    Duplex GetDuplex() const override;
    bool IsDuplexAvailable() const override;
    void SetMdiType(MdiType value) override;
    MdiType GetMdiType() const override;
    bool IsMdiTypeAvailable() const override;
    void SetConnector(Connector value) override;
    Connector GetConnector() const override;
    bool IsConnectorAvailable() const override;
    void SetClockMode(ClockMode value) override;
    ClockMode GetClockMode() const override;
    bool IsClockModeAvailable() const override;
    void SetBrPair(BrPair value) override;
    BrPair GetBrPair() const override;
    bool IsBrPairAvailable() const override;
    void SetDeviceOperationMode(int32_t value) override;
    int32_t GetDeviceOperationMode() const override;
    bool IsDeviceOperationModeAvailable() const override;
    EthernetStatusEncoding& operator=(const EthernetStatus& ref);
  private:
    rti1516ev::HLAinteger16LE mBusType;
    rti1516ev::HLAinteger64LE mHardwareChannel;
    rti1516ev::HLAinteger32LE mLinkStatus;
    rti1516ev::HLAinteger32LE mBitrate;
    rti1516ev::HLAinteger32LE mEthernetPhy;
    rti1516ev::HLAinteger32LE mDuplex;
    rti1516ev::HLAinteger32LE mMdiType;
    rti1516ev::HLAinteger32LE mConnector;
    rti1516ev::HLAinteger32LE mClockMode;
    rti1516ev::HLAinteger32LE mBrPair;
    rti1516ev::HLAinteger32LE mDeviceOperationMode;
}; // class EthernetStatusEncoding

// Ethernet Packet
class EthernetPacketEncoding : public EthernetPacket, public rti1516ev::HLAfixedRecord
{
  public:
    EthernetPacketEncoding();
    EthernetPacketEncoding(const EthernetPacketEncoding& ref);
    virtual ~EthernetPacketEncoding();
    uint32_t getVersion() const override;
    void SetClientIndex(int16_t value) override;
    int16_t GetClientIndex() const override;
    bool IsClientIndexAvailable() const override;
    void SetDir(DirMask value) override;
    DirMask GetDir() const override;
    bool IsDirAvailable() const override;
    void SetOriginalTimeStamp(VTimeNS value) override;
    VTimeNS GetOriginalTimeStamp() const override;
    bool IsOriginalTimeStampAvailable() const override;
    void SetSimulated(SimulatedFlag value) override;
    SimulatedFlag GetSimulated() const override;
    bool IsSimulatedAvailable() const override;
    void SetBusType(int16_t value) override;
    int16_t GetBusType() const override;
    bool IsBusTypeAvailable() const override;
    void SetHardwareChannel(int64_t value) override;
    int64_t GetHardwareChannel() const override;
    bool IsHardwareChannelAvailable() const override;
    void SetFrameDuration(VTimeNS value) override;
    VTimeNS GetFrameDuration() const override;
    bool IsFrameDurationAvailable() const override;
    void SetEthernetChecksum(int32_t value) override;
    int32_t GetEthernetChecksum() const override;
    bool IsEthernetChecksumAvailable() const override;
    void SetPacketData(const std::vector<uint8_t>& value) override;
    void SetPacketData(const uint8_t* value, size_t size) override;
    const std::vector<uint8_t>& GetPacketData() const override;
    bool IsPacketDataAvailable() const override;
    EthernetPacketEncoding& operator=(const EthernetPacket& ref);
  private:
    rti1516ev::HLAinteger16LE mClientIndex;
    rti1516ev::HLAoctet mDir;
    rti1516ev::HLAinteger64LE mOriginalTimeStamp;
    rti1516ev::HLAoctet mSimulated;
    rti1516ev::HLAinteger16LE mBusType;
    rti1516ev::HLAinteger64LE mHardwareChannel;
    rti1516ev::HLAinteger64LE mFrameDuration;
    rti1516ev::HLAinteger32LE mEthernetChecksum;
    EthernetPacketDataEncoding mPacketData;
}; // class EthernetPacketEncoding

class EthernetPacketForwardedEncoding : public EthernetPacketForwarded, public rti1516ev::HLAfixedRecord
{
  public:
    EthernetPacketForwardedEncoding();
    EthernetPacketForwardedEncoding(const EthernetPacketForwardedEncoding& ref);
    virtual ~EthernetPacketForwardedEncoding();
    uint32_t getVersion() const override;
    void SetClientIndex(int16_t value) override;
    int16_t GetClientIndex() const override;
    bool IsClientIndexAvailable() const override;
    void SetDir(DirMask value) override;
    DirMask GetDir() const override;
    bool IsDirAvailable() const override;
    void SetOriginalTimeStamp(VTimeNS value) override;
    VTimeNS GetOriginalTimeStamp() const override;
    bool IsOriginalTimeStampAvailable() const override;
    void SetSimulated(SimulatedFlag value) override;
    SimulatedFlag GetSimulated() const override;
    bool IsSimulatedAvailable() const override;
    void SetBusType(int16_t value) override;
    int16_t GetBusType() const override;
    bool IsBusTypeAvailable() const override;
    void SetHardwareChannel(int64_t value) override;
    int64_t GetHardwareChannel() const override;
    bool IsHardwareChannelAvailable() const override;
    void SetFrameDuration(VTimeNS value) override;
    VTimeNS GetFrameDuration() const override;
    bool IsFrameDurationAvailable() const override;
    void SetEthernetChecksum(int32_t value) override;
    int32_t GetEthernetChecksum() const override;
    bool IsEthernetChecksumAvailable() const override;
    void SetPacketData(const std::vector<uint8_t>& value) override;
    void SetPacketData(const uint8_t* value, size_t size) override;
    const std::vector<uint8_t>& GetPacketData() const override;
    bool IsPacketDataAvailable() const override;
    EthernetPacketForwardedEncoding& operator=(const EthernetPacketForwarded& ref);
  private:
    rti1516ev::HLAinteger16LE mClientIndex;
    rti1516ev::HLAoctet mDir;
    rti1516ev::HLAinteger64LE mOriginalTimeStamp;
    rti1516ev::HLAoctet mSimulated;
    rti1516ev::HLAinteger16LE mBusType;
    rti1516ev::HLAinteger64LE mHardwareChannel;
    rti1516ev::HLAinteger64LE mFrameDuration;
    rti1516ev::HLAinteger32LE mEthernetChecksum;
    EthernetPacketDataEncoding mPacketData;
}; // class EthernetPacketForwardedEncoding

class EthernetPacketErrorEncoding : public EthernetPacketError, public rti1516ev::HLAfixedRecord
{
  public:
    EthernetPacketErrorEncoding();
    EthernetPacketErrorEncoding(const EthernetPacketErrorEncoding& ref);
    virtual ~EthernetPacketErrorEncoding();
    uint32_t getVersion() const override;
    void SetClientIndex(int16_t value) override;
    int16_t GetClientIndex() const override;
    bool IsClientIndexAvailable() const override;
    void SetDir(DirMask value) override;
    DirMask GetDir() const override;
    bool IsDirAvailable() const override;
    void SetOriginalTimeStamp(VTimeNS value) override;
    VTimeNS GetOriginalTimeStamp() const override;
    bool IsOriginalTimeStampAvailable() const override;
    void SetSimulated(SimulatedFlag value) override;
    SimulatedFlag GetSimulated() const override;
    bool IsSimulatedAvailable() const override;
    void SetBusType(int16_t value) override;
    int16_t GetBusType() const override;
    bool IsBusTypeAvailable() const override;
    void SetHardwareChannel(int64_t value) override;
    int64_t GetHardwareChannel() const override;
    bool IsHardwareChannelAvailable() const override;
    void SetFrameDuration(VTimeNS value) override;
    VTimeNS GetFrameDuration() const override;
    bool IsFrameDurationAvailable() const override;
    void SetErrorCode(int32_t value) override;
    int32_t GetErrorCode() const override;
    bool IsErrorCodeAvailable() const override;
    void SetEthernetChecksum(int32_t value) override;
    int32_t GetEthernetChecksum() const override;
    bool IsEthernetChecksumAvailable() const override;
    void SetPacketData(const std::vector<uint8_t>& value) override;
    void SetPacketData(const uint8_t* value, size_t size) override;
    const std::vector<uint8_t>& GetPacketData() const override;
    bool IsPacketDataAvailable() const override;
    EthernetPacketErrorEncoding& operator=(const EthernetPacketError& ref);
  private:
    rti1516ev::HLAinteger16LE mClientIndex;
    rti1516ev::HLAoctet mDir;
    rti1516ev::HLAinteger64LE mOriginalTimeStamp;
    rti1516ev::HLAoctet mSimulated;
    rti1516ev::HLAinteger16LE mBusType;
    rti1516ev::HLAinteger64LE mHardwareChannel;
    rti1516ev::HLAinteger64LE mFrameDuration;
    rti1516ev::HLAinteger32LE mErrorCode;
    rti1516ev::HLAinteger32LE mEthernetChecksum;
    EthernetPacketDataEncoding mPacketData;
}; // class EthernetPacketErrorEncoding

class EthernetPacketErrorForwardedEncoding : public EthernetPacketErrorForwarded, public rti1516ev::HLAfixedRecord
{
  public:
    EthernetPacketErrorForwardedEncoding();
    EthernetPacketErrorForwardedEncoding(const EthernetPacketErrorForwardedEncoding& ref);
    virtual ~EthernetPacketErrorForwardedEncoding();
    uint32_t getVersion() const override;
    void SetClientIndex(int16_t value) override;
    int16_t GetClientIndex() const override;
    bool IsClientIndexAvailable() const override;
    void SetDir(DirMask value) override;
    DirMask GetDir() const override;
    bool IsDirAvailable() const override;
    void SetOriginalTimeStamp(VTimeNS value) override;
    VTimeNS GetOriginalTimeStamp() const override;
    bool IsOriginalTimeStampAvailable() const override;
    void SetSimulated(SimulatedFlag value) override;
    SimulatedFlag GetSimulated() const override;
    bool IsSimulatedAvailable() const override;
    void SetBusType(int16_t value) override;
    int16_t GetBusType() const override;
    bool IsBusTypeAvailable() const override;
    void SetHardwareChannel(int64_t value) override;
    int64_t GetHardwareChannel() const override;
    bool IsHardwareChannelAvailable() const override;
    void SetFrameDuration(VTimeNS value) override;
    VTimeNS GetFrameDuration() const override;
    bool IsFrameDurationAvailable() const override;
    void SetErrorCode(int32_t value) override;
    int32_t GetErrorCode() const override;
    bool IsErrorCodeAvailable() const override;
    void SetEthernetChecksum(int32_t value) override;
    int32_t GetEthernetChecksum() const override;
    bool IsEthernetChecksumAvailable() const override;
    void SetPacketData(const std::vector<uint8_t>& value) override;
    void SetPacketData(const uint8_t* value, size_t size) override;
    const std::vector<uint8_t>& GetPacketData() const override;
    bool IsPacketDataAvailable() const override;
    EthernetPacketErrorForwardedEncoding& operator=(const EthernetPacketErrorForwarded& ref);
  private:
    rti1516ev::HLAinteger16LE mClientIndex;
    rti1516ev::HLAoctet mDir;
    rti1516ev::HLAinteger64LE mOriginalTimeStamp;
    rti1516ev::HLAoctet mSimulated;
    rti1516ev::HLAinteger16LE mBusType;
    rti1516ev::HLAinteger64LE mHardwareChannel;
    rti1516ev::HLAinteger64LE mFrameDuration;
    rti1516ev::HLAinteger32LE mErrorCode;
    rti1516ev::HLAinteger32LE mEthernetChecksum;
    EthernetPacketDataEncoding mPacketData;
}; // class EthernetPacketErrorForwardedEncoding

// Payload of FlexRay Frame
class FlexRayPayloadEncoding : public FlexRayPayload, public rti1516ev::HLAfixedRecord
{
  public:
    FlexRayPayloadEncoding();
    FlexRayPayloadEncoding(const FlexRayPayloadEncoding& ref);
    virtual ~FlexRayPayloadEncoding();
    uint32_t getVersion() const override;
    void SetClientIndex(int16_t value) override;
    int16_t GetClientIndex() const override;
    bool IsClientIndexAvailable() const override;
    void SetDir(DirMask value) override;
    DirMask GetDir() const override;
    bool IsDirAvailable() const override;
    void SetOriginalTimeStamp(VTimeNS value) override;
    VTimeNS GetOriginalTimeStamp() const override;
    bool IsOriginalTimeStampAvailable() const override;
    void SetSimulated(SimulatedFlag value) override;
    SimulatedFlag GetSimulated() const override;
    bool IsSimulatedAvailable() const override;
    void SetPayloadData(const std::vector<uint8_t>& value) override;
    void SetPayloadData(const uint8_t* value, size_t size) override;
    const std::vector<uint8_t>& GetPayloadData() const override;
    bool IsPayloadDataAvailable() const override;
    void SetPayloadValid(bool value) override;
    bool GetPayloadValid() const override;
    bool IsPayloadValidAvailable() const override;
    FlexRayPayloadEncoding& operator=(const FlexRayPayload& ref);
  private:
    rti1516ev::HLAinteger16LE mClientIndex;
    rti1516ev::HLAoctet mDir;
    rti1516ev::HLAinteger64LE mOriginalTimeStamp;
    rti1516ev::HLAoctet mSimulated;
    FlexRayPayloadDataEncoding mPayloadData;
    rti1516ev::HLAboolean mPayloadValid;
}; // class FlexRayPayloadEncoding

// Header of FlexRay Frame
class FlexRayHeaderEncoding : public FlexRayHeader, public rti1516ev::HLAfixedRecord
{
  public:
    FlexRayHeaderEncoding();
    FlexRayHeaderEncoding(const FlexRayHeaderEncoding& ref);
    virtual ~FlexRayHeaderEncoding();
    uint32_t getVersion() const override;
    void SetSuFindicator(bool value) override;
    bool GetSuFindicator() const override;
    bool IsSuFindicatorAvailable() const override;
    void SetSyFIndicator(bool value) override;
    bool GetSyFIndicator() const override;
    bool IsSyFIndicatorAvailable() const override;
    void SetNFIndicator(bool value) override;
    bool GetNFIndicator() const override;
    bool IsNFIndicatorAvailable() const override;
    void SetPPIndicator(bool value) override;
    bool GetPPIndicator() const override;
    bool IsPPIndicatorAvailable() const override;
    void SetFrameID(int16_t value) override;
    int16_t GetFrameID() const override;
    bool IsFrameIDAvailable() const override;
    void SetpayloadLength(uint8_t value) override;
    uint8_t GetpayloadLength() const override;
    bool IspayloadLengthAvailable() const override;
    void SetHeaderCRC(int16_t value) override;
    int16_t GetHeaderCRC() const override;
    bool IsHeaderCRCAvailable() const override;
    void SetCycle(uint8_t value) override;
    uint8_t GetCycle() const override;
    bool IsCycleAvailable() const override;
    FlexRayHeaderEncoding& operator=(const FlexRayHeader& ref);
  private:
    rti1516ev::HLAboolean mSuFindicator;
    rti1516ev::HLAboolean mSyFIndicator;
    rti1516ev::HLAboolean mNFIndicator;
    rti1516ev::HLAboolean mPPIndicator;
    rti1516ev::HLAinteger16LE mFrameID;
    rti1516ev::HLAoctet mpayloadLength;
    rti1516ev::HLAinteger16LE mHeaderCRC;
    rti1516ev::HLAoctet mCycle;
}; // class FlexRayHeaderEncoding

// FlexRay Frame
class FlexRayFrameEncoding : public FlexRayFrame, public rti1516ev::HLAfixedRecord
{
  public:
    FlexRayFrameEncoding();
    FlexRayFrameEncoding(const FlexRayFrameEncoding& ref);
    virtual ~FlexRayFrameEncoding();
    uint32_t getVersion() const override;
    void SetClientIndex(int16_t value) override;
    int16_t GetClientIndex() const override;
    bool IsClientIndexAvailable() const override;
    void SetDir(DirMask value) override;
    DirMask GetDir() const override;
    bool IsDirAvailable() const override;
    void SetOriginalTimeStamp(VTimeNS value) override;
    VTimeNS GetOriginalTimeStamp() const override;
    bool IsOriginalTimeStampAvailable() const override;
    void SetSimulated(SimulatedFlag value) override;
    SimulatedFlag GetSimulated() const override;
    bool IsSimulatedAvailable() const override;
    void SetPayloadData(const std::vector<uint8_t>& value) override;
    void SetPayloadData(const uint8_t* value, size_t size) override;
    const std::vector<uint8_t>& GetPayloadData() const override;
    bool IsPayloadDataAvailable() const override;
    void SetPayloadValid(bool value) override;
    bool GetPayloadValid() const override;
    bool IsPayloadValidAvailable() const override;
    FlexRayFrameEncoding& operator=(const FlexRayFrame& ref);
  private:
    rti1516ev::HLAinteger16LE mClientIndex;
    rti1516ev::HLAoctet mDir;
    rti1516ev::HLAinteger64LE mOriginalTimeStamp;
    rti1516ev::HLAoctet mSimulated;
    FlexRayPayloadDataEncoding mPayloadData;
    rti1516ev::HLAboolean mPayloadValid;
}; // class FlexRayFrameEncoding

} // namespace NDistSimIB
} // namespace NRTFederateEncoding

