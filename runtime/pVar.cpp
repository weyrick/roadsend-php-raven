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


#include "rphp/runtime/pVar.h"
#include "rphp/runtime/pTypeOperators.h"

namespace rphp {

const pVarType pVar::getType() const {
    int w = pVarData_.which();
    if (w == pVarTriStateType_) {
        return (pNull(boost::get<pTriState>(pVarData_))) ? pVarNullType : pVarBoolType;
    }
    else {
        return (pVarType)w;
    }
}

void pVar::convertToNull() {
    pVarData_ = pNull;
}

pTriState& pVar::convertToBool() {
    if (!isBool()) {
        pVar_convertToBoolVisitor cv(pVarData_);
        boost::apply_visitor(cv, pVarData_);
    }
    return getBool();
}

pInt& pVar::convertToInt() {
    if (!isInt()) {
        pVar_convertToIntVisitor cv(pVarData_);
        boost::apply_visitor(cv, pVarData_);
    }
    return getInt();
}

pBString& pVar::convertToBString() {
    if (!isBString()) {
        pVar_convertToBStringVisitor cv(pVarData_);
        boost::apply_visitor(cv, pVarData_);
    }
    assert(isBString() && "convertToBString failed to convert");
    return getBString();
}

pInt pVar::copyAsInt() const {
    pVar v(*this);
    return v.convertToInt();
}

pBString pVar::copyAsBString() const {
    pVar v(*this);
    return v.convertToBString();
}

void pVar::newEmptyHash() {
    pVarData_ = pHashP(new pHash());
}

bool pVar::evalAsBool() const {
    pVar v(*this);
    return v.convertToBool();
}

// convert to BString or UString as appropriate
void pVar::convertToString() {
    if (isBString() || isUString())
        return;
    // TODO: check runtime setting for conversion,
    // do unicode or binary
    convertToBString();
}

void intrusive_ptr_add_ref(pVar* v) {
    v->incRefCount();
#ifdef RPHP_PVAR_DEBUG
    std::cout << "pVarP [" << v << "]: inc ref count, now: " << v->getRefCount() << std::endl;
#endif
}

void intrusive_ptr_release(pVar* v) {
    v->decRefCount();
    boost::int32_t c = v->getRefCount();
#ifdef RPHP_PVAR_DEBUG
    std::cout << "pVarP [" << v << "]: dec ref count, now: " << c << std::endl;
#endif
    if (c == 0) {
        delete v;
    }
    else if (c == 1) {
        // if refcount drops to 1, ensure the pVar is not flagged as a php reference
        v->unmakeRef();
    }
}

std::ostream& operator << (std::ostream& os, const pVar& v)
{
    return os << v.pVarData_ << std::endl;
}

} /* namespace rphp */
