# runtime IR interface

MESSAGE( STATUS "c-runtime check" )

add_custom_command(
   COMMAND ${LLVM_GXX_EXECUTABLE}
   ARGS -fPIC -emit-llvm -I${ICU_INCLUDE_DIRS} -I${Boost_INCLUDE_DIRS} -I${RPHP_INCLUDE_DIR} -I${LLVM_INCLUDE_DIR} -c -o ${RPHP_LIB_DIR}/c-runtime.bc ${CMAKE_CURRENT_SOURCE_DIR}/c-runtime.cpp
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/c-runtime.cpp
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/c-runtime.h
   # this list comes from the include list in c-runtime.cpp and c-runtime.h
   # since cmake doesn't do that for us here
   DEPENDS ${CMAKE_SOURCE_DIR}/include/rphp/runtime/pErrorManager.h
   DEPENDS ${CMAKE_SOURCE_DIR}/include/rphp/runtime/pFunctionManager.h
   DEPENDS ${CMAKE_SOURCE_DIR}/include/rphp/runtime/pHash.h
   DEPENDS ${CMAKE_SOURCE_DIR}/include/rphp/runtime/pObject.h
   DEPENDS ${CMAKE_SOURCE_DIR}/include/rphp/runtime/pRuntimeError.h
   #
   DEPENDS ${CMAKE_SOURCE_DIR}/include/rphp/runtime/pVar.h
   DEPENDS ${CMAKE_SOURCE_DIR}/include/rphp/runtime/pRuntimeEngine.h
   #
   OUTPUT ${RPHP_LIB_DIR}/c-runtime.bc
   )

add_custom_target(c-runtime-ir ALL
                  DEPENDS ${RPHP_LIB_DIR}/c-runtime.bc
                  COMMENT "building C runtime IR"
                  )

