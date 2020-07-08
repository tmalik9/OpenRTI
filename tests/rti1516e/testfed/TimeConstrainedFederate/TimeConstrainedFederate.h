
#pragma once

#include <RTI/RTIambassador.h>
#include <memory>
#include <windows.h>
#include "RTI1516ESimpleTestLib.h"
#include <atomic>

using namespace rti1516e;

/**
 * Example Federate implementation
 */
class TimeConstrainedFederate : public SimpleTestFederate
{
  public:
    TimeConstrainedFederate();
    virtual ~TimeConstrainedFederate();
    void step() override;
    void initializeSimulation() override;
    void cleanupSimulation() override;
  private:
    void sendInteraction();
    //ObjectInstanceHandle registerObject(const wchar_t* className);
    void updateAttributeValues(ObjectInstanceHandle objectHandle);
    void deleteObject(ObjectInstanceHandle objectHandle);

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
    InteractionClassHandle xHandle;
    ParameterHandle        xaHandle;
    ParameterHandle        xbHandle;

    // instance specific
    ObjectClassHandle      myPublishedObjectClass;
    ObjectInstanceHandle   myPublishedObject;
    AttributeHandleSet     myPublishedAttributes;

};

