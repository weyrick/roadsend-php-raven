
%{
/* This file is part of kdev-pg
   Copyright (C) 2005 Roberto Raggi <roberto@kdevelop.org>
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

#include "kdev-pg.h"
#include "kdev-pg-clone-tree.h"

#include <cassert>

extern int yylex();
extern void yyerror(char const *msg);

%}

%union {
    model::node *item;
    char* str;
    model::variable_declaration_item::declaration_type_enum declaration_type;
    model::variable_declaration_item::storage_type_enum     storage_type;
    model::variable_declaration_item::variable_type_enum    variable_type;
}

%token T_IDENTIFIER T_ARROW T_TERMINAL T_CODE T_STRING ';'
%token T_TOKEN_DECLARATION T_TOKEN_STREAM_DECLARATION T_NAMESPACE_DECLARATION
%token T_PARSERCLASS_DECLARATION T_PUBLIC T_PRIVATE T_PROTECTED T_DECLARATION
%token T_CONSTRUCTOR T_DESTRUCTOR T_TRY_RECOVER T_TRY_ROLLBACK T_CATCH
%token T_RULE_ARGUMENTS T_MEMBER T_TEMPORARY T_ARGUMENT T_EXPORT_MACRO
%token T_NODE T_NODE_SEQUENCE T_TOKEN T_VARIABLE T_EXPORT_MACRO_HEADER

%type<str> T_IDENTIFIER T_TERMINAL T_CODE T_STRING T_RULE_ARGUMENTS
%type<str> name code_opt rule_arguments_opt
%type<item> item primary_item try_item primary_atom unary_item
%type<item> postfix_item option_item item_sequence conditional_item
%type<item> member_declaration_rest variable_declarations variable_declaration
%type<declaration_type> declaration_type_opt
%type<storage_type>     scope storage_type
%type<variable_type>    variable_type

%%

system
    : code_opt { _G_system.decl = $1; }
      declarations
      rules
      code_opt { _G_system.bits = $5; }
    ;

declarations
    : declaration
    | declarations declaration
    ;

declaration
    : T_PARSERCLASS_DECLARATION member_declaration_rest
        { _G_system.push_parserclass_member($2); }
    | T_TOKEN_DECLARATION declared_tokens ';'
    | T_TOKEN_STREAM_DECLARATION T_IDENTIFIER ';'
        { _G_system.token_stream = $2;           }
    | namespace_declaration
    | T_EXPORT_MACRO T_STRING
        { _G_system.export_macro = $2;           }
    | T_EXPORT_MACRO_HEADER T_STRING
        { _G_system.export_macro_header = $2;   }
    ;

member_declaration_rest
    : '(' T_PUBLIC T_DECLARATION ')' T_CODE
        { $$ = pg::member(settings::member_item::public_declaration, $5);    }
    | '(' T_PROTECTED T_DECLARATION ')' T_CODE
        { $$ = pg::member(settings::member_item::protected_declaration, $5); }
    | '(' T_PRIVATE T_DECLARATION ')' T_CODE
        { $$ = pg::member(settings::member_item::private_declaration, $5);   }
    | '(' T_CONSTRUCTOR ')' T_CODE
        { $$ = pg::member(settings::member_item::constructor_code, $4);      }
    | '(' T_DESTRUCTOR ')' T_CODE
        { $$ = pg::member(settings::member_item::destructor_code, $4);       }
    ;

namespace_declaration
    : T_NAMESPACE_DECLARATION T_IDENTIFIER T_CODE
        { _G_system.push_namespace($2, $3); }
    ;

declared_tokens
    : T_TERMINAL                        { _G_system.push_terminal($1,$1); }
    | T_TERMINAL '(' T_STRING ')'       { _G_system.push_terminal($1,$3); }
    | declared_tokens ',' T_TERMINAL    { _G_system.push_terminal($3,$3); }
    | declared_tokens ',' T_TERMINAL '(' T_STRING ')'
                                        { _G_system.push_terminal($3,$5); }
    ;

rules
    : item ';'                          { _G_system.push_rule($1); }
    | rules item ';'                    { _G_system.push_rule($2); }
    | rules namespace_declaration
    ;

primary_item
    : '0'                               { $$ = _G_system.zero(); }
    | '(' option_item ')'               { $$ = $2; }
    | try_item                    { $$ = $1; }
    | primary_atom                      { $$ = $1; }
    | name scope primary_atom           { $$ = pg::annotation($1, $3, false, $2); }
    | '#' name scope primary_atom       { $$ = pg::annotation($2, $4, true, $3);  }
    ;

primary_atom
    : T_IDENTIFIER rule_arguments_opt   { $$ = pg::nonterminal(_G_system.push_symbol($1), $2); }
    | T_TERMINAL                        { $$ = _G_system.terminal($1); }
    ;

try_item
    : T_TRY_RECOVER '(' option_item ')'
        {
          _G_system.need_state_management = true;
          $$ = pg::try_catch($3, 0);
        }
    | T_TRY_ROLLBACK '(' option_item ')' T_CATCH '(' option_item ')'
        {
          _G_system.need_state_management = true;
          $$ = pg::try_catch($3, $7);
        }

rule_arguments_opt
    : /* empty */                       { $$ = ""; }
    | T_RULE_ARGUMENTS                  { $$ = $1; }
    ;

name
    : T_IDENTIFIER                      { $$ = $1; }
    | T_IDENTIFIER '.' T_IDENTIFIER
        {
          $$ = $3;
          fprintf(stderr, "** WARNING support for scoped name"
                          " ``%s.%s'' not implemented\n", $1, $3);
        }
    ;

scope
    : '=' { $$ = model::variable_declaration_item::storage_ast_member; }
    | ':' { $$ = model::variable_declaration_item::storage_temporary;  }
    ;

unary_item
    : primary_item '+'                  { $$ = pg::plus($1); }
    | primary_item '*'                  { $$ = pg::star($1); }
    | primary_item                      { $$ = $1; }
/*    | '?' primary_item                  { $$ = pg::alternative($2, _G_system.zero()); } */
    ;

postfix_item
    : unary_item                        { $$ = $1; }
    | postfix_item '@' primary_item
        {
          clone_tree cl;
          $$ = pg::cons($1, pg::star(pg::cons(cl.clone($3), cl.clone($1))));
        }
    | postfix_item T_CODE               { $$ = pg::action($1, $2); }
    ;

item_sequence
    : postfix_item                      { $$ = $1; }
    | item_sequence postfix_item        { $$ = pg::cons($1, $2); }
    ;

conditional_item
    : item_sequence                     { $$ = $1; }
    | '?' T_CODE item_sequence          { $$ = pg::condition($2, $3); }
    ;

option_item
    : conditional_item                  { $$ = $1; }
    | option_item '|' conditional_item  { $$ = pg::alternative($1, $3); }
    ;

item
    : option_item T_ARROW T_IDENTIFIER T_CODE '[' variable_declarations ']'
        {
          $$ = pg::evolve($1, _G_system.push_symbol($3),
                          (model::variable_declaration_item*) $6, $4);
        }
    | option_item T_ARROW T_IDENTIFIER '[' variable_declarations ']' code_opt
        {
          $$ = pg::evolve($1, _G_system.push_symbol($3),
                          (model::variable_declaration_item*) $5, $7);
        }
    | option_item T_ARROW T_IDENTIFIER code_opt
        { $$ = pg::evolve($1, _G_system.push_symbol($3), 0, $4); }
    ;

code_opt
    : /* empty */                       { $$ = ""; }
    | T_CODE                            { $$ = $1; }
    ;

variable_declarations
    : variable_declaration              { $$ = $1; }
    | variable_declarations variable_declaration
        {
          model::variable_declaration_item *last = (model::variable_declaration_item*) $1;
          while (last->_M_next != 0) {
            last = last->_M_next;
          }
          last->_M_next = (model::variable_declaration_item*) $2;
          $$ = $1;
        }
    ;

variable_declaration
    : declaration_type_opt storage_type variable_type T_IDENTIFIER ':' T_IDENTIFIER
        { $$ = pg::variable_declaration($1, $2, $3, false, $4, $6); }
    | declaration_type_opt storage_type T_TOKEN       T_IDENTIFIER ';'
        { $$ = pg::variable_declaration($1, $2, model::variable_declaration_item::type_token, false, $4, ""); }
    | declaration_type_opt storage_type variable_type '#' T_IDENTIFIER ':' T_IDENTIFIER
        { $$ = pg::variable_declaration($1, $2, $3, true, $5, $7); }
    | declaration_type_opt storage_type T_TOKEN       '#' T_IDENTIFIER ';'
        { $$ = pg::variable_declaration($1, $2, model::variable_declaration_item::type_token, true, $5, ""); }
    ;

declaration_type_opt
    : /* empty */       { $$ = model::variable_declaration_item::declaration_local;     }
    | T_ARGUMENT        { $$ = model::variable_declaration_item::declaration_argument;  }
    ;

storage_type
    : T_MEMBER          { $$ = model::variable_declaration_item::storage_ast_member;    }
    | T_TEMPORARY       { $$ = model::variable_declaration_item::storage_temporary;     }
    ;

variable_type
    : T_NODE            { $$ = model::variable_declaration_item::type_node;             }
    | T_VARIABLE        { $$ = model::variable_declaration_item::type_variable;         }
    ;

%%
