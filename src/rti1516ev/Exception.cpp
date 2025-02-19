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

// This time, the first include is above the api include.
// the rti1516ev/Exception header misses that.
#include "DebugNew.h"
#include <iosfwd>

#include <RTI/Exception.h>

#include <sstream>
#include "StringUtils.h"

#pragma warning(disable: 4100)

namespace rti1516ev
{

Exception::Exception()
{
}

Exception::Exception(Exception const & rhs)
{
}

Exception&
Exception::operator=(Exception const & rhs)
{
  return *this;
}

Exception::~Exception() noexcept
{
}

std::wostream&
operator<<(std::wostream& stream, Exception const& e)
{
  return stream << e.what();
}

std::ostream&
operator<<(std::ostream& stream, Exception const& e)
{
  return stream << OpenRTI::ucsToUtf8(e.what());
}

#define IMPLEMENT_RTI_EXCEPTION(A)             \
  A::A(std::wstring const & message) :         \
    _msg(message)                              \
  {                                            \
  }                                            \
  std::wstring                                 \
  A::what() const                              \
  {                                            \
    std::wstringstream stream;                 \
    stream << "rti1516ev::" #A ": " << _msg;    \
    return stream.str();                       \
  }

IMPLEMENT_RTI_EXCEPTION(AlreadyConnected)
IMPLEMENT_RTI_EXCEPTION(AsynchronousDeliveryAlreadyDisabled)
IMPLEMENT_RTI_EXCEPTION(AsynchronousDeliveryAlreadyEnabled)
IMPLEMENT_RTI_EXCEPTION(AttributeAcquisitionWasNotCanceled)
IMPLEMENT_RTI_EXCEPTION(AttributeAcquisitionWasNotRequested)
IMPLEMENT_RTI_EXCEPTION(AttributeAlreadyBeingAcquired)
IMPLEMENT_RTI_EXCEPTION(AttributeAlreadyBeingChanged)
IMPLEMENT_RTI_EXCEPTION(AttributeAlreadyBeingDivested)
IMPLEMENT_RTI_EXCEPTION(AttributeAlreadyOwned)
IMPLEMENT_RTI_EXCEPTION(AttributeDivestitureWasNotRequested)
IMPLEMENT_RTI_EXCEPTION(AttributeNotDefined)
IMPLEMENT_RTI_EXCEPTION(AttributeNotOwned)
IMPLEMENT_RTI_EXCEPTION(AttributeNotPublished)
IMPLEMENT_RTI_EXCEPTION(AttributeNotRecognized)
IMPLEMENT_RTI_EXCEPTION(AttributeNotSubscribed)
IMPLEMENT_RTI_EXCEPTION(AttributeRelevanceAdvisorySwitchIsOff)
IMPLEMENT_RTI_EXCEPTION(AttributeRelevanceAdvisorySwitchIsOn)
IMPLEMENT_RTI_EXCEPTION(AttributeScopeAdvisorySwitchIsOff)
IMPLEMENT_RTI_EXCEPTION(AttributeScopeAdvisorySwitchIsOn)
IMPLEMENT_RTI_EXCEPTION(BadInitializationParameter)
IMPLEMENT_RTI_EXCEPTION(CallNotAllowedFromWithinCallback)
IMPLEMENT_RTI_EXCEPTION(ConnectionFailed)
IMPLEMENT_RTI_EXCEPTION(CouldNotCreateLogicalTimeFactory)
IMPLEMENT_RTI_EXCEPTION(CouldNotDecode)
IMPLEMENT_RTI_EXCEPTION(CouldNotDiscover)
IMPLEMENT_RTI_EXCEPTION(CouldNotEncode)
IMPLEMENT_RTI_EXCEPTION(CouldNotOpenFDD)
IMPLEMENT_RTI_EXCEPTION(CouldNotOpenMIM)
IMPLEMENT_RTI_EXCEPTION(CouldNotInitiateRestore)
IMPLEMENT_RTI_EXCEPTION(DeletePrivilegeNotHeld)
IMPLEMENT_RTI_EXCEPTION(DesignatorIsHLAstandardMIM)
IMPLEMENT_RTI_EXCEPTION(RequestForTimeConstrainedPending)
IMPLEMENT_RTI_EXCEPTION(NoRequestToEnableTimeConstrainedWasPending)
IMPLEMENT_RTI_EXCEPTION(RequestForTimeRegulationPending)
IMPLEMENT_RTI_EXCEPTION(NoRequestToEnableTimeRegulationWasPending)
IMPLEMENT_RTI_EXCEPTION(NoFederateWillingToAcquireAttribute)
IMPLEMENT_RTI_EXCEPTION(ErrorReadingFDD)
IMPLEMENT_RTI_EXCEPTION(ErrorReadingMIM)
IMPLEMENT_RTI_EXCEPTION(FederateAlreadyExecutionMember)
IMPLEMENT_RTI_EXCEPTION(FederateHandleNotKnown)
IMPLEMENT_RTI_EXCEPTION(FederateHasNotBegunSave)
IMPLEMENT_RTI_EXCEPTION(FederateInternalError)
IMPLEMENT_RTI_EXCEPTION(FederateIsExecutionMember)
IMPLEMENT_RTI_EXCEPTION(FederateNameAlreadyInUse)
IMPLEMENT_RTI_EXCEPTION(FederateNotExecutionMember)
IMPLEMENT_RTI_EXCEPTION(FederateOwnsAttributes)
IMPLEMENT_RTI_EXCEPTION(FederateServiceInvocationsAreBeingReportedViaMOM)
IMPLEMENT_RTI_EXCEPTION(FederateUnableToUseTime)
IMPLEMENT_RTI_EXCEPTION(FederatesCurrentlyJoined)
IMPLEMENT_RTI_EXCEPTION(FederationExecutionAlreadyExists)
IMPLEMENT_RTI_EXCEPTION(FederationExecutionDoesNotExist)
IMPLEMENT_RTI_EXCEPTION(IllegalName)
IMPLEMENT_RTI_EXCEPTION(IllegalTimeArithmetic)
IMPLEMENT_RTI_EXCEPTION(InconsistentFDD)
IMPLEMENT_RTI_EXCEPTION(InteractionClassAlreadyBeingChanged)
IMPLEMENT_RTI_EXCEPTION(InteractionClassNotDefined)
IMPLEMENT_RTI_EXCEPTION(InteractionClassNotPublished)
IMPLEMENT_RTI_EXCEPTION(InteractionClassNotRecognized)
IMPLEMENT_RTI_EXCEPTION(InteractionClassNotSubscribed)
IMPLEMENT_RTI_EXCEPTION(InteractionParameterNotDefined)
IMPLEMENT_RTI_EXCEPTION(InteractionParameterNotRecognized)
IMPLEMENT_RTI_EXCEPTION(InteractionRelevanceAdvisorySwitchIsOff)
IMPLEMENT_RTI_EXCEPTION(InteractionRelevanceAdvisorySwitchIsOn)
IMPLEMENT_RTI_EXCEPTION(InTimeAdvancingState)
IMPLEMENT_RTI_EXCEPTION(InvalidAttributeHandle)
IMPLEMENT_RTI_EXCEPTION(InvalidDimensionHandle)
IMPLEMENT_RTI_EXCEPTION(InvalidFederateHandle)
IMPLEMENT_RTI_EXCEPTION(InvalidInteractionClassHandle)
IMPLEMENT_RTI_EXCEPTION(InvalidLocalSettingsDesignator)
IMPLEMENT_RTI_EXCEPTION(InvalidLogicalTime)
IMPLEMENT_RTI_EXCEPTION(InvalidLogicalTimeInterval)
IMPLEMENT_RTI_EXCEPTION(InvalidLookahead)
IMPLEMENT_RTI_EXCEPTION(InvalidObjectClassHandle)
IMPLEMENT_RTI_EXCEPTION(InvalidOrderName)
IMPLEMENT_RTI_EXCEPTION(InvalidOrderType)
IMPLEMENT_RTI_EXCEPTION(InvalidParameterHandle)
IMPLEMENT_RTI_EXCEPTION(InvalidRangeBound)
IMPLEMENT_RTI_EXCEPTION(InvalidRegion)
IMPLEMENT_RTI_EXCEPTION(InvalidResignAction)
IMPLEMENT_RTI_EXCEPTION(InvalidRegionContext)
IMPLEMENT_RTI_EXCEPTION(InvalidMessageRetractionHandle)
IMPLEMENT_RTI_EXCEPTION(InvalidServiceGroup)
IMPLEMENT_RTI_EXCEPTION(InvalidTransportationName)
IMPLEMENT_RTI_EXCEPTION(InvalidTransportationType)
IMPLEMENT_RTI_EXCEPTION(InvalidUpdateRateDesignator)
IMPLEMENT_RTI_EXCEPTION(JoinedFederateIsNotInTimeAdvancingState)
IMPLEMENT_RTI_EXCEPTION(LogicalTimeAlreadyPassed)
IMPLEMENT_RTI_EXCEPTION(MessageCanNoLongerBeRetracted)
IMPLEMENT_RTI_EXCEPTION(NameNotFound)
IMPLEMENT_RTI_EXCEPTION(NameSetWasEmpty)
IMPLEMENT_RTI_EXCEPTION(NoAcquisitionPending)
IMPLEMENT_RTI_EXCEPTION(NotConnected)
IMPLEMENT_RTI_EXCEPTION(ObjectClassNotDefined)
IMPLEMENT_RTI_EXCEPTION(ObjectClassNotKnown)
IMPLEMENT_RTI_EXCEPTION(ObjectClassNotPublished)
IMPLEMENT_RTI_EXCEPTION(ObjectClassRelevanceAdvisorySwitchIsOff)
IMPLEMENT_RTI_EXCEPTION(ObjectClassRelevanceAdvisorySwitchIsOn)
IMPLEMENT_RTI_EXCEPTION(ObjectInstanceNameInUse)
IMPLEMENT_RTI_EXCEPTION(ObjectInstanceNameNotReserved)
IMPLEMENT_RTI_EXCEPTION(ObjectInstanceNotKnown)
IMPLEMENT_RTI_EXCEPTION(OperationTimeout)
IMPLEMENT_RTI_EXCEPTION(OwnershipAcquisitionPending)
IMPLEMENT_RTI_EXCEPTION(RTIinternalError)
IMPLEMENT_RTI_EXCEPTION(RegionDoesNotContainSpecifiedDimension)
IMPLEMENT_RTI_EXCEPTION(RegionInUseForUpdateOrSubscription)
IMPLEMENT_RTI_EXCEPTION(RegionNotCreatedByThisFederate)
IMPLEMENT_RTI_EXCEPTION(RestoreInProgress)
IMPLEMENT_RTI_EXCEPTION(RestoreNotInProgress)
IMPLEMENT_RTI_EXCEPTION(RestoreNotRequested)
IMPLEMENT_RTI_EXCEPTION(SaveInProgress)
IMPLEMENT_RTI_EXCEPTION(SaveNotInProgress)
IMPLEMENT_RTI_EXCEPTION(SaveNotInitiated)
IMPLEMENT_RTI_EXCEPTION(SpecifiedSaveLabelDoesNotExist)
IMPLEMENT_RTI_EXCEPTION(SynchronizationPointLabelNotAnnounced)
IMPLEMENT_RTI_EXCEPTION(TimeConstrainedAlreadyEnabled)
IMPLEMENT_RTI_EXCEPTION(TimeConstrainedIsNotEnabled)
IMPLEMENT_RTI_EXCEPTION(TimeRegulationAlreadyEnabled)
IMPLEMENT_RTI_EXCEPTION(TimeRegulationIsNotEnabled)
IMPLEMENT_RTI_EXCEPTION(UnableToPerformSave)
IMPLEMENT_RTI_EXCEPTION(UnknownName)
IMPLEMENT_RTI_EXCEPTION(UnsupportedCallbackModel)
IMPLEMENT_RTI_EXCEPTION(InternalError)
IMPLEMENT_RTI_EXCEPTION(ResetInProgress)
IMPLEMENT_RTI_EXCEPTION(ResetNotInProgress)
IMPLEMENT_RTI_EXCEPTION(ResetNotBegun)
IMPLEMENT_RTI_EXCEPTION(ResetNotSupportedByRti)
#undef IMPLEMENT_RTI_EXCEPTION

}
