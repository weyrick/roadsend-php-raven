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

#ifndef RPHP_PTYPES_H_
#define RPHP_PTYPES_H_

// llvm JIT doesn't do inline asm, which is used by the atomic locking mechanism
// in shared_ptr. this forces generic pthreads version instead to avoid it for now.
//#define BOOST_SP_USE_PTHREADS

#include "rphp/runtime/CowPtr.h"
#include "rphp/runtime/pUString.h"
#include "rphp/runtime/pRuntimeError.h"

#include <boost/cstdint.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>

#include <vector>

namespace rphp {

/// a boost::tribool represents php true, false and null values
/// pTrue, pFalse and pNull are defined
typedef boost::logic::tribool pTriState;

// convenience accesors
BOOST_TRIBOOL_THIRD_STATE(pNull)
#define pTrue  pTriState(true)
#define pFalse pTriState(false)

/// runtime integer type
typedef signed long pInt;
/// runtime float type
typedef double pFloat;

/// runtime "binary" string type
/// these are simple byte-wide character arrays
typedef std::string pBString;

// see pUString.h for unicode string

class pHash;
/// copy-on-write auto shared hash table type
typedef CowPtr<pHash> pHashP;

class pObject;
/// auto shared pointer to an instantiated runtime object
typedef boost::shared_ptr<pObject> pObjectP;

class pResource;
/// auto shared pointer to an runtime resource
typedef boost::shared_ptr<pResource> pResourceP;

class pVar;
/// auto shared pointer to a pVar
/// used for a pVar that needs to live on the heap,
/// in particular: reference variables
typedef boost::intrusive_ptr<pVar> pVarP;

/// main pVar variant type
typedef boost::variant< pTriState,
                        pInt,
                        pFloat,
                        pBString,
                        pUStringP,
                        pHashP,
                        pObjectP,
                        pResourceP,
                        pVarP
                        > pVarDataType;

/// these should match the order of the actual types listed in the variant type
/// it is only used by the pVar type checker, which returns pVarType
/// this is only because pVarNullType and pVarBoolType are both handled
/// by pTriState
typedef enum {
    pVarTriStateType_ = 0,
    pVarIntType_      = 1,
    pVarFloatType_    = 2,
    pVarBStringType_  = 3,
    pVarUStringType_  = 4,
    pVarHashType_     = 5,
    pVarObjectType_   = 6,
    pVarResourceType_ = 7,
    pVarPtrType_      = 8
} pVarWhichType_;

/// an enum for determining the type of data stored in a pVar
typedef enum {
    pVarNullType,
    pVarBoolType,
    pVarIntType,
    pVarFloatType,
    pVarBStringType,
    pVarUStringType,
    pVarHashType,
    pVarObjectType,
    pVarResourceType,
    pVarPtrType
} pVarType;

/// note, pUInt is not a base PHP type (all PHP numbers are signed)
typedef boost::uint_fast32_t pUInt;

/// string type used for identifiers (classes, functions, variable names)
typedef std::string pIdentString;

/// string type used for filenames
typedef std::string pFileNameString;

/// source file description: filename/encoding
typedef boost::tuple<const pFileNameString, const std::string> pSourceFileDesc;

/// source locations: filename/linenum
typedef boost::tuple<const pFileNameString, const pUInt> pSourceLocation;

/// source locations: filename/startlinenum/endlinenum
typedef boost::tuple<const pFileNameString, const pUInt, const pUInt> pSourceStartEndLocation;

/// php function signature: no arguments
class pRuntimeEngine;

#define RPHP_STDFUNC_ARGS   pVar* retVal, pRuntimeEngine* runtime
#define RPHP_STDMETHOD_ARGS pVar* retVal, pRuntimeEngine* runtime, pObjectP pThis

/// php function signature: no arguments
typedef void (*pFunPointer0)(RPHP_STDFUNC_ARGS);
/// php function signature: one argument
typedef void (*pFunPointer1)(RPHP_STDFUNC_ARGS, pVar);
/// php function signature: two arguments
typedef void (*pFunPointer2)(RPHP_STDFUNC_ARGS, pVar, pVar);
/// php function signature: three arguments
typedef void (*pFunPointer3)(RPHP_STDFUNC_ARGS, pVar, pVar, pVar);
/// php function signature: four arguments
typedef void (*pFunPointer4)(RPHP_STDFUNC_ARGS, pVar, pVar, pVar, pVar);
/// php function signature: five arguments
typedef void (*pFunPointer5)(RPHP_STDFUNC_ARGS, pVar, pVar, pVar, pVar, pVar);
/// php function signature: n arguments
typedef void (*pFunPointerN)(RPHP_STDFUNC_ARGS, std::vector<pVar>);

/// php method signature: no arguments
typedef void (*pMethodPointer0)(RPHP_STDMETHOD_ARGS);
/// php method signature: one argument
typedef void (*pMethodPointer1)(RPHP_STDMETHOD_ARGS, pVar);
/// php method signature: two arguments
typedef void (*pMethodPointer2)(RPHP_STDMETHOD_ARGS, pVar, pVar);
/// php method signature: three arguments
typedef void (*pMethodPointer3)(RPHP_STDMETHOD_ARGS, pVar, pVar, pVar);
/// php method signature: four arguments
typedef void (*pMethodPointer4)(RPHP_STDMETHOD_ARGS, pVar, pVar, pVar, pVar);
/// php method signature: five arguments
typedef void (*pMethodPointer5)(RPHP_STDMETHOD_ARGS, pVar, pVar, pVar, pVar, pVar);
/// php method signature: n arguments
typedef void (*pMethodPointerN)(RPHP_STDMETHOD_ARGS, std::vector<pVar>);


// platform specific
#define RPHP_NEWLINE     '\n'
#define RPHP_WNEWLINE    L'\n'

} /* namespace rphp */


#endif /* RPHP_PTYPES_H_ */
