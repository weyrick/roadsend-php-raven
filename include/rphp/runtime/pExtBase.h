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

#ifndef PEXTENSIONBASE_H_
#define PEXTENSIONBASE_H_

#include "rphp/runtime/pRuntimeTypes.h"

namespace rphp {

class pRuntimeEngine;
class pFunction;

/**

 @brief Base class for extensions, with hooks for startup, shutdown, and registering builtin functions and methods.

 */
class pExtBase {

protected:
    pRuntimeEngine* runtime_;
    pIdentString extName_;

    /// register a builtin function associated with this extension
    template <typename fPointerType>
    pFunction* registerBuiltin(const pIdentString& name, fPointerType f, pUInt arity);

public:
    pExtBase(pRuntimeEngine *r, pIdentString eName) : runtime_(r), extName_(eName) { }

    /// trigged once at extension startup
    virtual void extensionStartup() = 0;
    /// trigged once at extension shutdown
    virtual void extensionShutdown() = 0;

};

}

#endif /* PEXTENSIONBASE_H_ */
