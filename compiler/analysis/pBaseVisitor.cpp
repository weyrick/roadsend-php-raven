/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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

#include "rphp/analysis/pBaseVisitor.h"
#include "rphp/analysis/pSourceModule.h"


#include <iostream>
#include <unicode/ustream.h>

namespace rphp { namespace AST {

pBaseVisitor::dispatchFunction pBaseVisitor::preDispatchTable_[] = {

#define STMT(CLASS, PARENT) reinterpret_cast<dispatchFunction>( &pBaseVisitor::visit_pre_##CLASS ),
#include "rphp/analysis/astNodes.def"

};

pBaseVisitor::dispatchFunction pBaseVisitor::postDispatchTable_[] = {

#define STMT(CLASS, PARENT) reinterpret_cast<dispatchFunction>( &pBaseVisitor::visit_post_##CLASS ),
#include "rphp/analysis/astNodes.def"

};

pBaseVisitor::childDispatchFunction pBaseVisitor::childrenDispatchTable_[] = {

#define STMT(CLASS, PARENT) reinterpret_cast<childDispatchFunction>( &pBaseVisitor::visit_children_##CLASS ),
#include "rphp/analysis/astNodes.def"

};

void pBaseVisitor::run(void) {
    module_->applyVisitor(this);
}

void pBaseVisitor::visit(stmt* s) {

    // PRE
    visit_pre_stmt(s);
    if (expr* n = dyn_cast<expr>(s))
        visit_pre_expr(n);

    (this->*preDispatchTable_[s->kind()])(s);

    // CHILDREN
    // we always try the custom first, and fall back to the standard unless
    // the custom returns true
    if ((this->*childrenDispatchTable_[s->kind()])(s) == false)
        visitChildren(s);

    // POST
    (this->*postDispatchTable_[s->kind()])(s);

    if (expr* n = dyn_cast<expr>(s))
        visit_post_expr(n);
    visit_post_stmt(s);

}

void pBaseVisitor::visitChildren(stmt* s) {

    stmt* child(0);
    for (stmt::child_iterator i = s->child_begin(), e = s->child_end(); i != e; ) {
      if ( (child = *i++) ) {
          visit(child);
      }
    }

}

void pBaseVisitor::visitChildren(stmt::child_iterator begin, stmt::child_iterator end) {

    stmt* child(0);
    for (stmt::child_iterator i = begin, e = end; i != e; ) {
      if ( (child = *i++) ) {
          visit(child);
      }
    }

}

} } // namespace

