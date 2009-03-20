/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.cob>
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

#include "rphp/runtime/pOutputBuffer.h"

#include "rphp/runtime/pVar.h"

namespace rphp {

pOutputBuffer& operator<< (pOutputBuffer& ob, const char* str) {
    ob.print(pBString(str));
    return ob;
}

pOutputBuffer& operator<< (pOutputBuffer& ob, const pInt i) {
    ob << pVar(i);
    return ob;
}

pOutputBuffer& operator<< (pOutputBuffer& ob, const pVar& var) {
    ob.print(var.copyAsBString());
    return ob;
}

pOutputBuffer& operator<< (pOutputBuffer& ob, const pBString& str) {
    ob.print(str);
    return ob;
}

pOutputBuffer& operator<< (pOutputBuffer& ob, const pUString& str) {
    ob.print(str);
    return ob;
}


}


