/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_IDENTIFIER = 258,
     T_ARROW = 259,
     T_TERMINAL = 260,
     T_CODE = 261,
     T_STRING = 262,
     T_TOKEN_DECLARATION = 263,
     T_TOKEN_STREAM_DECLARATION = 264,
     T_NAMESPACE_DECLARATION = 265,
     T_PARSERCLASS_DECLARATION = 266,
     T_PUBLIC = 267,
     T_PRIVATE = 268,
     T_PROTECTED = 269,
     T_DECLARATION = 270,
     T_CONSTRUCTOR = 271,
     T_DESTRUCTOR = 272,
     T_TRY_RECOVER = 273,
     T_TRY_ROLLBACK = 274,
     T_CATCH = 275,
     T_RULE_ARGUMENTS = 276,
     T_MEMBER = 277,
     T_TEMPORARY = 278,
     T_ARGUMENT = 279,
     T_EXPORT_MACRO = 280,
     T_NODE = 281,
     T_NODE_SEQUENCE = 282,
     T_TOKEN = 283,
     T_VARIABLE = 284,
     T_EXPORT_MACRO_HEADER = 285
   };
#endif
/* Tokens.  */
#define T_IDENTIFIER 258
#define T_ARROW 259
#define T_TERMINAL 260
#define T_CODE 261
#define T_STRING 262
#define T_TOKEN_DECLARATION 263
#define T_TOKEN_STREAM_DECLARATION 264
#define T_NAMESPACE_DECLARATION 265
#define T_PARSERCLASS_DECLARATION 266
#define T_PUBLIC 267
#define T_PRIVATE 268
#define T_PROTECTED 269
#define T_DECLARATION 270
#define T_CONSTRUCTOR 271
#define T_DESTRUCTOR 272
#define T_TRY_RECOVER 273
#define T_TRY_ROLLBACK 274
#define T_CATCH 275
#define T_RULE_ARGUMENTS 276
#define T_MEMBER 277
#define T_TEMPORARY 278
#define T_ARGUMENT 279
#define T_EXPORT_MACRO 280
#define T_NODE 281
#define T_NODE_SEQUENCE 282
#define T_TOKEN 283
#define T_VARIABLE 284
#define T_EXPORT_MACRO_HEADER 285




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 33 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
{
    model::node *item;
    char* str;
    model::variable_declaration_item::declaration_type_enum declaration_type;
    model::variable_declaration_item::storage_type_enum     storage_type;
    model::variable_declaration_item::variable_type_enum    variable_type;
}
/* Line 1489 of yacc.c.  */
#line 117 "/home/andreas/KDE-work/4.0/build/kdevelop-pg/kdev-pg/kdev-pg-parser.hh"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

