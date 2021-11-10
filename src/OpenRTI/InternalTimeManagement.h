/* -*-c++-*- OpenRTI - Copyright (C) 2009-2013 Mathias Froehlich
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

#ifndef OpenRTI_InternalTimeManagement_h
#define OpenRTI_InternalTimeManagement_h

#include "Export.h"
#include "Referenced.h"
#include "Message.h"
#include "Exception.h"
#include <string>

namespace OpenRTI {

class InternalAmbassador;

class OPENRTI_API InternalTimeManagement : public Referenced {
public:
  enum TimeRegulationMode {
    TimeRegulationDisabled,
    TimeRegulationEnablePending,
    TimeRegulationEnabled
  };
  enum TimeConstrainedMode {
    TimeConstrainedDisabled,
    TimeConstrainedEnablePending,
    TimeConstrainedEnabled
  };
  enum TimeAdvanceMode {
    TimeAdvanceGranted,
    TimeAdvanceRequest,
    TimeAdvanceRequestAvailable,
    NextMessageRequest,
    NextMessageRequestAvailable,
    FlushQueueRequest,
    ResetInitiated,
    ResetPending,
  };
  friend std::string OPENRTI_API to_string(TimeAdvanceMode);

  InternalTimeManagement();
  virtual ~InternalTimeManagement() noexcept = default;

  void setTimeRegulationMode(TimeRegulationMode timeRegulationMode)
  { _timeRegulationMode = timeRegulationMode; }
  TimeRegulationMode getTimeRegulationMode() const
  { return _timeRegulationMode; }
  bool getTimeRegulationDisabled() const
  { return _timeRegulationMode == TimeRegulationDisabled; }
  bool getTimeRegulationEnabled() const
  { return _timeRegulationMode == TimeRegulationEnabled; }
  bool getTimeRegulationEnablePending() const
  { return _timeRegulationMode == TimeRegulationEnablePending; }
  bool getTimeRegulationEnabledOrPending() const
  { return !getTimeRegulationDisabled(); }

  void setTimeConstrainedMode(TimeConstrainedMode timeConstrainedMode)
  { _timeConstrainedMode = timeConstrainedMode; }
  TimeConstrainedMode getTimeConstrainedMode() const
  { return _timeConstrainedMode; }
  bool getTimeConstrainedDisabled() const
  { return _timeConstrainedMode == TimeConstrainedDisabled; }
  bool getTimeConstrainedEnabled() const
  { return _timeConstrainedMode == TimeConstrainedEnabled; }
  bool getTimeConstrainedEnablePending() const
  { return _timeConstrainedMode == TimeConstrainedEnablePending; }
  bool getTimeConstrainedEnabledOrPending() const
  { return !getTimeConstrainedDisabled(); }

  void setTimeAdvanceMode(TimeAdvanceMode timeAdvanceMode)
  {
    //DebugPrintf("%s: %s => %s\n", __FUNCTION__, to_string(_timeAdvanceMode).c_str(), to_string(timeAdvanceMode).c_str());
    // if new state is ResetPending, previous state must be ResetInitiated
    OpenRTIAssert(timeAdvanceMode!=ResetPending||_timeAdvanceMode == ResetInitiated);
    // if new state is ResetInitiated, previous state must not be one of ResetInitiated or ResetPending
    OpenRTIAssert(timeAdvanceMode!=ResetInitiated||(_timeAdvanceMode != ResetInitiated && timeAdvanceMode!=ResetPending));
    _timeAdvanceMode = timeAdvanceMode;
  }
  TimeAdvanceMode getTimeAdvanceMode() const
  { return _timeAdvanceMode; }
  bool getTimeAdvancePending() const
  { return _timeAdvanceMode != TimeAdvanceGranted; }
  bool getFlushQueueMode() const
  { return _timeAdvanceMode == FlushQueueRequest; }
  bool getIsAnyAdvanceRequest() const
  { return _timeAdvanceMode == TimeAdvanceRequest || _timeAdvanceMode == TimeAdvanceRequestAvailable; }
  bool getIsAnyAvailableMode() const
  { return _timeAdvanceMode == TimeAdvanceRequestAvailable || _timeAdvanceMode == NextMessageRequestAvailable || _timeAdvanceMode == FlushQueueRequest; }
  bool getIsAnyNextMessageMode() const {
    return _timeAdvanceMode == NextMessageRequest || _timeAdvanceMode == NextMessageRequestAvailable;
  }
  bool getIsResetInitiated() const { return _timeAdvanceMode == ResetInitiated; }
  bool getIsResetPending() const { return _timeAdvanceMode == ResetPending; }

  bool getAsynchronousDeliveryEnabled() const {
    return _asynchronousDeliveryEnabled;
  }
  void setAsynchronousDeliveryEnabled(bool asynchronousDeliveryEnabled) {
    _asynchronousDeliveryEnabled = asynchronousDeliveryEnabled;
  }

  bool getAllowPendingTimeInNextMessageRequest() const {
    return _allowPendingTimeInNextMessageRequest;
  }
  void setAllowPendingTimeInNextMessageRequest(bool allowPendingTimeInNextMessageRequest) {
    _allowPendingTimeInNextMessageRequest = allowPendingTimeInNextMessageRequest;
  }

  OrderType getTimeStampOrderDelivery(OrderType orderType) const
  {
    if (!getTimeConstrainedEnabled())
      return OrderType::RECEIVE;
    return orderType;
  }

  uint32_t getNextMessageRetractionSerial()
  { return _messageRetractionSerial++; }

  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const JoinFederateNotifyMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const ResignFederateNotifyMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const EnableTimeRegulationRequestMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const EnableTimeRegulationResponseMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const DisableTimeRegulationRequestMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const CommitLowerBoundTimeStampMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const CommitLowerBoundTimeStampResponseMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const LockedByNextMessageRequestMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const ResetFederationInitiateMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const ResetFederationDoneMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const ResetFederationBegunMessage& message) = 0;
  virtual void acceptInternalMessage(InternalAmbassador& ambassador, const ResetFederationCompleteMessage& message) = 0;

  virtual void queueTimeStampedMessage(InternalAmbassador& ambassador, const VariableLengthData& timeStamp, const AbstractMessage& message, bool loopback) = 0;
  virtual void queueReceiveOrderMessage(InternalAmbassador& ambassador, const AbstractMessage& message) = 0;

  virtual bool dispatchCallback(const AbstractMessageDispatcher& dispatcher) = 0;
  virtual bool callbackMessageAvailable() = 0;
  virtual std::string logicalTimeToString(const VariableLengthData& nativeLogicalTime) = 0;

protected:

  // State values
  TimeRegulationMode _timeRegulationMode;
  TimeConstrainedMode _timeConstrainedMode;
  TimeAdvanceMode _timeAdvanceMode;
  bool _asynchronousDeliveryEnabled;
  bool _allowPendingTimeInNextMessageRequest;
  // Serial number for message retraction
  uint32_t _messageRetractionSerial;
};

std::string OPENRTI_API to_string(InternalTimeManagement::TimeAdvanceMode);

} // namespace OpenRTI

#endif
