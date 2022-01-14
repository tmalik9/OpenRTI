
// Code automatically generated by FOMCodeGen version 2.1.0.0 from SampleFDD.xml, do not edit

// Name: CANoe RT Federate
// Copyright: Copyright (c) Vector Informatik GmbH. All rights reserved.
// Version: 1.0
// Description: CANoe Simulation Object Model

#pragma once

#include <vector>
#include <functional>


#include "SampleFDDDataTypes.h"
#include "SampleFDDObjectInterfaces.h"

namespace NDistSimIB {
namespace NSampleFDDEncoding {

// IHLAinteractionRootInteractionClass represents the HLA interaction class HLAinteractionRoot.
class IHLAinteractionRootInteractionClass
{
  public:
    // Publish/unpublish the interaction class. Sending an interaction of this class requires publishing the interaction class.
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    // Subscribe the interaction class. Receiving interactions requires having subscribed to this interaction class.
    // Note that the appropriate callbacks should have been registered *before* subscribing to the interaction class.
    virtual void Subscribe(bool deliverToSelf) = 0;
    // Unsubscribe the interaction class, cancelling reception of interactions. Calling Unsubscribe will also clear all filtered subscriptions, if any.
    virtual void Unsubscribe() = 0;
    // Send an interaction as receive order message.
    virtual void send() = 0;
    // Send a time stamped interaction.
    virtual void sendWithTime(int64_t time) = 0;
    // Register an interaction callback. The optional time stamp will be delivered by the application. When the time stamp is valid,
    // the optional order type will indicate the source of the time stamp.
    using ReceiveCallback = std::function<void(optional<int64_t> time, optional<OrderType> orderType)>;
    using ReceiveCallbackHandle = uint32_t;
    constexpr static ReceiveCallbackHandle kInvalidCallbackHandle = 0xffffffff;
    virtual ReceiveCallbackHandle RegisterReceiveCallback(ReceiveCallback callback) = 0;
    virtual void UnregisterReceiveCallback(ReceiveCallbackHandle callbackHandle) = 0;
};

// IStartInteractionClass represents the HLA interaction class HLAinteractionRoot.Start.
class IStartInteractionClass
{
  public:
    // Publish/unpublish the interaction class. Sending an interaction of this class requires publishing the interaction class.
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    // Subscribe the interaction class. Receiving interactions requires having subscribed to this interaction class.
    // Note that the appropriate callbacks should have been registered *before* subscribing to the interaction class.
    virtual void Subscribe(bool deliverToSelf) = 0;
    // Unsubscribe the interaction class, cancelling reception of interactions. Calling Unsubscribe will also clear all filtered subscriptions, if any.
    virtual void Unsubscribe() = 0;
    // Send an interaction as receive order message.
    virtual void send() = 0;
    // Send a time stamped interaction.
    virtual void sendWithTime(int64_t time) = 0;
    // Register an interaction callback. The optional time stamp will be delivered by the application. When the time stamp is valid,
    // the optional order type will indicate the source of the time stamp.
    using ReceiveCallback = std::function<void(optional<int64_t> time, optional<OrderType> orderType)>;
    using ReceiveCallbackHandle = uint32_t;
    constexpr static ReceiveCallbackHandle kInvalidCallbackHandle = 0xffffffff;
    virtual ReceiveCallbackHandle RegisterReceiveCallback(ReceiveCallback callback) = 0;
    virtual void UnregisterReceiveCallback(ReceiveCallbackHandle callbackHandle) = 0;
};

// IStopInteractionClass represents the HLA interaction class HLAinteractionRoot.Stop.
class IStopInteractionClass
{
  public:
    // Publish/unpublish the interaction class. Sending an interaction of this class requires publishing the interaction class.
    virtual void Publish() = 0;
    virtual void Unpublish() = 0;
    // Subscribe the interaction class. Receiving interactions requires having subscribed to this interaction class.
    // Note that the appropriate callbacks should have been registered *before* subscribing to the interaction class.
    virtual void Subscribe(bool deliverToSelf) = 0;
    // Subscribe the interaction class with filter values. Only interactions with parameters matching the given values will be received.
    // Filtered subscriptions may be issued several times with differing value tuples. 'nullopt' may be used as a wildcard, matching
    // any incoming value.
    virtual void Subscribe(optional<std::wstring> NextFederationSuffix, bool deliverToSelf) = 0;
    // Unsubscribe the interaction class, cancelling reception of interactions. Calling Unsubscribe will also clear all filtered subscriptions, if any.
    virtual void Unsubscribe() = 0;
    // Send an interaction as receive order message.
    virtual void send(optional<std::wstring> NextFederationSuffix) = 0;
    // Send a time stamped interaction.
    virtual void sendWithTime(optional<std::wstring> NextFederationSuffix, int64_t time) = 0;
    // Register an interaction callback. The optional time stamp will be delivered by the application. When the time stamp is valid,
    // the optional order type will indicate the source of the time stamp.
    using ReceiveCallback = std::function<void(optional<std::wstring> NextFederationSuffix, optional<int64_t> time, optional<OrderType> orderType)>;
    using ReceiveCallbackHandle = uint32_t;
    constexpr static ReceiveCallbackHandle kInvalidCallbackHandle = 0xffffffff;
    virtual ReceiveCallbackHandle RegisterReceiveCallback(ReceiveCallback callback) = 0;
    virtual void UnregisterReceiveCallback(ReceiveCallbackHandle callbackHandle) = 0;
};


class IInteractionClassRegistry
{
  public:
    virtual IHLAinteractionRootInteractionClass* GetHLAinteractionRootInteractionClass() const = 0;
    virtual IStartInteractionClass* GetStartInteractionClass() const = 0;
    virtual IStopInteractionClass* GetStopInteractionClass() const = 0;
  protected:
    virtual ~IInteractionClassRegistry() {}
}; // class IInteractionClassRegistry

} // namespace NDistSimIB
} // namespace NSampleFDDEncoding
