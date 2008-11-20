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

set(RPHP_RUNTIME_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/runtime)
set(RPHP_COMPILER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/compiler)

Find_Package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
IF( Boost_VERSION LESS "103600" )
  MESSAGE(FATAL_ERROR "Boost version ${Boost_VERSION} too old, please install 1.36 or greater!")
ENDIF( Boost_VERSION LESS "103600" )

Find_Package(ICU REQUIRED)
Find_Package(LLVM REQUIRED)
FIND_PACKAGE(CppUnit)

add_subdirectory(runtime)
add_subdirectory(runtime-ir)
add_subdirectory(compiler)
add_subdirectory(frontend/cli)



IF(CPPUNIT_FOUND)
  ENABLE_TESTING()
  ADD_TEST(rphp-runtime-suite runtime/test/rphp-runtime-suite)
  ADD_TEST(rphp-sizes runtime/test/rphp-sizes)
ENDIF(CPPUNIT_FOUND)
