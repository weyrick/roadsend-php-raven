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

#ifndef RPHP_PCOMPILERTYPES_H_
#define RPHP_PCOMPILERTYPES_H_

#include <string>
#include <boost/range/iterator_range.hpp>

// import runtime types
#include "pTypes.h"

namespace rphp {

// string and iterator types used to expose the
// underlying source input streams
typedef std::wstring pSourceString;
typedef std::wstring::iterator pSourceCharIterator;

// iterator pair used as value for matched tokens
typedef boost::iterator_range<pSourceCharIterator> pSourceRange;

} // namespace

#endif /* RPHP_PCOMPILERTYPES_H_ */
