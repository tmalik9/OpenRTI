
#pragma once

#include <RTI/RTIambassador.h>
#include <memory>
#include <windows.h>
#include "RTI1516ESimpleTestLib.h"

using namespace rti1516e;

/**
 * Example Federate implementation
 */
class TimeRegulatingFederate : public SimpleTestFederate
{

  public:
    TimeRegulatingFederate();
    virtual ~TimeRegulatingFederate();
    void step() override;
    void initializeSimulation() override;
    void cleanupSimulation() override;
  private:
    //ObjectInstanceHandle registerObject(const wchar_t* className);
    void sendInteraction();
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

