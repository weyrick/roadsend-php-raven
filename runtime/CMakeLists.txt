project (rphp)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake/modules )
FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(ICU)

cmake_minimum_required(VERSION 2.6)

include_directories (${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR} ${Boost_INCLUDE_DIRS})

set(RUNTIME_SRC_FILES
  pHash.cpp
  pObject.cpp
  pTypes.cpp
)

add_library( rphp-runtime SHARED ${RUNTIME_SRC_FILES} )

