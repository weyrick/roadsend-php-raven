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

#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range.hpp>

#include <iostream>

#include "rphp/runtime/pRuntime.h"
#include "rphp/runtime/pFunction.h"
#include "rphp/runtime/standard/pStandardExt.h"

using namespace boost;
using namespace boost::algorithm;

namespace rphp {

void pStandardExt::extensionStartup() {

    //std::cout << "initializing standard extension" << std::endl;

    pFunction* f;

    f = registerBuiltin("var_dump", (pFunPointer1)bind(&pStandardExt::var_dump, this, _1));
    f->param(0)->setName("var");

    f = registerBuiltin("strlen", (pFunPointer1)bind(&pStandardExt::strlen, this, _1));
    f->param(0)->setName("string");

    f = registerBuiltin("strpos", (pFunPointer3)bind(&pStandardExt::strpos, this, _1, _2, _3));
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

class var_dump_visitor : public boost::static_visitor<void>
{
private:
    pRuntimeEngine* runtime_;
public:
    var_dump_visitor(pRuntimeEngine *r): runtime_(r) { }

    void operator()(const pTriState &v) const {
        if (v == pNull) {
            runtime_->outputManager->print(pBString("NULL\n"));
        }
        else {
            runtime_->outputManager->print(pBString("bool("));
            if (v == pTrue)
                runtime_->outputManager->print(pBString("true"));
            else
                runtime_->outputManager->print(pBString("false"));
            runtime_->outputManager->print(pBString(")\n"));
        }
    }

    void operator()(const pInt &v) const {
        runtime_->outputManager->print(pBString("int"));
    }

    void operator()(const pFloat &v) const {
        runtime_->outputManager->print(pBString("float"));
    }

    void operator()(const pBString &v) const {
        runtime_->outputManager->print(pBString("bstring"));
    }

    void operator()(const pUString &v) const {
        runtime_->outputManager->print(pBString("ustring"));
    }

    void operator()(const pHashP &v) const {
        runtime_->outputManager->print(pBString("array"));
    }

    void operator()(const pObjectP &v) const {
        runtime_->outputManager->print(pBString("object"));
    }

    void operator()(const pResourceP &v) const {
        runtime_->outputManager->print(pBString("resource"));
    }

    void operator()(const pVarP &v) const {
        runtime_->outputManager->print(pBString("ref"));
    }
};


pVar pStandardExt::var_dump(pVar v) {
    v.applyVisitor<var_dump_visitor>(runtime_);
    return pNull;
}

pInt pStandardExt::strlen(pVar v) {
    v.convertToString();
    if (v.isBString())
        return (pInt)v.getBString().length();
    else
        return (pInt)v.getUString().length();
}

pVar pStandardExt::strpos(pVar haystackV, pVar needleV, pVar offsetV) {

    // TODO: unicode

    pBString haystack = haystackV.convertToBString();
    pBString needle = needleV.convertToBString();
    pInt offset = offsetV.convertToInt();

    // TODO: use offset

    iterator_range<pBString::iterator> result = find_first(haystack, needle);
    if (result.begin() == haystack.end()) {
        return pNull;
    }
    else {
        return (pInt)(result.begin() - haystack.begin());
    }

}


}
