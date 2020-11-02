include_directories(${CMAKE_BINARY_DIR}/src/OpenRTI)
include_directories(${OPENRTI_CMAKEROOTDIR}/src/OpenRTI)

add_executable(InteractionFilters InteractionFilters.cpp 
${OPENRTI_CMAKEROOTDIR}/src/OpenRTI/AbstractMessage.cpp 
${OPENRTI_CMAKEROOTDIR}/src/OpenRTI/dprintf.cpp ${OPENRTI_CMAKEROOTDIR}/src/OpenRTI/Message.cpp 
${OPENRTI_CMAKEROOTDIR}/src/OpenRTI/ServerModel.cpp 
${OPENRTI_CMAKEROOTDIR}/src/OpenRTI/VariableLengthDataTuple.cpp)
target_link_libraries(InteractionFilters OpenRTI)
add_definitions(-DSTANDALONE_TEST)

add_test(OpenRTI/InteractionFilters "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/InteractionFilters")

