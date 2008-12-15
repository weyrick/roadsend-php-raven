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

#ifndef RPHP_POUTPUTMANAGER_H_
#define RPHP_POUTPUTMANAGER_H_

#include <stack>
#include "rphp/runtime/pOutputBuffer.h"
#include "rphp/runtime/pTypes.h"

namespace rphp {

class pRuntimeEngine;

class pOutputManager {

    pRuntimeEngine* runtime_;
    std::stack<pOutputBuffer*> bufferStack_;

public:

    // constructors
    pOutputManager(pRuntimeEngine *r) : runtime_(r) {
        // default output buffer
        // TODO: check the runtime config for which type of default buffer to use
        bufferStack_.push(new pOutputBuffer(pOutputBuffer::bufTypeUnicode));
    }

    ~pOutputManager() {
        flushAndFreeAll();
    }

    // flush one or more buffers
    void flushAndFreeAll();

    // printing to the current buffer
    void print(const pBString& str) {
        if (bufferStack_.empty())
            return;
        *bufferStack_.top() << str;
    }

    void print(const pUString& str) {
        if (bufferStack_.empty())
            return;
        *bufferStack_.top() << str.readonlyICUString();
    }

};

}

#endif /* RPHP_POUTPUTMANAGER_H_ */
