# compiler
MESSAGE( STATUS "compiler: JIT check" )

include_directories (
                     # this one is for rphp_grammar generated files
                     ${CMAKE_CURRENT_BINARY_DIR}
                    )

set(RJIT_SRC_FILES
    pJITTarget.cpp
    pCachedJIT.cpp    
   )

# these are llvm specific compile flags, needed only for source files that include llvm headers
set_source_files_properties( ${RJIT_SRC_FILES}
                             PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                           )

# STATIC
add_library( rphp-llvm-jit ${RJIT_SRC_FILES} ) 

set_target_properties( rphp-llvm-jit
   PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
   )
   
                       