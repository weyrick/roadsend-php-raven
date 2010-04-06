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

#include <gmpxx.h>

#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <boost/tuple/tuple.hpp>

#include <unicode/ucnv.h>

#include <string>

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/Twine.h>

namespace rphp {

/// string ref, see llvm::StringRef
typedef llvm::StringRef pStringRef;

/// Twine for fast string concats, see llvm::Twine
typedef llvm::Twine pTwine;

/// signed (fast) integer type (used in pVar)
typedef signed long pInt;
#define RPHP_INT_MIN LONG_MIN
#define RPHP_INT_MAX LONG_MAX

/// signed (arbitrary precision) integer type (used in pVar)
typedef mpz_class pBigInt;

/// float type (used in pVar). GMP float
typedef mpf_class pFloat;

/// unsigned integer type (not used in pVar)
typedef boost::uint_fast32_t pUInt;

/// string type used for identifiers (classes, functions, variable names)
typedef std::string pIdentString;

/// string type used for filenames
typedef std::string pFileNameString;

/// string type used for warning/notice messages
typedef std::string pMsgString;

// notifier emit function callback
typedef boost::function<void (pUInt level, pMsgString msg)> pNotifyEmitFun;

// lightweight character encoding wrapper
class pEncoding {

    std::string value_;

public:
    pEncoding(const pStringRef& e):value_(e) { }

    const std::string& value(void) const { return value_; }

    // this matches on aliases, e.g. utf8 == UTF8 and iso-8859-1==latin1
    bool matches(const pStringRef& e) const {
        return (ucnv_compareNames(value_.c_str(), e.data()) == 0);
    }

    bool is88591OrUTF8() const {
        return (matches("ISO-8859-1") || matches("UTF-8"));
    }

    // native format stored by libicu
    bool isNativeICU() const {
        return (matches("UTF-16"));
    }

    bool operator==(const pStringRef& e) const {
        return matches(e);
    }

    bool operator!=(const pStringRef& e) const {
        return !matches(e);
    }

};


// notifier log levels
#define E_ERROR             0x00000001
#define E_WARNING           0x00000002
#define E_PARSE             0x00000004
#define E_NOTICE            0x00000008
#define E_CORE_ERROR        0x00000010
#define E_CORE_WARNING      0x00000020
#define E_COMPILE_ERROR     0x00000040
#define E_COMPILE_WARNING   0x00000080
#define E_USER_ERROR        0x00000100
#define E_USER_WARNING      0x00000200
#define E_USER_NOTICE       0x00000400
#define E_STRICT            0x00000800
#define E_RECOVERABLE_ERROR 0x00001000
#define E_DEPRECATED        0x00002000
#define E_USER_DEPRECATED   0x00004000
// rphp: not catchable in php land
#define E_COMPILE_DEBUG     0x01000000
#define E_RUNTIME_DEBUG     0x02000000
#define E_ALL               0xffffffff

} /* namespace rphp */


#endif /* RPHP_PTYPES_H_ */
