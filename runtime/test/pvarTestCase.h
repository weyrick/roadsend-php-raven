
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
  CPPUNIT_TEST( test_pNull );
  CPPUNIT_TEST( test_pBool );
  // int
  // float
  CPPUNIT_TEST( test_pHash );
  // object
  // resource
  // ref
  // visitor
  CPPUNIT_TEST_SUITE_END();

protected:
  void test_pNull();
  void test_pBool();
  void test_pHash();

};


#endif
