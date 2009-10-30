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

#include "rphp/runtime/pFunctionManager.h"

#include "rphp/runtime/pFunction.h"
#include "rphp/runtime/pRuntimeEngine.h"
#include "rphp/runtime/pErrorManager.h"
#include "rphp/runtime/pSupport.h"

namespace rphp {

pFunctionManager::~pFunctionManager() {
    // free function entries
    foreach (functionRegistryType::value_type i, functionRegistry_) {
        delete i.second;
    }
}

template <typename fPointerType>
pFunction* pFunctionManager::registerBuiltin(const pExtBase* sourceExt, const pIdentString& funName, fPointerType fP, pUInt arity) {

    pFunction *f = new pFunction(sourceExt, funName, fP, arity);
    functionRegistry_[toLowerCopy(funName)] = f;
    return f;

}

template <typename fPointerType>
pFunction* pFunctionManager::registerUser(const pIdentString& funName, fPointerType fP, pUInt arity) {

    pFunction *f = new pFunction(funName, fP, arity);
    functionRegistry_[toLowerCopy(funName)] = f;
    return f;

}

pVar pFunctionManager::invoke(const pIdentString& funName) {
    functionRegistryType::iterator function = functionRegistry_.find(toLowerCopy(funName));
    if (function != functionRegistry_.end()) {
        return (*function).second->invoke(runtime_);
    }
    else {
        runtime_->errorManager->fatalError("undefined function: "+funName);
        return pNull;
    }
}

pVar pFunctionManager::invoke(const pIdentString& funName, const pVar& arg1) {
    functionRegistryType::iterator function = functionRegistry_.find(toLowerCopy(funName));
    if (function != functionRegistry_.end()) {
        return (*function).second->invoke(runtime_, arg1);
    }
    else {
        runtime_->errorManager->fatalError("undefined function: "+funName);
        return pNull;
    }
}

pVar pFunctionManager::invoke(const pIdentString& funName, const pVar& arg1, const pVar& arg2) {
    functionRegistryType::iterator function = functionRegistry_.find(toLowerCopy(funName));
    if (function != functionRegistry_.end()) {
        return (*function).second->invoke(runtime_, arg1, arg2);
    }
    else {
        runtime_->errorManager->fatalError("undefined function: "+funName);
        return pNull;
    }
}

pVar pFunctionManager::invoke(const pIdentString& funName, const pVar& arg1, const pVar& arg2, const pVar& arg3) {
    functionRegistryType::iterator function = functionRegistry_.find(toLowerCopy(funName));
    if (function != functionRegistry_.end()) {
        return (*function).second->invoke(runtime_, arg1, arg2, arg3);
    }
    else {
        runtime_->errorManager->fatalError("undefined function: "+funName);
        return pNull;
    }
}

pVar pFunctionManager::invoke(const pIdentString& funName, const pVar& arg1, const pVar& arg2, const pVar& arg3, const pVar& arg4) {
    functionRegistryType::iterator function = functionRegistry_.find(toLowerCopy(funName));
    if (function != functionRegistry_.end()) {
        return (*function).second->invoke(runtime_, arg1, arg2, arg3, arg4);
    }
    else {
        runtime_->errorManager->fatalError("undefined function: "+funName);
        return pNull;
    }
}

pVar pFunctionManager::invoke(const pIdentString& funName, const pVar& arg1, const pVar& arg2, const pVar& arg3, const pVar& arg4, const pVar& arg5) {
    functionRegistryType::iterator function = functionRegistry_.find(toLowerCopy(funName));
    if (function != functionRegistry_.end()) {
        return (*function).second->invoke(runtime_, arg1, arg2, arg3, arg4, arg5);
    }
    else {
        runtime_->errorManager->fatalError("undefined function: "+funName);
        return pNull;
    }
}


// template defines
template pFunction* pFunctionManager::registerBuiltin<pFunPointer0>(const pExtBase* sourceExt, const pIdentString& funName, pFunPointer0 fP, pUInt arity);
template pFunction* pFunctionManager::registerBuiltin<pFunPointer1>(const pExtBase* sourceExt, const pIdentString& funName, pFunPointer1 fP, pUInt arity);
template pFunction* pFunctionManager::registerBuiltin<pFunPointer2>(const pExtBase* sourceExt, const pIdentString& funName, pFunPointer2 fP, pUInt arity);
template pFunction* pFunctionManager::registerBuiltin<pFunPointer3>(const pExtBase* sourceExt, const pIdentString& funName, pFunPointer3 fP, pUInt arity);
template pFunction* pFunctionManager::registerBuiltin<pFunPointer4>(const pExtBase* sourceExt, const pIdentString& funName, pFunPointer4 fP, pUInt arity);
template pFunction* pFunctionManager::registerBuiltin<pFunPointer5>(const pExtBase* sourceExt, const pIdentString& funName, pFunPointer5 fP, pUInt arity);

template pFunction* pFunctionManager::registerUser<pFunPointer0>(const pIdentString& funName, pFunPointer0 fP, pUInt arity);
template pFunction* pFunctionManager::registerUser<pFunPointer1>(const pIdentString& funName, pFunPointer1 fP, pUInt arity);
template pFunction* pFunctionManager::registerUser<pFunPointer2>(const pIdentString& funName, pFunPointer2 fP, pUInt arity);
template pFunction* pFunctionManager::registerUser<pFunPointer3>(const pIdentString& funName, pFunPointer3 fP, pUInt arity);
template pFunction* pFunctionManager::registerUser<pFunPointer4>(const pIdentString& funName, pFunPointer4 fP, pUInt arity);
template pFunction* pFunctionManager::registerUser<pFunPointer5>(const pIdentString& funName, pFunPointer5 fP, pUInt arity);

}


