
/*

driver for testing pVars

*/

#include <cppunit/config/SourcePrefix.h>

#include "pvarTestCase.h"
#include "rphp/runtime/pHash.h"
#include "rphp/runtime/pObject.h"
#include "rphp/runtime/pResource.h"
#include "rphp/runtime/pVar.h"

#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( pvarTestCase );

using namespace rphp;

// ** NULL **
void pvarTestCase::test_pNull()
{

    // default null
    pVar a;
    CPPUNIT_ASSERT( a.isNull() );

    CPPUNIT_ASSERT( a.getType() == pVarNullType );

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

    CPPUNIT_ASSERT( b1.isBool() );
    CPPUNIT_ASSERT( b1.getType() == pVarBoolType );

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
    CPPUNIT_ASSERT( i.getType() == pVarIntType );
    CPPUNIT_ASSERT( i.getInt() == 123456 );

    i = -987654;
    CPPUNIT_ASSERT( i.getInt() == -987654 );

}

// ** BIG INT **
void pvarTestCase::test_pBigInt() {

    pVar i(pBigIntP(new pBigInt(123456)));
    CPPUNIT_ASSERT( i.isBigInt() );
    CPPUNIT_ASSERT( i.getType() == pVarBigIntType );
    CPPUNIT_ASSERT( *i.getBigIntP() == pBigInt(123456) );

    i = pBigIntP(new pBigInt(-987654));
    CPPUNIT_ASSERT( *i.getBigIntP() == pBigInt(-987654) );

}

// ** FLOAT **
void pvarTestCase::test_pFloat() {

    pVar i(123.1234);
    CPPUNIT_ASSERT( i.isFloat() );
    CPPUNIT_ASSERT( i.getType() == pVarFloatType );
    CPPUNIT_ASSERT( *i.getFloatP() == pFloat(123.1234) );

    i = -8.34233;
    CPPUNIT_ASSERT( *i.getFloatP() == pFloat(-8.34233) );

}

// ** BINARY STRING **
void pvarTestCase::test_pBString() {

    pVar s(pBString("foo"));

    CPPUNIT_ASSERT( s.isBString() );
    CPPUNIT_ASSERT( s.getType() == pVarBStringType );

    CPPUNIT_ASSERT( s.getBString() == "foo" );

}

