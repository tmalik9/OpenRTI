
#pragma once

#include <RTI/RTIambassador.h>
#include <memory>
#include "RTI1516ESimpleTestLib.h"

using namespace rti1516ev;

/**
 * Example Federate implementation
 */
class ResponderFederate : public SimpleTestFederate
{

  public:
    ResponderFederate();
    virtual ~ResponderFederate();
    void step() override;
    void initializeSimulation() override;

    void InitializeInteraction();

    void InitializeObjects();

    void cleanupSimulation() override;
    virtual void receiveInteraction(
      InteractionClassHandle theInteraction,
      ParameterHandleValueMap const& theParameterValues,
      VariableLengthData const& theUserSuppliedTag,
      OrderType sentOrder,
      TransportationType theType,
      LogicalTime const& theTime,
      OrderType receivedOrder,
      SupplementalReceiveInfo theReceiveInfo) override;

  private:
    //ObjectInstanceHandle registerObject(const wchar_t* className);
    void sendInteraction(HLAfloat64Time time);
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

    InteractionClassHandle x_InteractionClass;
    ParameterHandle        xa_Parameter;
    ParameterHandle        xb_Parameter;
    ParameterHandle        xc_Parameter;

    // instance specific
    ObjectClassHandle      myPublishedObjectClass;
    ObjectInstanceHandle   myPublishedObject;
    AttributeHandleSet     myPublishedAttributes;

};

