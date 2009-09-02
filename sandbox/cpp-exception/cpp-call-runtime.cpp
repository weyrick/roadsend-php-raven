#include "cpp-lib.h"


#include <iostream>
#include <stdexcept>
int main()
{
	//mockError();
	//mockRuntimeLib l;
	try {
	  //l.mockError();
	  mockError();
	}
	catch (std::runtime_error& e) {
	    std::cout << "caught: " << e.what() << std::endl;
	}
}
