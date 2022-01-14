
#pragma once

#include <RTI/RTIambassador.h>
#include <memory>
#include "RTI1516EImmediateCallbackTestLib.h"
#include <atomic>
#include <thread>
#include <list>
#include <chrono>

using namespace rti1516ev;

/**
 * Example Federate implementation
 */
class ImmediateCallbackFederate : public ImmediateCallbackTestFederate
{
  public:
    ImmediateCallbackFederate();
    virtual ~ImmediateCallbackFederate();
    void step() override;
    void initializeSimulation() override;
    void joined() override;
    void InitializeInteraction();

    void InitializeObjects();

    void cleanupSimulation() override;
  private:
    void sendInteraction(bool withTimeStamp);
    //ObjectInstanceHandle registerObject(const wchar_t* className);
    void updateAttributeValues(ObjectInstanceHandle objectHandle, bool withTimeStamp);
    void deleteObject(ObjectInstanceHandle objectHandle);

    std::list<std::thread> sendThreads;
    /// fom handles
    std::map<std::string, ObjectClassHandle> mObjectClassHandles;
    std::map<std::string, std::map<std::string, AttributeHandle> > mAttributeHandles;
    //ObjectClassHandle      aHandle;
    AttributeHandle        aaHandle;
    AttributeHandle        abHandle;
    AttributeHandle        acHandle;
    //ObjectClassHandle      bHandle;
    AttributeHandle        baHandle;
    AttributeHandle        bbHandle;
    AttributeHandle        bcHandle;

    InteractionClassHandle x_InteractionClass;
    ParameterHandle        xa_Parameter;
    ParameterHandle        xb_Parameter;
    ParameterHandle        xc_Parameter;

    // instance specific
    ObjectClassHandle      myPublishedObjectClass;
    ObjectInstanceHandle   myPublishedObject;
    AttributeHandleSet     myPublishedAttributes;

};

