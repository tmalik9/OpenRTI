add_executable(encode-1516e encode.cpp)
if (UNIX)
target_link_libraries(encode-1516e rti1516e fedtime1516e OpenRTI -lpthread)
else()
target_link_libraries(encode-1516e rti1516e fedtime1516e OpenRTI)
endif()

add_test(rti1516e/encode-1516e-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/encode-1516e" -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd-1.xml")
