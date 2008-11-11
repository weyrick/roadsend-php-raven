# compiler
MESSAGE( STATUS "compiler check" )

include_directories (
                     ${ICU_INCLUDE_DIRS}/unicode
                     ${ICU_INCLUDE_DIRS}
                     ${RPHP_RUNTIME_INCLUDE_DIR}
                     ${RPHP_COMPILER_INCLUDE_DIR}
                     ${LLVM_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     # this one is for rphp_grammar generated files
                     ${CMAKE_CURRENT_BINARY_DIR}
                    )

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
   OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp)

# Since rphp_grammar.c does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp GENERATED)
# make grammar depend on template so it regenerates if we change it
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lemon/lempar.c)

####

# source files that depend on llvm
set(RPHP_LLVM_DEP_FILES
    pGenerator.cpp
    pGenSupport.cpp
    pDriver.cpp
    pStandAloneTargets.cpp
    pModule.cpp
    pIRHelper.cpp
)

set(EVAL_SRC_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp
  pLexer.cpp
  pASTVisitors.cpp
  pParser.cpp
  pCompileTarget.cpp
  pInterpretTarget.cpp
  pCompileAndLinkTarget.cpp
  pLinkTarget.cpp
  pLibTargets.cpp
  ${RPHP_LLVM_DEP_FILES}
  ${LLVM_LIBS_INTERPRETER_OBJECTS}
  ${LLVM_LIBS_NATIVE_OBJECTS}
)

add_library( rphp-eval SHARED ${EVAL_SRC_FILES} )
set_target_properties( rphp-eval
                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
                       # this tells cmake not to link the llvm libs into targets that link to rphp-eval
                       LINK_INTERFACE_LIBRARIES "" )
# these are llvm specific compile flags, needed only for source files that include llvm headers
set_source_files_properties( ${RPHP_LLVM_DEP_FILES}
                             PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                           )
target_link_libraries( rphp-eval rphp-runtime ${ICU_LIBRARIES} ${ICU_I18_LIBRARIES} ${LLVM_LIBS_CORE} ${LLVM_LIBS_INTERPRETER} ${LLVM_LIBS_NATIVE})
