# compiler
MESSAGE( STATUS "compiler check" )

include_directories (
                     ${ICU_INCLUDE_DIRS}/unicode
                     ${ICU_INCLUDE_DIRS}
                     ${RPHP_RUNTIME_INCLUDE_DIR}
                     ${RPHP_COMPILER_INCLUDE_DIR}
                     ${LLVM_INCLUDE_DIRS}
                     ${Boost_INCLUDE_DIRS}
                     ${CMAKE_CURRENT_BINARY_DIR}
                    )

######

# llvm rules

# TODO: move this to a FindLLVM in cmake/modules
# see frontend/cli/CMakeLists.txt as well
EXECUTE_PROCESS(COMMAND llvm-config --libs core jit bitreader bitwriter linker interpreter OUTPUT_VARIABLE LLVM_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --includedir OUTPUT_VARIABLE LLVM_INCLUDE_DIRS OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --cppflags OUTPUT_VARIABLE LLVM_CPP_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
SEPARATE_ARGUMENTS(LLVM_LIBRARIES)
SEPARATE_ARGUMENTS(LLVM_INCLUDE_DIRS)
SEPARATE_ARGUMENTS(LLVM_CPP_FLAGS)

add_definitions(${LLVM_CPP_FLAGS} -fPIC)

#####

# lexer, parser generation
# http://www.cmake.org/pipermail/cmake/2002-September/003028.html

add_executable( lemon
                lemon/lemon.c )

# Create target for the parser
#ADD_CUSTOM_TARGET(grammarGen echo "Creating rphp_grammar.c")

# NOTE: we'll use this when we generate static spirit lexer

# Create custom command for flex/lex (note the outputs)
# ADD_CUSTOM_COMMAND(
# SOURCE ${Foo_SOURCE_DIR}/src/lexer.l
# COMMAND ${FLEX_EXECUTABLE}
# ARGS -o${Foo_BINARY_DIR}/src/lexer.c
#     ${Foo_SOURCE_DIR}/src/lexer.l
# TARGET FooParser
# OUTPUTS ${Foo_BINARY_DIR}/src/lexer.c)

# Create custom command for bison/yacc (note the DEPENDS)
ADD_CUSTOM_COMMAND(
   SOURCE lemon/rphp_grammar.y
   # note we do this copy because lemon will output to the source directory instead of the build
   # dir if we don't, so this keeps it tidy.
   # alternative (read: portable) way is to change lemon.c to not include the base directory
   # when outputing the generating parser files, but instead strip it and use current working
   COMMAND cp
   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lemon/rphp_grammar.y ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND cp
   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lemon/lempar.c ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/lemon
   ARGS ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.y
   TARGET rphp-eval
   DEPENDS lemon
   OUTPUTS rphp_grammar.cpp)

# Since rphp_grammar.c does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(rphp_grammar.cpp GENERATED)
# make grammar depend on template so it regenerates if we change it
SET_SOURCE_FILES_PROPERTIES(rphp_grammar.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lemon/lempar.c)

####

# rphp-eval lib rules

set(EVAL_SRC_FILES
  rphp_grammar.cpp
  pLexer.cpp
  pASTVisitors.cpp
  pDriver.cpp
  pParser.cpp
)

add_library( rphp-eval SHARED ${EVAL_SRC_FILES} )
set_target_properties( rphp-eval
                       PROPERTIES LINK_FLAGS ${LLVM_LD_FLAGS}
                       # this tells cmake not to link the llvm libs into targets that link to rphp-eval
                       LINK_INTERFACE_LIBRARIES "" )
target_link_libraries( rphp-eval rphp-runtime ${ICU_LIBRARIES} ${ICU_I18_LIBRARIES} ${LLVM_LIBRARIES})

