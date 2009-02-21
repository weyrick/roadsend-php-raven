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

#include "rphp/analysis/pAST.h"
#include "rphp/analysis/pSourceTypes.h"
#include "rphp/analysis/pSourceModule.h"

using namespace rphp;

#define TOKEN_LINE(T) pMod->getTokenLine(T->begin())

}  

%name rphpParse
%token_type {pSourceRange*}
%default_type {pSourceRange*}
%extra_argument {pSourceModule* pMod}


// tokens that don't need parse info or AST nodes
%type T_WHITESPACE {int}
%type T_OPEN_TAG {int}
%type T_CLOSE_TAG {int}
%type T_LEFTPAREN {int}
%type T_RIGHTPAREN {int}
%type T_LEFTCURLY {int}
%type T_RIGHTCURLY {int}
%type T_LEFTSQUARE {int}
%type T_RIGHTSQUARE {int}
%type T_COMMA {int}
%type T_SINGLELINE_COMMENT {int}
%type T_MULTILINE_COMMENT {int}
%type T_DOC_COMMENT {int}
%type T_INLINE_HTML {int}
%type T_WHILE {int}
%type T_ENDWHILE {int}
%type T_ELSE {int}
%type T_ELSEIF {int}
%type T_ARRAY {int}
%type T_ARROWKEY {int}
%type T_AND {int}
%type T_ASSIGN {int}
%type T_DQ_STRING {int}
%type T_NOT {int}
%type T_BOOLEAN_AND {int}
%type T_BOOLEAN_OR {int}
%type T_IDENTIFIER {int}
%type T_GLOBAL {int}
%type T_FUNCTION {int}
%type T_EMPTY {int}
%type T_EQUAL {int}
%type T_ISSET {int}
%type T_UNSET {int}
%type T_VAR {int}
%type T_CLASS {int}
%type T_CLASSDEREF {int}
%type T_FOREACH {int}
%type T_ENDFOREACH {int}
%type T_FOR {int}
%type T_ENDFOR {int}
%type T_AS {int}
%type T_RETURN {int}
%type T_DOT {int}
%type T_DOTEQUAL {int}
%type T_GREATER_THAN {int}
%type T_LESS_THAN {int}
%type T_GREATER_THAN_OR_EQUAL {int}
%type T_LESS_THAN_OR_EQUAL {int}
%type T_LIST {int}
%type T_EXTENDS {int}
%type T_PUBLIC {int}
%type T_PRIVATE {int}
%type T_PROTECTED {int}
%type T_NOTEQUAL {int}
%type T_INCLUDE {int}
%type T_INCLUDE_ONCE {int}
%type T_REQUIRE {int}
%type T_REQUIRE_ONCE {int}
%type T_IDENTICAL {int}
%type T_NOT_IDENTICAL {int}
%type T_QUESTION {int}
%type T_COLON {int}
%type T_DBL_COLON {int}
%type T_INC {int}
%type T_DEC {int}
%type T_EXIT {int}
%type T_MINUS {int}
%type T_PLUS {int}
%type T_DIV {int}
%type T_MOD {int}
%type T_MULT {int}
%type T_SWITCH {int}
%type T_ENDSWITCH {int}
%type T_CASE {int}
%type T_BREAK {int}
%type T_CONTINUE {int}
%type T_DEFAULT {int}
%type T_CLONE {int}
%type T_TRY {int}
%type T_CATCH {int}
%type T_THROW {int}
%type T_STATIC {int}
%type T_CONST {int}
%type T_AT {int}
%type T_INSTANCEOF {int}
%type T_LOGICAL_OR {int}
%type T_LOGICAL_AND {int}
%type T_LOGICAL_XOR {int}
%type T_PLUS_EQUAL {int}
%type T_MINUS_EQUAL {int}
%type T_EVAL {int}
%type T_SR_EQUAL {int}
%type T_SL_EQUAL {int}
%type T_XOR_EQUAL {int}
%type T_OR_EQUAL {int}
%type T_AND_EQUAL {int}
%type T_MOD_EQUAL {int}
%type T_CONCAT_EQUAL {int}
%type T_DIV_EQUAL {int}
%type T_MUL_EQUAL {int}
%type T_SR {int}
%type T_SL {int}
%type T_NAMESPACE {int}
%type T_INT_CAST {int}
%type T_FLOAT_CAST {int}
%type T_STRING_CAST {int}
%type T_UNICODE_CAST {int}
%type T_BINARY_CAST {int}
%type T_ARRAY_CAST {int}
%type T_OBJECT_CAST {int}
%type T_UNSET_CAST {int}
%type T_BOOL_CAST {int}
%type T_GOTO {int}
%type T_MAGIC_FILE {int}
%type T_MAGIC_LINE {int}
%type T_MAGIC_NS {int}
%type T_MAGIC_CLASS {int}
%type T_MAGIC_FUNCTION {int}
%type T_MAGIC_METHOD {int}
%type T_TICK {int}
%type T_TILDE {int}
%type T_PIPE {int}
%type T_PRINT {int}
%type T_INTERFACE {int}

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
statement(A) ::= functionDecl(B). { A = B; }
statement(A) ::= echo(B) T_SEMI. { A = B; }
statement(A) ::= expr(B) T_SEMI. { A = B; }
statement(A) ::= ifBlock(B). { A = B; }
statement ::= T_SEMI.

