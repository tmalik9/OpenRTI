
#include "DebugNew.h"
#include <iostream>
#include <memory.h>
#include "ImmediateCallbackFederate.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>
#include <RTI/LogicalTimeInterval.h>
#include "RTI/time/HLAfloat64Interval.h"
#include "RTI/time/HLAfloat64Time.h"

#include "dprintf.h"
#include <assert.h>
#include "RTI/encoding/BasicDataElements.h"
#include "RTI/encoding/HLAopaqueData.h"
#include <random>

ImmediateCallbackFederate::ImmediateCallbackFederate() : ImmediateCallbackTestFederate()
{
  setPrintVerbose(false);
}

ImmediateCallbackFederate::~ImmediateCallbackFederate()
{
}

void ImmediateCallbackFederate::step()
{
  try
  {
    /// update the attribute values of the instance
    updateAttributeValues(myPublishedObject, true);
    /// send an interaction
    sendInteraction(true);
    /// request a time advance and wait until we get it
    advanceTime(std::chrono::duration<double>(getStepDuration()).count());
    //cout << "Time Advanced to " << fedamb->federateTime << endl;
  }
  catch (const rti1516ev::InvalidLogicalTime& e)
  {
    std::unique_lock<std::mutex> lock(_coutMutex);
    std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ": " << e << std::endl;
  }
  catch (const rti1516ev::Exception& e)
  {
    std::unique_lock<std::mutex> lock(_coutMutex);
    std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ": " << e << std::endl;
    setDone();
  }
  catch (const std::exception& e)
  {
    std::unique_lock<std::mutex> lock(_coutMutex);
    std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ": " << e.what() << std::endl;
    setDone();
  }
  catch (...)
  {
    std::unique_lock<std::mutex> lock(_coutMutex);
    std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ": " << "unknown exception" << std::endl;
    setDone();
  }
}


void ImmediateCallbackFederate::initializeSimulation()
{
  InitializeObjects();
  InitializeInteraction();
}


void ImmediateCallbackFederate::joined()
{
  for (int i = 0; i < 10; i++)
  {
    sendThreads.push_back(std::thread([this] {
      std::random_device rd;  //Will be used to obtain a seed for the random number engine
      std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
      std::uniform_int_distribution<> distrib(10, 100);
      while (!_done)
      {
        try
        {
          updateAttributeValues(myPublishedObject, true);
          sendInteraction(true);
          std::this_thread::sleep_for(std::chrono::milliseconds(distrib(gen)));
        }
        catch (const rti1516ev::InvalidLogicalTime& e)
        {
          std::unique_lock<std::mutex> lock(_coutMutex);
          std::cout << "[TID=" << std::this_thread::get_id() << "] " << e << std::endl;
        }
        catch (const rti1516ev::Exception& e)
        {
          std::unique_lock<std::mutex> lock(_coutMutex);
          std::cout << "[TID=" << std::this_thread::get_id() << "] " << e << std::endl;
          setDone();
        }
        catch (const std::exception& e)
        {
          std::unique_lock<std::mutex> lock(_coutMutex);
          std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ": " << e.what() << std::endl;
          setDone();
        }
        catch (...)
        {
          std::unique_lock<std::mutex> lock(_coutMutex);
          std::cout << "[TID=" << std::this_thread::get_id() << "] " << ": " << "unknown exception" << std::endl;
          setDone();
        }
      }
      std::unique_lock<std::mutex> lock(_coutMutex);
      std::cout << "[TID=" << std::this_thread::get_id() << "] " << ": " << "exiting" << std::endl;
    }));
  }
}

