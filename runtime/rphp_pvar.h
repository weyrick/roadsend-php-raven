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
#include <unicode/ustream.h> // ostream API for UnicodeString

#include <iostream>

BOOST_TRIBOOL_THIRD_STATE(pNull)

namespace rphp {

// a boost::tribool represents php true, false and null values
typedef boost::logic::tribool p3state;

// pNull is defined above
#define pTrue rphp::p3state(true)
#define pFalse rphp::p3state(false)

// pvar numbers
typedef long pint;
typedef double pfloat;

// string types: binary and unicode flavor

// STL basic string.
// GNU stdc++ provides implicit sharing but this isn't part of the standard
typedef std::string bstring;

// UnicodeString uses 2 byte characters. Storage of base class is 32 bytes on 32bit, 40 on 64bit
// implicitly shared
typedef UnicodeString ustring;

// forward declarations for phash and pobject
class phash;
//class pobject;

// base variant that represents a php variable
typedef boost::variant< p3state/*int*/, pint/*long*/, pfloat/*double*/, bstring, ustring, phash/*, pobject*/> pvarBase;

// reference to a pvarBase, i.e. a container for pvar variables
// shared_ptr maintains a reference count and guarantees proper destruction
typedef boost::shared_ptr<pvarBase> pvarRef;

// full pvar definition: a variant that can hold a base type or a reference
typedef boost::variant< p3state/*int*/, pint/*long*/, pfloat/*double*/, bstring, ustring, phash, /*pobject,*/ pvarRef> pvar;

// associated enum for checking type
typedef enum {
    PVAR_NULL,     // rphp::p3state
    PVAR_BOOL,     // rphp::p3state
    PVAR_INT,      // rphp::pint
    PVAR_FLOAT,    // rphp::pfloat
    PVAR_BSTRING,  // rphp::bstring
    PVAR_USTRING,  // rphp::ustring
    PVAR_HASH,     // rphp::phash
    PVAR_OBJ,      // rphp::pobject
    PVAR_REF	   // rphp::pvarRef
} pvarType;

} /* namespace rphp */

#endif /* RPHP_PVAR_H_ */
