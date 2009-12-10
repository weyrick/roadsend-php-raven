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

#include "rphp/analysis/pASTVisitors.h"

#include <iostream>
#include <unicode/ustream.h>

namespace rphp { namespace AST {

baseVisitor::dispatchFunction baseVisitor::preDispatchTable_[] = {

#define STMT(CLASS, PARENT) reinterpret_cast<dispatchFunction>( &baseVisitor::visit_pre_##CLASS ),
#include "rphp/analysis/astNodes.def"

};

baseVisitor::dispatchFunction baseVisitor::postDispatchTable_[] = {

#define STMT(CLASS, PARENT) reinterpret_cast<dispatchFunction>( &baseVisitor::visit_post_##CLASS ),
#include "rphp/analysis/astNodes.def"

};

void baseVisitor::visit(stmt* s) {

    // PRE
    visit_pre_stmt(s);
    if (expr* n = llvm::dyn_cast<expr>(s))
        visit_pre_expr(n);
    else if (decl* n = llvm::dyn_cast<decl>(s))
        visit_pre_decl(n);

    (this->*preDispatchTable_[s->getKind()])(s);

    // CHILDREN
    stmt* child(0);
    for (stmt::child_iterator i = s->child_begin(), e = s->child_end(); i != e; ) {
      if ( (child = *i++) ) {
          visit(child);
      }
    }

    // POST
    (this->*postDispatchTable_[s->getKind()])(s);

    if (expr* n = llvm::dyn_cast<expr>(s))
        visit_post_expr(n);
    else if (decl* n = llvm::dyn_cast<decl>(s))
        visit_post_decl(n);
    visit_post_stmt(s);

}


/*
void baseVisitor::visit_stmt(stmt* s) {
    if (s) {
        visit_pre_stmt(s);
        dispatch(s);
        visit_post_stmt(s);
    }
}

void baseVisitor::visit_decl(decl* s) {
    if (s) {
        visit_pre_stmt(s);
        visit_pre_decl(s);
        dispatch(s);
        visit_post_decl(s);
        visit_post_stmt(s);
    }
}

void baseVisitor::visit_expr(expr* s) {
    if (s) {
        visit_pre_stmt(s);
        visit_pre_expr(s);
        dispatch(s);
        visit_post_expr(s);
        visit_post_stmt(s);
    }
}

void baseVisitor::visit_literalExpr(literalExpr* s) {
    if (s) {
        visit_pre_stmt(s);
        visit_pre_expr(s);
        visit_pre_literalExpr(s);
        dispatch(s);
        visit_post_literalExpr(s);
        visit_post_expr(s);
        visit_post_stmt(s);
    }
}

void baseVisitor::visit_block(block* b) {
    for (stmt::child_iterator i = b->child_begin(), e = b->child_end(); i != e; ) {
        if (stmt* child = *i++)
            visit(child);
    }
}
*/

// ** DUMP VISITOR **
void dumpVisitor::visit_pre_stmt(stmt* n) {
    ++indentLevel_;
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_stmt " << n << "\n";
}
void dumpVisitor::visit_post_stmt(stmt* n) {
    --indentLevel_;
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "post_stmt " << n << "\n";
}
void dumpVisitor::visit_pre_expr(expr* n) {
    ++indentLevel_;
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_expr " << n << "\n";
}
void dumpVisitor::visit_post_expr(expr* n) {
    --indentLevel_;
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "post_expr " << n << "\n";
}

void dumpVisitor::showindent() {
    if (indentLevel_)
        std::cout << std::string(indentLevel_, ' ');
}

void dumpVisitor::visit_pre_functionDecl(functionDecl* n) {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_functionDecl\n";
    /*
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
        std::cout << i+1 << ": $" << n->functionDef()->param(i)->name() << std::endl;
        // TODO more info on params
    }
    unindent();
    showindent();
    std::cout << ")" << std::endl;
    unindent();


    indent();
    showindent();
    std::cout << "(body: " << std::endl;

    indent();
    visit(n->body());
    unindent();

    showindent();
    std::cout << ")" << std::endl;
    unindent();

    showindent();
    std::cout << ")" << std::endl;
    */
}

void dumpVisitor::visit_pre_ifStmt(ifStmt* n) {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_ifStmt\n";
    /*
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
    */
}

void dumpVisitor::visit_pre_echoStmt(echoStmt* n) {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_echoStmt\n";
    /*
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(echoStmt:" << std::endl;
    indent();
    visit(n->rVal());
    unindent();
    showindent();
    std::cout << ")" << std::endl;
    */
}

void dumpVisitor::visit_pre_inlineHtml(inlineHtml* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_inlineHtml\n";
    /*
    // NOTE: newlines are lexed separately, don't dump them here
    if (n->getStringVal().length() == 1 &&
        n->getStringVal().at(0) == '\n')
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
    std::cout << n->getStringVal();
    std::cout << "\"" << std::endl;
    */
}

void dumpVisitor::visit_pre_literalString(literalString* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_literalString\n";
    /*
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal string ";
    if (n->isBinary()) {
        std::cout << "[binary]: \"";
    }
    else {
        std::cout << "[utf8]: \"";
    }
    std::cout << n->getStringVal();
    std::cout << "\"" << std::endl;
    */
}

void dumpVisitor::visit_pre_literalInt(literalInt* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_literalInt\n";
    /*
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal int: ";
    if (n->negative())
        std::cout << "(negative) ";
    std::cout << n->getStringVal();
    std::cout << std::endl;
    */
}

void dumpVisitor::visit_pre_literalFloat(literalFloat* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_literalFloat\n";
    /*
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal float: ";
    std::cout << n->getStringVal();
    std::cout << std::endl;
    */
}

void dumpVisitor::visit_pre_literalBool(literalBool* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_literalBool\n";
    /*
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal bool: ";
    if (n->getBoolVal())
        std::cout << "TRUE";
    else
        std::cout << "FALSE";
    std::cout << std::endl;
    */
}

void dumpVisitor::visit_pre_literalArray(literalArray* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_literalArray\n";
    /*
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(literal array: " << std::endl;

    indent();
    showindent();
    std::cout << "(contents: " << std::endl;
    indent();
    for (arrayList::reverse_iterator i = n->itemList().rbegin();
        i != n->itemList().rend();
        ++i)
    {
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
    */
}


void dumpVisitor::visit_pre_literalNull(literalNull* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_literalNull\n";
    /*
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "literal NULL" << std::endl;
    */
}

void dumpVisitor::visit_pre_logicalNot(logicalNot* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_literalNot\n";
    /*
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
    */
}


void dumpVisitor::visit_pre_assignment(assignment* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_assignment\n";
    /*
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
    */
}

void dumpVisitor::visit_pre_var(var* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_var\n";
    /*
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "var: $";
    std::cout << n->name();
    std::cout << std::endl;
    */
}

void dumpVisitor::visit_pre_functionInvoke(functionInvoke* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_finvoke\n";

/*
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
    */

}

void dumpVisitor::visit_pre_constructorInvoke(constructorInvoke* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_cinvoke\n";
/*
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
    */

}

void dumpVisitor::visit_pre_unaryArithmeticOp(unaryArithmeticOp* n)  {
    std::cout << "# line " << n->startLineNum() << "\n";
    std::cout << "pre_unary\n";
    /*
    showindent();
    std::cout << "# line " << n->startLineNum() << std::endl;
    showindent();
    std::cout << "(" << ((n->negative()) ? "-" : "+") << std::endl;
    indent();
    visit(n->rVal());
    unindent();
    showindent();
    std::cout << ")" << std::endl;
    */
}

} } // namespace

