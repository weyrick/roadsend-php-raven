# compiler
MESSAGE( STATUS "compiler: JIT check" )

include_directories (
                     ${RPHP_INCLUDE_DIR}
                     ${LLVM_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     # this one is for rphp_grammar generated files
                     ${CMAKE_CURRENT_BINARY_DIR}
                    )

# source files that depend on llvm
set(RJIT_SRC_FILES
    pJIT.cpp
  ${LLVM_LIBS_JIT_OBJECTS}
)

add_library( rphp-llvm-jit SHARED ${RJIT_SRC_FILES} )

# these are llvm specific compile flags, needed only for source files that include llvm headers
set_source_files_properties( ${RJIT_SRC_FILES}
                             PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                           )
target_link_libraries( rphp-llvm-jit
                       rphp-runtime
                       rphp-llvm-ir
                       ${LLVM_LIBS_JIT}
                      )

set_target_properties( rphp-llvm-jit
                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
                       # this tells cmake not to link the llvm libs into targets that link to rphp-llvm-jit
                       LINK_INTERFACE_LIBRARIES ""
                       )
