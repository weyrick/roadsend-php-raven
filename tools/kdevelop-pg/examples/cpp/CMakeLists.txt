
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})


# autogenerate the codemodel

SET(codemodel_srcs)


# Add command to generate the codemodel.
ADD_CUSTOM_COMMAND(
    OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel_fwd.h"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel.cm"
    COMMAND ${CMAKE_BINARY_DIR}/kdev-cmg/kdev-cmg
    ARGS    --language=cpp
            "cpp_codemodel.cm"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

SET(codemodel_srcs
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel_fwd.h)

SET_SOURCE_FILES_PROPERTIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_codemodel_fwd.h
    GENERATED
    )


# add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2)

# in order to generate only the parser, call "make codemodel"
ADD_CUSTOM_TARGET(codemodel echo "Generating the codemodel"
    DEPENDS ${codemodel_srcs}
    )

