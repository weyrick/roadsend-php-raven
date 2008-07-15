
#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "pRuntime.h"

#include "phashTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( phashTestCase );

void phashTestCase::basic()
{

    rphp::pVarP int1(new rphp::pVar(rphp::pInt(971)));

    // php hash
    rphp::pHash h;
    h.insert("var-test", int1);

    //std::cout << *(h["var-test"]) << std::endl;
    rphp::pVarP int2 = h["var-test"];

    CPPUNIT_ASSERT( pVar_getVal_int(*int1) == pVar_getVal_int(*int2) );

    // not found
    int2 = h["foo"];
    CPPUNIT_ASSERT( int2.get() == 0 );

    h.varDump();

}

/*
void phashTestCase::setUp()
{

  m_value1 = 2.0;
  m_value2 = 3.0;

}
*/

