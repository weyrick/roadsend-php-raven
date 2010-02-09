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

#include "rphp/analysis/passes/CheckMemoryManagement.h"
#include "rphp/analysis/passes/DumpAST.h"
#include "rphp/analysis/pSourceModule.h"

#include <iostream>

namespace rphp { namespace AST { namespace Pass {


void CheckMemoryManagement::visit_pre_stmt(stmt* s) {
    if(s && s->refCount_ != 1) {
        std::cerr << "The following AST Node has a refCount of " << s->refCount_ << std::endl; 
        DumpAST dump(module_);
        dump.pre_run();
        dump.visit(s);
        dump.post_run();
    }
}


} } } // namespace

