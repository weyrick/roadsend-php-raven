


IF(CPPUNIT_FOUND)
  MESSAGE(STATUS "cppunit found: configuring for tests")

  include_directories(${CPPUNIT_INCLUDE_DIR})

  # test apps
  add_executable( rphp-runtime-suite
                main.cpp
                phashTestCase.cpp
                pvarTestCase.cpp
                functionTestCase.cpp
                runtimeTestCase.cpp
               )


  add_executable( rphp-sizes rphp-sizes.cpp )
  target_link_libraries( rphp-runtime-suite ${CPPUNIT_LIBRARY} rphp-runtime ${ICU_LIBRARIES} ${ICU_IO_LIBRARIES})

ENDIF(CPPUNIT_FOUND)
