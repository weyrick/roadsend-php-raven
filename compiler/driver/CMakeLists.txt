# compiler
MESSAGE( STATUS "compiler: driver check" )

include_directories (
                     ${RPHP_INCLUDE_DIR}
                     ${LLVM_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     # FIXME: this is for rphp_grammar.h. need a better place
                    ${CMAKE_CURRENT_BINARY_DIR}/../analysis/
                    )

set(DRIVER_LLVM_SRC_FILES
    pStandAloneTargets.cpp
    pCompileTarget.cpp
    pInterpretTarget.cpp
    pDumpTarget.cpp
)

set(DRIVER_SRC_FILES
    ${DRIVER_LLVM_SRC_FILES}
    pCompileAndLinkTarget.cpp
    pLinkTarget.cpp
    pLibTargets.cpp
)

add_library( rphp-driver SHARED ${DRIVER_SRC_FILES} )

# these are llvm specific compile flags, needed only for source files that include llvm headers
set_source_files_properties( ${DRIVER_LLVM_SRC_FILES}
                             PROPERTIES
                             COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                           )

target_link_libraries( rphp-driver
                       rphp-analysis
                       rphp-llvm-ir
                       rphp-llvm-jit
                       )

set_target_properties( rphp-driver
                       PROPERTIES
                       LINK_FLAGS ${LLVM_LDFLAGS}
                       # this tells cmake not to link the llvm libs into targets that link to rphp-driver
                       #LINK_INTERFACE_LIBRARIES ""
                       )
