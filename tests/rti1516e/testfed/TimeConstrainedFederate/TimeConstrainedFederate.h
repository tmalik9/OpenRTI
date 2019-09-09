#pragma once

#include <RTI/RTIambassador.h>
#include <memory>
#include <windows.h>
#include "RTI1516ESimpleTestLib.h"

using namespace rti1516e;

/**
 * Example Federate implementation
 */
class TimeConstrainedFederate : public SimpleTestFederate
{
  public:
    // public methods //
    //! Constructor
    TimeConstrainedFederate();
    //! Destructor
    virtual ~TimeConstrainedFederate();
    //! Main Simulation Method
    void runFederate(unsigned int iterations);

  private:

    // instance specific
    ObjectClassHandle      myPublishedObjectClass;
    ObjectInstanceHandle   myPublishedObject;
    AttributeHandleSet     myPublishedAttributes;

    void publishAndSubscribe();
    //ObjectInstanceHandle registerObject(const wchar_t* className);
    void updateAttributeValues(ObjectInstanceHandle objectHandle);
    void deleteObject(ObjectInstanceHandle objectHandle);
};

