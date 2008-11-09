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

#ifndef RPHP_PVAR_H_
#define RPHP_PVAR_H_

#include <boost/cstdint.hpp>

#include <iostream>
#include "pTypes.h"

// each pVar holds an int32 which stores both refcount (lower 31 bits)
// and the "is php reference" flag (high bit)
#define PVAR_RCNT_BITS 0x7fffffff
#define PVAR_RFLAG_BIT 0x80000000

namespace rphp {

class pVar {

    // variant
    pVarDataType pVarData_;
    // ref count (lower 31) and ref flag (high bit)
    boost::int32_t refData_;

public:

    /* constructors */

    // default null
    pVar(void): pVarData_(pNull), refData_(0) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: default construct (pNull)" << std::endl;
#endif
    }

    // generic
    template <typename T>
    pVar(const T &v): pVarData_(v), refData_(0) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: generic construct to: " << pVarData_.which() << std::endl;
#endif
    }

    // some specializations to avoid ambiguity

    // default to binary strings
    pVar(const char* str): pVarData_(pBString(str)), refData_(0) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: binary string construct" << std::endl;
#endif
    }
    // specify type of string to make from literal
    pVar(const char* str, pVarType t): refData_(0) {
        if (t == pVarUStringType) {
            pVarData_ = pUString(str);
        }
        else {
            pVarData_ = pBString(str);
        }
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: string construct with type specification: " << t << std::endl;
#endif
    }
    pVar(int i): pVarData_(pInt(i)), refData_(0) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: int construct: " << i << std::endl;
#endif
    }

    // convenience function for creating a new empty hash
    void newEmptyHash(void);

    /* default copy constructor */
#ifdef RPHP_PVAR_DEBUG
    pVar(pVar const& v) {
        std::cout << "pVar [" << this << "]: copy construct from type: " << v.getType() << std::endl;
        pVarData_ = v.pVarData_;
        refData_ = v.refData_;
    }
#endif

    /* default destructor */
#ifdef RPHP_PVAR_DEBUG
    ~pVar(void) {
        std::cout << "pVar [" << this << "]: destruct" << std::endl;
    }
#endif    

    /* assignment */
    template <typename T>
    void operator=(T val) { pVarData_ = val; }

    // some specializations to avoid ambiguity
    // default to binary strings
    void operator=(const char* str) { pVarData_ = pBString(str); }
    void operator=(int i) { pVarData_ = pInt(i); }

    /* reference counting counting */
    inline boost::int32_t getRefCount(void) {
        // note, refcount only makes sense if variant type is NOT pVarP
        return refData_ & PVAR_RCNT_BITS;
    }

    inline boost::int32_t incRefCount(void) {
        // TODO: overflow?
        refData_ += 1;
    }
    inline boost::int32_t decRefCount(void) {
        refData_ -= 1;
    }

    /* php reference variable flag */
    inline bool isRef(void) {
        return refData_ & PVAR_RFLAG_BIT;
    }
    // this only make sense when the variant type is pVarP,
    inline void makeRef(void) {
        if (pVarData_.which() == pVarPtrType_)
            refData_ |= PVAR_RFLAG_BIT;
    }
    inline void unmakeRef(void) {
        refData_ ^= PVAR_RFLAG_BIT;
    }

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
    bool isBoxed() const {
        return (pVarData_.which() == pVarPtrType_);
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
    void convertToString();
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

    pUString& getUString() {
        return boost::get<pUString&>(pVarData_);
    }
    
    const pUString& getUString() const {
        return boost::get<const pUString&>(pVarData_);
    }

    pHashP& getHash() {
        return boost::get<pHashP&>(pVarData_);
    }
    
    const pHashP& getHash() const {
        return boost::get<const pHashP&>(pVarData_);
    }

    const pHashP& getConstHash() const {
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
    
    pVarP& getPtr() {
        return boost::get<pVarP&>(pVarData_);
    }
    
    const pVarP& getPtr() const {
        return boost::get<const pVarP&>(pVarData_);
    }

};

// used by boost::instrusive_ptr to handle ref counting
void intrusive_ptr_add_ref(pVar* v);
void intrusive_ptr_release(pVar* v);

} /* namespace rphp */

#endif /* RPHP_PVAR_H_ */
