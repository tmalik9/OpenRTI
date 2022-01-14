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

#include "LogStream.h"
#include "Options.h"
#include "StringUtils.h"

#include "RTI1516ETestLib.h"

namespace OpenRTI {

class OPENRTI_LOCAL TestAmbassador : public RTI1516ETestAmbassador {
public:
  TestAmbassador(const RTITest::ConstructorArgs& constructorArgs) :
    RTI1516ETestAmbassador(constructorArgs),
    _fail(false)
  { }
  virtual ~TestAmbassador()
  { }

  void requestAttributeOwnershipRelease(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
      rti1516ev::AttributeHandleSet const & candidateAttributes,
      rti1516ev::VariableLengthData const & tag) override
  {
    DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
    auto ownedIter = _ownedObjectInstanceHandles.find(objectInstanceHandle);
    if (ownedIter != _ownedObjectInstanceHandles.end())
    {
      _ownedObjectInstanceHandles.erase(ownedIter);
    }
    else
    {
      DebugPrintf("%s[%ls]: not owned: received objectInstance=%ls\n", __FUNCTION__, getFederateName().c_str(),
        _ambassador->getObjectInstanceName(objectInstanceHandle).c_str()
        );
      _fail = true;
    }
    auto objectClass = _ambassador->getKnownObjectClassHandle(objectInstanceHandle);
    for (auto attributeHandle : candidateAttributes)
    {
      DebugPrintf("%s[%ls]: divesting objectInstance=%ls attribute=%ls (%ls)\n", __FUNCTION__, getFederateName().c_str(),
        _ambassador->getObjectInstanceName(objectInstanceHandle).c_str(),
        _ambassador->getAttributeName(objectClass, attributeHandle).c_str(), attributeHandle.toString().c_str()
        );
    }
    try {
      _ambassador->unconditionalAttributeOwnershipDivestiture(objectInstanceHandle, candidateAttributes);
    }
    catch (const rti1516ev::Exception& e)
    {
      std::cout << __FUNCTION__ << L": ";
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
    }
    auto foreignIter = _foreignObjectInstanceHandles.find(objectInstanceHandle);
    if (foreignIter == _foreignObjectInstanceHandles.end())
    {
      _foreignObjectInstanceHandles.insert(objectInstanceHandle);
    }
    else
    {
      DebugPrintf("%s[%ls]: already registered as foreign: received objectInstance=%ls\n", __FUNCTION__, getFederateName().c_str(),
        _ambassador->getObjectInstanceName(objectInstanceHandle).c_str()
        );
      _fail = true;
    }
  }

  void attributeOwnershipAcquisitionNotification(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
        rti1516ev::AttributeHandleSet const & securedAttributes,
        rti1516ev::VariableLengthData const & tag) override
  {
    DebugPrintf("%s[%ls]\n", __FUNCTION__, getFederateName().c_str());
    auto objectClass = _ambassador->getKnownObjectClassHandle(objectInstanceHandle);
    for (auto attributeHandle : securedAttributes)
    {
      DebugPrintf("%s[%ls]: received objectInstance=%ls attribute=%ls (%ls)\n", __FUNCTION__, getFederateName().c_str(),
        _ambassador->getObjectInstanceName(objectInstanceHandle).c_str(),
        _ambassador->getAttributeName(objectClass, attributeHandle).c_str(), attributeHandle.toString().c_str()
        );
    }
    auto ownedIter = _ownedObjectInstanceHandles.find(objectInstanceHandle);
    if (ownedIter == _ownedObjectInstanceHandles.end())
    {
      _ownedObjectInstanceHandles.insert(objectInstanceHandle);
    }
    else
    {
      DebugPrintf("%s[%ls]: already owned: received objectInstance=%ls\n", __FUNCTION__, getFederateName().c_str(),
        _ambassador->getObjectInstanceName(objectInstanceHandle).c_str()
        );
      _fail = true;
    }
    auto foreignIter = _foreignObjectInstanceHandles.find(objectInstanceHandle);
    if (foreignIter != _foreignObjectInstanceHandles.end())
    {
      _foreignObjectInstanceHandles.erase(foreignIter);
    }
    else
    {
      DebugPrintf("%s[%ls]: not registered as foreign: received objectInstance=%ls\n", __FUNCTION__, getFederateName().c_str(),
        _ambassador->getObjectInstanceName(objectInstanceHandle).c_str()
        );
      _fail = true;
    }
  }
  
