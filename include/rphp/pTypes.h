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

#ifndef RPHP_PTYPES_H_
#define RPHP_PTYPES_H_

// llvm JIT doesn't do inline asm, which is used by the atomic locking mechanism
// in shared_ptr. this forces generic pthreads version instead to avoid it for now.
#define BOOST_SP_USE_PTHREADS

#include <boost/cstdint.hpp>
#include <boost/tuple/tuple.hpp>

#include <string>

namespace rphp {

/// signed integer type (used in pVar)
typedef signed long pInt;

/// float type (used in pVar)
typedef double pFloat;

/// unsigned integer type (not used in pVar)
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

} /* namespace rphp */


#endif /* RPHP_PTYPES_H_ */
