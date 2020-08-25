
#include <iostream>
#include <memory.h>
#include "TimeConstrainedFederate.h"
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

using namespace rti1516e;
using std::cout;
using std::cin;
using std::wcout;
using std::endl;

TimeConstrainedFederate::TimeConstrainedFederate() : SimpleTestFederate()
{
}

TimeConstrainedFederate::~TimeConstrainedFederate()
{
}

void TimeConstrainedFederate::step()
{
  /// 9.1 update the attribute values of the instance
  //updateAttributeValues(myPublishedObject);
  /// 9.2 send an interaction
  sendInteraction();
  /// 9.3 request a time advance and wait until we get it
  advanceTime(1.0);
  //cout << "Time Advanced to " << fedamb->federateTime << endl;
}


void TimeConstrainedFederate::initializeSimulation()
{
  //InitializeObjects();
  waitForUser("before subscribe");
  InitializeInteraction();
  waitForUser("after subscribe");
  setNotificationHandle();
}


void TimeConstrainedFederate::InitializeInteraction()
{
  x_InteractionClass = mRtiAmb->getInteractionClassHandle(L"HLAinteractionRoot.X");
  xa_Parameter = mRtiAmb->getParameterHandle(x_InteractionClass, to_wstring("xa"));
  xb_Parameter = mRtiAmb->getParameterHandle(x_InteractionClass, to_wstring("xb"));

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
  rti1516e::ParameterHandleValueMap filter;
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
  mRtiAmb->setInteractionClassDeliverToSelf(this->x_InteractionClass, true);
}

void TimeConstrainedFederate::InitializeObjects()
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

void TimeConstrainedFederate::cleanupSimulation()
{
  deleteObject(myPublishedObject);
  printf("Deleted Object, handle=%ls", myPublishedObject.toString().c_str());
}

void TimeConstrainedFederate::sendInteraction()
{
  ////////////////////////////////////////////////
  /// create the necessary container and values
  ////////////////////////////////////////////////
  /// create the collection to store the values in
  ParameterHandleValueMap parameters;
  /// generate the new values
  wchar_t xaValue[16], xbValue[16];
  swprintf(xaValue, 16, L"xa:%f", getLbts());
  swprintf(xbValue, 16, L"xb:%f", getLbts());
  parameters[xa_Parameter] = toVariableLengthData(xaValue);
  parameters[xb_Parameter] = toVariableLengthData(xbValue);
  ///////////////////////////
  /// send the interaction
  ///////////////////////////
  //mRtiAmb->sendInteraction(xHandle, parameters, toVariableLengthData(L"hi!"));
  /// if you want to associate a particular timestamp with the
  /// interaction, you will have to supply it to the RTI. Here
  /// we send another interaction, this time with a timestamp:
  HLAfloat64Time time = federateTime + federateLookahead;
  mRtiAmb->sendInteraction(x_InteractionClass, parameters, toVariableLengthData(L"hi!"), time);
}

void TimeConstrainedFederate::updateAttributeValues(ObjectInstanceHandle theObject)
{
  std::wcout << __FUNCTIONW__ << L": " << mRtiAmb->getObjectClassName(mRtiAmb->getKnownObjectClassHandle(theObject)) << std::endl;
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
void TimeConstrainedFederate::deleteObject(ObjectInstanceHandle objectHandle)
{
  mRtiAmb->deleteObjectInstance(objectHandle, toVariableLengthData(L""));
}
