# Test if dynamic linking works as required by the standard
add_executable(link-1516ev link.cpp)
target_link_libraries(link-1516ev rti1516ev fedtime1516ev)
