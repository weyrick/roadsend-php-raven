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

#ifndef RPHP_PFUNCTION
#define RPHP_PFUNCTION

#include <vector>
#include "pVar.h"

namespace rphp {

class pClass;
class pExtBase;

typedef enum { pBuiltinFunType, pUserFunType } pFunType;

struct pFunctionParam {
    pIdentString paramName;
    bool isOptional;
    pVar defaultValue;
    bool isByRef;
    bool isArray;      // array type hint
    pClass* classHint; // class type hint
};

class pFunction {

    // declaration location (user function)
    pSourceStartEndLocation sourceLocation_;

    // or parent extension (only builtins)
    const pExtBase* parentExtension_;

    // docComment?

    // signature
    pIdentString name_;
    pFunType funType_;
    pUInt requiredArity_;
    pUInt maxArity_;
    bool isVarArity_;

    std::vector<pFunctionParam> paramList_;

    const pFunPointer1 funPointer1_;

public:

    // standard builtin function: one argument
    pFunction(const pExtBase* e, const pIdentString& f, const pFunPointer1& fun) :
        parentExtension_(e),
        name_(f),
        funType_(pBuiltinFunType),
        requiredArity_(1),
        maxArity_(1),
        isVarArity_(false),
        funPointer1_(fun)
    {

    }

    // invocation
    pVar invoke(pVar arg1) {
        return funPointer1_(arg1);
    }

};

}

#endif /* RPHP_PFUNCTION */
