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

#include "pAST.h"
#include "pModule.h"
#include "pLangLexerDef.h"

using namespace rphp;

}  

%name rphpParse
%token_type {lexer::tokenPairType*}
%default_type {lexer::tokenPairType*}
%extra_argument {pModule* pMod}

// these don't exist in the parser, just in the lexer
%type T_WHITESPACE {int}
%type T_OPEN_TAG {int}
%type T_CLOSE_TAG {int}

// tokens
%type T_INLINE_HTML {int}
%type T_IF {int}
%type T_WHILE {int}
%type T_ELSE {int}

// xxx temp, these are real nodes
%type T_IDENTIFIER {int}
%type T_VARIABLE {int}

%syntax_error {  
  std::cerr << "Syntax error, unexpected: '" << *TOKEN << "'" << std::endl;
}   
%stack_overflow {  
  std::cerr << "Parser stack overflow" << std::endl;
}   

%type module {int}
module ::= statement_list.

%type statement_list {int}
statement_list ::= .
statement_list ::= statement_list statement(A). { pMod->getAST().push_back(A); }

/******** STATEMENTS ********/
%type statement {AST::stmt*}
statement(A) ::= echo(B). { A = B; }
statement(A) ::= inlineHTML(B). { A = B; }

// echo
%type echo {AST::echoStmt*}
echo(A) ::= T_ECHO expr(B) T_SEMI. { A = new AST::echoStmt(B); }

// inline html
%type inlineHTML {AST::inlineHtml*}
inlineHTML(A) ::= T_INLINE_HTML(B).
{
    A = new AST::inlineHtml(std::string((*B).begin(), (*B).end()));
}

/****** EXPRESSIONS *********/
%type expr {AST::expr*}
expr(A) ::= literal(B). { A = B; }

/** LITERALS **/
%type literal {AST::literalExpr*}

// literal bstring, double quotes
literal(A) ::= T_CONSTANT_ENCAPSED_STRING(B).
{
  // substring out the quotes, special case for empty string
  std::string::iterator start = (*B).begin();
  if (++start == (*B).end())
    A = new AST::literalBString(std::string());
  else
    A = new AST::literalBString(std::string(start, --(*B).end()));
}

// literal integers (decimal)
literal(A) ::= T_LNUMBER(B). { A = new AST::literalInt(std::string((*B).begin(), (*B).end())); }

// literal integers (float)
literal(A) ::= T_DNUMBER(B). { A = new AST::literalFloat(std::string((*B).begin(), (*B).end())); }

// literal identifier: null, true, false or string
literal(A) ::= T_IDENTIFIER(B). {
    // case insensitive checks
    std::string ciTmp((*B).begin(), (*B).end());
    transform(ciTmp.begin(), ciTmp.end(), ciTmp.begin(), toupper);
    if (ciTmp == "NULL") {
        A = new AST::literalNull();
    }
    else if (ciTmp == "TRUE") {
        A = new AST::literalBool(true);
    }
    else if (ciTmp == "FALSE") {
        A = new AST::literalBool(false);
    }
    else {
        // default to normal string
        A = new AST::literalBString(std::string((*B).begin(), (*B).end()));
    }
}
