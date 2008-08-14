# CLI frontend
MESSAGE( STATUS "CLI frontend check" )

EXECUTE_PROCESS(COMMAND llvm-config --libs x86 OUTPUT_VARIABLE LLVM_X86_JIT OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
SEPARATE_ARGUMENTS(LLVM_X86_JIT)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${RPHP_RUNTIME_INCLUDE_DIR} ${RPHP_COMPILER_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

set(CLI_SRC_FILES
  main.cpp
)

add_executable( rphp ${CLI_SRC_FILES} )
set_target_properties( rphp
                       PROPERTIES LINK_FLAGS ${LLVM_LD_FLAGS}
                     )
target_link_libraries( rphp rphp-eval rphp-runtime ${Boost_PROGRAM_OPTIONS_LIBRARY} ${LLVM_X86_JIT} )

