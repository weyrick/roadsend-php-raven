
/*

driver for testing functions

*/

#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "pOutputBuffer.h"
#include "runtimeTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( runtimeTestCase );

using namespace rphp;

// driver
void runtimeTestCase::outputBuffer()
{

    pOutputBuffer uBuffer(pOutputBuffer::bufTypeUnicode);

    uBuffer << pUString("12345");
    CPPUNIT_ASSERT( uBuffer.length() == 5 );

    uBuffer << pBString("12345");
    CPPUNIT_ASSERT( uBuffer.length() == 10 );

    CPPUNIT_ASSERT( *uBuffer.getUBuffer() == pUString("1234512345") );

}
