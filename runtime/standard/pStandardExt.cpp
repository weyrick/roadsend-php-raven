/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2009 Shannon Weyrick (weyrick@roadsend.com)
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

#include "rphp/runtime/standard/pStandardExt.h"

#include "rphp/runtime/pRuntimeEngine.h"
#include "rphp/runtime/pFunction.h"
#include "rphp/runtime/pVarOperators.h"

#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range.hpp>

namespace rphp {
 namespace ext {
   namespace standard {

void pStandardExt::extensionStartup() {

    //std::cout << "initializing standard extension" << std::endl;

    pFunction* f;

    // TODO: static initialization?
    f = registerBuiltin("var_dump", &var_dump, 1);
    f->param(0)->setName("var");

    f = registerBuiltin("strlen", &strlen, 1);
    f->param(0)->setName("string");

    f = registerBuiltin("strpos", &strpos, 3);
    f->setRequiredArity(2);
    f->param(0)->setName("haystack");
    f->param(1)->setName("needle");
    f->param(2)->setName("offset");
    f->param(2)->setDefault(0);

}

void pStandardExt::extensionShutdown() {

    //std::cout << "deinitializing standard extension" << std::endl;

}

/* Library Implementation */

void var_dump(RPHP_STDFUNC_ARGS, const pVar& v) {
    v.applyVisitor<pVar_var_dump>(runtime->output.topBuffer());
    *retVal = pNull;
}

void strlen(RPHP_STDFUNC_ARGS, const pVar& v) {
    pVar lv(v.copyAsString(runtime));
    if (lv.isBString())
        *retVal = (pInt)lv.getBString().length();
    else
        *retVal = (pInt)lv.getUStringP()->length();
}

void strpos(RPHP_STDFUNC_ARGS, const pVar& haystackV, const pVar& needleV, const pVar& offsetV) {

    // TODO: unicode

    pBString haystack(haystackV.copyAsBString());
    pBString needle(needleV.copyAsBString());
    pInt offset(offsetV.copyAsInt());

    // TODO: use offset

    boost::iterator_range<pBString::iterator> result = boost::algorithm::find_first(haystack, needle);
    if (result.begin() == haystack.end()) {
        *retVal = pNull;
    }
    else {
        *retVal = (pInt)(result.begin() - haystack.begin());
    }

}

  } // ns standard
 } // ns ext
} // ns rphp
