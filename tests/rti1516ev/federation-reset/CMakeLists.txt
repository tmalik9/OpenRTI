# Query something from an fdd
add_executable(federation-reset-1516ev federation-reset.cpp)
target_link_libraries(federation-reset-1516ev rti1516ev fedtime1516ev OpenRTI)

# No server - thread protocol, one ambassador
add_test(rti1516ev/federation-reset-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S0 -A1 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# No server - thread protocol, 2 ambassadors
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S0 -A2 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# No server - thread protocol, 2 ambassadors
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S0 -A3 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# No server - thread protocol, 1 ambassador, requestor aborts resets
add_test(rti1516ev/federation-reset-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S0 -A1 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" requestorAbortsReset)
# No server - thread protocol, 2 ambassadors , requestor aborts resets
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S0 -A2 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" requestorAbortsReset)
# No server - thread protocol, 2 ambassadors, requestor aborts resets
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S0 -A3 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" requestorAbortsReset)
# No server - thread protocol, 2 ambassadors, responders abort resets
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S0 -A2 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" responderAbortsReset)
# No server - thread protocol, 2 ambassadors, responders abort resets
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S0 -A3 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" responderAbortsReset)

# 1 server - thread protocol, one ambassador
add_test(rti1516ev/federation-reset-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S1 -A1 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - thread protocol, 2 ambassadors
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S1 -A2 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - thread protocol, 2 ambassadors
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S1 -A3 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - thread protocol, 1 ambassador, requestor aborts resets
add_test(rti1516ev/federation-reset-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S1 -A1 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" requestorAbortsReset)
# 1 server - thread protocol, 2 ambassadors , requestor aborts resets
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S1 -A2 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" requestorAbortsReset)
# 1 server - thread protocol, 3 ambassadors, requestor aborts resets
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S1 -A3 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" requestorAbortsReset)
# 1 server - thread protocol, 2 ambassadors, responders abort resets
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S1 -A2 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" responderAbortsReset)
# 1 server - thread protocol, 3 ambassadors, responders abort resets
add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S1 -A3 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" responderAbortsReset)

# 2 servers - thread protocol, 5 ambassadors
#add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S2 -A5 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 2 servers - thread protocol, 5 ambassadors, requestor aborts resets
#add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S2 -A5 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" requestorAbortsReset)
# 2 servers - thread protocol, 5 ambassadors, requestor aborts resets
#add_test(rti1516ev/federation-reset-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/federation-reset-1516ev" -S2 -A5 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml" responderAbortsReset)
