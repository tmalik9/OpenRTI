
#pragma once

#include <RTI/NullFederateAmbassador.h>
#include <RTI/RTIambassador.h>
#include "RTI1516ESimpleTestLib.h"

using namespace rti1516e;


#define READY_TO_RUN L"ReadyToRun"
#define ALL_DONE L"AllDone"

class TimeConstrainedFedAmb : public SimpleTestAmbassador
{
  public:
    // methods //
    //! Constructor
    TimeConstrainedFedAmb(RTIambassador* rtiamb);
    //! Destructor
    virtual ~TimeConstrainedFedAmb() throw();

};
