
#ifndef HASHTESTCASE_H
#define HASHTESTCASE_H

#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case for rphp pvar
 *
 */

class pvarTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( pvarTestCase );
  CPPUNIT_TEST( basic );
  CPPUNIT_TEST_SUITE_END();

/*
protected:
  pvar m_value1;
  pvar m_value2;

public:
  void setUp();
*/

protected:
  void basic();

};


#endif
