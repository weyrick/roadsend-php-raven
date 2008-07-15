
/*

driver for testing pVars

*/

#include <cppunit/config/SourcePrefix.h>
#include <iostream>

#include "rphp_runtime.h"
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

    int operator()(const rphp::pVarRef &h) const {
        std::cout << "i see a php reference" << std::endl;
        return 0;
    }
};

void changeRef(rphp::pVar r) {

	rphp::pVarRef rval;
	if (rval = rphp::pVar_getVal_ref(r)) {
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
    std::cout << "pVarBase: " << sizeof(rphp::pVarBase) << std::endl;
    std::cout << "pVarRef: " << sizeof(rphp::pVarRef) << std::endl;
    std::cout << "pVar: " << sizeof(rphp::pVar) << std::endl;

    // binary string
    u = rphp::pBString("hello world there");

    std::cout << u << std::endl;
    int result = boost::apply_visitor( my_visitor(), u );

    // unicode string
    u = new rphp::pUString("hello world there -- unicode style");

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // long
    u = rphp::pInt(15);

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // float
    u = rphp::pFloat(2.3123);

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // bool
    u = rphp::pTrue;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    if (rphp::pVar_getVal_bool(u)) {
    	std::cout << "the bool was true" << std::endl;
    }
    else {
        std::cout << "the bool was false" << std::endl;
    }
    
    u = rphp::pFalse;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    if (rphp::pVar_getVal_bool(u)) {
        std::cout << "the bool was true" << std::endl;
    }
    else {
        std::cout << "the bool was false" << std::endl;
    }

    // null
    u = rphp::pNull;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

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
    int pt = rphp::pVar_getType(u);
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
    boost::apply_visitor( my_visitor(), u );
    rphp::pVar_convertToNumber(u);
    boost::apply_visitor( my_visitor(), u );
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
    rphp::pVarRef r1(new rphp::pVarBase);

    // assign a value to the pVar
    *r1 = rphp::pInt(5);
    boost::apply_visitor( my_visitor(), *r1 );

    // call a function which takes a pVar (not strictly a pVarRef)
    changeRef(r1);
    boost::apply_visitor( my_visitor(), *r1 );

    // assign two variables to the same reference
    rphp::pVarRef r2 = r1;
    boost::apply_visitor( my_visitor(), *r1 );
    boost::apply_visitor( my_visitor(), *r2 );

	// change one
    *r2 = rphp::pInt(20);
    boost::apply_visitor( my_visitor(), *r1 );
    boost::apply_visitor( my_visitor(), *r2 );

}
