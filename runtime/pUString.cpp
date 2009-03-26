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

/*

ARCHIVE: not current used, but kept around for now

#include "rphp/runtime/pUString.h"
#include <iostream>

namespace rphp {

std::size_t pUString::hashCode(void) const {
    // note: this is from source/common/uhash.c in ICU distro
    // there is no public API for it. why?
    // it is the same hash function used by UnicodeString
    int32_t hash = 0;                         
    const UChar *p = (const UChar*) data_.get();
    if (p != NULL) {
        int32_t len = (int32_t)(strLen_);
        int32_t inc = ((len - 32) / 32) + 1;
        const UChar *limit = p + len;
        while (p<limit) {
            hash = (hash * 37) + *p;
            p += inc;
        }
    }
    return hash;
}

UnicodeString pUString::copyAsICUString(void) const {
    // copies buffer
    return UnicodeString(data_.get(), strLen_);
}


// a const version that uses our buffer
const UnicodeString pUString::readonlyICUString(void) const {
    return UnicodeString(data_.get(), strLen_, bufSize_);
}

UnicodeString pUString::checkoutICUString(void) {
    // this UnicodeString will use our buffer for write operations!
    detach();
    return UnicodeString(data_.get(), strLen_, bufSize_);
}

void pUString::checkinICUString(const UnicodeString& s) {

    UErrorCode errorCode(U_ZERO_ERROR);

    int32_t newLen = s.extract(data_.get(), bufSize_, errorCode);
    if (errorCode == U_BUFFER_OVERFLOW_ERROR) {
        // data was modified and doesn't fit in our buffer anymore
        // make room and try again with newLen
        errorCode = U_ZERO_ERROR;
        data_.reset(new UChar[newLen]);
        bufSize_ = newLen;
        strLen_ = newLen;
        s.extract(data_.get(), bufSize_, errorCode);
        if (!U_SUCCESS(errorCode)) {
            throw std::exception();
        }
    }
    else {
        // either the string was not resized (buffers are still the same),
        // or is smaller and so fit within our buffer (copied in). i.e. newlen <= bufSize_
        // in either case, just update our size
        strLen_ = newLen;
    }
}

bool pUString::operator== (const pUString& str) const {
    //return (u_strcmp(data_.get(), str.getBuffer()) == 0);
    return this->readonlyICUString() == str.readonlyICUString();
}
bool pUString::operator!= (const pUString& str) const {
    //return (u_strcmp(data_.get(), str.getBuffer()) != 0);
    return this->readonlyICUString() != str.readonlyICUString();
}

}
*/

