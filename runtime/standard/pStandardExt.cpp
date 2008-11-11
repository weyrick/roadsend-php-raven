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

#include "pFunction.h"
#include "pStandardExt.h"

namespace rphp {

void pStandardExt::extensionStartup() {

    //std::cout << "initializing standard extension" << std::endl;

    pFunction* f;

    f = registerBuiltin("strlen", (pFunPointer1)boost::bind(&pStandardExt::strlen, this, _1));
    f->param(0)->setName("string");
    
    f = registerBuiltin("strpos", (pFunPointer3)boost::bind(&pStandardExt::strpos, this, _1, _2, _3));
    f->setRequiredArity(2);
    f->param(0)->setName("haystack");
    f->param(1)->setName("needle");
    f->param(2)->setName("offset");
    f->param(2)->setDefault(0);

}

void pStandardExt::extensionShutdown() {

    //std::cout << "deinitializing standard extension" << std::endl;

}

/* Library Implementation */

pInt pStandardExt::strlen(pVar v) {
    return (pInt)v.convertToBString().length();
}

pVar pStandardExt::strpos(pVar haystack, pVar needle, pVar offset) {

    return pNull;

}


}
