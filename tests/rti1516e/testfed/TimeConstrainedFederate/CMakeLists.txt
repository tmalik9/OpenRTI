
add_executable(TimeConstrainedFederate TimeConstrainedFederate.cpp main.cpp ../../include/RTI1516ESimpleTestLib.cpp)
target_link_libraries(TimeConstrainedFederate rti1516e fedtime1516e OpenRTI)
