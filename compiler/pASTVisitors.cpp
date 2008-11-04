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

baseVisitor::dispatchFunType baseVisitor::dispatchTable_[] = {
    // NOTE: requires same order as nodeKind enum in pAST.h
    reinterpret_cast<dispatchFunType>(&baseVisitor::visit_echoStmt),
    reinterpret_cast<dispatchFunType>(&baseVisitor::visit_inlineHtml),
    reinterpret_cast<dispatchFunType>(&baseVisitor::visit_literalString),
    reinterpret_cast<dispatchFunType>(&baseVisitor::visit_literalInt),
    reinterpret_cast<dispatchFunType>(&baseVisitor::visit_literalFloat),
    reinterpret_cast<dispatchFunType>(&baseVisitor::visit_literalNull),
    reinterpret_cast<dispatchFunType>(&baseVisitor::visit_literalBool)
};


void baseVisitor::visit(stmt* s) {

    if (s)
      (this->*dispatchTable_[s->getKind()])(s);

}


void defaultVisitor::visit_echoStmt(echoStmt* n) {
    visit(n->getRVal());
}

// ** DUMP VISITOR **

void dumpVisitor::showindent() {
    if (indentLevel_)
        std::cout << std::string(" ", indentLevel_);
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

void dumpVisitor::visit_literalString(literalString* n)  {
    showindent();
    std::cout << "literal string: \"" << n->getStringVal() << "\"" << std::endl;
}

void dumpVisitor::visit_literalInt(literalInt* n)  {
    showindent();
    std::cout << "literal int: " << n->getStringVal() << std::endl;
}

void dumpVisitor::visit_literalFloat(literalFloat* n)  {
    showindent();
    std::cout << "literal float: " << n->getStringVal() << std::endl;
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

