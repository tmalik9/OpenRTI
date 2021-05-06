
add_executable(TimeRegulatingFederate-1516ev TimeRegulatingFederate.cpp main.cpp ../../include/RTI1516ESimpleTestLib.cpp)
target_link_libraries(TimeRegulatingFederate-1516ev rti1516ev fedtime1516ev OpenRTI)
