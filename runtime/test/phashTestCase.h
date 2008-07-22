
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
  CPPUNIT_TEST_SUITE_END();

protected:
  rphp::pHash testHash;

//public:
//  void setUp();

protected:
  void basic();

};


#endif
