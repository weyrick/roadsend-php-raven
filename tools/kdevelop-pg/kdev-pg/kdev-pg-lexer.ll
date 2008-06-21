%{
/* This file is part of kdev-pg
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

#include <iostream>
#include <cstdio>
#include "kdev-pg.h"
#include "kdev-pg-parser.hh"

int inp();
void append_linebuffer();
void newline();
void yyerror(char const *);

#define YY_INPUT(buf, result, max_size) \
  { \
    char c = inp(); \
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
  }

#define YY_USER_ACTION  append_linebuffer();

#define COPY_TO_YYLVAL(string, len) \
  yylval.str = (char*) calloc(yyleng+1, sizeof(char)); \
  strcpy(yylval.str, string); \
  yylval.str[len] = '\0';

namespace {
  enum rule_position_enum {
    rule_body,
    rule_footer,
  };
  rule_position_enum rule_position = rule_body;
  int open_brackets; // for rule arguments usage
}

%}

Whitespace  [ \f\t]
Newline     "\r\n"|\r|\n
String      ["]([^\r\n\"]|[\\][^\r\n])*["]

%x CODE
%x PARSERCLASS
%x RULE_ARGUMENTS
%x RULE_PARAMETERS_HEADER
%x RULE_PARAMETERS_VARNAME

%%


{Whitespace}*           /* skip */ ;
{Newline}               newline();
"--"[^\r\n]*            /* line comments, skip */ ;

";;"                    rule_position = rule_body;   return ';';
"->"                    rule_position = rule_footer; return T_ARROW;

"("                     return '(';
")"                     return ')';
","                     return ',';
"0"                     return '0';
"#"                     return '#';
"."                     return '.';
":"                     return ':';
"="                     return '=';
"+"                     return '+';
"*"                     return '*';
"?"                     return '?';
"@"                     return '@';
"|"                     return '|';

"try/recover"           return T_TRY_RECOVER;
"try/rollback"          return T_TRY_ROLLBACK;
"catch"                 return T_CATCH;

"%export_macro"         return T_EXPORT_MACRO;
"%export_macro_header"  return T_EXPORT_MACRO_HEADER;
"%token"                return T_TOKEN_DECLARATION;
"%token_stream"         return T_TOKEN_STREAM_DECLARATION;
"%namespace"            return T_NAMESPACE_DECLARATION;
"%parserclass"          BEGIN(PARSERCLASS); return T_PARSERCLASS_DECLARATION;


<PARSERCLASS>{
{Whitespace}*           /* skip */ ;
{Newline}               newline();
"("                     return '(';
"public"                return T_PUBLIC;
"private"               return T_PRIVATE;
"protected"             return T_PROTECTED;
"declaration"           return T_DECLARATION;
"constructor"           return T_CONSTRUCTOR;
"destructor"            return T_DESTRUCTOR;
")"                     BEGIN(INITIAL); return ')';
.                       BEGIN(INITIAL); REJECT; /* everything else */
}


"[" {
    if (rule_position == rule_body) { /* use the arguments in a rule call */
      open_brackets = 0;
      BEGIN(RULE_ARGUMENTS);
    }
    else if (rule_position == rule_footer) { /* declare the arguments */
      BEGIN(RULE_PARAMETERS_HEADER); return '[';
    }
}

<RULE_ARGUMENTS>{
{Newline}               newline(); yymore();
{String}                yymore(); /* this and... */
["]                     yymore(); /* ...this prevent brackets inside strings to be counted */
[^\[\]\n\r\"]*          yymore(); /* gather everything that's not a bracket, and append what comes next */
"["                     open_brackets++; yymore();
"]" {
    open_brackets--;
    if (open_brackets < 0) {
      COPY_TO_YYLVAL(yytext,yyleng-1); /* cut off the trailing bracket */
      BEGIN(INITIAL);
      return T_RULE_ARGUMENTS;
    }
}
<<EOF>> {
    BEGIN(INITIAL); // is not set automatically by yyrestart()
    std::cout << "Encountered end of file in an unclosed rule argument specification..." << std::endl;
    yyerror("");
    return 0;
}
}

<RULE_PARAMETERS_HEADER>{
{Whitespace}*           /* skip */ ;
{Newline}               newline();
"--"[^\r\n]*            /* line comments, skip */ ;
":"{Whitespace}*        BEGIN(RULE_PARAMETERS_VARNAME); return ':';
"#"                     return '#';
"member"                return T_MEMBER;
"temporary"             return T_TEMPORARY;
"argument"              return T_ARGUMENT;
"node"                  return T_NODE;
"token"                 return T_TOKEN;
"variable"              return T_VARIABLE;
";"                     return ';';  /* only used for "token" types */
[_a-zA-Z0-9]+           COPY_TO_YYLVAL(yytext,yyleng); return T_IDENTIFIER;
"]"                     BEGIN(INITIAL); return ']';
.                       BEGIN(INITIAL); REJECT; /* everything else */
}

<RULE_PARAMETERS_VARNAME>{
{Newline}               newline(); yymore();
[^;\r\n]*               yymore(); /* gather everything that's not a semicolon, and append what comes next */
";" {
    // strip trailing whitespace
    int length = yyleng-1; // and first, the trailing semicolon
    for (int i = length-1; i < 1; i--) {
      switch(yytext[i-1])
      {
        case ' ':
        case '\f':
        case '\t':
          continue;
        default:
          length = i;
          break;
      }
    }
    COPY_TO_YYLVAL(yytext,length);
    BEGIN(RULE_PARAMETERS_HEADER);
    return T_IDENTIFIER;
}
.                       BEGIN(INITIAL); REJECT; /* everything else */
}


"[:"                    BEGIN(CODE);
<CODE>{
{Newline}               newline(); yymore();
[^:\n\r]*               yymore(); /* gather everything that's not a colon, and append what comes next */
":"+[^:\]\n\r]*         yymore(); /* also gather colons that are not followed by colons or newlines */
":]" {
    COPY_TO_YYLVAL(yytext,yyleng-2); /* cut off the trailing stuff */
    BEGIN(INITIAL);
    return T_CODE;
}
<<EOF>> {
    BEGIN(INITIAL); // is not set automatically by yyrestart()
    std::cout << "Encountered end of file in an unclosed code segment..." << std::endl;
    yyerror("");
    return 0;
}
}


[_A-Z]+                 COPY_TO_YYLVAL(yytext,yyleng); return T_TERMINAL;
[_a-zA-Z0-9]+           COPY_TO_YYLVAL(yytext,yyleng); return T_IDENTIFIER;


{String} {
   yytext++;                         /* start inside the quotes */
   COPY_TO_YYLVAL(yytext,yyleng-2);  /* cut off the trailing quote */
   return T_STRING;
}

. {
  std::cout << "Unexpected character: ``" << yytext[0] << "''" << std::endl;
  yyerror("");
}


%%

extern FILE *file;

int ch = 0;
int yy_line = 1, current_offset = 0;
bool end_of_line = false;
char yy_text_line[256 * 1024];

int inp()
{
  return ch = fgetc(file);
}

void newline()
{
  yy_line++;
  end_of_line = true;
}

/* initialize the line buffer */
void clear_linebuffer()
{
  yy_text_line[0] = '\0';
  current_offset = 0;
  end_of_line = false;
}

 /* add the current token to the current line */
void append_linebuffer()
{
  if (end_of_line == true)
    clear_linebuffer();

  current_offset = strlen(yy_text_line); /* start of current */
  strcpy(yy_text_line+current_offset, yytext); /* append current */
  /* strcpy is faster than strcat */
}

void yyerror(char const *)
{
  std::cerr << "** SYNTAX ERROR at line " << yy_line << " column " << current_offset << std::endl;

  char *current_end = yy_text_line + strlen(yy_text_line);
  char *p;

  /* get the rest of the line if we are not already at the end */
  if(!end_of_line)
    {
      p = current_end;
      int c = ch;

      while(c != '\n')
        {
          *p++ = c;
          c = inp();
        }

      *p++ = '\n';
      *p = 0;
    }

  /* yy_text_line[] now has the whole line, with the current token */
  /* at current_offset */

  /* print error message and current line */
  std::cerr << yy_text_line;

  /* print a ^ under the most recent token */
  for (int i = 0; i < current_offset; i++)
    std::cerr << " ";

  std::cout << "^" << std::endl; /* current_offset spaces, then ^ */

  exit(EXIT_FAILURE);
}

int yywrap() { return 1; }

