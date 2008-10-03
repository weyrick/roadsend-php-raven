# runtime IR interface

MESSAGE( STATUS "runtime-ir check" )

add_custom_command(
   COMMAND ${LLVM_GXX_EXECUTABLE}
   ARGS -emit-llvm -I${Boost_INCLUDE_DIRS} -I${RPHP_RUNTIME_INCLUDE_DIR} -c -o ${CMAKE_CURRENT_SOURCE_DIR}/irRuntime.ir ${CMAKE_CURRENT_SOURCE_DIR}/lib/irRuntime.cpp
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lib/irRuntime.cpp
   OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/irRuntime.ir
   )

add_custom_target(runtime-ir ALL
                  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/irRuntime.ir
                  COMMENT "building runtime IR interface"
                  )

