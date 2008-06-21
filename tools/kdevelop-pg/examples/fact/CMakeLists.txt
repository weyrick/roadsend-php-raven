
PROJECT(fact_parser)

PROJECT(fact_parser)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})


# autogenerate the lexer and the parser

SET(parser_srcs)

FIND_PROGRAM(FLEX_EXECUTABLE
    NAMES flex
    PATHS /usr/bin
    DOC "Flex executable")


# Add command to generate the parser.
ADD_CUSTOM_COMMAND(
    OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/fact_ast.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/fact_parser.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/fact_parser.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/fact_visitor.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/fact_visitor.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/fact_default_visitor.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/fact_default_visitor.cpp"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/fact.g"
            "${CMAKE_CURRENT_SOURCE_DIR}/fact_lexer.h"
    COMMAND ${CMAKE_BINARY_DIR}/kdev-pg/kdev-pg
    ARGS    --output=fact
            "fact.g"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

SET(parser_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_default_visitor.cpp")

SET_SOURCE_FILES_PROPERTIES(
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_ast.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_parser.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_visitor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_default_visitor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/fact_default_visitor.cpp"
    GENERATED
    )


IF(FLEX_EXECUTABLE)
    # Add command to generate the lexer.
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/fact_lexer.cpp"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/fact_lexer.ll"
                "${CMAKE_CURRENT_SOURCE_DIR}/fact_lexer.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/fact_parser.h"
        COMMAND ${FLEX_EXECUTABLE}
        ARGS    -o"fact_lexer.cpp"
                "fact_lexer.ll"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

    SET(parser_srcs ${CMAKE_CURRENT_SOURCE_DIR}/fact_lexer.cpp ${parser_srcs})

    SET_SOURCE_FILES_PROPERTIES(
        ${CMAKE_CURRENT_SOURCE_DIR}/fact_lexer.cpp
        GENERATED
        )
ELSE(FLEX_EXECUTABLE)
    MESSAGE("--- Assuming existence of the generated lexer file fact_lexer.cpp")
    SET(parser_srcs ${CMAKE_CURRENT_SOURCE_DIR}/fact_lexer.cpp ${parser_srcs})
ENDIF(FLEX_EXECUTABLE)


SET(assembled_parser_srcs
    ${parser_srcs}
    decoder.cpp
    fact_io.cpp
)


# add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2)

# tell CMake to generate the executable
ADD_EXECUTABLE(fact-parser ${assembled_parser_srcs} main.cpp)

# in order to generate only the parser, call "make parser"
ADD_CUSTOM_TARGET(parser echo "Generating the parser"
    DEPENDS ${parser_srcs}
    )

# if you want it generated even if the file hasn't changed (but kdev-pg has,
# for example) then call "make parser-clean && make parser"
ADD_CUSTOM_TARGET(parser-clean echo "Cleaning up parser files")
ADD_CUSTOM_COMMAND(TARGET parser-clean
    PRE_BUILD
    COMMAND rm -f ${parser_srcs}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
