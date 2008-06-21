
PROJECT(csharp_parser)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})


# autogenerate the lexer and the two parsers (csharp and csharp_pp)

SET(parser_srcs)

FIND_PROGRAM(FLEX_EXECUTABLE
    NAMES flex
    PATHS /usr/bin
    DOC "Flex executable")


# Add commands to generate the parsers.
ADD_CUSTOM_COMMAND(
    OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/csharp_ast.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_parser.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_parser.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_visitor.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_visitor.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_default_visitor.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_default_visitor.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_serialize_visitor.h"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/csharp.g"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_lexer.h"
    COMMAND ${CMAKE_BINARY_DIR}/kdev-pg/kdev-pg
    ARGS    --output=csharp
            --serialize-visitor
            "csharp.g"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
ADD_CUSTOM_COMMAND(
    OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_ast.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_parser.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_parser.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_visitor.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_visitor.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_default_visitor.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_default_visitor.cpp"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp.g"
            "${CMAKE_CURRENT_SOURCE_DIR}/csharp_lexer.h"
    COMMAND ${CMAKE_BINARY_DIR}/kdev-pg/kdev-pg
    ARGS    --output=csharp_pp
            "csharp_pp.g"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

SET(parser_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_default_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_default_visitor.cpp")

SET_SOURCE_FILES_PROPERTIES(
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_ast.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_parser.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_visitor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_default_visitor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_default_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_serialize_visitor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_ast.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_parser.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_visitor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_default_visitor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_default_visitor.cpp"
    GENERATED
    )

IF(FLEX_EXECUTABLE)
    # Add command to generate the lexer.
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/csharp_lexer.cpp"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/csharp_lexer.ll"
                "${CMAKE_CURRENT_SOURCE_DIR}/csharp_lexer.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/csharp_parser.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/csharp_pp_parser.h"
        COMMAND ${FLEX_EXECUTABLE}
        ARGS    -o"csharp_lexer.cpp"
                "csharp_lexer.ll"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

    SET(parser_srcs ${CMAKE_CURRENT_SOURCE_DIR}/csharp_lexer.cpp ${parser_srcs})

    SET_SOURCE_FILES_PROPERTIES(
        "${CMAKE_CURRENT_SOURCE_DIR}/csharp_lexer.cpp"
        GENERATED
        )
ELSE(FLEX_EXECUTABLE)
    MESSAGE("--- Assuming existence of the generated lexer file csharp_lexer.cpp")
    SET(parser_srcs ${parser_srcs} ${CMAKE_CURRENT_SOURCE_DIR}/csharp_lexer.cpp)
ENDIF(FLEX_EXECUTABLE)


SET(assembled_parser_srcs
    ${parser_srcs}
    decoder.cpp
    csharp_io.cpp
    csharp_pp_scope.cpp
    csharp_pp_handler_visitor.cpp
)



# add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2)

# tell CMake to generate the executable
ADD_EXECUTABLE(csharp-parser ${assembled_parser_srcs} main.cpp)
ADD_EXECUTABLE(csharp-serializer ${assembled_parser_srcs} main_serialize.cpp)

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
