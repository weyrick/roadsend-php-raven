# credit: dgregor: http://svn.boost.org/trac/boost/browser/sandbox/troy/boost_1_34_0/tools/build/CMake/FindICU.cmake?rev=4096
#
# Finds the International Components for Unicode (ICU) Library
#
#  ICU_FOUND          - True if ICU found.
#  ICU_INCLUDE_DIRS   - Directory to include to get ICU headers
#                       Note: always include ICU headers as, e.g., 
#                       unicode/utypes.h
#  ICU_LIBRARIES      - Libraries to link against for the common ICU
#
#  ICU_IO_LIBRARIES   - Libraries for ostream output
#  ICU_LIB_DIRS       - Library search path
#

find_program(ICU_CONFIG_EXECUTABLE
    NAMES icu-config
    PATHS
    /opt/local/bin
)

exec_program(${ICU_CONFIG_EXECUTABLE} ARGS --cppflags-searchpath OUTPUT_VARIABLE ICU_INCLUDE_DIRS )
exec_program(${ICU_CONFIG_EXECUTABLE} ARGS --ldflags-searchpath OUTPUT_VARIABLE ICU_LIB_DIRS )
exec_program(${ICU_CONFIG_EXECUTABLE} ARGS --ldflags-libsonly OUTPUT_VARIABLE ICU_LIBRARIES )
exec_program(${ICU_CONFIG_EXECUTABLE} ARGS --ldflags-icuio OUTPUT_VARIABLE ICU_IO_LIBRARIES )

STRING(REPLACE "-I" ""  ICU_INCLUDE_DIRS ${ICU_INCLUDE_DIRS})
STRING(REPLACE "-L" ""  ICU_LIB_DIRS ${ICU_LIB_DIRS})
# osx icu weirdness?
STRING(REPLACE "\\c" ""  ICU_LIB_DIRS ${ICU_LIB_DIRS})
STRING(REPLACE "\\c" ""  ICU_INCLUDE_DIRS ${ICU_INCLUDE_DIRS})
STRING(REPLACE "\\c" ""  ICU_LIBRARIES ${ICU_LIBRARIES})
STRING(REPLACE "\\c" ""  ICU_IO_LIBRARIES ${ICU_IO_LIBRARIES})
set(ICU_LIBRARIES "${ICU_LIBRARIES} ${ICU_IO_LIBRARIES}")

# Copy the results to the output variables.
if(ICU_CONFIG_EXECUTABLE)

  set(ICU_FOUND 1)

  INCLUDE(TransformVersion)
  exec_program(${ICU_CONFIG_EXECUTABLE} ARGS --version OUTPUT_VARIABLE ICU_STRING_VERSION )
  MESSAGE(STATUS "ICU version: " ${ICU_STRING_VERSION})
  transform_version(ICU_VERSION ${ICU_STRING_VERSION})
  #MESSAGE(STATUS "ICU version num: " ${ICU_VERSION})

else(ICU_CONFIG_EXECUTABLE)
  set(ICU_FOUND 0)
  set(ICU_LIBRARIES)
  set(ICU_IO_LIBRARIES)
  set(ICU_INCLUDE_DIRS)
endif(ICU_CONFIG_EXECUTABLE)
