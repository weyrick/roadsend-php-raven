
/*

driver for testing functions

*/

#include <cppunit/config/SourcePrefix.h>

#include "functionTestCase.h"
#include "rphp/runtime/pFunctionManager.h"

CPPUNIT_TEST_SUITE_REGISTRATION( functionTestCase );

using namespace rphp;

// driver
void functionTestCase::basic()
{

    pVar testString(pBString("a new era has dawned"));

    pVar testStringSize = runtime.functionManager->invoke("strlen", testString);
    CPPUNIT_ASSERT( testStringSize.isInt() );
    CPPUNIT_ASSERT( testStringSize.getInt() == 20 );

    pVar posTest = runtime.functionManager->invoke("strpos", testString, "era", 0);
    CPPUNIT_ASSERT( !posTest.isNull() );
    CPPUNIT_ASSERT( posTest.isInt() );
    CPPUNIT_ASSERT( posTest.getInt() == 6 );

    //runtime.functionManager->invoke("var_dump", testString);

}
