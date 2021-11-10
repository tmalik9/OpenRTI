/* -*-c++-*- OpenRTI - Copyright (C) 2009-2015 Mathias Froehlich
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

#include <cstdlib>
#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include "StringUtils.h"

#include "RTI1516ETestLib.h"
#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAhandle.h"
#include "RTI/time/HLAinteger64Time.h"
#include "RTI/LogicalTime.h"

namespace OpenRTI {

enum RequestType
{
  Interaction0,
  Finished,
  Undefined
};

inline std::string to_string(RequestType requestType)
{
  switch (requestType)
  {
    case Interaction0: return "Interaction0";
    case Finished: return "Finished";
    default: return "Undefined";
  }
}

class OPENRTI_LOCAL TestRequestorAmbassador : public RTI1516ETestAmbassador
{
  public:
    TestRequestorAmbassador(const RTITest::ConstructorArgs& constructorArgs)
      : RTI1516ETestAmbassador(constructorArgs)
      , _time(0)
      , _interactionCount(0)
      , _interactionCountTotal(0)
      , _receivedInteraction(false)
      , _receivedRequestType(Undefined)
      , _subscriptionMask(0)
      , _fail(false)
      , _abortReset(false)
      , _expectAbortReset(false)
    {
      //DebugPrintf("%s\n", __FUNCTION__);
      for (auto arg : constructorArgs._argumentList)
      {
        if (arg == L"requestorAbortsReset")
        {
          _abortReset = true;
          _expectAbortReset = true;
        }
        if (arg == L"responderAbortsReset")
        {
          _expectAbortReset = true;
        }
      }
    }
    virtual ~TestRequestorAmbassador()
    {
      DebugPrintf("%s: Average roundtrip time: %fs\n", __FUNCTION__, _time / double(_interactionCountTotal));
      std::wcout << L"Average roundtrip time: " << _time / double(_interactionCountTotal) << L"s." << std::endl;
    }

    bool execJoined(uint32_t threadIndex) override
    {
      rti1516ev::RTIambassador* ambassador = getRtiAmbassador();
      DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
      if (!_timeRegulationEnabled)
      {
        ambassador->enableTimeRegulation(_lookahead);
        while (!_timeRegulationEnabled)
          ambassador->evokeCallback(0.1);
      }
      if (!_timeConstrainedEnabled)
      {
        ambassador->enableTimeConstrained();
        while (!_timeConstrainedEnabled)
          ambassador->evokeCallback(0.1);
      }
      ambassador->enableAsynchronousDelivery();

      try
      {
        _requestInteractionClassHandle = ambassador->getInteractionClassHandle(L"Request");
      }
      catch (const rti1516ev::Exception& e)
      {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      rti1516ev::ParameterHandle requestTypeHandle;
      try
      {
        requestTypeHandle = ambassador->getParameterHandle(_requestInteractionClassHandle, L"requestType");
      }
      catch (const rti1516ev::Exception& e)
      {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        _interactionClassHandle = ambassador->getInteractionClassHandle(L"InteractionClass0");
      }
      catch (const rti1516ev::Exception& e)
      {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        class0Parameter0Handle = ambassador->getParameterHandle(_interactionClassHandle, L"parameter0");
        senderParameterHandle = ambassador->getParameterHandle(_interactionClassHandle, L"sender");
      }
      catch (const rti1516ev::Exception& e)
      {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        ambassador->publishInteractionClass(_requestInteractionClassHandle);
      }
      catch (const rti1516ev::Exception& e)
      {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        ambassador->subscribeInteractionClass(_interactionClassHandle);
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      if (!waitForAllFederates(ambassador))
        return false;

      // iterate 10 times, each time resetting the federation at the end
      for (int iter=0; iter<10; iter++)
      {
        // do 10 timesteps in each iteration
        for (int step = 0; step < 10; step++)
        {
          DebugPrintf("%s[%ls]: iter=%d step=%d\n", __FUNCTION__, getFederateName().c_str(), iter, step);


          Clock clock = Clock::now();
          // send Interaction0 request
          try
          {
            rti1516ev::ParameterHandleValueMap parameterValues;
            parameterValues[requestTypeHandle] = toVariableLengthData(unsigned(RequestType::Interaction0));
            //DebugPrintf("%s[%ls]: send request(RequestType::Interaction0)\n", __FUNCTION__, getFederateName().c_str());
            ambassador->sendInteraction(_requestInteractionClassHandle, parameterValues, getFederateHandle().encode());

          }
          catch (const rti1516ev::Exception& e)
          {
            std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
            return false;
          }
          catch (...)
          {
            std::wcout << L"Unknown Exception!" << std::endl;
            return false;
          }
          _interactionCount=1;
          while (_interactionCount < getFederateList().size())
          {
            if (!WaitForInteractionReceived(ambassador, RequestType::Interaction0))
              return false;
            DebugPrintf("%s[%ls]: received interaction %d/%d\n", __FUNCTION__, getFederateName().c_str(), _interactionCount, getFederateList().size());
          }
          clock = Clock::now() - clock;
          _time += 1e-9*clock.getNanoSeconds();
          advanceTime(ambassador);
        } // for (step)

        // send 'Finished' request
        try
        {
          rti1516ev::ParameterHandleValueMap parameterValues;
          parameterValues[requestTypeHandle] = toVariableLengthData(unsigned(Finished));
          //DebugPrintf("%s: sendInteraction(%ls=_requestInteractionClassHandle)\n", __FUNCTION__, _requestInteractionClassHandle.toString().c_str());
          ambassador->sendInteraction(_requestInteractionClassHandle, parameterValues, getFederateHandle().encode());
          // do one more time step, such that the responders stay in sync
          advanceTime(ambassador);
        }
        catch (const rti1516ev::Exception& e)
        {
          std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
          return false;
        }
        catch (...)
        {
          std::wcout << L"Unknown Exception!" << std::endl;
          return false;
        }

        DebugPrintf("%s[%ls](1): request federation reset: resetInitiated=%d resetDone=%d resetAborted=%d\n", __FUNCTION__, getFederateName().c_str(), _resetInitiated, _resetDone, _resetAborted);
        _ambassador->requestFederationReset(rti1516ev::VariableLengthData());
        DebugPrintf("%s[%ls]: waiting for reset initiate: resetInitiated=%d resetDone=%d resetAborted=%d\n", __FUNCTION__, getFederateName().c_str(), _resetInitiated, _resetDone, _resetAborted);
        while(!_resetInitiated && !_resetAborted)
          _ambassador->evokeCallback(0.1);
        _ambassador->federationResetComplete(_abortReset != true, rti1516ev::VariableLengthData());
        DebugPrintf("%s[%ls]: waiting for reset done: resetInitiated=%d resetDone=%d resetAborted=%d\n", __FUNCTION__, getFederateName().c_str(), _resetInitiated, _resetDone, _resetAborted);
        while(!_resetDone && !_resetAborted)
          _ambassador->evokeCallback(0.1);
        if (_expectAbortReset && !_resetAborted)
        {
          std::wcout << L"reset not aborted as expected" << std::endl;
          return false;
        }
        _resetInitiated = false;
        _resetDone = false;
        _resetAborted = false;
      } // for (iter...)

      //DebugPrintf("%s[%ls]: done, waiting for other federates\n", __FUNCTION__, getFederateName().c_str());

      if (!waitForAllFederates(ambassador))
        return false;

      try
      {
        ambassador->unpublishInteractionClass(_requestInteractionClassHandle);
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        ambassador->unsubscribeInteractionClass(_interactionClassHandle);
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      return true;
    }

    bool WaitForInteractionReceived(rti1516ev::RTIambassador* ambassador, RequestType expectedAnswer)
    {
      try
      {
        _receivedInteraction = false;
        _receivedRequestType = Undefined;

        AbsTimeout timeout(Clock::now() + Clock::fromSeconds(10));
        while (!_receivedInteraction)
        {
          if (_resetInitiated || _resetDone || _resetAborted)
          {
            DebugPrintf("%s: unexpected federation reset!\n", __FUNCTION__);
            std::wcout << L"unexpected federation reset!" << std::endl;
            return false;
          }
          if (ambassador->evokeCallback(0.1))
            continue;
          if (timeout.isExpired())
          {
            std::wcout << L"Timeout waiting for response!" << std::endl;
            return false;
          }
        }
        if (_fail)
          return false;
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
      if (_receivedRequestType != expectedAnswer)
      {
        std::wcout << L"Received wrong Interaction class!" << std::endl;
        return false;
      }
      return true;
    }

    void advanceTime(rti1516ev::RTIambassador* ambassador)
    {
      _tick += 10;
      assert(!_inAdvancingState);
      if (_resetInitiated || _resetDone || _resetAborted) return;
      DebugPrintf("%s[%ls]: timeAdvanceRequest(%d)\n", __FUNCTION__, getFederateName().c_str(), _tick);
      ambassador->timeAdvanceRequest(rti1516ev::HLAinteger64Time(_tick));
      _inAdvancingState = true;
      while (_inAdvancingState && !_resetInitiated && !_resetDone && !_resetAborted)
      {
        //DebugPrintf("%s[%ls]: waiting ...\n", __FUNCTION__, getFederateName().c_str());
        ambassador->evokeCallback(0.1);
      }
      //DebugPrintf("%s[%ls]: _inAdvancingState=%d _resetInitiated=%d _resetDone=%d _resetAborted=%d\n", __FUNCTION__, getFederateName().c_str(),
      //  _inAdvancingState, _resetInitiated, _resetDone, _resetAborted);
    }

    void timeRegulationEnabled(rti1516ev::LogicalTime const & time) override
    {
      _timeRegulationEnabled = true;
      _grantedTime = static_cast<const rti1516ev::HLAinteger64Time&>(time);
    }

    void timeConstrainedEnabled(rti1516ev::LogicalTime const & time) override
    {
      _timeConstrainedEnabled = true;
      _grantedTime = static_cast<const rti1516ev::HLAinteger64Time&>(time);
    }

    void timeAdvanceGrant(rti1516ev::LogicalTime const & time) override
    {
      _grantedTime = static_cast<const rti1516ev::HLAinteger64Time&>(time);
      DebugPrintf("%s[%ls]: timeAdvanceGrant(%lld)\n", __FUNCTION__, getFederateName().c_str(), _grantedTime.getTime());
      _inAdvancingState = false;
    }

    void receiveInteraction(rti1516ev::InteractionClassHandle interactionClassHandle,
                            rti1516ev::ParameterHandleValueMap const & parameterValues,
                            rti1516ev::VariableLengthData const & tag,
                            rti1516ev::OrderType sentOrder,
                            rti1516ev::TransportationType theType,
                            rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
    {
      //DebugPrintf("%s[%ls]: %ls\n", __FUNCTION__, getFederateName().c_str(), _ambassador->getInteractionClassName(interactionClassHandle).c_str());
      if (getFederateHandle().encode() != tag)
      {
        DebugPrintf("%s: unknown tag\n", __FUNCTION__);
        return;
      }
      if (interactionClassHandle == _interactionClassHandle)
      {
        _receivedInteraction = true;
        _receivedRequestType = Interaction0;
        _interactionCount++;
        _interactionCountTotal++;
      }
      else
      {
        _receivedRequestType = Undefined;
      }
    }

    void federationResetInitiated(const rti1516ev::LogicalTime& logicalTime, const rti1516ev::VariableLengthData& tag) override
    {
      DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
      try
      {
        _ambassador->federationResetBegun(tag);
        _resetInitiated = true;
        _inAdvancingState = false;
      }
      catch (const rti1516ev::Exception& e)
      {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
      }
    }
    void federationResetDone(const rti1516ev::LogicalTime& logicalTime, const rti1516ev::VariableLengthData& tag) override
    {
      DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
      _resetDone = true;
      _inAdvancingState = false;
      _tick = 0;
    }
    void federationResetAborted(const rti1516ev::LogicalTime& logicalTime, const rti1516ev::VariableLengthData& tag) override
    {
      DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
      _resetAborted = true;
      _inAdvancingState = false;
    }

    RequestType getExpectedReponseType(RequestType sentResponseType) const
    {
      switch (sentResponseType)
      {
      case Interaction0:
        return Interaction0;
      case Finished:
      case Undefined:
      default:
        return Undefined;
      };
    }

    double _time;
    int64_t _tick = 0;
    bool _timeRegulationEnabled = false;
    bool _timeConstrainedEnabled = false;
    rti1516ev::HLAinteger64Interval _lookahead = rti1516ev::HLAinteger64Interval(1);
    rti1516ev::HLAinteger64Time _grantedTime = rti1516ev::HLAinteger64Time(0);
    bool _inAdvancingState = false;

    bool _resetInitiated = false;
    bool _resetDone = false;
    bool _resetAborted = false;

    unsigned _interactionCount;
    unsigned _interactionCountTotal;

    bool _receivedInteraction;
    RequestType _receivedRequestType;

    unsigned _subscriptionMask;

    bool _fail;
    bool _abortReset;
    bool _expectAbortReset;
    rti1516ev::InteractionClassHandle _requestInteractionClassHandle;
    rti1516ev::InteractionClassHandle _interactionClassHandle;

    rti1516ev::ParameterHandle class0Parameter0Handle;
    rti1516ev::ParameterHandle senderParameterHandle;
};

class OPENRTI_LOCAL TestResponderAmbassador : public RTI1516ETestAmbassador
{
  public:
    TestResponderAmbassador(const RTITest::ConstructorArgs& constructorArgs, unsigned count)
      : RTI1516ETestAmbassador(constructorArgs)
      , _receivedInteraction(false)
      , _fail(false)
      , _count(count)
      , _abortReset(false)
      , _expectAbortReset(false)
      , _requestType(Undefined)
      , _finished(false)
      , _resetInitiated(false)
      , _resetDone(false)
      , _resetAborted(false)
    {
      // if the number of federates (cmdline arg 'A') is odd, abort all resets
      for (auto arg : constructorArgs._argumentList)
      {
        if (arg == L"responderAbortsReset")
        {
          _abortReset = true;
          _expectAbortReset = true;
        }
        if (arg == L"requestorAbortsReset")
        {
          _expectAbortReset = true;
        }
      }
      //DebugPrintf("%s: count=%d abortResets=%d\n", __FUNCTION__, _count, _abortResets);
    }
    virtual ~TestResponderAmbassador()
    {
      DebugPrintf("%s\n", __FUNCTION__);
    }

    bool execJoined(uint32_t threadIndex) override
    {
      rti1516ev::RTIambassador* ambassador = getRtiAmbassador();
      DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
      if (!_timeRegulationEnabled)
      {
        ambassador->enableTimeRegulation(_lookahead);
        while (!_timeRegulationEnabled)
          ambassador->evokeCallback(0.1);
      }
      if (!_timeConstrainedEnabled)
      {
        ambassador->enableTimeConstrained();
        while (!_timeConstrainedEnabled)
          ambassador->evokeCallback(0.1);
      }
      ambassador->enableAsynchronousDelivery();

      try
      {
        _requestInteractionClassHandle = ambassador->getInteractionClassHandle(L"Request");
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        _requestTypeHandle = ambassador->getParameterHandle(_requestInteractionClassHandle, L"requestType");
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      rti1516ev::InteractionClassHandle interactionClassHandle0;
      try
      {
        interactionClassHandle0 = ambassador->getInteractionClassHandle(L"InteractionClass0");
        rti1516ev::InteractionClassHandle fqInteractionClassHandle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0");
        if (interactionClassHandle0 != fqInteractionClassHandle)
        {
          DebugPrintf("%s: HLAinteractionRoot.InteractionClass0 not found\n", __FUNCTION__);
          std::wcout << L"HLAinteractionRoot.InteractionClass0 not found" << std::endl;
          return false;
        }
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        class0Parameter0Handle = ambassador->getParameterHandle(interactionClassHandle0, L"parameter0");
        senderParameterHandle = ambassador->getParameterHandle(interactionClassHandle0, L"sender");
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        ambassador->subscribeInteractionClass(_requestInteractionClassHandle);
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        ambassador->publishInteractionClass(_requestInteractionClassHandle);
        ambassador->publishInteractionClass(interactionClassHandle0);
      }
      catch (const rti1516ev::Exception& e)
      {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
      if (!waitForAllFederates(ambassador))
      {
        DebugPrintf("%s: waitForAllFederates failed\n", __FUNCTION__);
        return false;
      }
      // loop 10 times, each time finishing the inner loop after a 'Finish' request ...
      for (int iter=0; iter<10; iter++)
      {
        //DebugPrintf("%s[%ls]: iter=%d\n", __FUNCTION__, getFederateName().c_str(), iter);
        _finished = false;
        // loop until we receive an interaction containing the 'finish' request
        while (!_finished)
        {
          if (!WaitForInteractionReceived(ambassador))
            return false;
          if (_receivedInteraction)
          {
            _receivedInteraction = false;
            try
            {

              rti1516ev::InteractionClassHandle interactionClassHandle;
              rti1516ev::ParameterHandleValueMap parameterValues;

              switch (_requestType)
              {
              case Finished:
                _finished = true;
                break;
              case Interaction0:
                parameterValues[class0Parameter0Handle] = rti1516ev::HLAinteger32LE(_count).encode();
                //DebugPrintf("%s: sendInteraction(%ls)\n", __FUNCTION__, interactionClassHandle.toString().c_str());
                ambassador->sendInteraction(interactionClassHandle0, parameterValues, _requestFederate);
                break;
              case Undefined:
                std::wcout << L"Unexpected Request type!" << std::endl;
                return false;
              }

              // advance time once, after each sent response
              advanceTime(ambassador);
            }
            catch (const rti1516ev::Exception& e)
            {
              DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
              std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
              return false;
            }
            catch (...)
            {
              std::wcout << L"Unknown Exception!" << std::endl;
              return false;
            }
          }
        }
        DebugPrintf("%s[%ls]: waiting for reset initiate: resetInitiated=%d resetDone=%d resetAborted=%d\n", __FUNCTION__, getFederateName().c_str(), _resetInitiated, _resetDone, _resetAborted);
        while (!_resetInitiated && !_resetAborted)
          _ambassador->evokeCallback(0.1);
        _ambassador->federationResetComplete(_abortReset != true, rti1516ev::VariableLengthData());
        DebugPrintf("%s[%ls]: waiting for reset done: resetInitiated=%d resetDone=%d resetAborted=%d\n", __FUNCTION__, getFederateName().c_str(), _resetInitiated, _resetDone, _resetAborted);
        while (!_resetDone && !_resetAborted)
          _ambassador->evokeCallback(0.1);
        if (_expectAbortReset && !_resetAborted)
        {
          std::wcout << L"reset not aborted as expected" << std::endl;
          return false;
        }
        _resetInitiated = false;
        _resetAborted = false;
        _resetDone = false;
      }

      //DebugPrintf("%s[%ls]: done, waiting for other federates\n", __FUNCTION__, getFederateName().c_str());

      if (!waitForAllFederates(ambassador))
        return false;

      try
      {
        ambassador->unpublishInteractionClass(_requestInteractionClassHandle);
        ambassador->unpublishInteractionClass(interactionClassHandle0);
      }
      catch (const rti1516ev::Exception& e)
      {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      try
      {
        ambassador->unsubscribeInteractionClass(_requestInteractionClassHandle);
      }
      catch (const rti1516ev::Exception& e)
      {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      return true;
    }

    bool WaitForInteractionReceived(rti1516ev::RTIambassador* ambassador)
    {
      try {
        AbsTimeout timeout(Clock::now() + Clock::fromSeconds(10));
        while (!_receivedInteraction) {
          if (_resetInitiated || _resetDone || _resetAborted)
          {
            DebugPrintf("%s: unexpected federation reset!\n", __FUNCTION__);
            std::wcout << L"unexpected federation reset!" << std::endl;
            return false;
          }
          if (ambassador->evokeCallback(0.1))
            continue;
          if (timeout.isExpired()) {
            DebugPrintf("%s: Timeout waiting for request!\n", __FUNCTION__);
            std::wcout << L"Timeout waiting for request!" << std::endl;
            return false;
          }
        }
        if (_fail)
          return false;

      } catch (const rti1516ev::Exception &e) {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__,
                    e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\""
                   << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
      return true;
    }

    void advanceTime(rti1516ev::RTIambassador* ambassador)
    {
      _tick += 10;
      assert(!_inAdvancingState);
      if (_resetInitiated || _resetDone || _resetAborted) return;
      DebugPrintf("%s[%ls]: timeAdvanceRequest(%d)\n", __FUNCTION__, getFederateName().c_str(), _tick);
      ambassador->timeAdvanceRequest(rti1516ev::HLAinteger64Time(_tick));
      _inAdvancingState = true;
      while (_inAdvancingState && !_resetInitiated && !_resetDone && !_resetAborted)
      {
        //DebugPrintf("%s[%ls]: waiting ...\n", __FUNCTION__, getFederateName().c_str());
        ambassador->evokeCallback(0.1);
      }
      //DebugPrintf("%s[%ls]: _inAdvancingState=%d _resetInitiated=%d _resetDone=%d _resetAborted=%d\n", __FUNCTION__, getFederateName().c_str(),
      //  _inAdvancingState, _resetInitiated, _resetDone, _resetAborted);
    }

    void receiveInteraction(rti1516ev::InteractionClassHandle interactionClassHandle,
                            rti1516ev::ParameterHandleValueMap const & parameterValues,
                            rti1516ev::VariableLengthData const & tag,
                            rti1516ev::OrderType sentOrder,
                            rti1516ev::TransportationType theType,
                            rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
    {
      //DebugPrintf("%s[%ls]: %ls\n", __FUNCTION__, getFederateName().c_str(), _ambassador->getInteractionClassName(interactionClassHandle).c_str());
      if (interactionClassHandle != _requestInteractionClassHandle)
      {
        std::wcout << L"Received interaction class that was not subscribed!" << std::endl;
        _fail = true;
      }

      // Retrieve what we should respond to
      rti1516ev::ParameterHandleValueMap::const_iterator i = parameterValues.find(_requestTypeHandle);
      if (i != parameterValues.end())
      {
        rti1516ev::HLAinteger32LE value;
        value.decode(i->second);
        _requestType = RequestType(value.get());
      }
      _requestFederate = tag;
      _receivedInteraction = true;
    }

    void timeRegulationEnabled(rti1516ev::LogicalTime const & time) override
    {
      _timeRegulationEnabled = true;
      _grantedTime = static_cast<const rti1516ev::HLAinteger64Time&>(time);
    }

    void timeConstrainedEnabled(rti1516ev::LogicalTime const & time) override
    {
      _timeConstrainedEnabled = true;
      _grantedTime = static_cast<const rti1516ev::HLAinteger64Time&>(time);
    }

    void timeAdvanceGrant(rti1516ev::LogicalTime const & time) override
    {
      _grantedTime = static_cast<const rti1516ev::HLAinteger64Time&>(time);
      DebugPrintf("%s[%ls]: timeAdvanceGrant(%lld)\n", __FUNCTION__, getFederateName().c_str(), _grantedTime.getTime());
      _inAdvancingState = false;
    }

    void federationResetInitiated(const rti1516ev::LogicalTime& logicalTime, const rti1516ev::VariableLengthData& tag) override
    {
      DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
      try
      {
        _ambassador->federationResetBegun(tag);
        _resetInitiated = true;
        _inAdvancingState = false;
      }
      catch (const rti1516ev::Exception& e)
      {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      }
      catch (...)
      {
        std::wcout << L"Unknown Exception!" << std::endl;
      }
    }
    void federationResetDone(const rti1516ev::LogicalTime& logicalTime, const rti1516ev::VariableLengthData& tag) override
    {
      DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
      _resetDone = true;
      _inAdvancingState = false;
      _tick = 0;
    }
    void federationResetAborted(const rti1516ev::LogicalTime& logicalTime, const rti1516ev::VariableLengthData& tag) override
    {
      DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
      _resetAborted = true;
      _inAdvancingState = false;
    }

    bool _timeRegulationEnabled = false;
    bool _timeConstrainedEnabled = false;
    rti1516ev::HLAinteger64Interval _lookahead = rti1516ev::HLAinteger64Interval(1);
    rti1516ev::HLAinteger64Time _grantedTime = rti1516ev::HLAinteger64Time(0);
    bool _inAdvancingState = false;
    int64_t _tick = 0;

    bool _receivedInteraction;
    bool _fail;
    unsigned _count;
    bool _abortReset;
    bool _expectAbortReset;
    rti1516ev::InteractionClassHandle _requestInteractionClassHandle;
    rti1516ev::ParameterHandle _requestTypeHandle;
    RequestType _requestType;
    rti1516ev::VariableLengthData _requestFederate;
    bool _finished = false;
    bool _resetInitiated = false;
    bool _resetDone = false;
    bool _resetAborted = false;

    rti1516ev::ParameterHandle class0Parameter0Handle;
    rti1516ev::ParameterHandle senderParameterHandle;
};

class OPENRTI_LOCAL Test : public RTITest
{
  public:
    Test(int argc, const char* const argv[]) :
      RTITest(argc, argv, false),
      _count(0)
    { }
    SharedPtr<Ambassador> createAmbassador(const ConstructorArgs& constructorArgs) override
    {
      // There is one ambassador who answers the ping requests
      if (_count++ == 0)
        return MakeShared<TestRequestorAmbassador>(constructorArgs);
      else
        return MakeShared<TestResponderAmbassador>(constructorArgs, _count);
    }
  private:
    unsigned _count;
};

}

int
main(int argc, char* argv[])
{
  OpenRTI::Test test(argc, argv);
  return test.exec();
}
