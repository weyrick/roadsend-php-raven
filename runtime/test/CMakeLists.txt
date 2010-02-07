


IF(CPPUNIT_FOUND)

  MESSAGE(STATUS "cppunit found: configuring runtime tests")

  include_directories(${CPPUNIT_INCLUDE_DIR})

  # test apps
  add_executable( rphp-runtime-suite
                main.cpp
                phashTestCase.cpp
                pvarTestCase.cpp
                functionTestCase.cpp
                runtimeTestCase.cpp
               )
   
  target_link_libraries( rphp-runtime-suite 
                         ${CPPUNIT_LIBRARY} 
                         rphp-runtime
                         ${ICU_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES}
                        )

ENDIF(CPPUNIT_FOUND)

add_executable( rphp-sizes rphp-sizes.cpp )

add_executable( rphp-pVar-speed rphp-pVar-speed.cpp )

target_link_libraries( rphp-pVar-speed 
                       rphp-runtime
                       ${ICU_LIBRARIES} ${GMP_LIBRARIES} ${GMPXX_LIBRARIES}
                    )
