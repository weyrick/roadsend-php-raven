/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2010 Cornelius Riemenschneider <c.r1@gmx.de>
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

#ifndef RPHP_CHECKMEMORYMANAGEMENT_H
#define RPHP_CHECKMEMORYMANAGEMENT_H

#include "rphp/analysis/pAST.h"
#include "rphp/analysis/pBaseVisitor.h"

namespace rphp { namespace AST { namespace Pass {

class CheckMemoryManagement: public pBaseVisitor {

public:
    CheckMemoryManagement(pSourceModule* m):
            pBaseVisitor("Check Memory Management","Checks the AST for correct refCounts.", m)
            { }

    void visit_pre_stmt(stmt* );

};

} } } // namespace

#endif /* RPHP_CHECKMEMORYMANAGEMENT_H */
