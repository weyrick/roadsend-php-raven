
/*

driver for testing pvars

*/

#include "rphp_runtime.h"
#include "unicode/ustream.h" // ostream API for UnicodeString
#include "boost/variant.hpp"
#include <iostream>

// generic visitor which can be applied to any pvar
// correct method runs based on the appropriate type
class my_visitor : public boost::static_visitor<int>
{
public:

    int operator()(const rphp::p3state &i) const {
        if (i == rphp::Null) {
            std::cout << "i see a null" << std::endl;
        }
        else {
            std::cout << "i see a bool" << std::endl;
        }
        return i;
    }

    int operator()(const rphp::pint &i) const {
        std::cout << "i see a pint" << std::endl;
        return i;
    }

    int operator()(const rphp::pfloat &i) const {
        std::cout << "i see a float" << std::endl;
        return 0;
    }

    int operator()(const rphp::bstring &str) const {
        std::cout << "i see a binary string" << std::endl;
        return str.length();
    }

    int operator()(const rphp::ustring &str) const {
        std::cout << "i see a unicode string" << std::endl;
        return str.length();
    }

    int operator()(const rphp::phash &h) const {
        std::cout << "i see a phash" << std::endl;
        return 0;
    }

    /*
    int operator()(const rphp::pobject &h) const {
        std::cout << "i see a pobject" << std::endl;
        return 0;
    }
    */

    int operator()(const rphp::pvarRef &h) const {
        std::cout << "i see a php reference" << std::endl;
        return 0;
    }
};

void changeRef(rphp::pvar r) {

	rphp::pvarRef rval;
	if (rval = rphp::pvar_getVal_ref(r)) {
		*rval = rphp::bstring("changed the ref to a string!");
	}
	else {
		std::cout << "not a ref" << std::endl;
	}

}

// driver
int main()
{
    rphp::pvar u,t,r;

    // sizes
    std::cout << "p3state: " << sizeof(rphp::p3state) << std::endl;
    std::cout << "pint: " << sizeof(rphp::pint) << std::endl;
    std::cout << "pfloat: " << sizeof(rphp::pfloat) << std::endl;
    std::cout << "bstring: " << sizeof(rphp::bstring) << std::endl;
    std::cout << "ustring: " << sizeof(rphp::ustring) << std::endl;
    std::cout << "phash: " << sizeof(rphp::phash) << std::endl;
    //std::cout << "pobj: " << sizeof(rphp::pobject) << std::endl;
    std::cout << "pvarBase: " << sizeof(rphp::pvarBase) << std::endl;
    std::cout << "pvarRef: " << sizeof(rphp::pvarRef) << std::endl;
    std::cout << "pvar: " << sizeof(rphp::pvar) << std::endl;

    // binary string
    u = rphp::bstring("hello world there");

    std::cout << u << std::endl;
    int result = boost::apply_visitor( my_visitor(), u );

    // unicode string
    u = rphp::ustring("hello world there -- unicode style");

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // long
    u = rphp::pint(15);

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // float
    u = rphp::pfloat(2.3123);

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // bool
    u = rphp::True;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    if (rphp::pvar_getVal_bool(u) == rphp::True) {
    	std::cout << "the bool was true" << std::endl;
    }

    // null
    u = rphp::Null;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // php hash
    rphp::phash h;
    h.insert("var-test", rphp::pint(971));
    rphp::pvar hole = rphp::pfloat(1.234);
    h.insert("var-test2", hole);
    std::cout << h;
    h.varDump();
    u = h;
    std::cout << u << std::endl;

    result = boost::apply_visitor( my_visitor(), u );

    ////

    // type checking?
    int pt = rphp::pvar_getType(u);
    switch (pt) {
    case rphp::PVAR_HASH:
    	std::cout << "found a hash" << std::endl;
    	break;
    case rphp::PVAR_FLOAT:
    	std::cout << "found a float" << std::endl;
    	break;
    default:
    	std::cout << "woops, what type was it?" << std::endl;
    }

    ////

    // type conversion
    std::cout << "type conversion:" << std::endl;
    u = std::string("55");
    boost::apply_visitor( my_visitor(), u );
    rphp::pvar_convertToNumber(u);
    boost::apply_visitor( my_visitor(), u );
    std::cout << "final: " << u << std::endl;

    // operators

    // try adding a long and a numeric string
    u = rphp::pint(10); // NOTE: this is a long. int's turn into p3state (bool/null)
    t = std::string("20");
    r = pvar_add(u, t);
    std::cout << "number add: " << r << std::endl;
    std::cout << "u is: " << u << std::endl;
    std::cout << "t is: " << t << std::endl;

    // references

    // create a new reference. can only be comprised of pvarBase items (i.e., can't be a ref to a ref)
    std::cout << "references----" << std::endl;
    rphp::pvarRef r1(new rphp::pvarBase);

    // assign a value to the pvar
    *r1 = rphp::pint(5);
    boost::apply_visitor( my_visitor(), *r1 );

    // call a function which takes a pvar (not strictly a pvarRef)
    changeRef(r1);
    boost::apply_visitor( my_visitor(), *r1 );

    // assign two variables to the same reference
    rphp::pvarRef r2 = r1;
    boost::apply_visitor( my_visitor(), *r1 );
    boost::apply_visitor( my_visitor(), *r2 );

	// change one
    *r2 = rphp::pint(20);
    boost::apply_visitor( my_visitor(), *r1 );
    boost::apply_visitor( my_visitor(), *r2 );

}
