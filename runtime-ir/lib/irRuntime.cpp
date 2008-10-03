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
#include "irRuntime.h"

extern "C" {

    // create a new runtime engine
    rphp::pRuntimeEngine* rphp_newRuntimeEngine() {
        rphp::pRuntimeEngine* rt = new rphp::pRuntimeEngine();
        return rt;
    }

    // destroy runtime engine
    void rphp_deleteRuntimeEngine(rphp::pRuntimeEngine* e) {
        delete e;
    }

    // print a c string to the current output buffer in the given runtime
    void rphp_print_cstr(rphp::pRuntimeEngine* e, char* str) {
        e->outputManager->print(rphp::pBString(str));
    }


}

