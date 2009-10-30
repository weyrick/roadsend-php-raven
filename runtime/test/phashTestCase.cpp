
#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "rphp/runtime/pHash.h"

#include "phashTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( phashTestCase );

using namespace rphp;

void phashTestCase::basic()
{

    // ** INSERT **

    // string key
    pVar int1(971);
    testHash.insert("var-test", int1);
    CPPUNIT_ASSERT( testHash.size() == 1 );

    // int key
    pVar str1("int key 1");
    testHash.insert(55, str1);
    CPPUNIT_ASSERT( testHash.size() == 2 );

    // next key
    pVar str2("next key");
    testHash.insertNext(str2);
    CPPUNIT_ASSERT( testHash.size() == 3 );

    testHash.insert("var-test2", -215);
    CPPUNIT_ASSERT( testHash.size() == 4 );

    // ** LOOKUP **
    pVar lu;

    // string key
    lu = testHash["var-test"];
    CPPUNIT_ASSERT( int1.getInt() == lu.getInt() );

    // int key
    lu = testHash[55];
    CPPUNIT_ASSERT( str1.getBString() == lu.getBString() );

    // next key
    lu = testHash[56];
    CPPUNIT_ASSERT( str2.getBString() == lu.getBString() );

    // not found
    lu = testHash["foo"];
    CPPUNIT_ASSERT( lu.isNull() );

    // ** REMOVE **
    pHash::size_type rc = testHash.remove("var-test");
    CPPUNIT_ASSERT( rc == 1 );
    CPPUNIT_ASSERT( testHash.size() == 3 );

    rc = testHash.remove("doesn't exist");
    CPPUNIT_ASSERT( rc == 0 );
    CPPUNIT_ASSERT( testHash.size() == 3 );

    //testHash.varDump();

}

/*
void phashTestCase::setUp()
{


}
*/


