
/*

driver for testing functions

*/

#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "functionTestCase.h"
#include "pFunctionManager.h"

CPPUNIT_TEST_SUITE_REGISTRATION( functionTestCase );

using namespace rphp;

// driver
void functionTestCase::basic()
{

    pVar testString(pBString("a new era has dawned"));

    pVar testStringSize = runtime.functionManager->invoke("strlen", testString);

    CPPUNIT_ASSERT( testStringSize.getInt() == 20 );

}
