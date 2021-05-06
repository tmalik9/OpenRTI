add_executable(logical-time-1516ev logical-time.cpp)
target_link_libraries(logical-time-1516ev fedtime1516ev OpenRTI)

# Test the different logical times by getting the values form the logical time factory
add_test(rti1516ev/logical-time-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/logical-time-1516ev")
add_test(rti1516ev/logical-time-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/logical-time-1516ev" -F HLAfloat64Time)
add_test(rti1516ev/logical-time-1516ev-3 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/logical-time-1516ev" -F HLAinteger64Time)
