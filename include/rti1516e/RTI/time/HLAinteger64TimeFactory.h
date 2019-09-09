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
   File: RTI/time/HLAinteger64TimeFactory.h
***********************************************************************/

#ifndef RTI_HLAinteger64TimeFactory_H_
#define RTI_HLAinteger64TimeFactory_H_

#include <RTI/LogicalTimeFactory.h>
#include <RTI/encoding/EncodingConfig.h>


// Defines interface for HLAinteger64TimeFactory which presents an integer-based
// time /interval representation in the range 0 - 2^63-1.

namespace rti1516e
{
   const std::wstring HLAinteger64TimeName(L"HLAinteger64Time");

   class HLAinteger64Time;
   class HLAinteger64Interval;

   class RTI_EXPORT HLAinteger64TimeFactory : public rti1516e::LogicalTimeFactory
   {
   public:
      HLAinteger64TimeFactory ();

      virtual ~HLAinteger64TimeFactory ();

      // Return a LogicalTime with a value of "initial"
      virtual std::unique_ptr< LogicalTime > makeInitial();

      // Return a LogicalTime with a value of "final"
      virtual std::unique_ptr< LogicalTime > makeFinal();

      // Return a LogicalTimeInterval with a value of "zero"
      virtual std::unique_ptr< LogicalTimeInterval > makeZero();

      // Return a LogicalTimeInterval with a value of "epsilon"
      virtual std::unique_ptr< LogicalTimeInterval > makeEpsilon();

      virtual std::unique_ptr< HLAinteger64Time > makeLogicalTime (
         Integer64 value);

      virtual std::unique_ptr< HLAinteger64Interval > makeLogicalTimeInterval (
         Integer64 value);

      // LogicalTime decode from an encoded LogicalTime
      virtual std::unique_ptr< LogicalTime > decodeLogicalTime (
         VariableLengthData const & encodedLogicalTime);

      // Alternate LogicalTime decode that reads directly from a buffer
      virtual std::unique_ptr< LogicalTime > decodeLogicalTime (
         void* buffer,
         size_t bufferSize);

      // LogicalTimeInterval decode from an encoded LogicalTimeInterval
      virtual std::unique_ptr< LogicalTimeInterval > decodeLogicalTimeInterval (
         VariableLengthData const & encodedValue);

      // Alternate LogicalTimeInterval decode that reads directly from a buffer
      virtual std::unique_ptr< LogicalTimeInterval > decodeLogicalTimeInterval (
         void* buffer,
         size_t bufferSize);

      virtual std::wstring getName () const;
   };
}

#endif // RTI_HLAinteger64TimeFactory_H_

