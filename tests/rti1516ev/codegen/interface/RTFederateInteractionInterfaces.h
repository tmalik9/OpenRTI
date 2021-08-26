
// Code automatically generated by FOMCodeGen version 2.0.1.15 from RTFederate.xml, do not edit

// Name: CANoe RT Federate
// Copyright: Copyright (c) Vector Informatik GmbH. All rights reserved.
// Version: 1.0
// Description: CANoe Simulation Object Model

#pragma once

#include <vector>
#include <functional>

#include "RTI/encoding/HLAhandle.h"
#include "RTFederateDataTypes.h"
#include "RTFederateObjectInterfaces.h"

namespace NDistSimIB {
namespace NRTFederateEncoding {

class IHLAinteractionRootInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send() = 0;
    virtual void sendWithTime(int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IMeasurementInitInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send() = 0;
    virtual void sendWithTime(int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IMeasurementStopInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<std::wstring> NextFederationSuffix, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<std::wstring> NextFederationSuffix) = 0;
    virtual void sendWithTime(optional<std::wstring> NextFederationSuffix, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<std::wstring> NextFederationSuffix, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IKeyEventInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<int32_t> KeyCode, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<int32_t> KeyCode) = 0;
    virtual void sendWithTime(optional<int32_t> KeyCode, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<int32_t> KeyCode, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class ITextLogInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<std::wstring> Sender, optional<std::wstring> Text, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<std::wstring> Sender, optional<std::wstring> Text) = 0;
    virtual void sendWithTime(optional<std::wstring> Sender, optional<std::wstring> Text, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<std::wstring> Sender, optional<std::wstring> Text, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IDOMemberTransmitDataInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<IDOMemberSource*> ObjInstanceHandle, optional<std::string> ConnectionType, optional<const std::vector<uint8_t>&> DataBytes, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<IDOMemberSource*> ObjInstanceHandle, optional<std::string> ConnectionType, optional<const std::vector<uint8_t>&> DataBytes) = 0;
    virtual void sendWithTime(optional<IDOMemberSource*> ObjInstanceHandle, optional<std::string> ConnectionType, optional<const std::vector<uint8_t>&> DataBytes, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<IDOMemberSource*> ObjInstanceHandle, optional<std::string> ConnectionType, optional<const std::vector<uint8_t>&> DataBytes, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class ISystemVariableUpdateInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<std::string> Id, optional<const std::vector<uint8_t>&> Value, optional<int32_t> Client, optional<bool> HasChanged, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<std::string> Id, optional<const std::vector<uint8_t>&> Value, optional<int32_t> Client, optional<bool> HasChanged) = 0;
    virtual void sendWithTime(optional<std::string> Id, optional<const std::vector<uint8_t>&> Value, optional<int32_t> Client, optional<bool> HasChanged, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<std::string> Id, optional<const std::vector<uint8_t>&> Value, optional<int32_t> Client, optional<bool> HasChanged, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class ISystemVariableModificationInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<const std::vector<uint8_t>&> Value, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<const std::vector<uint8_t>&> Value) = 0;
    virtual void sendWithTime(optional<const std::vector<uint8_t>&> Value, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<const std::vector<uint8_t>&> Value, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IValueEntityUpdateInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<const std::vector<uint8_t>&> Id, optional<const std::vector<uint8_t>&> Value, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<const std::vector<uint8_t>&> Id, optional<const std::vector<uint8_t>&> Value) = 0;
    virtual void sendWithTime(optional<const std::vector<uint8_t>&> Id, optional<const std::vector<uint8_t>&> Value, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<const std::vector<uint8_t>&> Id, optional<const std::vector<uint8_t>&> Value, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IBusMessageInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IEthPacketInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacket&> Frame, optional<std::string> PortName, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacket&> Frame, optional<std::string> PortName) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacket&> Frame, optional<std::string> PortName, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacket&> Frame, optional<std::string> PortName, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IEthPacketErrorInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketError&> Frame, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketError&> Frame) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketError&> Frame, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketError&> Frame, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IEthPacketErrorForwardedInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketErrorForwarded&> Frame, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketErrorForwarded&> Frame) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketErrorForwarded&> Frame, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketErrorForwarded&> Frame, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IEthForwardedPacketInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketForwarded&> Frame, optional<std::string> PortName, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketForwarded&> Frame, optional<std::string> PortName) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketForwarded&> Frame, optional<std::string> PortName, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetPacketForwarded&> Frame, optional<std::string> PortName, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IEthStatusInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetStatus&> Frame, optional<std::string> PortName, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetStatus&> Frame, optional<std::string> PortName) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetStatus&> Frame, optional<std::string> PortName, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const EthernetStatus&> Frame, optional<std::string> PortName, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class ICANMessageInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<int32_t> Id, optional<const CANFrame&> Frame, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<int32_t> Id, optional<const CANFrame&> Frame) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<int32_t> Id, optional<const CANFrame&> Frame, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<int32_t> Id, optional<const CANFrame&> Frame, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class ICANErrorFrameInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const std::vector<uint8_t>&> Frame, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const std::vector<uint8_t>&> Frame) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const std::vector<uint8_t>&> Frame, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<const std::vector<uint8_t>&> Frame, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IFlexRaySymbolInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<FlexRaySymbolPattern> SymbolPattern, optional<FlexRayChannel> FlexRayChannel, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<FlexRaySymbolPattern> SymbolPattern, optional<FlexRayChannel> FlexRayChannel) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<FlexRaySymbolPattern> SymbolPattern, optional<FlexRayChannel> FlexRayChannel, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<FlexRaySymbolPattern> SymbolPattern, optional<FlexRayChannel> FlexRayChannel, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IFlexRayCycleStartInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<uint8_t> Cycle, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<uint8_t> Cycle) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<uint8_t> Cycle, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<uint8_t> Cycle, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IFlexRayFrameInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<int16_t> FrameID, optional<bool> PayloadPreambleIndicator, optional<const FlexRayHeader&> Header, optional<const FlexRayPayload&> Payload, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<int16_t> FrameID, optional<bool> PayloadPreambleIndicator, optional<const FlexRayHeader&> Header, optional<const FlexRayPayload&> Payload) = 0;
    virtual void sendWithTime(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<int16_t> FrameID, optional<bool> PayloadPreambleIndicator, optional<const FlexRayHeader&> Header, optional<const FlexRayPayload&> Payload, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<bool> IsRequest, optional<std::string> ChannelName, optional<BusType> BusType, optional<rti1516ev::HLAhandle> RequestingFederate, optional<IBusController*> Sender, optional<IBusController*> Receiver, optional<int16_t> FrameID, optional<bool> PayloadPreambleIndicator, optional<const FlexRayHeader&> Header, optional<const FlexRayPayload&> Payload, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};

