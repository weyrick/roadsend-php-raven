# compiler
MESSAGE( STATUS "compiler: analysis check" )

include_directories (
                     ${ICU_INCLUDE_DIRS}/unicode
                     ${ICU_INCLUDE_DIRS}
                     ${RPHP_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     ${LLVM_INCLUDE_DIR}
		     ${TINYXML_INCLUDE_DIR} # dumpast
                     # this one is for rphp_grammar generated files
                     ${CMAKE_CURRENT_BINARY_DIR}
                    )

#### PARSER ####

# lexer, parser generation
# http://www.cmake.org/pipermail/cmake/2002-September/003028.html

add_executable( lemon-pg
                grammar_src/lemon.c )

ADD_CUSTOM_COMMAND(
   # note we do this copy because lemon will output to the source directory instead of the build
   # dir if we don't, so this keeps it tidy.
   # alternative (read: portable) way is to change lemon.c to not include the base directory
   # when outputing the generating parser files, but instead strip it and use current working
   COMMAND cp
   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/grammar_src/rphp_grammar.y ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND cp
   ARGS ${CMAKE_CURRENT_SOURCE_DIR}/grammar_src/lempar.c ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/lemon-pg
   ARGS ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.y
   DEPENDS lemon-pg
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/grammar_src/rphp_grammar.y
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp)

# Since rphp_grammar.c does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp GENERATED)

#### LEXER ####

add_executable( rphp-lexer-gen
                lexer_src/rphp-lexer-gen.cpp )

# we need rphp_grammar.h
SET_SOURCE_FILES_PROPERTIES(lexer_src/rphp-lexer-gen.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp)

ADD_CUSTOM_COMMAND(
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/rphp-lexer-gen
   DEPENDS rphp-lexer-gen
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/rphp_lang_lexer.h
          ${CMAKE_CURRENT_BINARY_DIR}/rphp_dq_lexer.h
   )

# Since rphp_lexer.cpp does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/rphp_lang_lexer.h GENERATED)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/rphp_dq_lexer.h GENERATED)
# make lexer depend on generator so it regenerates if we change it
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/rphp_lang_lexer.h OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lexer_src/rphp-lexer-gen.cpp)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/rphp_dq_lexer.h OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lexer_src/rphp-lexer-gen.cpp)

# pLexer depends on generated lexer
SET_SOURCE_FILES_PROPERTIES(pLexer.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/rphp_lang_lexer.h)
SET_SOURCE_FILES_PROPERTIES(passes/SimplifyStrings.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/rphp_dq_lexer.h)

####

set(PARSER_SRC_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp
  pAST.cpp
  pBaseVisitor.cpp
  pBaseTransformer.cpp
  pParseContext.cpp
  pSourceFile.cpp
  pLexer.cpp
  pParser.cpp
  pSourceModule.cpp
  pPass.cpp
  pPassManager.cpp
  # passes
  passes/CheckMemoryManagement.cpp
  passes/SimplifyStrings.cpp
  passes/Desugar.cpp
  passes/DumpAST.cpp
  passes/DumpStats.cpp
  passes/Split_Builtins.cpp
  passes/Early_Lower_Loops.cpp
  passes/Lower_Binary_Op.cpp
  passes/Lower_Conditional_Expr.cpp
  passes/Lower_Control_Flow.cpp
  passes/MainFunction.cpp
)

# PASSES
add_definitions("-DTIXML_USE_STL")

set_source_files_properties( ${PARSER_SRC_FILES}
                             PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                            )

# STATIC
add_library( rphp-analysis ${PARSER_SRC_FILES} )
target_link_libraries (rphp-analysis rphp-runtime)

