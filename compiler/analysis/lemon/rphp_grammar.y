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

%include {   

#include <iostream>
#include <string>
#include <cctype> // for toupper
#include <algorithm>
#include <unicode/unistr.h>

#include "rphp/analysis/pSourceTypes.h"
#include "rphp/analysis/pAST.h"
#include "rphp/analysis/pSourceModule.h"

using namespace rphp;

}  

%name rphpParse
%token_type {pSourceRange*}
%default_type {pSourceRange*}
%extra_argument {pSourceModule* pMod}

// these don't exist in the parser, just in the lexer
%type T_WHITESPACE {int}
%type T_OPEN_TAG {int}
%type T_CLOSE_TAG {int}
%type T_LEFTPAREN {int}
%type T_RIGHTPAREN {int}
%type T_LEFTCURLY {int}
%type T_RIGHTCURLY {int}
%type T_COMMA {int}
%type T_SINGLELINE_COMMENT {int}
%type T_MULTILINE_COMMENT {int}

// tokens
%type T_INLINE_HTML {int}
%type T_IF {int}
%type T_WHILE {int}
%type T_ELSE {int}
%type T_ASSIGN {int}
%type T_DQ_STRING {int}
%type T_NOT {int}
%type T_IDENTIFIER {int}

%syntax_error {  
  pMod->parseError(TOKEN);
}   
%stack_overflow {  
  std::cerr << "Parser stack overflow" << std::endl;
}   

%type module {int}
module ::= statement_list(LIST). { pMod->setAST(LIST); delete LIST; }

%type statement_list {AST::statementList*}
statement_list(A) ::= . { A = new AST::statementList(); }
statement_list(A) ::= statement_list(B) statement(C). { B->push_back(C); A = B; }

/******** STATEMENTS ********/
%type statement {AST::stmt*}
statement(A) ::= statementBlock(B). { A = B; }
statement(A) ::= inlineHTML(B). { A = B; }
statement(A) ::= echo(B) T_SEMI. { A = B; }
statement(A) ::= expr(B) T_SEMI. { A = B; }
statement(A) ::= ifBlock(B). { A = B; }
statement ::= T_SEMI.

// statement block
%type statementBlock{AST::block*}
statementBlock(A) ::= T_LEFTCURLY statement_list(B) T_RIGHTCURLY.
{
    A = new AST::block();
    A->statements.assign(B->begin(), B->end());
    A->setLine(pMod->currentLineNum());
    delete B;
}

// echo
%type echo {AST::echoStmt*}
echo(A) ::= T_ECHO expr(B).
{
    A = new AST::echoStmt(B);
    A->setLine(pMod->currentLineNum());
}

// inline html
%type inlineHTML {AST::inlineHtml*}
inlineHTML(A) ::= T_INLINE_HTML(B).
{
    A = new AST::inlineHtml(*B);
    A->setLine(pMod->currentLineNum());
}

// conditionals
%type ifBlock {AST::ifStmt*}
ifBlock(A) ::= T_IF T_LEFTPAREN expr(COND) T_RIGHTPAREN statementBlock(TRUEBODY).
{
    A = new AST::ifStmt(COND, TRUEBODY);
    A->setLine(pMod->currentLineNum());
}

/****** EXPRESSIONS *********/
%type expr {AST::expr*}
expr(A) ::= literal(B). { A = B; }
expr(A) ::= assignment(B). { A = B; }
expr(A) ::= lval(B). { A = B; }
expr(A) ::= functionInvoke(B). { A = B; }
expr(A) ::= constructorInvoke(B). { A = B; }
expr(A) ::= logicalNot(B). { A = B; }

/** LITERALS **/
%type literal {AST::literalExpr*}

// literal string
// note, this assumes the preprocessor has been run, and thus we
// only need to handle single quoted strings
literal(A) ::= T_SQ_STRING(B).
{
  // binary specifier?
  bool binaryString = false;
  pSourceCharIterator start;
  if ( *(*B).begin() == 'b') {
      // binary
      binaryString = true;
      start = ++(*B).begin();
  }
  else {
      // according to module default
      start = (*B).begin();
  }
  // substring out the quotes, special case for empty string
  if (++start == (*B).end()) {
    A = new AST::literalString(binaryString);
  }
  else {
    A = new AST::literalString(pSourceRange(start, --(*B).end()), binaryString);
  }
  A->setLine(pMod->currentLineNum());
}

// literal integers (decimal)
literal(A) ::= T_LNUMBER(B).
{
    A = new AST::literalInt(*B);
    A->setLine(pMod->currentLineNum());
}

// literal integers (float)
literal(A) ::= T_DNUMBER(B).
{
    A = new AST::literalFloat(*B);
    A->setLine(pMod->currentLineNum());    
}

// literal identifier: null, true, false or string
literal(A) ::= T_IDENTIFIER(B).
{
    // case insensitive checks
    pSourceString ciTmp((*B).begin(), (*B).end());
    transform(ciTmp.begin(), ciTmp.end(), ciTmp.begin(), toupper);
    if (ciTmp == L"NULL") {
        A = new AST::literalNull();
    }
    else if (ciTmp == L"TRUE") {
        A = new AST::literalBool(true);
    }
    else if (ciTmp == L"FALSE") {
        A = new AST::literalBool(false);
    }
    else {
        // default to normal string
        A = new AST::literalString(*B);
    }
    A->setLine(pMod->currentLineNum());
}

/** LOGICAL OPERATORS **/
%type logicalNot {AST::logicalNot*}
logicalNot(A) ::= T_NOT expr(R).
{
    A = new AST::logicalNot(R);
    A->setLine(pMod->currentLineNum());
}

/** ASSIGNMENT **/
%type assignment {AST::assignment*}
assignment(A) ::= lval(L) T_ASSIGN expr(R).
{
    A = new AST::assignment(L, R);
    A->setLine(pMod->currentLineNum());
}

/** LVALS **/
%type lval {AST::expr*}
lval(A) ::= variable_lVal(B). { A = B; }

%type variable_lVal {AST::var*}
variable_lVal(A) ::= T_VARIABLE(B).
{
    // strip $
    A = new AST::var(pSourceRange(++(*B).begin(), (*B).end()));
    A->setLine(pMod->currentLineNum());
}

/** ARGLIST **/
%type argList {AST::expressionList*}
argList(A) ::= expr(B).
{
    A = new AST::expressionList();
    A->push_back(B);
}
argList(A) ::= expr(B) T_COMMA argList(C).
{
    C->push_back(B);
    A = C;
}
argList(A) ::= .
{
    A = new AST::expressionList();
}

/** FUNCTION INVOKE **/
%type functionInvoke {AST::functionInvoke*}
functionInvoke(A) ::= T_IDENTIFIER(B) T_LEFTPAREN argList(C) T_RIGHTPAREN.
{
    A = new AST::functionInvoke(*B, // f name
                                 C  // expression list: arguments
                                );
    A->setLine(pMod->currentLineNum());
}

/** CONSTRUCTOR INVOKE **/
%type constructorInvoke {AST::constructorInvoke*}
constructorInvoke(A) ::= T_NEW T_IDENTIFIER(B) T_LEFTPAREN argList(C) T_RIGHTPAREN.
{
    A = new AST::constructorInvoke(*B, // f name
                                    C  // expression list: arguments
                                   );
    A->setLine(pMod->currentLineNum());
}

