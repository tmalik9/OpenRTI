/**
 * @file
 *
 * Copyright (C) 2013 Max Oberberger <max@oberbergers.de>
 *
 * Last modified: 2013 February 23, 20:05:02 by max
 *
 * This file is part of ba-hla.
 *
 * ba-hla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ba-hla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ba-hla.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXAMPLECPPFEDERATE_H_
#define EXAMPLECPPFEDERATE_H_

#include "TimeConstrainedAmb.h"
#include <RTI/RTIambassador.h>
#include <memory>

using namespace rti1516;

/**
 * Example Federate implementation
 */
class TimeConstrainedFederate
{
  public:
    std::unique_ptr<rti1516::RTIambassador> rtiamb; /*!< RTI ambassador to comunicate with RTI */
    TimeConstrainedFedAmb*      fedamb; /*!< Own Federate Ambassador implementation */

    // public methods //
    //! Constructor
    TimeConstrainedFederate();
    //! Destructor
    virtual ~TimeConstrainedFederate();
    //! Main Simulation Method
    void runFederate(std::string federateName, std::string fom, std::string address);

  private:
    /// fom handles
    ObjectClassHandle      aHandle;
    AttributeHandle        aaHandle;
    AttributeHandle        abHandle;
    AttributeHandle        acHandle;
    ObjectClassHandle      bHandle;
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

    std::wstring convertStringToWstring(const std::string& str);
    rti1516::VariableLengthData toVariableLengthData(const wchar_t* s);

    void initializeHandles();
    void waitForUser();
    void publishAndSubscribe();
    //ObjectInstanceHandle registerObject(const wchar_t* className);
    void updateAttributeValues(ObjectInstanceHandle objectHandle);
    void sendInteraction();
    void advanceTime(double timestep);
    void deleteObject(ObjectInstanceHandle objectHandle);

    double getFederateTime() const { return fedamb->federateTime; }
    double getLbts() const
    {
      return (fedamb->federateTime + fedamb->federateLookahead);
    }

};

#endif /* EXAMPLECPPFEDERATE_H_ */
