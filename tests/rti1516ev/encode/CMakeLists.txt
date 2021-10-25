add_executable(encode-1516ev encode.cpp)
if (UNIX)
target_link_libraries(encode-1516ev rti1516ev fedtime1516ev OpenRTI -lpthread)
else()
target_link_libraries(encode-1516ev rti1516ev fedtime1516ev OpenRTI)
endif()

add_test(rti1516ev/encode-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/encode-1516ev" -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd-1.xml")
