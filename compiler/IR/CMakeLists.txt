# compiler
MESSAGE( STATUS "compiler: IR check" )

include_directories (
                     # FIXME: this is for rphp_grammar.h. need a better place
                     ${CMAKE_CURRENT_BINARY_DIR}/../analysis/                     
                    )

# source files that depend on llvm
set(GENERATOR_SRC_FILES
    pGenerator.cpp
    pGenSupport.cpp
    pIRHelper.cpp
    pCodeGen.cpp
    pDeclare.cpp
    # targets
    #pCompileAndLinkTarget.cpp
    #pLinkTarget.cpp
    #pLibTargets.cpp
    #pStandAloneTargets.cpp
    #pCompileTarget.cpp
    pDumpTarget.cpp    
)

# these are llvm specific compile flags, needed only for source files that include llvm headers
set_source_files_properties( ${GENERATOR_SRC_FILES}
                             PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                            )

# STATIC
add_library( rphp-llvm-ir ${GENERATOR_SRC_FILES} )

target_link_libraries( rphp-llvm-ir ${GMP_LIBRARIES} ) 

set_target_properties( rphp-llvm-ir
                        PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
                     )                        