// statement block
%type statementBlock{AST::block*}
statementBlock(A) ::= T_LEFTCURLY(LC) statement_list(B) T_RIGHTCURLY(RC).
{
    A = new AST::block();
    A->statements.assign(B->begin(), B->end());
    A->setLine(TOKEN_LINE(LC), TOKEN_LINE(RC));
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
ifBlock(A) ::= T_IF(IF) T_LEFTPAREN expr(COND) T_RIGHTPAREN statementBlock(TRUEBODY).
{
    A = new AST::ifStmt(COND, TRUEBODY);
    A->setLine(TOKEN_LINE(IF));
}

/** DECLARATIONS **/

/** DECLARATION ARGLIST **/
%type decl_argList {pFunction::paramListType*}
decl_argList(A) ::= T_VARIABLE(NAMED_PARAM).
{
	A = new pFunction::paramListType();
	pFunctionParam*p = new pFunctionParam();
	p->setName(pIdentString(++(*NAMED_PARAM).begin(), (*NAMED_PARAM).end()));
	A->push_back(p);
}
decl_argList(A) ::= T_VARIABLE(NAMED_PARAM) T_COMMA decl_argList(C).
{
	pFunctionParam*p = new pFunctionParam();
	p->setName(pIdentString(++(*NAMED_PARAM).begin(), (*NAMED_PARAM).end()));
	C->push_back(p);
	A = C;
}
decl_argList(A) ::= .
{
	A = new pFunction::paramListType();
}

// function declaration
%type functionDecl {AST::functionDecl*}
functionDecl(A) ::= T_FUNCTION T_IDENTIFIER(NAME) T_LEFTPAREN decl_argList(ARGS) T_RIGHTPAREN statementBlock(BODY).
{
	pFunction* funDef = new pFunction(pIdentString((*NAME).begin(), (*NAME).end()));
	funDef->setParamList(*ARGS); // takes ownership of pFunctionParam objs
	delete ARGS; // free container from parse
	A = new AST::functionDecl(funDef, BODY, false/* ref? */);
	A->setLine(TOKEN_LINE(NAME));
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

/** LITERAL ARRAY ITEMS **/
%type arrayItemList {AST::arrayList*}
arrayItemList(A) ::= arrayItem(B).
{
    A = new AST::arrayList();
    A->push_back(*B); // copy item into vector
    delete B;
}
arrayItemList(A) ::= arrayItem(B) T_COMMA arrayItemList(C).
{
    C->push_back(*B); // copy item into vector
    A = C;
    delete B;
}
arrayItemList(A) ::= .
{
    A = new AST::arrayList();
}

%type arrayItem {AST::arrayItem*}
arrayItem(A) ::= expr(B).
{
    A = new AST::arrayItem(NULL, B, false);
}
arrayItem(A) ::= T_REF expr(B).
{
    A = new AST::arrayItem(NULL, B, true);
}
arrayItem(A) ::= expr(KEY) T_ARROWKEY expr(VAL).
{
    A = new AST::arrayItem(KEY, VAL, false);
}
arrayItem(A) ::= expr(KEY) T_ARROWKEY T_REF expr(VAL).
{
    A = new AST::arrayItem(KEY, VAL, true);
}

// literal array
literal(A) ::= T_ARRAY(ARY) T_LEFTPAREN arrayItemList(B) T_RIGHTPAREN.
{
    A = new AST::literalArray(B);
    A->setLine(TOKEN_LINE(ARY));
    delete B; // deletes the vector, NOT the exprs in it!
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
assignment(A) ::= lval(L) T_ASSIGN(EQ_SIGN) expr(R).
{
    A = new AST::assignment(L, R);
    A->setLine(TOKEN_LINE(EQ_SIGN));
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
                                 C  // expression list: arguments, copied
                                );
    A->setLine(pMod->currentLineNum());
// NOTE: i'm not sure why we don't need to delete C here
//       it segfaults when we do, and doesn't leak when we don't...
//    delete C; // deletes the vector, NOT the exprs in it!
}

/** CONSTRUCTOR INVOKE **/
%type constructorInvoke {AST::constructorInvoke*}
constructorInvoke(A) ::= T_NEW T_IDENTIFIER(B) T_LEFTPAREN argList(C) T_RIGHTPAREN.
{
    A = new AST::constructorInvoke(*B, // f name
                                    C  // expression list: arguments, copied
                                   );
    A->setLine(pMod->currentLineNum());
// NOTE: i'm not sure why we don't need to delete C here
//       it segfaults when we do, and doesn't leak when we don't...
//    delete C; // deletes the vector, NOT the exprs in it!
}

