
#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "rphp_runtime.h"

#include "phashTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( phashTestCase );

void phashTestCase::basic()
{
    /*
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, 1.1, 0.05 );
  CPPUNIT_ASSERT( 1 == 0 );
  CPPUNIT_ASSERT( 1 == 1 );
    */

    rphp::pvar u;

    // php hash
    rphp::phash h;
    h.insert("var-test", rphp::pint(971));
    rphp::pvar hole = rphp::pfloat(1.234);
    h.insert("var-test2", hole);
    std::cout << h;
    h.varDump();
    u = h;
    std::cout << u << std::endl;    
    
}

/*
void phashTestCase::anotherExample()
{
  CPPUNIT_ASSERT (1 == 2);
}
*/
void phashTestCase::setUp()
{
    /*
  m_value1 = 2.0;
  m_value2 = 3.0;
    */
}

/*
void phashTestCase::testAdd()
{
  double result = m_value1 + m_value2;
  CPPUNIT_ASSERT( result == 6.0 );
}


void phashTestCase::testEquals()
{
  long* l1 = new long(12);
  long* l2 = new long(12);

  CPPUNIT_ASSERT_EQUAL( 12, 12 );
  CPPUNIT_ASSERT_EQUAL( 12L, 12L );
  CPPUNIT_ASSERT_EQUAL( *l1, *l2 );

  delete l1;
  delete l2;

  CPPUNIT_ASSERT( 12L == 12L );
  CPPUNIT_ASSERT_EQUAL( 12, 13 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 12.0, 11.99, 0.5 );
}
*/
