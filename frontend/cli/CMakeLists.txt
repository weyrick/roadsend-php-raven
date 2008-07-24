# CLI frontend
MESSAGE( STATUS "CLI frontend check" )

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${RPHP_RUNTIME_INCLUDE_DIR} ${RPHP_COMPILER_INCLUDE_DIR})

set(CLI_SRC_FILES
  main.cpp
)

add_executable( rphp ${CLI_SRC_FILES} )
target_link_libraries( rphp rphp-eval rphp-runtime boost_program_options )

