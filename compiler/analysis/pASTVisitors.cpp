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

#include <iostream>
#include <unicode/ustream.h>
#include "rphp/analysis/pASTVisitors.h"

namespace rphp { namespace AST {

baseVisitor::dispatchFunction baseVisitor::dispatchTable_[] = {
    // NOTE: requires same order as nodeKind enum in pAST.h
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_block),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_functionDecl),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_ifStmt),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_echoStmt),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_inlineHtml),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_literalString),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_literalInt),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_literalFloat),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_literalNull),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_literalBool),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_literalArray),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_logicalNot),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_assignment),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_var),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_functionInvoke),
    reinterpret_cast<dispatchFunction>(&baseVisitor::visit_constructorInvoke)
};


void baseVisitor::visit(stmt* s) {

    if (s) {
      (this->*dispatchTable_[s->getKind()])(s);
    }

}

void baseVisitor::visit_block(block* b) {
    for (statementList::size_type i=0; i < b->statements.size(); i++) {
        (this->*dispatchTable_[b->statements[i]->getKind()])(b->statements[i]);
    }
}

// ** DUMP VISITOR **

void dumpVisitor::showindent() {
    if (indentLevel_)
        std::cout << std::string(indentLevel_, ' ');
}

void dumpVisitor::visit_functionDecl(functionDecl* n) {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(function declare: " << n->functionDef()->name() << std::endl;

    indent();
    showindent();
    std::cout << "(arguments: " << std::endl;
    indent();
    for (pFunction::paramListType::size_type i = 0; i < n->functionDef()->numParams(); i++) {
        showindent();
        std::cout << i+1 << ": " << n->functionDef()->param(i)->name() << std::endl;
        // TODO more info on params
    }
    unindent();
    showindent();
    std::cout << ")" << std::endl;
    unindent();

    showindent();
    std::cout << ")" << std::endl;
}

void dumpVisitor::visit_ifStmt(ifStmt* n) {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(if:" << std::endl;
    std::cout << " cond:" << std::endl;
    indent();
    visit(n->condition());
    unindent();
    showindent();
    std::cout << " true:" << std::endl;
    indent();
    visit(n->trueBlock());
    unindent();
    showindent();
    std::cout << ")" << std::endl;
}

void dumpVisitor::visit_echoStmt(echoStmt* n) {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(echoStmt:" << std::endl;
    indent();
    visit(n->rVal());
    unindent();
    showindent();
    std::cout << ")" << std::endl;
}

void dumpVisitor::visit_inlineHtml(inlineHtml* n)  {
    // NOTE: newlines are lexed separately, don't dump them here
    if (n->getStringVal().length() == 1 &&
        n->getStringVal().at(0) == RPHP_WNEWLINE)
        return;
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "inline HTML ";
    if (n->isBinary()) {
        std::cout << "[binary]: \"";
    }
    else {
        std::cout << "[unicode]: \"";
    }
    std::wcout << n->getStringVal();
    std::cout << "\"" << std::endl;
}

void dumpVisitor::visit_literalString(literalString* n)  {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal string ";
    if (n->isBinary()) {
        std::cout << "[binary]: \"";
    }
    else {
        std::cout << "[unicode]: \"";
    }
    std::wcout << n->getStringVal();
    std::cout << "\"" << std::endl;
}

void dumpVisitor::visit_literalInt(literalInt* n)  {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal int: ";
    std::wcout << n->getStringVal();
    std::cout << std::endl;
}

void dumpVisitor::visit_literalFloat(literalFloat* n)  {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal float: ";
    std::wcout << n->getStringVal();
    std::cout << std::endl;
}

void dumpVisitor::visit_literalBool(literalBool* n)  {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal bool: ";
    if (n->getBoolVal())
        std::cout << "TRUE";
    else
        std::cout << "FALSE";
    std::cout << std::endl;
}

void dumpVisitor::visit_literalArray(literalArray* n)  {

    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(literal array: " << std::endl;

    indent();
    showindent();
    std::cout << "(contents: " << std::endl;
    indent();
    for (arrayList::const_reverse_iterator i = n->itemList().rbegin();
        i != n->itemList().rend();
        ++i)
    {
        showindent();
        std::cout << "is ref? " << (*i).isRef << std::endl;
        showindent();
        std::cout << "key: " << std::endl;
        indent();
        if ((*i).key) {
            visit((*i).key);
        }
        else {
            showindent();
            std::cout << "<NEXT>" << std::endl;
        }
        unindent();
        showindent();
        std::cout << "val: " << std::endl;
        indent();
        visit((*i).val);
        unindent();
    }
    unindent();
    showindent();
    std::cout << ")" << std::endl;
    unindent();
    showindent();
    std::cout << ")" << std::endl;

}


void dumpVisitor::visit_literalNull(literalNull* n)  {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal NULL" << std::endl;
}

void dumpVisitor::visit_logicalNot(logicalNot* n)  {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(logical NOT:" << std::endl;
    indent();

    showindent();
    std::cout << "(rval: " << std::endl;
    indent();
    visit(n->rVal());
    unindent();
    showindent();
    std::cout << ")" << std::endl;

    unindent();
    showindent();
    std::cout << ")" << std::endl;
}


void dumpVisitor::visit_assignment(assignment* n)  {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(assignment:" << std::endl;
    indent();

    showindent();
    std::cout << "(lval: " << std::endl;
    indent();
    visit(n->lVal());
    unindent();
    showindent();
    std::cout << ")" << std::endl;

    showindent();
    std::cout << "(rval: " << std::endl;
    indent();
    visit(n->rVal());
    unindent();
    showindent();
    std::cout << ")" << std::endl;

    unindent();
    showindent();
    std::cout << ")" << std::endl;
}

void dumpVisitor::visit_var(var* n)  {
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "var: $";
    std::cout << n->name();
    std::cout << std::endl;
}

void dumpVisitor::visit_functionInvoke(functionInvoke* n)  {

    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(function invoke: ";
    std::cout << n->name() << std::endl;

    indent();
    showindent();
    std::cout << "(arguments: " << std::endl;
    indent();
    for (expressionList::size_type i = 0; i < n->argList().size(); i++) {
        showindent();
        std::cout << i+1 << ": " << std::endl;
        visit(n->argList()[i]);
    }
    unindent();
    showindent();
    std::cout << ")" << std::endl;
    unindent();
    showindent();
    std::cout << ")" << std::endl;

}

void dumpVisitor::visit_constructorInvoke(constructorInvoke* n)  {

    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(constructor invoke: ";
    std::cout << n->name() << std::endl;

    indent();
    showindent();
    std::cout << "(arguments: " << std::endl;
    indent();
    for (expressionList::size_type i = 0; i < n->argList().size(); i++) {
        showindent();
        std::cout << i+1 << ": " << std::endl;
        visit(n->argList()[i]);
    }
    unindent();
    showindent();
    std::cout << ")" << std::endl;
    unindent();
    showindent();
    std::cout << ")" << std::endl;

}

} } // namespace

