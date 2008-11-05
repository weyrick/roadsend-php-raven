
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake/modules )

cmake_minimum_required(VERSION 2.6)

FIND_PACKAGE(ICU)

# XXX fixme
link_directories (${CMAKE_SOURCE_DIR}/../../build/runtime/)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/..)

# test apps
add_executable( rphp-runtime-suite
                main.cpp
                phashTestCase.cpp
                pvarTestCase.cpp
                functionTestCase.cpp
                runtimeTestCase.cpp
               )
add_executable( rphp-sizes rphp-sizes.cpp )
target_link_libraries( rphp-runtime-suite cppunit rphp-runtime ${ICU_LIBRARIES} ${ICU_IO_LIBRARIES})
