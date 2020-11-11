
#pragma once

#include <iosfwd>
#include <string>
#include "Export.h"
#include "Message.h"
#include "StringUtils.h"

namespace OpenRTI {

template<typename T>
inline T from_string(const std::string& str)
{
  std::istringstream ss(str);
  T ret;
  ss >> ret;
  return ret;
}

template<>
inline Endianness from_string<Endianness>(const std::string& str)
{
  if (caseInSensitiveStringEqual(str, "little"))
    return Endianness::LittleEndian;
  else
    return Endianness::BigEndian;
}

template<>
inline ArrayDataTypeEncoding from_string<ArrayDataTypeEncoding>(const std::string& str)
{
  if (caseInSensitiveStringEqual(str, "HLAfixedArray"))
    return ArrayDataTypeEncoding::FixedArrayDataTypeEncoding;
  else
    return ArrayDataTypeEncoding::VariableArrayDataTypeEncoding;
}

static inline std::string to_string(ArrayDataTypeEncoding value)
{
  if (value == ArrayDataTypeEncoding::FixedArrayDataTypeEncoding)
    return "HLAfixedArray";
  else
    return "HLAVariableArray";
}

class OPENRTI_API TranslateTypes
{
public:
  TranslateTypes();
  ~TranslateTypes();
  static FOMStringModule translate(const FOMStringModule2& ref);
  static FOMStringModuleList translate(const FOMStringModule2List& ref);
  static FOMStringArrayDataType translate(const FOMStringArrayDataType2& ref);
  static FOMStringArrayDataTypeList translate(const FOMStringArrayDataType2List& ref);
};

}