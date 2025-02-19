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

namespace OpenRTI {

enum RequestType {
  Interaction0,
  Interaction1,
  Interaction2,
  Interaction3,
  Finished,
  Undefined
};

inline std::string to_string(RequestType requestType)
{
  switch (requestType)
  {
    case Interaction0: return "Interaction0";
    case Interaction1: return "Interaction1";
    case Interaction2: return "Interaction2";
    case Interaction3: return "Interaction3";
    case Finished: return "Finished";
    default: return "Undefined";
  }
}


class OPENRTI_LOCAL TestResponderAmbassador : public RTI1516ETestAmbassador {
public:
  TestResponderAmbassador(const RTITest::ConstructorArgs& constructorArgs) :
    RTI1516ETestAmbassador(constructorArgs),
    _fail(false)
  {
    DebugPrintf("%s\n", __FUNCTION__);
  }
  virtual ~TestResponderAmbassador()
  {
    DebugPrintf("%s\n", __FUNCTION__);
  }

  bool execJoined(uint32_t threadIndex) override
  {
    rti1516ev::RTIambassador* ambassador = getRtiAmbassador();
    DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
    try {
      _requestInteractionClassHandle = ambassador->getInteractionClassHandle(L"Request");
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      _requestTypeHandle = ambassador->getParameterHandle(_requestInteractionClassHandle, L"requestType");
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    rti1516ev::InteractionClassHandle interactionClassHandle0;
    rti1516ev::InteractionClassHandle interactionClassHandle1;
    rti1516ev::InteractionClassHandle interactionClassHandle2;
    rti1516ev::InteractionClassHandle interactionClassHandle3;
    try {
      interactionClassHandle0 = ambassador->getInteractionClassHandle(L"InteractionClass0");
      rti1516ev::InteractionClassHandle fqInteractionClassHandle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0");
      if (interactionClassHandle0 != fqInteractionClassHandle) {
        DebugPrintf("%s: Full qualified interaction class lookup failed\n", __FUNCTION__);
        std::wcout << L"Full qualified interaction class lookup failed" << std::endl;
        return false;
      }
      interactionClassHandle1 = ambassador->getInteractionClassHandle(L"InteractionClass1");
      fqInteractionClassHandle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0.InteractionClass1");
      if (interactionClassHandle1 != fqInteractionClassHandle) {
        DebugPrintf("%s: Full qualified interaction class lookup failed\n", __FUNCTION__);
        std::wcout << L"Full qualified interaction class lookup failed" << std::endl;
        return false;
      }
      interactionClassHandle2 = ambassador->getInteractionClassHandle(L"InteractionClass2");
      fqInteractionClassHandle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0.InteractionClass1.InteractionClass2");
      if (interactionClassHandle2 != fqInteractionClassHandle) {
        DebugPrintf("%s: Full qualified interaction class lookup failed\n", __FUNCTION__);
        std::wcout << L"Full qualified interaction class lookup failed" << std::endl;
        return false;
      }
      interactionClassHandle3 = ambassador->getInteractionClassHandle(L"InteractionClass3");
      fqInteractionClassHandle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0.InteractionClass1.InteractionClass3");
      if (interactionClassHandle3 != fqInteractionClassHandle) {
        DebugPrintf("%s: Full qualified interaction class lookup failed\n", __FUNCTION__);
        std::wcout << L"Full qualified interaction class lookup failed" << std::endl;
        return false;
      }
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    rti1516ev::ParameterHandle class0Parameter0Handle;
    rti1516ev::ParameterHandle class1Parameter0Handle;
    rti1516ev::ParameterHandle class1Parameter1Handle;
    rti1516ev::ParameterHandle class2Parameter0Handle;
    rti1516ev::ParameterHandle class2Parameter1Handle;
    rti1516ev::ParameterHandle class2Parameter2Handle;
    rti1516ev::ParameterHandle class3Parameter0Handle;
    rti1516ev::ParameterHandle class3Parameter1Handle;
    rti1516ev::ParameterHandle class3Parameter3Handle;
    try {
      class0Parameter0Handle = ambassador->getParameterHandle(interactionClassHandle0, L"parameter0");
      class1Parameter0Handle = ambassador->getParameterHandle(interactionClassHandle1, L"parameter0");
      class1Parameter1Handle = ambassador->getParameterHandle(interactionClassHandle1, L"parameter1");
      class2Parameter0Handle = ambassador->getParameterHandle(interactionClassHandle2, L"parameter0");
      class2Parameter1Handle = ambassador->getParameterHandle(interactionClassHandle2, L"parameter1");
      class2Parameter2Handle = ambassador->getParameterHandle(interactionClassHandle2, L"parameter2");
      class3Parameter0Handle = ambassador->getParameterHandle(interactionClassHandle3, L"parameter0");
      class3Parameter1Handle = ambassador->getParameterHandle(interactionClassHandle3, L"parameter1");
      class3Parameter3Handle = ambassador->getParameterHandle(interactionClassHandle3, L"parameter3");
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      DebugPrintf("%s: subscribeInteractionClass(%ls)\n", __FUNCTION__, ambassador->getInteractionClassName(_requestInteractionClassHandle).c_str());
      ambassador->subscribeInteractionClass(_requestInteractionClassHandle);
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      ambassador->publishInteractionClass(_requestInteractionClassHandle);
      ambassador->publishInteractionClass(interactionClassHandle0);
      ambassador->publishInteractionClass(interactionClassHandle1);
      ambassador->publishInteractionClass(interactionClassHandle2);
      ambassador->publishInteractionClass(interactionClassHandle3);
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
    _finishedCount = 1;
    while (_finishedCount < getFederateList().size()) {
      DebugPrintf("%s: _finishedCount=%d  federates=%d\n", __FUNCTION__, _finishedCount, getFederateList().size());

      try {
        _receivedInteraction = false;
        Clock timeout = Clock::now() + Clock::fromSeconds(10);
        while (!_receivedInteraction) {
          if (ambassador->evokeCallback(10.0))
            continue;
          if (timeout < Clock::now()) {
            DebugPrintf("%s: Timeout waiting for request!\n", __FUNCTION__);
            std::wcout << L"Timeout waiting for request!" << std::endl;
            return false;
          }
        }
        if (_fail)
          return false;

      } catch (const rti1516ev::Exception& e) {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
      if (_requestType == Finished)
        continue;

      try {

        rti1516ev::InteractionClassHandle interactionClassHandle;
        rti1516ev::ParameterHandleValueMap parameterValues;

        switch (_requestType) {
        case Finished:
        case Undefined:
          std::wcout << L"Unexpected Request type!" << std::endl;
          return false;
        case Interaction0:
          interactionClassHandle = interactionClassHandle0;
          parameterValues[class0Parameter0Handle] = toVariableLengthData("parameter0");
          break;
        case Interaction1:
          interactionClassHandle = interactionClassHandle1;
          parameterValues[class1Parameter0Handle] = toVariableLengthData("parameter0");
          parameterValues[class1Parameter1Handle] = toVariableLengthData("parameter1");
          break;
        case Interaction2:
          interactionClassHandle = interactionClassHandle2;
          parameterValues[class2Parameter0Handle] = toVariableLengthData("parameter0");
          parameterValues[class2Parameter1Handle] = toVariableLengthData("parameter1");
          parameterValues[class2Parameter2Handle] = toVariableLengthData("parameter2");
          break;
        case Interaction3:
          interactionClassHandle = interactionClassHandle3;
          parameterValues[class3Parameter0Handle] = toVariableLengthData("parameter0");
          parameterValues[class3Parameter1Handle] = toVariableLengthData("parameter1");
          parameterValues[class3Parameter3Handle] = toVariableLengthData("parameter3");
          break;
        }

        DebugPrintf("%s: sendInteraction(%ls)\n", __FUNCTION__, ambassador->getInteractionClassName(interactionClassHandle).c_str());
        ambassador->sendInteraction(interactionClassHandle, parameterValues, _requestFederate);

        // Also send when we reponded, so that the requestor knows when we have sent something
        parameterValues.clear();
        parameterValues[_requestTypeHandle] = toVariableLengthData(unsigned(_requestType));
        DebugPrintf("%s: sendInteraction(%ls)\n", __FUNCTION__, ambassador->getInteractionClassName(_requestInteractionClassHandle).c_str());
        ambassador->sendInteraction(_requestInteractionClassHandle, parameterValues, _requestFederate);

      } catch (const rti1516ev::Exception& e) {
        DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
    }

    if (!waitForAllFederates(ambassador))
      return false;

    try {
      ambassador->unpublishInteractionClass(_requestInteractionClassHandle);
      ambassador->unpublishInteractionClass(interactionClassHandle0);
      ambassador->unpublishInteractionClass(interactionClassHandle1);
      ambassador->unpublishInteractionClass(interactionClassHandle2);
    } catch (const rti1516ev::Exception& e) {
      DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      DebugPrintf("%s: unsubscribeInteractionClass(%ls)\n", __FUNCTION__, ambassador->getInteractionClassName(_requestInteractionClassHandle).c_str());
      ambassador->unsubscribeInteractionClass(_requestInteractionClassHandle);
    } catch (const rti1516ev::Exception& e) {
      DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    return true;
  }

  void receiveInteraction(rti1516ev::InteractionClassHandle interactionClassHandle,
                          rti1516ev::ParameterHandleValueMap const & parameterValues,
                          rti1516ev::VariableLengthData const & tag,
                          rti1516ev::OrderType sentOrder,
                          rti1516ev::TransportationType theType,
                          rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
  {
    DebugPrintf("%s\n", __FUNCTION__);
    if (interactionClassHandle != _requestInteractionClassHandle) {
      std::wcout << L"Received interaction class that was not subscribed!" << std::endl;
      _fail = true;
      _receivedInteraction = true;
    }

    // Retrieve what we should respond to
    rti1516ev::ParameterHandleValueMap::const_iterator i = parameterValues.find(_requestTypeHandle);
    if (i != parameterValues.end())
      _requestType = RequestType(toUnsigned(i->second));
    if (_requestType == Finished)
      ++_finishedCount;
    _requestFederate = tag;
    _receivedInteraction = true;
  }

  bool _receivedInteraction;
  bool _fail;
  rti1516ev::InteractionClassHandle _requestInteractionClassHandle;
  rti1516ev::ParameterHandle _requestTypeHandle;
  RequestType _requestType;
  rti1516ev::VariableLengthData _requestFederate;
  unsigned _finishedCount;
};

class OPENRTI_LOCAL TestRequestorAmbassador : public RTI1516ETestAmbassador {
public:
  TestRequestorAmbassador(const RTITest::ConstructorArgs& constructorArgs)
    : RTI1516ETestAmbassador(constructorArgs)
    , _time(0)
    , _interactionCount(0)
    , _receivedInteraction(false)
    , _receivedRequestType(Undefined)
    , _subscriptionMask(0)
    , _fail(false)
  {
    DebugPrintf("%s\n", __FUNCTION__);
  }
  virtual ~TestRequestorAmbassador()
  {
    DebugPrintf("%s: Average roundtrip time: %fs\n", __FUNCTION__, _time / double(_interactionCount));
    std::wcout << L"Average roundtrip time: " << _time / double(_interactionCount) << L"s." << std::endl;
  }

  bool execJoined(uint32_t threadIndex) override
  {
    rti1516ev::RTIambassador* ambassador = getRtiAmbassador();
    DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
    try {
      _requestInteractionClassHandle = ambassador->getInteractionClassHandle(L"Request");
    } catch (const rti1516ev::Exception& e) {
      DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    rti1516ev::ParameterHandle requestTypeHandle;
    try {
      requestTypeHandle = ambassador->getParameterHandle(_requestInteractionClassHandle, L"requestType");
    } catch (const rti1516ev::Exception& e) {
      DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      _interactionClassHandles[0] = ambassador->getInteractionClassHandle(L"InteractionClass0");
      _interactionClassHandles[1] = ambassador->getInteractionClassHandle(L"InteractionClass1");
      _interactionClassHandles[2] = ambassador->getInteractionClassHandle(L"InteractionClass2");
      _interactionClassHandles[3] = ambassador->getInteractionClassHandle(L"InteractionClass3");
    } catch (const rti1516ev::Exception& e) {
      DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      class0Parameter0Handle = ambassador->getParameterHandle(_interactionClassHandles[0], L"parameter0");
      class1Parameter0Handle = ambassador->getParameterHandle(_interactionClassHandles[1], L"parameter0");
      class1Parameter1Handle = ambassador->getParameterHandle(_interactionClassHandles[1], L"parameter1");
      class2Parameter0Handle = ambassador->getParameterHandle(_interactionClassHandles[2], L"parameter0");
      class2Parameter1Handle = ambassador->getParameterHandle(_interactionClassHandles[2], L"parameter1");
      class2Parameter2Handle = ambassador->getParameterHandle(_interactionClassHandles[2], L"parameter2");
      class3Parameter0Handle = ambassador->getParameterHandle(_interactionClassHandles[3], L"parameter0");
      class3Parameter1Handle = ambassador->getParameterHandle(_interactionClassHandles[3], L"parameter1");
      class3Parameter3Handle = ambassador->getParameterHandle(_interactionClassHandles[3], L"parameter3");
    } catch (const rti1516ev::Exception& e) {
      DebugPrintf("%s: rti1516ev::Exception: \"%ls\"\n", __FUNCTION__, e.what().c_str());
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      ambassador->publishInteractionClass(_requestInteractionClassHandle);
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      DebugPrintf("%s: subscribeInteractionClass(%ls)\n", __FUNCTION__, ambassador->getInteractionClassName(_requestInteractionClassHandle).c_str());
      ambassador->subscribeInteractionClass(_requestInteractionClassHandle);
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    if (!waitForAllFederates(ambassador))
      return false;

    for (unsigned i = 0; i < 0xf; ++i) {
      try {
        // Change subscriptions due to the subscription mask
        for (unsigned j = 0; j < 4; ++j) {
          unsigned mask = (1u << j);
          if ((_subscriptionMask ^ i) & mask) {
            if (i & mask) {
              DebugPrintf("%s: subscribeInteractionClass(%ls)\n", __FUNCTION__, ambassador->getInteractionClassName(_interactionClassHandles[j]).c_str());
              ambassador->subscribeInteractionClass(_interactionClassHandles[j]);
            } else {
              DebugPrintf("%s: unsubscribeInteractionClass(%ls)\n", __FUNCTION__, ambassador->getInteractionClassName(_interactionClassHandles[j]).c_str());
              ambassador->unsubscribeInteractionClass(_interactionClassHandles[j]);
            }
          }
        }
        _subscriptionMask = i;
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      for (unsigned j = 0; j < 4; ++j) {
        for (unsigned k = 0; k < 2; ++k) {

          Clock clock = Clock::now();

          RequestType requestedType = RequestType(j);

          try {
            rti1516ev::ParameterHandleValueMap parameterValues;
            parameterValues[requestTypeHandle] = toVariableLengthData(unsigned(requestedType));
            DebugPrintf("%s: sendInteraction(%ls)\n", __FUNCTION__, ambassador->getInteractionClassName(_requestInteractionClassHandle).c_str());
            ambassador->sendInteraction(_requestInteractionClassHandle, parameterValues, getFederateHandle().encode());

          } catch (const rti1516ev::Exception& e) {
            std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
            return false;
          } catch (...) {
            std::wcout << L"Unknown Exception!" << std::endl;
            return false;
          }

          try {
            _receivedInteraction = false;
            _receivedRequestType = Undefined;

            Clock timeout = Clock::now() + Clock::fromSeconds(10);
            while (!_receivedInteraction) {
              if (ambassador->evokeCallback(10.0))
                continue;
              if (timeout < Clock::now()) {
                std::wcout << L"Timeout waiting for response!" << std::endl;
                return false;
              }
            }
            if (_fail)
              return false;
          } catch (const rti1516ev::Exception& e) {
            std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
            return false;
          } catch (...) {
            std::wcout << L"Unknown Exception!" << std::endl;
            return false;
          }

          if (_receivedRequestType != getExpectedReponseType(requestedType)) {
            std::wcout << L"Received wrong Interaction class!" << std::endl;
            return false;
          }

          clock = Clock::now() - clock;
          _time += 1e-9*clock.getNanoSeconds();
          ++_interactionCount;
        }
      }
    }

    try {
      rti1516ev::ParameterHandleValueMap parameterValues;
      parameterValues[requestTypeHandle] = toVariableLengthData(unsigned(Finished));
      DebugPrintf("%s: sendInteraction(%ls=_requestInteractionClassHandle)\n", __FUNCTION__, _requestInteractionClassHandle.toString().c_str());
      ambassador->sendInteraction(_requestInteractionClassHandle, parameterValues, getFederateHandle().encode());

    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    if (!waitForAllFederates(ambassador))
      return false;

    try {
      ambassador->unpublishInteractionClass(_requestInteractionClassHandle);
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      for (unsigned i = 0; i < 4; ++i) {
        if (_subscriptionMask & (1u << i))
        {
          DebugPrintf("%s: unsubscribeInteractionClass(%ls)\n", __FUNCTION__, ambassador->getInteractionClassName(_interactionClassHandles[i]).c_str());
          ambassador->unsubscribeInteractionClass(_interactionClassHandles[i]);
        }
      }
      _subscriptionMask = 0;
    } catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    return true;
  }

  void receiveInteraction(rti1516ev::InteractionClassHandle interactionClassHandle,
                          rti1516ev::ParameterHandleValueMap const & parameterValues,
                          rti1516ev::VariableLengthData const & tag,
                          rti1516ev::OrderType sentOrder,
                          rti1516ev::TransportationType theType,
                          rti1516ev::SupplementalReceiveInfo theReceiveInfo) override
  {
    DebugPrintf("%s\n", __FUNCTION__);
    for (unsigned i = 0; i < 4; ++i) {
      if (interactionClassHandle == _interactionClassHandles[i] && !(_subscriptionMask & (1u << i))) {
        std::wcout << L"Received interaction class that was not subscribed!" << std::endl;
        _fail = true;
        _receivedInteraction = true;
      }
    }

    if (getFederateHandle().encode() != tag)
      return;

    if (interactionClassHandle == _interactionClassHandles[0])
      _receivedRequestType = Interaction0;
    else if (interactionClassHandle == _interactionClassHandles[1])
      _receivedRequestType = Interaction1;
    else if (interactionClassHandle == _interactionClassHandles[2])
      _receivedRequestType = Interaction2;
    else if (interactionClassHandle == _interactionClassHandles[3])
      _receivedRequestType = Interaction3;
    else if (interactionClassHandle == _requestInteractionClassHandle)
      _receivedInteraction = true;
    else
      _receivedRequestType = Undefined;
  }

  RequestType getExpectedReponseType(RequestType sentResponseType) const
  {
    switch (sentResponseType) {
    case Interaction0:
      if (_subscriptionMask & (1u << 0))
        return Interaction0;
      else
        return Undefined;
    case Interaction1:
      if (_subscriptionMask & (1u << 1))
        return Interaction1;
      else if (_subscriptionMask & (1u << 0))
        return Interaction0;
      else
        return Undefined;
    case Interaction2:
      if (_subscriptionMask & (1u << 2))
        return Interaction2;
      else if (_subscriptionMask & (1u << 1))
        return Interaction1;
      else if (_subscriptionMask & (1u << 0))
        return Interaction0;
      else
        return Undefined;
    case Interaction3:
      if (_subscriptionMask & (1u << 3))
        return Interaction3;
      else if (_subscriptionMask & (1u << 1))
        return Interaction1;
      else if (_subscriptionMask & (1u << 0))
        return Interaction0;
      else
        return Undefined;
    case Finished:
    case Undefined:
    default:
      return Undefined;
    };
  }

  double _time;
  unsigned _interactionCount;

  bool _receivedInteraction;
  RequestType _receivedRequestType;

  unsigned _subscriptionMask;

  bool _fail;

  rti1516ev::InteractionClassHandle _requestInteractionClassHandle;
  rti1516ev::InteractionClassHandle _interactionClassHandles[4];

  rti1516ev::ParameterHandle class0Parameter0Handle;
  rti1516ev::ParameterHandle class1Parameter0Handle;
  rti1516ev::ParameterHandle class1Parameter1Handle;
  rti1516ev::ParameterHandle class2Parameter0Handle;
  rti1516ev::ParameterHandle class2Parameter1Handle;
  rti1516ev::ParameterHandle class2Parameter2Handle;
  rti1516ev::ParameterHandle class3Parameter0Handle;
  rti1516ev::ParameterHandle class3Parameter1Handle;
  rti1516ev::ParameterHandle class3Parameter3Handle;
};

class OPENRTI_LOCAL Test : public RTITest {
public:
  Test(int argc, const char* const argv[]) :
    RTITest(argc, argv, false),
    _count(0)
  { }
  SharedPtr<Ambassador> createAmbassador(const ConstructorArgs& constructorArgs) override
  {
    // There is one ambassador who answers the ping requests
    if (_count++ == 0)
      return MakeShared<TestResponderAmbassador>(constructorArgs);
    else
      return MakeShared<TestRequestorAmbassador>(constructorArgs);
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
