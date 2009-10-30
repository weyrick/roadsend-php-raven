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

#include "rphp/runtime/pExtBase.h"

#include "rphp/runtime/pRuntimeEngine.h"
#include "rphp/runtime/pFunctionManager.h"

namespace rphp {

template <typename fPointerType>
pFunction* pExtBase::registerBuiltin(const pIdentString& name, fPointerType f, pUInt arity) {
    return runtime_->functionManager->registerBuiltin(this, name, f, arity);
}

// template defines
template pFunction* pExtBase::registerBuiltin<pFunPointer0>(const pIdentString& name, pFunPointer0 f, pUInt arity);
template pFunction* pExtBase::registerBuiltin<pFunPointer1>(const pIdentString& name, pFunPointer1 f, pUInt arity);
template pFunction* pExtBase::registerBuiltin<pFunPointer2>(const pIdentString& name, pFunPointer2 f, pUInt arity);
template pFunction* pExtBase::registerBuiltin<pFunPointer3>(const pIdentString& name, pFunPointer3 f, pUInt arity);
template pFunction* pExtBase::registerBuiltin<pFunPointer4>(const pIdentString& name, pFunPointer4 f, pUInt arity);
template pFunction* pExtBase::registerBuiltin<pFunPointer5>(const pIdentString& name, pFunPointer5 f, pUInt arity);
// N arity

}
