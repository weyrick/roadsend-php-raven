
#include "rphp/runtime/pHash.h"
#include "rphp/runtime/pVar.h"

using namespace rphp;

void testScalar(int reps) {

    pVar foo;
    for(int i = 0; i < reps; i++) {
        foo = pNull;
        foo = pTrue;
        foo = 5;
        foo = 5.5;
    }

}

void testBString(int reps) {

    pVar foo;
    for(int i = 0; i < reps; i++) {
        foo = pBString("foo");
        foo = pNull;
    }

}

void testBStringLong(int reps) {
    
    pVar foo;
    for(int i = 0; i < reps; i++) {
        foo = pBString("012345678901234567890123456789");
        foo = pNull;
    }

}

void testUString(int reps) {
    
    pVar foo;
    for(int i = 0; i < reps; i++) {
        foo = pUStringP("foo");
        foo = pNull;
    }

}

void testUStringLong(int reps) {
    
    pVar foo;
    for(int i = 0; i < reps; i++) {
        foo = pUStringP("012345678901234567890123456789");
        foo = pNull;
    }

}

void testHashEmpty(int reps) {
    
    pVar foo;
    for(int i = 0; i < reps; i++) {
        foo.newEmptyHash();
        foo = pNull;
    }

}

void testHash(int reps, int size) {
    
    pVar foo;
    for(int i = 0; i < reps; i++) {
        foo.newEmptyHash();
        for (int j = 0; j < size; j++) {
            foo.getHashP()->insert(j,42);
        }
        foo = pNull;
    }

}

int main(void) {

    int reps=500000;
    testScalar(reps);
    testBString(reps);
    testBStringLong(reps);
    testUString(reps);
    testUStringLong(reps);
    testHashEmpty(reps);
    testHash(reps, 5);

}
 
