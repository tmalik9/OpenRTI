# Install script for directory: D:/Scratch/OpenRTI-0.9.0/src/rti1516e

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Scratch/openrti")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/Debug/fedtime1516ed.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/Release/fedtime1516e.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/MinSizeRel/fedtime1516e.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/RelWithDebInfo/fedtime1516e.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/Debug/libfedtime1516ed.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/Release/libfedtime1516e.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/MinSizeRel/libfedtime1516e.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/RelWithDebInfo/libfedtime1516e.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/Debug/rti1516ed.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/Release/rti1516e.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/MinSizeRel/rti1516e.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/RelWithDebInfo/rti1516e.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/Debug/librti1516ed.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/Release/librti1516e.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/MinSizeRel/librti1516e.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/RelWithDebInfo/librti1516e.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/rti1516e/RTI" TYPE FILE FILES
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/Enums.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/Exception.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/FederateAmbassador.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/Handle.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/LogicalTimeFactory.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/LogicalTime.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/LogicalTimeInterval.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/RangeBounds.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/NullFederateAmbassador.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/RTI1516.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/RTIambassadorFactory.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/RTIambassador.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/SpecificConfig.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/Typedefs.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/VariableLengthData.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/rti1516e/RTI/encoding" TYPE FILE FILES
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/encoding/BasicDataElements.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/encoding/DataElement.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/encoding/EncodingExceptions.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/encoding/EncodingConfig.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/encoding/HLAfixedArray.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/encoding/HLAfixedRecord.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/encoding/HLAopaqueData.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/encoding/HLAvariableArray.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/encoding/HLAvariantRecord.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/rti1516e/RTI/time" TYPE FILE FILES
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/time/HLAfloat64Interval.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/time/HLAfloat64TimeFactory.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/time/HLAfloat64Time.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/time/HLAinteger64Interval.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/time/HLAinteger64TimeFactory.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516e/RTI/time/HLAinteger64Time.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "D:/Scratch/openrti/share/OpenRTI/rti1516e/HLAstandardMIM.xml")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "D:/Scratch/openrti/share/OpenRTI/rti1516e" TYPE FILE FILES "D:/Scratch/OpenRTI-0.9.0/share/rti1516e/HLAstandardMIM.xml")
endif()

