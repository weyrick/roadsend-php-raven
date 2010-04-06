# runtime

MESSAGE( STATUS "runtime check" )

set(RUNTIME_SRC_FILES
  #pUString.cpp
  pErrorManager.cpp
  pSupport.cpp
  pVar.cpp
  pVarOperators.cpp
  pTypeOperators.cpp
  pResource.cpp
  pHash.cpp
  pObject.cpp
  pClass.cpp
  pRuntimeEngine.cpp
  pOutputBuffer.cpp
  pOutputManager.cpp
  pClassManager.cpp
  pFunctionManager.cpp
  pFunction.cpp
  pExtBase.cpp
  pExtManager.cpp
  standard/pStandardExt.cpp
)

add_subdirectory(test)

set_source_files_properties( ${RUNTIME_SRC_FILES}
                             PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                            )


add_library( rphp-runtime SHARED ${RUNTIME_SRC_FILES} )
target_link_libraries( rphp-runtime ${ICU_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES} ) 
