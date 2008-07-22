
#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "pRuntime.h"

#include "phashTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( phashTestCase );

void phashTestCase::basic()
{

    rphp::pVarP lu;

    // string key
    rphp::pVarP int1(new rphp::pVar(rphp::pInt(971)));
    testHash.insert("var-test", int1);
    CPPUNIT_ASSERT( testHash.getSize() == 1 );

    // int key
    rphp::pVarP str1(new rphp::pVar(rphp::pBString("int key 1")));
    testHash.insert(55, str1);
    CPPUNIT_ASSERT( testHash.getSize() == 2 );

    // next key
    rphp::pVarP str2(new rphp::pVar(rphp::pBString("next key")));
    testHash.insertNext(str2);
    CPPUNIT_ASSERT( testHash.getSize() == 3 );

    //testHash.varDump();

    // string key
    lu = testHash["var-test"];
    CPPUNIT_ASSERT( pVar_getVal_pInt(*int1) == pVar_getVal_pInt(*lu) );

    // int key
    lu = testHash[55];
    CPPUNIT_ASSERT( pVar_getVal_pBString(*str1) == pVar_getVal_pBString(*lu) );

    // next key
    lu = testHash[56];
    CPPUNIT_ASSERT( pVar_getVal_pBString(*str2) == pVar_getVal_pBString(*lu) );

    // not found
    lu = testHash["foo"];
    CPPUNIT_ASSERT( lu.get() == 0 );

}

/*
void phashTestCase::setUp()
{


}
*/


