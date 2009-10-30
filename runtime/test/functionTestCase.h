
#ifndef FUNCTIONTESTCASE_H
#define FUNCTIONTESTCASE_H

#include <cppunit/extensions/HelperMacros.h>

#include "rphp/runtime/pRuntimeEngine.h"

/*
 * A test case for php functions
 *
 */

class functionTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( functionTestCase );
  CPPUNIT_TEST( basic );
  CPPUNIT_TEST_SUITE_END();

protected:
  rphp::pRuntimeEngine runtime;

  /*
public:
  void setUp();
*/

protected:
  void basic();

};


#endif
