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

#ifndef OpenRTI_Ambassador_h
#define OpenRTI_Ambassador_h

#include <algorithm>
#include <sstream>
#include "AbstractConnect.h"
#include "InternalAmbassador.h"
#include "Clock.h"
#include "Exception.h"
#include "Federate.h"
#include "LogStream.h"
#include "Message.h"
#include "URL.h"
#include "StringUtils.h"
#include "TimeManagement.h"
#include "TranslateTypes.h"
#include "AbsTimeout.h"

namespace OpenRTI {

// Ambassador<RTI(VERSION)Traits> is the base class of rti(VERSION)::RTIambassadorImplementation::RTI(VERSION)AmbassadorInterface,
// which is referenced by RTIambassadorImplementation in the rti(VERSION) libraries.
// These, in turn, implement the Version-specific RT ambassador interfaces (which are visible to the user).
// Each call into the RTI ambassador goes through the RTI(VERSION)AmbassadorInterface into Ambassador<RTI(VERSION)Traits>,
// where the functionality is actually implemented.
// the Traits parameter is mainly used for getting access to the native logical time class, which is again version-specific.
template<typename T>
class OPENRTI_LOCAL Ambassador : public InternalAmbassador {
public:
  typedef T Traits;

  // Times
  typedef typename Traits::NativeLogicalTime NativeLogicalTime;
  typedef typename Traits::NativeLogicalTimeInterval NativeLogicalTimeInterval;
  static const uint32_t kInfinite = static_cast<uint32_t>(-1);
  Ambassador()
    : _connectWaitTimeout(70 * 1000)
    , _operationWaitTimeout(70 * 1000)
  {

  }

  const FederateHandle& getFederateHandle() const
  { return _federate->getFederateHandle(); }
  const FederationHandle& getFederationHandle() const
  { return _federate->getFederationHandle(); }

  void setConnectWaitTimeoutMilliSeconds(uint32_t timeoutMilliSeconds)
  {
    _connectWaitTimeout = timeoutMilliSeconds;
  }
  uint32_t getConnectWaitTimeoutMilliSeconds() const
  {
    return _connectWaitTimeout;
  }
  void setOperationWaitTimeoutMilliSeconds(uint32_t timeoutMilliSeconds)
  {
    _operationWaitTimeout = timeoutMilliSeconds;
  }
  uint32_t getOperationWaitTimeoutMilliSeconds() const
  {
    return _operationWaitTimeout;
  }
  void connect(const URL& url, const StringStringListMap& stringStringListMap, uint32_t timeoutMilliSeconds)
  {
    if (isConnected())
      throw AlreadyConnected("Ambassador is already connected!");

    InternalAmbassador::connect(url, stringStringListMap, timeoutMilliSeconds);

    if (!isConnected())
      throw ConnectionFailed("Connection failed!");
  }

  void disconnect()
  {
    if (_federate.valid())
      throw FederateIsExecutionMember();

    if (!isConnected())
      return;
    InternalAmbassador::disconnect();
  }

  void createFederationExecution(const std::string& federationExecutionName,
                                 const FOMStringModule2List& fomModules,
                                 const std::string& logicalTimeFactoryName)
  {
    if (!isConnected())
      throw NotConnected(std::string("Could not get connect RTI of federation execution \"") +
                         federationExecutionName + std::string("\"."));
    if (getProtocolVersion() == 8)
    {
      // The create request message
      SharedPtr<CreateFederationExecutionRequestMessage> request = MakeShared<CreateFederationExecutionRequestMessage>();
      request->setFederationExecution(federationExecutionName);
      request->setLogicalTimeFactoryName(logicalTimeFactoryName);
      request->setFOMStringModuleList(TranslateTypes::translate(fomModules));

      // Send this message and wait for the response
      send(request);
    }
    else if (getProtocolVersion() == 9)
    {
      // The create request message
      SharedPtr<CreateFederationExecutionRequest2Message> request = MakeShared<CreateFederationExecutionRequest2Message>();
      request->setFederationExecution(federationExecutionName);
      request->setLogicalTimeFactoryName(logicalTimeFactoryName);
      request->setFOMStringModuleList(fomModules);

      // Send this message and wait for the response
      send(request);
    }
    std::pair<CreateFederationExecutionResponseType, std::string> responseTypeStringPair;
    Clock abstime = (_operationWaitTimeout == kInfinite) ? Clock::max() : Clock::now() + Clock::fromMilliSeconds(_operationWaitTimeout);
    AbsTimeout timeout(abstime);
    responseTypeStringPair = dispatchWaitCreateFederationExecutionResponse(timeout);
    if (responseTypeStringPair.first == CreateFederationExecutionResponseFederationExecutionAlreadyExists)
      throw FederationExecutionAlreadyExists(federationExecutionName);
    if (responseTypeStringPair.first == CreateFederationExecutionResponseCouldNotCreateLogicalTimeFactory)
      throw CouldNotCreateLogicalTimeFactory(logicalTimeFactoryName);
    if (responseTypeStringPair.first == CreateFederationExecutionResponseInconsistentFDD)
      throw InconsistentFDD(responseTypeStringPair.second);
    if (responseTypeStringPair.first == CreateFederationExecutionResponseTimeout)
      throw OperationTimeout(responseTypeStringPair.second);
    if (responseTypeStringPair.first != CreateFederationExecutionResponseSuccess)
      throw RTIinternalError(responseTypeStringPair.second);
  }

  void destroyFederationExecution(const std::string& federationExecutionName)
    // throw (FederatesCurrentlyJoined,
    //        FederationExecutionDoesNotExist,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected(std::string("Could not get connect RTI of federation execution \"") +
                         federationExecutionName + std::string("\"."));

    // The destroy request message
    SharedPtr<DestroyFederationExecutionRequestMessage> request = MakeShared<DestroyFederationExecutionRequestMessage>();
    request->setFederationExecution(federationExecutionName);


    // Send this message and wait for the response
    send(request);