  bool execJoined(uint32_t threadIndex) override
  {
    DebugPrintf("%s[%ls] url=%ls\n", __FUNCTION__, getFederateName().c_str(), getConnectUrl().c_str());
    rti1516ev::RTIambassador* ambassador = getRtiAmbassador();
    try {
      rti1516ev::ObjectClassHandle objectClassHandle = ambassador->getObjectClassHandle(L"ObjectClass1");
      rti1516ev::ObjectClassHandle fqObjectClassHandle = ambassador->getObjectClassHandle(L"HLAobjectRoot.ObjectClass0.ObjectClass1");
      if (objectClassHandle != fqObjectClassHandle) {
        std::wcout << L"Full qualified object class lookup failed" << std::endl;
        return false;
      }
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador->getAttributeHandle(objectClassHandle, L"attribute0"));
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador->getAttributeHandle(objectClassHandle, L"attribute1"));
      _privilegeToDeleteObject[objectClassHandle] = ambassador->getAttributeHandle(objectClassHandle, L"HLAprivilegeToDeleteObject");
    } catch (const rti1516ev::Exception& e) {
      std::cout << __FUNCTION__ << L": ";
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    for (auto& attributesByObjectClass : _objectClassAttributeHandleSet) {
      auto objectClass = attributesByObjectClass.first;
      auto attributes = attributesByObjectClass.second;
      attributes.insert(_privilegeToDeleteObject[attributesByObjectClass.first]);
      try {
        ambassador->publishObjectClassAttributes(objectClass, attributes);
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
      try {
        ambassador->subscribeObjectClassAttributes(objectClass, attributes);
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
    }

    for (auto& attributesByObjectClass : _objectClassAttributeHandleSet) {
      rti1516ev::ObjectInstanceHandle objectInstanceHandle = rti1516ev::ObjectInstanceHandle();
      auto objectClass = attributesByObjectClass.first;
      auto attributes = attributesByObjectClass.second;
      _expectedAttributeHandles = attributesByObjectClass.second;
      if (threadIndex == 0)
      {
        try {
          objectInstanceHandle = ambassador->registerObjectInstance(objectClass);
          _ownedObjectInstanceHandles.insert(objectInstanceHandle);
        }
        catch (const rti1516ev::Exception& e) {
          std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
          return false;
        }
        catch (...) {
          std::wcout << L"Unknown Exception!" << std::endl;
          return false;
        }
      }
      else {
        try {
          AbsTimeout timeout(Clock::now() + Clock::fromSeconds(10));
          _expectedObjectClassHandle = attributesByObjectClass.first;
          while (_foreignObjectInstanceHandles.size() != 1 && !_fail) {
            if (ambassador->evokeCallback(60.0))
              continue;
            if (timeout.isExpired()) {
              std::wcout << L"Timeout waiting for next message" << std::endl;
              return false;
            }
          }
          if (_fail)
            return false;

        }
        catch (const rti1516ev::Exception& e) {
          std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
          return false;
        }
        catch (...) {
          std::wcout << L"Unknown Exception!" << std::endl;
          return false;
        }
      } // if (threadIndex == 0)
      //if (!waitForAllFederates(ambassador))
      //  return false;
    } // for (auto& attributesByObjectClass : _objectClassAttributeHandleSet)

    if (!waitForAllFederates(ambassador))
      return false;

    if (!updateAndReceiveAttributeValues(ambassador))
      return false;
    
    if (!_foreignObjectInstanceHandles.empty()) {
      auto foreignObjIt = _foreignObjectInstanceHandles.begin();
      rti1516ev::ObjectInstanceHandle objectInstance = *foreignObjIt;
      rti1516ev::ObjectClassHandle objectClass = ambassador->getKnownObjectClassHandle(objectInstance);
      auto classIter = _objectClassAttributeHandleSet.find(objectClass);
      if (classIter != _objectClassAttributeHandleSet.end())
      {
        auto attributes = classIter->second;
        attributes.insert(_privilegeToDeleteObject[objectClass]);
        ambassador->attributeOwnershipAcquisition(objectInstance, attributes, rti1516ev::VariableLengthData());
        std::wcout << L"attributeOwnershipAcquisition sent" << std::endl;
      }
      while (_ownedObjectInstanceHandles.find(objectInstance) == _ownedObjectInstanceHandles.end() && !_fail)
      {
        if (ambassador->evokeCallback(60.0))
          continue;
      }
    }

    if (!waitForAllFederates(ambassador))
      return false;

    if (!updateAndReceiveAttributeValues(ambassador))
      return false;

    for (auto objectInstance : _ownedObjectInstanceHandles)
    {
      try {
        //{std::wstringstream msg; msg << getFederateType() << L" deleteObjectInstance " << objectInstanceHandle.toString() << std::endl; std::wcout << msg.str(); }
        DebugPrintf("%s[%ls]: deleting object instance %ls\n", __FUNCTION__, getFederateName().c_str(), _ambassador->getObjectInstanceName(objectInstance).c_str());
        ambassador->deleteObjectInstance(objectInstance, toVariableLengthData("tag"));
      } catch (const rti1516ev::Exception& e) {
        std::wcout << getFederateName() << L": rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << getFederateName() << L"Unknown Exception!" << std::endl;
        return false;
      }
    }

    // Wait until all attribute updates are received and foreign objects were erased in removeObjectInstance
    try {
      AbsTimeout timeout(Clock::now() + Clock::fromSeconds(10));

      while (!_foreignObjectInstanceHandles.empty() && !_fail) {
        if (ambassador->evokeCallback(60.0))
          continue;
        if (timeout.isExpired()) {
          std::wcout << L"Timeout waiting for next message" << std::endl;
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

    if (!waitForAllFederates(ambassador))
      return false;

    for (auto& attributesByObjectClass : _objectClassAttributeHandleSet) {
      rti1516ev::ObjectInstanceHandle objectInstanceHandle = rti1516ev::ObjectInstanceHandle();
      auto objectClass = attributesByObjectClass.first;
      try {
        ambassador->unsubscribeObjectClass(objectClass);
      } catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
      try {
        ambassador->unpublishObjectClass(objectClass);
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

  bool updateAndReceiveAttributeValues(rti1516ev::RTIambassador* ambassador)
  {
    try {
      for (auto objectInstance : _ownedObjectInstanceHandles) {
        auto objectClass = _ambassador->getKnownObjectClassHandle(objectInstance);
        rti1516ev::AttributeHandleValueMap attributeValues;
        rti1516ev::VariableLengthData tag = toVariableLengthData(Clock::now());
        auto iter = _objectClassAttributeHandleSet.find(objectClass);
        if (iter == _objectClassAttributeHandleSet.end())
          return false;
        for (auto& attributeHandle : iter->second) {
          attributeValues[attributeHandle] = toVariableLengthData(ambassador->getAttributeName(objectClass, attributeHandle));
        }
        DebugPrintf("%s[%ls]: sending update for object instance %ls\n", __FUNCTION__, getFederateName().c_str(), _ambassador->getObjectInstanceName(objectInstance).c_str());
        ambassador->updateAttributeValues(objectInstance, attributeValues, tag);
      }
    }
    catch (const rti1516ev::AttributeNotOwned& e) {
      std::wcout << L"rti1516ev::AttributeNotOwned: \"" << e.what() << L"\"" << std::endl;
    }
    catch (const rti1516ev::Exception& e) {
      std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    }
    catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    if (!_foreignObjectInstanceHandles.empty())
    {
      try {
        AbsTimeout timeout(Clock::now() + Clock::fromSeconds(10));
        //while (_reflectedObjectInstanceHandleSet.size() != 1 && !_fail) {
        while (_reflectedObjectInstanceHandleSet != _foreignObjectInstanceHandles && !_fail) {
          if (ambassador->evokeCallback(60.0))
            continue;
          if (timeout.isExpired()) {
            std::wcout << L"Timeout waiting for next message" << std::endl;
            return false;
          }
        }
      }
      catch (const rti1516ev::Exception& e) {
        std::wcout << L"rti1516ev::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      }
      catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
    }
    if (!waitForAllFederates(ambassador))
      return false;

    return true;
  }

  void discoverObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle,
                              rti1516ev::ObjectClassHandle objectClassHandle,
                              const std::wstring& objectInstanceName) override
  {
    DebugPrintf("%s[%ls]: discovered object instance %ls\n", __FUNCTION__, getFederateName().c_str(), _ambassador->getObjectInstanceName(objectInstanceHandle).c_str());
    if (_expectedObjectClassHandle != objectClassHandle) {
      Log(Assert, Error) << "Expected object class " << _expectedObjectClassHandle.toString()
                         << ", but discovered object class "  << objectClassHandle.toString() << std::endl;
      _fail = true;
    }
    if (_foreignObjectInstanceHandles.find(objectInstanceHandle) != _foreignObjectInstanceHandles.end()) {
      Log(Assert, Error) << "Duplicate discoverObjectInstance callback for object instance "
                         << objectInstanceHandle.toString() << std::endl;
      _fail = true;
    }
    _foreignObjectInstanceHandles.insert(objectInstanceHandle);
  }

  void turnUpdatesOnForObjectInstance(rti1516ev::ObjectInstanceHandle theObject,
    rti1516ev::AttributeHandleSet const& theAttributes,
    std::wstring const& updateRateDesignator) override
  {
    //{std::wstringstream msg; msg << getFederateType() << L" turnUpdatesOnForObjectInstance " << theObject.toString() << std::endl; std::wcout << msg.str(); }
  }

  virtual void turnUpdatesOffForObjectInstance(rti1516ev::ObjectInstanceHandle theObject,
    rti1516ev::AttributeHandleSet const& theAttributes) override
  {
    //{std::wstringstream msg; msg << getFederateType() << L" turnUpdatesOffForObjectInstance " << theObject.toString() << std::endl; std::wcout << msg.str(); }
  }

  void removeObjectInstance(rti1516ev::ObjectInstanceHandle objectInstanceHandle, rti1516ev::VariableLengthData const& tag,
    rti1516ev::OrderType sentOrder, rti1516ev::SupplementalRemoveInfo theRemoveInfo) override
  {
    //{std::wstringstream msg; msg << getFederateType() << L" remove " << objectInstanceHandle.toString() << std::endl; std::wcout << msg.str(); }

    if (_foreignObjectInstanceHandles.find(objectInstanceHandle) == _foreignObjectInstanceHandles.end()) {
      DebugPrintf("%s[%ls]: Spurious removeObjectInstance callback for object instance %ls\n", __FUNCTION__, getFederateName().c_str(), _ambassador->getObjectInstanceName(objectInstanceHandle).c_str());
      _fail = true;
    }
    if (_ownedObjectInstanceHandles.find(objectInstanceHandle) != _ownedObjectInstanceHandles.end()) {
      DebugPrintf("%s[%ls]: removeObjectInstance callback for owned object instance %ls\n", __FUNCTION__, getFederateName().c_str(), _ambassador->getObjectInstanceName(objectInstanceHandle).c_str());
      _fail = true;
    }
    // Log(Assert, Error) << "remove " << objectInstanceHandle.toString() << std::endl;

    _foreignObjectInstanceHandles.erase(objectInstanceHandle);
  }

  // this is the only virtual override actually required!
  void reflectAttributeValues(rti1516ev::ObjectInstanceHandle objectInstanceHandle, rti1516ev::AttributeHandleValueMap const& attributeValues,
    rti1516ev::VariableLengthData const&, rti1516ev::OrderType, rti1516ev::TransportationType, rti1516ev::SupplementalReflectInfo) override
  {
    DebugPrintf("%s[%ls]: reflected object instance %ls\n", __FUNCTION__, getFederateName().c_str(), _ambassador->getObjectInstanceName(objectInstanceHandle).c_str());
    _checkReflectedAttributeValues(objectInstanceHandle, attributeValues);
  }

  void _checkReflectedAttributeValues(const rti1516ev::ObjectInstanceHandle& objectInstanceHandle, const rti1516ev::AttributeHandleValueMap& attributeValues)
  {
    // Check if we got at most a single attribute update
    if (!_reflectedObjectInstanceHandleSet.insert(objectInstanceHandle).second) {
      std::cerr << "Duplicate reflectAttributeValues." << std::endl;
      _fail = true;
    }
    auto expectedAttributes = _objectClassAttributeHandleSet[_ambassador->getKnownObjectClassHandle(objectInstanceHandle)];
    for (auto& attributeHandleValuePair : attributeValues) {
      if (_expectedAttributeHandles.find(attributeHandleValuePair.first) != _expectedAttributeHandles.end())
        continue;
      std::wcerr << L"Received attribute value for unsubscribed attribute " << attributeHandleValuePair.first.toString() << std::endl;
      _fail = true;
    }
  }

  bool _fail;

  // The available object class, attribute set
  typedef std::map<rti1516ev::ObjectClassHandle, rti1516ev::AttributeHandleSet> ObjectClassAttributeHandleSet;
  ObjectClassAttributeHandleSet _objectClassAttributeHandleSet;

  std::map<rti1516ev::ObjectClassHandle, rti1516ev::AttributeHandle> _privilegeToDeleteObject;

  typedef std::set<rti1516ev::ObjectInstanceHandle> ObjectInstanceHandleSet;

  // objects we sent updates for, in the current iteration
  ObjectInstanceHandleSet _updatedObjectInstanceHandleSet;
  // objects we received updates for, in the current iteration
  ObjectInstanceHandleSet _reflectedObjectInstanceHandleSet;

  rti1516ev::ObjectClassHandle _expectedObjectClassHandle;
  rti1516ev::AttributeHandleSet _expectedAttributeHandles;

  std::set<rti1516ev::ObjectInstanceHandle> _ownedObjectInstanceHandles;
  std::set<rti1516ev::ObjectInstanceHandle> _foreignObjectInstanceHandles;
};

class OPENRTI_LOCAL Test : public RTITest {
public:
  Test(int argc, const char* const argv[]) :
    RTITest(argc, argv, false)
  { }
  virtual SharedPtr<Ambassador> createAmbassador(const ConstructorArgs& constructorArgs) override
  {
    return MakeShared<TestAmbassador>(constructorArgs);
  }
};

}

int
main(int argc, char* argv[])
{
  OpenRTI::Test test(argc, argv);
  return test.exec();
}
