# Just for propper recursion
add_subdirectory(OpenRTI)

option(OPENRTI_BUILD_RTINODE "Build the rtinode" ON)
if (OPENRTI_BUILD_RTINODE)
	add_subdirectory(rtinode)
endif()

option(OPENRTI_BUILD_TOOLS "Build additional tools (fom2cpp, FOMCodeGen)" ON)
if (OPENRTI_BUILD_TOOLS)
	add_subdirectory(Tools)
endif()

if(OPENRTI_ENABLE_RTI13)
  if(NOT WIN32 OR OPENRTI_BUILD_SHARED)
    add_subdirectory(RTI13)
  endif()
endif()
if(OPENRTI_ENABLE_RTI1516)
  add_subdirectory(rti1516)
endif()
if(OPENRTI_ENABLE_RTI1516E)
  add_subdirectory(rti1516e)
endif()
