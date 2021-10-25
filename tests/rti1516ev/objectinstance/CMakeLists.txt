
# Name reservation
add_executable(objectname-1516ev objectname.cpp)
target_link_libraries(objectname-1516ev rti1516ev fedtime1516ev OpenRTI)


# Object attribute updates
add_executable(objectinstance-1516ev objectinstance.cpp)
target_link_libraries(objectinstance-1516ev rti1516ev fedtime1516ev OpenRTI)

# No server - thread protocol, one ambassador
add_test(rti1516ev/objectname-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516ev" -S0 -A1 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# No server - thread protocol, 10 ambassadors
add_test(rti1516ev/objectname-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516ev" -S0 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, one ambassador
add_test(rti1516ev/objectname-1516ev-3 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516ev" -S1 -A1 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, 10 ambassadors
add_test(rti1516ev/objectname-1516ev-4 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516ev" -S1 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 5 servers - rti protocol, 10 ambassadors
#add_test(rti1516ev/objectname-1516ev-5 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516ev" -S5 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")

# No server - thread protocol, one ambassador
add_test(rti1516ev/objectinstance-1516ev-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516ev" -S0 -A1 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# No server - thread protocol, 10 ambassadors
add_test(rti1516ev/objectinstance-1516ev-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516ev" -S0 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, one ambassador
add_test(rti1516ev/objectinstance-1516ev-3 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516ev" -S1 -A1 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, 10 ambassadors
add_test(rti1516ev/objectinstance-1516ev-4 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516ev" -S1 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 5 servers - rti protocol, 10 ambassadors
#add_test(rti1516ev/objectinstance-1516ev-5 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516ev" -S5 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
