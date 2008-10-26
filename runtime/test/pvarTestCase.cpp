
/*

driver for testing pVars

*/

#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "pRuntime.h"
#include "pvarTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( pvarTestCase );

using namespace rphp;

// ** NULL ** 
void pvarTestCase::test_pNull()
{
    
    // default null
    pVar a;
    CPPUNIT_ASSERT( a.isNull() );

    // null construct
    pVar a1(pNull);
    CPPUNIT_ASSERT( a1.isNull() );
    
    // null assignment
    a = pNull;
    CPPUNIT_ASSERT( a.isNull() );
    
    // null evaluates to false
    CPPUNIT_ASSERT( a.evalAsBool() == false );

    // null copy
    pVar a2(1);
    a2 = a;
    CPPUNIT_ASSERT( a2.isNull() );

    // null convert
    pVar a3(1);
    a3.convertToNull();
    CPPUNIT_ASSERT( a3.isNull() );

}

// ** BOOL **
void pvarTestCase::test_pBool() {

    // bool construct
    pVar b1(pTrue);
    CPPUNIT_ASSERT( b1.getBool() == pTrue );
    CPPUNIT_ASSERT( b1.evalAsBool() );

    pVar b2(pFalse);
    CPPUNIT_ASSERT( b2.getBool() == pFalse );
    CPPUNIT_ASSERT( !b2.evalAsBool() );

    // bool assign
    pVar b3(1);
    CPPUNIT_ASSERT( b3.isInt() );
    b3 = pTrue;
    CPPUNIT_ASSERT( b3.isBool() );
    CPPUNIT_ASSERT( b3.evalAsBool() );

    // bool copy
    pVar b4(1);
    CPPUNIT_ASSERT( b4.isInt() );
    b4 = b3;
    CPPUNIT_ASSERT( b4.isBool() );
    CPPUNIT_ASSERT( b4.evalAsBool() );

    // bool convert
    // TODO: add more convert cases here

    // BOOL conversion
    pVar b6;
    CPPUNIT_ASSERT( b6.isNull() );
    b6.convertToBool();
    CPPUNIT_ASSERT( b6.isBool() );
    CPPUNIT_ASSERT( !b6.evalAsBool() );

    // INT conversion
    pVar b5(1);
    CPPUNIT_ASSERT( b5.isInt() );
    b5.convertToBool();
    CPPUNIT_ASSERT( b5.isBool() );
    CPPUNIT_ASSERT( b5.evalAsBool() );
    b5 = 0;
    CPPUNIT_ASSERT( b5.isInt() );
    b5.convertToBool();
    CPPUNIT_ASSERT( b5.isBool() );
    CPPUNIT_ASSERT( !b5.evalAsBool() );
    

}

// ** INT **
void pvarTestCase::test_pInt() {

    pVar i(123456);
    CPPUNIT_ASSERT( i.isInt() );
    CPPUNIT_ASSERT( i.getInt() == 123456 );

    i = -987654;
    CPPUNIT_ASSERT( i.getInt() == -987654 );

}

// ** FLOAT **
void pvarTestCase::test_pFloat() {

    pVar i(123.1234);
    CPPUNIT_ASSERT( i.isFloat() );
    CPPUNIT_ASSERT( i.getFloat() == 123.1234 );

    i = -8.34233;
    CPPUNIT_ASSERT( i.getFloat() == -8.34233 );

}


// ** HASH **
pVar hashObserver(pVar H) {
    // use const pHash, which will not copy data
    CPPUNIT_ASSERT( H.getConstHash()->getSize() == 1 );
    return H;
}

pVar hashMutator(pVar H) {
    // use non const pHash which copies data
    CPPUNIT_ASSERT( H.getHash()->getSize() == 1 );
    return H;
}