void ImmediateCallbackFederate::InitializeInteraction()
{
  x_InteractionClass = mRtiAmb->getInteractionClassHandle(L"HLAinteractionRoot.X");
  xa_Parameter = mRtiAmb->getParameterHandle(x_InteractionClass, to_wstring("xa"));
  xb_Parameter = mRtiAmb->getParameterHandle(x_InteractionClass, to_wstring("xb"));
  xc_Parameter = mRtiAmb->getParameterHandle(x_InteractionClass, to_wstring("xc"));

  //////////////////////////////////////////////////////
  /// publish the interaction class InteractionRoot.X
  //////////////////////////////////////////////////////
  /// we want to send interactions of type InteractionRoot.X, so we need
  /// to tell the RTI that we're publishing it first. We don't need to
  /// inform it of the parameters, only the class, making it much simpler
  /// do the publication
  mRtiAmb->publishInteractionClass(this->x_InteractionClass);
  /////////////////////////////////////////////////////
  /// subscribe to the InteractionRoot.X interaction //
  /////////////////////////////////////////////////////
  /// we also want to receive other interaction of the same type that are
  /// sent out by other federates, so we have to subscribe to it first
  //mRtiAmb->subscribeInteractionClass(this->xHandle);
  rti1516ev::ParameterHandleValueMap filter;
  filter[xa_Parameter] = HLAunicodeString(L"test1").encode();
  mRtiAmb->subscribeInteractionClassWithFilter(this->x_InteractionClass, filter);
  HLAunicodeString decoded;
  decoded.decode(filter[xa_Parameter]);
  printf("subscribed to interaction %ls with filter %ls => %ls\n",
         mRtiAmb->getInteractionClassName(this->x_InteractionClass).c_str(), mRtiAmb->getParameterName(this->x_InteractionClass, xa_Parameter).c_str(), decoded.get().c_str());
  filter[xa_Parameter] = HLAunicodeString(L"test2").encode();
  printf("subscribed to interaction %ls with filter %ls => %ls\n",
         mRtiAmb->getInteractionClassName(this->x_InteractionClass).c_str(), mRtiAmb->getParameterName(this->x_InteractionClass, xa_Parameter).c_str(), decoded.get().c_str());
  mRtiAmb->subscribeInteractionClassWithFilter(this->x_InteractionClass, filter);
  //mRtiAmb->setInteractionClassDeliverToSelf(this->x_InteractionClass, true);
}

void ImmediateCallbackFederate::InitializeObjects()
{
  ObjectClassHandle aHandle = mRtiAmb->getObjectClassHandle(L"HLAobjectRoot.A");
  ObjectClassHandle bHandle = mRtiAmb->getObjectClassHandle(L"HLAobjectRoot.B");
  mObjectClassHandles["HLAobjectRoot.A"] = aHandle;
  mObjectClassHandles["HLAobjectRoot.B"] = bHandle;
  //aHandle  = mRtiAmb->getObjectClassHandle(L"HLAobjectRoot.A");
  aaHandle = mRtiAmb->getAttributeHandle(aHandle, L"aa");
  abHandle = mRtiAmb->getAttributeHandle(aHandle, L"ab");
  acHandle = mRtiAmb->getAttributeHandle(aHandle, L"ac");
  //bHandle  = mRtiAmb->getObjectClassHandle(L"HLAobjectRoot.B");
  baHandle = mRtiAmb->getAttributeHandle(bHandle, L"ba");
  bbHandle = mRtiAmb->getAttributeHandle(bHandle, L"bb");
  bcHandle = mRtiAmb->getAttributeHandle(bHandle, L"bc");

  AttributeHandleSet attributesOfA;
  AttributeHandleSet attributesOfB;
  attributesOfA.insert(this->aaHandle);
  attributesOfA.insert(this->abHandle);
  attributesOfA.insert(this->acHandle);

  attributesOfB.insert(this->baHandle);
  attributesOfB.insert(this->bbHandle);
  attributesOfB.insert(this->bcHandle);

  /// do the actual publication
  myPublishedObjectClass = mObjectClassHandles["HLAobjectRoot.B"];
  myPublishedAttributes = attributesOfB;
  mRtiAmb->publishObjectClassAttributes(myPublishedObjectClass, myPublishedAttributes);

  //////////////////////////////////////////////////
  /// subscribe to all attributes of ObjectRoot.A
  //////////////////////////////////////////////////
  /// we also want to hear about the same sort of information as it is
  ///// created and altered in other federates, so we need to subscribe to it
  mRtiAmb->subscribeObjectClassAttributes(mObjectClassHandles["HLAobjectRoot.A"], attributesOfA, true);
  mRtiAmb->subscribeObjectClassAttributes(mObjectClassHandles["HLAobjectRoot.B"], attributesOfB, true);

  myPublishedObject = mRtiAmb->registerObjectInstance(myPublishedObjectClass);
  printf("Registered Object, handle=%ls class==%ls\n", myPublishedObject.toString().c_str(), mRtiAmb->getObjectClassName(mRtiAmb->getKnownObjectClassHandle(myPublishedObject)).c_str());

}

