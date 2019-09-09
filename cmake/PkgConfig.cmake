FUNCTION(CREATE_PKG_CONFIG _TARGET _DESCRIPTION _INCLUDESUBDIR _LIBSUBDIR)
  SET(TARGET_NAME ${_TARGET})
  SET(PKGCONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.pc")

  GET_PROPERTY(LIBRARY_NAME TARGET ${_TARGET} PROPERTY LIBRARY_OUTPUT_NAME)
  IF(NOT LIBRARY_NAME)
    SET(LIBRARY_NAME ${TARGET_NAME})
  ENDIF()
  
  SET(RPATH_LINK)
  IF(CMAKE_SHARED_LIBRARY_RPATH_LINK_CXX_FLAG)
    SET(RPATH_LINK "${CMAKE_SHARED_LIBRARY_RPATH_LINK_CXX_FLAG}\${libdir}")
  ENDIF()

  FILE(WRITE ${PKGCONFIG_FILE}
    "prefix=${CMAKE_INSTALL_PREFIX}\n"
    "exec_prefix=\${prefix}\n"
    "libdir=\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}/${_LIBSUBDIR}\n"
    "includedir=\${prefix}/include/${_INCLUDESUBDIR}\n"
    "Name: ${TARGET_NAME}\n"
    "Description: ${_DESCRIPTION}\n"
    "Version: ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}\n"
    "Libs: -L\${libdir} ${RPATH_LINK} -l${LIBRARY_NAME}\n"
    "Cflags: -I\${includedir}\n"
    )
  INSTALL(FILES ${PKGCONFIG_FILE} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
ENDFUNCTION()

FUNCTION(CREATE_API_PKG_CONFIG _NAME _DESCRIPTION)
  SET(PKGCONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${_NAME}.pc")
  STRING(REPLACE ";" " " _REQUIRES "${ARGN}")
  FILE(WRITE ${PKGCONFIG_FILE}
    "Name: ${_NAME}\n"
    "Description: ${_DESCRIPTION}\n"
    "Version: ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}\n"
    "Requires: ${_REQUIRES}\n"
    )
  INSTALL(FILES ${PKGCONFIG_FILE} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
ENDFUNCTION()
