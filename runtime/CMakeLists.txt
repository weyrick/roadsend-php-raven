# runtime

MESSAGE( STATUS "runtime check" )

include_directories (${Boost_INCLUDE_DIRS} ${RPHP_RUNTIME_INCLUDE_DIR})

set(RUNTIME_SRC_FILES
  pResource.cpp
  pHash.cpp
  pObject.cpp
  pTypes.cpp
  pRuntime.cpp
  pOutputBuffer.cpp
  pOutputManager.cpp
  pFunctionManager.cpp
  pFunctionSig.cpp
  pExtBase.cpp
  pExtManager.cpp
  standard/pStandardExt.cpp
)

add_library( rphp-runtime SHARED ${RUNTIME_SRC_FILES} )
target_link_libraries( rphp-runtime ${ICU_LIBRARIES} ${ICU_I18_LIBRARIES} ${ICU_IO_LIBRARIES} )
