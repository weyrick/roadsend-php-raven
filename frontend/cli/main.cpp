
#include <iostream>
#include "pDriver.h"

int main( int argc, char* argv[] )
{

    rphp::pDriver driver;
    
    std::cout << "Roadsend PHP" << std::endl;
    driver.compile("fiddle.php");

}
