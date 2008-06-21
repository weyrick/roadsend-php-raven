project (rphp)

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

include(FindKDevelop-PG)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

set(SRC_FILES
  compiler/parser/rphp_default_visitor.cpp
  compiler/parser/rphp_parser.cpp
  compiler/parser/rphp_visitor.cpp
)

add_executable( rphp ${SRC_FILES} )

