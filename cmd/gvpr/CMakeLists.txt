add_definitions(-DEXPORT_GVPR)

add_executable(gvpr-bin
  # Source files
  gvprmain.c
)

target_include_directories(gvpr-bin PRIVATE
  ../../lib
  ../../lib/cdt
  ../../lib/cgraph
  ../../lib/gvpr
)

target_link_libraries(gvpr-bin PRIVATE
  ast
  cgraph
  gvc
  gvpr
  expr
  sfio
)

set_target_properties(gvpr-bin
  PROPERTIES OUTPUT_NAME gvpr)

# Installation location of executables
install(
  TARGETS gvpr-bin
  RUNTIME DESTINATION ${BINARY_INSTALL_DIR}
  LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
)

# Specify man pages to be installed
if(GZIP)
  add_custom_target(man-gvpr ALL DEPENDS gvpr.1.gz
                    COMMENT "gvpr man page")
  add_custom_command(
    OUTPUT gvpr.1.gz
    COMMAND ${GZIP} -9 --no-name --to-stdout gvpr.1
      >"${CMAKE_CURRENT_BINARY_DIR}/gvpr.1.gz"
    MAIN_DEPENDENCY gvpr.1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "compress gvpr man page")
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/gvpr.1.gz
    DESTINATION ${MAN_INSTALL_DIR}/man1)
else()
  install(
    FILES gvpr.1
    DESTINATION ${MAN_INSTALL_DIR}/man1
  )
endif()
