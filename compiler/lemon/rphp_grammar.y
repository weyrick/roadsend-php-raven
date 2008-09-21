/*

rphp language grammar

*/

%include {   

#include <iostream>
#include <string>

#include "pAST.h"
#include "pLangLexerDef.h"

using namespace rphp;

}  

%name rphpParse
%token_type {lexer::tokenPairType*}
%default_type {lexer::tokenPairType*}
%extra_argument {AST::treeTop* ast}

%type T_WHITESPACE {int}
   
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
statement_list ::= statement_list statement(A). { ast->statementList.push_back(A); }

%type statement {AST::statementNode*}
statement(A) ::= echo(B). { A = new AST::statementNode(B); }

%type echo {AST::echoNode*}
echo(A) ::= T_ECHO T_CONSTANT_ENCAPSED_STRING(B) T_SEMI. { A = new AST::echoNode(std::string((*B).begin(), (*B).end())); }
