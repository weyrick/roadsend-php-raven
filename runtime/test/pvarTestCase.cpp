
/*

driver for testing pVars

*/

#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "pRuntime.h"
#include "pvarTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( pvarTestCase );

using namespace rphp;

// generic visitor which can be applied to any pvar
// correct method runs based on the appropriate type
/*
class my_visitor : public boost::static_visitor<int>
{
public:

    int operator()(const pTriState &i) const {
        if (pNull(i)) {
            std::cout << "i see a null" << std::endl;
        }
        else {
            std::cout << "i see a bool" << std::endl;
        }
        return (i) ? 1 : 0;
    }

    int operator()(const pInt &i) const {
        std::cout << "i see a pInt" << std::endl;
        return i;
    }

    int operator()(const pFloat &i) const {
        std::cout << "i see a float" << std::endl;
        return 0;
    }

    int operator()(const pBString &str) const {
        std::cout << "i see a binary string" << std::endl;
        return str.length();
    }

    int operator()(const pUStringP &str) const {
        std::cout << "i see a unicode string" << std::endl;
        return str->length();
    }

    int operator()(const pHashP &h) const {
        std::cout << "i see a pHash" << std::endl;
        return 0;
    }

    int operator()(const pObjectP &h) const {
        std::cout << "i see a pObject" << std::endl;
        return 0;
    }

    int operator()(const pResourceP &h) const {
        std::cout << "i see a resource" << std::endl;
        return 0;
    }

    int operator()(const pVarP &h) const {
        std::cout << "i see a php reference" << std::endl;
        return 0;
    }
};
*/

void changeRef(pVar r) {

    pVarP rval;

    CPPUNIT_ASSERT( r.isRef() );
    
    if (rval = r.getRef()) {
        *rval = pBString("changed the ref to a string!");
    }
    else {
        std::cout << "not a ref" << std::endl;
    }

}

// driver
void pvarTestCase::test_pNull()
{
    // ** NULL **
    
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

void pvarTestCase::test_pBool() {

    // ** BOOL **

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

    /*
    // sizes
    std::cout << std::endl;
    std::cout << "pTriState: " << sizeof(pTriState) << std::endl;
    std::cout << "pInt: " << sizeof(pInt) << std::endl;
    std::cout << "pFloat: " << sizeof(pFloat) << std::endl;
    std::cout << "pBString: " << sizeof(pBString) << std::endl;
    std::cout << "pUString: " << sizeof(pUString) << std::endl;
    std::cout << "pUStringP: " << sizeof(pUStringP) << std::endl;
    std::cout << "pHash: " << sizeof(pHash) << std::endl;
    std::cout << "pHashP: " << sizeof(pHashP) << std::endl;
    std::cout << "pObject: " << sizeof(pObject) << std::endl;
    std::cout << "pObjectP: " << sizeof(pObjectP) << std::endl;
    std::cout << "pResource: " << sizeof(pResource) << std::endl;
    std::cout << "pResourceP: " << sizeof(pResourceP) << std::endl;
    std::cout << "pVarRef: " << sizeof(pVarP) << std::endl;
    std::cout << "pVar: " << sizeof(pVar) << std::endl;

    // binary string
    u = pBString("hello world there");

    std::cout << "bstring: " << u << std::endl;
    //int result = u.applyVisitor<my_visitor>();

    // unicode string
    u = new pUString("hello world there -- unicode style");

    std::cout << "ustring: " << u << std::endl;
    //result = u.applyVisitor<my_visitor>();

    // long
    u = pInt(15);

    std::cout << "pint: " << u << std::endl;
    //result = u.applyVisitor<my_visitor>();
    
    // float
    u = pFloat(2.3123);

    std::cout << u << std::endl;
    //result = u.applyVisitor<my_visitor>();

    // bool
    u = pTrue;

    std::cout << u << std::endl;
    //result = u.applyVisitor<my_visitor>();

    if (u.getBool()) {
    	std::cout << "the bool was true" << std::endl;
    }
    else {
        std::cout << "the bool was false" << std::endl;
    }

    u = pFalse;

    std::cout << u << std::endl;
    //result = u.applyVisitor<my_visitor>();

    if (u.getBool()) {
        std::cout << "the bool was true" << std::endl;
    }
    else {
        std::cout << "the bool was false" << std::endl;
    }

    // null
    u = pNull;

    std::cout << u << std::endl;
    //result = u.applyVisitor<my_visitor>();

    // php hash
    
    pHash h;
    h.insert("var-test", pInt(971));
    pVar hole = pFloat(1.234);
    h.insert("var-test2", hole);
    std::cout << h;
    h.varDump();
    u = h;
    std::cout << u << std::endl;

    result = boost::apply_visitor( my_visitor(), u );
    
    ////

    // type checking?
    pVarType pt = u.getType();
    switch (pt) {
    case pVarHashType:
    	std::cout << "found a hash" << std::endl;
    	break;
    case pVarFloatType:
    	std::cout << "found a float" << std::endl;
    	break;
    case pVarNullType:
        std::cout << "found a null" << std::endl;
        break;
    default:
    	std::cout << "woops, what type was it?" << std::endl;
    }

    ////

    // type conversion
    std::cout << "type conversion:" << std::endl;
    u = std::string("55");
    //u.applyVisitor<my_visitor>();
    u.convertToInt();
    //u.applyVisitor<my_visitor>();
    std::cout << "final: " << u << std::endl;

    // operators

    // try adding a long and a numeric string
    u = pInt(10); // NOTE: this is a long. int's turn into p3state (bool/null)
    t = std::string("20");
    r = pVar_add(u, t);
    std::cout << "number add: " << r << std::endl;
    std::cout << "u is: " << u << std::endl;
    std::cout << "t is: " << t << std::endl;

    // references

    // create a new reference. can only be comprised of pVarBase items (i.e., can't be a ref to a ref)
    std::cout << "references----" << std::endl;
    pVarP r1(new pVar());

    // assign a value to the pVar
    *r1 = pInt(5);
    //(*r1).applyVisitor<my_visitor>();

    // call a function which takes a pVar (not strictly a pVarRef)
    changeRef(r1);
    //(*r1).applyVisitor<my_visitor>();

    // assign two variables to the same reference
    pVarP r2 = r1;
    //(*r1).applyVisitor<my_visitor>();
    //(*r2).applyVisitor<my_visitor>();

	// change one
    *r2 = pInt(20);
    //(*r1).applyVisitor<my_visitor>();
    //(*r2).applyVisitor<my_visitor>();
    */
    
