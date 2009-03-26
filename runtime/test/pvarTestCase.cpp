
/*

driver for testing pVars

*/

#include <cppunit/config/SourcePrefix.h>
#include <iostream>
#include <unicode/ustream.h>

#include "rphp/runtime/pRuntime.h"
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

// ** UNICODE STRING **
void pvarTestCase::test_pUString() {

    const pVar v(pUStringP("12345"));

    CPPUNIT_ASSERT( v.getUString()->length() == 5 );
    CPPUNIT_ASSERT( *v.getUString() == UnicodeString("12345") );

    // test COW. there are two levels here: one in CowPtr, one in UnicodeString
    // but UnicodeString has it's own small buffer on the stack and only does
    // COW for larger buffers (> 7?), so we do that here:
    pUStringP base("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam auctor condimentum est. Nunc id leo. Donec vel tortor. Suspendisse odio");
    pUStringP copy(base);
    const pUStringP constCopy(base);
    CPPUNIT_ASSERT( base == constCopy );
    CPPUNIT_ASSERT( base == copy );
    CPPUNIT_ASSERT( base.use_count() == 3 );
    CPPUNIT_ASSERT( copy.use_count() == 3 );
    CPPUNIT_ASSERT( constCopy.use_count() == 3 );
    
    // calling a const method will not detach
    CPPUNIT_ASSERT( constCopy->length() > 5 );
    
    CPPUNIT_ASSERT( base == constCopy );
    CPPUNIT_ASSERT( base == copy );
    CPPUNIT_ASSERT( base.use_count() == 3 );
    CPPUNIT_ASSERT( copy.use_count() == 3 );
    CPPUNIT_ASSERT( constCopy.use_count() == 3 );
    
    // but since copy is non const, dereferencing it will detach
    CPPUNIT_ASSERT( copy->length() > 5 );
    
    CPPUNIT_ASSERT( !(base == copy) );
    CPPUNIT_ASSERT( base == constCopy );
    CPPUNIT_ASSERT( base.use_count() == 2 );
    CPPUNIT_ASSERT( constCopy.use_count() == 2 );
    CPPUNIT_ASSERT( copy.use_count() == 1 );
    
    // so, they now point to different UnicodeString objects on the heap
    // however, these objects should share a pointer to the actual string
    // (this is UnicodeString's COW mechanism)
    CPPUNIT_ASSERT( base->getBuffer() == copy->getBuffer() );
    
    // until we change one of them...
    copy = "copy on write complete";
    CPPUNIT_ASSERT( base->getBuffer() != copy->getBuffer() );
    
    /*
    
    ARCHIVE: old pUString tests
    
    pUString u = v.getUString();
    pVar x(u); // x should have a shared ptr to v's buffer now, until write
    CPPUNIT_ASSERT( x.getUString().getBuffer() == v.getUString().getBuffer() );
    pUString& z = x.getUString(); // should be the same
    // now we change z via a UnicodeString. it should detach and not change v/x
    UnicodeString ustr(z.checkoutICUString());
    CPPUNIT_ASSERT( x.getUString().getBuffer() != v.getUString().getBuffer() );
    CPPUNIT_ASSERT( ustr.getBuffer() == z.getBuffer() );
    ustr = "abcde";
    ustr.toUpper();
    z.checkinICUString(ustr);
    // ustr shouldn't be used anymore
    CPPUNIT_ASSERT( v.getUString().readonlyICUString() == UnicodeString("12345") );
    CPPUNIT_ASSERT( z.readonlyICUString() == UnicodeString("ABCDE") );

    // expand
    UnicodeString ustr2(z.checkoutICUString());
    ustr2 = "foobarbaz";
    z.checkinICUString(ustr2);
    CPPUNIT_ASSERT( z.readonlyICUString() == UnicodeString("foobarbaz") );

    // contract
    UnicodeString ustr3(z.checkoutICUString());
    ustr3 = "1";
    z.checkinICUString(ustr3);
    CPPUNIT_ASSERT( z.readonlyICUString() == UnicodeString("1") );

    // equality
    pUString lhs("foo123");
    pUString rhs("foo123");
    CPPUNIT_ASSERT( lhs == rhs );
    rhs = pUString("foo1234");
    CPPUNIT_ASSERT( lhs != rhs );
    */
}

// ** HASH **
void pvarTestCase::test_pHash() {

    // NOTE: this mostly tests pHash functionality as it relates to pVar
    // see phashTestCase for the full pHash testsuite
    pVar h1;
    h1.newEmptyHash();
    CPPUNIT_ASSERT( h1.isHash() );

    h1.getHash()->insert("foo", 5);
    CPPUNIT_ASSERT( h1.getHash()->size() == 1 );

    // test copy on write
    pHashP base(new pHash());
    base->insert("foo", 5);
    pHashP copy(base);
    const pHashP constCopy(base);
    
    CPPUNIT_ASSERT( base == constCopy );
    CPPUNIT_ASSERT( base == copy );
    CPPUNIT_ASSERT( base.use_count() == 3 );
    CPPUNIT_ASSERT( copy.use_count() == 3 );
    CPPUNIT_ASSERT( constCopy.use_count() == 3 );
    
    // calling a const method will not detach
    CPPUNIT_ASSERT( constCopy->size() == 1 );
    
    CPPUNIT_ASSERT( base == constCopy );
    CPPUNIT_ASSERT( base == copy );
    CPPUNIT_ASSERT( base.use_count() == 3 );
    CPPUNIT_ASSERT( copy.use_count() == 3 );
    CPPUNIT_ASSERT( constCopy.use_count() == 3 );
    
    // but since copy is non const, dereferencing it will detach
    CPPUNIT_ASSERT( copy->size() == 1 );
    
    CPPUNIT_ASSERT( !(base == copy) );
    CPPUNIT_ASSERT( base == constCopy );
    CPPUNIT_ASSERT( base.use_count() == 2 );
    CPPUNIT_ASSERT( constCopy.use_count() == 2 );
    CPPUNIT_ASSERT( copy.use_count() == 1 );
    

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

void pvarTestCase::test_pVarBoxed() {

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

    // php reference flag (aliases)
    CPPUNIT_ASSERT( r.isAlias() == false );
    r.makeAlias();
    CPPUNIT_ASSERT( r.isAlias() == true );
    r.unmakeAlias();
    CPPUNIT_ASSERT( r.isAlias() == false );

    // if it's not already boxed, makeAlias will move it to heap
    pVar x(5);
    CPPUNIT_ASSERT( x.isAlias() == false );
    CPPUNIT_ASSERT( x.isBoxed() == false );
    x.makeAlias();
    CPPUNIT_ASSERT( x.isAlias() == true );
    CPPUNIT_ASSERT( x.isBoxed() == true );

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
    const pVar& p_;
public:

    tvisitor(const pVar& p):p_(p) { }

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
        CPPUNIT_ASSERT( *v/*.readonlyICUString()*/ == UnicodeString("utest") );
    }

    void operator()(const pHashP &v) const {
        CPPUNIT_ASSERT( v->size() == 1 );
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
    p = pUStringP("utest");
    p.applyVisitor<tvisitor>();
    p.newEmptyHash();
    p.getHash()->insert("foo", 1);
    p.applyVisitor<tvisitor>();
    // object
    // resource
    p = pVarP(new pVar(1));
    p.applyVisitor<tvisitor>();
}

void pvarTestCase::test_conversion() {

    pVar ustr(pUStringP("foo"));
    pBString bstr(ustr.copyAsBString());
    CPPUNIT_ASSERT( bstr == "foo" );
    
}
