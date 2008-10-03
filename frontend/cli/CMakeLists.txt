# CLI frontend
MESSAGE( STATUS "CLI frontend check" )

add_definitions(${LLVM_COMPILE_FLAGS})

include_directories (${CMAKE_SOURCE_DIR} 
                     ${CMAKE_BINARY_DIR} 
                     ${RPHP_RUNTIME_INCLUDE_DIR} 
                     ${RPHP_COMPILER_INCLUDE_DIR} 
                     ${Boost_INCLUDE_DIRS}
                     ${LLVM_INCLUDE_DIR})

set(CLI_SRC_FILES
  main.cpp
  # NOTE: this is temporary, and should be in rphp-eval when fPIC it can compile fPIC on x86-64
  ${LLVM_LIBS_NATIVE_OBJECTS}
)

add_executable( rphp ${CLI_SRC_FILES} )
set_target_properties( rphp
                       PROPERTIES LINK_FLAGS
                       # NOTE: this is temporary, and should be in rphp-eval when fPIC it can compile fPIC on x86-64
                       ${LLVM_LDFLAGS}
                     )
target_link_libraries( rphp rphp-eval rphp-runtime ${LLVM_LIBS_NATIVE} )

