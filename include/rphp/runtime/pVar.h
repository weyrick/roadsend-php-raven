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

#ifndef RPHP_PVAR_H_
#define RPHP_PVAR_H_

#include "rphp/runtime/pRuntimeTypes.h"

#ifdef RPHP_PVAR_DEBUG
#include <iostream>
#endif

namespace rphp {

/**
 @brief pVar is the basic container for a runtime variable

 pVar is built to model PHP runtime variable semantics within C++. This makes it very easy to use within the
 runtime and extensions. It tries to be as efficient as possible while still maintaining this goal. It is meant
 to be a simple container for passing and retrieving values of dynamic type (while still maintaining c++ type info).

 It is implemented with a boost::variant, which is a type-safe, stack based, discriminated union.
 pVar can represent all of the basic runtime types

 - pNull
 - pBool
 - pInt
 - pBitInt(mpz_t) (arbitrary precision integers)
 - pFloat(mpf_t)
 - pBString (binary string)
 - pUString (unicode string)
 - pHash
 - pObject
 - pResource
 - pVarP (reference counted, shared ptr to a pVar)
 * [future] pFunction (lambda function)

 In the documentation below, the following notation is used:

 X{T1}

 represents pVar named X, with active type T1. For example, a pVar named A representing a pInt = 5 would be:

 X{pInt} = 5

 When type T1 is a pVarP, we have a boxed pVar, represented as:

 X{pVarP[Y{T2}]}

 where X is the "outer" pVar (on the stack) with active type pVarP, and Y is the "inner" pVar (on the heap) of
 some type T2. As noted further below, T2 may be any type T above except pVarP.

 Data for each type is stored on the stack (i.e. within the pVar class itself), which requires
 max(sizeof(T1),..,sizeof(TN)) for types T above, plus a possible alignment.
 Currently this amounts to 4 words on x86 and x86-64: 2 for the data storage, and 1 for the
 active type discriminator

 A pVar on the heap is implemented via a reference counted, shared pointer pVarP.
 pVar* should normally not be used directly.

 Memory management is automatic: the constructor, assignment operator, and destructor for each type is run
 according to normal C++ semantics for the active type. For local variables, this means memory is reclaimed
 when the variable goes out of scope, by the destructor. For boxed pVars, memory is reclaimed by pVarP when
 the reference count drops to 0, at which point the destructor for T2 runs. Note that the destructor is also
 run (and memory may be reclaimed) when type T1 changes upon assignment.

 The default copy semantic is by value for all types except pObjectP, pResourceP, pVarP. These three are always
 shared pointers, and so copying it and then mutating it will change the shared copy. The rest are either always
 simple types copied by value, or more complex types but with copy-on-write semantics and so still relatively
 efficient to copy (until mutated).

   pVar a(5); pVar b(a); b = 4;

   a is initialized to pInt 5. b is initialized from a, and the value is copied. a = 5 and b = 4.

   pVar a("foo"); pVar b(a); b = "baz";

   a is initialized to pBString "foo". b is initialized from a. However (GNU libstdc++), because std::string is
   copy-on-write, the string is not copied until the assignment to b.

 A pVar on the stack can represent a pVarP (with T1=pVarP), in which case we say the data is "boxed". However, pVar
 guarantees that pVars cannot be chained. That is, the boxed (or inner) pVar Y can never itself represent a pVarP,
 only simpler types. This is enforced during construction and assignment by asserting that T2!=pVarP

 pVar itself hides the details of working with data on the heap: there is no difference from general user code.

 PHP "references", where more than one symbol in the runtime references the same variable data, are not supported
 in Roadsend PHP. Any circumstance that requires reference semantics can be rewritten using a PHP object.

 [future] pVar has operator overloading to support the basic arithmetic operators + - * / %, which adjust type as necessary
 according to PHP semantics for these operations. One difference between Zend and Roadsend PHP is with integer overflows:
 in Roadsend PHP, an hardware integer will overflow to an arbitrary precision integer.

 The current active type is available through getType() or the type predicates isInt(), etc. Once the type is known,
 a reference to the correctly typed data is available through the getInt(), et al. methods (available const and nonconst)
 Types may be arbitrarily converted through the appropriate methods offered, also according to PHP type conversion semantics.
 Different methods are available to change the types in place, or return a copy of the type converted data as a new pVar.

 pVar is const correct and within C++ functions and methods, should be passed by const C++ reference (i.e. "const pVar&")
 for efficiency. The runtime uses this method for PHP functions and methods.

 A visitor is available to code that needs to operate differently according to each type. This is safer than a switch statement
 that checks getType() because if pVar is changed in the future, the visitor will result in a compile error while the switch
 may go unhandled.

 -- thread safety?
 -- memory pools, creation using static function with runtime

*/

// we encapsulate the boxing of pVar data. this is used to determine
// where our data lives. this macro chooses inner or out data.
#define PVAR_DATA          (isBoxed() ? getPtr()->pVarData_ : pVarData_)

// fwd
class pRuntimeEngine;

class pVar {