void pvarTestCase::test_pHash() {

    // NOTE: this mostly tests pHash functionality as it relates to pVar
    // see phashTestCase for the full pHash testsuite
    pVar h1;
    h1.newEmptyHash();
    CPPUNIT_ASSERT( h1.isHash() );

    h1.getHash()->insert("foo", 5);
    CPPUNIT_ASSERT( h1.getHash()->getSize() == 1 );

    // test copy on write
    pVar h2(h1);
    CPPUNIT_ASSERT( h2.getConstHash()->getSize() == 1 );
    // this calls CowPtr::operator==, which calls boost::shared_ptr::operator== to
    // see if they point to the same object
    CPPUNIT_ASSERT( h1.getHash() == h2.getHash() );

    h2 = hashObserver(h1);
    CPPUNIT_ASSERT( h1.getHash() == h2.getHash() );
    h2 = hashMutator(h1);
    CPPUNIT_ASSERT( !(h1.getHash() == h2.getHash()) );

}

// ** REF **
pVar changeRef(pVar r) {

    CPPUNIT_ASSERT( r.isBoxed() );
    CPPUNIT_ASSERT( r.getPtr()->getRefCount() == 3 ); // (r, newr) from test_pVarRef, plus our r

    pVarP r2(r.getPtr());
    CPPUNIT_ASSERT( r2->getRefCount() == 4 ); // (r, newr) from test_pVarRef, plus our r and r2
    *r2 = 10;

    return pVar(r2);
    
}

void pvarTestCase::test_pVarRef() {

    pVarP ptr = new pVar(5);
    pVar r(ptr);

    // basic
    CPPUNIT_ASSERT( r.isBoxed() );
    CPPUNIT_ASSERT( r.getPtr()->isInt() );
    CPPUNIT_ASSERT( r.getPtr()->getInt() == 5 );


    // ref counting
    CPPUNIT_ASSERT( ptr->getRefCount() == 2 ); // ptr and r
    ptr.reset();
    CPPUNIT_ASSERT( r.getPtr()->getRefCount() == 1 ); // r

    pVar newr(r);
    CPPUNIT_ASSERT( newr.getPtr()->getRefCount() == 2 ); // r, newr

    // php reference flag
    CPPUNIT_ASSERT( r.isRef() == false );
    r.makeRef();
    CPPUNIT_ASSERT( r.isRef() == true );
    r.unmakeRef();
    CPPUNIT_ASSERT( r.isRef() == false );

    // ref flag only valid on boxed pvars
    pVar x;
    x.makeRef();
    CPPUNIT_ASSERT( x.isRef() == false );

    pVar r2 = changeRef(r);
    CPPUNIT_ASSERT( r.isBoxed() );
    CPPUNIT_ASSERT( r.getPtr()->getRefCount() == 3 ); // r, r2, newr
    CPPUNIT_ASSERT( r.getPtr()->isInt() );
    CPPUNIT_ASSERT( r.getPtr()->getInt() == 10 );
    CPPUNIT_ASSERT( r.isBoxed() );
    CPPUNIT_ASSERT( r.getPtr() == r2.getPtr() );

    
}

// ** VISITOR **
class tvisitor : public boost::static_visitor<void>
{
public:

    void operator()(const pTriState &v) const {
        /* ? */
    }

    void operator()(const pInt &v) const {
        CPPUNIT_ASSERT( v == 1 );
    }

    void operator()(const pFloat &v) const {
        CPPUNIT_ASSERT( v == 2.0 );
    }

    void operator()(const pBString &v) const {
        CPPUNIT_ASSERT( v == "test" );
    }

    void operator()(const pUStringP &v) const {
        CPPUNIT_ASSERT( *v == "utest" );
    }

    void operator()(const pHashP &v) const {
        CPPUNIT_ASSERT( v->getSize() == 1 );
    }

    void operator()(const pObjectP &v) const {
        /* ? */
    }

    void operator()(const pResourceP &v) const {
        /* ? */
    }

    void operator()(const pVarP &v) const {
        CPPUNIT_ASSERT( (*v).isInt() );
    }
};

void pvarTestCase::test_visitor() {
    pVar p(pNull);
    p.applyVisitor<tvisitor>();
    p = 1;
    p.applyVisitor<tvisitor>();
    p = 2.0;
    p.applyVisitor<tvisitor>();
    p = "test";
    p.applyVisitor<tvisitor>();
    p = pUStringP(new pUString("utest"));
    p.applyVisitor<tvisitor>();
    p.newEmptyHash();
    p.getHash()->insert("foo", 1);
    p.applyVisitor<tvisitor>();
    // object
    // resource
    p = pVarP(new pVar(1));
    p.applyVisitor<tvisitor>();
}
