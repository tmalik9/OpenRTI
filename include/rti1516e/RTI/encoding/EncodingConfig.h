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
  File: EncodingConfig.h
***********************************************************************/
#ifndef RTI_EncodingConfig_h_
#define RTI_EncodingConfig_h_

#include <utility>
#include <vector>
#include <cstdint>


namespace rti1516e
{
   // Platform Specific Typedefs
#if defined(_WIN32)
   typedef int8_t      Integer8;
   typedef int16_t     Integer16;
   typedef int32_t     Integer32;
   typedef int64_t     Integer64;
#else
#if defined(RTI_USE_64BIT_LONGS)
   typedef char      Integer8;
   typedef short     Integer16;
   typedef int       Integer32;
   typedef long      Integer64;
#else
   typedef char      Integer8;
   typedef short     Integer16;
   typedef int       Integer32;
   typedef long long Integer64;
#endif
#endif

   typedef Integer8  Octet;
   typedef std::pair< Octet, Octet > OctetPair;
}

#endif // RTI_EncodingConfig_h_


