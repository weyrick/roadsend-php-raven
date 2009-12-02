/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
   ***** END LICENSE BLOCK *****
*/

#ifndef RPHP_PSOURCETYPES_H_
#define RPHP_PSOURCETYPES_H_

#include "pTypes.h"

#include <string>
#include <boost/range/iterator_range.hpp>

namespace rphp {

// string and iterator types used to expose the
// underlying source input streams
typedef std::string pSourceString;
typedef std::string::const_iterator pSourceCharIterator;

// iterator pair used as value for matched tokens
typedef boost::iterator_range<pSourceCharIterator> pSourceRange;

/// source locations: filename/linenum
typedef boost::tuple<const pFileNameString, const pUInt> pSourceLocation;

/// source locations: filename/startlinenum/endlinenum
typedef boost::tuple<const pFileNameString, const pUInt, const pUInt> pSourceStartEndLocation;

/// source file description: filename/encoding
class pSourceFileDesc {

    pFileNameString fileName_;
    pEncoding encoding_;

public:
    pSourceFileDesc(const pStringRef& n, const pStringRef& e):
    fileName_(n),encoding_(e) { }

    const pFileNameString& fileName(void) const { return fileName_; }
    const pEncoding& encoding(void) const { return encoding_; }

};


} // namespace

#endif /* RPHP_PSOURCETYPES_H_ */
