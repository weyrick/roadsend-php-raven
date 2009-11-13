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


#include "rphp/runtime/pVar.h"
#include "rphp/runtime/pTypeOperators.h"
#include "rphp/runtime/pRuntimeEngine.h"

#include <unicode/ustream.h>

namespace rphp {

pVarType pVar::getType() const {
    int w = (isBoxed() ? getPtr()->pVarData_.which() : pVarData_.which());
    if (w == pVarTriStateType_) {
        return (pNull(boost::get<pTriState>(pVarData_))) ? pVarNullType : pVarBoolType;
    }
    else {
        return (pVarType)w;
    }
}

void pVar::convertToNull() {
    PVAR_DATA = pNull;
}

pTriState& pVar::convertToBool() {
    if (!isBool()) {
        pVarDataType* d;
        d = (isBoxed() ? &getPtr()->pVarData_ : &pVarData_);
        pVar_convertToBoolVisitor cv(*d);
        boost::apply_visitor(cv, *d);
    }
    return getBool();
}

pHashP& pVar::convertToHash() {
	if(!isHash()) {
        pVarDataType* d = (isBoxed() ? &getPtr()->pVarData_ : &pVarData_);
		pVar_convertToHashVisitor cv(*d);
		boost::apply_visitor(cv, *d);
	}
	return getHashP();
}

pInt& pVar::convertToInt() {
    if (!isInt()) {
        pVarDataType* d = (isBoxed() ? &getPtr()->pVarData_ : &pVarData_);
        pVar_convertToIntVisitor cv(*d);
        boost::apply_visitor(cv, *d);
    }
    return getInt();
}

pBString& pVar::convertToBString() {
    if (!isBString()) {
        pVarDataType* d = (isBoxed() ? &getPtr()->pVarData_ : &pVarData_);
        pVar_convertToBStringVisitor cv(*d);
        boost::apply_visitor(cv, *d);
    }
    assert(isBString() && "convertToBString failed to convert");
    return getBString();
}

pUStringP& pVar::convertToUString() {
    if (!isUString()) {
        pVarDataType* d = (isBoxed() ? &getPtr()->pVarData_ : &pVarData_);
        pVar_convertToUStringVisitor cv(*d);
        boost::apply_visitor(cv, *d);
    }
    assert(isUString() && "convertToUString failed to convert");
    return getUStringP();
}

pInt pVar::copyAsInt() const {
    pVar v(*this);
    return v.convertToInt();
}

pHashP pVar::copyAsHash() const {
    pVar v(*this);
    return v.convertToHash();
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
void pVar::convertToString(const pRuntimeEngine* r) {
    if (isBString() || isUString())
        return;
    // do unicode or binary according to runtime
    if (r->unicode().byDefault())
        convertToUString();
    else
        convertToBString();
}

// copy as BString or UString as appropriate
pVar pVar::copyAsString(const pRuntimeEngine* r) const {
    pVar v(*this);
    v.convertToString(r);
    return v;
}

pVar& pVar::operator+=(const pVar& rhs) {

    // we want this to be our fastest case
    if (isInt() && rhs.isInt()) {
        const pInt& a(getInt());
        const pInt& b(rhs.getInt());
        pInt lval(a+b);
        if ( (a & RPHP_INT_MIN) == (b & RPHP_INT_MIN)
          && (a & RPHP_INT_MIN) != (lval & RPHP_INT_MIN) ) {
            // overflow
            PVAR_DATA = pBigIntP(new pBigInt(pBigInt(a)+pBigInt(b)));
        }
        else {
            PVAR_DATA = lval;
        }
    }
    else if (isBigInt() && rhs.isBigInt()) {
        pBigIntP& a = getBigIntP();
        const pBigIntP& b = rhs.getBigIntP();
        *a += *b;
    }
    else if (isHash() && rhs.isHash()) {
        // TODO: array concat
        PVAR_DATA = pInt(0);
    }
    else {
        // default case
        PVAR_DATA = copyAsInt() + rhs.copyAsInt();
    }

    return *this;

}

pVar& pVar::operator-=(const pVar& rhs) {
    return *this;
}

pVar& pVar::operator*=(const pVar& rhs) {
    return *this;
}

pVar& pVar::operator/=(const pVar& rhs) {
    return *this;
}

const pVar pVar::operator+(const pVar& rhs) const {
    return pVar(*this) += rhs;
}

const pVar pVar::operator-(const pVar& rhs) const {
    return pVar(*this) -= rhs;
}

const pVar pVar::operator*(const pVar& rhs) const {
    return pVar(*this) *= rhs;

}

const pVar pVar::operator/(const pVar& rhs) const {
    return pVar(*this) /= rhs;
}


std::ostream& operator << (std::ostream& os, const pVar& v)
{
    return os << v.pVarData_ << std::endl;
}

} /* namespace rphp */
