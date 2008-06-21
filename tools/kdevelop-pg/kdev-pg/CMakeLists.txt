
PROJECT(kdevpg)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})


SET(kdevpg_srcs
    ASBeautifier.cpp
    ASFormatter.cpp
    kdev-pg.cpp
    kdev-pg-visitor.cpp
    kdev-pg-default-visitor.cpp
    kdev-pg-pretty-printer.cpp
    kdev-pg-global.cpp
    kdev-pg-code-gen.cpp
    kdev-pg-ast-gen.cpp
    kdev-pg-visitor-gen.cpp
    kdev-pg-visitor-bits-gen.cpp
    kdev-pg-default-visitor-gen.cpp
    kdev-pg-default-visitor-bits-gen.cpp
    kdev-pg-serialize-visitor-gen.cpp
    kdev-pg-debug-visitor-gen.cpp
    kdev-pg-beautifier.cpp
    kdev-pg-checker.cpp
    kdev-pg-main.cpp
    kdev-pg-generate.cpp
    kdev-pg-first.cpp
    kdev-pg-follow.cpp
    kdev-pg-clone-tree.cpp)


# Custom support for flex/bison
SET(parser_srcs)

FIND_PROGRAM(FLEX_EXECUTABLE
    NAMES flex
    PATHS /usr/bin
    DOC "Flex executable")

FIND_PROGRAM(BISON_EXECUTABLE
    NAMES bison
    PATHS /usr/bin
    DOC "bison executable")

IF(BISON_EXECUTABLE)
    # Add command to run the parser.
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-parser.cc"
                "${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-parser.hh"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/kdev-pg-parser.yy"
        COMMAND ${BISON_EXECUTABLE}
        ARGS    -o"${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-parser.cc"
                -d "${CMAKE_CURRENT_SOURCE_DIR}/kdev-pg-parser.yy"
        )

    SET(parser_srcs ${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-parser.cc)
    SET_SOURCE_FILES_PROPERTIES(
        ${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-parser.cc
        ${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-parser.hh
        GENERATED
        )
ELSE(BISON_EXECUTABLE)
    MESSAGE("'bison' was not found!")
    MESSAGE("Assuming existence of the generated parser files kdev-pg-parser.[hh,cc]")
    SET(parser_srcs ${CMAKE_CURRENT_SOURCE_DIR}/kdev-pg-parser.cc)
ENDIF(BISON_EXECUTABLE)

IF(FLEX_EXECUTABLE)
    # Add command to run the lexer.
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-lexer.cc"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/kdev-pg-lexer.ll"
                "${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-parser.hh"
        COMMAND ${FLEX_EXECUTABLE}
        ARGS    -o"${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-lexer.cc"
                "${CMAKE_CURRENT_SOURCE_DIR}/kdev-pg-lexer.ll"
        )

    SET(parser_srcs ${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-lexer.cc ${parser_srcs})
    SET_SOURCE_FILES_PROPERTIES(
        ${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-lexer.cc
        GENERATED
        )
ELSE(FLEX_EXECUTABLE)
    MESSAGE("'flex' was not found!")
    MESSAGE("Assuming existence of the generated lexer file kdev-pg-lexer.cc")
    SET(parser_srcs ${CMAKE_CURRENT_SOURCE_DIR}/kdev-pg-lexer.cc)
ENDIF(FLEX_EXECUTABLE)


# tell CMake to generate the executable
ADD_EXECUTABLE(kdev-pg ${parser_srcs} ${kdevpg_srcs})
INSTALL(TARGETS kdev-pg DESTINATION bin )

# in order to generate only the parser, call "make parser"
ADD_CUSTOM_TARGET(parser echo "Generating the parser"
    DEPENDS ${parser_srcs}
    )
