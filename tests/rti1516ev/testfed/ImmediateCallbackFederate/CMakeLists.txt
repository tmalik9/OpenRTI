
add_executable(ImmediateCallbackFederate-1516ev ImmediateCallbackFederate.cpp main.cpp RTI1516EImmediateCallbackTestLib.cpp)
target_link_libraries(ImmediateCallbackFederate-1516ev rti1516ev fedtime1516ev OpenRTI)
if(LINUX)
  target_link_libraries(ImmediateCallbackFederate-1516ev pthread)
endif()