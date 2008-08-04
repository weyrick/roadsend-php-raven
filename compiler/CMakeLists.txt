# compiler
MESSAGE( STATUS "compiler check" )

EXECUTE_PROCESS(COMMAND llvm-config --libs core jit native OUTPUT_VARIABLE LLVM_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --includedir OUTPUT_VARIABLE LLVM_INCLUDE_DIRS OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --cppflags OUTPUT_VARIABLE LLVM_CPP_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
SEPARATE_ARGUMENTS(LLVM_LIBRARIES)
SEPARATE_ARGUMENTS(LLVM_INCLUDE_DIRS)
SEPARATE_ARGUMENTS(LLVM_CPP_FLAGS)

add_definitions(${LLVM_CPP_FLAGS} -fPIC)

include_directories (${PROJECT_SOURCE_DIR}/tools/kdevelop-pg/include
                     ${ICU_INCLUDE_DIRS}/unicode
                     ${ICU_INCLUDE_DIRS}
                     ${RPHP_RUNTIME_INCLUDE_DIR}
                     ${LLVM_INCLUDE_DIRS}
                     )

set(EVAL_SRC_FILES
  parser/rphp_default_visitor.cpp
  parser/rphp_parser.cpp
  parser/rphp_visitor.cpp
  parser/phplexer.cpp
  pDriver.cpp
)

add_library( rphp-eval SHARED ${EVAL_SRC_FILES} )
set_target_properties( rphp-eval
                       PROPERTIES LINK_FLAGS ${LLVM_LD_FLAGS} )
target_link_libraries( rphp-eval rphp-runtime ${ICU_LIBRARIES} ${ICU_I18_LIBRARIES} ${LLVM_LIBRARIES})

