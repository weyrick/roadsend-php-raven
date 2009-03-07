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

#include <iostream>
#include "rphp/runtime/pSupport.h"
#include "rphp/runtime/pFunction.h"
#include "rphp/runtime/pFunctionManager.h"

namespace rphp {

pFunctionManager::~pFunctionManager() {
    // free function entries
    foreach (functionRegistryType::value_type i, functionRegistry_) {
        delete i.second;
    }
}

template <typename fPointerType>
pFunction* pFunctionManager::registerBuiltin(const pExtBase* sourceExt, const pIdentString& funName, fPointerType fP) {

    pFunction *f = new pFunction(sourceExt, funName, fP);
    functionRegistry_[toLowerCopy(funName)] = f;
    return f;

}

template <typename fPointerType>
pFunction* pFunctionManager::registerUser(const pIdentString& funName, fPointerType fP) {
    pFunction *f = new pFunction(funName, fP);
    functionRegistry_[toLowerCopy(funName)] = f;
    return f;
    
}

pVar pFunctionManager::invoke(const pIdentString& funName) {
    functionRegistryType::iterator function = functionRegistry_.find(toLowerCopy(funName));
    if (function != functionRegistry_.end()) {
        return (*function).second->invoke(runtime_);
    }
    else {
        throw pRuntimeError("undefined function: "+funName);
        //return pNull;
    }
}

pVar pFunctionManager::invoke(const pIdentString& funName, pVar arg1) {
    functionRegistryType::iterator function = functionRegistry_.find(toLowerCopy(funName));
    if (function != functionRegistry_.end()) {
        return (*function).second->invoke(runtime_, arg1);
    }
    else {
        throw pRuntimeError("undefined function: "+funName);
        //return pNull;
    }
}

pVar pFunctionManager::invoke(const pIdentString& funName, pVar arg1, pVar arg2, pVar arg3) {
    functionRegistryType::iterator function = functionRegistry_.find(toLowerCopy(funName));
    // TODO this needs to throw a runtime error if the function wasn't found
    if (function != functionRegistry_.end()) {
        return (*function).second->invoke(runtime_, arg1,arg2,arg3);
    }
    else {
        throw pRuntimeError("undefined function: "+funName);
        //return pNull;
    }
}


// template defines
//template pFunction* pFunctionManager::registerBuiltin<pFunPointer0>(const pExtBase* sourceExt, const pIdentString& funName, pFunPointer0 fP);
template pFunction* pFunctionManager::registerBuiltin<pFunPointer1>(const pExtBase* sourceExt, const pIdentString& funName, pFunPointer1 fP);
template pFunction* pFunctionManager::registerBuiltin<pFunPointer3>(const pExtBase* sourceExt, const pIdentString& funName, pFunPointer3 fP);

template pFunction* pFunctionManager::registerUser<pFunPointer0>(const pIdentString& funName, pFunPointer0 fP);


}


