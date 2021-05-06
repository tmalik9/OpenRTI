/* -*-c++-*- OpenRTI - Copyright (C) 2009-2016 Mathias Froehlich
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

#ifndef OpenRTI_RTI1516ETestLib_h
#define OpenRTI_RTI1516ETestLib_h

#include <algorithm>
#include <cstring>
#include <iterator>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <RTI/FederateAmbassador.h>
#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>
#include <RTI/LogicalTime.h>
#include <RTI/LogicalTimeInterval.h>
#include <RTI/LogicalTimeFactory.h>
#include <RTI/RangeBounds.h>

#include <TestLib.h>

namespace OpenRTI {

inline bool
operator==(const rti1516ev::VariableLengthData& lhs, const rti1516ev::VariableLengthData& rhs)
{
  size_t size = lhs.size();
  if (size != rhs.size())
    return false;
  return 0 == memcmp(lhs.data(), rhs.data(), size);
}

inline bool
operator!=(const rti1516ev::VariableLengthData& lhs, const rti1516ev::VariableLengthData& rhs)
{ return !operator==(lhs, rhs); }

inline rti1516ev::VariableLengthData
toVariableLengthData(const char* s)
{
    rti1516ev::VariableLengthData variableLengthData;
    if (s)
        variableLengthData.setData(s, strlen(s));
    return variableLengthData;
}

inline rti1516ev::VariableLengthData
toVariableLengthData(const std::string& s)
{
    rti1516ev::VariableLengthData variableLengthData;
    variableLengthData.setData(s.data(), s.size());
    return variableLengthData;
}

inline std::string
toString(const rti1516ev::VariableLengthData& variableLengthData)
{
    if (!variableLengthData.size())
        return std::string();
    return std::string((const char*)variableLengthData.data(), variableLengthData.size());
}

inline rti1516ev::VariableLengthData
toVariableLengthData(const std::wstring& s)
{
    rti1516ev::VariableLengthData variableLengthData;
    variableLengthData.setData(s.data(), sizeof(std::wstring::value_type)*s.size());
    return variableLengthData;
}

inline std::wstring
toWString(const rti1516ev::VariableLengthData& variableLengthData)
{
    if (!variableLengthData.size())
        return std::wstring();
    return std::wstring((const wchar_t*)variableLengthData.data(), variableLengthData.size()/sizeof(std::wstring::value_type));
}

inline rti1516ev::VariableLengthData
toVariableLengthData(const Clock& c)
{
    // May be at some time make this endian safe
    rti1516ev::VariableLengthData variableLengthData;
    variableLengthData.setData(&c, sizeof(c));
    return variableLengthData;
}

inline Clock
toClock(const rti1516ev::VariableLengthData& variableLengthData)
{
    Clock c;
    // May be at some time make this endian safe
    if (variableLengthData.size() == sizeof(Clock))
        memcpy(&c, variableLengthData.data(), sizeof(Clock));
    return c;
}

inline rti1516ev::VariableLengthData
toVariableLengthData(unsigned u)
{
    // May be at some time make this endian safe
    rti1516ev::VariableLengthData variableLengthData;
    variableLengthData.setData(&u, sizeof(u));
    return variableLengthData;
}

inline unsigned
toUnsigned(const rti1516ev::VariableLengthData& variableLengthData)
{
    unsigned u = (unsigned)-1;
    // May be at some time make this endian safe
    if (variableLengthData.size() == sizeof(unsigned))
        memcpy(&u, variableLengthData.data(), sizeof(unsigned));
    return u;
}

inline std::wstring to_wstring(const std::string& str)
{
  if (str.empty()) return std::wstring();
  const std::ctype<wchar_t>& CType = std::use_facet<std::ctype<wchar_t> >(std::locale());
  std::vector<wchar_t> wideStringBuffer(str.length());
  CType.widen(str.data(), str.data() + str.length(), &wideStringBuffer[0]);
  return std::wstring(&wideStringBuffer[0], wideStringBuffer.size());
}

inline std::string to_string(const std::wstring& str)
{
  if (str.empty()) return std::string();
  const std::ctype<wchar_t>& CType = std::use_facet<std::ctype<wchar_t> >(std::locale());
  std::vector<char> stringBuffer(str.length());
  CType.narrow(str.data(), str.data() + str.length(), '_', &stringBuffer[0]);
  return std::string(&stringBuffer[0], stringBuffer.size());
}

class OPENRTI_LOCAL RTI1516ETestAmbassador : public RTITest::Ambassador, public rti1516ev::FederateAmbassador {
public:
  RTI1516ETestAmbassador(const RTITest::ConstructorArgs& constructorArgs) :
    RTITest::Ambassador(constructorArgs),
    _synchronized(0)
  { }
  virtual ~RTI1516ETestAmbassador()
  { }

  std::wstring getArgument() const
  {
    std::wstring arg;
    std::vector<std::wstring> args = getArgumentList();
    for (std::vector<std::wstring>::const_iterator i = args.begin(); i != args.end(); ++i) {
      if (!arg.empty())
        arg += L";";
      arg += *i;
    }
    return arg;
  }

  virtual bool execJoined(rti1516ev::RTIambassador& ambassador) = 0;

  bool waitForAllFederates(rti1516ev::RTIambassador& ambassador)
  {
    _synchronized = 0;

    // FIXME need a test for concurrent announces
    try {
      ambassador.registerFederationSynchronizationPoint(getFederateType(), rti1516ev::VariableLengthData());
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      Clock timeout = Clock::now() + Clock::fromSeconds(10);
      while (!_federateSet.empty()) {
        if (ambassador.evokeCallback(10.0))
          continue;
        if (timeout < Clock::now()) {
          std::wcout << L"Timeout waiting for other federates to join!" << std::endl;
          return false;
        }
      }
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    // Fill for the next time
    _federateSet.insert(getFederateList().begin(), getFederateList().end());

    try {
      for (std::vector<std::wstring>::const_iterator i = getFederateList().begin(); i != getFederateList().end(); ++i) {
        ambassador.synchronizationPointAchieved(*i);
      }
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      Clock timeout = Clock::now() + Clock::fromSeconds(10);
      while (_synchronized < getFederateList().size()) {
        if (ambassador.evokeCallback(10.0))
          continue;
        if (timeout < Clock::now()) {
          std::wcout << L"Timeout waiting for other federates to synchronize!" << std::endl;
          return false;
        }
      }
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    return true;
  }

  virtual bool execJoinOnce()
  {
    std::unique_ptr<rti1516ev::RTIambassador> ambassador;
    rti1516ev::RTIambassadorFactory factory;
    ambassador = factory.createRTIambassador();
    ambassador->setOperationWaitTimeout(10000);
    _connectionLost = false;
    ambassador->connect(*this, rti1516ev::HLA_EVOKED, getConnectUrl());

    // create, must work once
    try {
      if (getMimFile().empty())
        ambassador->createFederationExecution(getFederationExecution(), getFddFileList(), std::wstring(L"HLAinteger64Time"));
      else
        ambassador->createFederationExecutionWithMIM(getFederationExecution(), getFddFileList(), getMimFile(), std::wstring(L"HLAinteger64Time"));

      if (!getFederationBarrier()->success())
        return false;
    } catch (const rti1516ev::FederationExecutionAlreadyExists&) {
      // Can happen in this test

      if (!getFederationBarrier()->fail())
        return false;
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    // Try that several times. Ensure correct cleanup
    unsigned n = 99;
    for (unsigned i = 0; i < n; ++i) {

      // join must work
      try {
        ambassador->joinFederationExecution(getFederateType(), getFederationExecution());
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      _federateSet.insert(getFederateList().begin(), getFederateList().end());

      if (!execJoined(*ambassador))
        return false;

      // and now resign must work
      try {
        ambassador->resignFederationExecution(rti1516ev::CANCEL_THEN_DELETE_THEN_DIVEST);
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

    }
    // Wait for all threads, this is to ensure that we do not destroy before we are ready
    wait();

    // destroy, must work once
    try {
      ambassador->destroyFederationExecution(getFederationExecution());

      if (!getFederationBarrier()->success())
        return false;
    } catch (const rti1516ev::FederatesCurrentlyJoined&) {
      // Can happen in this test
      // Other threads just might have still joined.

      if (!getFederationBarrier()->fail())
        return false;
    } catch (const rti1516ev::FederationExecutionDoesNotExist&) {
      // Can happen in this test
      // Other threads might have been faster

      if (!getFederationBarrier()->fail())
        return false;
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    return true;
  }

  virtual bool execJoinMultiple()
  {
    std::unique_ptr<rti1516ev::RTIambassador> ambassador;
    rti1516ev::RTIambassadorFactory factory;
    ambassador = factory.createRTIambassador();
    ambassador->setOperationWaitTimeout(10000);

    _connectionLost = false;
    ambassador->connect(*this, rti1516ev::HLA_EVOKED, getConnectUrl());

    // Try that several times. Ensure correct cleanup
    unsigned n = 99;
    for (unsigned i = 0; i < n; ++i) {

      // create, must work once
      try {
        if (getMimFile().empty())
          ambassador->createFederationExecution(getFederationExecution(), getFddFileList(), std::wstring(L"HLAinteger64Time"));
        else
          ambassador->createFederationExecutionWithMIM(getFederationExecution(), getFddFileList(), getMimFile(), std::wstring(L"HLAinteger64Time"));

        if (!getFederationBarrier()->success())
          return false;
      } catch (const rti1516ev::FederationExecutionAlreadyExists&) {
        // Can happen in this test

        if (!getFederationBarrier()->fail())
          return false;
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      // join must work
      try {
        ambassador->joinFederationExecution(getFederateType(), getFederationExecution());
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      _federateSet.insert(getFederateList().begin(), getFederateList().end());

      if (!execJoined(*ambassador))
        return false;

      // and now resign must work
      try {
        ambassador->resignFederationExecution(rti1516ev::CANCEL_THEN_DELETE_THEN_DIVEST);
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      // Wait for all threads, this is to ensure that we do not destroy before we are ready
      wait();

      // destroy, must work once
      try {
        ambassador->destroyFederationExecution(getFederationExecution());

        if (!getFederationBarrier()->success())
          return false;
      } catch (const rti1516ev::FederatesCurrentlyJoined&) {
        // Can happen in this test
        // Other threads just might have still joined.

        if (!getFederationBarrier()->fail())
          return false;
      } catch (const rti1516ev::FederationExecutionDoesNotExist&) {
        // Can happen in this test
        // Other threads might have been faster

        if (!getFederationBarrier()->fail())
          return false;
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
    }

    return true;
  }

  virtual bool exec() override
  {
    if (_constructorArgs._joinOnce)
      return execJoinOnce();
    else
      return execJoinMultiple();
  }

  virtual void connectionLost(const std::wstring& faultDescription) override
  {
    std::wcout << L"connection lost:" << faultDescription << std::endl;
    _connectionLost = true;
  }

  bool isConnectionLost() const
  {
    return _connectionLost;
  }

  virtual void reportFederationExecutions(const rti1516ev::FederationExecutionInformationVector& theFederationExecutionInformationList) override
  {
  }

  virtual void synchronizationPointRegistrationSucceeded(const std::wstring& label) override
  {
  }

  virtual void synchronizationPointRegistrationFailed(const std::wstring& label, rti1516ev::SynchronizationPointFailureReason reason) override
  {
  }

  virtual void announceSynchronizationPoint(const std::wstring& label, const rti1516ev::VariableLengthData& tag) override
  {
    _federateSet.erase(label);
  }

  virtual void federationSynchronized(const std::wstring& label, const rti1516ev::FederateHandleSet& failedToSyncSet) override
  {
    ++_synchronized;
  }

  virtual void initiateFederateSave(const std::wstring& label) override
  {
  }

  virtual void initiateFederateSave(const std::wstring& label, const rti1516ev::LogicalTime& logicalTime) override
  {
  }

  virtual void federationSaved() override
  {
  }

  virtual void federationNotSaved(rti1516ev::SaveFailureReason theSaveFailureReason) override
  {
  }

  virtual void federationSaveStatusResponse(const rti1516ev::FederateHandleSaveStatusPairVector& theFederateStatusVector) override
  {
  }

  virtual void requestFederationRestoreSucceeded(const std::wstring& label) override
  {
  }

  virtual void requestFederationRestoreFailed(const std::wstring& label) override
  {
  }

  virtual void federationRestoreBegun() override
  {
  }

  virtual void initiateFederateRestore(const std::wstring& label, const std::wstring& federateName, rti1516ev::FederateHandle handle) override
  {
  }

  virtual void federationRestored() override
  {
  }

  virtual void federationNotRestored(rti1516ev::RestoreFailureReason theRestoreFailureReason) override
  {
  }

  virtual void federationRestoreStatusResponse(const rti1516ev::FederateRestoreStatusVector& theFederateStatusVector) override
  {
  }

  virtual void startRegistrationForObjectClass(rti1516ev::ObjectClassHandle) override
  {
  }

  virtual void stopRegistrationForObjectClass(rti1516ev::ObjectClassHandle) override
  {
  }

  virtual void turnInteractionsOn(rti1516ev::InteractionClassHandle) override
  {
  }

  virtual void turnInteractionsOff(rti1516ev::InteractionClassHandle) override
  {
  }

  virtual void objectInstanceNameReservationSucceeded(const std::wstring&) override
  {
  }

  virtual void objectInstanceNameReservationFailed(const std::wstring&) override
  {
  }

  virtual void multipleObjectInstanceNameReservationSucceeded(const std::set<std::wstring>& theObjectInstanceNames) override
  {
  }

  virtual void multipleObjectInstanceNameReservationFailed(const std::set<std::wstring>& theObjectInstanceNames) override
  {
  }

  virtual void discoverObjectInstance(rti1516ev::ObjectInstanceHandle theObject, rti1516ev::ObjectClassHandle theObjectClass,
                                      std::wstring const & theObjectInstanceName) override
  {
  }

  virtual void discoverObjectInstance(rti1516ev::ObjectInstanceHandle theObject, rti1516ev::ObjectClassHandle theObjectClass,
                                      std::wstring const & theObjectInstanceName, rti1516ev::FederateHandle producingFederate) override
  {
    discoverObjectInstance(theObject, theObjectClass, theObjectInstanceName);
  }

  virtual void reflectAttributeValues(rti1516ev::ObjectInstanceHandle theObject, rti1516ev::AttributeHandleValueMap const & theAttributeValues,
                                      rti1516ev::VariableLengthData const & theUserSuppliedTag, rti1516ev::OrderType sentOrder,
                                      rti1516ev::TransportationType theType, rti1516ev::SupplementalReflectInfo theReflectInfo) override
  {
  }

  virtual void reflectAttributeValues(rti1516ev::ObjectInstanceHandle theObject, rti1516ev::AttributeHandleValueMap const & theAttributeValues,
                                      rti1516ev::VariableLengthData const & theUserSuppliedTag, rti1516ev::OrderType sentOrder,
                                      rti1516ev::TransportationType theType, rti1516ev::LogicalTime const & theTime,
                                      rti1516ev::OrderType receivedOrder, rti1516ev::SupplementalReflectInfo theReflectInfo) override
  {
    reflectAttributeValues(theObject, theAttributeValues, theUserSuppliedTag, sentOrder, theType, theReflectInfo);
  }

  virtual void reflectAttributeValues(rti1516ev::ObjectInstanceHandle theObject, rti1516ev::AttributeHandleValueMap const & theAttributeValues,
                                      rti1516ev::VariableLengthData const & theUserSuppliedTag, rti1516ev::OrderType sentOrder,
                                      rti1516ev::TransportationType theType, rti1516ev::LogicalTime const & theTime, rti1516ev::OrderType receivedOrder,
                                      rti1516ev::MessageRetractionHandle theHandle, rti1516ev::SupplementalReflectInfo theReflectInfo) override
  {
    reflectAttributeValues(theObject, theAttributeValues, theUserSuppliedTag, sentOrder, theType, theTime, receivedOrder, theReflectInfo);
  }

  virtual void receiveInteraction(rti1516ev::InteractionClassHandle theInteraction, rti1516ev::ParameterHandleValueMap const & theParameterValues,
                                  rti1516ev::VariableLengthData const & theUserSuppliedTag, rti1516ev::OrderType sentOrder, rti1516ev::TransportationType theType,
                                  rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
  {
  }

  virtual void receiveInteraction(rti1516ev::InteractionClassHandle theInteraction, rti1516ev::ParameterHandleValueMap const & theParameterValues,
                                  rti1516ev::VariableLengthData const & theUserSuppliedTag, rti1516ev::OrderType sentOrder, rti1516ev::TransportationType theType,
                                  rti1516ev::LogicalTime const & theTime, rti1516ev::OrderType receivedOrder, rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
  {
    receiveInteraction(theInteraction, theParameterValues, theUserSuppliedTag, sentOrder, theType, theReceiveInfo);
  }

  virtual void receiveInteraction(rti1516ev::InteractionClassHandle theInteraction, rti1516ev::ParameterHandleValueMap const & theParameterValues,
                                  rti1516ev::VariableLengthData const & theUserSuppliedTag, rti1516ev::OrderType sentOrder, rti1516ev::TransportationType theType,
                                  rti1516ev::LogicalTime const & theTime, rti1516ev::OrderType receivedOrder, rti1516ev::MessageRetractionHandle theHandle,
                                  rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
  {
    receiveInteraction(theInteraction, theParameterValues, theUserSuppliedTag, sentOrder, theType, theTime, receivedOrder, theReceiveInfo);
  }

  virtual void removeObjectInstance(rti1516ev::ObjectInstanceHandle theObject, rti1516ev::VariableLengthData const & theUserSuppliedTag,
                                    rti1516ev::OrderType sentOrder, rti1516ev::SupplementalRemoveInfo theRemoveInfo) override
  {
  }

  virtual void removeObjectInstance(rti1516ev::ObjectInstanceHandle theObject, rti1516ev::VariableLengthData const & theUserSuppliedTag, rti1516ev::OrderType sentOrder,
                                    rti1516ev::LogicalTime const & theTime, rti1516ev::OrderType receivedOrder, rti1516ev::SupplementalRemoveInfo theRemoveInfo) override
  {
    removeObjectInstance(theObject, theUserSuppliedTag, sentOrder, theRemoveInfo);
  }

  virtual void removeObjectInstance(rti1516ev::ObjectInstanceHandle theObject, rti1516ev::VariableLengthData const & theUserSuppliedTag, rti1516ev::OrderType sentOrder,
                                    rti1516ev::LogicalTime const & theTime, rti1516ev::OrderType receivedOrder, rti1516ev::MessageRetractionHandle theHandle,
                                    rti1516ev::SupplementalRemoveInfo theRemoveInfo) override
  {
    removeObjectInstance(theObject, theUserSuppliedTag, sentOrder, theTime, receivedOrder, theRemoveInfo);
  }

  virtual void attributesInScope(rti1516ev::ObjectInstanceHandle theObject,
                                 rti1516ev::AttributeHandleSet const & theAttributes) override
  {
  }

  virtual void attributesOutOfScope(rti1516ev::ObjectInstanceHandle theObject,
                                    rti1516ev::AttributeHandleSet const & theAttributes) override
  {
  }

  virtual void provideAttributeValueUpdate(rti1516ev::ObjectInstanceHandle theObject,
                                           rti1516ev::AttributeHandleSet const & theAttributes,
                                           rti1516ev::VariableLengthData const & theUserSuppliedTag) override
  {
  }

  virtual void turnUpdatesOnForObjectInstance(rti1516ev::ObjectInstanceHandle theObject,
                                              rti1516ev::AttributeHandleSet const & theAttributes) override
  {
  }

  virtual void turnUpdatesOnForObjectInstance(rti1516ev::ObjectInstanceHandle theObject,
                                              rti1516ev::AttributeHandleSet const & theAttributes,
                                              std::wstring const & updateRateDesignator) override
  {
  }

  virtual void turnUpdatesOffForObjectInstance(rti1516ev::ObjectInstanceHandle theObject,
                                               rti1516ev::AttributeHandleSet const & theAttributes) override
  {
  }

  virtual void confirmAttributeTransportationTypeChange(rti1516ev::ObjectInstanceHandle theObject,
                                                        rti1516ev::AttributeHandleSet theAttributes,
                                                        rti1516ev::TransportationType theTransportation) override
  {
  }

  virtual void reportAttributeTransportationType(rti1516ev::ObjectInstanceHandle theObject,
                                                 rti1516ev::AttributeHandle theAttribute,
                                                 rti1516ev::TransportationType theTransportation) override
  {
  }

  virtual void confirmInteractionTransportationTypeChange(rti1516ev::InteractionClassHandle theInteraction,
                                                          rti1516ev::TransportationType theTransportation) override
  {
  }

  virtual void reportInteractionTransportationType(rti1516ev::FederateHandle federateHandle,
                                                   rti1516ev::InteractionClassHandle theInteraction,
                                                   rti1516ev::TransportationType theTransportation) override
  {
  }

  virtual void requestAttributeOwnershipAssumption(rti1516ev::ObjectInstanceHandle theObject,
                                                   rti1516ev::AttributeHandleSet const & offeredAttributes,
                                                   rti1516ev::VariableLengthData const & theUserSuppliedTag) override
  {
  }

  virtual void requestDivestitureConfirmation(rti1516ev::ObjectInstanceHandle theObject,
                                              rti1516ev::AttributeHandleSet const & releasedAttributes) override
  {
  }

  virtual void attributeOwnershipAcquisitionNotification(rti1516ev::ObjectInstanceHandle theObject,
                                                         rti1516ev::AttributeHandleSet const & securedAttributes,
                                                         rti1516ev::VariableLengthData const & theUserSuppliedTag) override
  {
  }

  virtual void attributeOwnershipUnavailable(rti1516ev::ObjectInstanceHandle theObject,
                                             rti1516ev::AttributeHandleSet const & theAttributes) override
  {
  }

  virtual void requestAttributeOwnershipRelease(rti1516ev::ObjectInstanceHandle theObject,
                                                rti1516ev::AttributeHandleSet const & candidateAttributes,
                                                rti1516ev::VariableLengthData const & theUserSuppliedTag) override
  {
  }

  virtual void confirmAttributeOwnershipAcquisitionCancellation(rti1516ev::ObjectInstanceHandle theObject,
                                                                rti1516ev::AttributeHandleSet const & theAttributes) override
  {
  }

  virtual void informAttributeOwnership(rti1516ev::ObjectInstanceHandle theObject,
                                        rti1516ev::AttributeHandle theAttribute,
                                        rti1516ev::FederateHandle theOwner) override
  {
  }

  virtual void attributeIsNotOwned(rti1516ev::ObjectInstanceHandle theObject,
                                   rti1516ev::AttributeHandle theAttribute) override
  {
  }

  virtual void attributeIsOwnedByRTI(rti1516ev::ObjectInstanceHandle theObject,
                                     rti1516ev::AttributeHandle theAttribute) override
  {
  }

  virtual void timeRegulationEnabled(rti1516ev::LogicalTime const & theFederateTime) override
  {
  }

  virtual void timeConstrainedEnabled(rti1516ev::LogicalTime const & theFederateTime) override
  {
  }

  virtual void timeAdvanceGrant(rti1516ev::LogicalTime const & theTime) override
  {
  }

  virtual void requestRetraction(rti1516ev::MessageRetractionHandle theHandle) override
  {
  }

private:
  unsigned _synchronized;
  bool _connectionLost = false;
  std::set<std::wstring> _federateSet;
};

class OPENRTI_LOCAL RTI1516ESimpleAmbassador : public rti1516ev::FederateAmbassador {
public:
  RTI1516ESimpleAmbassador() :
    _useDataUrlObjectModels(false),
    // _fail(false),
    _timeRegulationEnabled(false),
    _timeConstrainedEnabled(false),
    _timeAdvancePending(false)
  {
    rti1516ev::RTIambassadorFactory factory;
    _ambassador = factory.createRTIambassador();
  }
  virtual ~RTI1516ESimpleAmbassador()
  {
  }

  void setUseDataUrlObjectModels(bool useDataUrlObjectModels)
  { _useDataUrlObjectModels = useDataUrlObjectModels; }
  bool getUseDataUrlObjectModels() const
  { return _useDataUrlObjectModels; }

  // bool getFail() const
  // { return _fail; }

  bool getTimeRegulationEnabled() const
  { return _timeRegulationEnabled; }
  bool getTimeConstrainedEnabled() const
  { return _timeConstrainedEnabled; }
  bool getTimeAdvancePending() const
  { return _timeAdvancePending; }

  const rti1516ev::FederateHandle& getFederateHandle() const
  { return _federateHandle; }

  void connect(const std::wstring& url)
  {
    _ambassador->connect(*this, rti1516ev::HLA_EVOKED, url);
    setLogicalTimeFactory();
  }

  void disconnect()
  {
    _ambassador->disconnect();
  }
  void shutdown()
  {
    _ambassador->shutdown();
  }

  void setConnectWaitTimeout(uint32_t timeoutMilliSeconds)
  {
    _ambassador->setConnectWaitTimeout(timeoutMilliSeconds);
  }

  void setOperationWaitTimeout(uint32_t timeoutMilliSeconds)
  {
    _ambassador->setOperationWaitTimeout(timeoutMilliSeconds);
  }

  void setLogicalTimeFactory(const std::wstring& logicalTimeImplementationName = std::wstring(L"HLAinteger64Time"))
  {
    _logicalTimeImplementationName = logicalTimeImplementationName;
    _logicalTimeFactory = rti1516ev::LogicalTimeFactoryFactory::makeLogicalTimeFactory(logicalTimeImplementationName);
  }

  void createFederationExecution(const std::wstring& federationExecutionName, std::wstring fddFile)
  {
    _replaceFileWithDataIfNeeded(fddFile);
    _ambassador->createFederationExecution(federationExecutionName, fddFile, _logicalTimeImplementationName);
  }
  void createFederationExecution(const std::wstring& federationExecutionName, std::vector<std::wstring> fomModules)
  {
    _replaceFilesWithDataIfNeeded(fomModules);
    _ambassador->createFederationExecution(federationExecutionName, fomModules, _logicalTimeImplementationName);
  }
  void createFederationExecutionWithMIM(const std::wstring& federationExecutionName, std::vector<std::wstring> fomModules,
                                        std::wstring mimModule)
  {
    _replaceFilesWithDataIfNeeded(fomModules);
    _replaceFileWithDataIfNeeded(mimModule);
    _ambassador->createFederationExecutionWithMIM(federationExecutionName, fomModules, mimModule, _logicalTimeImplementationName);
  }

  void destroyFederationExecution(const std::wstring& federationExecutionName)
  {
    _ambassador->destroyFederationExecution(federationExecutionName);
  }

  const rti1516ev::FederateHandle& joinFederationExecution(const std::wstring& federateType,
                                                          const std::wstring& federationExecutionName,
                                                          std::vector<std::wstring> additionalFomModules = std::vector<std::wstring>())
  {
    _replaceFilesWithDataIfNeeded(additionalFomModules);
    _federateHandle = _ambassador->joinFederationExecution(federateType, federationExecutionName, additionalFomModules);
    _grantedLogicalTime = _logicalTimeFactory->makeInitial();
    return _federateHandle;
  }

  const rti1516ev::FederateHandle& joinFederationExecution(const std::wstring& federateName, const std::wstring& federateType,
                                                          const std::wstring& federationExecutionName,
                                                          std::vector<std::wstring> additionalFomModules = std::vector<std::wstring>())
  {
    _replaceFilesWithDataIfNeeded(additionalFomModules);
    _federateHandle = _ambassador->joinFederationExecution(federateName, federateType, federationExecutionName, additionalFomModules);
    _grantedLogicalTime = _logicalTimeFactory->makeInitial();
    return _federateHandle;
  }

  void resignFederationExecution(rti1516ev::ResignAction resignAction)
  {
    _ambassador->resignFederationExecution(resignAction);
    _federateHandle = rti1516ev::FederateHandle();
  }

  void registerFederationSynchronizationPoint(const std::wstring& label, const rti1516ev::VariableLengthData& tag)
  {
    _ambassador->registerFederationSynchronizationPoint(label, tag);
  }

  void registerFederationSynchronizationPoint(const std::wstring& label, const rti1516ev::VariableLengthData& tag,
                                              const rti1516ev::FederateHandleSet& federateHandleSet)
  {
    _ambassador->registerFederationSynchronizationPoint(label, tag, federateHandleSet);
  }

  void synchronizationPointAchieved(const std::wstring& label)
  {
    _ambassador->synchronizationPointAchieved(label);
  }

  void requestFederationSave(const std::wstring& label)
  {
    _ambassador->requestFederationSave(label);
  }

  void requestFederationSave(const std::wstring& label, const rti1516ev::LogicalTime& logicalTime)
  {
    _ambassador->requestFederationSave(label, logicalTime);
  }

  void federateSaveComplete()
  {
    _ambassador->federateSaveComplete();
  }

  void federateSaveNotComplete()
  {
    _ambassador->federateSaveNotComplete();
  }

  void queryFederationSaveStatus()
  {
    _ambassador->queryFederationSaveStatus();
  }

  void requestFederationRestore(const std::wstring& label)
  {
    _ambassador->requestFederationRestore(label);
  }

  void federateRestoreComplete()
  {
    _ambassador->federateRestoreComplete();
  }

  void federateRestoreNotComplete()
  {
    _ambassador->federateRestoreNotComplete();
  }

  void queryFederationRestoreStatus()
  {
    _ambassador->queryFederationRestoreStatus();
  }

  void publishObjectClassAttributes(const rti1516ev::ObjectClassHandle& objectClassHandle,
                                    const rti1516ev::AttributeHandleSet& attributeList)
  {
    _ambassador->publishObjectClassAttributes(objectClassHandle, attributeList);
  }

  void unpublishObjectClass(const rti1516ev::ObjectClassHandle& objectClassHandle)
  {
    _ambassador->unpublishObjectClass(objectClassHandle);
  }

  void unpublishObjectClassAttributes(const rti1516ev::ObjectClassHandle& objectClassHandle,
                                      const rti1516ev::AttributeHandleSet& attributeList)
  {
    _ambassador->unpublishObjectClassAttributes(objectClassHandle, attributeList);
  }

  void publishInteractionClass(const rti1516ev::InteractionClassHandle& interactionClassHandle)
  {
    _ambassador->publishInteractionClass(interactionClassHandle);
  }

  void unpublishInteractionClass(const rti1516ev::InteractionClassHandle& interactionClassHandle)
  {
    _ambassador->unpublishInteractionClass(interactionClassHandle);
  }

  void subscribeObjectClassAttributes(const rti1516ev::ObjectClassHandle& objectClassHandle,
                                      const rti1516ev::AttributeHandleSet& attributeHandleSet,
                                      bool active = true)
  {
      _ambassador->subscribeObjectClassAttributes(objectClassHandle, attributeHandleSet, active);
      // _subscribedObjectClassAttributeHandleSetMap[objectClassHandle].insert(attributeHandleSet.begin(), attributeHandleSet.end());
  }

  void unsubscribeObjectClass(const rti1516ev::ObjectClassHandle& objectClassHandle)
  {
      _ambassador->unsubscribeObjectClass(objectClassHandle);
      // _subscribedObjectClassAttributeHandleSetMap.erase(objectClassHandle);
  }

  void unsubscribeObjectClassAttributes(const rti1516ev::ObjectClassHandle& objectClassHandle,
                                        const rti1516ev::AttributeHandleSet& attributeHandleSet)
  {
      _ambassador->unsubscribeObjectClassAttributes(objectClassHandle, attributeHandleSet);
      // ObjectClassAttributeHandleSetMap::iterator i = _subscribedObjectClassAttributeHandleSetMap.find(objectClassHandle);
      // for (rti1516ev::AttributeHandleSet::const_iterator j = attributeHandleSet.begin();
      //      j != attributeHandleSet.end(); ++j)
      //     i->second.erase(*j);
  }

  void subscribeInteractionClass(const rti1516ev::InteractionClassHandle& interactionClassHandle, bool active = true)
  {
      _ambassador->subscribeInteractionClass(interactionClassHandle, active);
  }

  void unsubscribeInteractionClass(const rti1516ev::InteractionClassHandle& interactionClassHandle)
  {
      _ambassador->unsubscribeInteractionClass(interactionClassHandle);
  }

  void reserveObjectInstanceName(const std::wstring& objectInstanceName)
  {
      _ambassador->reserveObjectInstanceName(objectInstanceName);
  }

  rti1516ev::ObjectInstanceHandle registerObjectInstance(const rti1516ev::ObjectClassHandle& objectClassHandle)
  {
      return _ambassador->registerObjectInstance(objectClassHandle);
  }

  rti1516ev::ObjectInstanceHandle registerObjectInstance(const rti1516ev::ObjectClassHandle& objectClassHandle,
                                                       const std::wstring& objectInstanceName)
  {
      return _ambassador->registerObjectInstance(objectClassHandle, objectInstanceName);
  }

  void updateAttributeValues(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle,
                             const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap,
                             const rti1516ev::VariableLengthData& tag)
  {
      _ambassador->updateAttributeValues(objectInstanceHandle, attributeHandleValueMap, tag);
  }

  rti1516ev::MessageRetractionHandle updateAttributeValues(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle,
                                                         const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap,
                                                         const rti1516ev::VariableLengthData& tag,
                                                         const rti1516ev::LogicalTime& logicalTime)
  {
    return _ambassador->updateAttributeValues(objectInstanceHandle, attributeHandleValueMap, tag, logicalTime);
  }

  void sendInteraction(const rti1516ev::InteractionClassHandle& interactionClassHandle,
                       const rti1516ev::ParameterHandleValueMap& parameterHandleValueMap,
                       const rti1516ev::VariableLengthData& tag)
  {
      _ambassador->sendInteraction(interactionClassHandle, parameterHandleValueMap, tag);
  }

  rti1516ev::MessageRetractionHandle sendInteraction(const rti1516ev::InteractionClassHandle& interactionClassHandle,
                                                   const rti1516ev::ParameterHandleValueMap& parameterHandleValueMap,
                                                   const rti1516ev::VariableLengthData& tag,
                                                   const rti1516ev::LogicalTime& logicalTime)
  {
      return _ambassador->sendInteraction(interactionClassHandle, parameterHandleValueMap, tag, logicalTime);
  }

  void deleteObjectInstance(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle,
                            const rti1516ev::VariableLengthData& tag)
  {
    _ambassador->deleteObjectInstance(objectInstanceHandle, tag);
  }

  rti1516ev::MessageRetractionHandle deleteObjectInstance(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle,
                                                        const rti1516ev::VariableLengthData& tag,
                                                        const rti1516ev::LogicalTime& logicalTime)
  {
    return _ambassador->deleteObjectInstance(objectInstanceHandle, tag, logicalTime);
  }

  void localDeleteObjectInstance(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle)
  {
    _ambassador->localDeleteObjectInstance(objectInstanceHandle);
  }

  void requestAttributeTransportationTypeChange(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle,
                                                const rti1516ev::AttributeHandleSet& attributeHandleSet,
                                                const rti1516ev::TransportationType& transportationType)
  {
    _ambassador->requestAttributeTransportationTypeChange(objectInstanceHandle, attributeHandleSet, transportationType);
  }

  void requestInteractionTransportationTypeChange(const rti1516ev::InteractionClassHandle& interactionClassHandle,
                                                  const rti1516ev::TransportationType& transportationType)
  {
    _ambassador->requestInteractionTransportationTypeChange(interactionClassHandle, transportationType);
  }

  void requestAttributeValueUpdate(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle,
                                   const rti1516ev::AttributeHandleSet& attributeHandleSet,
                                   const rti1516ev::VariableLengthData& tag)
  {
    _ambassador->requestAttributeValueUpdate(objectInstanceHandle, attributeHandleSet, tag);
  }

  void requestAttributeValueUpdate(const rti1516ev::ObjectClassHandle& objectClassHandle,
                                   const rti1516ev::AttributeHandleSet& attributeHandleSet,
                                   const rti1516ev::VariableLengthData& tag)
  {
    _ambassador->requestAttributeValueUpdate(objectClassHandle, attributeHandleSet, tag);
  }

  void unconditionalAttributeOwnershipDivestiture(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle,
                                                  const rti1516ev::AttributeHandleSet& attributeHandleSet)
  {
    _ambassador->unconditionalAttributeOwnershipDivestiture(objectInstanceHandle, attributeHandleSet);
  }

    // // 7.3
    //  void negotiatedAttributeOwnershipDivestiture
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSet const & attributes,
    //  VariableLengthData const & tag)

    // // 7.6
    //  void confirmDivestiture
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSet const & confirmedAttributes,
    //  VariableLengthData const & tag)

    // // 7.8
    //  void attributeOwnershipAcquisition
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSet const & desiredAttributes,
    //  VariableLengthData const & tag)

    // // 7.9
    //  void attributeOwnershipAcquisitionIfAvailable
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSet const & desiredAttributes)

    // // 7.12
    //  void attributeOwnershipDivestitureIfWanted
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSet const & attributes,
    //  AttributeHandleSet & theDivestedAttributes) // filled by RTI

    // // 7.13
    //  void cancelNegotiatedAttributeOwnershipDivestiture
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSet const & attributes)

    // // 7.14
    //  void cancelAttributeOwnershipAcquisition
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSet const & attributes)

    // // 7.16
    //  void queryAttributeOwnership
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandle attribute)

    // // 7.18
    //  bool isAttributeOwnedByFederate
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandle attribute)

  void enableTimeRegulation(const rti1516ev::LogicalTimeInterval& logicalTimeInterval)
  {
    _ambassador->enableTimeRegulation(logicalTimeInterval);
  }

  void disableTimeRegulation()
  {
    _timeRegulationEnabled = false;
    _ambassador->disableTimeRegulation();
  }

  void enableTimeConstrained()
  {
    _ambassador->enableTimeConstrained();
  }

  void disableTimeConstrained()
  {
    _timeConstrainedEnabled = false;
    _ambassador->disableTimeConstrained();
  }

  void timeAdvanceRequest(const rti1516ev::LogicalTime& logicalTime)
  {
    _timeAdvancePending = true;
    _ambassador->timeAdvanceRequest(logicalTime);
  }

  void timeAdvanceRequestAvailable(const rti1516ev::LogicalTime& logicalTime)
  {
    _timeAdvancePending = true;
    _ambassador->timeAdvanceRequestAvailable(logicalTime);
  }

  void nextMessageRequest(const rti1516ev::LogicalTime& logicalTime)
  {
    _timeAdvancePending = true;
    _ambassador->nextMessageRequest(logicalTime);
  }

  void nextMessageRequestAvailable(const rti1516ev::LogicalTime& logicalTime)
  {
    _timeAdvancePending = true;
    _ambassador->nextMessageRequestAvailable(logicalTime);
  }

  void flushQueueRequest(const rti1516ev::LogicalTime& logicalTime)
  {
    // _timeAdvancePending = true;
    _ambassador->flushQueueRequest(logicalTime);
  }

  void enableAsynchronousDelivery()
  {
    _ambassador->enableAsynchronousDelivery();
  }

  void disableAsynchronousDelivery()
  {
    _ambassador->disableAsynchronousDelivery();
  }

    //  bool queryGALT (LogicalTime & logicalTime)

    //  void queryLogicalTime (LogicalTime & logicalTime)

    //  bool queryLITS (LogicalTime & logicalTime)

    //  void modifyLookahead

    //  void queryLookahead (LogicalTimeInterval & interval)

    //  void retract
    // (MessageRetractionHandle theHandle)

    //  void changeAttributeOrderType
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSet const & attributes,
    //  OrderType theType)

    //  void changeInteractionOrderType
    // (InteractionClassHandle theClass,
    //  OrderType theType)

    //  RegionHandle createRegion
    // (DimensionHandleSet const & theDimensions)

    //  void commitRegionModifications
    // (RegionHandleSet const & regionHandleSet)

    //  void deleteRegion
    // (RegionHandle region)

    //  ObjectInstanceHandle registerObjectInstanceWithRegions
    // (ObjectClassHandle theClass,
    //  AttributeHandleSetRegionHandleSetPairVector const &
    //  attributeHandleSetRegionHandleSetPairVector)

    //  ObjectInstanceHandle registerObjectInstanceWithRegions
    // (ObjectClassHandle theClass,
    //  AttributeHandleSetRegionHandleSetPairVector const &
    //  attributeHandleSetRegionHandleSetPairVector,
    //  std::wstring const & objectInstanceName)

    //  void associateRegionsForUpdates
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSetRegionHandleSetPairVector const &
    //  attributeHandleSetRegionHandleSetPairVector)

    //  void unassociateRegionsForUpdates
    // (ObjectInstanceHandle objectInstanceHandle,
    //  AttributeHandleSetRegionHandleSetPairVector const &
    //  attributeHandleSetRegionHandleSetPairVector)

    //  void subscribeObjectClassAttributesWithRegions
    // (ObjectClassHandle theClass,
    //  AttributeHandleSetRegionHandleSetPairVector const &
    //  attributeHandleSetRegionHandleSetPairVector,
    //  bool active = true)

    //  void unsubscribeObjectClassAttributesWithRegions
    // (ObjectClassHandle theClass,
    //  AttributeHandleSetRegionHandleSetPairVector const &
    //  attributeHandleSetRegionHandleSetPairVector)

    //  void subscribeInteractionClassWithRegions
    // (InteractionClassHandle theClass,
    //  RegionHandleSet const & regionHandleSet,
    //  bool active = true)

    //  void unsubscribeInteractionClassWithRegions
    // (InteractionClassHandle theClass,
    //  RegionHandleSet const & regionHandleSet)

    //  void sendInteractionWithRegions
    // (InteractionClassHandle interaction,
    //  ParameterHandleValueMap const & parameterValues,
    //  RegionHandleSet const & regionHandleSet,
    //  VariableLengthData const & tag)

    //  MessageRetractionHandle sendInteractionWithRegions
    // (InteractionClassHandle interaction,
    //  ParameterHandleValueMap const & parameterValues,
    //  RegionHandleSet const & regionHandleSet,
    //  VariableLengthData const & tag,
    //  LogicalTime const & logicalTime)

    //  void requestAttributeValueUpdateWithRegions
    // (ObjectClassHandle theClass,
    //  AttributeHandleSetRegionHandleSetPairVector const & theSet,
    //  VariableLengthData const & tag)

  rti1516ev::ObjectClassHandle getObjectClassHandle(std::wstring const & theName)
  {
    return _ambassador->getObjectClassHandle(theName);
  }

  std::wstring getObjectClassName(rti1516ev::ObjectClassHandle theHandle)
  {
    return _ambassador->getObjectClassName(theHandle);
  }

  rti1516ev::AttributeHandle getAttributeHandle(rti1516ev::ObjectClassHandle whichClass, std::wstring const & attributeName)
  {
    return _ambassador->getAttributeHandle(whichClass, attributeName);
  }

  std::wstring getAttributeName(rti1516ev::ObjectClassHandle whichClass, rti1516ev::AttributeHandle theHandle)
  {
    return _ambassador->getAttributeName(whichClass, theHandle);
  }

  double getUpdateRateValue(std::wstring const & updateRateDesignator)
  {
    return _ambassador->getUpdateRateValue(updateRateDesignator);
  }

  double getUpdateRateValueForAttribute(rti1516ev::ObjectInstanceHandle objectInstanceHandle, rti1516ev::AttributeHandle attributeHandle)
  {
    return _ambassador->getUpdateRateValueForAttribute(objectInstanceHandle, attributeHandle);
  }

  rti1516ev::InteractionClassHandle getInteractionClassHandle(std::wstring const & theName)
  {
    return _ambassador->getInteractionClassHandle(theName);
  }

  std::wstring getInteractionClassName(rti1516ev::InteractionClassHandle theHandle)
  {
    return _ambassador->getInteractionClassName(theHandle);
  }

  rti1516ev::ParameterHandle getParameterHandle(rti1516ev::InteractionClassHandle whichClass, std::wstring const & theName)
  {
    return _ambassador->getParameterHandle(whichClass, theName);
  }

  std::wstring getParameterName(rti1516ev::InteractionClassHandle whichClass, rti1516ev::ParameterHandle theHandle)
  {
    return _ambassador->getParameterName(whichClass, theHandle);
  }

  rti1516ev::ObjectInstanceHandle getObjectInstanceHandle(std::wstring const & theName)
  {
    return _ambassador->getObjectInstanceHandle(theName);
  }

  std::wstring getObjectInstanceName(rti1516ev::ObjectInstanceHandle theHandle)
  {
    return _ambassador->getObjectInstanceName(theHandle);
  }

  rti1516ev::DimensionHandle getDimensionHandle(std::wstring const & theName)
  {
    return _ambassador->getDimensionHandle(theName);
  }

  std::wstring getDimensionName(rti1516ev::DimensionHandle theHandle)
  {
    return _ambassador->getDimensionName(theHandle);
  }

  unsigned long getDimensionUpperBound(rti1516ev::DimensionHandle theHandle)
  {
    return _ambassador->getDimensionUpperBound(theHandle);
  }

  rti1516ev::DimensionHandleSet getAvailableDimensionsForClassAttribute(rti1516ev::ObjectClassHandle theClass,
                                                                      rti1516ev::AttributeHandle theHandle)
  {
    return _ambassador->getAvailableDimensionsForClassAttribute(theClass, theHandle);
  }

  rti1516ev::ObjectClassHandle getKnownObjectClassHandle(rti1516ev::ObjectInstanceHandle object)
  {
    return _ambassador->getKnownObjectClassHandle(object);
  }

  rti1516ev::DimensionHandleSet getAvailableDimensionsForInteractionClass(rti1516ev::InteractionClassHandle theClass)
  {
    return _ambassador->getAvailableDimensionsForInteractionClass(theClass);
  }

  rti1516ev::TransportationType getTransportationType(std::wstring const & transportationName)
  {
    return _ambassador->getTransportationType(transportationName);
  }

  std::wstring getTransportationName(rti1516ev::TransportationType transportationType)
  {
    return _ambassador->getTransportationName(transportationType);
  }

  rti1516ev::OrderType getOrderType(std::wstring const & orderName)
  {
    return _ambassador->getOrderType(orderName);
  }

  std::wstring getOrderName(rti1516ev::OrderType orderType)
  {
    return _ambassador->getOrderName(orderType);
  }

  void enableObjectClassRelevanceAdvisorySwitch()
  {
    _ambassador->enableObjectClassRelevanceAdvisorySwitch();
  }

  void disableObjectClassRelevanceAdvisorySwitch()
  {
    _ambassador->disableObjectClassRelevanceAdvisorySwitch();
  }

  void enableAttributeRelevanceAdvisorySwitch()
  {
    _ambassador->enableAttributeRelevanceAdvisorySwitch();
  }

  void disableAttributeRelevanceAdvisorySwitch()
  {
    _ambassador->disableAttributeRelevanceAdvisorySwitch();
  }

  void enableAttributeScopeAdvisorySwitch()
  {
    _ambassador->enableAttributeScopeAdvisorySwitch();
  }

  void disableAttributeScopeAdvisorySwitch()
  {
    _ambassador->disableAttributeScopeAdvisorySwitch();
  }

  void enableInteractionRelevanceAdvisorySwitch()
  {
    _ambassador->enableInteractionRelevanceAdvisorySwitch();
  }

  void disableInteractionRelevanceAdvisorySwitch()
  {
    _ambassador->disableInteractionRelevanceAdvisorySwitch();
  }

  rti1516ev::DimensionHandleSet getDimensionHandleSet(rti1516ev::RegionHandle regionHandle)
  {
    return _ambassador->getDimensionHandleSet(regionHandle);
  }

  rti1516ev::RangeBounds getRangeBounds(rti1516ev::RegionHandle regionHandle, rti1516ev::DimensionHandle theDimensionHandle)
  {
    return _ambassador->getRangeBounds(regionHandle, theDimensionHandle);
  }

  void setRangeBounds(rti1516ev::RegionHandle regionHandle, rti1516ev::DimensionHandle theDimensionHandle,
                      rti1516ev::RangeBounds const & rangeBounds)
  {
    return _ambassador->setRangeBounds(regionHandle, theDimensionHandle, rangeBounds);
  }

  unsigned long normalizeFederateHandle(rti1516ev::FederateHandle federateHandle)
  {
    return _ambassador->normalizeFederateHandle(federateHandle);
  }

  unsigned long normalizeServiceGroup(rti1516ev::ServiceGroup theServiceGroup)
  {
    return _ambassador->normalizeServiceGroup(theServiceGroup);
  }

  bool evokeCallback(double approximateMinimumTimeInSeconds)
  {
    return _ambassador->evokeCallback(approximateMinimumTimeInSeconds);
  }

  bool evokeMultipleCallbacks(double approximateMinimumTimeInSeconds,
                              double approximateMaximumTimeInSeconds)
  {
    return _ambassador->evokeMultipleCallbacks(approximateMinimumTimeInSeconds, approximateMaximumTimeInSeconds);
  }

  void enableCallbacks()
  {
    _ambassador->enableCallbacks();
  }

  void disableCallbacks()
  {
    _ambassador->disableCallbacks();
  }

  rti1516ev::FederateHandle decodeFederateHandle(rti1516ev::VariableLengthData const& encodedValue) const
  {
    return _ambassador->decodeFederateHandle(encodedValue);
  }

  rti1516ev::ObjectClassHandle decodeObjectClassHandle(rti1516ev::VariableLengthData const & encodedValue) const
  {
    return _ambassador->decodeObjectClassHandle(encodedValue);
  }

  rti1516ev::InteractionClassHandle decodeInteractionClassHandle(rti1516ev::VariableLengthData const & encodedValue) const
  {
    return _ambassador->decodeInteractionClassHandle(encodedValue);
  }

  rti1516ev::ObjectInstanceHandle decodeObjectInstanceHandle(rti1516ev::VariableLengthData const & encodedValue) const
  {
    return _ambassador->decodeObjectInstanceHandle(encodedValue);
  }

  rti1516ev::AttributeHandle decodeAttributeHandle(rti1516ev::VariableLengthData const & encodedValue) const
  {
    return _ambassador->decodeAttributeHandle(encodedValue);
  }

  rti1516ev::ParameterHandle decodeParameterHandle(rti1516ev::VariableLengthData const & encodedValue) const
  {
    return _ambassador->decodeParameterHandle(encodedValue);
  }

  rti1516ev::DimensionHandle decodeDimensionHandle(rti1516ev::VariableLengthData const & encodedValue) const
  {
    return _ambassador->decodeDimensionHandle(encodedValue);
  }

  rti1516ev::MessageRetractionHandle decodeMessageRetractionHandle(rti1516ev::VariableLengthData const & encodedValue) const
  {
    return _ambassador->decodeMessageRetractionHandle(encodedValue);
  }

  rti1516ev::RegionHandle decodeRegionHandle(rti1516ev::VariableLengthData const & encodedValue) const
  {
    return _ambassador->decodeRegionHandle(encodedValue);
  }

  bool isConnectionLost() const
  {
    return _connectionLost;
  }

protected:
  virtual void connectionLost(const std::wstring& faultDescription) override
  {
    std::wcout << L"connection lost:" << faultDescription << std::endl;
    _connectionLost = true;
  }

  virtual void reportFederationExecutions(const rti1516ev::FederationExecutionInformationVector& theFederationExecutionInformationList) override
  {
  }

  virtual void synchronizationPointRegistrationSucceeded(const std::wstring& label) override
  {
  }

  virtual void synchronizationPointRegistrationFailed(const std::wstring& label, rti1516ev::SynchronizationPointFailureReason reason) override
  {
  }

  virtual void announceSynchronizationPoint(const std::wstring& label, const rti1516ev::VariableLengthData& tag) override
  {
  }

  virtual void federationSynchronized(const std::wstring& label, const rti1516ev::FederateHandleSet& failedToSyncSet) override
  {
  }

  virtual void initiateFederateSave(const std::wstring& label) override
  {
  }

  virtual void initiateFederateSave(const std::wstring& label, const rti1516ev::LogicalTime& logicalTime) override
  {
  }

  virtual void federationSaved() override
  {
  }

  virtual void federationNotSaved(rti1516ev::SaveFailureReason theSaveFailureReason) override
  {
  }

  virtual void federationSaveStatusResponse(const rti1516ev::FederateHandleSaveStatusPairVector& federateStatusVector) override
  {
  }

  virtual void requestFederationRestoreSucceeded(const std::wstring& label) override
  {
  }

  virtual void requestFederationRestoreFailed(const std::wstring& label) override
  {
  }

  virtual void federationRestoreBegun() override
  {
  }

  virtual void initiateFederateRestore(const std::wstring& label, const std::wstring& federateName, rti1516ev::FederateHandle handle) override
  {
  }

  virtual void federationRestored() override
  {
  }

  virtual void federationNotRestored(rti1516ev::RestoreFailureReason restoreFailureReason) override
  {
  }

  virtual void federationRestoreStatusResponse(const rti1516ev::FederateRestoreStatusVector& federateStatusVector) override
  {
  }

  virtual void startRegistrationForObjectClass(rti1516ev::ObjectClassHandle) override
  {
  }

  virtual void stopRegistrationForObjectClass(rti1516ev::ObjectClassHandle) override
  {
  }

  virtual void turnInteractionsOn(rti1516ev::InteractionClassHandle) override
  {
  }

  virtual void turnInteractionsOff(rti1516ev::InteractionClassHandle) override
  {
  }

  virtual void objectInstanceNameReservationSucceeded(const std::wstring&) override
  {
  }

  virtual void objectInstanceNameReservationFailed(const std::wstring&) override
  {
  }

  virtual void multipleObjectInstanceNameReservationSucceeded(const std::set<std::wstring>& theObjectInstanceNames) override
  {
  }

  virtual void multipleObjectInstanceNameReservationFailed(const std::set<std::wstring>& theObjectInstanceNames) override
  {
  }

  virtual void discoverObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                      rti1516ev::ObjectClassHandle objectClassHandle,
                                      const std::wstring& objectInstanceName) override
  {
      // ObjectClassAttributeHandleSetMap::iterator i = _subscribedObjectClassAttributeHandleSetMap.find(objectClassHandle);
      // if (i == _subscribedObjectClassAttributeHandleSetMap.end()) {
      //     fail();
      //     throw rti1516ev::FederateInternalError(objectClassHandle.toString());
      // }

      // if (_objectInstanceMap.find(objectInstanceHandle) != _objectInstanceMap.end()) {
      //     fail();
      //     throw rti1516ev::FederateInternalError(objectInstanceHandle.toString());
      // }

      // _objectInstanceMap[objectInstanceHandle]._objectClassHandle = objectClassHandle;
  }

  virtual void discoverObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                      rti1516ev::ObjectClassHandle objectClassHandle, const std::wstring& objectInstanceName,
                                      rti1516ev::FederateHandle producingFederate) override
  {
    discoverObjectInstance(objectInstanceHandle, objectClassHandle, objectInstanceName);
  }

  virtual void reflectAttributeValues(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                      const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap,
                                      const rti1516ev::VariableLengthData& tag, rti1516ev::OrderType, rti1516ev::TransportationType,
                                      rti1516ev::SupplementalReflectInfo) override
  {
      // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
  }

  //virtual void reflectAttributeValues(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
  //                                    const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap,
  //                                    const rti1516ev::VariableLengthData&, rti1516ev::OrderType, rti1516ev::TransportationType,
  //                                    const rti1516ev::RegionHandleSet&, rti1516ev::SupplementalReflectInfo)
  //{
  //    // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
  //}

  virtual void reflectAttributeValues(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                      const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap,
                                      const rti1516ev::VariableLengthData&, rti1516ev::OrderType, rti1516ev::TransportationType,
                                      const rti1516ev::LogicalTime&, rti1516ev::OrderType, rti1516ev::SupplementalReflectInfo) override
  {
      // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
  }

  //virtual void reflectAttributeValues(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
  //                                    const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap,
  //                                    const rti1516ev::VariableLengthData&, rti1516ev::OrderType, rti1516ev::TransportationType,
  //                                    const rti1516ev::LogicalTime&, rti1516ev::OrderType, const rti1516ev::RegionHandleSet&,
  //                                    rti1516ev::SupplementalReflectInfo)
  //{
  //    // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
  //}

  virtual void reflectAttributeValues(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                      const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap,
                                      const rti1516ev::VariableLengthData&, rti1516ev::OrderType, rti1516ev::TransportationType,
                                      const rti1516ev::LogicalTime&, rti1516ev::OrderType, rti1516ev::MessageRetractionHandle,
                                      rti1516ev::SupplementalReflectInfo) override
  {
      // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
  }

  //virtual void reflectAttributeValues(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
  //                                    const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap,
  //                                    const rti1516ev::VariableLengthData&, rti1516ev::OrderType, rti1516ev::TransportationType,
  //                                    const rti1516ev::LogicalTime&, rti1516ev::OrderType, rti1516ev::MessageRetractionHandle,
  //                                    const rti1516ev::RegionHandleSet&, rti1516ev::SupplementalReflectInfo)
  //{
  //    // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
  //}

  virtual void receiveInteraction(rti1516ev::InteractionClassHandle, const rti1516ev::ParameterHandleValueMap&,
                                  const rti1516ev::VariableLengthData&, rti1516ev::OrderType, rti1516ev::TransportationType,
                                  rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
  {
  }

  //virtual void receiveInteraction(rti1516ev::InteractionClassHandle, const rti1516ev::ParameterHandleValueMap&,
  //                                const rti1516ev::VariableLengthData&, rti1516ev::OrderType, rti1516ev::TransportationType,
  //                                const rti1516ev::RegionHandleSet&, rti1516ev::SupplementalReceiveInfo theReceiveInfo)
  //{
  //}

  virtual void receiveInteraction(rti1516ev::InteractionClassHandle interaction,
                                  rti1516ev::ParameterHandleValueMap const & parameterValues,
                                  rti1516ev::VariableLengthData const & tag,
                                  rti1516ev::OrderType sentOrder,
                                  rti1516ev::TransportationType theType,
                                  rti1516ev::LogicalTime const & logicalTime,
                                  rti1516ev::OrderType receivedOrder,
                                  rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
  {
  }

  //virtual void receiveInteraction(rti1516ev::InteractionClassHandle interaction,
  //                                rti1516ev::ParameterHandleValueMap const & parameterValues,
  //                                rti1516ev::VariableLengthData const & tag,
  //                                rti1516ev::OrderType sentOrder,
  //                                rti1516ev::TransportationType theType,
  //                                rti1516ev::LogicalTime const & logicalTime,
  //                                rti1516ev::OrderType receivedOrder,
  //                                rti1516ev::RegionHandleSet const & theSentRegionHandleSet,
  //                                rti1516ev::SupplementalReceiveInfo theReceiveInfo)
  //{
  //}

  virtual void receiveInteraction(rti1516ev::InteractionClassHandle interaction,
                                  rti1516ev::ParameterHandleValueMap const & parameterValues,
                                  rti1516ev::VariableLengthData const & tag,
                                  rti1516ev::OrderType sentOrder,
                                  rti1516ev::TransportationType theType,
                                  rti1516ev::LogicalTime const & logicalTime,
                                  rti1516ev::OrderType receivedOrder,
                                  rti1516ev::MessageRetractionHandle theHandle,
                                  rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
  {
  }

  //virtual void receiveInteraction(rti1516ev::InteractionClassHandle interaction,
  //                                rti1516ev::ParameterHandleValueMap const & parameterValues,
  //                                rti1516ev::VariableLengthData const & tag,
  //                                rti1516ev::OrderType sentOrder,
  //                                rti1516ev::TransportationType theType,
  //                                rti1516ev::LogicalTime const & logicalTime,
  //                                rti1516ev::OrderType receivedOrder,
  //                                rti1516ev::MessageRetractionHandle theHandle,
  //                                rti1516ev::RegionHandleSet const & theSentRegionHandleSet,
  //                                rti1516ev::SupplementalReceiveInfo theReceiveInfo)
  //{
  //}

  virtual void removeObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                    rti1516ev::VariableLengthData const & tag,
                                    rti1516ev::OrderType sentOrder,
                                    rti1516ev::SupplementalRemoveInfo theRemoveInfo) override
  {
      // _verifyRemoveObjectInstance(objectInstanceHandle);
  }

  virtual void removeObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                    rti1516ev::VariableLengthData const & tag,
                                    rti1516ev::OrderType sentOrder,
                                    rti1516ev::LogicalTime const & logicalTime,
                                    rti1516ev::OrderType receivedOrder,
                                    rti1516ev::SupplementalRemoveInfo theRemoveInfo) override
  {
      // _verifyRemoveObjectInstance(objectInstanceHandle);
  }

  virtual void removeObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                    rti1516ev::VariableLengthData const & tag,
                                    rti1516ev::OrderType sentOrder,
                                    rti1516ev::LogicalTime const & logicalTime,
                                    rti1516ev::OrderType receivedOrder,
                                    rti1516ev::MessageRetractionHandle theHandle,
                                    rti1516ev::SupplementalRemoveInfo theRemoveInfo) override
  {
      // _verifyRemoveObjectInstance(objectInstanceHandle);
  }

  virtual void attributesInScope(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                 rti1516ev::AttributeHandleSet const & attributes) override
  {
  }

  virtual void attributesOutOfScope(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                    rti1516ev::AttributeHandleSet const & attributes) override
  {
  }

  virtual void provideAttributeValueUpdate(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                           rti1516ev::AttributeHandleSet const & attributes,
                                           rti1516ev::VariableLengthData const & tag) override
  {
  }

  virtual void turnUpdatesOnForObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                              rti1516ev::AttributeHandleSet const & attributes) override
  {
  }

  virtual void turnUpdatesOnForObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                              rti1516ev::AttributeHandleSet const & attributes,
                                              const std::wstring& updateRateDesignator) override
  {
  }

  virtual void turnUpdatesOffForObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                               rti1516ev::AttributeHandleSet const & attributes) override
  {
  }

  virtual void confirmAttributeTransportationTypeChange(rti1516ev::ObjectInstanceHandle theObject,
                                                        rti1516ev::AttributeHandleSet theAttributes,
                                                        rti1516ev::TransportationType theTransportation) override
  {
  }

  virtual void reportAttributeTransportationType(rti1516ev::ObjectInstanceHandle theObject,
                                                 rti1516ev::AttributeHandle theAttribute,
                                                 rti1516ev::TransportationType theTransportation) override
  {
  }

  virtual void confirmInteractionTransportationTypeChange(rti1516ev::InteractionClassHandle theInteraction,
                                                          rti1516ev::TransportationType theTransportation) override
  {
  }

  virtual void reportInteractionTransportationType(rti1516ev::FederateHandle federateHandle,
                                                   rti1516ev::InteractionClassHandle theInteraction,
                                                   rti1516ev::TransportationType theTransportation) override
  {
  }

  virtual void requestAttributeOwnershipAssumption(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                                   rti1516ev::AttributeHandleSet const & offeredAttributes,
                                                   rti1516ev::VariableLengthData const & tag) override
  {
  }

  virtual void requestDivestitureConfirmation(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                              rti1516ev::AttributeHandleSet const & releasedAttributes) override
  {
  }

  virtual void attributeOwnershipAcquisitionNotification(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                                         rti1516ev::AttributeHandleSet const & securedAttributes,
                                                         rti1516ev::VariableLengthData const & tag) override
  {
  }

  virtual void attributeOwnershipUnavailable(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                             rti1516ev::AttributeHandleSet const & attributes) override
  {
  }

  virtual void requestAttributeOwnershipRelease(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                                rti1516ev::AttributeHandleSet const & candidateAttributes,
                                                rti1516ev::VariableLengthData const & tag) override
  {
  }

  virtual void confirmAttributeOwnershipAcquisitionCancellation(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                                                rti1516ev::AttributeHandleSet const & attributes) override
  {
  }

  virtual void informAttributeOwnership(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                        rti1516ev::AttributeHandle attribute,
                                        rti1516ev::FederateHandle owner) override
  {
  }

  virtual void attributeIsNotOwned(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                   rti1516ev::AttributeHandle attribute) override
  {
  }

  virtual void attributeIsOwnedByRTI(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                                     rti1516ev::AttributeHandle attribute) override
  {
  }

  virtual void timeRegulationEnabled(const rti1516ev::LogicalTime& logicalTime) override
  {
    _timeRegulationEnabled = true;
    *_grantedLogicalTime = logicalTime;
  }

  virtual void timeConstrainedEnabled(const rti1516ev::LogicalTime& logicalTime) override
  {
    _timeConstrainedEnabled = true;
    *_grantedLogicalTime = logicalTime;
  }

  virtual void timeAdvanceGrant(const rti1516ev::LogicalTime& logicalTime) override
  {
    _timeAdvancePending = false;
    *_grantedLogicalTime = logicalTime;
  }

  virtual void requestRetraction(rti1516ev::MessageRetractionHandle theHandle) override
  {
  }

  // void fail()
  // { _fail = true; }

private:
  void _replaceFileWithDataIfNeeded(std::wstring& fddFile)
  {
    if (!_useDataUrlObjectModels)
      return;
    // already a data url
    if (fddFile.compare(0, 5, L"data:") == 0)
      return;
    std::wifstream stream;
    if (fddFile.compare(0, 8, L"file:///") == 0)
      stream.open(OpenRTI::ucsToLocale(fddFile.substr(8)).c_str());
    else
      stream.open(OpenRTI::ucsToLocale(fddFile).c_str());
    if (!stream.is_open())
      return;
    fddFile = L"data:,";
    std::copy(std::istreambuf_iterator<wchar_t>(stream), std::istreambuf_iterator<wchar_t>(), std::back_inserter(fddFile));
  }

  void _replaceFilesWithDataIfNeeded(std::vector<std::wstring>& fomModules)
  {
    for (std::vector<std::wstring>::iterator i = fomModules.begin(); i != fomModules.end(); ++i)
      _replaceFileWithDataIfNeeded(*i);
  }

  // bool _verifyGrantedLogicalTime(const rti1516ev::LogicalTime& logicalTime) const
  // { return logicalTime == *_grantedLogicalTime; }

  // void _verifyReflectAttributeValues(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle,
  //                                    const rti1516ev::AttributeHandleValueMap& attributeHandleValueMap)
  // {
  //     ObjectInstanceMap::iterator i = _objectInstanceMap.find(objectInstanceHandle);
  //     if (i == _objectInstanceMap.end()) {
  //         fail();
  //         throw rti1516ev::ObjectInstanceNotKnown(objectInstanceHandle.toString());
  //     }

  //     ObjectClassAttributeHandleSetMap::iterator j = _subscribedObjectClassAttributeHandleSetMap.find(i->second._objectClassHandle);
  //     for (rti1516ev::AttributeHandleValueMap::const_iterator k = attributeHandleValueMap.begin();
  //          k != attributeHandleValueMap.end(); ++k) {
  //         if (j->second.find(k->first) != j->second.end())
  //             continue;
  //         fail();
  //         throw rti1516ev::AttributeNotSubscribed(k->first.toString());
  //     }
  // }

  // void _verifyRemoveObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle)
  // {
  //     if (_objectInstanceMap.find(objectInstanceHandle) == _objectInstanceMap.end()) {
  //         fail();
  //         throw rti1516ev::ObjectInstanceNotKnown(objectInstanceHandle.toString());
  //     }

  //     _objectInstanceMap.erase(objectInstanceHandle);
  // }

  // bool _fail;

  std::unique_ptr<rti1516ev::RTIambassador> _ambassador;

  bool _useDataUrlObjectModels;

  std::wstring _logicalTimeImplementationName;
  std::unique_ptr<rti1516ev::LogicalTimeFactory> _logicalTimeFactory;

  rti1516ev::FederateHandle _federateHandle;

  std::unique_ptr<rti1516ev::LogicalTime> _grantedLogicalTime;
  bool _timeRegulationEnabled;
  bool _timeConstrainedEnabled;
  bool _timeAdvancePending;
  // Hmm, FIXME: make an additional derived checking ambassador for the tests, keep a simple one without expensive tests
  // FIXME make this and for example the simple log below callbacks that we can attach or not as apropriate

  // // FIXME implement subscription tracking also for regions at some time
  // typedef std::map<rti1516ev::ObjectClassHandle, rti1516ev::AttributeHandleSet> ObjectClassAttributeHandleSetMap;
  // ObjectClassAttributeHandleSetMap _subscribedObjectClassAttributeHandleSetMap;

  // struct ObjectInstance {
  //   rti1516ev::ObjectClassHandle _objectClassHandle;
  // };

  // typedef std::map<rti1516ev::ObjectInstanceHandle, ObjectInstance> ObjectInstanceMap;
  // ObjectInstanceMap _objectInstanceMap;
  bool _connectionLost = false;
};

}

#endif
