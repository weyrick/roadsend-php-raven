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

#ifndef RPHP_PFUNCTIONMANAGER
#define RPHP_PFUNCTIONMANAGER

#include <boost/unordered_map.hpp>

#include "rphp/runtime/pTypes.h"

namespace rphp {

typedef boost::unordered_map<pIdentString, pFunction*> functionRegistryType;

class pRuntimeEngine;
class pFunction;
class pExtBase;

class pFunctionManager {

private:
    pRuntimeEngine* runtime_;
    functionRegistryType functionRegistry_;

public:

    pFunctionManager(pRuntimeEngine *r) : runtime_(r), functionRegistry_() { }
    ~pFunctionManager();

    template <typename fPointerType>
    pFunction* registerBuiltin(const pExtBase*, const pIdentString&, fPointerType);

    template <typename fPointerType>
    pFunction* registerUser(const pIdentString&, fPointerType);
    
    pVar invoke(const pIdentString& funName);
    pVar invoke(const pIdentString& funName, pVar arg1);
    pVar invoke(const pIdentString& funName, pVar arg1, pVar arg2, pVar arg3);
    
};

}

#endif /* RPHP_PFUNCTIONMANAGER_ */
