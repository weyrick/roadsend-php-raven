/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009 Shannon Weyrick <weyrick@roadsend.com>
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

#include "rphp/analysis/pBaseTransformer.h"

#include <iostream>
#include <unicode/ustream.h>

namespace rphp { namespace AST {

pBaseTransformer::dispatchFunction pBaseTransformer::preDispatchTable_[] = {

#define STMT(CLASS, PARENT) reinterpret_cast<dispatchFunction>( &pBaseTransformer::visit_pre_##CLASS ),
#include "rphp/analysis/astNodes.def"

};

pBaseTransformer::dispatchFunction pBaseTransformer::postDispatchTable_[] = {

#define STMT(CLASS, PARENT) reinterpret_cast<dispatchFunction>( &pBaseTransformer::visit_post_##CLASS ),
#include "rphp/analysis/astNodes.def"

};

pBaseTransformer::childDispatchFunction pBaseTransformer::childrenDispatchTable_[] = {

#define STMT(CLASS, PARENT) reinterpret_cast<childDispatchFunction>( &pBaseTransformer::visit_children_##CLASS ),
#include "rphp/analysis/astNodes.def"

};

const char* pBaseTransformer::nodeDescTable_[] = {
#define STMT(CLASS, PARENT) #CLASS,
#include "rphp/analysis/astNodes.def"
};

void pBaseTransformer::visit(stmt* s) {

    // PRE
    visit_pre_stmt(s);
    if (expr* n = dyn_cast<expr>(s))
        visit_pre_expr(n);

    (this->*preDispatchTable_[s->getKind()])(s);

    // CHILDREN
    // we always try the custom first, and fall back to the standard unless
    // the custom returns true
    if ((this->*childrenDispatchTable_[s->getKind()])(s) == false)
        visitChildren(s);

    // POST
    (this->*postDispatchTable_[s->getKind()])(s);

    if (expr* n = dyn_cast<expr>(s))
        visit_post_expr(n);
    visit_post_stmt(s);

}

void pBaseTransformer::visitChildren(stmt* s) {

    stmt* child(0);
    for (stmt::child_iterator i = s->child_begin(), e = s->child_end(); i != e; ) {
      if ( (child = *i++) ) {
          visit(child);
      }
    }

}

} } // namespace

