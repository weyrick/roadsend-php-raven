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

#include "boost/variant.hpp"
#include "boost/lexical_cast.hpp"
#include "unicode/unistr.h"
#include "rphp_hash.h"
#include "rphp_object.h"
#include <iostream>

namespace rphp {

// this represents a php bool and null value
// it is stored in the variant as an int type
enum p3state
{
	Null,
	False,
	True
};

// string types: binary and unicode flavor
typedef std::string bstring;
typedef UnicodeString ustring;

// a variant that represents a php variable
typedef boost::variant< p3state/*int*/, long, double, bstring, ustring, php_hash, php_object> pvar;

// associated enum for checking type
typedef enum {
	PVAR_NULL,     // p3state
	PVAR_BOOL,     // p3state
	PVAR_LONG,     // long
	PVAR_DOUBLE,   // double
	PVAR_BSTRING,  // bstring
	PVAR_USTRING,  // ustring
	PVAR_HASH,     // rphp::hash
	PVAR_OBJ       // rphp::object
} pvarType;

} /* namespace rphp */


#endif /* RPHP_PVAR_H_ */
