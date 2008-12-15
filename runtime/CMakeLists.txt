# runtime

MESSAGE( STATUS "runtime check" )

include_directories (${Boost_INCLUDE_DIRS} ${RPHP_INCLUDE_DIR})

set(RUNTIME_SRC_FILES
  pUString.cpp
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
add_library( rphp-runtime SHARED ${RUNTIME_SRC_FILES} )
target_link_libraries( rphp-runtime ${ICU_LIBRARIES} ${ICU_I18_LIBRARIES} ${ICU_IO_LIBRARIES} )
