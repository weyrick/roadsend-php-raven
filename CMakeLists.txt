project (rphp)

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

add_subdirectory(tools)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} tools/kdevelop-pg/include)

set(SRC_FILES
  compiler/parser/rphp_default_visitor.cpp
  compiler/parser/rphp_parser.cpp
  compiler/parser/rphp_visitor.cpp
)

add_executable( rphp ${SRC_FILES} )

