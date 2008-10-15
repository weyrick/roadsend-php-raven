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

#include <iostream>
#include "pASTVisitors.h"

namespace rphp { namespace AST {

void baseVisitor::visit(stmt* s) {
    // TODO: implement the dispatch better
    assert(s != NULL && "statement to visit is NULL");
    switch (s->getKind()) {
    case echoStmtKind:
        visit_echoStmt(static_cast<echoStmt*>(s));
        break;
    case inlineHtmlKind:
        visit_inlineHtml(static_cast<inlineHtml*>(s));
        break;
    case literalBStringKind:
        visit_literalBString(static_cast<literalBString*>(s));
        break;
    case literalIntKind:
        visit_literalInt(static_cast<literalInt*>(s));
        break;
    }
}


void defaultVisitor::visit_echoStmt(echoStmt* n) {
    visit(n->getRVal());
}

void dumpVisitor::visit_echoStmt(echoStmt* n) {
    std::cout << "echoStmt: ";
    defaultVisitor::visit_echoStmt(n);
}

void dumpVisitor::visit_inlineHtml(inlineHtml* n)  {
    std::cout << "inline HTML: " << n->getVal() << std::endl;
}

void dumpVisitor::visit_literalBString(literalBString* n)  {
    std::cout << "literal bstring: " << n->getVal() << std::endl;
}

void dumpVisitor::visit_literalInt(literalInt* n)  {
    std::cout << "literal int: " << n->getVal() << std::endl;
}

} } // namespace

