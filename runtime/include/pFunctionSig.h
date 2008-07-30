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
#include <string>
#include <boost/function.hpp>
#include "pVar.h"

namespace rphp {

    class pClass;
    class pExtBase;

    typedef boost::function<pVar (pVar)> pFunPointer1;

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
            // declaration location
            pUInt startLineNum;
            pUInt endLineNum;
            pUString fileName;
            pExtBase* parentExtension;

            // docComment?

            // signature
            pUString functionName;
            pUString canonicalName;
            pFunType funType;
            pUInt arity;
            pUInt minRequiredArity;
            bool isVarArity;

            std::vector<pFunctionParam> paramList;

        public:

    };

}

#endif /* RPHP_PFUNCTIONSIG */
