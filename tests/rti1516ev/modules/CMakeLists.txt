add_executable(modules-1516ev modules.cpp)
target_link_libraries(modules-1516ev rti1516ev fedtime1516ev OpenRTI)

# # No server - thread protocol, one ambassador
# add_test(rti1516ev/modules-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S0 -A1 -D -O "${CMAKE_CURRENT_SOURCE_DIR}")
# # No server - thread protocol, 10 ambassadors
# add_test(rti1516ev/modules-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S0 -A10 -D -O "${CMAKE_CURRENT_SOURCE_DIR}")
# # 1 server - rti protocol, one ambassador
# add_test(rti1516ev/modules-1516ev-3 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S1 -A1 -D -O "${CMAKE_CURRENT_SOURCE_DIR}")
# # 1 server - rti protocol, 10 ambassadors
# add_test(rti1516ev/modules-1516ev-4 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S1 -A10 -D -O "${CMAKE_CURRENT_SOURCE_DIR}")
# 5 servers - rti protocol, 10 ambassadors
# REVIEW: add_test(rti1516ev/modules-1516ev-5 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S5 -A10 -D -O "${CMAKE_CURRENT_SOURCE_DIR}")

# No server - thread protocol, one ambassador
add_test(rti1516ev/modules-1516ev-6 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S0 -A1 -O "${CMAKE_CURRENT_SOURCE_DIR}")
# No server - thread protocol, 10 ambassadors
# REVIEW: add_test(rti1516ev/modules-1516ev-7 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S0 -A10 -O "${CMAKE_CURRENT_SOURCE_DIR}")
# 1 server - rti protocol, one ambassador
add_test(rti1516ev/modules-1516ev-8 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S1 -A1 -O "${CMAKE_CURRENT_SOURCE_DIR}")
# 1 server - rti protocol, 10 ambassadors
# REVIEW: add_test(rti1516ev/modules-1516ev-9 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S1 -A10 -O "${CMAKE_CURRENT_SOURCE_DIR}")
# 5 servers - rti protocol, 10 ambassadors
# REVIEW: add_test(rti1516ev/modules-1516ev-10 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/modules-1516ev" -S5 -A10 -O "${CMAKE_CURRENT_SOURCE_DIR}")
