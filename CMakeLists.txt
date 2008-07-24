# root

project (rphp)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules )
MESSAGE( STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH} )

set(RPHP_RUNTIME_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/runtime/include)
set(RPHP_COMPILER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/compiler)

FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(ICU)

add_subdirectory(tools)
add_subdirectory(runtime)
add_subdirectory(compiler)
add_subdirectory(frontend/cli)