// ** UNICODE STRING **
void pvarTestCase::test_pUString() {

    const pVar v(pUStringP("12345"));

    CPPUNIT_ASSERT( v.isUString() );
    CPPUNIT_ASSERT( v.getType() == pVarUStringType );

    CPPUNIT_ASSERT( v.getUStringP()->length() == 5 );
    CPPUNIT_ASSERT( *v.getUStringP() == UnicodeString("12345") );

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

    h1.getHashP()->insert("foo", 5);
    const pVar& ch1(h1);
    CPPUNIT_ASSERT( ch1.getHashP()->size() == 1 );

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

void pvarTestCase::test_pObject() {

    pVar o(pObjectP(new pObject("testClass")));

    CPPUNIT_ASSERT( o.isObject() );
    CPPUNIT_ASSERT( o.getType() == pVarObjectType );

}

void pvarTestCase::test_pResource() {

    pVar o(pResourceP(new pResource()));

    CPPUNIT_ASSERT( o.isResource() );
    CPPUNIT_ASSERT( o.getType() == pVarResourceType );

}


// ** HEAP **
pVar changeBox(pVar r) {

    CPPUNIT_ASSERT( r.isBoxed() );
    CPPUNIT_ASSERT( r.getPtr().use_count()== 4 ); // (r, newr,ptr) from test_pVarRef, plus our r

    pVarP r2(r.getPtr());
    CPPUNIT_ASSERT( r2.use_count() == 5 ); // (r, newr,ptr) from test_pVarRef, plus our r and r2
    *r2 = 10;

    return pVar(r2);

}

void pvarTestCase::test_pVarBoxed() {

    // new pVar on the heap, which will be "boxed"
    pVarP ptr(new pVar(5));
    // new pVar on the stack: becomes the "box" itself
    pVar r(ptr);

    // basic
    CPPUNIT_ASSERT( r.isBoxed() == true );
    CPPUNIT_ASSERT( r.isInt() );
    CPPUNIT_ASSERT( r.getPtr()->isInt() );
    CPPUNIT_ASSERT( r.getInt() == 5 );
    CPPUNIT_ASSERT( r.getPtr()->getInt() == 5 );

    // ref counting
    CPPUNIT_ASSERT( ptr.use_count() == 2 ); // ptr and r

    pVar newr(r);
    CPPUNIT_ASSERT( newr.getPtr().use_count() == 3 ); // ptr, r, newr

    // make sure they point to the same thing
    CPPUNIT_ASSERT( newr.isInt() );
    CPPUNIT_ASSERT( r.isInt() );
    CPPUNIT_ASSERT( newr.getInt() == r.getInt() );
    CPPUNIT_ASSERT( r.getPtr() == newr.getPtr() );

    // if it's not already boxed, makeAlias will move it to heap
    pVar x(5);
    CPPUNIT_ASSERT( x.isBoxed() == false );
    x.boxData();
    CPPUNIT_ASSERT( x.isBoxed() == true );

    pVar r2 = changeBox(r);
    CPPUNIT_ASSERT( r.isBoxed() );
    CPPUNIT_ASSERT( r.getPtr().use_count() == 4 ); // r, r2, newr, ptr
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

    void operator()(const pBigIntP &v) const {
        CPPUNIT_ASSERT( *v == 2 );
    }

    void operator()(const pFloatP &v) const {
        CPPUNIT_ASSERT( *v == 2.0 );
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
    p.getHashP()->insert("foo", 1);
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

void pvarTestCase::test_math_operators() {

    pVar lhs(5);
    pVar rhs(6);
    CPPUNIT_ASSERT( lhs.isInt() );
    CPPUNIT_ASSERT( rhs.isInt() );

    // += on basic pInt
    lhs += 5;
    CPPUNIT_ASSERT( lhs.isInt() );
    CPPUNIT_ASSERT( lhs.getInt() == 10 );

    // + on basic pInt
    pVar sum(lhs+rhs);
    CPPUNIT_ASSERT( sum.isInt() );
    CPPUNIT_ASSERT( sum.getInt() == 16 );

    // + overflow to pBigInt
    lhs = RPHP_INT_MAX;
    lhs += RPHP_INT_MAX;
    CPPUNIT_ASSERT( lhs.isBigInt() );
    CPPUNIT_ASSERT( *lhs.getBigIntP() == (pBigInt(RPHP_INT_MAX)+pBigInt(RPHP_INT_MAX)) );

    // -= on basic pInt
    // - on basic pInt
    // - underflow to pBigInt

    // *= on basic pInt
    // * on basic pInt
    // * overflow to pBigInt

    // /= on basic pInt
    // / on basic pInt

    // += on basic pBigInt
    lhs = pBigIntP(new pBigInt(RPHP_INT_MAX));
    rhs = pBigIntP(new pBigInt(RPHP_INT_MAX));
    CPPUNIT_ASSERT( lhs.isBigInt() );
    lhs += rhs;
    CPPUNIT_ASSERT( lhs.isBigInt() );
    CPPUNIT_ASSERT( *rhs.getBigIntP() == (pBigInt(RPHP_INT_MAX)) );
    CPPUNIT_ASSERT( *lhs.getBigIntP() == (pBigInt(RPHP_INT_MAX)+pBigInt(RPHP_INT_MAX)) );

    // + on basic pBigInt
    lhs = pBigIntP(new pBigInt(RPHP_INT_MAX));
    rhs = pBigIntP(new pBigInt(RPHP_INT_MAX));
    sum = lhs + rhs;
    CPPUNIT_ASSERT( sum.isBigInt() );
    CPPUNIT_ASSERT( *sum.getBigIntP() == (pBigInt(RPHP_INT_MAX)+pBigInt(RPHP_INT_MAX)) );

    // -= on basic pBigInt
    // - on basic pBigInt

    // *= on basic pBigInt
    // * on basic pBigInt

    // /= on basic pBigInt
    // / on basic pBigInt

    // += on basic pFloat
    // + on basic pFloat

    // -= on basic pFloat
    // - on basic pFloat

    // *= on basic pFloat
    // * on basic pFloat

    // /= on basic pFloat
    // / on basic pFloat

    // += on pInt/pFloat
    // += on pInt/pBigInt
    // += on pFloat/pBigInt
    // -= on pInt/pFloat
    // -= on pInt/pBigInt
    // -= on pFloat/pBigInt
    // *= on pInt/pFloat
    // *= on pInt/pBigInt
    // *= on pFloat/pBigInt
    // /= on pInt/pFloat
    // /= on pInt/pBigInt
    // /= on pFloat/pBigInt



}
