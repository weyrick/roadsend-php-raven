
#ifndef FUNCTIONTESTCASE_H
#define FUNCTIONTESTCASE_H

#include <cppunit/extensions/HelperMacros.h>
#include "rphp/runtime/pRuntime.h"

/*
 * A test case for the runtime
 *
 */

class runtimeTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( runtimeTestCase );
  CPPUNIT_TEST( outputBuffer );
  CPPUNIT_TEST_SUITE_END();

protected:
  rphp::pRuntimeEngine runtime;

  /*
public:
  void setUp();
*/

protected:
  void outputBuffer();

};


#endif
