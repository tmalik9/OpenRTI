
add_executable(TR_TC_Responder-1516ev TR_TC_Responder.cpp main.cpp ../../include/RTI1516ESimpleTestLib.cpp)
target_link_libraries(TR_TC_Responder-1516ev rti1516ev fedtime1516ev OpenRTI)
