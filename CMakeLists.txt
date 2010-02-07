# root

cmake_minimum_required(VERSION 2.6)

# this policy warning is due to lib command lines arg output from llvm-config
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project (rphp)

set(LLVM_MIN_VERSION       "2006000")
set(LLVM_MIN_VERSION_TEXT  "2.6")
set(BOOST_MIN_VERSION      "103800")
set(BOOST_MIN_VERSION_TEXT "1.38")
set(ICU_MIN_VERSION        "4002000")
set(ICU_MIN_VERSION_TEXT   "4.2")

set(RPHP_SHARED_LIBS       0)

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


IF( Boost_VERSION LESS ${BOOST_MIN_VERSION} )
  MESSAGE(FATAL_ERROR "Boost version ${Boost_VERSION} too old, please install ${BOOST_MIN_VERSION_TEXT} or greater!")
ENDIF( Boost_VERSION LESS ${BOOST_MIN_VERSION} )

Find_Package(ICU REQUIRED)
Find_Package(LLVM REQUIRED)
Find_Package(GMP REQUIRED)
Find_Package(CppUnit) # optional
Find_Package(Doxygen) # optional

#set(RPHP_INCLUDE_FULL ${RPHP_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${ICU_INCLUDE_DIRS} ${GMP_INCLUDE_DIR} ${LLVM_INCLUDE_DIR})
#Message(STATUS "Includes: ${RPHP_INCLUDE_FULL}")

include_directories(${RPHP_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${ICU_INCLUDE_DIRS} ${GMP_INCLUDE_DIR} ${LLVM_INCLUDE_DIR})
link_directories(${ICU_LIB_DIRS})

set(TINYXML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/tinyxml")

# require ICU 4.2
IF(ICU_VERSION LESS ${ICU_MIN_VERSION})
  MESSAGE(FATAL_ERROR "ICU version ${ICU_STRING_VERSION} is too old, please install ${ICU_MIN_VERSION_TEXT} or greater!")
ENDIF(ICU_VERSION LESS ${ICU_MIN_VERSION})

IF( LLVM_VERSION LESS ${LLVM_MIN_VERSION} )
  MESSAGE(FATAL_ERROR "LLVM version ${LLVM_STRING_VERSION} is too old, please install ${LLVM_MIN_VERSION_TEXT} of greater!")
ENDIF( LLVM_VERSION LESS ${LLVM_MIN_VERSION} )
add_definitions(-DLLVM_VERSION=${LLVM_VERSION})

add_subdirectory(3rdparty)
add_subdirectory(runtime)
add_subdirectory(c-runtime)
add_subdirectory(compiler)
add_subdirectory(frontend/cli)
add_subdirectory(frontend/http)

# check whether to include the sandbox
IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
add_subdirectory(sandbox)
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")

IF(CPPUNIT_FOUND)
  ENABLE_TESTING()
  ADD_TEST(rphp-runtime-suite runtime/test/rphp-runtime-suite)
  ADD_TEST(rphp-sizes runtime/test/rphp-sizes)
ENDIF(CPPUNIT_FOUND)

IF(DOXYGEN_FOUND)
  SET(DOXYFILE doc/Doxyfile.rphp)
  ADD_CUSTOM_TARGET(doc ${DOXYGEN_EXECUTABLE} ${DOXYFILE} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF(DOXYGEN_FOUND)

