# compiler
MESSAGE( STATUS "compiler: analysis check" )

include_directories (
                     ${ICU_INCLUDE_DIRS}/unicode
                     ${ICU_INCLUDE_DIRS}
                     ${RPHP_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     # this one is for rphp_grammar generated files
                     ${CMAKE_CURRENT_BINARY_DIR}
                    )

#####

# lexer, parser generation
# http://www.cmake.org/pipermail/cmake/2002-September/003028.html

add_executable( lemon-pg
                lemon/lemon.c )

# Create target for the parser
#ADD_CUSTOM_TARGET(grammarGen echo "Creating rphp_grammar.c")

# NOTE: we'll use this when we generate static lexer

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
   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/lemon-pg
   ARGS ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.y
   TARGET rphp-parser
   DEPENDS lemon
   OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp)

# Since rphp_grammar.c does not exists yet when cmake is run, mark
# it as generated
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp GENERATED)
# make grammar depend on template so it regenerates if we change it
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lemon/lempar.c)

####

set(PARSER_SRC_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/rphp_grammar.cpp
  pASTVisitors.cpp
  pSourceFile.cpp
  pLexer.cpp
  pParser.cpp
  pSourceModule.cpp
)


if (${RPHP_SHARED_LIBS})
 # SHARED 
 add_library( rphp-analysis SHARED ${PARSER_SRC_FILES} )
 target_link_libraries( rphp-analysis
 					   rphp-runtime
                        ${ICU_LIBRARIES}
                        ${ICU_I18_LIBRARIES}
                        )
else (${RPHP_SHARED_LIBS})
 # STATIC
 add_library( rphp-analysis ${PARSER_SRC_FILES} )
endif (${RPHP_SHARED_LIBS})

