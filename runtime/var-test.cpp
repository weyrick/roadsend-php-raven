
/*

playing around with boost::variant, looking some something suitable to store php vals

*/

#include "boost/variant.hpp"
#include <iostream>

// just a demo class, we'd also have php_object
class php_hash {
    private:
        int x;
    public:
        php_hash(int xvar) : x(xvar) { }
        
        php_hash(const php_hash& h) {
            x = h.x;
        }
        int getx() const { return x; }

};

std::ostream& operator << (std::ostream& os, const php_hash& h)
{
    return os << "php_hash" << std::endl;
}

// this would be the typedef for a generic php variable in our runtime
typedef boost::variant< int, std::string, double, php_hash> pvar;

class my_visitor : public boost::static_visitor<int>
{
public:
    int operator()(int i) const
    {
        std::cout << "i see an int" << std::endl;
        return i;
    }
    
    int operator()(double i) const
    {
        std::cout << "i see a float" << std::endl;
        return 0;
    }
    
    int operator()(const std::string & str) const
    {
        std::cout << "i see a string" << std::endl;
        return str.length();
    }
    
    int operator()(const php_hash & h) const
    {
        std::cout << "i see a php_hash" << std::endl;
        return h.getx();
    }
    
};

// driver
int main()
{
    pvar u;
    u = "hello world there";
    
    std::cout << u << std::endl; 
    int result = boost::apply_visitor( my_visitor(), u );

    u = 15;
    
    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );
    
    u = 2.3123;
    
    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    php_hash h(5);
    std::cout << h;
    u = h;
    std::cout << u << std::endl;
    
    result = boost::apply_visitor( my_visitor(), u );
    
    //std::cout << result << std::endl; // output: 11 (i.e., length of "hello world")
    
}
