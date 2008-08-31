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

    // default null
    pVar(void): pVarData_(pNull) { }

    // generic
    template <typename T>
    pVar(const T &v): pVarData_(v) { }

    // some specializations to avoid ambiguity

    // default to binary strings
    pVar(const char* str): pVarData_(pBString(str)) { }
    // specify type of string to make from literal
    pVar(const char* str, pVarType t) {
        if (t == pVarUStringType) {
            pVarData_ = pUStringP(new UnicodeString(str));
        }
        else {
            pVarData_ = pBString(str);
        }
    }
    pVar(int i): pVarData_(pInt(i)) { }

    // convenience function for creating a new empty hash
    void newEmptyHash(void);

    /* default copy constructor */

    /* assignment */
    template <typename T>
    void operator=(T val) { pVarData_ = val; }

    // some specializations to avoid ambiguity
    // default to binary strings
    void operator=(const char* str) { pVarData_ = pBString(str); }
    void operator=(int i) { pVarData_ = pInt(i); }

    /* custom visitors */
    template <typename T>
    typename T::result_type applyVisitor() {
        return boost::apply_visitor( T(), pVarData_ );
    }

    /* type checks */
    const pVarType getType() const;
    bool isNull() const {
        return ((pVarData_.which() == pVarTriStateType_) && pNull(boost::get<pTriState>(pVarData_)));
    }
    bool isBool() const {
        return ((pVarData_.which() == pVarTriStateType_) && !pNull(boost::get<pTriState>(pVarData_)));
    }
    bool isInt() const {
        return (pVarData_.which() == pVarIntType_);
    }
    bool isFloat() const {
        return (pVarData_.which() == pVarFloatType_);
    }
    bool isBString() const {
        return (pVarData_.which() == pVarBStringType_);
    }
    bool isUString() const {
        return (pVarData_.which() == pVarUStringType_);
    }
    bool isHash() const {
        return (pVarData_.which() == pVarHashType_);
    }
    bool isObject() const {
        return (pVarData_.which() == pVarObjectType_);
    }
    bool isResource() const {
        return (pVarData_.which() == pVarResourceType_);
    }
    bool isRef() const {
        return (pVarData_.which() == pVarRefType_);
    }

    // stream interface
    friend std::ostream& operator << (std::ostream& os, const rphp::pVar& v);

    // evaluation
    bool evalAsBool() const;
    
    /* in place type conversion */
    void convertToNull();
    pTriState& convertToBool();
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
