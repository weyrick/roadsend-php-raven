# compiler
MESSAGE( STATUS "compiler: IR check" )

include_directories (
                     ${ICU_INCLUDE_DIRS}/unicode
                     ${ICU_INCLUDE_DIRS}
                     ${RPHP_INCLUDE_DIR}
                     ${LLVM_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
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
    pCompileAndLinkTarget.cpp
    pLinkTarget.cpp
    pLibTargets.cpp
    pStandAloneTargets.cpp
    pCompileTarget.cpp
    pDumpTarget.cpp    
)

add_library( rphp-llvm-ir SHARED ${GENERATOR_SRC_FILES} )

# these are llvm specific compile flags, needed only for source files that include llvm headers
set_source_files_properties( ${GENERATOR_SRC_FILES}
                             PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                           )

target_link_libraries( rphp-llvm-ir
                       rphp-analysis
                       )

# on x86_64, some of the core libs are not currently -fPIC and need to be
# to create a shared library here. for now, we link them in the rphpi
# executable instead.
IF( NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
    target_link_libraries( rphp-llvm-ir ${LLVM_LIBS_CORE} ) 
else( NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
    message(STATUS "x86_64 link work around: LLVM core libs will be linked to rphp binary")
ENDIF( NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

set_target_properties( rphp-llvm-ir
                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
                       # this tells cmake not to link the llvm libs into targets that link to rphp-llvm-ir
                       LINK_INTERFACE_LIBRARIES ""
                       )
