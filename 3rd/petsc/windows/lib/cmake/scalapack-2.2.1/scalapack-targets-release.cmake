#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "scalapack" for configuration "Release"
set_property(TARGET scalapack APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(scalapack PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;Fortran"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libscalapack.a"
  )

list(APPEND _cmake_import_check_targets scalapack )
list(APPEND _cmake_import_check_files_for_scalapack "${_IMPORT_PREFIX}/lib/libscalapack.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
