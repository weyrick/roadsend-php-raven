# CLI frontend
MESSAGE( STATUS "CLI frontend check" )

add_definitions(${LLVM_COMPILE_FLAGS})

include_directories (${CMAKE_SOURCE_DIR} 
                     ${CMAKE_BINARY_DIR} 
                     ${RPHP_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     ${LLVM_INCLUDE_DIR}
                     ${GMP_INCLUDE_DIR}
                    )

set(RPHP_SRC_FILES
  rphp.cpp
  ${LLVM_LIBS_JIT_OBJECTS}
)

#set(RPHPI_SRC_FILES
#  rphpi.cpp
#  ${LLVM_LIBS_JIT_OBJECTS}
#)

add_executable( rphp ${RPHP_SRC_FILES} )
#add_executable( rphpi ${RPHPI_SRC_FILES} )


set_target_properties( rphp
                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
                     )
                     
#set_target_properties( rphpi
#                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
#                     )

target_link_libraries( rphp
                       rphp-llvm-jit
                       rphp-llvm-ir
                       rphp-analysis
                       rphp-runtime
#                       ${LLVM_LIBS_CORE}
                       ${LLVM_LIBS_JIT}
                       ${ICU_LIBRARIES} ${ICU_I18_LIBRARIES} ${ICU_IO_LIBRARIES}
                      ) 
#target_link_libraries( rphpi
#                       rphp-llvm-jit
#                       rphp-llvm-ir
#                       rphp-analysis
#                       rphp-runtime
#                       ${LLVM_LIBS_JIT}
#                       ${ICU_LIBRARIES} ${ICU_I18_LIBRARIES} ${ICU_IO_LIBRARIES}
#                       )                       

 