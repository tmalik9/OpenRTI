/***********************************************************************
   The IEEE hereby grants a general, royalty-free license to copy, distribute,
   display and make derivative works from this material, for all purposes,
   provided that any use of the material contains the following
   attribution: "Reprinted with permission from IEEE 1516.1(TM)-2010".
   Should you require additional information, contact the Manager, Standards
   Intellectual Property, IEEE Standards Association (stds-ipr@ieee.org).
***********************************************************************/
/***********************************************************************
   IEEE 1516.1 High Level Architecture Interface Specification C++ API
   File: RTI/RTIambassadorFactory.h
***********************************************************************/

#ifndef RTI_RTIambassadorFactory_h
#define RTI_RTIambassadorFactory_h

namespace rti1516ev
{
   class RTIambassador;
}

#include <RTI/SpecificConfig.h>
#include <RTI/Exception.h>
#include <vector>
#include <string>
#include <memory>

namespace rti1516ev
{
   class RTI_EXPORT RTIambassadorFactory
   {
   public:
      RTIambassadorFactory() noexcept;

      virtual ~RTIambassadorFactory() noexcept;

      // 10.35
      std::unique_ptr< RTIambassador > createRTIambassador ();
   };
}

#endif // RTI_RTIambassadorFactory_h
