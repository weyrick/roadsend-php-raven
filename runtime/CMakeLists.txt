project (rphp)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake/modules )
FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(ICU)

cmake_minimum_required(VERSION 2.6)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${Boost_INCLUDE_DIRS})

# runtime library
set(RUNTIME_SRC_FILES
  rphp_hash.cpp
  rphp_object.cpp
  rphp_types.cpp
)

add_library( rphp_runtime SHARED ${RUNTIME_SRC_FILES} )


