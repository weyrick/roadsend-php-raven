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

#include <boost/logic/tribool.hpp>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <iostream>

namespace rphp {

    /*
     * Definition of the various types used in the rphp runtime, including the
     * main pVar variant
     */

    // a boost::tribool represents php true, false and null values
    // pTrue, pFalse and pNull are defined
    typedef boost::logic::tribool pTriState;
    // convenience accesors
    BOOST_TRIBOOL_THIRD_STATE(pNull)
    #define pTrue  pTriState(true)
    #define pFalse pTriState(false)

    // numeric types
    typedef long   pInt;
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

    // base variant that represents a php variable
    typedef boost::variant< pTriState, pInt, pFloat, pBString, pUStringP, pHashP, pObjectP > pVarBase;

    // reference to a pvarBase, i.e. a container for pvar variables
    // shared_ptr maintains a reference count and guarantees proper destruction
    typedef boost::shared_ptr<pVarBase> pVarRef;

    // full pvar definition: a variant that can hold a base type or a reference
    typedef boost::variant< pTriState, pInt, pFloat, pBString, pUStringP, pHashP, pObjectP, pVarRef > pVar;
    typedef boost::shared_ptr<pVar> pVarP;

    // associated enum for checking type
    typedef enum {
        pVarNullType,
        pVarBoolType,
        pVarIntType,
        pVarFloatType,
        pVarBStringType,
        pVarUStringType,
        pVarHashType,
        pVarObjectType,
        pVarRefType
    } pVarType;

} /* namespace rphp */

#endif /* RPHP_PVAR_H_ */
