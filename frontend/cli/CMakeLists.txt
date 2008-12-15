# CLI frontend
MESSAGE( STATUS "CLI frontend check" )

add_definitions(${LLVM_COMPILE_FLAGS})

include_directories (${CMAKE_SOURCE_DIR} 
                     ${CMAKE_BINARY_DIR} 
                     ${RPHP_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     ${LLVM_INCLUDE_DIR})

set(CLI_SRC_FILES
  main.cpp
)

add_executable( rphp ${CLI_SRC_FILES} )
target_link_libraries( rphp
                       rphp-driver
                      )