class IPythonCommandInteractionClass
{
  public:
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    virtual void Subscribe(bool deliverToSelf) = 0;
    virtual void Subscribe(optional<const std::vector<uint8_t>&> Code, optional<const std::vector<uint8_t>&> Target, optional<const std::vector<uint8_t>&> RefID, bool deliverToSelf) = 0;
    virtual void Unsubscribe() = 0;
    virtual void send(optional<const std::vector<uint8_t>&> Code, optional<const std::vector<uint8_t>&> Target, optional<const std::vector<uint8_t>&> RefID) = 0;
    virtual void sendWithTime(optional<const std::vector<uint8_t>&> Code, optional<const std::vector<uint8_t>&> Target, optional<const std::vector<uint8_t>&> RefID, int64_t time) = 0;
    using ReceiveCallback = std::function<void(optional<const std::vector<uint8_t>&> Code, optional<const std::vector<uint8_t>&> Target, optional<const std::vector<uint8_t>&> RefID, optional<int64_t> time, optional<OrderType> orderType)>;
    virtual uint32_t RegisterReceiveCallback(ReceiveCallback callback) = 0;
};


class IInteractionClassRegistry
{
  public:
    virtual IHLAinteractionRootInteractionClass* GetHLAinteractionRootInteractionClass() const = 0;
    virtual IMeasurementInitInteractionClass* GetMeasurementInitInteractionClass() const = 0;
    virtual IMeasurementStopInteractionClass* GetMeasurementStopInteractionClass() const = 0;
    virtual IKeyEventInteractionClass* GetKeyEventInteractionClass() const = 0;
    virtual ITextLogInteractionClass* GetTextLogInteractionClass() const = 0;
    virtual IDOMemberTransmitDataInteractionClass* GetDOMemberTransmitDataInteractionClass() const = 0;
    virtual ISystemVariableUpdateInteractionClass* GetSystemVariableUpdateInteractionClass() const = 0;
    virtual ISystemVariableModificationInteractionClass* GetSystemVariableModificationInteractionClass() const = 0;
    virtual IValueEntityUpdateInteractionClass* GetValueEntityUpdateInteractionClass() const = 0;
    virtual IBusMessageInteractionClass* GetBusMessageInteractionClass() const = 0;
    virtual IEthPacketInteractionClass* GetEthPacketInteractionClass() const = 0;
    virtual IEthPacketErrorInteractionClass* GetEthPacketErrorInteractionClass() const = 0;
    virtual IEthPacketErrorForwardedInteractionClass* GetEthPacketErrorForwardedInteractionClass() const = 0;
    virtual IEthForwardedPacketInteractionClass* GetEthForwardedPacketInteractionClass() const = 0;
    virtual IEthStatusInteractionClass* GetEthStatusInteractionClass() const = 0;
    virtual ICANMessageInteractionClass* GetCANMessageInteractionClass() const = 0;
    virtual ICANErrorFrameInteractionClass* GetCANErrorFrameInteractionClass() const = 0;
    virtual IFlexRaySymbolInteractionClass* GetFlexRaySymbolInteractionClass() const = 0;
    virtual IFlexRayCycleStartInteractionClass* GetFlexRayCycleStartInteractionClass() const = 0;
    virtual IFlexRayFrameInteractionClass* GetFlexRayFrameInteractionClass() const = 0;
    virtual IPythonCommandInteractionClass* GetPythonCommandInteractionClass() const = 0;
  protected:
    virtual ~IInteractionClassRegistry() {}
}; // class IInteractionClassRegistry

} // namespace NDistSimIB
} // namespace NRTFederateEncoding
