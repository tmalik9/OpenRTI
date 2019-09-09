# CMake generated Testfile for 
# Source directory: D:/Scratch/OpenRTI-0.9.0/tests/rti1516/create
# Build directory: D:/Scratch/OpenRTI-0.9.0/tests/rti1516/create
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
message(STATUS "CTEST_BINARY_DIRECTORY=${CTEST_BINARY_DIRECTORY} CTEST_SOURCE_DIRECTORY=${CTEST_SOURCE_DIRECTORY}")
add_test(rti1516/create-1516-1 "${CTEST_BINARY_DIRECTORY}/create-1516" "-S0" "-A1" "-O" "${CTEST_SOURCE_DIRECTORY}/rti1516/fdd.xml")
add_test(rti1516/create-1516-2 "${CTEST_BINARY_DIRECTORY}/create-1516" "-S0" "-A10" "-O" "${CTEST_SOURCE_DIRECTORY}/rti1516/fdd.xml")
add_test(rti1516/create-1516-3 "${CTEST_BINARY_DIRECTORY}/create-1516" "-S1" "-A1" "-O" "${CTEST_SOURCE_DIRECTORY}/rti1516/fdd.xml")
add_test(rti1516/create-1516-4 "${CTEST_BINARY_DIRECTORY}/create-1516" "-S1" "-A10" "-O" "${CTEST_SOURCE_DIRECTORY}/rti1516/fdd.xml")
add_test(rti1516/create-1516-5 "${CTEST_BINARY_DIRECTORY}/create-1516" "-S5" "-A10" "-O" "${CTEST_SOURCE_DIRECTORY}/rti1516/fdd.xml")
add_test(rti1516/concurrent-create-1516-1 "${CTEST_BINARY_DIRECTORY}/concurrent-create-1516" "-S0" "-A10" "-O" "${CTEST_SOURCE_DIRECTORY}/rti1516/fdd.xml")
add_test(rti1516/concurrent-create-1516-2 "${CTEST_BINARY_DIRECTORY}/concurrent-create-1516" "-S1" "-A10" "-O" "${CTEST_SOURCE_DIRECTORY}/rti1516/fdd.xml")
add_test(rti1516/concurrent-create-1516-3 "${CTEST_BINARY_DIRECTORY}/concurrent-create-1516" "-S5" "-A10" "-O" "${CTEST_SOURCE_DIRECTORY}/rti1516/fdd.xml")
