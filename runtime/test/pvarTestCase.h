
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
  CPPUNIT_TEST( test_pBigInt );
  CPPUNIT_TEST( test_pFloat );
  CPPUNIT_TEST( test_pHash );
  CPPUNIT_TEST( test_pBString );
  CPPUNIT_TEST( test_pUString );
  CPPUNIT_TEST( test_pObject );
  CPPUNIT_TEST( test_pResource );
  CPPUNIT_TEST( test_pVarBoxed );
  CPPUNIT_TEST( test_visitor );
  CPPUNIT_TEST( test_conversion );
  CPPUNIT_TEST( test_math_operators );
  CPPUNIT_TEST_SUITE_END();

protected:
  void test_pNull();
  void test_pBool();
  void test_pInt();
  void test_pBigInt();
  void test_pFloat();
  void test_pHash();
  void test_pBString();
  void test_pUString();
  void test_pObject();
  void test_pResource();
  void test_pVarBoxed();
  void test_visitor();
  void test_conversion();
  void test_math_operators();

};


#endif
