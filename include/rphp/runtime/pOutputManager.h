/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_POUTPUTMANAGER_H_
#define RPHP_POUTPUTMANAGER_H_

#include "rphp/runtime/pRuntimeTypes.h"
#include "rphp/runtime/pVar.h"

#include <stack>

namespace rphp {

class pRuntimeEngine;
class pOutputBuffer;

class pOutputManager {

    pRuntimeEngine* runtime_;
    std::stack<pOutputBuffer*> bufferStack_;

public:

    // constructors
    pOutputManager(pRuntimeEngine *r);

    ~pOutputManager() {
        flushAndFreeAll();
    }

    // flush and/or free one or more buffers
    void flushAndFreeAll();
    void freeAll();

    // get top buffer object
    pOutputBuffer* topBuffer() { return bufferStack_.top(); }
    const pOutputBuffer* topBuffer() const { return bufferStack_.top(); }

    // printing to the current buffer

    // TODO: logic for determining unicode mode
    void print(const char* str) {
        print(pBString(str));
    }

    void print(const pVar& v) {
        print(v.copyAsBString());
    }

    void print(pInt i) {
        pVar p(i);
        print(p.copyAsBString());
    }

    void print(const pBString& str);

    void print(const pUString& str);


};

// iostream like interface
pOutputManager& operator<< (pOutputManager& om, const char* str);
pOutputManager& operator<< (pOutputManager& om, const pInt i);
pOutputManager& operator<< (pOutputManager& om, const pVar& var);
pOutputManager& operator<< (pOutputManager& om, const pBString& str);
pOutputManager& operator<< (pOutputManager& om, const pUString& str);

}

#endif /* RPHP_POUTPUTMANAGER_H_ */
