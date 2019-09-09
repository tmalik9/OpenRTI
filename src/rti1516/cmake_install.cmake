# Install script for directory: D:/Scratch/OpenRTI-0.9.0/src/rti1516

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/Debug/fedtime1516d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/Release/fedtime1516.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/MinSizeRel/fedtime1516.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/RelWithDebInfo/fedtime1516.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/Debug/fedtime1516d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/Release/fedtime1516.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/MinSizeRel/fedtime1516.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/RelWithDebInfo/fedtime1516.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/Debug/rti1516d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/Release/rti1516.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/MinSizeRel/rti1516.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Scratch/OpenRTI-0.9.0/lib/RelWithDebInfo/rti1516.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/Debug/rti1516d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/Release/rti1516.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/MinSizeRel/rti1516.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Scratch/OpenRTI-0.9.0/bin/RelWithDebInfo/rti1516.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/rti1516/RTI" TYPE FILE FILES
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/Enums.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/FederateAmbassador.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/LogicalTimeFactory.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/LogicalTimeInterval.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/RangeBounds.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/RTIambassadorFactory.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/SpecificConfig.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/VariableLengthData.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/Exception.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/Handle.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/LogicalTime.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/NullFederateAmbassador.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/RTI1516.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/RTIambassador.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/Typedefs.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/HLAfloat64Interval.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/HLAfloat64TimeFactory.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/HLAfloat64Time.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/HLAinteger64Interval.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/HLAinteger64TimeFactory.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/HLAinteger64Time.h"
    "D:/Scratch/OpenRTI-0.9.0/include/rti1516/RTI/RTI1516fedTime.h"
    )
endif()

