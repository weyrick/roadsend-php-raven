/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
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

#ifndef RPHP_PFUNCTIONSIG
#define RPHP_PFUNCTIONSIG

#include <vector>
#include "pVar.h"

namespace rphp {

    class pClass;
    class pExtBase;

    typedef enum { pBuiltinFunType, pUserFunType } pFunType;

    struct pFunctionParam {
        pUString paramName;
        bool isOptional;
        pVar defaultValue;
        bool isByRef;
        bool isArray;      // array type hint
        pClass* classHint; // class type hint
    };

    class pFunctionSig {
        private:
            // declaration location (user function)
            const pSourceStartEndLocation sourceLocation;

            // or parent extension (only builtins)
            const pExtBase* parentExtension;

            // docComment?

            // signature
            const pUString functionName;
            const pFunType funType;
            const pUInt requiredArity;
            const pUInt maxArity;
            const bool isVarArity;

            std::vector<pFunctionParam> paramList;

            const pFunPointer1 funPointer1;

        public:

            // standard builtin function: one argument
            pFunctionSig(const pExtBase* e, const pUString& f, const pFunPointer1& fun) :
                parentExtension(e),
                functionName(f),
                funType(pBuiltinFunType),
                requiredArity(1),
                maxArity(1),
                isVarArity(false),
                funPointer1(fun)
            {

            }

            // invocation
            pVar invoke(pVar arg1) {
                return funPointer1(arg1);
            }

    };

}

#endif /* RPHP_PFUNCTIONSIG */
