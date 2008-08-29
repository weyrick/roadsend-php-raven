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

#ifndef RPHP_PVAR_H_
#define RPHP_PVAR_H_

#include <iostream>
#include "pTypes.h"

namespace rphp {

class pVar {

private:
    pVarDataType pVarData_;

public:

    /* constructors */
    pVar(void): pVarData_(pNull) { }
    template <typename T>
    pVar(const T &v): pVarData_(v) { }
    
    /* default copy constructor */

    /* assignment */
    template <typename T>
    void operator=(T val) { pVarData_ = val; }

    /* custom visitors */
    template <typename T>
    typename T::result_type applyVisitor() {
        return boost::apply_visitor( T(), pVarData_ );
    }

    /* type checks */
    const pVarType getType() const;
    const bool isNull() const {
        return ((pVarData_.which() == pVarTriStateType_) && pNull(boost::get<pTriState>(pVarData_)));
    }
    const bool isBool() const {
        return ((pVarData_.which() == pVarTriStateType_) && !pNull(boost::get<pTriState>(pVarData_)));
    }
    const bool isInt() const {
        return (pVarData_.which() == pVarIntType_);
    }
    const bool isFloat() const {
        return (pVarData_.which() == pVarFloatType_);
    }
    const bool isBString() const {
        return (pVarData_.which() == pVarBStringType_);
    }
    const bool isUString() const {
        return (pVarData_.which() == pVarUStringType_);
    }
    const bool isHash() const {
        return (pVarData_.which() == pVarHashType_);
    }
    const bool isObject() const {
        return (pVarData_.which() == pVarObjectType_);
    }
    const bool isResource() const {
        return (pVarData_.which() == pVarResourceType_);
    }

    // stream interface
    friend std::ostream& operator << (std::ostream& os, const rphp::pVar& v);

    /* in place type conversion */
    // null
    // bool
    pInt& convertToInt();
    // float
    pBString& convertToBString();
    // ustring
    // hash
    // object
    // resource

    /* return type casted copy */
    // null
    // bool
    pInt copyAsInt() const;
    // float
    pBString copyAsBString() const;
    // ustring
    // hash
    // object
    // resource

    /* these do no conversions, and throw exception if the wrong type is accessed */
    pTriState& getBool() {
        return boost::get<pTriState&>(pVarData_);
    }

    const pTriState& getBool() const {
        return boost::get<const pTriState&>(pVarData_);
    }
    
    pInt& getInt() {
        return boost::get<pInt&>(pVarData_);
    }
    
    const pInt& getInt() const {
        return boost::get<const pInt&>(pVarData_);
    }

    pFloat& getFloat() {
        return boost::get<pFloat&>(pVarData_);
    }
    
    const pFloat& getFloat() const {
        return boost::get<const pFloat&>(pVarData_);
    }

    pBString& getBString() {
        return boost::get<pBString&>(pVarData_);
    }
    
    const pBString& getBString() const {
        return boost::get<const pBString&>(pVarData_);
    }

    pUStringP& getUString() {
        return boost::get<pUStringP&>(pVarData_);
    }
    
    const pUStringP& getUString() const {
        return boost::get<const pUStringP&>(pVarData_);
    }

    pHashP& getHash() {
        return boost::get<pHashP&>(pVarData_);
    }
    
    const pHashP& getHash() const {
        return boost::get<const pHashP&>(pVarData_);
    }

    pObjectP& getObject() {
        return boost::get<pObjectP&>(pVarData_);
    }
    
    const pObjectP& getObject() const {
        return boost::get<const pObjectP&>(pVarData_);
    }

    pResourceP& getResource() {
        return boost::get<pResourceP&>(pVarData_);
    }
    
    const pResourceP& getResource() const {
        return boost::get<const pResourceP&>(pVarData_);
    }
    
    pVarP& getRef() {
        return boost::get<pVarP&>(pVarData_);
    }
    
    const pVarP& getRef() const {
        return boost::get<const pVarP&>(pVarData_);
    }

};

} /* namespace rphp */

#endif /* RPHP_PVAR_H_ */
