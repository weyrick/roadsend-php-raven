/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * ***** END LICENSE BLOCK ***** */

#include <iostream>
#include "pRuntime.h"
#include "pExtManager.h"
#include "pFunctionManager.h"

namespace rphp {

pRuntimeEngine::pRuntimeEngine() : extManager(new pExtManager(this)),
                                   functionManager(new pFunctionManager(this))
{
    std::cout << "runtime is starting" << std::endl;

    // runtime initialization
    extManager->startUp();

}


pRuntimeEngine::~pRuntimeEngine() {

    std::cout << "runtime is shutting down" << std::endl;

    // runtime shutdown
    delete functionManager;
    delete extManager;
}

} /* end namespace rphp */


/* start C interface */
extern "C" {

    // create a new runtime engine
    rphp::pRuntimeEngine* rphp_newRuntimeEngine() {
        std::cout << "in C API runtime now" << std::endl;
        rphp::pRuntimeEngine* rt = new rphp::pRuntimeEngine();
        return rt;
    }

    // destroy runtime engine
    void rphp_deleteRuntimeEngine(rphp::pRuntimeEngine* e) {
        std::cout << "closing in C API runtime now" << std::endl;
        delete e;
    }

    void sayHello() {
        std::cout << "saying hello" << std::endl;
    }
    
}

