
#include <iostream>
#include <memory.h>
#include "TimeRegulatingFederate.h"
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

using namespace rti1516ev;
using std::cout;
using std::cin;
using std::wcout;
using std::endl;

TimeRegulatingFederate::TimeRegulatingFederate() : SimpleTestFederate()
{
  setPrintVerbose(false);
}

TimeRegulatingFederate::~TimeRegulatingFederate()
{
}


void TimeRegulatingFederate::step()
{
  /// update the attribute values of the instance
  //updateAttributeValues(myPublishedObject);
  /// send an interaction
  sendInteraction();
  /// request a time advance
  advanceTime(std::chrono::duration<double>(getStepDuration()).count());
  //cout << "Time Advanced to " << fedamb->federateTime << endl;
}

void TimeRegulatingFederate::initializeSimulation()
{
  //InitializeObjects();
  InitializeInteraction();
#ifdef _WIN32
  setNotificationHandle();
#endif
}


void TimeRegulatingFederate::InitializeInteraction()
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
  printf("publish interaction %ls\n", mRtiAmb->getInteractionClassName(this->x_InteractionClass).c_str());
  /////////////////////////////////////////////////////
  /// subscribe to the InteractionRoot.X interaction //
  /////////////////////////////////////////////////////
  /// we also want to receive other interaction of the same type that are
  /// sent out by other federates, so we have to subscribe to it first
  mRtiAmb->subscribeInteractionClass(this->x_InteractionClass);
  //mRtiAmb->setInteractionClassDeliverToSelf(this->x_InteractionClass, true);
}

void TimeRegulatingFederate::InitializeObjects()
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
  myPublishedObjectClass = mObjectClassHandles["HLAobjectRoot.A"];
  myPublishedAttributes = attributesOfA;
  mRtiAmb->publishObjectClassAttributes(myPublishedObjectClass, myPublishedAttributes);

  //////////////////////////////////////////////////
  /// subscribe to all attributes of ObjectRoot.A
  //////////////////////////////////////////////////
  /// we also want to hear about the same sort of information as it is
  /// created and altered in other federates, so we need to subscribe to it
  mRtiAmb->subscribeObjectClassAttributes(mObjectClassHandles["HLAobjectRoot.A"], attributesOfA, true);
  mRtiAmb->subscribeObjectClassAttributes(mObjectClassHandles["HLAobjectRoot.B"], attributesOfB, true);
  myPublishedObject = mRtiAmb->registerObjectInstance(myPublishedObjectClass);
  printf("Registered Object, handle=%ls class==%ls", myPublishedObject.toString().c_str(), mRtiAmb->getObjectClassName(mRtiAmb->getKnownObjectClassHandle(myPublishedObject)).c_str());
}

void TimeRegulatingFederate::cleanupSimulation()
{
  if (myPublishedObject.isValid())
  {
    deleteObject(myPublishedObject);
    printf("Deleted Object, handle=%ls", myPublishedObject.toString().c_str());
  }
}

void TimeRegulatingFederate::sendInteraction()
{
  ////////////////////////////////////////////////
  /// create the necessary container and values
  ////////////////////////////////////////////////
  /// create the collection to store the values in
  ParameterHandleValueMap parameters;
  /// generate the new values
  //wchar_t xaValue[16], xbValue[16];
  wchar_t xbValue[16];
  unsigned char data[1024];
  for (size_t i = 0; i < sizeof(data); i++)
  {
    data[i] = i % 256;
  }
  //swprintf(xaValue, 16, L"xa:%f", getLbts());
  swprintf(xbValue, 16, L"xb:%f", getLbts());
  //parameters[xaHandle] = xaEncoded.encode();
  parameters[xa_Parameter] = HLAunicodeString(L"test1").encode();
  parameters[xb_Parameter] = HLAunicodeString(xbValue).encode();
  parameters[xc_Parameter] = HLAopaqueData(data, sizeof(data)).encode();
  ///////////////////////////
  /// send the interaction
  ///////////////////////////
  //mRtiAmb->sendInteraction(xHandle, parameters, toVariableLengthData(L"hi!"));
  /// if you want to associate a particular timestamp with the
  /// interaction, you will have to supply it to the RTI. Here
  /// we send another interaction, this time with a timestamp:
  HLAfloat64Time time = federateTime + federateLookahead;
  VariableLengthData tag;
  mRtiAmb->sendInteraction(x_InteractionClass, parameters, tag, time);
}

void TimeRegulatingFederate::updateAttributeValues(ObjectInstanceHandle theObject)
{
  std::cout << __FUNCTION__ << ": " << to_string(mRtiAmb->getObjectClassName(mRtiAmb->getKnownObjectClassHandle(theObject))) << std::endl;
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
  //rtiamb->updateAttributeValues(myPublishedObject, attributeValues, toVariableLengthData(L"hi!"));
  /// note that if you want to associate a particular timestamp with the
  /// update. here we send another update, this time with a timestamp:
  HLAfloat64Time time = federateTime + federateLookahead;
  mRtiAmb->updateAttributeValues(myPublishedObject, attributeValues, toVariableLengthData(L"hi!"), time);
}

/**
 * This method will attempt to delete the object instance of the given
 * handle. We can only delete objects we created, or for which we own the
 * privilegeToDelete attribute.
 */
void TimeRegulatingFederate::deleteObject(ObjectInstanceHandle objectHandle)
{
  mRtiAmb->deleteObjectInstance(objectHandle, toVariableLengthData(L""));
}
