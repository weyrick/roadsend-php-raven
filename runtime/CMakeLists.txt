# runtime

MESSAGE( STATUS "runtime check" )

include_directories (${Boost_INCLUDE_DIRS} ${RPHP_INCLUDE_DIR} ${ICU_INCLUDE_DIRS})

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
  pRuntime.cpp
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

# STATIC
add_library( rphp-runtime ${RUNTIME_SRC_FILES} )

