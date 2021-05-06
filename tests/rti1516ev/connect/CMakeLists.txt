# Query something from an fdd
add_executable(connect-1516ev connect.cpp)
target_link_libraries(connect-1516ev rti1516ev fedtime1516ev OpenRTI)

# Run tests with failing connects
add_test(rti1516/connect-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/connect-1516ev" -O "${RTI1516_FDD_FILE}")
