file(READ "src/OpenRTI/OpenRTIVersion.h" VersionFileContent)

string(REGEX MATCH "#define OPENRTI_MAJOR_NUMBER( *)([0-9]*)" _ ${VersionFileContent})
set(OPENRTI_MAJOR_VERSION ${CMAKE_MATCH_2})

string(REGEX MATCH "#define OPENRTI_MINOR_NUMBER( *)([0-9]*)" _ ${VersionFileContent})
set(OPENRTI_MINOR_VERSION ${CMAKE_MATCH_2})

string(REGEX MATCH "#define OPENRTI_BUILD_NUMBER( *)([0-9]*)" _ ${VersionFileContent})
set(OPENRTI_BUILD_VERSION ${CMAKE_MATCH_2})

string(REGEX MATCH "#define OPENRTI_PATCH_NUMBER( *)([0-9]*)" _ ${VersionFileContent})
set(OPENRTI_PATCH_VERSION ${CMAKE_MATCH_2})

set(OPENRTI_VERSION "${OPENRTI_MAJOR_VERSION}.${OPENRTI_MINOR_VERSION}.${OPENRTI_BUILD_VERSION}.${OPENRTI_PATCH_VERSION}")
