
add_executable(TimeRegulatingFederate TimeRegulatingFederate.cpp main.cpp ../../include/RTI1516ESimpleTestLib.cpp)
target_link_libraries(TimeRegulatingFederate rti1516e fedtime1516e OpenRTI)
