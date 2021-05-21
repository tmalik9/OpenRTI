
// Code automatically generated from D:\vfs\OpenRTI-codegen\tests\rti1516ev\codegen\\RTFederate.xml, do not edit

#pragma once

#include <vector>
#include <cstring>

#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAfixedRecord.h"
#include "RTI/encoding/HLAfixedArray.h"
#include "RTI/encoding/HLAvariableArray.h"
#include "RTI/encoding/HLAopaqueData.h"

#include "RTFederateDataTypes.h"

namespace NDistSimIB {
namespace NRTFederateEncoding {



// simple data type VTimeNS
using VTimeNSEncoding=rti1516ev::HLAinteger64LE;
// simple data type VMessageId
using VMessageIdEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type BusType
using BusTypeEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type SimulatedFlag
using SimulatedFlagEncoding=rti1516ev::HLAoctet;
// enumerated data type DirMask
using DirMaskEncoding=rti1516ev::HLAoctet;
// enumerated data type MessageFlags
using MessageFlagsEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type LinkStatus
using LinkStatusEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type EthernetPhy
using EthernetPhyEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type Duplex
using DuplexEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type MdiType
using MdiTypeEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type Connector
using ConnectorEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type ClockMode
using ClockModeEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type BrPair
using BrPairEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type CanOperationMode
using CanOperationModeEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type CanSamplingMode
using CanSamplingModeEncoding=rti1516ev::HLAinteger32LE;
// enumerated data type CanBusState
using CanBusStateEncoding=rti1516ev::HLAinteger32LE;
// 
// Bytes carried in a CAN frame. Note that the actual number of bytes is carried in
// a distinct member field of the CANFrame.
//         
// fixed array of simple type HLAbyte
class CANMessageDataEncoding : public rti1516ev::HLAfixedArray
{
  public:
    CANMessageDataEncoding();
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

// CAN Frame
class CANFrameEncoding : public CANFrame, public rti1516ev::HLAfixedRecord
{
  public:
    CANFrameEncoding();
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

} // namespace NDistSimIB
} // namespace NRTFederateEncoding

