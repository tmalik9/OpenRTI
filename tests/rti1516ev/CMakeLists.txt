# The current standard fdd used for testing
set(RTI1516E_FDD_FILE ${OPENRTI_CMAKEROOTDIR}/share/rti1516ev/HLAstandardMIM.xml)

# Copy to build dir that findHLAstandardMIMFileCandidates() works
file(COPY ${RTI1516E_FDD_FILE} DESTINATION "${PROJECT_BINARY_DIR}/share/OpenRTI/rti1516ev/")
message("Copy ${RTI1516E_FDD_FILE} to ${PROJECT_BINARY_DIR}/share/OpenRTI/rti1516ev/HLAstandardMIM.xml")


include_directories(${OPENRTI_CMAKEROOTDIR}/include/rti1516ev)
include_directories(${CMAKE_BINARY_DIR}/src/OpenRTI)
include_directories(${OPENRTI_CMAKEROOTDIR}/src/OpenRTI)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT OPENRTI_BUILD_SHARED)
  add_definitions(-DSTATIC_RTI)
  add_definitions(-DSTATIC_FEDTIME)
endif()

# Test directories should be listed here in sensible testing order
add_subdirectory(link)
add_subdirectory(connect)
add_subdirectory(encode)
add_subdirectory(fddget)
add_subdirectory(time)
add_subdirectory(objectinstance)
add_subdirectory(testfed)
add_subdirectory(interaction)
