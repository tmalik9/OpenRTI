include_directories(${CMAKE_BINARY_DIR}/src/OpenRTI)
include_directories(${OPENRTI_CMAKEROOTDIR}/src/OpenRTI)

add_executable(abstimeout abstimeouttest.cpp)
target_link_libraries(abstimeout OpenRTI)

add_test(OpenRTI/abstimeout-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/abstimeout")

