
#ifndef CPP_UNIT_EXAMPLETESTCASE_H
#define CPP_UNIT_EXAMPLETESTCASE_H

#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case for rphp hashes
 *
 */

class HashTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( HashTestCase );
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
