
/*

driver for testing functions

*/

#include <cppunit/config/SourcePrefix.h>

#include "rphp/runtime/pOutputBuffer.h"
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

    CPPUNIT_ASSERT( *uBuffer.getUBuffer() == UnicodeString("1234512345") );

}

void runtimeTestCase::outputManager()
{

    pRuntimeEngine runtime;

    runtime.output << "testing 123";
    if (runtime.output.topBuffer()->getBufferType() == pOutputBuffer::bufTypeBinary)
        CPPUNIT_ASSERT( runtime.output.topBuffer()->getBBuffer()->compare("testing 123") == 0 );
    else
        CPPUNIT_ASSERT( runtime.output.topBuffer()->getUBuffer()->compare("testing 123") == 0 );

    runtime.output.freeAll();

}
