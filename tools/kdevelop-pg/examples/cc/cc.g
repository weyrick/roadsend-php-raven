--   Copyright (C) 2005 Roberto Raggi <roberto@kdevelop.org>
--
--   This library is free software; you can redistribute it and/or
--   modify it under the terms of the GNU Library General Public
--   License as published by the Free Software Foundation; either
--   version 2 of the License, or (at your option) any later version.
--
--   This library is distributed in the hope that it will be useful,
--   but WITHOUT ANY WARRANTY; without even the implied warranty of
--   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
--   Library General Public License for more details.
--
--   You should have received a copy of the GNU Library General Public License
--   along with this library; see the file COPYING.LIB.  If not, write to
--   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
--   Boston, MA 02111-1307, USA.

------------------------------------------------------------
-- T O K E N   L I S T
------------------------------------------------------------

-- keywords:
%token CASE ("case"), DEFAULT ("default"), IF ("if"), ELSE ("else"),
       SWITCH ("switch"), WHILE ("while"), DO ("do"), FOR ("for"),
       BREAK ("break"), CONTINUE ("continue"), GOTO ("goto"),
       RETURN ("return"), TYPEDEF ("typedef"), EXTERN ("extern"),
       STATIC ("static"), AUTO ("auto"), REGISTER ("register"), VOID ("void"),
       CHAR ("char"), SHORT ("short"), INT ("int"), LONG ("long"),
       FLOAT ("float"), DOUBLE ("double"), SIGNED ("signed"),
       UNSIGNED ("unsigned"), TYPEDEF_NAME ("pre-defined type specification"),
       STRUCT ("struct"), UNION ("union"), ENUM ("enum"), CONST ("const"),
       VOLATILE ("volatile") ;;

-- seperators:
%token LPAREN ("("), RPAREN (")"), LBRACE ("{"), RBRACE ("}"), LBRACKET ("["),
       RBRACKET ("]"), DOT ("."), ARROW ("->"), COLON (":"), COMMA (","),
       SEMICOLON (";") ;;

-- operators:
%token PLUS ("+"), MINUS ("-"), STAR ("*"), DIVIDE ("/"), REMAINDER ("%"),
       TILDE ("~"), AND ("&"), OR ("|"), XOR ("^"), NOT ("!"),
       SIZEOF ("sizeof"), PLUS_PLUS ("++"), MINUS_MINUS ("--"), LSHIFT ("<<"),
       RSHIFT (">>"), AND_AND ("&&"), OR_OR ("||"), QUESTION ("?"),
       EQUAL ("="), PLUS_EQUAL ("+="), MINUS_EQUAL ("-="), STAR_EQUAL ("*="),
       DIVIDE_EQUAL ("/="), REMAINDER_EQUAL ("%="), AND_EQUAL ("&="),
       OR_EQUAL ("|="), XOR_EQUAL ("^="), LSHIFT_EQUAL ("<<="),
       RSHIFT_EQUAL (">>="), EQUAL_EQUAL ("=="), NOT_EQUAL ("!="),
       LESS ("<"), GREATER (">"), LESS_EQUAL ("<="), GREATER_EQUAL (">="),
       ELLIPSIS ("...") ;;

-- identifiers and literals:
%token IDENTIFIER ("identifier"), STRING_LITERAL ("string literal"),
       X_CONSTANT ;;

------------------------------------------------------------
-- E X T E R N A L    D E C L A R A T I O N S
------------------------------------------------------------

   external_declaration*
-> translation_unit ;;

   declaration_specifier declaration_specifier*
-> declaration_header ;;

   declaration_header variable_or_function
-> external_declaration ;;

   declarator (COMMA init_declarator @ COMMA SEMICOLON
               | SEMICOLON
               | ?[:is_fun_definition:] declaration* compound_statement
               | initializer (COMMA init_declarator)* SEMICOLON)
-> variable_or_function ;;

------------------------------------------------------------
-- E X P R E S S I O N S
------------------------------------------------------------

   IDENTIFIER
 | constant
 | STRING_LITERAL
 | LPAREN expression RPAREN
-> primary_expression ;;

   primary_expression postfix_expression_rest*
-> postfix_expression ;;

   (DOT | ARROW) IDENTIFIER
 | PLUS_PLUS
 | MINUS_MINUS
 | LPAREN (argument_expression_list | 0) RPAREN
 | LBRACKET expression RBRACKET
-> postfix_expression_rest ;;

   assignment_expression @ COMMA
-> argument_expression_list ;;

   postfix_expression
 | PLUS_PLUS unary_expression
 | MINUS_MINUS unary_expression
 | unary_operator cast_expression
 | SIZEOF (?(LPAREN type_name RPAREN) LPAREN type_name RPAREN | unary_expression)
-> unary_expression ;;

   AND
 | STAR
 | PLUS
 | MINUS
 | TILDE
 | NOT
-> unary_operator ;;

   ?(LPAREN type_name RPAREN) LPAREN type_name RPAREN cast_expression
 | unary_expression
-> cast_expression ;;

   cast_expression @ (STAR | DIVIDE | REMAINDER)
-> multiplicative_expression ;;

   multiplicative_expression @ (PLUS | MINUS)
-> additive_expression ;;

   additive_expression @ (LSHIFT | RSHIFT)
