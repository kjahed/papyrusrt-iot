#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "flatbuffers::flatbuffers" for configuration ""
set_property(TARGET flatbuffers::flatbuffers APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(flatbuffers::flatbuffers PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "/media/psf/Home/Desktop/mqtt-paper/Implementation/umlrts/lib/linux/x86-g++-5.4.0/debug/libflatbuffers.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS flatbuffers::flatbuffers )
list(APPEND _IMPORT_CHECK_FILES_FOR_flatbuffers::flatbuffers "/media/psf/Home/Desktop/mqtt-paper/Implementation/umlrts/lib/linux/x86-g++-5.4.0/debug/libflatbuffers.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
