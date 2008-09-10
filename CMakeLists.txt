# root

cmake_minimum_required(VERSION 2.4)

# this policy warning is due to lib command lines arg output from llvm-config
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project (rphp)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules )
MESSAGE( STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH} )

set(RPHP_RUNTIME_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/runtime/include)
set(RPHP_COMPILER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/compiler)

Find_Package(Boost)
FIND_PACKAGE(ICU)

#add_subdirectory(tools)
add_subdirectory(runtime)
add_subdirectory(compiler)
add_subdirectory(frontend/cli)

