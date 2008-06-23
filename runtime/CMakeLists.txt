project (rphp)

FIND_PACKAGE(Boost)

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

set(SRC_FILES
  var-test.cpp
)

add_executable( rphp ${SRC_FILES} )

