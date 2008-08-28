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

#ifndef RPHP_PTYPEOPERATIONS_H_
#define RPHP_PTYPEOPERATIONS_H_

#include "pTypes.h"

U_NAMESPACE_BEGIN
// boost compatible hash function for UnicodeString
std::size_t hash_value(const rphp::pUString &v);
U_NAMESPACE_END

namespace rphp {

/* a visitor for determining type of pVar. set pVar_getType  */
// TODO: is there a quick, reliable way to check which() in the variant?
class pVar_typeCheckVisitor : public boost::static_visitor<pVarType> {

public:

    pVarType operator()(const pTriState &v) const {
        return (pNull(v)) ? pVarNullType : pVarBoolType;
    }

    pVarType operator()(const pInt &v) const {
        return pVarIntType;
    }

    pVarType operator()(const pFloat &v) const {
        return pVarFloatType;
    }

    pVarType operator()(const pBString &v) const {
        return pVarBStringType;
    }

    pVarType operator()(const pUStringP &v) const {
        return pVarUStringType;
    }

    pVarType operator()(const pHashP &v) const {
        return pVarHashType;
    }

    pVarType operator()(const pObjectP &v) const {
        return pVarObjectType;
    }

    pVarType operator()(const pResourceP &v) const {
        return pVarResourceType;
    }

    pVarType operator()(const pVarRef &v) const {
        return pVarRefType;
    }

};


/* a visitor for converting to a php number (long or float), in place */
class pVar_convertToNumberVisitor : public boost::static_visitor<void> {
protected:
    pVar &var;

public:
    pVar_convertToNumberVisitor(pVar &v) : var(v) { }

    void operator()(pTriState &v)  {
            (v) ? var = 1l : var = 0l;
    }

    void operator()(pInt &v) { /* nothing */ }
    
    void operator()(pFloat &v) { /* nothing */ }

    void operator()(pBString &v);

    void operator()(pUStringP &v);

    void operator()(pHashP &v) {
        var = (pInt)v->getSize();
    }

    void operator()(pObjectP &v) {
        var = (pInt)v->getNumProperties();
    }

    void operator()(pResourceP &v) {
        // TODO: return static resource count
        var = 0l;
    }

    void operator()(pVarRef &v) {
        // unbox
        //boost::apply_visitor(pVar_convertToNumberVisitor(*r), *r);
    }

};

class pVar_convertToBStringVisitor : public boost::static_visitor<void> {
protected:
    pVar &var;

public:
    pVar_convertToBStringVisitor(pVar &v) : var(v) { }

    void operator()(pTriState &v) {
            (v) ? var = pBString("1") : var = pBString("0");
    }

    void operator()(pInt &v) {
        // TODO: real conversion
        var = pBString("some pInt");
    }

    void operator()(pFloat &v) {
        // TODO: real conversion
        var = pBString("some pFloat");
    }

    void operator()(pBString &v) { /* nothing */ }

    void operator()(pUStringP &v) {
        // TODO
    }

    void operator()(pHashP &v) {
        var = pBString("array");
    }

    void operator()(pObjectP &v) {
        // TODO: toString
        var = pBString("object");
    }

    void operator()(pResourceP &v) {
        var = pBString("resource");
    }

    void operator()(pVarRef &v) const {
        // TODO
        // unbox
        //boost::apply_visitor(convertToNumber(*r), *r);
    }

};

/* type checker */
inline pVarType pVar_getType(const pVar &v) {
    return boost::apply_visitor(pVar_typeCheckVisitor(), v);
}

/* type conversion (destructive, in place) */
inline void pVar_convertToNumber(pVar &v) {
    pVar_convertToNumberVisitor cv(v);
    boost::apply_visitor(cv, v);
}

/* these don't do any conversion, they simply return the variant
   as the type requested */
inline pTriState pVar_getVal_pBool(const pVar &v) {
        return boost::get<rphp::pTriState>(v);
}

inline pInt pVar_getVal_pInt(const pVar &v) {
        return boost::get<pInt>(v);
}

inline pBString pVar_getVal_pBString(const pVar &v) {
        return boost::get<pBString>(v);
}

inline pUStringP pVar_getVal_pUString(const pVar &v) {
        return boost::get<pUStringP>(v);
}

inline pVarRef pVar_getVal_pRef(const pVar &v) {
        return boost::get<pVarRef>(v);
}

/* type casts: these make a copy and conversion, returning the
   requested type */
pVar pVar_castToNumber(const pVar &v);
pVar pVar_castToBString(const pVar &v);

} /* namespace rphp */


#endif /* RPHP_PTYPEOPERATIONS_H_ */
