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
    case literalBoolKind:
        visit_literalBool(static_cast<literalBool*>(s));
        break;
    case literalNullKind:
        visit_literalNull(static_cast<literalNull*>(s));
        break;
    }
}


void defaultVisitor::visit_echoStmt(echoStmt* n) {
    visit(n->getRVal());
}

// ** DUMP VISITOR **

void dumpVisitor::showindent() {
    if (indentLevel)
        std::cout << std::string(" ", indentLevel);
}

void dumpVisitor::visit_echoStmt(echoStmt* n) {
    showindent();
    std::cout << "(echoStmt:" << std::endl;
    indent();
    defaultVisitor::visit_echoStmt(n);
    unindent();
    showindent();
    std::cout << ")" << std::endl;
}

void dumpVisitor::visit_inlineHtml(inlineHtml* n)  {
    showindent();
    std::cout << "inline HTML: " << n->getStringVal() << std::endl;
}

void dumpVisitor::visit_literalBString(literalBString* n)  {
    showindent();
    std::cout << "literal bstring: \"" << n->getStringVal() << "\"" << std::endl;
}

void dumpVisitor::visit_literalInt(literalInt* n)  {
    showindent();
    std::cout << "literal int: " << n->getStringVal() << std::endl;
}

void dumpVisitor::visit_literalBool(literalBool* n)  {
    showindent();
    std::cout << "literal bool: ";
    if (n->getBoolVal())
        std::cout << "TRUE";
    else
        std::cout << "FALSE";
    std::cout << std::endl;
}

void dumpVisitor::visit_literalNull(literalNull* n)  {
    showindent();
    std::cout << "literal NULL" << std::endl;
}

} } // namespace

