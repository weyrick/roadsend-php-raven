/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#include "rphp/runtime/pOutputManager.h"

#include <unicode/ustream.h>
#include <iostream>

namespace rphp {

void pOutputManager::flushAndFreeAll() {
    while( !bufferStack_.empty() ) {
        if (bufferStack_.top()->pOutputBuffer::bufTypeUnicode) {
            std::cout << *(bufferStack_.top()->getUBuffer());
        }
        else {
            std::cout << *(bufferStack_.top()->getBBuffer());
        }
        delete bufferStack_.top();
        bufferStack_.pop();
    }
}

void pOutputManager::freeAll() {
    while( !bufferStack_.empty() ) {
        delete bufferStack_.top();
        bufferStack_.pop();
    }
}


pOutputManager& operator<< (pOutputManager& om, const char* str) {
    om.print(str);
    return om;
}

pOutputManager& operator<< (pOutputManager& om, const pInt i) {
    om.print(i);
    return om;
}

pOutputManager& operator<< (pOutputManager& om, const pVar& var) {
    if (var.isBString())
        om.print(var.getBString());
    else if (var.isUString())
        om.print(var.getUString());
    else
        om.print(var.copyAsBString());
    return om;
}

pOutputManager& operator<< (pOutputManager& om, const pBString& str) {
    om.print(str);
    return om;
}

pOutputManager& operator<< (pOutputManager& om, const pUString& str) {
    om.print(str);
    return om;
}

}


