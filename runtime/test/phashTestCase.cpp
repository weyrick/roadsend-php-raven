
#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "rphp_runtime.h"

#include "phashTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( phashTestCase );

void phashTestCase::basic()
{

    rphp::pvar* int1 = new rphp::pvar(rphp::pint(971));

    // php hash
    rphp::phash h;
    h.insert("var-test", int1);

    //std::cout << *(h["var-test"]) << std::endl;
    rphp::pvar* int2 = h["var-test"];

    CPPUNIT_ASSERT( pvar_getVal_int(*int1) == pvar_getVal_int(*int2) );

    h.varDump();

}

/*
void phashTestCase::setUp()
{

  m_value1 = 2.0;
  m_value2 = 3.0;

}
*/

