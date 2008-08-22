# CLI frontend
MESSAGE( STATUS "CLI frontend check" )

EXECUTE_PROCESS(COMMAND llvm-config --libs x86 support OUTPUT_VARIABLE LLVM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --includedir OUTPUT_VARIABLE LLVM_INCLUDE_DIRS OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --cppflags OUTPUT_VARIABLE LLVM_CPP_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
SEPARATE_ARGUMENTS(LLVM_LIBS)
SEPARATE_ARGUMENTS(LLVM_INCLUDE_DIRS)
SEPARATE_ARGUMENTS(LLVM_CPP_FLAGS)

add_definitions(${LLVM_CPP_FLAGS})

include_directories (${CMAKE_SOURCE_DIR} 
                     ${CMAKE_BINARY_DIR} 
                     ${RPHP_RUNTIME_INCLUDE_DIR} 
                     ${RPHP_COMPILER_INCLUDE_DIR} 
                     ${Boost_INCLUDE_DIRS}
                     ${LLVM_INCLUDE_DIRS})

set(CLI_SRC_FILES
  main.cpp
)

add_executable( rphp ${CLI_SRC_FILES} )
set_target_properties( rphp
                       PROPERTIES LINK_FLAGS ${LLVM_LD_FLAGS}
                     )
target_link_libraries( rphp rphp-eval rphp-runtime ${LLVM_LIBS} )

