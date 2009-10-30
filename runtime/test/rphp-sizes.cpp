
#include "rphp/runtime/pRuntimeTypes.h"
#include "rphp/runtime/pVar.h"
#include "rphp/runtime/pFunction.h"
#include "rphp/runtime/pHash.h"
#include "rphp/runtime/pObject.h"
#include "rphp/runtime/pResource.h"

#include <iostream>

using namespace rphp;

int main(void) {

    std::cout << "pTriState: " << sizeof(pTriState) << std::endl;
    std::cout << "pInt: " << sizeof(pInt) << std::endl;
    std::cout << "pBigInt: " << sizeof(pBigInt) << std::endl;
    std::cout << "pFloat: " << sizeof(pFloat)<< std::endl;
    std::cout << "pBString: " << sizeof(pBString) << std::endl;
    std::cout << "pUString: " << sizeof(pUString) << std::endl;
    std::cout << "pUStringP: " << sizeof(pUStringP) << std::endl;
    std::cout << "UnicodeString: " << sizeof(UnicodeString) << std::endl;
    std::cout << "pHash: " << sizeof(pHash) << std::endl;
    std::cout << "pHashP: " << sizeof(pHashP) << std::endl;
    std::cout << "pObject: " << sizeof(pObject) << std::endl;
    std::cout << "pObjectP: " << sizeof(pObjectP) << std::endl;
    std::cout << "pResource: " << sizeof(pResource) << std::endl;
    std::cout << "pResourceP: " << sizeof(pResourceP) << std::endl;
    std::cout << "pFunction: " << sizeof(pFunction) << std::endl;
    std::cout << "pVarP: " << sizeof(pVarP) << std::endl;
    std::cout << "pVar: " << sizeof(pVar) << std::endl;

}
 
