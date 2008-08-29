
/*

driver for testing pVars

*/

#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "pRuntime.h"
#include "pvarTestCase.h"

CPPUNIT_TEST_SUITE_REGISTRATION( pvarTestCase );

// generic visitor which can be applied to any pvar
// correct method runs based on the appropriate type
class my_visitor : public boost::static_visitor<int>
{
public:

    int operator()(const rphp::pTriState &i) const {
        if (rphp::pNull(i)) {
            std::cout << "i see a null" << std::endl;
        }
        else {
            std::cout << "i see a bool" << std::endl;
        }
        return (i) ? 1 : 0;
    }

    int operator()(const rphp::pInt &i) const {
        std::cout << "i see a pInt" << std::endl;
        return i;
    }

    int operator()(const rphp::pFloat &i) const {
        std::cout << "i see a float" << std::endl;
        return 0;
    }

    int operator()(const rphp::pBString &str) const {
        std::cout << "i see a binary string" << std::endl;
        return str.length();
    }

    int operator()(const rphp::pUStringP &str) const {
        std::cout << "i see a unicode string" << std::endl;
        return str->length();
    }

    int operator()(const rphp::pHashP &h) const {
        std::cout << "i see a pHash" << std::endl;
        return 0;
    }

    int operator()(const rphp::pObjectP &h) const {
        std::cout << "i see a pObject" << std::endl;
        return 0;
    }

    int operator()(const rphp::pResourceP &h) const {
        std::cout << "i see a resource" << std::endl;
        return 0;
    }

    int operator()(const rphp::pVarP &h) const {
        std::cout << "i see a php reference" << std::endl;
        return 0;
    }
};

void changeRef(rphp::pVar r) {

	rphp::pVarP rval;
	if (rval = r.getRef()) {
		*rval = rphp::pBString("changed the ref to a string!");
	}
	else {
		std::cout << "not a ref" << std::endl;
	}

}

// driver
void pvarTestCase::basic()
{
    rphp::pVar u,t,r;

    // sizes
    std::cout << std::endl;
    std::cout << "pTriState: " << sizeof(rphp::pTriState) << std::endl;
    std::cout << "pInt: " << sizeof(rphp::pInt) << std::endl;
    std::cout << "pFloat: " << sizeof(rphp::pFloat) << std::endl;
    std::cout << "pBString: " << sizeof(rphp::pBString) << std::endl;
    std::cout << "pUString: " << sizeof(rphp::pUString) << std::endl;
    std::cout << "pUStringP: " << sizeof(rphp::pUStringP) << std::endl;
    std::cout << "pHash: " << sizeof(rphp::pHash) << std::endl;
    std::cout << "pHashP: " << sizeof(rphp::pHashP) << std::endl;
    std::cout << "pObject: " << sizeof(rphp::pObject) << std::endl;
    std::cout << "pObjectP: " << sizeof(rphp::pObjectP) << std::endl;
    std::cout << "pResource: " << sizeof(rphp::pResource) << std::endl;
    std::cout << "pResourceP: " << sizeof(rphp::pResourceP) << std::endl;
    std::cout << "pVarRef: " << sizeof(rphp::pVarP) << std::endl;
    std::cout << "pVar: " << sizeof(rphp::pVar) << std::endl;

    // binary string
    u = rphp::pBString("hello world there");

    std::cout << "bstring: " << u << std::endl;
    int result = u.applyVisitor<my_visitor>();

    // unicode string
    u = new rphp::pUString("hello world there -- unicode style");

    std::cout << "ustring: " << u << std::endl;
    result = u.applyVisitor<my_visitor>();

    // long
    u = rphp::pInt(15);

    std::cout << "pint: " << u << std::endl;
    result = u.applyVisitor<my_visitor>();
    
    // float
    u = rphp::pFloat(2.3123);

    std::cout << u << std::endl;
    result = u.applyVisitor<my_visitor>();

    // bool
    u = rphp::pTrue;

    std::cout << u << std::endl;
    result = u.applyVisitor<my_visitor>();

    if (u.getBool()) {
    	std::cout << "the bool was true" << std::endl;
    }
    else {
        std::cout << "the bool was false" << std::endl;
    }

    u = rphp::pFalse;

    std::cout << u << std::endl;
    result = u.applyVisitor<my_visitor>();

    if (u.getBool()) {
        std::cout << "the bool was true" << std::endl;
    }
    else {
        std::cout << "the bool was false" << std::endl;
    }

    // null
    u = rphp::pNull;

    std::cout << u << std::endl;
    result = u.applyVisitor<my_visitor>();

    // php hash
    /*
    rphp::pHash h;
    h.insert("var-test", rphp::pInt(971));
    rphp::pVar hole = rphp::pFloat(1.234);
    h.insert("var-test2", hole);
    std::cout << h;
    h.varDump();
    u = h;
    std::cout << u << std::endl;

    result = boost::apply_visitor( my_visitor(), u );
    */
    ////

    // type checking?
    rphp::pVarType pt = u.getType();
    switch (pt) {
    case rphp::pVarHashType:
    	std::cout << "found a hash" << std::endl;
    	break;
    case rphp::pVarFloatType:
    	std::cout << "found a float" << std::endl;
    	break;
    case rphp::pVarNullType:
        std::cout << "found a null" << std::endl;
        break;
    default:
    	std::cout << "woops, what type was it?" << std::endl;
    }

    ////

    // type conversion
    std::cout << "type conversion:" << std::endl;
    u = std::string("55");
    u.applyVisitor<my_visitor>();
    u.convertToInt();
    u.applyVisitor<my_visitor>();
    std::cout << "final: " << u << std::endl;

    // operators

    // try adding a long and a numeric string
    u = rphp::pInt(10); // NOTE: this is a long. int's turn into p3state (bool/null)
    t = std::string("20");
    r = pVar_add(u, t);
    std::cout << "number add: " << r << std::endl;
    std::cout << "u is: " << u << std::endl;
    std::cout << "t is: " << t << std::endl;

    // references

    // create a new reference. can only be comprised of pVarBase items (i.e., can't be a ref to a ref)
    std::cout << "references----" << std::endl;
    rphp::pVarP r1(new rphp::pVar());

    // assign a value to the pVar
    *r1 = rphp::pInt(5);
    (*r1).applyVisitor<my_visitor>();

    // call a function which takes a pVar (not strictly a pVarRef)
    changeRef(r1);
    (*r1).applyVisitor<my_visitor>();

    // assign two variables to the same reference
    rphp::pVarP r2 = r1;
    (*r1).applyVisitor<my_visitor>();
    (*r2).applyVisitor<my_visitor>();

	// change one
    *r2 = rphp::pInt(20);
    (*r1).applyVisitor<my_visitor>();
    (*r2).applyVisitor<my_visitor>();

}
