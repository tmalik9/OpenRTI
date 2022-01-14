# The current standard fdd used for testing
file(COPY testfom.xml DESTINATION "${PROJECT_BINARY_DIR}/bin")
message("Copy testfom.xml to ${PROJECT_BINARY_DIR}/bin")


include_directories(${OPENRTI_CMAKEROOTDIR}/include/rti1516ev)
include_directories(${CMAKE_BINARY_DIR}/src/OpenRTI)
include_directories(${OPENRTI_CMAKEROOTDIR}/src/OpenRTI)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT OPENRTI_BUILD_SHARED)
  add_definitions(-DSTATIC_RTI)
  add_definitions(-DSTATIC_FEDTIME)
endif()

add_subdirectory(TimeConstrainedFederate)
add_subdirectory(TimeRegulatingFederate)
add_subdirectory(ImmediateCallbackFederate)
add_subdirectory(TR_TC_Responder)
