
/*

playing around with boost::variant, to see if it's suitable to store php vals

*/

#include "boost/variant.hpp"
#include "boost/lexical_cast.hpp"
#include <iostream>
#include <typeinfo>

// just a demo class, we'd also have php_object
class php_hash {
    private:
        int size;
    public:
        php_hash(int sizevar) : size(sizevar) { }

        php_hash(const php_hash& h) {
            size = h.size;
        }
        int getSize() const { return size; }

};

std::ostream& operator << (std::ostream& os, const php_hash& h)
{
    return os << "php_hash" << std::endl;
}

// using the c++ bool as a type seems to cause problems, so we implement php bool
// as an enum. note that php integers are type long
// we also use in to decide if this value is NULL. unfortunately a null value would evaluate
// to true (in c++, since it's non 0). can we do this better?

namespace RPHP {
    enum p3state
    {
	False,
	True,
	Null
    };
}


// this would be the typedef for a generic php variable in our runtime
typedef boost::variant< RPHP::p3state, long, double, std::string, php_hash> pvar;

// used for determining type at runtime
typedef enum { PVAR_NULL, PVAR_BOOL, PVAR_LONG, PVAR_DOUBLE, PVAR_STRING, PVAR_HASH } pvartype;

// generic visitor which can be applied to any pvar
// correct method runs based on the appropriate type
class my_visitor : public boost::static_visitor<int>
{
public:
    int operator()(const RPHP::p3state &i) const
    {
        if (i == RPHP::Null) {
            std::cout << "i see a null" << std::endl;
        }
        else {
            std::cout << "i see a bool" << std::endl;
        }
        return i;
    }

    int operator()(const long &i) const
    {
        std::cout << "i see a long" << std::endl;
        return i;
    }

    int operator()(const double &i) const
    {
        std::cout << "i see a float" << std::endl;
        return 0;
    }

    int operator()(const std::string &str) const
    {
        std::cout << "i see a string" << std::endl;
        return str.length();
    }

    int operator()(const php_hash &h) const
    {
        std::cout << "i see a php_hash" << std::endl;
        return 0;
    }

};

// a visitor for determining type
class type_checker : public boost::static_visitor<int>
{
public:
    pvartype operator()(const long &i) const
    {
        return PVAR_LONG;
    }

    pvartype operator()(const double &i) const
    {
        return PVAR_DOUBLE;
    }

    pvartype operator()(const std::string &str) const
    {
        return PVAR_STRING;
    }

    pvartype operator()(const php_hash &h) const
    {
        return PVAR_HASH;
    }

    pvartype operator()(const RPHP::p3state &h) const
    {
        (h == RPHP::Null) ? PVAR_NULL : PVAR_BOOL;
    }

};

// a visitor for converting to a num
class convert_to_num_visitor : public boost::static_visitor<>
{
protected:
  pvar &var;
public:
  convert_to_num_visitor(pvar &v) : var(v) {}

  void operator()(const std::string &a) const {
    try {
      var = boost::lexical_cast<long>(a);
    } catch(boost::bad_lexical_cast &) {
      var = 0l;
    }
  }

  void operator()(const double &i) const
  {
        // nothing, already numeric
  }

  void operator()(const php_hash &h) const
  {
        var = (long)h.getSize();
  }

    void operator()(const RPHP::p3state &h) const
  {
        (h == RPHP::True) ? var = 1l : var = 0l;
  }

  void operator()(const long &a) const {
        // Do nothing - already correct type
  }
};


// non destructive cast (makes a copy)
pvar pvar_cast_to_number(const pvar p) {

    pvar r = p;
    boost::apply_visitor( convert_to_num_visitor(r), r );
    return r;
    
}


// define operators for working with pvars
// php hashes would concat, otherwise convert to number
pvar pvar_add (const pvar lhs, const pvar rhs)
{
    pvar l,r,result;

    int lhs_type = boost::apply_visitor( type_checker(), lhs );
    int rhs_type = boost::apply_visitor( type_checker(), rhs );
    if ( (lhs_type == PVAR_HASH) && (rhs_type == PVAR_HASH) ) {
        std::cout << "fixme: concat hashes" << std::endl;
        result = 0l;
    }
    else {
        // convert to number, then add
        l = pvar_cast_to_number(lhs);
        std::cout << "pvar_add: l is " << l << std::endl;
        r = pvar_cast_to_number(rhs);
        std::cout << "pvar_add: r is " << r << std::endl;
        result = boost::get<long>(l) + boost::get<long>(r);
        std::cout << "pvar_add: result is " << result << std::endl;
    }

    return result;
}

// driver
int main()
{
    pvar u,t,r;

    // string
    u = "hello world there";

    std::cout << u << std::endl;
    int result = boost::apply_visitor( my_visitor(), u );

    // long
    u = 15l;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // float
    u = 2.3123;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // bool
    u = RPHP::True;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    if (boost::get<RPHP::p3state>(u) == RPHP::True) {
    	std::cout << "the bool was true" << std::endl;
    }

    // null
    u = RPHP::Null;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // php hash
    php_hash h(5);
    std::cout << h;
    u = h;
    std::cout << u << std::endl;

    result = boost::apply_visitor( my_visitor(), u );

    ////

    // type checking?
    int pt = boost::apply_visitor( type_checker(), u );
    switch (pt) {
    case PVAR_HASH:
    	std::cout << "found a hash" << std::endl;
    	break;
    case PVAR_DOUBLE:
    	std::cout << "found a float" << std::endl;
    	break;
    default:
    	std::cout << "woops, what type was it?" << std::endl;
    }
    // RTTI: output is ugly, it also might not be available everywhere
    std::cout << typeid( u ).name() << std::endl;

    ////

    // type conversion
    std::cout << "type conversion:" << std::endl;
    u = "55";
    boost::apply_visitor( my_visitor(), u );
    boost::apply_visitor( convert_to_num_visitor(u), u );
    boost::apply_visitor( my_visitor(), u );
    std::cout << "final: " << u << std::endl;

    // operators
    
    // try adding a long and a numeric string
    u = 10l;
    t = "20";
    r = pvar_add(u, t);
    std::cout << "number add: " << r << std::endl;
    std::cout << "u is: " << u << std::endl;
    std::cout << "t is: " << t << std::endl;

}
