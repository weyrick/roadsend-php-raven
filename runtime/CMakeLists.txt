project (rphp)

FIND_PACKAGE(Boost REQUIRED)

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${Boost_INCLUDE_DIRS})

set(SRC_FILES
  var-test.cpp
)

add_executable( rphp ${SRC_FILES} )

