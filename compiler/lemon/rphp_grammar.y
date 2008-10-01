/*

rphp language grammar

*/

%include {   

#include <iostream>
#include <string>

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

// 
%type T_INLINE_HTML {int}

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

