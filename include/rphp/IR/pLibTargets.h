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

#ifndef RPHP_PLIBTARGETS_H_
#define RPHP_PLIBTARGETS_H_

#include "pLinkTarget.h"

namespace rphp {

// generate a library (static or dynamic)
// this means a standard .so, .a, .dll, etc
class pLibTarget : public pLinkTarget {

};


// generate a fastcgi binary
class pFastCGITarget : public pLibTarget {

};

// generate a microserver binary
class pMicroServerTarget : public pLibTarget {

};

} // namespace

#endif /* RPHP_PLIBTARGETS_H_ */
