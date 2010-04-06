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

#ifndef RPHP_PSUPPORT_H_
#define RPHP_PSUPPORT_H_

#include "rphp/runtime/pRuntimeTypes.h"

#include <boost/foreach.hpp>

#define foreach         BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

namespace rphp {

// boost compatible hash function for pUString
std::size_t hash_value(const pUString &v);

// case conversion
pBString toLowerCopy(const pBString& v);
pUString toLowerCopy(const pUString& v);

// codepage conversion. convert unicode string in s to arbitrary codepage.
// this will allocate using new[], and returns the allocated buffer which
// the caller is responsible for deleteing
char* convertCodepage(const pUString& s, const char* codepage);
pBigInt convertStringLiteralToBigInt(pSourceString& s);


}

#endif /* RPHP_PSUPPORT_H_ */
