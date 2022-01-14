
add_executable(TimeConstrainedFederate-1516ev TimeConstrainedFederate.cpp main.cpp ../../include/RTI1516ESimpleTestLib.cpp)
target_link_libraries(TimeConstrainedFederate-1516ev rti1516ev fedtime1516ev OpenRTI)