-> shift_expression ;;

   shift_expression @ (LESS | GREATER | LESS_EQUAL | GREATER_EQUAL)
-> relational_expression ;;

   relational_expression @ (EQUAL_EQUAL | NOT_EQUAL)
-> equality_expression ;;

   equality_expression @ AND
-> AND_expression ;;

   AND_expression @ XOR
-> exclusive_OR_expression ;;

   exclusive_OR_expression @ OR
-> inclusive_OR_expression ;;

   inclusive_OR_expression @ AND_AND
-> logical_AND_expression ;;

   logical_AND_expression @ OR_OR
-> logical_OR_expression ;;

   logical_OR_expression (QUESTION expression COLON conditional_expression | 0)
-> conditional_expression ;;

   conditional_expression @ assignment_operator
-> assignment_expression ;;

   EQUAL
 | STAR_EQUAL
 | DIVIDE_EQUAL
 | REMAINDER_EQUAL
 | PLUS_EQUAL
 | MINUS_EQUAL
 | LSHIFT_EQUAL
 | RSHIFT_EQUAL
 | AND_EQUAL
 | XOR_EQUAL
 | OR_EQUAL
-> assignment_operator ;;

   assignment_expression @ COMMA
-> expression ;;

   conditional_expression
-> constant_expression ;;

   X_CONSTANT
-> constant ;;

------------------------------------------------------------
-- S T A T E M E N T S
------------------------------------------------------------
   ?(IDENTIFIER COLON) IDENTIFIER COLON
 | labeled_statement
 | compound_statement
 | expression_statement
 | selection_statement
 | iteration_statement
 | jump_statement
 | SEMICOLON
-> statement ;;

   CASE constant_expression COLON statement
 | DEFAULT COLON statement
-> labeled_statement ;;

   LBRACE declaration* statement* RBRACE
-> compound_statement ;;

   expression SEMICOLON
-> expression_statement ;;

   IF LPAREN expression RPAREN statement (ELSE statement | 0)
 | SWITCH LPAREN expression RPAREN statement
-> selection_statement ;;

   WHILE LPAREN expression RPAREN statement
 | DO statement WHILE LPAREN expression RPAREN SEMICOLON
 | FOR LPAREN (expression|0) SEMICOLON (expression|0) SEMICOLON (expression|0) RPAREN statement
-> iteration_statement ;;

   GOTO IDENTIFIER SEMICOLON
 | CONTINUE SEMICOLON
 | BREAK SEMICOLON
 | RETURN (expression | 0) SEMICOLON
-> jump_statement ;;

------------------------------------------------------------
-- D E C L A R A T I O N S
------------------------------------------------------------

   declaration_specifier declaration_specifier* (init_declarator @ COMMA | 0) SEMICOLON
-> declaration ;;

   storage_class_specifier
 | type_specifier
 | type_qualifier
-> declaration_specifier ;;

   declarator (EQUAL initializer | 0)
-> init_declarator ;;

   TYPEDEF
 | EXTERN
 | STATIC
 | AUTO
 | REGISTER
-> storage_class_specifier ;;

   VOID
 | CHAR
 | SHORT
 | INT
 | LONG
 | FLOAT
 | DOUBLE
 | SIGNED
 | UNSIGNED
 | struct_or_union_specifier
 | enum_specifier
 | TYPEDEF_NAME
-> type_specifier ;;

   (STRUCT | UNION) (IDENTIFIER LBRACE struct_declaration* RBRACE
                     | LBRACE struct_declaration* RBRACE)
-> struct_or_union_specifier ;;

   specifier_qualifier specifier_qualifier specifier_qualifier* struct_declarator* SEMICOLON
-> struct_declaration ;;

   type_specifier
 | type_qualifier
-> specifier_qualifier ;;

   declarator (constant_expression | 0)
 | COLON constant_expression
-> struct_declarator ;;

   ENUM (IDENTIFIER LBRACE enumerator @ COMMA RBRACE | LBRACE enumerator @ COMMA RBRACE)
-> enum_specifier ;;

   IDENTIFIER (EQUAL constant_expression | 0)
-> enumerator ;;

   CONST
 | VOLATILE
-> type_qualifier ;;

   (pointer direct_declarator | direct_declarator) direct_declarator_rest*
-> declarator ;;

   IDENTIFIER
 | LPAREN declarator RPAREN
-> direct_declarator ;;

   LBRACKET (constant_expression | 0) RBRACKET
 | LPAREN (IDENTIFIER @ COMMA | parameter_type_list) RPAREN
-> direct_declarator_rest ;;

   STAR (type_qualifier | STAR)*
-> pointer ;;

   (parameter_declaration | ELLIPSIS) @ COMMA
-> parameter_type_list ;;

   declaration_specifier declaration_specifier* (?(declarator) declarator | abstract_declarator | 0)
-> parameter_declaration ;;

   specifier_qualifier specifier_qualifier* (abstract_declarator | 0)
-> type_name ;;

   (pointer direct_abstract_declarator | direct_abstract_declarator) direct_abstract_declarator*
-> abstract_declarator ;;

   LPAREN (abstract_declarator | parameter_type_list) RPAREN
 | LBRACKET (constant_expression | 0) RBRACKET
-> direct_abstract_declarator ;;

   assignment_expression
 | LBRACE initializer (COMMA (initializer | 0))* RBRACE
-> initializer ;;

