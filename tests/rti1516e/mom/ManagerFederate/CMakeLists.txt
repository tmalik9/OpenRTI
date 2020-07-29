add_custom_command(
    OUTPUT  emptyFOM.inc
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/fom2cpp ${CMAKE_SOURCE_DIR}/tests/empty.xml
    DEPENDS empty.xml
)
add_executable(ManagerFederate ManagerFederate.cpp
	Mom.cpp
	MomFederate.cpp
	MomFederation.cpp
	main.cpp
	../../../../src/OpenRTI/dprintf.cpp
	../../include/RTI1516ESimpleTestLib.cpp
)
target_link_libraries(ManagerFederate rti1516e fedtime1516e)
