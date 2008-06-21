
PROJECT(kdevcmg)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
#INCLUDE_DIRECTORIES(${QT_QTCORE_INCLUDES})


SET(kdevcmg_srcs
    kdev-cmg-main.cpp
    kdev-cmg-initialize-ast.cpp
    kdev-cmg-forward-declarations-gen.cpp
    kdev-cmg-declarations-gen.cpp
    kdev-cmg-implementation-gen.cpp
    kdev-cmg-chameleon-gen.cpp
    kdev-cmg-generate.cpp
    kdev-cmg-string-tools.cpp
    kdev-cmg-beautifier.cpp
    ASBeautifier.cpp
    ASFormatter.cpp)


SET(parser_srcs)

FIND_PROGRAM(FLEX_EXECUTABLE
    NAMES flex
    PATHS /usr/bin
    DOC "Flex executable")


# Add command to run the parser.
ADD_CUSTOM_COMMAND(
    OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_ast.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_parser.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_parser.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_visitor.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_visitor.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_default_visitor.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_default_visitor.cpp"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/kdev-cmg-parser.g"
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../kdev-pg/kdev-pg
    ARGS    --output=kdevcmg
            "kdev-cmg-parser.g"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

SET(parser_srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_default_visitor.cpp")

SET_SOURCE_FILES_PROPERTIES(
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_ast.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_parser.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_visitor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_visitor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_default_visitor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_default_visitor.cpp"
    GENERATED
    )


IF(FLEX_EXECUTABLE)
    # Add command to run the lexer.
    ADD_CUSTOM_COMMAND(
        OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/kdev-cmg-lexer.cc"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/kdev-cmg-lexer.ll"
                "${CMAKE_CURRENT_SOURCE_DIR}/kdevcmg_parser.h"
        COMMAND ${FLEX_EXECUTABLE}
        ARGS    -o"${CMAKE_CURRENT_BINARY_DIR}/kdev-cmg-lexer.cc"
                "${CMAKE_CURRENT_SOURCE_DIR}/kdev-cmg-lexer.ll"
        )

    SET(parser_srcs
        ${CMAKE_CURRENT_BINARY_DIR}/kdev-cmg-lexer.cc
        ${parser_srcs}
        )
    SET_SOURCE_FILES_PROPERTIES(
        ${CMAKE_CURRENT_BINARY_DIR}/kdev-cmg-lexer.cc
        GENERATED
        )
ELSE(FLEX_EXECUTABLE)
    MESSAGE("Assuming existence of the generated lexer file kdev-cmg-lexer.cc")
    SET(parser_srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/kdev-cmg-lexer.cc
        ${parser_srcs}
        )
ENDIF(FLEX_EXECUTABLE)


SET(assembled_parser_srcs ${parser_srcs} kdev-cmg-io.cpp kdev-cmg-decoder.cpp)


# add definitions, compiler switches, etc.
#ADD_DEFINITIONS(-Wall -O2)
ADD_DEFINITIONS(-Wall -g)

# tell CMake to generate the executable
ADD_EXECUTABLE(kdev-cmg ${assembled_parser_srcs} ${kdevcmg_srcs})
INSTALL(TARGETS kdev-cmg DESTINATION bin )

#TARGET_LINK_LIBRARIES(kdev-cmg ${QT_QTCORE_LIBRARY_DEBUG})
#TARGET_LINK_LIBRARIES(kdev-cmg ${QT_QTCORE_LIBRARY_RELEASE})

# in order to generate only the parser, call "make parser"
ADD_CUSTOM_TARGET(parser echo "Generating the parser"
    DEPENDS ${parser_srcs}
    )
