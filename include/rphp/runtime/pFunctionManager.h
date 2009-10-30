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

#ifndef RPHP_PFUNCTIONMANAGER
#define RPHP_PFUNCTIONMANAGER

#include "rphp/runtime/pRuntimeTypes.h"

#include <boost/unordered_map.hpp>

namespace rphp {

class pRuntimeEngine;
class pFunction;
class pExtBase;

typedef boost::unordered_map<pIdentString, pFunction*> functionRegistryType;

/**

 @brief Manage functions globally available in this runtime

 */
class pFunctionManager {

private:
    pRuntimeEngine* runtime_;
    functionRegistryType functionRegistry_;

public:

    pFunctionManager(pRuntimeEngine *r) : runtime_(r), functionRegistry_() { }
    ~pFunctionManager();

    /**
        register a new builtin function, i.e. from an extension. these do not get
        lost on page reset
    */
    template <typename fPointerType>
    pFunction* registerBuiltin(const pExtBase*, const pIdentString&, fPointerType, pUInt arity);

    /**
        register a new user function. these are valid only for one page load
    */
    template <typename fPointerType>
    pFunction* registerUser(const pIdentString&, fPointerType, pUInt arity);

    pVar invoke(const pIdentString& funName);
    pVar invoke(const pIdentString& funName, const pVar& arg1);
    pVar invoke(const pIdentString& funName, const pVar& arg1, const pVar& arg2);
    pVar invoke(const pIdentString& funName, const pVar& arg1, const pVar& arg2, const pVar& arg3);
    pVar invoke(const pIdentString& funName, const pVar& arg1, const pVar& arg2, const pVar& arg3, const pVar& arg4);
    pVar invoke(const pIdentString& funName, const pVar& arg1, const pVar& arg2, const pVar& arg3, const pVar& arg4, const pVar& arg5);

};

}

#endif /* RPHP_PFUNCTIONMANAGER_ */
