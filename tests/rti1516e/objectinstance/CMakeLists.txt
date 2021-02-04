
# Name reservation
add_executable(objectname-1516e objectname.cpp)
target_link_libraries(objectname-1516e rti1516e fedtime1516e OpenRTI)


# Object attribute updates
add_executable(objectinstance-1516e objectinstance.cpp)
target_link_libraries(objectinstance-1516e rti1516e fedtime1516e OpenRTI)

# No server - thread protocol, one ambassador
add_test(rti1516e/objectname-1516e-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516e" -S0 -A1 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# No server - thread protocol, 10 ambassadors
add_test(rti1516e/objectname-1516e-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516e" -S0 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, one ambassador
add_test(rti1516e/objectname-1516e-3 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516e" -S1 -A1 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, 10 ambassadors
add_test(rti1516e/objectname-1516e-4 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516e" -S1 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 5 servers - rti protocol, 10 ambassadors
#add_test(rti1516e/objectname-1516e-5 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectname-1516e" -S5 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")

# No server - thread protocol, one ambassador
add_test(rti1516e/objectinstance-1516e-1 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516e" -S0 -A1 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# No server - thread protocol, 10 ambassadors
add_test(rti1516e/objectinstance-1516e-2 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516e" -S0 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, one ambassador
add_test(rti1516e/objectinstance-1516e-3 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516e" -S1 -A1 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 1 server - rti protocol, 10 ambassadors
add_test(rti1516e/objectinstance-1516e-4 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516e" -S1 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
# 5 servers - rti protocol, 10 ambassadors
#add_test(rti1516e/objectinstance-1516e-5 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/objectinstance-1516e" -S5 -A10 -J -O "${CMAKE_CURRENT_SOURCE_DIR}/fdd.xml")
