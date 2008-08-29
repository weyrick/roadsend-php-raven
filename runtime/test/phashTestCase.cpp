
#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "pRuntime.h"

#include "phashTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( phashTestCase );

void phashTestCase::basic()
{

    rphp::pVarP lu;

    // ** INSERT **

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

    // ** LOOKUP **
    
    // string key
    lu = testHash["var-test"];
    CPPUNIT_ASSERT( (*int1).getInt() == (*lu).getInt() );

    // int key
    lu = testHash[55];
    CPPUNIT_ASSERT( (*str1).getBString() == (*lu).getBString() );

    // next key
    lu = testHash[56];
    CPPUNIT_ASSERT( (*str2).getBString() == (*lu).getBString() );

    // not found
    lu = testHash["foo"];
    CPPUNIT_ASSERT( lu.get() == 0 );

    // ** REMOVE **
    rphp::pHash::size_type rc = testHash.remove("var-test");
    CPPUNIT_ASSERT( rc == 1 );
    CPPUNIT_ASSERT( testHash.getSize() == 2 );
    
    rc = testHash.remove("doesn't exist");
    CPPUNIT_ASSERT( rc == 0 );
    CPPUNIT_ASSERT( testHash.getSize() == 2 );
    
    testHash.varDump();
    
}

/*
void phashTestCase::setUp()
{


}
*/


