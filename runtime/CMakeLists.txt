project (rphp)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake/modules )
FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(ICU)

cmake_minimum_required(VERSION 2.6)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${Boost_INCLUDE_DIRS})

set(SRC_FILES
  var-test.cpp
)

add_executable( var-test ${SRC_FILES} )
