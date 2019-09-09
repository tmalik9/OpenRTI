# CMake generated Testfile for 
# Source directory: D:/Scratch/OpenRTI-0.9.0/tests/OpenRTI/network
# Build directory: D:/Scratch/OpenRTI-0.9.0/tests/OpenRTI/network
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.

message(STATUS "CTEST_BINARY_DIRECTORY=${CTEST_BINARY_DIRECTORY} CTEST_SOURCE_DIRECTORY=${CTEST_SOURCE_DIRECTORY}")
add_test(OpenRTI/socketaddress  "${CTEST_BINARY_DIRECTORY}/socketaddress.exe")
add_test(OpenRTI/url "${CTEST_BINARY_DIRECTORY}/url.exe")
