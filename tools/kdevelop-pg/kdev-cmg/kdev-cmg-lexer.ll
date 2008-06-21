%{
/* This file is part of kdev-cmg
   Copyright (C) 2006 Jakob Petsovits <jpetso@gmx.at>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kdev-cmg.h"
#include "kdevcmg_parser.h"

#include <iostream>

/* call this before calling yylex(): */
void lexer_restart(kdevcmg::parser* parser);

extern std::size_t _G_token_begin, _G_token_end;
extern world _G_system;



/* the rest of these declarations are internal to the lexer,
 * don't use them outside of this file. */

namespace
{
  std::size_t _G_current_offset;
  std::size_t _G_temp_begin;
  kdevcmg::parser* _G_parser;
}


#define YY_INPUT(buf, result, max_size) \
  { \
    int c = _G_system.contents[_G_current_offset++]; \
    result = c == 0 ? YY_NULL : (buf[0] = c, 1); \
  }

#define YY_USER_INIT \
_G_token_begin = _G_token_end = 0; \
_G_current_offset = 0;

#define YY_USER_ACTION \
_G_token_begin = _G_token_end; \
_G_token_end += yyleng;

%}

Whitespace  [ \f\t]
Newline     "\r\n"|\r|\n
Space       ({Whitespace}|{Newline})

%x TYPE
%x CODE
%x DECLARATION

%%


{Whitespace}*           /* skip */ ;
{Newline}               /* skip */ ;
"--"[^\r\n]*            /* line comments, skip */ ;

":"{Space}*             BEGIN(TYPE);    return kdevcmg::parser::Token_COLON;
";"                     return kdevcmg::parser::Token_SEMICOLON;
"{"                     return kdevcmg::parser::Token_LBRACE;
"}"                     return kdevcmg::parser::Token_RBRACE;
"("                     return kdevcmg::parser::Token_LPAREN;
")"                     return kdevcmg::parser::Token_RPAREN;
","                     return kdevcmg::parser::Token_COMMA;
"#"                     return kdevcmg::parser::Token_HASH;
"="                     return kdevcmg::parser::Token_ASSIGN;

<INITIAL,TYPE>"CodeModel"{Whitespace}*      BEGIN(INITIAL); return kdevcmg::parser::Token_CODEMODEL;
"root"                  return kdevcmg::parser::Token_ROOT;

"constructor"           return kdevcmg::parser::Token_CONSTRUCTOR;
"destructor"            return kdevcmg::parser::Token_DESTRUCTOR;
"implementation"        return kdevcmg::parser::Token_IMPLEMENTATION;
"public"                BEGIN(DECLARATION); return kdevcmg::parser::Token_PUBLIC;
"private"               BEGIN(DECLARATION); return kdevcmg::parser::Token_PRIVATE;
"protected"             BEGIN(DECLARATION); return kdevcmg::parser::Token_PROTECTED;

<DECLARATION>{
{Space}*                /* skip */ ;
"declaration"           BEGIN(INITIAL); return kdevcmg::parser::Token_DECLARATION;
.                       BEGIN(INITIAL); return kdevcmg::parser::Token_INVALID;
<<EOF>> {
    BEGIN(INITIAL); // is not set automatically by yyrestart()
    _G_parser->report_problem( kdevcmg::parser::error,
      "Encountered end of file in an unclosed declaration clause" );
    return kdevcmg::parser::Token_EOF;
}
}

"init"                  return kdevcmg::parser::Token_INIT;
"hashed"                return kdevcmg::parser::Token_HASHED;
"multihashed"           return kdevcmg::parser::Token_MULTIHASHED;
"unique"                return kdevcmg::parser::Token_UNIQUE;
"childitem"             return kdevcmg::parser::Token_CHILDITEM;


"[:"                    BEGIN(CODE); _G_temp_begin = _G_token_begin;
<CODE>{
{Newline}
[^:\n\r]*               /* gather everything that's not a colon, and append what comes next */
":"+[^:\]\n\r]*         /* also gather colons that are not followed by colons or newlines */
":]" {
    BEGIN(INITIAL);
    _G_token_begin = _G_temp_begin;
    return kdevcmg::parser::Token_CODE;
}
<<EOF>> {
    BEGIN(INITIAL); // is not set automatically by yyrestart()
    _G_parser->report_problem( kdevcmg::parser::error,
      "Encountered end of file in an unclosed code segment" );
    return kdevcmg::parser::Token_EOF;
}
}


<INITIAL>[_a-zA-Z0-9]+  return kdevcmg::parser::Token_IDENTIFIER;

<TYPE>{
[^\r\n{};\-]+           BEGIN(INITIAL); return kdevcmg::parser::Token_TYPE;
<<EOF>> {
    BEGIN(INITIAL); // is not set automatically by yyrestart()
    _G_parser->report_problem( kdevcmg::parser::error,
      "Encountered end of file in an unclosed code segment" );
    return kdevcmg::parser::Token_EOF;
}
}



 /* everything else is not a valid lexeme */

.                       return kdevcmg::parser::Token_INVALID;


%%

void lexer_restart(kdevcmg::parser* _parser) {
  _G_parser = _parser;
  yyrestart(NULL);
  BEGIN(INITIAL); // is not set automatically by yyrestart()
  YY_USER_INIT
}

int yywrap() { return 1; }
