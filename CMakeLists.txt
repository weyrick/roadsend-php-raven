# root

cmake_minimum_required(VERSION 2.6)

# this policy warning is due to lib command lines arg output from llvm-config
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project (rphp)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules )
set(RPHP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(ASIO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/asio)
set(RPHP_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS_DEBUG "-fexceptions -g -Wall -Winline -W -Wwrite-strings -Wno-unused")

MESSAGE( STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH} )

MESSAGE( STATUS "RPHP_INCLUDE_DIR: " ${RPHP_INCLUDE_DIR} )
MESSAGE( STATUS "RPHP_LIB_DIR: " ${RPHP_LIB_DIR} )
MESSAGE( STATUS "CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR} )

Find_Package(Boost REQUIRED)
include_directories(${RPHP_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
IF( Boost_VERSION LESS "103800" )
  MESSAGE(FATAL_ERROR "Boost version ${Boost_VERSION} too old, please install 1.38 or greater!")
ENDIF( Boost_VERSION LESS "103800" )

Find_Package(ICU REQUIRED)
Find_Package(LLVM REQUIRED)
FIND_PACKAGE(CppUnit) # optional
Find_PACKAGE(Doxygen) # optional

# OS dependant linking items

# determine if we will link llvm jit objects to the jit library or the rphpi binary
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(RPHP_JIT_LINK "library")
ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(RPHP_JIT_LINK "binary")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

MESSAGE( STATUS "JIT libs linked to: " ${RPHP_JIT_LINK} )

add_subdirectory(runtime)
add_subdirectory(c-runtime)
add_subdirectory(compiler)
add_subdirectory(frontend/cli)
add_subdirectory(frontend/http)

# check whether to include the sandbox
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
add_subdirectory(sandbox)
ENDIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

IF(CPPUNIT_FOUND)
  ENABLE_TESTING()
  ADD_TEST(rphp-runtime-suite runtime/test/rphp-runtime-suite)
  ADD_TEST(rphp-sizes runtime/test/rphp-sizes)
ENDIF(CPPUNIT_FOUND)


IF(DOXYGEN_FOUND)
  SET(DOXYFILE doc/Doxyfile.rphp)
  ADD_CUSTOM_TARGET(doc ${DOXYGEN_EXECUTABLE} ${DOXYFILE} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF(DOXYGEN_FOUND)