    /**
        variant data
     */
    pVarDataType pVarData_;

public:

    /* constructors */

    /// default constructor holds a pNull
    pVar(void): pVarData_(pNull) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: default construct (pNull)" << std::endl;
#endif
    }

    /// generic constructor will accept any type that is valid for the pVar variant (see pVarDataType)
    template <typename T>
    pVar(const T& v): pVarData_(v) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: generic construct to: " << pVarData_.which() << std::endl;
#endif
    }

    // some specializations to avoid ambiguity

    /// construction from char* defaults to a binary strings
    pVar(const char* str): pVarData_(pBString(str)) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: binary string construct" << std::endl;
#endif
    }

    /// construction from int creates pInt
    pVar(int i): pVarData_(pInt(i)) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: int construct: " << i << std::endl;
#endif
    }

    /// construction from float creates pFloat
    pVar(float f): pVarData_(pFloatP(new pFloat(f))) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: float construct: " << i << std::endl;
#endif
    }

    /// construction from double creates pFloat
    pVar(double d): pVarData_(pFloatP(new pFloat(d))) {
#ifdef RPHP_PVAR_DEBUG
        std::cout << "pVar [" << this << "]: double construct: " << i << std::endl;
#endif
    }


    /// convenience function for creating a new empty hash
    void newEmptyHash(void);

    /* default copy constructor */
#ifdef RPHP_PVAR_DEBUG
    pVar(const pVar& v) {
        std::cout << "pVar [" << this << "]: copy construct from [" << &v << "] type: " << v.getType() << std::endl;
        pVarData_ = v.pVarData_;
    }
#endif

    /* default destructor */
#ifdef RPHP_PVAR_DEBUG
    ~pVar(void) {
        std::cout << "pVar [" << this << "]: destruct" << std::endl;
    }
