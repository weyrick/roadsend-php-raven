# runtime IR interface

MESSAGE( STATUS "c-runtime check" )

add_custom_command(
   COMMAND ${LLVM_GXX_EXECUTABLE}
   ARGS -fPIC -emit-llvm -I${Boost_INCLUDE_DIRS} -I${RPHP_INCLUDE_DIR} -c -o ${RPHP_LIB_DIR}/c-runtime.ir ${CMAKE_CURRENT_SOURCE_DIR}/c-runtime.cpp
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/c-runtime.cpp
   OUTPUT ${RPHP_LIB_DIR}/c-runtime.ir
   )

add_custom_target(c-runtime-ir ALL
                  DEPENDS ${RPHP_LIB_DIR}/c-runtime.ir
                  COMMENT "building C runtime IR"
                  )

