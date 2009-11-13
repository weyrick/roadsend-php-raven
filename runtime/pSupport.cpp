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

#include "rphp/runtime/pSupport.h"

#include <boost/algorithm/string/case_conv.hpp>

namespace rphp {

//   This defines a boost compatible hash function for pUString
std::size_t hash_value(const rphp::pUString &v) {
    return v.hashCode();
}

// std
pBString toLowerCopy(const pBString& v) {
    return boost::algorithm::to_lower_copy(v);
}

// unicode
pUString toLowerCopy(const pUString& v) {
    //return v.copyAsICUString().toLower();
    pUString str(v);
    return str.toLower();
}

char* convertCodepage(const pUString& s, const char* codepage) {

    // we're going from utf-16 to a byte array, thus l*2
    int32_t newlen(0), len(s.length()*2);

    char* uBuf = new char[len];

    newlen = s.extract(0, len, uBuf, codepage);

    if (newlen != len) {
        // need resize?
        delete uBuf;
        uBuf = new char[newlen];
        s.extract(0, newlen, uBuf, codepage);
    }

    return uBuf;

}

}

