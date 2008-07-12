
#ifndef PHASHTESTCASE_H
#define PHASHTESTCASE_H

#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case for rphp hashes
 *
 */

class phashTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( phashTestCase );
  CPPUNIT_TEST( basic );
  /*
  CPPUNIT_TEST( anotherExample );
  CPPUNIT_TEST( testAdd );
  CPPUNIT_TEST( testEquals );
  */
  CPPUNIT_TEST_SUITE_END();

/*
protected:
  double m_value1;
  double m_value2;
*/

public:
  void setUp();

protected:
  void basic();
  /*
  void anotherExample();
  void testAdd();
  void testEquals();
  */
};


#endif
