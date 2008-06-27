
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

    int operator()(const long &i) const {
        std::cout << "i see a long" << std::endl;
        return i;
    }

    int operator()(const double &i) const {
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

    int operator()(const rphp::php_hash &h) const {
        std::cout << "i see a php_hash" << std::endl;
        return 0;
    }

    int operator()(const rphp::php_object &h) const {
        std::cout << "i see a php_object" << std::endl;
        return 0;
    }

};

// driver
int main()
{
    rphp::pvar u,t,r;

    // binary string
    u = rphp::bstring("hello world there");

    std::cout << u << std::endl;
    int result = boost::apply_visitor( my_visitor(), u );

    // unicode string
    u = rphp::ustring("hello world there -- unicode style");

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );


    // long
    u = 15l;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // float
    u = 2.3123;

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
    rphp::php_hash h(5);
    std::cout << h;
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
    case rphp::PVAR_DOUBLE:
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
    u = 10l; // NOTE: this is a long. int's turn into p3state (bool/null)
    t = std::string("20");
    r = pvar_add(u, t);
    std::cout << "number add: " << r << std::endl;
    std::cout << "u is: " << u << std::endl;
    std::cout << "t is: " << t << std::endl;

}
