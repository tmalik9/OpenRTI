# Query something from an fdd
add_executable(interaction-1516ev interaction.cpp)
target_link_libraries(interaction-1516ev rti1516ev fedtime1516ev OpenRTI)

# No server - thread protocol, one ambassador
add_test(rti1516ev/interaction-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/interaction-1516ev" -S0 -A1 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# No server - thread protocol, two ambassadors
add_test(rti1516ev/interaction-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/interaction-1516ev" -S0 -A2 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# No server - thread protocol, 10 ambassadors
add_test(rti1516ev/interaction-1516ev-3 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/interaction-1516ev" -S0 -A10 -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, one ambassador
add_test(rti1516ev/interaction-1516ev-4 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/interaction-1516ev" -S1 -A1 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, 10 ambassadors
add_test(rti1516ev/interaction-1516ev-5 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/interaction-1516ev" -S1 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 5 servers - rti protocol, 10 ambassadors
add_test(rti1516ev/interaction-1516ev-6 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/interaction-1516ev" -S5 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
