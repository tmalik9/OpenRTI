add_executable(ManagerFederate ManagerFederate.cpp
	Mom.cpp
	MomFederate.cpp
	MomFederation.cpp
	main.cpp
	../../../../src/OpenRTI/dprintf.cpp
	../../include/RTI1516ESimpleTestLib.cpp
)
target_link_libraries(ManagerFederate rti1516e fedtime1516e)
