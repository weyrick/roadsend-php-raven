/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009-2010 Shannon Weyrick <weyrick@roadsend.com>
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

#include "rphp/analysis/pAST.h"

namespace rphp { namespace AST {

void stmt::destroyChildren(pParseContext &C) {
    for (child_iterator i = child_begin(), e = child_end(); i != e; ) {
        // NOTE: it's valid for some children to be NULL
        if (stmt* child = *i++)
            child->destroy(C);
    }
}

void stmt::doDestroy(pParseContext &C) {
  destroyChildren(C);
  this->~stmt();
  C.deallocate((void *)this);

}

const pUInt memberFlags::PUBLIC    = 1;
const pUInt memberFlags::PROTECTED = 2;
const pUInt memberFlags::PRIVATE   = 4;
const pUInt memberFlags::STATIC    = 8;
const pUInt memberFlags::ABSTRACT  = 16;
const pUInt memberFlags::FINAL     = 32;
const pUInt memberFlags::CONST     = 64;

} } // namespace