#endif

    /// generic assignment works for any type pVarDataType supports
    template <typename T>
    void operator=(const T& val) {
        PVAR_DATA = val;
    }

    // this specializes on pVarP to ensure we don't create chains of pVars on the heap.
    // it says that we can only assign a pVarP to the outer pVar, not the inner
    void operator=(const pVarP& p) {
        pVarData_ = p;
    }

    // some specializations to avoid ambiguity
    /// default assignment from char* to binary strings
    void operator=(const char* str) {
        PVAR_DATA = pBString(str);
    }

    /// default assignment from int to pInt
    void operator=(int i) {
        PVAR_DATA = pInt(i);
    }

    /// default assignment from float to pFloat
    void operator=(float f) {
        PVAR_DATA = pFloatP(new pFloat(f));
    }

    /// default assignment from float to pFloat
    void operator=(double d) {
        PVAR_DATA = pFloatP(new pFloat(d));
    }

    /// promote this pVar's data to the heap, if it's not already there
    void boxData(void) {
        if (!isBoxed()) {
            pVarData_ = pVarP(new pVar(pVarData_));
        }
    }

    /* custom visitors */
    /// apply a boost static visitor to the variant
    // TODO small optimization here is to not use PVAR_DATA twice since it involves
    // a redundant isBoxed()
    template <typename T>
    typename T::result_type applyVisitor() const {
        return boost::apply_visitor( T(PVAR_DATA), PVAR_DATA );
    }
    template <typename T, typename T2>
    typename T::result_type applyVisitor(T2 param1) const {
        return boost::apply_visitor( T(PVAR_DATA, param1), PVAR_DATA );
    }
    template <typename T, typename T2, typename T3>
    typename T::result_type applyVisitor(T2 param1, T3 param2) const {
        return boost::apply_visitor( T(PVAR_DATA, param1, param2), PVAR_DATA );
    }

    /* type checks */
    /// return the current type represented by this pVar
    pVarType getType() const;

    /// return true if pVar is currently a pNull. no type conversion.
    inline bool isNull() const {
        return (PVAR_DATA.which() == pVarTriStateType_) && pNull(boost::get<pTriState>(PVAR_DATA));
    }
    /// return true if pVar is currently a pBool. no type conversion.
    inline bool isBool() const {
        return (PVAR_DATA.which() == pVarTriStateType_) && !pNull(boost::get<pTriState>(PVAR_DATA));
    }
    /// return true if pVar is currently a pInt. no type conversion.
    inline bool isInt() const {
        return (PVAR_DATA.which() == pVarIntType_);
    }
    /// return true if pVar is currently a pInt. no type conversion.
    inline bool isBigInt() const {
        return (PVAR_DATA.which() == pVarBigIntType_);
    }
    /// return true if pVar is currently a pFloat. no type conversion.
    inline bool isFloat() const {
        return (PVAR_DATA.which() == pVarFloatType_);
    }
    /// return true if pVar is currently a pBString. no type conversion.
    inline bool isBString() const {
        return (PVAR_DATA.which() == pVarBStringType_);
    }
    /// return true if pVar is currently a pUString. no type conversion.
    inline bool isUString() const {
        return (PVAR_DATA.which() == pVarUStringType_);
    }
    /// return true if pVar is currently a pHash. no type conversion.
    inline bool isHash() const {
        return (PVAR_DATA.which() == pVarHashType_);
    }
    /// return true if pVar is currently a pObject. no type conversion.
    inline bool isObject() const {
        return (PVAR_DATA.which() == pVarObjectType_);
    }
    /// return true if pVar is currently a pResource. no type conversion.
    inline bool isResource() const {
        return (PVAR_DATA.which() == pVarResourceType_);
    }
    /// return true if pVar is currently a boxed pVarP. no type conversion.
    // note this is the only one that checks local pVarData_
    inline bool isBoxed() const {
        return (pVarData_.which() == pVarPtrType_);
    }

    /// stream interface
    friend std::ostream& operator << (std::ostream& os, const rphp::pVar& v);

    /** @brief evaluate as boolean

        evaluates according to the language semantics for bool conversion.
        does not mutate.

     */
    bool evalAsBool() const;

    /* in place type conversion */
    /// free current value and represent a pNull instead
    void convertToNull();
    /// convert current value to a boolean. returns converted value.
    pTriState& convertToBool();
    /// convert current value to an integer. return converted value.
    pInt& convertToInt();
    // Converts the current pVar to an pHash. returns the converted value.
    pHashP& convertToHash();
    // float
    /** @brief convert to binary or unicode string

       it may convert to a binary or unicode string, depending on runtime settings
       or it's current value, so you must check which it converted to after the conversion
     */
    void convertToString(const pRuntimeEngine* r);
    /// force convertion to a binary string. if it was a unicode string, the conversion is lossy
    pBString& convertToBString();
    // ustring
    pUStringP& convertToUString();
    // object
    // resource

    /* return type casted copy */
    // null
    // bool
    /// cast to pInt, does not mutate
    pInt copyAsInt() const;
    // float
    /// cast to pBString, does not mutate
    pBString copyAsBString() const;
    /// cast to string (binary or unicode), does not mutate
    pVar copyAsString(const pRuntimeEngine* r) const;
    pHashP copyAsHash() const;
    // ustring
    // object
    // resource

    /* these do no conversions, and throw exception if the wrong type is accessed */
    /// bool accessor. throws exception if pVar is wrong type
    pTriState& getBool() {
        return boost::get<pTriState&>(PVAR_DATA);
    }

    /// const bool accessor. throws exception if pVar is wrong type
    const pTriState& getBool() const {
        return boost::get<const pTriState&>(PVAR_DATA);
    }

    /// pInt accessor. throws exception if pVar is wrong type
    pInt& getInt() {
        return boost::get<pInt&>(PVAR_DATA);
    }

    /// const pInt accessor. throws exception if pVar is wrong type
    const pInt& getInt() const {
        return boost::get<const pInt&>(PVAR_DATA);
    }

    /// pBigInt accessor. throws exception if pVar is wrong type
    pBigIntP& getBigIntP() {
        return boost::get<pBigIntP&>(PVAR_DATA);
    }

    /// pBigInt accessor. throws exception if pVar is wrong type
    const pBigIntP& getBigIntP() const {
        return boost::get<const pBigIntP&>(PVAR_DATA);
    }

    /// pFloat accessor. throws exception if pVar is wrong type
    pFloatP& getFloatP() {
        return boost::get<pFloatP&>(PVAR_DATA);
    }

    /// const pFloat accessor. throws exception if pVar is wrong type
    const pFloatP& getFloatP() const {
        return boost::get<const pFloatP&>(PVAR_DATA);
    }

    /// pBString accessor. throws exception if pVar is wrong type
    pBString& getBString() {
        return boost::get<pBString&>(PVAR_DATA);
    }

    /// const pBString accessor. throws exception if pVar is wrong type
    const pBString& getBString() const {
        return boost::get<const pBString&>(PVAR_DATA);
    }

    /// pUString accessor. throws exception if pVar is wrong type
    pUStringP& getUStringP() {
        return boost::get<pUStringP&>(PVAR_DATA);
    }

    /// const pUString accessor. throws exception if pVar is wrong type
    const pUStringP& getUStringP() const {
        return boost::get<const pUStringP&>(PVAR_DATA);
    }

    /// pHash accessor. throws exception if pVar is wrong type
    pHashP& getHashP() {
        return boost::get<pHashP&>(PVAR_DATA);
    }

    /// const pHash accessor. throws exception if pVar is wrong type
    const pHashP& getHashP() const {
        return boost::get<const pHashP&>(PVAR_DATA);
    }

    /// read only hash access
    //const pHashP& getConstHash() const {
    //    return boost::get<const pHashP&>(pVarData_);
    //}

    /// pObject accessor. throws exception if pVar is wrong type
    pObjectP& getObjectP() {
        return boost::get<pObjectP&>(PVAR_DATA);
    }

    /// const pObject accessor. throws exception if pVar is wrong type
    const pObjectP& getObjectP() const {
        return boost::get<const pObjectP&>(PVAR_DATA);
    }

    /// pResource accessor. throws exception if pVar is wrong type
    pResourceP& getResourceP() {
        return boost::get<pResourceP&>(PVAR_DATA);
    }

    /// const pResource accessor. throws exception if pVar is wrong type
    const pResourceP& getResourceP() const {
        return boost::get<const pResourceP&>(PVAR_DATA);
    }

    /// boxed pVar accessor. throws exception if pVar is wrong type
    /*
    // note: you don't want this, you want the const version below.
    pVarP& getPtr() {
        return boost::get<pVarP&>(pVarData_);
    }
    */

    /**
      @brief boxed pVar accessor (const). throws exception if pVar is wrong type
      note that this returns a const pVarP: this means the pointer itself can't
      change, but the pVar data it points to can.
     */
    const pVarP& getPtr() const {
        return boost::get<const pVarP&>(pVarData_);
    }

    /// operators
    pVar& operator+=(const pVar& rhs);
    pVar& operator-=(const pVar& rhs);
    pVar& operator*=(const pVar& rhs);
    pVar& operator/=(const pVar& rhs);

    const pVar operator+(const pVar& v) const;
    const pVar operator-(const pVar& v) const;
    const pVar operator*(const pVar& v) const;
    const pVar operator/(const pVar& v) const;

};

} /* namespace rphp */

#endif /* RPHP_PVAR_H_ */
