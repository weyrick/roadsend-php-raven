# runtime IR interface

MESSAGE( STATUS "c-runtime check" )

add_custom_command(
   COMMAND ${LLVM_GXX_EXECUTABLE}
   ARGS -fPIC -emit-llvm -I${Boost_INCLUDE_DIRS} -I${RPHP_INCLUDE_DIR} -c -o ${RPHP_LIB_DIR}/c-runtime.bc ${CMAKE_CURRENT_SOURCE_DIR}/c-runtime.cpp
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/c-runtime.cpp
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/c-runtime.h
   OUTPUT ${RPHP_LIB_DIR}/c-runtime.bc
   )

add_custom_target(c-runtime-ir ALL
                  DEPENDS ${RPHP_LIB_DIR}/c-runtime.bc
                  COMMENT "building C runtime IR"
                  )

