# credit: dgregor: http://svn.boost.org/trac/boost/browser/sandbox/troy/boost_1_34_0/tools/build/CMake/FindICU.cmake?rev=4096
#
# Finds the International Components for Unicode (ICU) Library
#
#  ICU_FOUND          - True if ICU found.
#  ICU_I18N_FOUND     - True if ICU's internationalization library found.
#  ICU_IO_FOUND       - True if ICU's ostream output lib is found.
#  ICU_INCLUDE_DIRS   - Directory to include to get ICU headers
#                       Note: always include ICU headers as, e.g., 
#                       unicode/utypes.h
#  ICU_LIBRARIES      - Libraries to link against for the common ICU
#  ICU_I18N_LIBRARIES - Libraries to link against for ICU internationaliation
#                       (note: in addition to ICU_LIBRARIES)
#
#  ICU_IO_LIBRARIES   - Libraries for ostream output
#
#
# Look for the header file.
find_path(
  ICU_INCLUDE_DIR 
  NAMES unicode/utypes.h
  DOC "Include directory for the ICU library")
mark_as_advanced(ICU_INCLUDE_DIR)

# Look for the library.
find_library(
  ICU_LIBRARY
  NAMES icuuc cygicuuc cygicuuc32
  DOC "Libraries to link against for the common parts of ICU")
mark_as_advanced(ICU_LIBRARY)

# Copy the results to the output variables.
if(ICU_INCLUDE_DIR AND ICU_LIBRARY)
  set(ICU_FOUND 1)
  set(ICU_LIBRARIES ${ICU_LIBRARY})
  set(ICU_INCLUDE_DIRS ${ICU_INCLUDE_DIR})

  # Look for the ICU internationalization libraries
  find_library(
    ICU_I18N_LIBRARY
    NAMES icuin icui18n cygicuin cygicuin32
    DOC "Libraries to link against for ICU internationalization")
  mark_as_advanced(ICU_I18N_LIBRARY)

  if (ICU_I18N_LIBRARY)
    set(ICU_I18N_FOUND 1)
    set(ICU_I18N_LIBRARIES ${ICU_I18N_LIBRARY})
  else (ICU_I18N_LIBRARY)
    set(ICU_I18N_FOUND 0)
    set(ICU_I18N_LIBRARIES)
  endif (ICU_I18N_LIBRARY)

  # Look for the ICU ostream library
  find_library(
    ICU_IO_LIBRARY
    NAMES icuio
    DOC "Libraries to link against for ICU ostream output")
  mark_as_advanced(ICU_IO_LIBRARY)

  if (ICU_IO_LIBRARY)
    set(ICU_IO_FOUND 1)
    set(ICU_IO_LIBRARIES ${ICU_IO_LIBRARY})
  else (ICU_IO_LIBRARY)
    set(ICU_IO_FOUND 0)
    set(ICU_IO_LIBRARIES)
  endif (ICU_IO_LIBRARY)

else(ICU_INCLUDE_DIR AND ICU_LIBRARY)
  set(ICU_FOUND 0)
  set(ICU_I18N_FOUND 0)
  set(ICU_IO_FOUND 0)
  set(ICU_LIBRARIES)
  set(ICU_I18N_LIBRARIES)
  set(ICU_IO_LIBRARIES)
  set(ICU_INCLUDE_DIRS)
endif(ICU_INCLUDE_DIR AND ICU_LIBRARY)
