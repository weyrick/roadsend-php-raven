# compiler
MESSAGE( STATUS "compiler: IR check" )

include_directories (
                     ${ICU_INCLUDE_DIRS}/unicode
                     ${ICU_INCLUDE_DIRS}
                     ${RPHP_INCLUDE_DIR}
                     ${LLVM_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                    )

# source files that depend on llvm
set(GENERATOR_SRC_FILES
    pGenerator.cpp
    pGenSupport.cpp
    pIRHelper.cpp
)

add_library( rphp-llvm-ir SHARED ${GENERATOR_SRC_FILES} )

# these are llvm specific compile flags, needed only for source files that include llvm headers
set_source_files_properties( ${GENERATOR_SRC_FILES}
                             PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                           )

target_link_libraries( rphp-llvm-ir
                       rphp-analysis
                       ${LLVM_LIBS_CORE}
                       )

set_target_properties( rphp-llvm-ir
                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
                       # this tells cmake not to link the llvm libs into targets that link to rphp-llvm-ir
                       LINK_INTERFACE_LIBRARIES ""
                       )
