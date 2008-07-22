project (rphp)

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

FIND_PACKAGE(ICU REQUIRED)

add_subdirectory(tools)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} tools/kdevelop-pg/include ${ICU_INCLUDE_DIRS}/unicode ${ICU_INCLUDE_DIRS})

set(SRC_FILES
  compiler/parser/rphp_default_visitor.cpp
  compiler/parser/rphp_parser.cpp
  compiler/parser/rphp_visitor.cpp
  compiler/parser/phplexer.cpp
  compiler/parser/main.cpp
)

add_executable( rphp ${SRC_FILES} )

target_link_libraries( rphp  ${ICU_LIBRARIES} ${ICU_I18_LIBRARIES} )
