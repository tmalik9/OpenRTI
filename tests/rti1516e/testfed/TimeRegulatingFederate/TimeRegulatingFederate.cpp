
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

using namespace rti1516e;
using std::cout;
using std::cin;
using std::wcout;
using std::endl;

TimeRegulatingFederate::TimeRegulatingFederate() : SimpleTestFederate()
{
}

TimeRegulatingFederate::~TimeRegulatingFederate()
{
}

/**
 * Everything will be handled here
 *
 * @param federateName How the federate is called in simulation
 * @return void
 */
void TimeRegulatingFederate::runFederate(unsigned int iterations)
{
  myPublishedObjectClass = rtiamb->getObjectClassHandle(L"HLAobjectRoot.A");

  //////////////////////////////
  // 7. publish and subscribe
  //////////////////////////////
  // in this section we tell the RTI of all the data we are going to
  // produce, and all the data we want to know about
  publishAndSubscribe();
  cout << "Published and Subscribed" << endl;
  /////////////////////////////////////
  // 8. register an object to update
  /////////////////////////////////////
  myPublishedObject = rtiamb->registerObjectInstance(myPublishedObjectClass);
  wcout << L"Registered Object, handle=" << myPublishedObject << L" class=" << rtiamb->getObjectClassName(rtiamb->getKnownObjectClassHandle(myPublishedObject)) << endl;
  ////////////////////////////////////
  // 9. do the main simulation loop
  ////////////////////////////////////
  // here is where we do the meat of our work. in each iteration, we will
  // update the attribute values of the object we registered, and will
  // send an interaction.
  rtiamb->setNotificationHandle(mHandle.get());
  DebugPrintf("%s: TID=%d: starting loop\n", __FUNCTION__, ::GetCurrentThreadId());
  for (unsigned int i = 0; i < iterations; i++)
  {
    /// 9.1 update the attribute values of the instance
    updateAttributeValues(myPublishedObject);
    /// 9.2 send an interaction
    sendInteraction();
    /// 9.3 request a time advance and wait until we get it
    advanceTime(1.0);
    //cout << "Time Advanced to " << fedamb->federateTime << endl;
  }

  rtiamb->synchronizationPointAchieved(ALL_DONE);
  while (fedamb->allDone == false)
  {
    rtiamb->evokeCallback(12.0);
  }
  //////////////////////////////////////
  // 10. delete the object we created
  //////////////////////////////////////
  deleteObject(myPublishedObject);
  wcout << "Deleted Object, handle=" << myPublishedObject << endl;
  ////////////////////////////////////
  // 11. resign from the federation
  ////////////////////////////////////
  rtiamb->resignFederationExecution(NO_ACTION);
  cout << "Resigned from Federation" << endl;
  ////////////////////////////////////////
  // 12. try and destroy the federation
  ////////////////////////////////////////
  // NOTE: we won't die if we can't do this because other federates
  //       remain. in that case we'll leave it for them to clean up
  try
  {
    rtiamb->destroyFederationExecution(L"ExampleFederation");
    cout << "Destroyed Federation" << endl;
  }
  catch (FederationExecutionDoesNotExist dne)
  {
    cout << "No need to destroy federation, it doesn't exist" << endl;
  }
  catch (FederatesCurrentlyJoined fcj)
  {
    cout << "Didn't destroy federation, federates still joined" << endl;
  }
  rtiamb->disconnect();
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Helper Methods ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * This method will inform the RTI about the types of data that the federate will
 * be creating, and the types of data we are interested in hearing about as other
 * federates produce it.
 */
void TimeRegulatingFederate::publishAndSubscribe()
{
  /////////////////////////////////////////////
  /// publish all attributes of ObjectRoot.A
  /////////////////////////////////////////////
  /// before we can register instance of the object class ObjectRoot.A and
  /// update the values of the various attributes, we need to tell the RTI
  /// that we intend to publish this information\n
  /// package the information into a handle set
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
  rtiamb->publishObjectClassAttributes(myPublishedObjectClass, myPublishedAttributes);

  //////////////////////////////////////////////////
  /// subscribe to all attributes of ObjectRoot.A
  //////////////////////////////////////////////////
  /// we also want to hear about the same sort of information as it is
  /// created and altered in other federates, so we need to subscribe to it
  rtiamb->subscribeObjectClassAttributes(mObjectClassHandles["HLAobjectRoot.A"], attributesOfA, true);
  rtiamb->subscribeObjectClassAttributes(mObjectClassHandles["HLAobjectRoot.B"], attributesOfB, true);
  //////////////////////////////////////////////////////
  /// publish the interaction class InteractionRoot.X
  //////////////////////////////////////////////////////
  /// we want to send interactions of type InteractionRoot.X, so we need
  /// to tell the RTI that we're publishing it first. We don't need to
  /// inform it of the parameters, only the class, making it much simpler
  /// do the publication
  rtiamb->publishInteractionClass(this->xHandle);
  /////////////////////////////////////////////////////
  /// subscribe to the InteractionRoot.X interaction //
  /////////////////////////////////////////////////////
  /// we also want to receive other interaction of the same type that are
  /// sent out by other federates, so we have to subscribe to it first
  rtiamb->subscribeInteractionClass(this->xHandle);
}

/**
 * This method will register an instance of the class ObjectRoot.A and will
 * return the federation-wide unique handle for that instance. Later in the
 * simulation, we will update the attribute values for this instance
 */
//ObjectInstanceHandle ExampleCPPFederate::registerObject(ObjectClassHandle theObjectClass)
//{
//  myPublishedObject = rtiamb->registerObjectInstance(theObjectClass);
//  return myPublishedObject;
//}

/**
 * This method will update all the values of the given object instance. It will
 * set each of the values to be a string which is equal to the name of the
 * attribute plus the current time. eg "aa:10.0" if the time is 10.0.
 * <p/>
 * Note that we don't actually have to update all the attributes at once, we
 * could update them individually, in groups or not at all!
 */
void TimeRegulatingFederate::updateAttributeValues(ObjectInstanceHandle theObject)
{
  std::wcout << __FUNCTIONW__ << L": " << rtiamb->getObjectClassName(rtiamb->getKnownObjectClassHandle(theObject)) << std::endl;
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
    swprintf(value, 16, L"%s:%f", rtiamb->getAttributeName(myPublishedObjectClass, attr).c_str(), getLbts());
    attributeValues[attr] = toVariableLengthData(value);
  }
  ///////////////////////////
  /// do the actual update
  ///////////////////////////
  //rtiamb->updateAttributeValues(myPublishedObject, attributeValues, toVariableLengthData(L"hi!"));
  /// note that if you want to associate a particular timestamp with the
  /// update. here we send another update, this time with a timestamp:
  HLAfloat64Time time = fedamb->federateTime + fedamb->federateLookahead;
  rtiamb->updateAttributeValues(myPublishedObject, attributeValues, toVariableLengthData(L"hi!"), time);
}

/**
 * This method will attempt to delete the object instance of the given
 * handle. We can only delete objects we created, or for which we own the
 * privilegeToDelete attribute.
 */
void TimeRegulatingFederate::deleteObject(ObjectInstanceHandle objectHandle)
{
  rtiamb->deleteObjectInstance(objectHandle, toVariableLengthData(L""));
}
