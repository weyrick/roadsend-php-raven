


IF(CPPUNIT_FOUND)

  MESSAGE(STATUS "cppunit found: configuring runtime tests")

  include_directories(${CPPUNIT_INCLUDE_DIR})

  set(RPHP_RUNTIME_SUITE_SRC_FILES
    main.cpp
    phashTestCase.cpp
    pvarTestCase.cpp
    functionTestCase.cpp
    runtimeTestCase.cpp
  )

  # test apps
  add_executable( rphp-runtime-suite ${RPHP_RUNTIME_SUITE_SRC_FILES})
   
  target_link_libraries( rphp-runtime-suite 
                         ${CPPUNIT_LIBRARY} 
                         rphp-runtime
                         ${ICU_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES}
                        )

ENDIF(CPPUNIT_FOUND)

set(RPHP_SIZES_SRC_FILES
  rphp-sizes.cpp)

add_executable( rphp-sizes rphp-sizes.cpp )

set(PVAR_SPEED_SRC_FILES 
  rphp-pVar-speed.cpp)

add_executable( rphp-pVar-speed rphp-pVar-speed.cpp )

set_source_files_properties( ${PVAR_SPEED_SRC_FILES} ${RPHP_SIZES_SRC_FILES} ${RPHP_RUNTIME_SUITE_SRC_FILES}
                             PROPERTIES COMPILE_FLAGS ${LLVM_COMPILE_FLAGS}
                            )

target_link_libraries( rphp-pVar-speed 
                       rphp-runtime
                       ${ICU_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES}
                    )