    DestroyFederationExecutionResponseType responseType;
    Clock abstime = (_operationWaitTimeout == kInfinite) ? Clock::max() : Clock::now() + Clock::fromMilliSeconds(_operationWaitTimeout);
    AbsTimeout timeout(abstime);
    responseType = dispatchWaitDestroyFederationExecutionResponse(timeout);
    if (responseType == DestroyFederationExecutionResponseFederatesCurrentlyJoined)
      throw FederatesCurrentlyJoined(federationExecutionName);
    if (responseType == DestroyFederationExecutionResponseFederationExecutionDoesNotExist)
      throw FederationExecutionDoesNotExist(federationExecutionName);
    if (responseType == DestroyFederationExecutionResponseRTIinternalError)
      throw NotConnected("disconnected while destroying federation execution");
    if (responseType != DestroyFederationExecutionResponseSuccess)
      throw RTIinternalError("Unspecified internal error FIXME");
  }

  void listFederationExecutions()
    // throw (NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();

    SharedPtr<EnumerateFederationExecutionsRequestMessage> request;
    request = MakeShared<EnumerateFederationExecutionsRequestMessage>();
    send(request);
  }

  FederateHandle joinFederationExecution(const std::string& federateName, const std::string& federateType,
                                         const std::string& federationExecutionName,
                                         const FOMStringModule2List& fomModules)
  {
    if (!isConnected())
      throw NotConnected(std::string("Could not get connect RTI of federation execution \"") +
                         federationExecutionName + std::string("\"."));
    if (_federate.valid())
      throw FederateAlreadyExecutionMember();

   
    // The join request message
    // the protocol version might even be checked later
    if (getProtocolVersion() == 8)
    {
      SharedPtr<JoinFederationExecutionRequestMessage> request = MakeShared<JoinFederationExecutionRequestMessage>();
      request->setFederationExecution(federationExecutionName);
      request->setFederateType(federateType);
      request->setFederateName(federateName);
      request->setFOMStringModuleList(TranslateTypes::translate(fomModules));

      // Send this message and wait for the response
      send(request);
    }
    else if (getProtocolVersion() == 9)
    {
      SharedPtr<JoinFederationExecutionRequest2Message> request = MakeShared<JoinFederationExecutionRequest2Message>();
      request->setFederationExecution(federationExecutionName);
      request->setFederateType(federateType);
      request->setFederateName(federateName);
      request->setFOMStringModuleList(fomModules);

      // Send this message and wait for the response
      send(request);
    }
    std::pair<JoinFederationExecutionResponseType, std::string> response;
    // The maximum abstime to try to connect
    Clock abstime = (_operationWaitTimeout == kInfinite) ? Clock::max() : Clock::now() + Clock::fromMilliSeconds(_operationWaitTimeout);
    AbsTimeout timeout(abstime);
    response = dispatchWaitJoinFederationExecutionResponse(timeout, federateName);
    switch (response.first) {
    case JoinFederationExecutionResponseFederateNameAlreadyInUse:
      _federate.reset();
      throw FederateNameAlreadyInUse(federateName);
      break;
    case JoinFederationExecutionResponseFederationExecutionDoesNotExist:
      _federate.reset();
      throw FederationExecutionDoesNotExist(federationExecutionName);
      break;
    case JoinFederationExecutionResponseSaveInProgress:
      _federate.reset();
      throw SaveInProgress();
      break;
    case JoinFederationExecutionResponseRestoreInProgress:
      _federate.reset();
      throw RestoreInProgress();
      break;
    case JoinFederationExecutionResponseInconsistentFDD:
      _federate.reset();
      throw InconsistentFDD(response.second);
      break;
    case JoinFederationExecutionResponseTimeout:
      _federate.reset();
      throw OperationTimeout(response.second);
      break;
    default:
      break;
    }
    if (!_federate.valid())
      throw RTIinternalError("Federate is not valid!");

    if (!_timeManagement.valid()) {
      std::string logicalTimeFactoryName = _federate->getLogicalTimeFactoryName();

      _resignFederationExecution(CANCEL_THEN_DELETE_THEN_DIVEST);

      if (!logicalTimeFactoryName.empty())
        throw CouldNotCreateLogicalTimeFactory(logicalTimeFactoryName);
      else
        throw RTIinternalError();
    }

    // Request new object instance handles, once we are here ...
    _requestObjectInstanceHandles(16);

    return getFederateHandle();
  }

  void resignFederationExecution(ResignAction resignAction)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    _resignFederationExecution(resignAction);
  }

  void _resignFederationExecution(ResignAction resignAction)
  {
    if (!_federate.valid())
      return;

    SharedPtr<ResignFederationExecutionLeafRequestMessage> resign;
    resign = MakeShared<ResignFederationExecutionLeafRequestMessage>();
    resign->setFederationHandle(getFederationHandle());
    resign->setFederateHandle(getFederateHandle());
    resign->setResignAction(resignAction);
    send(resign);

    // We should no longer respond to time regulation requests.
    _timeManagement.reset();

    Clock abstime = (_operationWaitTimeout == kInfinite) ? Clock::max() : Clock::now() + Clock::fromMilliSeconds(_operationWaitTimeout);
    AbsTimeout timeout(abstime);
    if (!dispatchWaitEraseFederationExecutionResponse(timeout))
      throw RTIinternalError("resignFederationExecution hit timeout!");

    _federate.reset();
  }


  void registerFederationSynchronizationPoint(const std::string& label, VariableLengthData& tag, FederateHandleVector& syncSet)
    // throw (FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();

    // Tell all federates about that label
    SharedPtr<RegisterFederationSynchronizationPointMessage> message;
    message = MakeShared<RegisterFederationSynchronizationPointMessage>();
    message->setFederationHandle(getFederationHandle());
    message->setFederateHandle(getFederateHandle());
    message->setLabel(label);
    message->getTag().swap(tag);
    message->getFederateHandleVector().swap(syncSet);
    send(message);
  }

  void synchronizationPointAchieved(const std::string& label, bool successfully)
    // throw (SynchronizationPointLabelNotAnnounced,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (!_federate->synchronizationLabelAnnounced(label))
      throw SynchronizationPointLabelNotAnnounced();

    // tell all federates about that label
    SharedPtr<SynchronizationPointAchievedMessage> message;
    message = MakeShared<SynchronizationPointAchievedMessage>();
    message->setFederationHandle(getFederationHandle());
    message->getFederateHandleBoolPairVector().push_back(FederateHandleBoolPair(getFederateHandle(), successfully));
    message->setLabel(label);
    send(message);
  }

  void requestFederationSave(const std::string& label)
    // throw (FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void requestFederationSave(const std::string& label, const NativeLogicalTime& locicalTime)
    // throw (LogicalTimeAlreadyPassed,
    //        InvalidLogicalTime,
    //        FederateUnableToUseTime,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void federateSaveBegun()
    // throw (SaveNotInitiated,
    //        FederateNotExecutionMember,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void federateSaveComplete()
    // throw (FederateHasNotBegunSave,
    //        FederateNotExecutionMember,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void federateSaveNotComplete()
    // throw (FederateHasNotBegunSave,
    //        FederateNotExecutionMember,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void abortFederationSave()
    // throw (SaveNotInProgress,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void queryFederationSaveStatus()
    // throw (FederateNotExecutionMember,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void requestFederationRestore(const std::string& label)
    // throw (FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void federateRestoreComplete()
    // throw (RestoreNotRequested,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void federateRestoreNotComplete()
    // throw (RestoreNotRequested,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void abortFederationRestore()
    // throw (RestoreNotInProgress,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  void queryFederationRestoreStatus()
    // throw (FederateNotExecutionMember,
    //        SaveInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Save/Restore not implemented!");
  }

  // Note that the attributeHandleVector is consumed as if it could be with a lref in c++11
  void publishObjectClassAttributes(ObjectClassHandle objectClassHandle, AttributeHandleVector& attributeHandleVector)
    // throw (ObjectClassNotDefined,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    // At first the complete error checks
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());
    for (AttributeHandleVector::const_iterator i = attributeHandleVector.begin(); i != attributeHandleVector.end(); ++i)
      if (!objectClass->getAttribute(*i))
        throw AttributeNotDefined(i->toString());

    bool objectClassFreshPublished = false;
    // Mark the objectclass itself as published.
    // Append this to the request if this publication has changed
    AttributeHandleVector::iterator j = attributeHandleVector.begin();
    if (objectClass->setPublicationType(Published)) {
      objectClassFreshPublished = true;
      if (attributeHandleVector.empty() || attributeHandleVector.front() != AttributeHandle(0))
        j = attributeHandleVector.insert(attributeHandleVector.begin(), AttributeHandle(0));
      ++j;
    }
    for (AttributeHandleVector::const_iterator i = j; i != attributeHandleVector.end(); ++i) {
      // returns true if there is a change in the publication state
      if (!objectClass->setAttributePublicationType(*i, Published))
        continue;
      if (i != j)
        *j = *i;
      ++j;
    }
    if (j != attributeHandleVector.end())
      attributeHandleVector.erase(j, attributeHandleVector.end());
    // If there has nothing changed, don't send anything.
    if (attributeHandleVector.empty())
      return;

    SharedPtr<ChangeObjectClassPublicationMessage> request = MakeShared<ChangeObjectClassPublicationMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectClassHandle(objectClassHandle);
    request->setPublicationType(Published);
    request->getAttributeHandles().swap(attributeHandleVector);
    send(request);

    // Simple implementation only listening to the ambassadors own publication
    // see comment in enableObjectClassRelevanceAdvisorySwitch()
    if (objectClassFreshPublished && _federate->getObjectClassRelevanceAdvisorySwitchEnabled()) {
      SharedPtr<RegistrationForObjectClassMessage> message = MakeShared<RegistrationForObjectClassMessage>();
      message->setObjectClassHandle(objectClassHandle);
      message->setStart(true);

      queueCallback(message);
    }
  }

  void unpublishObjectClass(ObjectClassHandle objectClassHandle)
    // throw (ObjectClassNotDefined,
    //        OwnershipAcquisitionPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    // At first the complete error checks
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());

    // now that we know not to throw, handle the request
    AttributeHandleVector attributeHandleVector;
    attributeHandleVector.reserve(objectClass->getNumAttributes());
    // Mark the objectclass itself as unpublished.
    // Append this to the request if this publication has changed
    if (objectClass->setPublicationType(Unpublished))
      attributeHandleVector.push_back(AttributeHandle(0));
    for (uint32_t i = 0; i < objectClass->getNumAttributes(); ++i) {
      // returns true if there is a change in the publication state
      if (!objectClass->setAttributePublicationType(AttributeHandle(i), Unpublished))
        continue;
      attributeHandleVector.push_back(AttributeHandle(i));
    }
    // If there has nothing changed, don't send anything.
    if (attributeHandleVector.empty())
      return;

    SharedPtr<ChangeObjectClassPublicationMessage> request = MakeShared<ChangeObjectClassPublicationMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectClassHandle(objectClassHandle);
    request->setPublicationType(Unpublished);
    request->getAttributeHandles().swap(attributeHandleVector);
    send(request);

    // No advisory callbacks in unpublished state
  }

  // Note that the attributeHandleVector is consumed as if it could be with a lref in c++11
  void unpublishObjectClassAttributes(ObjectClassHandle objectClassHandle, AttributeHandleVector& attributeHandleVector)
    // throw (ObjectClassNotDefined,
    //        AttributeNotDefined,
    //        OwnershipAcquisitionPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    // At first the complete error checks
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());
    for (AttributeHandleVector::const_iterator i = attributeHandleVector.begin(); i != attributeHandleVector.end(); ++i)
      if (!objectClass->getAttribute(*i))
        throw AttributeNotDefined(i->toString());

    // Mark the objectclass itself as unpublished.
    // Append this to the request if this publication has changed
    AttributeHandleVector::iterator j = attributeHandleVector.begin();
    for (AttributeHandleVector::const_iterator i = j; i != attributeHandleVector.end(); ++i) {
      // returns true if there is a change in the publication state
      if (!objectClass->setAttributePublicationType(*i, Unpublished))
        continue;
      if (i != j)
        *j = *i;
      ++j;
    }
    if (j != attributeHandleVector.end())
      attributeHandleVector.erase(j, attributeHandleVector.end());
    // If there has nothing changed, don't send anything.
    if (attributeHandleVector.empty())
      return;

    SharedPtr<ChangeObjectClassPublicationMessage> request = MakeShared<ChangeObjectClassPublicationMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectClassHandle(objectClassHandle);
    request->setPublicationType(Unpublished);
    request->getAttributeHandles().swap(attributeHandleVector);
    send(request);

    // No advisory callbacks in unpublished state
  }

  void publishInteractionClass(InteractionClassHandle interactionClassHandle)
    // throw (InteractionClassNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());
    if (!interactionClass->setPublicationType(Published))
      return;

    SharedPtr<ChangeInteractionClassPublicationMessage> request = MakeShared<ChangeInteractionClassPublicationMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setInteractionClassHandle(interactionClassHandle);
    request->setPublicationType(Published);

    send(request);

    // Simple implementation only listening to the ambassadors own publication
    // see comment in enableInteractionRelevanceAdvisorySwitch()
    if (_federate->getInteractionRelevanceAdvisorySwitchEnabled()) {
      SharedPtr<TurnInteractionsOnMessage> message = MakeShared<TurnInteractionsOnMessage>();
      message->setInteractionClassHandle(interactionClassHandle);
      message->setOn(true);

      queueCallback(message);
    }
  }

  void unpublishInteractionClass(InteractionClassHandle interactionClassHandle)
    // throw (InteractionClassNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();

    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());
    if (!interactionClass->setPublicationType(Unpublished))
      return;

    SharedPtr<ChangeInteractionClassPublicationMessage> request = MakeShared<ChangeInteractionClassPublicationMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setInteractionClassHandle(interactionClassHandle);
    request->setPublicationType(Unpublished);

    send(request);

    // No advisory callbacks in unpublished state
  }

  void subscribeObjectClassAttributes(ObjectClassHandle objectClassHandle, AttributeHandleVector& attributeHandleVector, bool active, const std::string& updateRateDesignator)
    // throw (ObjectClassNotDefined,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        InvalidUpdateRateDesignator,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    // At first the complete error checks
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());
    for (AttributeHandleVector::const_iterator i = attributeHandleVector.begin(); i != attributeHandleVector.end(); ++i)
      if (!objectClass->getAttribute(*i))
        throw AttributeNotDefined(i->toString());
    if (!updateRateDesignator.empty() && _federate->getUpdateRateValue(updateRateDesignator) < 0)
      throw InvalidUpdateRateDesignator(updateRateDesignator);
    if (!updateRateDesignator.empty())
      throw RTIinternalError("Non trivial update rate designators are not implemented yet!");

    // now that we know not to throw, handle the request
    SubscriptionType subscriptionType;
    if (active) {
      subscriptionType = SubscribedActive;
    } else {
      subscriptionType = SubscribedPassive;
    }

    // now filter out all class attributes from attributeHandleVector,
    // where subscription didn't change by this request.

    AttributeHandleVector::iterator j = attributeHandleVector.begin();
    if (objectClass->setSubscriptionType(subscriptionType)) {
      // Class subscription changed: insert AttributeHandle(0) (HLAprivilegeToDeleteObject) to front, if not existing
      if (attributeHandleVector.empty() || attributeHandleVector.front() != AttributeHandle(0))
        j = attributeHandleVector.insert(attributeHandleVector.begin(), AttributeHandle(0));
      ++j;
    }
    // j should now point to next attribute after HLAprivilegeToDeleteObject.
    // NOTE: if class has been subscribed already, it will actually point to begin of list. bug?
    // Iterate through remaining class attributes, updating their subscriptions
    for (AttributeHandleVector::const_iterator i = j; i != attributeHandleVector.end(); ++i) {
      // returns true if there is a change in the subscription state
      if (!objectClass->setAttributeSubscriptionType(*i, subscriptionType))
        continue;
      if (i != j)
        *j = *i;
      ++j;
    }
    // j now points to last attribute in attributeHandleVector, where subscription changed.
    // Remove remaining attributes (with unchanged subscriptions) from list.
    if (j != attributeHandleVector.end())
      attributeHandleVector.erase(j, attributeHandleVector.end());
    // If there has nothing changed, don't send anything.
    if (attributeHandleVector.empty())
      return;

    SharedPtr<ChangeObjectClassSubscriptionMessage> request = MakeShared<ChangeObjectClassSubscriptionMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectClassHandle(objectClassHandle);
    request->getAttributeHandles().swap(attributeHandleVector);
    request->setSubscriptionType(subscriptionType);
    send(request);
  }

  void unsubscribeObjectClass(ObjectClassHandle objectClassHandle)
    // throw (ObjectClassNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    // At first the complete error checks
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());

    // now that we know not to throw, handle the request
    AttributeHandleVector attributeHandleVector;
    attributeHandleVector.reserve(objectClass->getNumAttributes());
    // Mark the objectclass itself as unpublished.
    // Append this to the request if this subscription has changed
    if (objectClass->setSubscriptionType(Unsubscribed))
      attributeHandleVector.push_back(AttributeHandle(0));
    for (uint32_t i = 0; i < objectClass->getNumAttributes(); ++i) {
      // returns true if there is a change in the subscription state
      if (!objectClass->setAttributeSubscriptionType(AttributeHandle(i), Unsubscribed))
        continue;
      attributeHandleVector.push_back(AttributeHandle(i));
    }
    // If there has nothing changed, don't send anything.
    if (attributeHandleVector.empty())
      return;

    SharedPtr<ChangeObjectClassSubscriptionMessage> request = MakeShared<ChangeObjectClassSubscriptionMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectClassHandle(objectClassHandle);
    request->setSubscriptionType(Unsubscribed);
    request->getAttributeHandles().swap(attributeHandleVector);
    send(request);

    OpenRTIAssert(objectClass->getEffectiveSubscriptionType() == Unsubscribed);

    for (Federate::ObjectInstanceHandleMap::const_iterator i = _federate->getObjectInstanceHandleMap().begin();
         i != _federate->getObjectInstanceHandleMap().end();) {
      if (i->second->getObjectClassHandle() != objectClassHandle) {
        ++i;
      } else if (i->second->isOwnedByFederate()) {
        ++i;
      } else {
        _releaseObjectInstance(ObjectInstanceHandle((i++)->first));
      }
    }
  }

  void unsubscribeObjectClassAttributes(ObjectClassHandle objectClassHandle, AttributeHandleVector& attributeHandleVector)
    // throw (ObjectClassNotDefined,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    // At first the complete error checks
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());
    for (AttributeHandleVector::const_iterator i = attributeHandleVector.begin(); i != attributeHandleVector.end(); ++i)
      if (!objectClass->getAttribute(*i))
        throw AttributeNotDefined(i->toString());

    // now that we know not to throw, handle the request
    AttributeHandleVector::iterator j = attributeHandleVector.begin();
    for (AttributeHandleVector::const_iterator i = j; i != attributeHandleVector.end(); ++i) {
      // returns true if there is a change in the subscription state
      if (!objectClass->setAttributeSubscriptionType(*i, Unsubscribed))
        continue;
      if (i != j)
        *j = *i;
      ++j;
    }
    if (j != attributeHandleVector.end())
      attributeHandleVector.erase(j, attributeHandleVector.end());
    // If there has nothing changed, don't send anything.
    if (attributeHandleVector.empty())
      return;

    SharedPtr<ChangeObjectClassSubscriptionMessage> request = MakeShared<ChangeObjectClassSubscriptionMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectClassHandle(objectClassHandle);
    request->setSubscriptionType(Unsubscribed);
    request->getAttributeHandles().swap(attributeHandleVector);
    send(request);

    if (objectClass->getEffectiveSubscriptionType() == Unsubscribed) {
      for (Federate::ObjectInstanceHandleMap::const_iterator i = _federate->getObjectInstanceHandleMap().begin();
           i != _federate->getObjectInstanceHandleMap().end();) {
        if (i->second->getObjectClassHandle() != objectClassHandle) {
          ++i;
        } else if (i->second->isOwnedByFederate()) {
          ++i;
        } else {
          _releaseObjectInstance(ObjectInstanceHandle((i++)->first));
        }
      }
    }
  }

  void unsubscribeObjectInstance(ObjectInstanceHandle objectInstanceHandle)
    // throw (ObjectClassNotDefined,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    // At first the complete error checks
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    ObjectClassHandle objectClassHandle = objectInstance->getObjectClassHandle();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());

    if (objectInstance->setSubscriptionType(Unsubscribed)) // returns true if changed
    {

      //DebugPrintf("unsubscribeObjectInstance: objInst.getName=%s, objInst.getHandle=%s, objInst.subscription=%d\n",
      //  objectInstance->getName().c_str(),
      //  objectInstanceHandle.toString().c_str(),
      //  objectInstance->getSubscriptionType() != Unsubscribed);

      // Tell all others this federate has unsubscribed the object instance
      SharedPtr<ChangeObjectInstanceSubscriptionMessage> request = MakeShared<ChangeObjectInstanceSubscriptionMessage>();
      request->setFederationHandle(getFederationHandle());
      request->setObjectClassHandle(objectClassHandle);
      request->setObjectInstanceHandle(objectInstanceHandle);
      request->setSubscriptionType(Unsubscribed);
      send(request);
    }
  }

  void subscribeInteractionClass(InteractionClassHandle interactionClassHandle, bool active)
    // throw (InteractionClassNotDefined,
    //        FederateServiceInvocationsAreBeingReportedViaMOM,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());

    SubscriptionType subscriptionType;
    if (active) {
      subscriptionType = SubscribedActive;
    } else {
      subscriptionType = SubscribedPassive;
    }
    bool subscriptionChanged = interactionClass->setSubscriptionType(subscriptionType, ParameterValueVector());
    if (subscriptionChanged)
    {
      SharedPtr<ChangeInteractionClassSubscriptionMessage> request = MakeShared<ChangeInteractionClassSubscriptionMessage>();
      request->setFederationHandle(getFederationHandle());
      request->setInteractionClassHandle(interactionClassHandle);
      request->setSubscriptionType(subscriptionType);
      send(request);
    }
  }

  void subscribeInteractionClassWithFilter(InteractionClassHandle interactionClassHandle,
                                           std::vector<ParameterValue>& filterValues,
                                           bool active)
    // throw (InteractionClassNotDefined,
    //        FederateServiceInvocationsAreBeingReportedViaMOM,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());

    SubscriptionType subscriptionType;
    if (active) {
      subscriptionType = SubscribedActive;
    } else {
      subscriptionType = SubscribedPassive;
    }

    bool subscriptionChanged = interactionClass->setSubscriptionType(subscriptionType, filterValues);
    if (subscriptionChanged)
    {
      // maybe we want to store parameter filters in the federate's InteractionClass as well.
      SharedPtr<ChangeInteractionClassSubscriptionMessage> request = MakeShared<ChangeInteractionClassSubscriptionMessage>();
      request->setFederationHandle(getFederationHandle());
      request->setInteractionClassHandle(interactionClassHandle);
      request->setSubscriptionType(subscriptionType);
      request->getParameterFilterValues().swap(filterValues);

      send(request);
    }
  }

  void unsubscribeInteractionClass(InteractionClassHandle interactionClassHandle)
    // throw (InteractionClassNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());

    bool subscriptionChanged = interactionClass->setSubscriptionType(Unsubscribed, ParameterValueVector());
    if (subscriptionChanged)
    {
      SharedPtr<ChangeInteractionClassSubscriptionMessage> request = MakeShared<ChangeInteractionClassSubscriptionMessage>();
      request->setFederationHandle(getFederationHandle());
      request->setInteractionClassHandle(interactionClassHandle);
      request->setSubscriptionType(Unsubscribed);

      send(request);
    }
  }

  void unsubscribeInteractionClassWithFilter(InteractionClassHandle interactionClassHandle,
                                             std::vector<ParameterValue>& filterValues)
    // throw (InteractionClassNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());

    bool subscriptionChanged = interactionClass->setSubscriptionType(Unsubscribed, filterValues);
    if (subscriptionChanged)
    {
      SharedPtr<ChangeInteractionClassSubscriptionMessage> request = MakeShared<ChangeInteractionClassSubscriptionMessage>();
      request->setFederationHandle(getFederationHandle());
      request->setInteractionClassHandle(interactionClassHandle);
      request->setSubscriptionType(Unsubscribed);
      request->getParameterFilterValues().swap(filterValues);

      send(request);
    }
  }

  void reserveObjectInstanceName(const std::string& objectInstanceName)
    // throw (IllegalName,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (objectInstanceName.empty())
      throw IllegalName("Empty object names are not allowed!");
    if (objectInstanceName.compare(0, 3, "HLA") == 0)
      throw IllegalName("Object instance names starting with \"HLA\" are reserved for the RTI.");

    SharedPtr<ReserveObjectInstanceNameRequestMessage> request;
    request = MakeShared<ReserveObjectInstanceNameRequestMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setFederateHandle(getFederateHandle());
    request->setName(objectInstanceName);

    send(request);
  }

  void releaseObjectInstanceName(const std::string& objectInstanceName)
    // throw (ObjectInstanceNameNotReserved,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    ObjectInstanceHandle objectInstanceHandle = _federate->takeReservedObjectInstanceName(objectInstanceName);
    if (!objectInstanceHandle.valid())
      throw ObjectInstanceNameNotReserved(objectInstanceName);

    // ... and send the release message to the rti
    SharedPtr<ReleaseMultipleObjectInstanceNameHandlePairsMessage> message;
    message = MakeShared<ReleaseMultipleObjectInstanceNameHandlePairsMessage>();
    message->setFederationHandle(getFederationHandle());
    message->getObjectInstanceHandleVector().push_back(objectInstanceHandle);

    send(message);
  }

  void reserveMultipleObjectInstanceName(const std::set<std::string>& objectInstanceNameSet)
    // throw (IllegalName,
    //        NameSetWasEmpty,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (objectInstanceNameSet.empty())
      throw NameSetWasEmpty("Empty object name set is not allowed!");

    SharedPtr<ReserveMultipleObjectInstanceNameRequestMessage> request;
    request = MakeShared<ReserveMultipleObjectInstanceNameRequestMessage>();
    request->getNameList().reserve(objectInstanceNameSet.size());
    for (std::set<std::string>::const_iterator i = objectInstanceNameSet.begin(); i != objectInstanceNameSet.end(); ++i) {
      if (i->empty())
        throw IllegalName("Empty object hames are not allowed!");
      if (i->compare(0, 3, "HLA") == 0)
        throw IllegalName("Object instance names starting with \"HLA\" are reserved for the RTI.");
      request->getNameList().push_back(*i);
    }
    request->setFederationHandle(getFederationHandle());
    request->setFederateHandle(getFederateHandle());

    send(request);
  }

  void releaseMultipleObjectInstanceName(const std::set<std::string>& objectInstanceNameSet)
    // throw (ObjectInstanceNameNotReserved,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    for (std::set<std::string>::const_iterator i = objectInstanceNameSet.begin(); i != objectInstanceNameSet.end(); ++i) {
      if (!_federate->objectInstanceNameReserved(*i))
        throw ObjectInstanceNameNotReserved(*i);
    }

    SharedPtr<ReleaseMultipleObjectInstanceNameHandlePairsMessage> message;
    message = MakeShared<ReleaseMultipleObjectInstanceNameHandlePairsMessage>();
    message->setFederationHandle(getFederationHandle());
    message->getObjectInstanceHandleVector().reserve(objectInstanceNameSet.size());
    for (std::set<std::string>::const_iterator i = objectInstanceNameSet.begin(); i != objectInstanceNameSet.end(); ++i) {
      ObjectInstanceHandle objectInstanceHandle = _federate->takeReservedObjectInstanceName(*i);
      OpenRTIAssert(objectInstanceHandle.valid());
      message->getObjectInstanceHandleVector().push_back(objectInstanceHandle);
    }
    send(message);
  }

  ObjectInstanceHandle registerObjectInstance(ObjectClassHandle objectClassHandle)
    // throw (ObjectClassNotDefined,
    //        ObjectClassNotPublished,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    // At first the complete error checks
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());
    if (Published != objectClass->getEffectivePublicationType())
      throw ObjectClassNotPublished(objectClass->getName());

    ObjectInstanceHandleNamePair handleNamePair = _getFreeObjectInstanceHandleNamePair();

    _federate->insertObjectInstance(handleNamePair.first, handleNamePair.second, objectClassHandle, true);

    SharedPtr<InsertObjectInstanceMessage> request;
    request = MakeShared<InsertObjectInstanceMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectClassHandle(objectClassHandle);
    request->setObjectInstanceHandle(handleNamePair.first);
    request->setName(handleNamePair.second);
    uint32_t numAttributes = objectClass->getNumAttributes();
    request->getAttributeStateVector().reserve(numAttributes);
    AttributeHandleVector attributeHandleVector;
    attributeHandleVector.reserve(numAttributes);
    for (uint32_t i = 0; i < numAttributes; ++i) {
      if (!objectClass->isAttributePublished(AttributeHandle(i)))
        continue;
      AttributeState attributeState;
      attributeState.setAttributeHandle(AttributeHandle(i));
      attributeState.setOwnerFederate(_federate->getFederateHandle());
      request->getAttributeStateVector().push_back(attributeState);
      attributeHandleVector.push_back(AttributeHandle(i));
    }
    send(request);

    if (!attributeHandleVector.empty()) {
      if (_federate->getAttributeScopeAdvisorySwitchEnabled()) {
        SharedPtr<AttributesInScopeMessage> message = MakeShared<AttributesInScopeMessage>();
        message->setObjectInstanceHandle(handleNamePair.first);
        // Copy this here since we might need it later
        message->setAttributeHandles(attributeHandleVector);
        message->setInScope(true);
        queueCallback(message);
      }
      if (_federate->getAttributeRelevanceAdvisorySwitchEnabled()) {
        SharedPtr<TurnUpdatesOnForInstanceMessage> message = MakeShared<TurnUpdatesOnForInstanceMessage>();
        message->setObjectInstanceHandle(handleNamePair.first);
        message->getAttributeHandles().swap(attributeHandleVector);
        message->setOn(true);
        // message->setUpdateRate(/*FIXME*/);
        queueCallback(message);
      }
    }

    return handleNamePair.first;
  }

  ObjectInstanceHandle registerObjectInstance(ObjectClassHandle objectClassHandle, const std::string& objectInstanceName,
                                              bool allowUnreservedObjectNames)
    // throw (ObjectClassNotDefined,
    //        ObjectClassNotPublished,
    //        ObjectInstanceNameNotReserved,
    //        ObjectInstanceNameInUse,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    // At first the complete error checks
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());
    if (Published != objectClass->getEffectivePublicationType())
      throw ObjectClassNotPublished(objectClass->getName());
    // Do not allow empty object instance names
    if (objectInstanceName.empty())
      throw ObjectInstanceNameNotReserved(objectInstanceName);

    // The already available objectInstanceHandle should be stored here.
    ObjectInstanceHandle objectInstanceHandle;
    objectInstanceHandle = _federate->takeReservedObjectInstanceName(objectInstanceName);

    // Check if we already have the object instance name reserved
    if (!objectInstanceHandle.valid()) {
      // If not, either policy tells us to just bail out - the standard rti1516 behaviour ...
      if (!allowUnreservedObjectNames) {
        throw ObjectInstanceNameNotReserved(objectInstanceName);
      } else {
        // Or, we try to reserve this object name behind the scenes.
        // Ok, if this is allowed, like for a rti13 federate or for the option
        // of allowing that to emulate certi behavior, we need to do the reservation
        // of the name now. This is the only syncronous operation in the rti.
        Clock abstime = (_operationWaitTimeout == kInfinite) ? Clock::max() : Clock::now() + Clock::fromMilliSeconds(_operationWaitTimeout);
        AbsTimeout timeout(abstime);
        objectInstanceHandle = dispatchWaitReserveObjectInstanceName(timeout, objectInstanceName);
        if (!objectInstanceHandle.valid())
          throw ObjectInstanceNameInUse(objectInstanceName);
      }
    }

    // Once we have survived, we know that the objectInstanceName given in the argument is unique and ours.
    // Also the object instance handle in this local scope must be valid and ours.

    _federate->insertObjectInstance(objectInstanceHandle, objectInstanceName, objectClassHandle, true);

    SharedPtr<InsertObjectInstanceMessage> request;
    request = MakeShared<InsertObjectInstanceMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectClassHandle(objectClassHandle);
    request->setObjectInstanceHandle(objectInstanceHandle);
    request->setName(objectInstanceName);
    uint32_t numAttributes = objectClass->getNumAttributes();
    request->getAttributeStateVector().reserve(numAttributes);
    AttributeHandleVector attributeHandleVector;
    attributeHandleVector.reserve(numAttributes);
    for (uint32_t i = 0; i < numAttributes; ++i) {
      if (!objectClass->isAttributePublished(AttributeHandle(i)))
        continue;
      AttributeState attributeState;
      attributeState.setAttributeHandle(AttributeHandle(i));
      attributeState.setOwnerFederate(_federate->getFederateHandle());
      request->getAttributeStateVector().push_back(attributeState);
      attributeHandleVector.push_back(AttributeHandle(i));
    }
    send(request);

    if (!attributeHandleVector.empty()) {
      if (_federate->getAttributeScopeAdvisorySwitchEnabled()) {
        SharedPtr<AttributesInScopeMessage> message = MakeShared<AttributesInScopeMessage>();
        message->setObjectInstanceHandle(objectInstanceHandle);
        // Copy this here since we might need it later
        message->setAttributeHandles(attributeHandleVector);
        message->setInScope(true);
        queueCallback(message);
      }
      if (_federate->getAttributeRelevanceAdvisorySwitchEnabled()) {
        SharedPtr<TurnUpdatesOnForInstanceMessage> message = MakeShared<TurnUpdatesOnForInstanceMessage>();
        message->setObjectInstanceHandle(objectInstanceHandle);
        message->getAttributeHandles().swap(attributeHandleVector);
        message->setOn(true);
        // message->setUpdateRate(/*FIXME*/);
        queueCallback(message);
      }
    }

    return objectInstanceHandle;
  }

  void updateAttributeValues(ObjectInstanceHandle objectInstanceHandle,
                             AttributeValueVector& attributeValues,
                             VariableLengthData& tag)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        AttributeNotOwned,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    // passels
    AttributeValueVector passels[2];
    for (std::vector<OpenRTI::AttributeValue>::iterator i = attributeValues.begin(); i != attributeValues.end(); ++i) {
      const Federate::InstanceAttribute* instanceAttribute = objectInstance->getInstanceAttribute(i->getAttributeHandle());
      if (!instanceAttribute)
        throw AttributeNotDefined(i->getAttributeHandle().toString());
      if (!instanceAttribute->getIsOwnedByFederate())
        throw AttributeNotOwned(i->getAttributeHandle().toString());
#ifdef DEBUG_PRINTF
      Federate::ObjectClass* objectClass = _federate->getObjectClass(objectInstance->getObjectClassHandle());
      if (objectClass->getEffectiveAttributeSubscriptionType(i->getAttributeHandle()) == Unsubscribed)
      {
        Federate::Attribute* classAttribute = objectClass->getAttribute(i->getAttributeHandle());
        DebugPrintf("%s: InstanceAttribute %s::%s of %s is unsubscribed\n", __FUNCTION__, objectClass->getFQName().c_str(), classAttribute->getName().c_str(), objectInstance->getName().c_str());
      }
#endif
      unsigned index = instanceAttribute->getTransportationType();
      passels[index].reserve(attributeValues.size());
      passels[index].push_back(AttributeValue());
      passels[index].back().setAttributeHandle(i->getAttributeHandle());
      passels[index].back().getValue().swap(i->getValue());

      //DebugPrintf("updateAttributeValues: objInst.getName=%s, objInst.getHandle=%s, objInst.AttrHandle=%s, objInst.subscription=%d\n",
      //  objectInstance->getName().c_str(),
      //  objectInstanceHandle.toString().c_str(),
      //  i->getAttributeHandle().toString().c_str(),
      //  objectInstance->getSubscriptionType() != Unsubscribed);
    }

    for (unsigned i = 0; i < 2; ++i) {
      if (passels[i].empty())
        continue;
      SharedPtr<AttributeUpdateMessage> request;
      request = MakeShared<AttributeUpdateMessage>();
      request->setFederationHandle(getFederationHandle());
      request->setFederateHandle(getFederateHandle());
      request->setObjectInstanceHandle(objectInstanceHandle);
      request->getAttributeValues().swap(passels[i]);
      request->setTransportationType(TransportationType(i));
      request->getTag().swap(tag);
      Federate::ObjectClass* objectClass = _federate->getObjectClass(objectInstance->getObjectClassHandle());
      if (objectInstance->getSubscriptionType() != Unsubscribed && objectClass->getDeliverToSelf())
      {
        queueReceiveOrderMessage(*request);
      }
      send(request);
    }
  }

  MessageRetractionHandle updateAttributeValues(ObjectInstanceHandle objectInstanceHandle,
                                                AttributeValueVector& attributeValues,
                                                VariableLengthData& tag,
                                                const NativeLogicalTime& nativeLogicalTime)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    bool timeRegulationEnabled = getTimeManagement()->getTimeRegulationEnabled();
    // passels
    AttributeValueVector passels[2][2];
    for (std::vector<OpenRTI::AttributeValue>::iterator i = attributeValues.begin(); i != attributeValues.end(); ++i) {
      const Federate::InstanceAttribute* instanceAttribute = objectInstance->getInstanceAttribute(i->getAttributeHandle());
      if (!instanceAttribute)
        throw AttributeNotDefined(i->getAttributeHandle().toString());
      if (!instanceAttribute->getIsOwnedByFederate())
        throw AttributeNotOwned(i->getAttributeHandle().toString());
      unsigned index0 = instanceAttribute->getTransportationType();
      unsigned index1 = RECEIVE;
      if (timeRegulationEnabled)
        index1 = instanceAttribute->getOrderType();
      passels[index0][index1].reserve(attributeValues.size());
      passels[index0][index1].push_back(AttributeValue());
      passels[index0][index1].back().setAttributeHandle(i->getAttributeHandle());
      passels[index0][index1].back().getValue().swap(i->getValue());

      //DebugPrintf("updateAttributeValues: objInst.getName=%s, objInst.getHandle=%s, objInst.AttrHandle=%s, objInst.subscription=%d\n",
      //  objectInstance->getName().c_str(),
      //  objectInstanceHandle.toString().c_str(),
      //  i->getAttributeHandle().toString().c_str(),
      //  objectInstance->getSubscriptionType() != Unsubscribed);
    }
    std::string reason;
    if (timeRegulationEnabled && getTimeManagement()->logicalTimeAlreadyPassed(nativeLogicalTime, reason))
      throw InvalidLogicalTime(reason);

    MessageRetractionHandle messageRetractionHandle = getNextMessageRetractionHandle();
    VariableLengthData timeStamp = getTimeManagement()->encodeLogicalTime(nativeLogicalTime);
    //DebugPrintf("Ambassador::%s: T=%s\n", __func__, getTimeManagement()->logicalTimeToString(nativeLogicalTime).c_str());

    for (unsigned i = 0; i < 2; ++i) {
      for (unsigned j = 0; j < 2; ++j) {
        if (passels[i][j].empty())
          continue;
        SharedPtr<TimeStampedAttributeUpdateMessage> request;
        request = MakeShared<TimeStampedAttributeUpdateMessage>();
        request->setFederationHandle(getFederationHandle());
        request->setFederateHandle(getFederateHandle());
        request->setObjectInstanceHandle(objectInstanceHandle);
        request->getAttributeValues().swap(passels[i][j]);
        request->setTimeStamp(timeStamp);
        request->setTransportationType(TransportationType(i));
        request->setOrderType(OrderType(j));
        request->getTag().swap(tag);
        request->setMessageRetractionHandle(messageRetractionHandle);
        Federate::ObjectClass* objectClass = _federate->getObjectClass(objectInstance->getObjectClassHandle());
        if (objectInstance->getSubscriptionType() != Unsubscribed && objectClass->getDeliverToSelf())
        {
          if (OrderType(j) == OrderType::TIMESTAMP)
          {
            queueTimeStampedMessage(timeStamp, *request, true);
          }
          else
          {
            queueReceiveOrderMessage(*request);
          }
        }
        send(request);
      }
    }

    return messageRetractionHandle;
  }

  void sendInteraction(InteractionClassHandle interactionClassHandle, std::vector<ParameterValue>& parameterValues,
                       VariableLengthData& tag)
    // throw (InteractionClassNotPublished,
    //        InteractionClassNotDefined,
    //        InteractionParameterNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());
    if (!interactionClass->isPublished())
      throw InteractionClassNotPublished(interactionClassHandle.toString());
    for (std::vector<ParameterValue>::const_iterator i = parameterValues.begin(); i != parameterValues.end(); ++i)
      if (!interactionClass->getParameter(i->getParameterHandle()))
        throw InteractionParameterNotDefined(i->getParameterHandle().toString());

    SharedPtr<InteractionMessage> request;
    request = MakeShared<InteractionMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setFederateHandle(getFederateHandle());
    request->setInteractionClassHandle(interactionClassHandle);
    request->setTransportationType(interactionClass->getTransportationType());
    request->getTag().swap(tag);
    request->getParameterValues().swap(parameterValues);
    if (interactionClass->getSubscriptionType() != Unsubscribed && interactionClass->getDeliverToSelf())
    {
      queueReceiveOrderMessage(*request);
    }
    send(request);
  }

  MessageRetractionHandle sendInteraction(InteractionClassHandle interactionClassHandle,
                                          std::vector<ParameterValue>& parameterValues,
                                          VariableLengthData& tag,
                                          const NativeLogicalTime& logicalTime)
    // throw (InteractionClassNotPublished,
    //        InteractionClassNotDefined,
    //        InteractionParameterNotDefined,
    //        InvalidLogicalTime,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();

    const Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());
    if (!interactionClass->isPublished())
      throw InteractionClassNotPublished(interactionClassHandle.toString());
    for (std::vector<ParameterValue>::const_iterator i = parameterValues.begin(); i != parameterValues.end(); ++i)
      if (!interactionClass->getParameter(i->getParameterHandle()))
        throw InteractionParameterNotDefined(i->getParameterHandle().toString());
    bool timeRegulationEnabled = getTimeManagement()->getTimeRegulationEnabled();

    std::string reason;
    if (timeRegulationEnabled && getTimeManagement()->logicalTimeAlreadyPassed(logicalTime, reason))
    {
      throw InvalidLogicalTime(reason);
    }
    MessageRetractionHandle messageRetractionHandle = getNextMessageRetractionHandle();
    VariableLengthData timeStamp = getTimeManagement()->encodeLogicalTime(logicalTime);
    SharedPtr<TimeStampedInteractionMessage> request;
    request = MakeShared<TimeStampedInteractionMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setFederateHandle(getFederateHandle());
    request->setInteractionClassHandle(interactionClassHandle);
    if (timeRegulationEnabled)
      request->setOrderType(interactionClass->getOrderType());
    else
      request->setOrderType(RECEIVE);
    request->setTransportationType(interactionClass->getTransportationType());
    request->getTag().swap(tag);
    request->setTimeStamp(timeStamp);
    request->setMessageRetractionHandle(messageRetractionHandle);
    request->getParameterValues().swap(parameterValues);

    if (interactionClass->getSubscriptionType() != Unsubscribed && interactionClass->getDeliverToSelf())
    {
      if (request->getOrderType() == OrderType::TIMESTAMP)
      {
        //DebugPrintf("Ambassador::%s: send to self: T=%s\n", __func__, getTimeManagement()->logicalTimeToString(logicalTime).c_str());
        queueTimeStampedMessage(timeStamp, *request, true);
      }
      else
      {
        queueReceiveOrderMessage(*request);
      }
    }
    send(request);

    return messageRetractionHandle;
  }

  void deleteObjectInstance(ObjectInstanceHandle objectInstanceHandle, VariableLengthData& tag)
    // throw (DeletePrivilegeNotHeld,
    //        ObjectInstanceNotKnown,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    if (!objectInstance->isOwnedByFederate())
      throw DeletePrivilegeNotHeld(objectInstanceHandle.toString());

    SharedPtr<DeleteObjectInstanceMessage> request;
    request = MakeShared<DeleteObjectInstanceMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setFederateHandle(getFederateHandle());
    request->setObjectInstanceHandle(objectInstanceHandle);
    request->getTag().swap(tag);

    send(request);

    // Note that this also sends the unreference message just past the delete
    _releaseObjectInstance(objectInstanceHandle);
  }

  MessageRetractionHandle deleteObjectInstance(ObjectInstanceHandle objectInstanceHandle, VariableLengthData& tag, const NativeLogicalTime& logicalTime)
    // throw (DeletePrivilegeNotHeld,
    //        ObjectInstanceNotKnown,
    //        InvalidLogicalTime,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    const Federate::InstanceAttribute* instanceAttribute = objectInstance->getInstanceAttribute(AttributeHandle(0));
    if (!instanceAttribute || !objectInstance->isOwnedByFederate())
      throw DeletePrivilegeNotHeld(objectInstanceHandle.toString());
    bool timeRegulationEnabled = getTimeManagement()->getTimeRegulationEnabled();
    if (timeRegulationEnabled && getTimeManagement()->logicalTimeAlreadyPassed(logicalTime))
      throw InvalidLogicalTime(getTimeManagement()->logicalTimeToString(logicalTime));

    MessageRetractionHandle messageRetractionHandle = getNextMessageRetractionHandle();

    SharedPtr<TimeStampedDeleteObjectInstanceMessage> request;
    request = MakeShared<TimeStampedDeleteObjectInstanceMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setFederateHandle(getFederateHandle());
    request->setObjectInstanceHandle(objectInstanceHandle);
    if (timeRegulationEnabled)
      request->setOrderType(instanceAttribute->getOrderType());
    else
      request->setOrderType(RECEIVE);
    request->getTag().swap(tag);
    request->setTimeStamp(getTimeManagement()->encodeLogicalTime(logicalTime));
    request->setMessageRetractionHandle(messageRetractionHandle);

    send(request);

    // FIXME do this once the logical time has passed
    // When implementing message retraction this needs to be delayed probably ...
    // Note that this also sends the unreference message just past the delete
    _releaseObjectInstance(objectInstanceHandle);

    // FIXME: see above - make sure we do not hand a valid retraction handle
    // over to the user until this is implemented.
    // return messageRetractionHandle;
    return MessageRetractionHandle();
  }

  void localDeleteObjectInstance(ObjectInstanceHandle objectInstanceHandle)
    // throw (ObjectInstanceNotKnown,
    //        FederateOwnsAttributes,
    //        OwnershipAcquisitionPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    if (objectInstance->ownsAnyAttribute())
      throw FederateOwnsAttributes(objectInstanceHandle.toString());

    throw RTIinternalError("Not implemented!");
  }

  void changeAttributeTransportationType(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector, TransportationType transportationType)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        AttributeNotOwned,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    for (AttributeHandleVector::const_iterator j = attributeHandleVector.begin(); j != attributeHandleVector.end(); ++j) {
      const Federate::InstanceAttribute* attribute = objectInstance->getInstanceAttribute(j->getHandle());
      if (!attribute)
        throw AttributeNotDefined(j->toString());
      if (!attribute->getIsOwnedByFederate())
        throw AttributeNotOwned(j->toString());
    }
    for (AttributeHandleVector::const_iterator j = attributeHandleVector.begin(); j != attributeHandleVector.end(); ++j) {
      objectInstance->getInstanceAttribute(j->getHandle())->setTransportationType(transportationType);
    }
  }

  void changeInteractionTransportationType(InteractionClassHandle interactionClassHandle, TransportationType transportationType)
    // throw (InteractionClassNotDefined,
    //        InteractionClassNotPublished,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());
    if (!interactionClass->isPublished())
      throw InteractionClassNotPublished(interactionClass->getName());
    interactionClass->setTransportationType(transportationType);
  }

  void requestAttributeValueUpdate(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector& attributeHandleVector,
                                   VariableLengthData& tag)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    for (AttributeHandleVector::const_iterator j = attributeHandleVector.begin(); j != attributeHandleVector.end(); ++j) {
      if (!objectInstance->getInstanceAttribute(j->getHandle()))
        throw AttributeNotDefined(j->toString());
    }

   // Implicit resubscribe on requestAttributeValueUpdate
   objectInstance->setSubscriptionType(SubscribedPassive);

    SharedPtr<RequestAttributeUpdateMessage> request;
    request = MakeShared<RequestAttributeUpdateMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectInstanceHandle(objectInstanceHandle);
    request->getAttributeHandles().swap(attributeHandleVector);
    request->getTag().swap(tag);
    send(request);

  }

  void requestAttributeValueUpdate(ObjectClassHandle objectClassHandle, AttributeHandleVector& attributeHandleVector,
                                   VariableLengthData& tag)
    // throw (ObjectClassNotDefined,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());
    for (AttributeHandleVector::const_iterator j = attributeHandleVector.begin(); j != attributeHandleVector.end(); ++j) {
      if (!objectClass->getAttribute(*j))
        throw AttributeNotDefined(j->toString());
    }

    SharedPtr<RequestClassAttributeUpdateMessage> request;
    request = MakeShared<RequestClassAttributeUpdateMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectClassHandle(objectClassHandle);
    request->getAttributeHandles().swap(attributeHandleVector);
    request->getTag().swap(tag);
    send(request);
  }

  void requestAttributeTransportationTypeChange(ObjectInstanceHandle, const AttributeHandleVector&, TransportationType)
    // throw (AttributeAlreadyBeingChanged,
    //        AttributeNotOwned,
    //        AttributeNotDefined,
    //        ObjectInstanceNotKnown,
    //        InvalidTransportationType,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void queryAttributeTransportationType(ObjectInstanceHandle, AttributeHandle)
    // throw (AttributeNotDefined,
    //        ObjectInstanceNotKnown,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void requestInteractionTransportationTypeChange(InteractionClassHandle, TransportationType)
    // throw (InteractionClassAlreadyBeingChanged,
    //        InteractionClassNotPublished,
    //        InteractionClassNotDefined,
    //        InvalidTransportationType,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void queryInteractionTransportationType(FederateHandle, InteractionClassHandle)
    // throw (InteractionClassNotDefined,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void unconditionalAttributeOwnershipDivestiture(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector& attributeHandleVector)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        AttributeNotOwned,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void negotiatedAttributeOwnershipDivestiture(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector& attributeHandleVector, VariableLengthData& tag)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        AttributeNotOwned,
    //        AttributeAlreadyBeingDivested,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void confirmDivestiture(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector& attributeHandleVector, VariableLengthData& tag)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        AttributeNotOwned,
    //        AttributeDivestitureWasNotRequested,
    //        NoAcquisitionPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void attributeOwnershipAcquisition(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector& attributeHandleVector, VariableLengthData& tag)
    // throw (ObjectInstanceNotKnown,
    //        ObjectClassNotPublished,
    //        AttributeNotDefined,
    //        AttributeNotPublished,
    //        FederateOwnsAttributes,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void attributeOwnershipAcquisitionIfAvailable(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector& attributeHandleVector)
    // throw (ObjectInstanceNotKnown,
    //        ObjectClassNotPublished,
    //        AttributeNotDefined,
    //        AttributeNotPublished,
    //        FederateOwnsAttributes,
    //        AttributeAlreadyBeingAcquired,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void attributeOwnershipReleaseDenied(ObjectInstanceHandle, AttributeHandleVector&)
    // throw (AttributeNotOwned,
    //        AttributeNotDefined,
    //        ObjectInstanceNotKnown,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void attributeOwnershipDivestitureIfWanted(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector,
                                             AttributeHandleVector& divestedAttributes /*this is output*/)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        AttributeNotOwned,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void cancelNegotiatedAttributeOwnershipDivestiture(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector& attributeHandleVector)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        AttributeNotOwned,
    //        AttributeDivestitureWasNotRequested,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void cancelAttributeOwnershipAcquisition(ObjectInstanceHandle objectInstanceHandle, AttributeHandleVector& attributeHandleVector)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        AttributeAlreadyOwned,
    //        AttributeAcquisitionWasNotRequested,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void queryAttributeOwnership(ObjectInstanceHandle objectInstanceHandle, AttributeHandle attributeHandle)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    const Federate::InstanceAttribute* attribute = objectInstance->getInstanceAttribute(attributeHandle);
    if (!attribute)
      throw AttributeNotDefined(attributeHandle.toString());
    SharedPtr<QueryAttributeOwnershipRequestMessage> request;
    request = MakeShared<QueryAttributeOwnershipRequestMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setObjectInstanceHandle(objectInstanceHandle);
    request->setAttributeHandle(attributeHandle);
    send(request);
  }

  bool isAttributeOwnedByFederate(ObjectInstanceHandle objectInstanceHandle, AttributeHandle attributeHandle)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    const Federate::InstanceAttribute* attribute = objectInstance->getInstanceAttribute(attributeHandle);
    if (!attribute)
      throw AttributeNotDefined(attributeHandle.toString());
    return attribute->getIsOwnedByFederate();
  }

  void enableTimeRegulation(const NativeLogicalTimeInterval& lookahead)
    // throw (TimeRegulationAlreadyEnabled,
    //        InvalidLookahead,
    //        InTimeAdvancingState,
    //        RequestForTimeRegulationPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->getTimeRegulationEnabled())
      throw TimeRegulationAlreadyEnabled();
    if (_timeManagement->getTimeRegulationEnablePending())
      throw RequestForTimeRegulationPending();
    if (_timeManagement->getTimeAdvancePending())
      throw InTimeAdvancingState();
    if (!_timeManagement->isPositiveLogicalTimeInterval(lookahead))
      throw InvalidLookahead(_timeManagement->logicalTimeIntervalToString(lookahead));
    _timeManagement->enableTimeRegulation(*this, lookahead);
  }

  // the RTI13 variant
  void enableTimeRegulation(const NativeLogicalTime& logicalTime, const NativeLogicalTimeInterval& lookahead)
    // throw (TimeRegulationAlreadyEnabled,
    //        InvalidLogicalTime,
    //        InvalidLookahead,
    //        InTimeAdvancingState,
    //        RequestForTimeRegulationPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->getTimeRegulationEnabled())
      throw TimeRegulationAlreadyEnabled();
    if (_timeManagement->getTimeRegulationEnablePending())
      throw RequestForTimeRegulationPending();
    if (_timeManagement->getTimeAdvancePending())
      throw InTimeAdvancingState();
    if (_timeManagement->isLogicalTimeInThePast(logicalTime))
      throw InvalidLogicalTime(_timeManagement->logicalTimeToString(logicalTime));
    if (!_timeManagement->isPositiveLogicalTimeInterval(lookahead))
      throw InvalidLookahead(_timeManagement->logicalTimeIntervalToString(lookahead));
    _timeManagement->enableTimeRegulation(*this, logicalTime, lookahead);
  }

  void disableTimeRegulation()
    // throw (TimeRegulationIsNotEnabled,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (!_timeManagement->getTimeRegulationEnabled())
      throw TimeRegulationIsNotEnabled();
    _timeManagement->disableTimeRegulation(*this);
  }

  void enableTimeConstrained()
    // throw (TimeConstrainedAlreadyEnabled,
    //        InTimeAdvancingState,
    //        RequestForTimeConstrainedPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->getTimeConstrainedEnabled())
      throw TimeConstrainedAlreadyEnabled();
    if (_timeManagement->getTimeConstrainedEnablePending())
      throw RequestForTimeConstrainedPending();
    if (_timeManagement->getTimeAdvancePending())
      throw InTimeAdvancingState();
    _timeManagement->enableTimeConstrained(*this);
  }

  void disableTimeConstrained()
    // throw (TimeConstrainedIsNotEnabled,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (!_timeManagement->getTimeConstrainedEnabled())
      throw TimeConstrainedIsNotEnabled();
    _timeManagement->disableTimeConstrained(*this);
  }

  void timeAdvanceRequest(const NativeLogicalTime& logicalTime)
    // throw (InvalidLogicalTime,
    //        LogicalTimeAlreadyPassed,
    //        InTimeAdvancingState,
    //        RequestForTimeRegulationPending,
    //        RequestForTimeConstrainedPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->isLogicalTimeInThePast(logicalTime))
      throw LogicalTimeAlreadyPassed(_timeManagement->logicalTimeToString(logicalTime));
    if (_timeManagement->getTimeAdvancePending())
      throw InTimeAdvancingState();
    if (_timeManagement->getTimeRegulationEnablePending())
      throw RequestForTimeRegulationPending();
    if (_timeManagement->getTimeConstrainedEnablePending())
      throw RequestForTimeConstrainedPending();
    _timeManagement->timeAdvanceRequest(*this, logicalTime, InternalTimeManagement::TimeAdvanceRequest);
  }

  void timeAdvanceRequestAvailable(const NativeLogicalTime& logicalTime)
    // throw (InvalidLogicalTime,
    //        LogicalTimeAlreadyPassed,
    //        InTimeAdvancingState,
    //        RequestForTimeRegulationPending,
    //        RequestForTimeConstrainedPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->isLogicalTimeInThePast(logicalTime))
      throw LogicalTimeAlreadyPassed(_timeManagement->logicalTimeToString(logicalTime));
    if (_timeManagement->getTimeAdvancePending())
      throw InTimeAdvancingState();
    if (_timeManagement->getTimeRegulationEnablePending())
      throw RequestForTimeRegulationPending();
    if (_timeManagement->getTimeConstrainedEnablePending())
      throw RequestForTimeConstrainedPending();
    _timeManagement->timeAdvanceRequest(*this, logicalTime, InternalTimeManagement::TimeAdvanceRequestAvailable);
  }

  void nextMessageRequest(const NativeLogicalTime& logicalTime)
    // throw (InvalidLogicalTime,
    //        LogicalTimeAlreadyPassed,
    //        InTimeAdvancingState,
    //        RequestForTimeRegulationPending,
    //        RequestForTimeConstrainedPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->isLogicalTimeInThePast(logicalTime))
      throw LogicalTimeAlreadyPassed(_timeManagement->logicalTimeToString(logicalTime));
    if (_timeManagement->getTimeAdvancePending())
      throw InTimeAdvancingState();
    if (_timeManagement->getTimeRegulationEnablePending())
      throw RequestForTimeRegulationPending();
    if (_timeManagement->getTimeConstrainedEnablePending())
      throw RequestForTimeConstrainedPending();
    _timeManagement->timeAdvanceRequest(*this, logicalTime, InternalTimeManagement::NextMessageRequest);
  }

  void nextMessageRequestAvailable(const NativeLogicalTime& logicalTime)
    // throw (InvalidLogicalTime,
    //        LogicalTimeAlreadyPassed,
    //        InTimeAdvancingState,
    //        RequestForTimeRegulationPending,
    //        RequestForTimeConstrainedPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->isLogicalTimeInThePast(logicalTime))
      throw LogicalTimeAlreadyPassed(_timeManagement->logicalTimeToString(logicalTime));
    if (_timeManagement->getTimeAdvancePending())
      throw InTimeAdvancingState();
    if (_timeManagement->getTimeRegulationEnablePending())
      throw RequestForTimeRegulationPending();
    if (_timeManagement->getTimeConstrainedEnablePending())
      throw RequestForTimeConstrainedPending();
    _timeManagement->timeAdvanceRequest(*this, logicalTime, InternalTimeManagement::NextMessageRequestAvailable);
  }

  void flushQueueRequest(const NativeLogicalTime& logicalTime)
    // throw (InvalidLogicalTime,
    //        LogicalTimeAlreadyPassed,
    //        InTimeAdvancingState,
    //        RequestForTimeRegulationPending,
    //        RequestForTimeConstrainedPending,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->isLogicalTimeInThePast(logicalTime))
      throw LogicalTimeAlreadyPassed(_timeManagement->logicalTimeToString(logicalTime));
    if (_timeManagement->getTimeAdvancePending())
      throw InTimeAdvancingState();
    if (_timeManagement->getTimeRegulationEnablePending())
      throw RequestForTimeRegulationPending();
    if (_timeManagement->getTimeConstrainedEnablePending())
      throw RequestForTimeConstrainedPending();
    _timeManagement->timeAdvanceRequest(*this, logicalTime, InternalTimeManagement::FlushQueueRequest);
  }

  void enableAsynchronousDelivery()
    // throw (AsynchronousDeliveryAlreadyEnabled,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->getAsynchronousDeliveryEnabled())
      throw AsynchronousDeliveryAlreadyEnabled();
    _timeManagement->setAsynchronousDeliveryEnabled(true);
  }

  void disableAsynchronousDelivery()
    // throw (AsynchronousDeliveryAlreadyDisabled,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (!_timeManagement->getAsynchronousDeliveryEnabled())
      throw AsynchronousDeliveryAlreadyDisabled();
    _timeManagement->setAsynchronousDeliveryEnabled(false);
  }

  void allowPendingTimeInNextMessageRequest()
    // throw (AsynchronousDeliveryAlreadyEnabled,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (_timeManagement->getAllowPendingTimeInNextMessageRequest())
      throw AllowPendingTimeInNextMessageRequestAlreadyEnabled();
    _timeManagement->setAllowPendingTimeInNextMessageRequest(true);
  }

  void disallowPendingTimeInNextMessageRequest()
    // throw (AsynchronousDeliveryAlreadyDisabled,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (!_timeManagement->getAllowPendingTimeInNextMessageRequest())
      throw AllowPendingTimeInNextMessageRequestAlreadyDisabled();
    _timeManagement->setAllowPendingTimeInNextMessageRequest(false);
  }

  bool queryGALT(NativeLogicalTime& logicalTime)
    // throw (FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    return _timeManagement->queryGALT(*this, logicalTime);
  }

  void queryLogicalTime(NativeLogicalTime& logicalTime)
    // throw (FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    _timeManagement->queryLogicalTime(*this, logicalTime);
  }

  bool queryLITS(NativeLogicalTime& logicalTime)
    // throw (FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    return _timeManagement->queryLITS(*this, logicalTime);
  }

  void modifyLookahead(const NativeLogicalTimeInterval& lookahead, bool checkForTimeRegulation)
    // throw (TimeRegulationIsNotEnabled,
    //        InvalidLookahead,
    //        InTimeAdvancingState,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (checkForTimeRegulation && !_timeManagement->getTimeRegulationEnabled())
      throw TimeRegulationIsNotEnabled();
    if (!_timeManagement->isPositiveLogicalTimeInterval(lookahead))
      throw InvalidLookahead(_timeManagement->logicalTimeIntervalToString(lookahead));
    if (_timeManagement->getTimeAdvancePending() || _timeManagement->getTimeConstrainedEnablePending())
      throw InTimeAdvancingState();
    _timeManagement->modifyLookahead(*this, lookahead);
  }

  void queryLookahead(NativeLogicalTimeInterval& logicalTimeInterval, bool checkForTimeRegulation)
    // throw (TimeRegulationIsNotEnabled,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!_timeManagement.valid())
      throw FederateNotExecutionMember();
    if (checkForTimeRegulation && !_timeManagement->getTimeRegulationEnabled())
      throw TimeRegulationIsNotEnabled();
    _timeManagement->queryLookahead(*this, logicalTimeInterval);
  }

  void retract(MessageRetractionHandle messageRetractionHandle)
    // throw (InvalidMessageRetractionHandle,
    //        TimeRegulationIsNotEnabled,
    //        MessageCanNoLongerBeRetracted,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (!messageRetractionHandle.valid())
      throw InvalidMessageRetractionHandle(messageRetractionHandle.toString());
    if (messageRetractionHandle.getFederateHandle() != getFederateHandle())
      throw InvalidMessageRetractionHandle(messageRetractionHandle.toString());
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (!getTimeManagement()->getTimeRegulationEnabled())
      throw TimeRegulationIsNotEnabled();

    throw RTIinternalError("Message retraction is not implemented!");
  }

  void changeAttributeOrderType(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector, OrderType orderType)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        AttributeNotOwned,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    for (AttributeHandleVector::const_iterator j = attributeHandleVector.begin(); j != attributeHandleVector.end(); ++j) {
      const Federate::InstanceAttribute* instanceAttribute = objectInstance->getInstanceAttribute(*j);
      if (!instanceAttribute)
        throw AttributeNotDefined(j->toString());
      if (!instanceAttribute->getIsOwnedByFederate())
        throw AttributeNotOwned(j->toString());
    }
    for (AttributeHandleVector::const_iterator j = attributeHandleVector.begin(); j != attributeHandleVector.end(); ++j) {
      objectInstance->getInstanceAttribute(*j)->setOrderType(orderType);
    }
  }

  void changeInteractionOrderType(InteractionClassHandle interactionClassHandle, OrderType orderType)
    // throw (InteractionClassNotDefined,
    //        InteractionClassNotPublished,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());
    if (!interactionClass->isPublished())
      throw InteractionClassNotPublished(interactionClass->getName());
    interactionClass->setOrderType(orderType);
  }

  RegionHandle createRegion(const DimensionHandleSet& dimensionHandleSet)
    // throw (InvalidDimensionHandle,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    for (DimensionHandleSet::const_iterator i = dimensionHandleSet.begin(); i != dimensionHandleSet.end(); ++i) {
      if (!_federate->getDimension(*i))
        throw InvalidDimensionHandle(i->toString());
    }
    RegionHandle regionHandle = _federate->insertLocalRegion(dimensionHandleSet);

    SharedPtr<InsertRegionMessage> request = MakeShared<InsertRegionMessage>();
    request->setFederationHandle(getFederationHandle());
    RegionHandleDimensionHandleSetPairVector value(1);
    value[0].first = regionHandle;
    value[0].second = dimensionHandleSet;
    request->getRegionHandleDimensionHandleSetPairVector().swap(value);
    send(request);

    return regionHandle;
  }

  void commitRegionModifications(RegionHandleVector& regionHandleVector)
    // throw (InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    RegionHandleRegionValuePairVector regionHandleRegionValuePairVector;
    regionHandleRegionValuePairVector.reserve(regionHandleVector.size());
    for (RegionHandleVector::const_iterator i = regionHandleVector.begin(); i != regionHandleVector.end(); ++i) {
      if (!i->valid())
        throw InvalidRegion(i->toString());
      if (i->getFederateHandle() != getFederateHandle())
        throw RegionNotCreatedByThisFederate(i->toString());
      Federate::RegionData* region = _federate->getRegion(*i);
      if (!region)
        throw InvalidRegion(i->toString());
      region->getRegion().getRegionValue(regionHandleRegionValuePairVector.back().second);
    }
    SharedPtr<CommitRegionMessage> request = MakeShared<CommitRegionMessage>();
    request->setFederationHandle(getFederationHandle());
    request->getRegionHandleRegionValuePairVector().swap(regionHandleRegionValuePairVector);
    send(request);
  }

  void deleteRegion(RegionHandle regionHandle)
    // throw (InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        RegionInUseForUpdateOrSubscription,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (!regionHandle.valid())
      throw InvalidRegion(regionHandle.toString());
    Federate::RegionData* region = _federate->getRegion(regionHandle);
    if (!region)
      throw InvalidRegion(regionHandle.toString());
    if (regionHandle.getFederateHandle() != getFederateHandle())
      throw RegionNotCreatedByThisFederate(regionHandle.toString());

    // FIXME check for in use
    _federate->eraseLocalRegion(regionHandle);

    SharedPtr<EraseRegionMessage> request = MakeShared<EraseRegionMessage>();
    request->setFederationHandle(getFederationHandle());
    RegionHandleVector value(1);
    value[0] = regionHandle;
    request->getRegionHandleVector().swap(value);

    send(request);
  }

  ObjectInstanceHandle registerObjectInstanceWithRegions(ObjectClassHandle objectClassHandle,
                                                         AttributeHandleVectorRegionHandleVectorPairVector&
                                                         attributeHandleVectorRegionHandleVectorPairVector)
    // throw (ObjectClassNotDefined,
    //        ObjectClassNotPublished,
    //        AttributeNotDefined,
    //        AttributeNotPublished,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        InvalidRegionContext,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
    //return ObjectInstanceHandle();
  }

  ObjectInstanceHandle
    registerObjectInstanceWithRegions(ObjectClassHandle objectClassHandle,
                                      AttributeHandleVectorRegionHandleVectorPairVector&
                                      attributeHandleVectorRegionHandleVectorPairVector,
                                      const std::string& objectInstanceName)
    // throw (ObjectClassNotDefined,
    //        ObjectClassNotPublished,
    //        AttributeNotDefined,
    //        AttributeNotPublished,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        InvalidRegionContext,
    //        ObjectInstanceNameNotReserved,
    //        ObjectInstanceNameInUse,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
    //return ObjectInstanceHandle();
  }

  void associateRegionsForUpdates(ObjectInstanceHandle objectInstanceHandle,
                                  AttributeHandleVectorRegionHandleVectorPairVector&
                                  attributeHandleVectorRegionHandleVectorPairVector)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        InvalidRegionContext,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void unassociateRegionsForUpdates(ObjectInstanceHandle objectInstanceHandle,
                                    AttributeHandleVectorRegionHandleVectorPairVector&
                                    attributeHandleVectorRegionHandleVectorPairVector)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void subscribeObjectClassAttributesWithRegions(ObjectClassHandle objectClassHandle,
                                                 AttributeHandleVectorRegionHandleVectorPairVector&
                                                 attributeHandleVectorRegionHandleVectorPairVector,
                                                 bool active, const std::string& updateRateDesignator)
    // throw (ObjectClassNotDefined,
    //        AttributeNotDefined,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        InvalidRegionContext,
    //        FederateNotExecutionMember,
    //        InvalidUpdateRateDesignator,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void unsubscribeObjectClassAttributesWithRegions(ObjectClassHandle objectClassHandle,
                                                   AttributeHandleVectorRegionHandleVectorPairVector&
                                                   attributeHandleVectorRegionHandleVectorPairVector)
    // throw (ObjectClassNotDefined,
    //        AttributeNotDefined,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void subscribeInteractionClassWithRegions(InteractionClassHandle objectClassHandle, RegionHandleVector& regionHandleVector, bool active)
    // throw (InteractionClassNotDefined,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        InvalidRegionContext,
    //        FederateServiceInvocationsAreBeingReportedViaMOM,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void unsubscribeInteractionClassWithRegions(InteractionClassHandle objectClassHandle, RegionHandleVector& regionHandleVector)
    // throw (InteractionClassNotDefined,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  void sendInteractionWithRegions(InteractionClassHandle interactionClassHandle,
                                  std::vector<ParameterValue>& parameterValues,
                                  RegionHandleVector& regionHandleVector,
                                  VariableLengthData& tag)
    // throw (InteractionClassNotDefined,
    //        InteractionClassNotPublished,
    //        InteractionParameterNotDefined,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        InvalidRegionContext,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  MessageRetractionHandle
    sendInteractionWithRegions(InteractionClassHandle interactionClassHandle,
                               std::vector<ParameterValue>& parameterValues,
                               RegionHandleVector& regionHandleVector,
                               VariableLengthData& tag,
                               const NativeLogicalTime& logicalTime)
    // throw (InteractionClassNotDefined,
    //        InteractionClassNotPublished,
    //        InteractionParameterNotDefined,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        InvalidRegionContext,
    //        InvalidLogicalTime,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
    //return MessageRetractionHandle();
  }

  void requestAttributeValueUpdateWithRegions(ObjectClassHandle objectClassHandle,
                                              AttributeHandleVectorRegionHandleVectorPairVector&
                                              attributeHandleVectorRegionHandleVectorPairVector,
                                              VariableLengthData& tag)
    // throw (ObjectClassNotDefined,
    //        AttributeNotDefined,
    //        InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        InvalidRegionContext,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    throw RTIinternalError("Not implemented");
  }

  ResignAction getAutomaticResignDirective()
    // throw (FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    return _federate->getAutomaticResignDirective();
  }

  void setAutomaticResignDirective(ResignAction resignAction)
    // throw (InvalidResignAction,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    switch (resignAction) {
    case UNCONDITIONALLY_DIVEST_ATTRIBUTES:
    case DELETE_OBJECTS:
    case CANCEL_PENDING_OWNERSHIP_ACQUISITIONS:
    case DELETE_OBJECTS_THEN_DIVEST:
    case CANCEL_THEN_DELETE_THEN_DIVEST:
    case NO_ACTION:
      break;
    default:
      throw InvalidResignAction();
    }

    if (_federate->getAutomaticResignDirective() == resignAction)
      return;

    _federate->setAutomaticResignDirective(resignAction);

    SharedPtr<ChangeAutomaticResignDirectiveMessage> message;
    message = MakeShared<ChangeAutomaticResignDirectiveMessage>();
    message->setFederationHandle(getFederationHandle());
    message->setFederateHandle(getFederateHandle());
    message->setResignAction(resignAction);
    send(message);
  }

  FederateHandle getFederateHandle(const std::string& name)
    // throw (NameNotFound,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (_federate->getFederateName() == name)
      return _federate->getFederateHandle();
    FederateHandle federateHandle = _federate->getFederateHandle(name);
    if (!federateHandle.valid())
      throw NameNotFound(name);
    return federateHandle;
  }

  const std::string& getFederateName(FederateHandle federateHandle)
    // throw (InvalidFederateHandle,
    //        FederateHandleNotKnown,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (_federate->getFederateHandle() == federateHandle)
      return _federate->getFederateName();
    const Federate::_Federate* federate = _federate->getFederate(federateHandle);
    if (!federate)
      throw InvalidFederateHandle(federateHandle.toString());
    return federate->getName();
  }

  ObjectClassHandle getObjectClassHandle(const std::string& name)
    // throw (NameNotFound,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    ObjectClassHandle objectClassHandle = _federate->getObjectClassHandle(name);
    if (!objectClassHandle.valid())
      throw NameNotFound(name);
    return objectClassHandle;
  }

  const std::string& getObjectClassName(ObjectClassHandle objectClassHandle)
    // throw (InvalidObjectClassHandle,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw InvalidObjectClassHandle(objectClassHandle.toString());
    return objectClass->getName();
  }

  AttributeHandle getAttributeHandle(ObjectClassHandle objectClassHandle, const std::string& name)
    // throw (InvalidObjectClassHandle,
    //        NameNotFound,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw InvalidObjectClassHandle(objectClassHandle.toString());
    AttributeHandle attributeHandle = objectClass->getAttributeHandle(name);
    if (!attributeHandle.valid())
      throw NameNotFound(name);
    return attributeHandle;
  }

  const std::string& getAttributeName(ObjectClassHandle objectClassHandle, AttributeHandle attributeHandle)
    // throw (InvalidObjectClassHandle,
    //        InvalidAttributeHandle,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw InvalidObjectClassHandle(objectClassHandle.toString());
    const Federate::Attribute* attribute = objectClass->getAttribute(attributeHandle);
    if (!attribute)
      throw InvalidAttributeHandle(attributeHandle.toString());
    return attribute->getName();
  }

  std::string getAttributeDataType(ObjectClassHandle objectClassHandle, AttributeHandle attributeHandle)
    // throw (InvalidObjectClassHandle,
    //        NameNotFound,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw InvalidObjectClassHandle(objectClassHandle.toString());
    const Federate::Attribute* attribute = objectClass->getAttribute(attributeHandle);
    if (!attribute)
      throw InvalidAttributeHandle(attributeHandle.toString());
    return attribute->getDataType();
  }

  double getUpdateRateValue(const std::string& updateRateDesignator)
    // throw (InvalidUpdateRateDesignator,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    double updateRateValue = _federate->getUpdateRateValue(updateRateDesignator);
    if (updateRateValue < 0)
      throw InvalidUpdateRateDesignator(updateRateDesignator);
    return updateRateValue;
  }

  double getUpdateRateValueForAttribute(ObjectInstanceHandle objectInstanceHandle, AttributeHandle attributeHandle)
    // throw (ObjectInstanceNotKnown,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    const Federate::InstanceAttribute* instanceAttribute = objectInstance->getInstanceAttribute(attributeHandle);
    if (!instanceAttribute)
      throw AttributeNotDefined(attributeHandle.toString());
    return instanceAttribute->getUpdateRate();
  }

  InteractionClassHandle getInteractionClassHandle(const std::string& name)
    // throw (NameNotFound,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    InteractionClassHandle interactionClassHandle;
    interactionClassHandle = _federate->getInteractionClassHandle(name);
    if (!interactionClassHandle.valid())
      throw NameNotFound(name);
    return interactionClassHandle;
  }

  const std::string& getInteractionClassName(InteractionClassHandle interactionClassHandle)
    // throw (InvalidInteractionClassHandle,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InvalidInteractionClassHandle(interactionClassHandle.toString());
    return interactionClass->getName();
  }

  ParameterHandle getParameterHandle(InteractionClassHandle interactionClassHandle, const std::string& name)
    // throw (InvalidInteractionClassHandle,
    //        NameNotFound,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InvalidInteractionClassHandle(interactionClassHandle.toString());
    ParameterHandle parameterHandle;
    parameterHandle = interactionClass->getParameterHandle(name);
    if (!parameterHandle.valid())
      throw NameNotFound(name);
    return parameterHandle;
  }

  const std::string& getParameterName(InteractionClassHandle interactionClassHandle, ParameterHandle parameterHandle)
    // throw (InvalidInteractionClassHandle,
    //        InvalidParameterHandle,
    //        InteractionParameterNotDefined,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InvalidInteractionClassHandle(interactionClassHandle.toString());
    const Federate::Parameter* parameter = interactionClass->getParameter(parameterHandle);
    if (!parameter)
      throw InvalidParameterHandle(parameterHandle.toString());
    return parameter->getName();
  }

  std::string getParameterDataType(InteractionClassHandle interactionClassHandle, ParameterHandle parameterHandle)
    // throw (InvalidInteractionClassHandle,
    //        InvalidParameterHandle,
    //        InteractionParameterNotDefined,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InvalidInteractionClassHandle(interactionClassHandle.toString());
    const Federate::Parameter* parameter = interactionClass->getParameter(parameterHandle);
    if (!parameter)
      throw InvalidParameterHandle(parameterHandle.toString());
    return parameter->getDataType();
  }

  ObjectInstanceHandle getObjectInstanceHandle(const std::string& name)
    // throw (ObjectInstanceNotKnown,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    ObjectInstanceHandle objectInstanceHandle = _federate->getObjectInstanceHandle(name);
    if (!objectInstanceHandle.valid())
      throw ObjectInstanceNotKnown(name);
    return objectInstanceHandle;
  }

  const std::string& getObjectInstanceName(ObjectInstanceHandle objectInstanceHandle)
    // throw (ObjectInstanceNotKnown,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    return objectInstance->getName();
  }

  DimensionHandle getDimensionHandle(const std::string& name)
    // throw (NameNotFound,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    DimensionHandle dimensionHandle = _federate->getDimensionHandle(name);
    if (!dimensionHandle.valid())
      throw NameNotFound(name);
    return dimensionHandle;
  }

  const std::string& getDimensionName(DimensionHandle dimensionHandle)
    // throw (InvalidDimensionHandle,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::Dimension* dimension = _federate->getDimension(dimensionHandle);
    if (!dimension)
      throw InvalidDimensionHandle(dimensionHandle.toString());
    return dimension->getName();
  }

  unsigned long getDimensionUpperBound(DimensionHandle dimensionHandle)
    // throw (InvalidDimensionHandle,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::Dimension* dimension = _federate->getDimension(dimensionHandle);
    if (!dimension)
      throw InvalidDimensionHandle(dimensionHandle.toString());
    return dimension->getUpperBound();
  }

  DimensionHandleSet getAvailableDimensionsForClassAttribute(ObjectClassHandle objectClassHandle,
                                                             AttributeHandle attributeHandle)
    // throw (InvalidObjectClassHandle,
    //        InvalidAttributeHandle,
    //        AttributeNotDefined,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw InvalidObjectClassHandle(objectClassHandle.toString());
    const Federate::Attribute* attribute = objectClass->getAttribute(attributeHandle);
    if (!attribute)
      throw InvalidAttributeHandle(attributeHandle.toString());
    return attribute->getDimensionHandleSet();
  }

  ObjectClassHandle getKnownObjectClassHandle(ObjectInstanceHandle objectInstanceHandle)
    // throw (ObjectInstanceNotKnown,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      throw ObjectInstanceNotKnown(objectInstanceHandle.toString());
    return objectInstance->getObjectClassHandle();
  }

  DimensionHandleSet getAvailableDimensionsForInteractionClass(InteractionClassHandle interactionClassHandle)
    // throw (InvalidInteractionClassHandle,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InvalidInteractionClassHandle(interactionClassHandle.toString());
    return interactionClass->getDimensionHandleSet();
  }

  TransportationType getTransportationType(const std::string& name)
    // throw (InvalidTransportationName,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const TransportationType* transportationType = _federate->getTransportationType(name);
    if (!transportationType)
      throw InvalidTransportationName(name);
    return *transportationType;
  }

  const std::string& getTransportationName(TransportationType transportationType)
    // throw (InvalidTransportationType,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const std::string* name = _federate->getTransportationName(transportationType);
    if (!name)
      throw InvalidTransportationType();
    return *name;
  }

  OrderType getOrderType(const std::string& name)
    // throw (InvalidOrderName,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const OrderType* orderType = _federate->getOrderType(name);
    if (!orderType)
      throw InvalidOrderName(name);
    return *orderType;
  }

  std::string getOrderName(OrderType orderType)
    // throw (InvalidOrderType,
    //        FederateNotExecutionMember,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const std::string* name = _federate->getOrderName(orderType);
    if (!name)
      throw InvalidOrderType();
    return *name;
  }

  void enableObjectClassRelevanceAdvisorySwitch()
    // throw (ObjectClassRelevanceAdvisorySwitchIsOn,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (_federate->getObjectClassRelevanceAdvisorySwitchEnabled())
      throw ObjectClassRelevanceAdvisorySwitchIsOn();
    _federate->setObjectClassRelevanceAdvisorySwitchEnabled(true);

    // This is a very simple and actually wrong implementation as it only
    // directly responds to the ambassadors own publication state. But
    // implementing this without real subscription tracking helps a lot of simple
    // examples to run correctly.
    for (uint32_t i = 0; i < _federate->getNumObjectClasses(); ++i) {
      const Federate::ObjectClass* objectClass = _federate->getObjectClass(ObjectClassHandle(i));
      if (!objectClass)
        continue;
      if (!objectClass->isPublished())
        continue;
      SharedPtr<RegistrationForObjectClassMessage> message = MakeShared<RegistrationForObjectClassMessage>();
      message->setObjectClassHandle(ObjectClassHandle(i));
      message->setStart(true);
      queueCallback(message);
    }
  }

  void disableObjectClassRelevanceAdvisorySwitch()
    // throw (ObjectClassRelevanceAdvisorySwitchIsOff,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (!_federate->getObjectClassRelevanceAdvisorySwitchEnabled())
      throw ObjectClassRelevanceAdvisorySwitchIsOff();
    _federate->setObjectClassRelevanceAdvisorySwitchEnabled(false);
    // I think we do not want the stop registration for object class messages for each object class ...
  }

  void enableAttributeRelevanceAdvisorySwitch()
    // throw (AttributeRelevanceAdvisorySwitchIsOn,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (_federate->getAttributeRelevanceAdvisorySwitchEnabled())
      throw AttributeRelevanceAdvisorySwitchIsOn();
    _federate->setAttributeRelevanceAdvisorySwitchEnabled(true);

    for (Federate::ObjectInstanceHandleMap::const_iterator i = _federate->getObjectInstanceHandleMap().begin();
         i != _federate->getObjectInstanceHandleMap().end(); ++i) {
      const Federate::ObjectInstance* objectInstance = i->second.get();
      if (!objectInstance)
        continue;
      const Federate::ObjectClass* objectClass = _federate->getObjectClass(objectInstance->getObjectClassHandle());
      if (!objectClass)
        continue;
      uint32_t numAttributes = objectClass->getNumAttributes();
      AttributeHandleVector attributeHandleVector;
      for (uint32_t j = 0; j < numAttributes; ++j) {
        const Federate::InstanceAttribute* instanceAttribute = objectInstance->getInstanceAttribute(AttributeHandle(j));
        if (!instanceAttribute->getIsOwnedByFederate())
          continue;
        attributeHandleVector.reserve(numAttributes);
        attributeHandleVector.push_back(AttributeHandle(j));
      }
      if (attributeHandleVector.empty())
        continue;
      SharedPtr<TurnUpdatesOnForInstanceMessage> message = MakeShared<TurnUpdatesOnForInstanceMessage>();
      message->setObjectInstanceHandle(i->first);
      message->getAttributeHandles().swap(attributeHandleVector);
      message->setOn(true);
      // message->setUpdateRate(/*FIXME*/);
      queueCallback(message);
    }
  }

  void disableAttributeRelevanceAdvisorySwitch()
    // throw (AttributeRelevanceAdvisorySwitchIsOff,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (!_federate->getAttributeRelevanceAdvisorySwitchEnabled())
      throw AttributeRelevanceAdvisorySwitchIsOff();
    _federate->setAttributeRelevanceAdvisorySwitchEnabled(false);
    // I think we do not want the attribute update turn off messages for each attribute ...
  }

  void enableAttributeScopeAdvisorySwitch()
    // throw (AttributeScopeAdvisorySwitchIsOn,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (_federate->getAttributeScopeAdvisorySwitchEnabled())
      throw AttributeScopeAdvisorySwitchIsOn();
    _federate->setAttributeScopeAdvisorySwitchEnabled(true);

    for (Federate::ObjectInstanceHandleMap::const_iterator i = _federate->getObjectInstanceHandleMap().begin();
         i != _federate->getObjectInstanceHandleMap().end(); ++i) {
      const Federate::ObjectInstance* objectInstance = i->second.get();
      if (!objectInstance)
        continue;
      const Federate::ObjectClass* objectClass = _federate->getObjectClass(objectInstance->getObjectClassHandle());
      if (!objectClass)
        continue;
      uint32_t numAttributes = objectClass->getNumAttributes();
      AttributeHandleVector attributeHandleVector;
      for (uint32_t j = 0; j < numAttributes; ++j) {
        const Federate::InstanceAttribute* instanceAttribute = objectInstance->getInstanceAttribute(AttributeHandle(j));
        if (!instanceAttribute->getIsOwnedByFederate())
          continue;
        attributeHandleVector.reserve(numAttributes);
        attributeHandleVector.push_back(AttributeHandle(j));
      }
      if (attributeHandleVector.empty())
        continue;
      SharedPtr<AttributesInScopeMessage> message = MakeShared<AttributesInScopeMessage>();
      message->setObjectInstanceHandle(i->first);
      message->getAttributeHandles().swap(attributeHandleVector);
      message->setInScope(true);
      queueCallback(message);
    }
  }

  void disableAttributeScopeAdvisorySwitch()
    // throw (AttributeScopeAdvisorySwitchIsOff,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (!_federate->getAttributeScopeAdvisorySwitchEnabled())
      throw AttributeScopeAdvisorySwitchIsOff();
    _federate->setAttributeScopeAdvisorySwitchEnabled(false);
    // I think we do not want the attribute update turn off messages for each attribute ...
  }

  void enableInteractionRelevanceAdvisorySwitch()
    // throw (InteractionRelevanceAdvisorySwitchIsOn,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (_federate->getInteractionRelevanceAdvisorySwitchEnabled())
      throw InteractionRelevanceAdvisorySwitchIsOn();
    _federate->setInteractionRelevanceAdvisorySwitchEnabled(true);

    // This is a very simple and actually wrong implementation as it only
    // directly responds to the ambassadors own publication state. But
    // implementing this without real subscription tracking helps a lot of simple
    // examples to run correctly.
    for (uint32_t i = 0; i < _federate->getNumInteractionClasses(); ++i) {
      const Federate::InteractionClass* interactionClass = _federate->getInteractionClass(InteractionClassHandle(i));
      if (!interactionClass)
        continue;
      if (!interactionClass->isPublished())
        continue;
      SharedPtr<TurnInteractionsOnMessage> message = MakeShared<TurnInteractionsOnMessage>();
      message->setInteractionClassHandle(InteractionClassHandle(i));
      message->setOn(true);
      queueCallback(message);
    }
  }

  void disableInteractionRelevanceAdvisorySwitch()
    // throw (InteractionRelevanceAdvisorySwitchIsOff,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    if (!_federate->getInteractionRelevanceAdvisorySwitchEnabled())
      throw InteractionRelevanceAdvisorySwitchIsOff();
    _federate->setInteractionRelevanceAdvisorySwitchEnabled(false);
    // I think we do not want the turn off messages for each published class ...
  }

  DimensionHandleSet getDimensionHandleSet(RegionHandle regionHandle)
    // throw (InvalidRegion,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::RegionData* region = _federate->getRegion(regionHandle);
    if (!region)
      throw InvalidRegion(regionHandle.toString());
    return region->getDimensionHandleSet();
  }

  RangeBounds getRangeBounds(RegionHandle regionHandle, DimensionHandle dimensionHandle)
    // throw (InvalidRegion,
    //        RegionDoesNotContainSpecifiedDimension,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    const Federate::RegionData* region = _federate->getRegion(regionHandle);
    if (!region)
      throw InvalidRegion(regionHandle.toString());
    if (!region->containsDimensionHandle(dimensionHandle))
      throw RegionDoesNotContainSpecifiedDimension();
    return region->getRegion().getRangeBounds(dimensionHandle);
  }

  void setRangeBounds(RegionHandle regionHandle, DimensionHandle dimensionHandle, const RangeBounds& rangeBounds)
    // throw (InvalidRegion,
    //        RegionNotCreatedByThisFederate,
    //        RegionDoesNotContainSpecifiedDimension,
    //        InvalidRangeBound,
    //        FederateNotExecutionMember,
    //        SaveInProgress,
    //        RestoreInProgress,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    Federate::RegionData* region = _federate->getRegion(regionHandle);
    if (!region)
      throw InvalidRegion(regionHandle.toString());
    if (regionHandle.getFederateHandle() != getFederateHandle())
      throw RegionNotCreatedByThisFederate(regionHandle.toString());
    if (!region->containsDimensionHandle(dimensionHandle))
      throw RegionDoesNotContainSpecifiedDimension();
    const Federate::Dimension* dimension = _federate->getDimension(dimensionHandle);
    OpenRTIAssert(dimension);
    if (dimension->getUpperBound() < rangeBounds.getUpperBound())
      throw InvalidRangeBound();
    region->getRegion().setRangeBounds(dimensionHandle, rangeBounds);
  }

  unsigned long normalizeFederateHandle(FederateHandle federateHandle)
    // throw (FederateNotExecutionMember,
    //        InvalidFederateHandle,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    return federateHandle.getHandle();
  }

  unsigned long normalizeServiceGroup(ServiceGroupIndicator serviceGroup)
    // throw (FederateNotExecutionMember,
    //        InvalidServiceGroup,
    //        NotConnected,
    //        RTIinternalError)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();
    return serviceGroup;
  }

  bool evokeCallback(double approximateMinimumTimeInSeconds)
    // throw (RTIinternalError)
  {
    Clock clock = Clock::now() + Clock::fromSeconds(approximateMinimumTimeInSeconds);
    AbsTimeout timeout(clock);
    return dispatchCallback(timeout);
  }

  bool evokeMultipleCallbacks(double approximateMinimumTimeInSeconds, double approximateMaximumTimeInSeconds)
    // throw (RTIinternalError)
  {
    Clock reference = Clock::now();
    // 10.42 [...] The service shall continue to process available callbacks
    // until the minimum time specified wall clock time. At that
    // wall clock time if there are no additional callbacks to be
    // delivered to the federate, the service shall complete.
    Clock clock = reference + Clock::fromSeconds(approximateMinimumTimeInSeconds);
    AbsTimeout timeout(clock);
    do {
      if (!dispatchCallback(timeout))
        return false;
    } while (!timeout.isExpired());

    clock = reference + Clock::fromSeconds(approximateMaximumTimeInSeconds);
    AbsTimeout zeroTimeout(Clock::zero());
    do {
      if (!dispatchCallback(zeroTimeout))
        return false;
    } while (!timeout.isExpired());

    return true;
  }

  // Vector Extension
  void setInteractionClassDeliverToSelf(InteractionClassHandle interactionClassHandle, bool enable)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();

    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      throw InteractionClassNotDefined(interactionClassHandle.toString());
    interactionClass->setDeliverToSelf(enable);
  }

  // Vector Extension
  void setObjectClassDeliverToSelf(ObjectClassHandle objectClassHandle, bool enable)
  {
    if (!isConnected())
      throw NotConnected();
    if (!_federate.valid())
      throw FederateNotExecutionMember();

    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      throw ObjectClassNotDefined(objectClassHandle.toString());
    objectClass->setDeliverToSelf(enable);
  }


  void _requestObjectInstanceHandles(unsigned count)
  {
    SharedPtr<ObjectInstanceHandlesRequestMessage> request;
    request = MakeShared<ObjectInstanceHandlesRequestMessage>();
    request->setFederationHandle(getFederationHandle());
    request->setFederateHandle(getFederateHandle());
    request->setCount(count);
    send(request);
  }

  ObjectInstanceHandleNamePair
    _getFreeObjectInstanceHandleNamePair()
  {
    // Usually we already have some handles locally available, but for each new one,
    // start requesting the next from the server, this way we should stay asyncronous for ever.
    // May be the initial amount of object handles should be configuration option ...
    _requestObjectInstanceHandles(1);

    if (!_federate->haveFreeObjectInstanceHandleNamePair()) {
      Clock abstime = (_operationWaitTimeout == kInfinite) ? Clock::max() : Clock::now() + Clock::fromMilliSeconds(_operationWaitTimeout);
      AbsTimeout timeout(abstime);
      while (!_federate->haveFreeObjectInstanceHandleNamePair()) {
        if (receiveAndDispatchInternalMessage(timeout))
          continue;
        if (timeout.isExpired())
          throw RTIinternalError("Timeout while waiting for free object handles.");
      }
    }

    return _federate->takeFreeObjectInstanceHandleNamePair();
  }

  void _releaseObjectInstance(const ObjectInstanceHandle& objectInstanceHandle)
  {
    // Remove the instance from the object model
    _federate->eraseObjectInstance(objectInstanceHandle);

    // Unreference the object instance handle resource
    SharedPtr<ReleaseMultipleObjectInstanceNameHandlePairsMessage> message;
    message = MakeShared<ReleaseMultipleObjectInstanceNameHandlePairsMessage>();
    message->setFederationHandle(getFederationHandle());
    message->getObjectInstanceHandleVector().push_back(objectInstanceHandle);
    send(message);

    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    if (_timeManagement.valid())
      _timeManagement->eraseMessagesForObjectInstance(*this, objectInstanceHandle);
  }

  // Get a next message retraction handle
  MessageRetractionHandle getNextMessageRetractionHandle() {
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    return MessageRetractionHandle(getFederateHandle(), getTimeManagement()->getNextMessageRetractionSerial());
  }

  class OPENRTI_LOCAL _CallbackDispatchFunctor {
  public:
    _CallbackDispatchFunctor(Ambassador& ambassador) :
      _ambassador(ambassador)
    { }
    template<typename M>
    void operator()(const M& message) const
    {
      _ambassador.acceptCallbackMessage(message);
    }
  private:
    Ambassador & _ambassador;
  };

  // Returns true if a callback has been dispatched
  bool _dispatchCallbackMessage()
  {
    _CallbackDispatchFunctor callbackDispatchfunctor(*this);
    FunctorMessageDispatcher<_CallbackDispatchFunctor> dispatcher(callbackDispatchfunctor);
    return InternalAmbassador::_dispatchCallbackMessage(dispatcher);
  }

  // Here we just should see messages which do callbacks in the ambassador
  bool dispatchCallback(const AbsTimeout& timeout)
  {
    flushAndDispatchInternalMessage();
    if (!_callbacksEnabled)
    {
      return false;
    }
    while (!_dispatchCallbackMessage()) {
      if (!receiveAndDispatchInternalMessage(timeout))
      {
        return false;
      }
    }
    bool avail = _callbackMessageAvailable();
    return avail;
  }
  // same, w.o. timeout
  bool dispatchCallback() override
  {
    flushAndDispatchInternalMessage();
    if (!_callbacksEnabled)
    {
      return false;
    }
    while (!_dispatchCallbackMessage()) {
      // NOTE: receive w.o. timeout will return nullptr without waiting, if no message is readily available
      if (!receiveAndDispatchInternalMessage())
      {
        return false;
      }
    }
    bool avail = _callbackMessageAvailable();
    return avail;
  }

  /// Default internal message processing method
  void acceptCallbackMessage(const AbstractMessage& message)
  { throw RTIinternalError("Unexpected message in callback message processing!"); }
  void acceptCallbackMessage(const ChangeObjectInstanceSubscriptionMessage& message)
  { 
    throw RTIinternalError("Unexpected message in callback message processing!");
  }

  void acceptCallbackMessage(const ConnectionLostMessage& message)
  {
    connectionLost(message.getFaultDescription());
  }
  void acceptCallbackMessage(const EnumerateFederationExecutionsResponseMessage& message)
  { reportFederationExecutions(message.getFederationExecutionInformationVector()); }

  void acceptCallbackMessage(const RegisterFederationSynchronizationPointResponseMessage& message)
  { synchronizationPointRegistrationResponse(message.getLabel(), message.getRegisterFederationSynchronizationPointResponseType()); }
  void acceptCallbackMessage(const AnnounceSynchronizationPointMessage& message)
  {
    if (!_federate.valid())
      return;
    _federate->insertAnnouncedFederationSynchonizationLabel(message.getLabel());
    announceSynchronizationPoint(message.getLabel(), message.getTag());
  }
  void acceptCallbackMessage(const FederationSynchronizedMessage& message)
  {
    if (!_federate.valid())
      return;
    federationSynchronized(message.getLabel(), message.getFederateHandleBoolPairVector());
    _federate->eraseAnnouncedFederationSynchonizationLabel(message.getLabel());
  }
  void acceptCallbackMessage(const RegistrationForObjectClassMessage& message)
  { registrationForObjectClass(message.getObjectClassHandle(), message.getStart()); }
  void acceptCallbackMessage(const AttributesInScopeMessage& message)
  {
    if (message.getInScope())
      attributesInScope(message.getObjectInstanceHandle(), message.getAttributeHandles());
    else
      attributesOutOfScope(message.getObjectInstanceHandle(), message.getAttributeHandles());
  }
  void acceptCallbackMessage(const TurnUpdatesOnForInstanceMessage& message)
  {
    if (message.getOn())
      turnUpdatesOnForObjectInstance(message.getObjectInstanceHandle(), message.getAttributeHandles(), message.getUpdateRate());
    else
      turnUpdatesOffForObjectInstance(message.getObjectInstanceHandle(), message.getAttributeHandles());
  }
  void acceptCallbackMessage(const TurnInteractionsOnMessage& message)
  { turnInteractionsOn(message.getInteractionClassHandle(), message.getOn()); }

  void acceptCallbackMessage(const ReserveObjectInstanceNameResponseMessage& message)
  {
    if (!_federate.valid())
      return;
    if (message.getSuccess()) {
      _federate->insertReservedObjectInstanceHandleNamePair(message.getObjectInstanceHandleNamePair());
      objectInstanceNameReservationSucceeded(message.getObjectInstanceHandleNamePair().second);
    } else {
      objectInstanceNameReservationFailed(message.getObjectInstanceHandleNamePair().second);
    }
  }
  void acceptCallbackMessage(const ReserveMultipleObjectInstanceNameResponseMessage& message)
  {
    if (!_federate.valid())
      return;
    StringVector stringVector;
    stringVector.reserve(message.getObjectInstanceHandleNamePairVector().size());
    for (ObjectInstanceHandleNamePairVector::const_iterator i = message.getObjectInstanceHandleNamePairVector().begin();
         i != message.getObjectInstanceHandleNamePairVector().end(); ++i) {
      stringVector.push_back(i->second);
      if (message.getSuccess())
        _federate->insertReservedObjectInstanceHandleNamePair(*i);
    }
    if (message.getSuccess()) {
      multipleObjectInstanceNameReservationSucceeded(stringVector);
    } else {
      multipleObjectInstanceNameReservationFailed(stringVector);
    }
  }

  void acceptCallbackMessage(const InsertObjectInstanceMessage& message)
  {
    if (!_federate.valid())
      return;
    ObjectClassHandle objectClassHandle = message.getObjectClassHandle();
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectClassHandle);
    if (!objectClass)
      return;
    while (Unsubscribed == objectClass->getEffectiveSubscriptionType()) {
      objectClassHandle = objectClass->getParentObjectClassHandle();
      objectClass = _federate->getObjectClass(objectClassHandle);
      if (!objectClass)
        return;
    }
    // Ok we get duplicate inserts. FIXME investigate this
    if (_federate->getObjectInstance(message.getObjectInstanceHandle()))
      return;
    _federate->insertObjectInstance(message.getObjectInstanceHandle(), message.getName(), objectClassHandle, false);
    discoverObjectInstance(message.getObjectInstanceHandle(), objectClassHandle, message.getName());
  }
  
  void acceptCallbackMessage(const DeleteObjectInstanceMessage& message)
  {
    if (!_federate.valid())
      return;
    ObjectInstanceHandle objectInstanceHandle = message.getObjectInstanceHandle();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      return;
    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectInstance->getObjectClassHandle());
    if (objectClass) {
      if (Unsubscribed != objectClass->getEffectiveSubscriptionType()) {
        removeObjectInstance(message.getObjectInstanceHandle(), message.getTag(), OpenRTI::RECEIVE, message.getFederateHandle());
      }
    }
    _releaseObjectInstance(message.getObjectInstanceHandle());
  }
  
  void acceptCallbackMessage(const TimeStampedDeleteObjectInstanceMessage& message)
  {
    if (!_federate.valid())
      return;
    if (!_timeManagement.valid())
      return;
    ObjectInstanceHandle objectInstanceHandle = message.getObjectInstanceHandle();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      return;
    if (Federate::ObjectClass* objectClass = _federate->getObjectClass(objectInstance->getObjectClassHandle())) {
      if (Unsubscribed != objectClass->getEffectiveSubscriptionType()) {
        OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
        _timeManagement->removeObjectInstance(*this, message);
      }
    }
    _releaseObjectInstance(message.getObjectInstanceHandle());
  }
  
  void acceptCallbackMessage(const AttributeUpdateMessage& message)
  {
    if (!_federate.valid())
      return;
    // Look for the known object class of this object instance.
    // Is required for the right subset of attributes that are reflected
    ObjectInstanceHandle objectInstanceHandle = message.getObjectInstanceHandle();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      return;
    // objectInstance update arrived but was just unsubscribed
    if (objectInstance->getSubscriptionType() == Unsubscribed)
      return;

    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectInstance->getObjectClassHandle());
    if (!objectClass)
      return;
    reflectAttributeValues(*objectClass, message.getObjectInstanceHandle(), message.getAttributeValues(), message.getTag(),
                           OpenRTI::RECEIVE, message.getTransportationType(), message.getFederateHandle());
  }
  
  void acceptCallbackMessage(const TimeStampedAttributeUpdateMessage& message)
  {
    if (!_federate.valid())
      return;
    if (!_timeManagement.valid())
      return;
    // Look for the known object class of this object instance.
    // Is required for the right subset of attributes that are reflected
    ObjectInstanceHandle objectInstanceHandle = message.getObjectInstanceHandle();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      return;

    // objectInstance update arrived but was just unsubscribed
    if (objectInstance->getSubscriptionType() == Unsubscribed)
      return;

    Federate::ObjectClass* objectClass = _federate->getObjectClass(objectInstance->getObjectClassHandle());
    if (!objectClass)
      return;
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    _timeManagement->reflectAttributeValues(*this, *objectClass, message);
  }
  void acceptCallbackMessage(const InteractionMessage& message)
  {
    CondDebugPrintf("%s: message=%s\n", __FUNCTION__, message.toString().c_str());
    if (!_federate.valid())
      return;
    InteractionClassHandle interactionClassHandle = message.getInteractionClassHandle();
    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      return;
    // Subscriptions can race against sending them to this federate
    // FIXME: store the effective next interaction class that is subscribed for all interaction classes.
    // This would avoid this loop
    while (Unsubscribed == interactionClass->getSubscriptionType()) {
      interactionClassHandle = interactionClass->getParentInteractionClassHandle();
      interactionClass = _federate->getInteractionClass(interactionClassHandle);
      if (!interactionClass)
        return;
    }
    receiveInteraction(*interactionClass, interactionClassHandle, message.getParameterValues(), message.getTag(),
                       OpenRTI::RECEIVE, message.getTransportationType(), message.getFederateHandle());
  }
  void acceptCallbackMessage(const TimeStampedInteractionMessage& message)
  {
    if (!_federate.valid())
      return;
    if (!_timeManagement.valid())
      return;
    InteractionClassHandle interactionClassHandle = message.getInteractionClassHandle();
    Federate::InteractionClass* interactionClass = _federate->getInteractionClass(interactionClassHandle);
    if (!interactionClass)
      return;
    // Subscriptions can race against sending them to this federate
    // FIXME: store the effective next interaction class that is subscribed for all interaction classes.
    // This would avoid this loop
    while (Unsubscribed == interactionClass->getSubscriptionType()) {
      interactionClassHandle = interactionClass->getParentInteractionClassHandle();
      interactionClass = _federate->getInteractionClass(interactionClassHandle);
      if (!interactionClass)
      {
        return;
      }
    }
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    _timeManagement->receiveInteraction(*this, *interactionClass, interactionClassHandle, message);
  }
  void acceptCallbackMessage(const TimeConstrainedEnabledMessage& message)
  {
    if (!_timeManagement.valid())
      return;
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    _timeManagement->acceptCallbackMessage(*this, message);
  }
  void acceptCallbackMessage(const TimeRegulationEnabledMessage& message)
  {
    if (!_timeManagement.valid())
      return;
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    _timeManagement->acceptCallbackMessage(*this, message);
  }
  void acceptCallbackMessage(const TimeAdvanceGrantedMessage& message)
  {
    if (!_timeManagement.valid())
      return;
    OpenRTI::RecursiveScopeLock timeManagementLock(_timeManagementMutex);
    _timeManagement->acceptCallbackMessage(*this, message);
  }
  void acceptCallbackMessage(const RequestAttributeUpdateMessage& message)
  {
    if (!_federate.valid())
      return;
    // FIXME: can fail. Check that already in the internal processing
    ObjectInstanceHandle objectInstanceHandle = message.getObjectInstanceHandle();
    Federate::ObjectInstance* objectInstance = _federate->getObjectInstance(objectInstanceHandle);
    if (!objectInstance)
      return;
    AttributeHandleVector attributeHandleVector;
    for (AttributeHandleVector::const_iterator j = message.getAttributeHandles().begin(); j != message.getAttributeHandles().end(); ++j) {
      Federate::InstanceAttribute* attribute = objectInstance->getInstanceAttribute(j->getHandle());
      if (!attribute)
        continue;
      if (!attribute->getIsOwnedByFederate())
        continue;
      attributeHandleVector.reserve(message.getAttributeHandles().size());
      attributeHandleVector.push_back(*j);
    }
    if (attributeHandleVector.empty())
      return;
    provideAttributeValueUpdate(objectInstanceHandle, attributeHandleVector, message.getTag());
  }



  // The callback into the binding concrete implementation.
  virtual void connectionLost(const std::string& faultDescription) = 0;

  virtual void reportFederationExecutions(const FederationExecutionInformationVector& federationExecutionInformationVector) = 0;

  virtual void synchronizationPointRegistrationResponse(const std::string& label, RegisterFederationSynchronizationPointResponseType reason) = 0;
  virtual void announceSynchronizationPoint(const std::string& label, const VariableLengthData& tag) = 0;
  virtual void federationSynchronized(const std::string& label, const FederateHandleBoolPairVector& federateHandleBoolPairVector) = 0;

  virtual void registrationForObjectClass(ObjectClassHandle objectClassHandle, bool start) = 0;

  virtual void turnInteractionsOn(InteractionClassHandle interactionClassHandle, bool on) = 0;

  virtual void objectInstanceNameReservationSucceeded(const std::string& objectInstanceName) = 0;
  virtual void objectInstanceNameReservationFailed(const std::string& objectInstanceName) = 0;

  virtual void multipleObjectInstanceNameReservationSucceeded(const std::vector<std::string>& objectInstanceNames) = 0;
  virtual void multipleObjectInstanceNameReservationFailed(const std::vector<std::string>& objectInstanceNames) = 0;

  virtual void discoverObjectInstance(ObjectInstanceHandle objectInstanceHandle, ObjectClassHandle objectClassHandle,
                                      const std::string& objectInstanceName) = 0;

  virtual void reflectAttributeValues(const Federate::ObjectClass& objectClass, ObjectInstanceHandle objectInstanceHandle,
                                      const AttributeValueVector& attributeValueVector, const VariableLengthData& tag,
                                      OrderType sentOrder, TransportationType transportationType, FederateHandle federateHandle) = 0;
  virtual void reflectAttributeValues(const Federate::ObjectClass& objectClass, ObjectInstanceHandle objectInstanceHandle,
                                      const AttributeValueVector& attributeValueVector, const VariableLengthData& tag,
                                      OrderType sentOrder, TransportationType transportationType, const NativeLogicalTime& logicalTime,
                                      OrderType receivedOrder, FederateHandle federateHandle) = 0;
  virtual void reflectAttributeValues(const Federate::ObjectClass& objectClass, ObjectInstanceHandle objectInstanceHandle,
                                      const AttributeValueVector& attributeValueVector, const VariableLengthData& tag,
                                      OrderType sentOrder, TransportationType transportationType, const NativeLogicalTime& logicalTime,
                                      OrderType receivedOrder, FederateHandle federateHandle, MessageRetractionHandle messageRetractionHandle) = 0;

  virtual void removeObjectInstance(ObjectInstanceHandle objectInstanceHandle, const VariableLengthData& tag, OrderType sentOrder,
                                    FederateHandle federateHandle) = 0;
  virtual void removeObjectInstance(ObjectInstanceHandle objectInstanceHandle, const VariableLengthData& tag, OrderType sentOrder,
                                    const NativeLogicalTime& logicalTime, OrderType receivedOrder, FederateHandle federateHandle) = 0;
  virtual void removeObjectInstance(ObjectInstanceHandle objectInstanceHandle, const VariableLengthData& tag, OrderType sentOrder,
                                    const NativeLogicalTime& logicalTime, OrderType receivedOrder, FederateHandle federateHandle,
                                    MessageRetractionHandle messageRetractionHandle) = 0;
  virtual void receiveInteraction(const Federate::InteractionClass& interactionClass, InteractionClassHandle interactionClassHandle,
                                  const ParameterValueVector& parameterValueVector, const VariableLengthData& tag,
                                  OrderType sentOrder, TransportationType transportationType, FederateHandle federateHandle) = 0;
  virtual void receiveInteraction(const Federate::InteractionClass& interactionClass, InteractionClassHandle interactionClassHandle,
                                  const ParameterValueVector& parameterValueVector, const VariableLengthData& tag,
                                  OrderType sentOrder, TransportationType transportationType, const NativeLogicalTime& logicalTime,
                                  OrderType receivedOrder, FederateHandle federateHandle) = 0;
  virtual void receiveInteraction(const Federate::InteractionClass& interactionClass, InteractionClassHandle interactionClassHandle,
                                  const ParameterValueVector& parameterValueVector, const VariableLengthData& tag,
                                  OrderType sentOrder, TransportationType transportationType, const NativeLogicalTime& logicalTime,
                                  OrderType receivedOrder, FederateHandle federateHandle, MessageRetractionHandle messageRetractionHandle) = 0;

  virtual void attributesInScope(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector) = 0;
  virtual void attributesOutOfScope(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector) = 0;

  virtual void provideAttributeValueUpdate(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector,
                                           const VariableLengthData& tag) = 0;

  virtual void turnUpdatesOnForObjectInstance(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector, const std::string& updateRate) = 0;
  virtual void turnUpdatesOffForObjectInstance(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector) = 0;

  virtual void requestAttributeOwnershipAssumption(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector,
                                                   const VariableLengthData& tag) = 0;
  virtual void requestDivestitureConfirmation(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector) = 0;

  virtual void attributeOwnershipAcquisitionNotification(ObjectInstanceHandle objectInstanceHandle,
                                                         const AttributeHandleVector& attributeHandleVector,
                                                         const VariableLengthData& tag) = 0;
  virtual void attributeOwnershipUnavailable(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector) = 0;
  virtual void requestAttributeOwnershipRelease(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector,
                                                const VariableLengthData& tag) = 0;
  virtual void confirmAttributeOwnershipAcquisitionCancellation(ObjectInstanceHandle objectInstanceHandle, const AttributeHandleVector& attributeHandleVector) = 0;
  virtual void informAttributeOwnership(ObjectInstanceHandle objectInstanceHandle, AttributeHandle attributeHandle, FederateHandle theOwner) = 0;
  virtual void attributeIsNotOwned(ObjectInstanceHandle objectInstanceHandle, AttributeHandle attributeHandle) = 0;
  virtual void attributeIsOwnedByRTI(ObjectInstanceHandle objectInstanceHandle, AttributeHandle attributeHandle) = 0;

  virtual void timeRegulationEnabled(const NativeLogicalTime& logicalTime) = 0;
  virtual void timeConstrainedEnabled(const NativeLogicalTime& logicalTime) = 0;
  virtual void timeAdvanceGrant(const NativeLogicalTime& logicalTime) = 0;

  virtual void requestRetraction(MessageRetractionHandle messageRetractionHandle) = 0;

  virtual SharedPtr<TimeManagement<Traits>> createTimeManagement(Federate& federate) = 0;

  Federate* getFederate() override
  {
    return _federate.get();
  }
  TimeManagement<Traits>* getTimeManagement() override
  {
    return _timeManagement.get();
  }
  
  void acceptInternalMessage(const InsertFederationExecutionMessage& message) override
  {
    // IMPORTANT NOTE: this object's data will be completed later in 
    // InternalAmbassador::acceptInternalMessage(const JoinFederationExecutionResponseMessage& message)
    _federate = MakeShared<Federate>();
    _federate->setFederationHandle(message.getFederationHandle());
    _federate->setLogicalTimeFactoryName(message.getLogicalTimeFactoryName());

    _timeManagement = createTimeManagement(*_federate);
    _timeManagement->setNotificationHandle(_getNotificationHandle());
  }

  void acceptInternalMessage(const JoinFederationExecutionResponseMessage& message) override
  {
    Federate* federate = getFederate();
    if (!federate)
      return;
    federate->setFederateHandle(message.getFederateHandle());
    federate->setFederateName(message.getFederateName());
    federate->setFederateType(message.getFederateType());
  }

  void setNotificationHandle(std::shared_ptr<AbstractNotificationHandle> h)
  {
    _setNotificationHandle(h);
    if (_timeManagement.get() != nullptr)
    {
      _timeManagement->setNotificationHandle(h);
    }
  };

private:
  // The federate if available
  SharedPtr<Federate> _federate;
  // The timestamped queues
  SharedPtr<TimeManagement<Traits> > _timeManagement;
  uint32_t _connectWaitTimeout;
  uint32_t _operationWaitTimeout;
};

} // namespace OpenRTI

#endif
