# Library configuration file used by dependent projects
# via find_package() built-in directive in "config" mode.

if(NOT DEFINED UTIL_FOUND)
  # Locate library headers.
  FIND_PATH(UTIL_INCLUDE_DIRS 
    NAMES filemap.h
    PATHS ${UTIL_DIR} 
    NO_DEFAULT_PATH
  )

  # Common name for exported library targets.
  SET(UTIL_LIBRARIES
    util
    CACHE INTERNAL "util library" FORCE
  )

  # Usual "required" et. al. directive logic.
  INCLUDE(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(
    util DEFAULT_MSG
    UTIL_INCLUDE_DIRS
    UTIL_LIBRARIES
  )

  # Add targets to dependent project.
  add_subdirectory(
    ${UTIL_DIR}
    ${CMAKE_BINARY_DIR}/util
  )
endif()
