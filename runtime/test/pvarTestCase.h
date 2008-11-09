
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
  CPPUNIT_TEST( test_pInt );
  CPPUNIT_TEST( test_pFloat );
  CPPUNIT_TEST( test_pHash );
  // pBString
  CPPUNIT_TEST( test_pUString );
  // object
  // resource
  CPPUNIT_TEST( test_pVarRef );
  CPPUNIT_TEST( test_visitor );
  CPPUNIT_TEST_SUITE_END();

protected:
  void test_pNull();
  void test_pBool();
  void test_pInt();
  void test_pFloat();
  void test_pHash();
  void test_pVarRef();
  void test_pUString();
  void test_visitor();

};


#endif
