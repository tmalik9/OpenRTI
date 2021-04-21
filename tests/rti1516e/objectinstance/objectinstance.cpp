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

  virtual bool execJoined(rti1516e::RTIambassador& ambassador) override
  {
    try {
      rti1516e::ObjectClassHandle objectClassHandle = ambassador.getObjectClassHandle(L"ObjectClass0");
      rti1516e::ObjectClassHandle fqObjectClassHandle = ambassador.getObjectClassHandle(L"HLAobjectRoot.ObjectClass0");
      if (objectClassHandle != fqObjectClassHandle) {
        std::wcout << L"Full qualified object class lookup failed" << std::endl;
        return false;
      }
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador.getAttributeHandle(objectClassHandle, L"attribute0"));
      _objectClassObjectClassHandleSet[objectClassHandle];
    } catch (const rti1516e::Exception& e) {
      std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      rti1516e::ObjectClassHandle objectClassHandle = ambassador.getObjectClassHandle(L"ObjectClass1");
      rti1516e::ObjectClassHandle fqObjectClassHandle = ambassador.getObjectClassHandle(L"HLAobjectRoot.ObjectClass0.ObjectClass1");
      if (objectClassHandle != fqObjectClassHandle) {
        std::wcout << L"Full qualified object class lookup failed" << std::endl;
        return false;
      }
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador.getAttributeHandle(objectClassHandle, L"attribute0"));
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador.getAttributeHandle(objectClassHandle, L"attribute1"));
      _objectClassObjectClassHandleSet[objectClassHandle].insert(ambassador.getObjectClassHandle(L"ObjectClass0"));
    } catch (const rti1516e::Exception& e) {
      std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      rti1516e::ObjectClassHandle objectClassHandle = ambassador.getObjectClassHandle(L"ObjectClass2");
      rti1516e::ObjectClassHandle fqObjectClassHandle = ambassador.getObjectClassHandle(L"HLAobjectRoot.ObjectClass0.ObjectClass1.ObjectClass2");
      if (objectClassHandle != fqObjectClassHandle) {
        std::wcout << L"Full qualified object class lookup failed" << std::endl;
        return false;
      }
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador.getAttributeHandle(objectClassHandle, L"attribute0"));
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador.getAttributeHandle(objectClassHandle, L"attribute1"));
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador.getAttributeHandle(objectClassHandle, L"attribute2"));
      _objectClassObjectClassHandleSet[objectClassHandle].insert(ambassador.getObjectClassHandle(L"ObjectClass0"));
      _objectClassObjectClassHandleSet[objectClassHandle].insert(ambassador.getObjectClassHandle(L"ObjectClass1"));
    } catch (const rti1516e::Exception& e) {
      std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      rti1516e::ObjectClassHandle objectClassHandle = ambassador.getObjectClassHandle(L"ObjectClass3");
      rti1516e::ObjectClassHandle fqObjectClassHandle = ambassador.getObjectClassHandle(L"HLAobjectRoot.ObjectClass0.ObjectClass1.ObjectClass3");
      if (objectClassHandle != fqObjectClassHandle) {
        std::wcout << L"Full qualified object class lookup failed" << std::endl;
        return false;
      }
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador.getAttributeHandle(objectClassHandle, L"attribute0"));
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador.getAttributeHandle(objectClassHandle, L"attribute1"));
      _objectClassAttributeHandleSet[objectClassHandle].insert(ambassador.getAttributeHandle(objectClassHandle, L"attribute3"));
      _objectClassObjectClassHandleSet[objectClassHandle].insert(ambassador.getObjectClassHandle(L"ObjectClass0"));
      _objectClassObjectClassHandleSet[objectClassHandle].insert(ambassador.getObjectClassHandle(L"ObjectClass1"));
    } catch (const rti1516e::Exception& e) {
      std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    for (std::map<rti1516e::ObjectClassHandle, rti1516e::AttributeHandleSet>::const_iterator i = _objectClassAttributeHandleSet.begin();
         i != _objectClassAttributeHandleSet.end(); ++i) {
      try {
        ambassador.publishObjectClassAttributes(i->first, i->second);
      } catch (const rti1516e::Exception& e) {
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }
    }

    // All is published, now subscribe step by step and see what we receive

    for (ObjectClassAttributeHandleSet::const_iterator i = _objectClassAttributeHandleSet.begin();
         i != _objectClassAttributeHandleSet.end(); ++i) {


      rti1516e::ObjectClassHandle subscribedObjectClass = i->first;
      rti1516e::AttributeHandleSet subscribedAttributeHandles = i->second;

      try {
        ambassador.subscribeObjectClassAttributes(subscribedObjectClass, subscribedAttributeHandles);
      } catch (const rti1516e::Exception& e) {
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

      if (!waitForAllFederates(ambassador))
        return false;

      for (ObjectClassAttributeHandleSet::const_iterator j = _objectClassAttributeHandleSet.begin();
           j != _objectClassAttributeHandleSet.end(); ++j) {
        rti1516e::ObjectClassHandle registeredObjectClass = j->first;
        rti1516e::ObjectInstanceHandle objectInstanceHandle;
        bool expectDiscovered = true;

        if (registeredObjectClass == subscribedObjectClass) {
          _expectedObjectClassHandle = subscribedObjectClass;
          _expectedAttributeHandles = subscribedAttributeHandles;
        } else if (_objectClassObjectClassHandleSet[registeredObjectClass].count(subscribedObjectClass)) {
          _expectedObjectClassHandle = subscribedObjectClass;
          _expectedAttributeHandles = j->second;
        } else {
          _expectedObjectClassHandle = rti1516e::ObjectClassHandle();
          _expectedAttributeHandles.clear();
          expectDiscovered = false;
        }

        try {
          objectInstanceHandle = ambassador.registerObjectInstance(registeredObjectClass);
        } catch (const rti1516e::Exception& e) {
          std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
          return false;
        } catch (...) {
          std::wcout << L"Unknown Exception!" << std::endl;
          return false;
        }

        if (expectDiscovered) {
          try {
            Clock timeout = Clock::now() + Clock::fromSeconds(10);
            while (getFederateList().size() != _foreignObjectInstanceHandles.size() + 1 && !_fail) {
              if (ambassador.evokeCallback(60.0))
                continue;
              if (timeout < Clock::now()) {
                std::wcout << L"Timeout waiting for next message" << std::endl;
                return false;
              }
            }
            if (_fail)
              return false;

          } catch (const rti1516e::Exception& e) {
            std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
            return false;
          } catch (...) {
            std::wcout << L"Unknown Exception!" << std::endl;
            return false;
          }

          if (!_foreignObjectInstanceHandles.empty()) {

            // The first foreign objectInstance gets unsubscribed
            auto foreignObjIt = _foreignObjectInstanceHandles.begin();
            rti1516e::ObjectInstanceHandle obj = *foreignObjIt;
            _unsubscribedObjectInstanceHandle = obj;

            ambassador.unsubscribeObjectInstance(obj);

             // The second foreign objectInstance gets unsubscribed and resubscribed. Needs at least 3 federates (to get the 2 foreign objects)
            if (_foreignObjectInstanceHandles.size() > 1 && !_resubscribedObjectInstanceHandle.isValid()) {

              _resubscribedObjectInstanceHandle = *(++foreignObjIt);
              ambassador.unsubscribeObjectInstance(_resubscribedObjectInstanceHandle);

              try {
                // Implicit resubscribe on requestAttributeValueUpdate
                ambassador.requestAttributeValueUpdate(_resubscribedObjectInstanceHandle, subscribedAttributeHandles, toVariableLengthData(Clock::now()));
              }
              catch (const rti1516e::Exception & e) {
                std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
                return false;
              }
              catch (...) {
                std::wcout << L"Unknown Exception!" << std::endl;
                return false;
              }

              //{std::wstringstream msg; msg << getFederateType() << L" unsubscribeObjectInstance + requestAttributeValueUpdate for " << _resubscribedObjectInstanceHandle.toString() << std::endl; std::wcout << msg.str(); }

            }
          }

        }

        if (!waitForAllFederates(ambassador))
          return false;

        try {
          rti1516e::AttributeHandleValueMap attributeValues;
          rti1516e::VariableLengthData tag = toVariableLengthData(Clock::now());
          for (rti1516e::AttributeHandleSet::const_iterator k = j->second.begin();
               k != j->second.end(); ++k) {
            attributeValues[*k] = toVariableLengthData(ambassador.getAttributeName(registeredObjectClass, *k));
          }
          ambassador.updateAttributeValues(objectInstanceHandle, attributeValues, tag);
        } catch (const rti1516e::Exception& e) {
          std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
          return false;
        } catch (...) {
          std::wcout << L"Unknown Exception!" << std::endl;
          return false;
        }

        try {
          //{std::wstringstream msg; msg << getFederateType() << L" deleteObjectInstance " << objectInstanceHandle.toString() << std::endl; std::wcout << msg.str(); }
          ambassador.deleteObjectInstance(objectInstanceHandle, toVariableLengthData("tag"));
        } catch (const rti1516e::Exception& e) {
          std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
          return false;
        } catch (...) {
          std::wcout << L"Unknown Exception!" << std::endl;
          return false;
        }

        // Wait until all attribute updates are received and foreign objects were erased in removeObjectInstance
        if (expectDiscovered)
        {
          try {
            Clock timeout = Clock::now() + Clock::fromSeconds(10);

            while (!_foreignObjectInstanceHandles.empty() && !_fail) {
              if (ambassador.evokeCallback(60.0))
                continue;
              if (timeout < Clock::now()) {
                std::wcout << L"Timeout waiting for next message" << std::endl;
                return false;
              }
            }
            if (_fail)
              return false;

          } catch (const rti1516e::Exception& e) {
            std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
            return false;
          } catch (...) {
            std::wcout << L"Unknown Exception!" << std::endl;
            return false;
          }
        }

        _unsubscribedObjectInstanceHandle = rti1516e::ObjectInstanceHandle();
        _resubscribedObjectInstanceHandle = rti1516e::ObjectInstanceHandle();

        if (!waitForAllFederates(ambassador))
          return false;

      }

      try {
        ambassador.unsubscribeObjectClass(subscribedObjectClass);
      } catch (const rti1516e::Exception& e) {
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }

    }
    
    return true;
  }

  void discoverObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,
                              rti1516e::ObjectClassHandle objectClassHandle,
                              const std::wstring& objectInstanceName) override
  {
    //{std::wstringstream msg; msg << getFederateType() << L" discoverObjectInstance " << objectInstanceHandle.toString() << std::endl; std::wcout << msg.str(); }

    // Log(Assert, Error) << "discover "  << objectClassHandle.toString() << " " << objectInstanceHandle.toString() << std::endl;
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

  void turnUpdatesOnForObjectInstance(rti1516e::ObjectInstanceHandle theObject,
    rti1516e::AttributeHandleSet const& theAttributes,
    std::wstring const& updateRateDesignator) override
  {
    //{std::wstringstream msg; msg << getFederateType() << L" turnUpdatesOnForObjectInstance " << theObject.toString() << std::endl; std::wcout << msg.str(); }
  }

  virtual void turnUpdatesOffForObjectInstance(rti1516e::ObjectInstanceHandle theObject,
    rti1516e::AttributeHandleSet const& theAttributes) override
  {
    //{std::wstringstream msg; msg << getFederateType() << L" turnUpdatesOffForObjectInstance " << theObject.toString() << std::endl; std::wcout << msg.str(); }
  }

  void removeObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle, rti1516e::VariableLengthData const& tag,
    rti1516e::OrderType sentOrder, rti1516e::SupplementalRemoveInfo theRemoveInfo) override
  {
    //{std::wstringstream msg; msg << getFederateType() << L" remove " << objectInstanceHandle.toString() << std::endl; std::wcout << msg.str(); }

    if (_foreignObjectInstanceHandles.find(objectInstanceHandle) == _foreignObjectInstanceHandles.end()) {
      Log(Assert, Error) << "Spurious removeObjectInstance callback for object instance "
                         << objectInstanceHandle.toString() << std::endl;
      _fail = true;
    }
    // Log(Assert, Error) << "remove " << objectInstanceHandle.toString() << std::endl;

    // We should get no attribute update for the unsubscribed objectInstance
    if (_objectInstanceHandleSet.find(_unsubscribedObjectInstanceHandle) != _objectInstanceHandleSet.end()) {
      Log(Assert, Error) << getFederateType() << " Should not receive attribute updates for unsubscribed objectInstance "
        << objectInstanceHandle.toString() << std::endl;
      _fail = true;
    }

    // We should have received an attribute update for the resubscribed objectInstance
    if (_resubscribedObjectInstanceHandle.isValid() && objectInstanceHandle == _resubscribedObjectInstanceHandle &&  _objectInstanceHandleSet.find(_resubscribedObjectInstanceHandle) == _objectInstanceHandleSet.end()) {
      Log(Assert, Error) << getFederateType() << " Should receive attribute updates for resubscribed objectInstance "
        << _resubscribedObjectInstanceHandle.toString() << std::endl;
      _fail = true;
    }

    // Check if we got a single attribute update
    if (objectInstanceHandle != _unsubscribedObjectInstanceHandle && 1 != _objectInstanceHandleSet.erase(objectInstanceHandle)) {
      Log(Assert, Error) << getFederateType() << " Should have received at least a single message for "
                         << objectInstanceHandle.toString() << " " << _unsubscribedObjectInstanceHandle.toString() << std::endl;
      _fail = true;
    }

    _foreignObjectInstanceHandles.erase(objectInstanceHandle);
  }

  // this is the only virtual override actually required!
  virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle, rti1516e::AttributeHandleValueMap const& attributeValues,
    rti1516e::VariableLengthData const&, rti1516e::OrderType, rti1516e::TransportationType, rti1516e::SupplementalReflectInfo) override
  {
    _checkReflectedAttributeValues(objectInstanceHandle, attributeValues);
  }


  //void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle, const rti1516e::AttributeHandleValueMap& attributeValues,
  //  const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType) override
  //{
  //  _checkReflectedAttributeValues(objectInstanceHandle, attributeValues);
  //}

  //virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle, const rti1516e::AttributeHandleValueMap& attributeValues,
  //                                    const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
  //                                    const rti1516e::RegionHandleSet&)
  //{
  //  _checkReflectedAttributeValues(objectInstanceHandle, attributeValues);
  //}

  //virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle, const rti1516e::AttributeHandleValueMap& attributeValues,
  //                                    const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
  //                                    const rti1516e::LogicalTime&, rti1516e::OrderType)
  //{
  //  _checkReflectedAttributeValues(objectInstanceHandle, attributeValues);
  //}

  //virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle, const rti1516e::AttributeHandleValueMap& attributeValues,
  //                                    const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
  //                                    const rti1516e::LogicalTime&, rti1516e::OrderType, const rti1516e::RegionHandleSet&)
  //{
  //  _checkReflectedAttributeValues(objectInstanceHandle, attributeValues);
  //}

  //virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle, const rti1516e::AttributeHandleValueMap& attributeValues,
  //                                    const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
  //                                    const rti1516e::LogicalTime&, rti1516e::OrderType, rti1516e::MessageRetractionHandle)
  //{
  //  _checkReflectedAttributeValues(objectInstanceHandle, attributeValues);
  //}

  //virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle, const rti1516e::AttributeHandleValueMap& attributeValues,
  //                                    const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
  //                                    const rti1516e::LogicalTime&, rti1516e::OrderType, rti1516e::MessageRetractionHandle,
  //                                    const rti1516e::RegionHandleSet&)
  //{
  //  _checkReflectedAttributeValues(objectInstanceHandle, attributeValues);
  //}

  void _checkReflectedAttributeValues(const rti1516e::ObjectInstanceHandle& objectInstanceHandle, const rti1516e::AttributeHandleValueMap& attributeValues)
  {
    if (objectInstanceHandle == _unsubscribedObjectInstanceHandle) {
      Log(Assert, Error) << "Received attribute value for unsubscribed object instance " << objectInstanceHandle.toString() << std::endl;
      _fail = true;
    }

    // Check if we got at most a single attribute update
    if (!_objectInstanceHandleSet.insert(objectInstanceHandle).second) {
      Log(Assert, Error) << "Duplicate reflectAttributeValues." << std::endl;
      _fail = true;
    }
    //else {
    //  {std::wstringstream msg; msg << getFederateType() << L" reflect " << objectInstanceHandle.toString() << std::endl; std::wcout << msg.str(); }
    //}

    for (rti1516e::AttributeHandleValueMap::const_iterator i = attributeValues.begin();
         i != attributeValues.end(); ++i) {
      if (_expectedAttributeHandles.find(i->first) != _expectedAttributeHandles.end())
        continue;
      Log(Assert, Error) << "Received attribute value for unsubscribed attribute " << i->first.toString() << std::endl;
      _fail = true;
    }
  }

  bool _fail;

  // Store the 'is derived of' relation
  typedef std::map<rti1516e::ObjectClassHandle, std::set<rti1516e::ObjectClassHandle> > ObjectClassObjectClassHandleSet;
  ObjectClassObjectClassHandleSet _objectClassObjectClassHandleSet;

  // The available object class, attribute set
  typedef std::map<rti1516e::ObjectClassHandle, rti1516e::AttributeHandleSet> ObjectClassAttributeHandleSet;
  ObjectClassAttributeHandleSet _objectClassAttributeHandleSet;

  typedef std::set<rti1516e::ObjectInstanceHandle> ObjectInstanceHandleSet;
  ObjectInstanceHandleSet _objectInstanceHandleSet;

  rti1516e::ObjectClassHandle _expectedObjectClassHandle;
  rti1516e::AttributeHandleSet _expectedAttributeHandles;

  rti1516e::ObjectInstanceHandle _unsubscribedObjectInstanceHandle;
  rti1516e::ObjectInstanceHandle _resubscribedObjectInstanceHandle;

  std::set<rti1516e::ObjectInstanceHandle> _foreignObjectInstanceHandles;
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
