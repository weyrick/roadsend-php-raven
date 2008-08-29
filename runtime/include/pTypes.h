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

#ifndef RPHP_PTYPES_H_
#define RPHP_PTYPES_H_

#include <boost/logic/tribool.hpp>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>

#include <unicode/unistr.h>

namespace rphp {

// a boost::tribool represents php true, false and null values
// pTrue, pFalse and pNull are defined
typedef boost::logic::tribool pTriState;
// convenience accesors
BOOST_TRIBOOL_THIRD_STATE(pNull)
#define pTrue  pTriState(true)
#define pFalse pTriState(false)

// numeric types
typedef signed long pInt;
typedef double pFloat;

// string types: binary and unicode flavor
// "binary" strings
typedef std::string pBString;

// unicode strings, using the ICU library
typedef UnicodeString pUString;
typedef boost::shared_ptr<pUString> pUStringP;

// php hash tables
class pHash;
typedef boost::shared_ptr<pHash> pHashP;

// php objects
class pObject;
typedef boost::shared_ptr<pObject> pObjectP;

// php resources
class pResource;
typedef boost::shared_ptr<pResource> pResourceP;

// a shared container for pvar variables: php reference variables
class pVar;
typedef boost::shared_ptr<pVar> pVarP;

// main pVar variant type
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

// these should match the order of the actual types listed in the variant type
// it is only used by the pVar type checker, which returns pVarType
// this is only because pVarNullType and pVarBoolType are both handled
// by pTriState
typedef enum {
    pVarTriStateType_ = 0,
    pVarIntType_      = 1,
    pVarFloatType_    = 2,
    pVarBStringType_  = 3,
    pVarUStringType_  = 4,
    pVarHashType_     = 5,
    pVarObjectType_   = 6,
    pVarResourceType_ = 7,
    pVarRefType_      = 8
} pVarWhichType_;

// this should be used by user code
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
    pVarRefType
} pVarType;
    
// note, pUInt is not a base PHP type (all PHP numbers are signed)
typedef unsigned long pUInt;

// source locations: filename/linenum
typedef boost::tuple<const pUString, const pUInt> pSourceLocation;

// source locations: filename/startlinenum/endlinenum
typedef boost::tuple<const pUString, const pUInt, const pUInt> pSourceStartEndLocation;

// php function signatures
typedef boost::function<pVar (pVar)> pFunPointer1;
    
} /* namespace rphp */


#endif /* RPHP_PTYPES_H_ */
