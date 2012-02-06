# CLI frontend
MESSAGE( STATUS "CLI frontend check" )

add_definitions(${LLVM_COMPILE_FLAGS})

include_directories (${CMAKE_SOURCE_DIR} 
                     ${CMAKE_BINARY_DIR} 
                     # this one is for rphp_grammar generated files
                     ${PROJECT_BINARY_DIR}/compiler/analysis
                    )

set(RPHP_SRC_FILES
  rphp.cpp
  ${LLVM_LIBS_JIT_OBJECTS}
)

#set(RPHPI_SRC_FILES
#  rphpi.cpp
#  ${LLVM_LIBS_JIT_OBJECTS}
#)

set(RPHP_ANALYSIS_FILES
    rphp-analyzer.cpp)

add_executable( rphp ${RPHP_SRC_FILES} )
#add_executable( rphpi ${RPHPI_SRC_FILES} )
add_executable( rphp-analyzer ${RPHP_ANALYSIS_FILES} )

set_target_properties( rphp
                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
                     )
                     
#set_target_properties( rphpi
#                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
#                     )

set_target_properties( rphp-analyzer
                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
                     )

target_link_libraries( rphp
#                       rphp-llvm-jit
                       rphp-llvm-ir
                       rphp-analysis
                       rphp-runtime
#                       ${LLVM_LIBS_CORE}
                       ${LLVM_LIBS_JIT}
                       ${ICU_LIBRARIES}
                        dl pthread
                      )
#target_link_libraries( rphpi
#                       rphp-llvm-jit
#                       rphp-llvm-ir
#                       rphp-analysis
#                       rphp-runtime
#                       ${LLVM_LIBS_JIT}
#                       ${ICU_LIBRARIES}
#                       )                       

target_link_libraries ( rphp-analyzer
                        rphp-analysis
                        tinyxml
                       ${LLVM_LIBS_SUPPORT} 
                       ${ICU_LIBRARIES}
                        dl pthread
			)