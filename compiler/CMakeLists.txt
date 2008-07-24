# compiler
MESSAGE( STATUS "compiler check" )

include_directories (${PROJECT_SOURCE_DIR}/tools/kdevelop-pg/include ${ICU_INCLUDE_DIRS}/unicode ${ICU_INCLUDE_DIRS} ${RPHP_RUNTIME_INCLUDE_DIR})

set(EVAL_SRC_FILES
  parser/rphp_default_visitor.cpp
  parser/rphp_parser.cpp
  parser/rphp_visitor.cpp
  parser/phplexer.cpp
  pDriver.cpp
)

add_library( rphp-eval SHARED ${EVAL_SRC_FILES} )
target_link_libraries( rphp-eval rphp-runtime ${ICU_LIBRARIES} ${ICU_I18_LIBRARIES} )