void ImmediateCallbackFederate::cleanupSimulation()
{
  assert(_done);
  for (auto& thread : sendThreads)
  {
    thread.join();
  }
  deleteObject(myPublishedObject);
  printf("Deleted Object, handle=%ls", myPublishedObject.toString().c_str());
}

void ImmediateCallbackFederate::sendInteraction(bool withTimeStamp)
{
  ////////////////////////////////////////////////
  /// create the necessary container and values
  ////////////////////////////////////////////////
  /// create the collection to store the values in
  ParameterHandleValueMap parameters;
  /// generate the new values
  wchar_t xbValue[16];
  swprintf(xbValue, 16, L"xb:%f", getLbts());
  unsigned char data[1024];
  for (size_t i = 0; i < sizeof(data); i++)
  {
    data[i] = i % 256;
  }
  parameters[xa_Parameter] = HLAunicodeString(L"test1").encode();
  parameters[xb_Parameter] = HLAunicodeString(xbValue).encode();
  parameters[xc_Parameter] = HLAopaqueData(data, sizeof(data)).encode();
  ///////////////////////////
  /// send the interaction
  ///////////////////////////
  if (withTimeStamp)
  {
    std::unique_lock<std::mutex> lock(_federateTimeMutex);
    HLAfloat64Time time = getFederateTime() + _federateLookahead;
    {
      std::unique_lock<std::mutex> coutLock(_coutMutex);
      std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ": "
                << to_string(mRtiAmb->getInteractionClassName(x_InteractionClass))
                << ", time=" << time
                << std::endl;
    }
    mRtiAmb->sendInteraction(x_InteractionClass, parameters, toVariableLengthData(L"hi!"), time);
  }
  else
  {
    {
      std::unique_lock<std::mutex> coutLock(_coutMutex);
      std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ": "
                << to_string(mRtiAmb->getInteractionClassName(x_InteractionClass))
                << std::endl;
    }
    mRtiAmb->sendInteraction(x_InteractionClass, parameters, toVariableLengthData(L"hi!"));
  }
}

void ImmediateCallbackFederate::updateAttributeValues(ObjectInstanceHandle theObject, bool withTimeStamp)
{
  ////////////////////////////////////////////////
  /// create the necessary container and values
  ///////////////////////////////////////////////
  /// create the collection to store the values in, as you can see
  /// this is quite a lot of work
  AttributeHandleValueMap attributeValues;
  /// generate the new values
  for (auto attr : myPublishedAttributes)
  {
    wchar_t value[16];
    swprintf(value, 16, L"%s:%f", mRtiAmb->getAttributeName(myPublishedObjectClass, attr).c_str(), getLbts());
    attributeValues[attr] = toVariableLengthData(value);
  }
  ///////////////////////////
  /// do the actual update
  ///////////////////////////
  if (withTimeStamp)
  {
    std::unique_lock<std::mutex> lock(_federateTimeMutex);
    HLAfloat64Time time = getFederateTime() + _federateLookahead;
    {
      std::unique_lock<std::mutex> coutLock(_coutMutex);
      std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ": " << to_string(mRtiAmb->getObjectClassName(mRtiAmb->getKnownObjectClassHandle(theObject)))
                << ", time=" << time
                << std::endl;
    }
    mRtiAmb->updateAttributeValues(myPublishedObject, attributeValues, toVariableLengthData(L"hi!"), time);
  }
  else
  {
    {
      std::unique_lock<std::mutex> lock(_coutMutex);
      std::cout << "[TID=" << std::this_thread::get_id() << "] " << __FUNCTION__ << ": " << to_string(mRtiAmb->getObjectClassName(mRtiAmb->getKnownObjectClassHandle(theObject)))
                << std::endl;
    }
    mRtiAmb->updateAttributeValues(myPublishedObject, attributeValues, toVariableLengthData(L"hi!"));
  }
}

/**
 * This method will attempt to delete the object instance of the given
 * handle. We can only delete objects we created, or for which we own the
 * privilegeToDelete attribute.
 */
void ImmediateCallbackFederate::deleteObject(ObjectInstanceHandle objectHandle)
{
  mRtiAmb->deleteObjectInstance(objectHandle, toVariableLengthData(L""));
}
