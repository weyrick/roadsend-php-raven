
/*

driver for testing functions

*/

#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "functionTestCase.h"
#include "pFunctionManager.h"

CPPUNIT_TEST_SUITE_REGISTRATION( functionTestCase );


// driver
void functionTestCase::basic()
{

    rphp::pVar testString = rphp::pBString("a new era has dawned");

    rphp::pVar testStringSize = runtime.functionManager->invoke("strlen", testString);

    CPPUNIT_ASSERT( testStringSize.getInt() == 20 );

}
