-- This file is part of KDevelop.
-- Copyright (c) 2006 Jakob Petsovits <jpetso@gmx.at>
--
-- This grammar is free software; you can redistribute it and/or
-- modify it under the terms of the GNU Library General Public
-- License as published by the Free Software Foundation; either
-- version 2 of the License, or (at your option) any later version.
--
-- This grammar is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-- Lesser General Public License for more details.
--
-- You should have received a copy of the GNU Library General Public License
-- along with this library; see the file COPYING.LIB.  If not, write to
-- the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
-- Boston, MA 02110-1301, USA.


------------------------------------------------------------
-- Global declarations
------------------------------------------------------------

[:
#include <string>
#include <iostream>

namespace kdevcmg {
  class decoder;
}
:]


------------------------------------------------------------
-- Parser class members
------------------------------------------------------------

%parserclass (public declaration)
[:
  /**
   * Transform the raw input into tokens.
   * When this method returns, the parser's token stream has been filled
   * and any parse_*() method can be called.
   */
  void tokenize();

  enum problem_type {
    error,
    warning,
    info,
  };
  void report_problem( parser::problem_type type, const char* message );
  void report_problem( parser::problem_type type, std::string message );

  decoder *dec;
:]


------------------------------------------------------------
-- Enums for AST members
------------------------------------------------------------

%namespace code_item
[:
  enum item_type_enum {
    type_public_declaration,
    type_protected_declaration,
    type_private_declaration,
    type_constructor,
    type_destructor,
    type_implementation,
  };
:]


------------------------------------------------------------
-- List of defined tokens
------------------------------------------------------------

%token COLON (":"), SEMICOLON (";"), LBRACE ("{"), RBRACE ("}"),
       LPAREN ("("), RPAREN (")"), COMMA (","), HASH("#"), ASSIGN ("=") ;;

%token CODEMODEL ("CodeModel"), ROOT ("root"), DECLARATION ("declaration"),
       PUBLIC ("public"), PRIVATE ("private"), PROTECTED ("protected"),
       CONSTRUCTOR ("constructor"), DESTRUCTOR ("destructor"),
       IMPLEMENTATION ("implementation"), CODE ("code segment (\"[: ... :]\")") ;;

%token INIT ("init"), UNIQUE ("unique"), HASHED ("hashed"),
       MULTIHASHED ("multihashed"), CHILDITEM ("childitem") ;;

%token IDENTIFIER ("identifier"), TYPE ("type specification") ;;

%token INVALID ("invalid token") ;;



------------------------------------------------------------
-- Start of the actual grammar
------------------------------------------------------------

-- Overview and class declarations
------------------------------------------------------------

   0 [: dec = new decoder(token_stream); :]
   (  declaration_code:code [: _G_system.declarations = declaration_code->code; :]
    | 0
   )
   (#class_declaration=class_declaration)+
   (  implementation_code:code [: _G_system.implementation = implementation_code->code; :]
    | 0
   )
   [: delete dec; :]
-> model ;;

   code_model_declaration=code_model_declaration
 | type_declaration=type_declaration
-> class_declaration ;;


   CODEMODEL LBRACE (root_item=root_item | #item=item)* RBRACE
-> code_model_declaration ;;

   type=type
   (  ?[: (*yynode)->type->raw_type_name != "Code" :] COLON base_type=type
    | 0
   )
   LBRACE (#item=item)* RBRACE
   item_declaration=item_declaration[
     (*yynode)->type, (*yynode)->base_type, (*yynode)->item_sequence]
   non_item_type_declaration=non_item_type_declaration[
     (*yynode)->type, (*yynode)->base_type, (*yynode)->item_sequence]
     -- one of those two is set to 0 by the initialize_ast visitor
-> type_declaration ;;

   0
-> item_declaration [
     argument member node type: type;
     argument member node base_type: type;
     argument member node #item: item;
     member variable is_base_class: bool; -- to be populated after parsing
] ;;

   0
-> non_item_type_declaration [
     argument member node type: type;
     argument member node base_type: type;
     argument member node #item: item;
] ;;



-- Items
------------------------------------------------------------

   ROOT ASSIGN item=member_item
-> root_item ;;

 (
   member_item=member_item
 | code_item=code_item
 )
-> item ;;


   (  HASH [: (*yynode)->is_list = true;  :]
    | 0    [: (*yynode)->is_list = false; :]
   )
   name=identifier COLON type=type
   ( LBRACE options=options RBRACE | 0 )
   SEMICOLON
-> member_item [
     member variable is_list: bool;
] ;;

   0 [: (*yynode)->hashed = false; (*yynode)->childitem = false; :]
   (
      (  CHILDITEM
           [: (*yynode)->childitem = true; :]
       | INIT initialization_code=code
       | UNIQUE unique_condition=code
       |
         ?[: (*yynode)->multihashed_member == 0 :]
         HASHED LPAREN hashed_member=identifier RPAREN
           [: (*yynode)->hashed = true; :]
       |
         ?[: (*yynode)->hashed_member == 0 :]
         MULTIHASHED LPAREN multihashed_member=identifier RPAREN
           [: (*yynode)->hashed = true; :]
      )
      @ COMMA
    |
      0
   )
-> options [
     member variable hashed: bool;
     member variable childitem: bool;
] ;;


   (  PUBLIC DECLARATION
      [: (*yynode)->type = code_item::type_public_declaration;    :]
    | PROTECTED DECLARATION
      [: (*yynode)->type = code_item::type_protected_declaration; :]
    | PRIVATE DECLARATION
      [: (*yynode)->type = code_item::type_private_declaration;   :]
    | CONSTRUCTOR
      [: (*yynode)->type = code_item::type_constructor;           :]
    | DESTRUCTOR
      [: (*yynode)->type = code_item::type_destructor;            :]
    | IMPLEMENTATION
      [: (*yynode)->type = code_item::type_implementation;        :]
   )
   code=code
-> code_item [
     member variable type: code_item::item_type_enum;
] ;;



-- Basics
------------------------------------------------------------

   id_index:IDENTIFIER
   [: (*yynode)->identifier = dec->decode_id(id_index); :]
-> identifier [
     member variable identifier: std::string;
] ;;

   0 [: (*yynode)->is_item_type = false; :]
   (
      identifier:identifier
      [: (*yynode)->raw_type_name = identifier->identifier; :]
    |
      id_index:TYPE
      [: (*yynode)->raw_type_name = dec->decode_id(id_index);
         std::size_t lastchar = (*yynode)->raw_type_name.find_last_not_of(" \f\t\r\n");
         (*yynode)->raw_type_name = (*yynode)->raw_type_name.substr(0, lastchar+1);
      :]
   )
-> type [
     member variable raw_type_name: std::string;
     member variable type_name: std::string; -- to be populated after parsing
     member variable is_item_type: bool;     -- to be populated after parsing
] ;;

   code_index:CODE
   [: (*yynode)->code = dec->decode_id(code_index);
      (*yynode)->code = (*yynode)->code.substr(2, (*yynode)->code.length() - 4); :]
-> code [
     member variable code: std::string;
] ;;



------------------------------------------------------------
-- Additional code for the .cpp file
------------------------------------------------------------

[:
#include "kdev-cmg.h"
#include "kdev-cmg-decoder.h"
:]

