# CMake generated Testfile for 
# Source directory: D:/Scratch/OpenRTI-0.9.0/tests/rti1516/connect
# Build directory: D:/Scratch/OpenRTI-0.9.0/tests/rti1516/connect
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.

# SET(CTEST_BINARY_DIRECTORY "D:/vfs/CANoe/private/mth/12.0_DistSim_HLA/bin/Exec32_Debug")
add_test(rti1516/connect-1516-1 "${CTEST_BINARY_DIRECTORY}/connect-1516" "-O" "${CTEST_SOURCE_DIRECTORY}/rti1516/fdd.xml")
