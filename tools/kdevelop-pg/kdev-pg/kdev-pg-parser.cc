/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 2 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 187 of yacc.c.  */
#line 195 "/home/andreas/KDE-work/4.0/build/kdevelop-pg/kdev-pg/kdev-pg-parser.cc"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 208 "/home/andreas/KDE-work/4.0/build/kdevelop-pg/kdev-pg/kdev-pg-parser.cc"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   164

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  69
/* YYNRULES -- Number of states.  */
#define YYNSTATES  146

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    36,     2,     2,     2,     2,
      32,    33,    41,    40,    34,     2,    37,     2,    35,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,     7,
       2,    38,     2,    43,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    46,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    44,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,    10,    12,    15,    18,    22,    26,
      28,    31,    34,    40,    46,    52,    57,    62,    66,    68,
      73,    77,    84,    87,    91,    94,    96,   100,   102,   104,
     108,   113,   116,   118,   123,   132,   133,   135,   137,   141,
     143,   145,   148,   151,   153,   155,   159,   162,   164,   167,
     169,   173,   175,   179,   187,   195,   200,   201,   203,   205,
     208,   215,   221,   229,   236,   237,   239,   241,   243,   245
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      48,     0,    -1,    -1,    68,    49,    50,    55,    68,    -1,
      51,    -1,    50,    51,    -1,    12,    52,    -1,     9,    54,
       7,    -1,    10,     3,     7,    -1,    53,    -1,    26,     8,
      -1,    31,     8,    -1,    32,    13,    16,    33,     6,    -1,
      32,    15,    16,    33,     6,    -1,    32,    14,    16,    33,
       6,    -1,    32,    17,    33,     6,    -1,    32,    18,    33,
       6,    -1,    11,     3,     6,    -1,     5,    -1,     5,    32,
       8,    33,    -1,    54,    34,     5,    -1,    54,    34,     5,
      32,     8,    33,    -1,    67,     7,    -1,    55,    67,     7,
      -1,    55,    53,    -1,    35,    -1,    32,    66,    33,    -1,
      58,    -1,    57,    -1,    60,    61,    57,    -1,    36,    60,
      61,    57,    -1,     3,    59,    -1,     5,    -1,    19,    32,
      66,    33,    -1,    20,    32,    66,    33,    21,    32,    66,
      33,    -1,    -1,    22,    -1,     3,    -1,     3,    37,     3,
      -1,    38,    -1,    39,    -1,    56,    40,    -1,    56,    41,
      -1,    56,    -1,    62,    -1,    63,    42,    56,    -1,    63,
       6,    -1,    63,    -1,    64,    63,    -1,    64,    -1,    43,
       6,    64,    -1,    65,    -1,    66,    44,    65,    -1,    66,
       4,     3,     6,    45,    69,    46,    -1,    66,     4,     3,
      45,    69,    46,    68,    -1,    66,     4,     3,    68,    -1,
      -1,     6,    -1,    70,    -1,    69,    70,    -1,    71,    72,
      73,     3,    39,     3,    -1,    71,    72,    29,     3,     7,
      -1,    71,    72,    73,    36,     3,    39,     3,    -1,    71,
      72,    29,    36,     3,     7,    -1,    -1,    25,    -1,    23,
      -1,    24,    -1,    27,    -1,    30,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    60,    60,    60,    67,    68,    72,    74,    75,    77,
      78,    80,    85,    87,    89,    91,    93,    98,   103,   104,
     105,   106,   111,   112,   113,   117,   118,   119,   120,   121,
     122,   126,   127,   131,   136,   143,   144,   148,   149,   158,
     159,   163,   164,   165,   170,   171,   176,   180,   181,   185,
     186,   190,   191,   195,   200,   205,   210,   211,   215,   216,
     228,   230,   232,   234,   239,   240,   244,   245,   249,   250
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IDENTIFIER", "T_ARROW", "T_TERMINAL",
  "T_CODE", "';'", "T_STRING", "T_TOKEN_DECLARATION",
  "T_TOKEN_STREAM_DECLARATION", "T_NAMESPACE_DECLARATION",
  "T_PARSERCLASS_DECLARATION", "T_PUBLIC", "T_PRIVATE", "T_PROTECTED",
  "T_DECLARATION", "T_CONSTRUCTOR", "T_DESTRUCTOR", "T_TRY_RECOVER",
  "T_TRY_ROLLBACK", "T_CATCH", "T_RULE_ARGUMENTS", "T_MEMBER",
  "T_TEMPORARY", "T_ARGUMENT", "T_EXPORT_MACRO", "T_NODE",
  "T_NODE_SEQUENCE", "T_TOKEN", "T_VARIABLE", "T_EXPORT_MACRO_HEADER",
  "'('", "')'", "','", "'0'", "'#'", "'.'", "'='", "':'", "'+'", "'*'",
  "'@'", "'?'", "'|'", "'['", "']'", "$accept", "system", "@1",
  "declarations", "declaration", "member_declaration_rest",
  "namespace_declaration", "declared_tokens", "rules", "primary_item",
  "primary_atom", "try_item", "rule_arguments_opt", "name", "scope",
  "unary_item", "postfix_item", "item_sequence", "conditional_item",
  "option_item", "item", "code_opt", "variable_declarations",
  "variable_declaration", "declaration_type_opt", "storage_type",
  "variable_type", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,    59,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,    40,    41,    44,    48,    35,    46,    61,    58,
      43,    42,    64,    63,   124,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    49,    48,    50,    50,    51,    51,    51,    51,
      51,    51,    52,    52,    52,    52,    52,    53,    54,    54,
      54,    54,    55,    55,    55,    56,    56,    56,    56,    56,
      56,    57,    57,    58,    58,    59,    59,    60,    60,    61,
      61,    62,    62,    62,    63,    63,    63,    64,    64,    65,
      65,    66,    66,    67,    67,    67,    68,    68,    69,    69,
      70,    70,    70,    70,    71,    71,    72,    72,    73,    73
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     5,     1,     2,     2,     3,     3,     1,
       2,     2,     5,     5,     5,     4,     4,     3,     1,     4,
       3,     6,     2,     3,     2,     1,     3,     1,     1,     3,
       4,     2,     1,     4,     8,     0,     1,     1,     3,     1,
       1,     2,     2,     1,     1,     3,     2,     1,     2,     1,
       3,     1,     3,     7,     7,     4,     0,     1,     1,     2,
       6,     5,     7,     6,     0,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      56,    57,     0,     2,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     9,    18,     0,     0,     0,     0,
       6,    10,    11,    35,    32,     0,     0,     0,    25,     0,
       0,     5,    56,    43,    28,    27,     0,    44,    47,    49,
      51,     0,     0,     0,     7,     0,     8,    17,     0,     0,
       0,     0,     0,    36,     0,    31,     0,     0,     0,    37,
       0,     0,    24,     0,     3,    41,    42,    39,    40,     0,
      46,     0,    48,     0,     0,    22,     0,    20,     0,     0,
       0,     0,     0,    38,     0,     0,    26,     0,    50,    23,
      35,    29,    45,    56,    52,    19,     0,     0,     0,     0,
      15,    16,    33,     0,    30,    57,    64,    55,     0,    12,
      14,    13,     0,    64,    65,    64,    58,     0,    21,     0,
      64,    56,    59,    66,    67,     0,     0,    53,    54,    68,
       0,    69,     0,    34,     0,     0,     0,     0,    61,     0,
       0,     0,    63,    60,     0,    62
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    12,    13,    20,    14,    16,    32,    33,
      34,    35,    55,    36,    69,    37,    38,    39,    40,    41,
      42,     3,   115,   116,   117,   125,   132
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -64
static const yytype_int16 yypact[] =
{
       2,   -64,    14,   -64,   -64,    69,    54,    63,    86,    36,
      93,   100,     0,   -64,   -64,    40,    11,   104,   106,    92,
     -64,   -64,   -64,    65,   -64,    81,    82,    41,   -64,   112,
     110,   -64,    22,   -19,   -64,   -64,    58,   -64,    -2,    50,
     -64,     3,   111,   109,   -64,   114,   -64,   -64,   105,   107,
     108,    87,    89,   -64,   122,   -64,    41,    41,   -10,    90,
      58,    50,   -64,   119,   -64,   -64,   -64,   -64,   -64,    34,
     -64,    50,    -2,   125,    41,   -64,    96,    98,    99,   101,
     102,   127,   130,   -64,    23,    30,   -64,    34,    50,   -64,
     115,   -64,   -64,     7,   -64,   -64,   123,   132,   133,   134,
     -64,   -64,   -64,   120,   -64,    97,   118,   -64,   113,   -64,
     -64,   -64,   116,   118,   -64,    -8,   -64,    75,   -64,    41,
      25,     2,   -64,   -64,   -64,    64,    31,   -64,   -64,   -64,
      12,   -64,    13,   -64,   137,   142,   117,   144,   -64,   143,
     146,   121,   -64,   -64,   148,   -64
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -64,   -64,   -64,   -64,   140,   -64,   126,   -64,   -64,    83,
     -63,   -64,   -64,   124,    95,   -64,   -37,   103,    85,   -27,
     129,   -31,    44,   -32,   -64,   -64,   -64
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -38
static const yytype_int16 yytable[] =
{
      58,    64,    72,    23,    70,    24,    91,    73,     1,     6,
       7,     8,     9,   105,     4,   134,   136,   114,    44,    25,
      26,    65,    66,    86,   104,    23,    10,    24,     1,    84,
      85,    11,    27,     8,    74,    28,    29,    90,   121,    24,
      71,    25,    26,    30,    23,    45,    24,    74,   135,   137,
     114,    72,   106,    23,    27,    24,   102,    28,    29,    15,
      25,    26,   107,   103,   133,    30,    17,    74,    19,    25,
      26,   127,    43,    27,    74,    74,    28,    29,     6,     7,
       8,     9,    27,   122,    30,    28,    29,    53,   122,    18,
     128,   129,   126,   130,   131,    10,    67,    68,   123,   124,
      11,    21,    54,   -37,   -37,    48,    49,    50,    22,    51,
      52,    46,    47,    56,    57,    59,    61,    76,    75,    77,
      81,    78,    82,    79,    80,    83,    89,    54,    93,    95,
      96,   108,    97,   100,    98,    99,   101,    53,   109,   110,
     111,   112,   113,   114,   138,   139,   118,   141,   119,   143,
     142,   145,    31,    60,    92,    87,   140,   120,    62,    94,
     144,    63,     0,     0,    88
};

static const yytype_int8 yycheck[] =
{
      27,    32,    39,     3,     6,     5,    69,     4,     6,     9,
      10,    11,    12,     6,     0,     3,     3,    25,     7,    19,
      20,    40,    41,    33,    87,     3,    26,     5,     6,    56,
      57,    31,    32,    11,    44,    35,    36,     3,    46,     5,
      42,    19,    20,    43,     3,    34,     5,    44,    36,    36,
      25,    88,    45,     3,    32,     5,    33,    35,    36,     5,
      19,    20,    93,    33,    33,    43,     3,    44,    32,    19,
      20,    46,    32,    32,    44,    44,    35,    36,     9,    10,
      11,    12,    32,   115,    43,    35,    36,    22,   120,     3,
     121,    27,   119,    29,    30,    26,    38,    39,    23,    24,
      31,     8,    37,    38,    39,    13,    14,    15,     8,    17,
      18,     7,     6,    32,    32,     3,     6,     8,     7,     5,
      33,    16,    33,    16,    16,     3,     7,    37,     3,    33,
      32,     8,    33,     6,    33,    33,     6,    22,     6,     6,
       6,    21,    45,    25,     7,     3,    33,     3,    32,     3,
       7,     3,    12,    29,    71,    60,    39,   113,    32,    74,
      39,    32,    -1,    -1,    61
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,    48,    68,     0,    49,     9,    10,    11,    12,
      26,    31,    50,    51,    53,     5,    54,     3,     3,    32,
      52,     8,     8,     3,     5,    19,    20,    32,    35,    36,
      43,    51,    55,    56,    57,    58,    60,    62,    63,    64,
      65,    66,    67,    32,     7,    34,     7,     6,    13,    14,
      15,    17,    18,    22,    37,    59,    32,    32,    66,     3,
      60,     6,    53,    67,    68,    40,    41,    38,    39,    61,
       6,    42,    63,     4,    44,     7,     8,     5,    16,    16,
      16,    33,    33,     3,    66,    66,    33,    61,    64,     7,
       3,    57,    56,     3,    65,    33,    32,    33,    33,    33,
       6,     6,    33,    33,    57,     6,    45,    68,     8,     6,
       6,     6,    21,    45,    25,    69,    70,    71,    33,    32,
      69,    46,    70,    23,    24,    72,    66,    46,    68,    27,
      29,    30,    73,    33,     3,    36,     3,    36,     7,     3,
      39,     3,     7,     3,    39,     3
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 60 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.decl = (yyvsp[(1) - (1)].str); ;}
    break;

  case 3:
#line 63 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.bits = (yyvsp[(5) - (5)].str); ;}
    break;

  case 6:
#line 73 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.push_parserclass_member((yyvsp[(2) - (2)].item)); ;}
    break;

  case 8:
#line 76 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.token_stream = (yyvsp[(2) - (3)].str);           ;}
    break;

  case 10:
#line 79 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.export_macro = (yyvsp[(2) - (2)].str);           ;}
    break;

  case 11:
#line 81 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.export_macro_header = (yyvsp[(2) - (2)].str);   ;}
    break;

  case 12:
#line 86 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::member(settings::member_item::public_declaration, (yyvsp[(5) - (5)].str));    ;}
    break;

  case 13:
#line 88 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::member(settings::member_item::protected_declaration, (yyvsp[(5) - (5)].str)); ;}
    break;

  case 14:
#line 90 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::member(settings::member_item::private_declaration, (yyvsp[(5) - (5)].str));   ;}
    break;

  case 15:
#line 92 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::member(settings::member_item::constructor_code, (yyvsp[(4) - (4)].str));      ;}
    break;

  case 16:
#line 94 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::member(settings::member_item::destructor_code, (yyvsp[(4) - (4)].str));       ;}
    break;

  case 17:
#line 99 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.push_namespace((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].str)); ;}
    break;

  case 18:
#line 103 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.push_terminal((yyvsp[(1) - (1)].str),(yyvsp[(1) - (1)].str)); ;}
    break;

  case 19:
#line 104 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.push_terminal((yyvsp[(1) - (4)].str),(yyvsp[(3) - (4)].str)); ;}
    break;

  case 20:
#line 105 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.push_terminal((yyvsp[(3) - (3)].str),(yyvsp[(3) - (3)].str)); ;}
    break;

  case 21:
#line 107 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.push_terminal((yyvsp[(3) - (6)].str),(yyvsp[(5) - (6)].str)); ;}
    break;

  case 22:
#line 111 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.push_rule((yyvsp[(1) - (2)].item)); ;}
    break;

  case 23:
#line 112 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { _G_system.push_rule((yyvsp[(2) - (3)].item)); ;}
    break;

  case 25:
#line 117 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = _G_system.zero(); ;}
    break;

  case 26:
#line 118 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = (yyvsp[(2) - (3)].item); ;}
    break;

  case 27:
#line 119 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); ;}
    break;

  case 28:
#line 120 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); ;}
    break;

  case 29:
#line 121 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::annotation((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].item), false, (yyvsp[(2) - (3)].storage_type)); ;}
    break;

  case 30:
#line 122 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::annotation((yyvsp[(2) - (4)].str), (yyvsp[(4) - (4)].item), true, (yyvsp[(3) - (4)].storage_type));  ;}
    break;

  case 31:
#line 126 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::nonterminal(_G_system.push_symbol((yyvsp[(1) - (2)].str)), (yyvsp[(2) - (2)].str)); ;}
    break;

  case 32:
#line 127 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = _G_system.terminal((yyvsp[(1) - (1)].str)); ;}
    break;

  case 33:
#line 132 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    {
          _G_system.need_state_management = true;
          (yyval.item) = pg::try_catch((yyvsp[(3) - (4)].item), 0);
        ;}
    break;

  case 34:
#line 137 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    {
          _G_system.need_state_management = true;
          (yyval.item) = pg::try_catch((yyvsp[(3) - (8)].item), (yyvsp[(7) - (8)].item));
        ;}
    break;

  case 35:
#line 143 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.str) = ""; ;}
    break;

  case 36:
#line 144 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 37:
#line 148 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 38:
#line 150 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    {
          (yyval.str) = (yyvsp[(3) - (3)].str);
          fprintf(stderr, "** WARNING support for scoped name"
                          " ``%s.%s'' not implemented\n", (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str));
        ;}
    break;

  case 39:
#line 158 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.storage_type) = model::variable_declaration_item::storage_ast_member; ;}
    break;

  case 40:
#line 159 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.storage_type) = model::variable_declaration_item::storage_temporary;  ;}
    break;

  case 41:
#line 163 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::plus((yyvsp[(1) - (2)].item)); ;}
    break;

  case 42:
#line 164 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::star((yyvsp[(1) - (2)].item)); ;}
    break;

  case 43:
#line 165 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); ;}
    break;

  case 44:
#line 170 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); ;}
    break;

  case 45:
#line 172 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    {
          clone_tree cl;
          (yyval.item) = pg::cons((yyvsp[(1) - (3)].item), pg::star(pg::cons(cl.clone((yyvsp[(3) - (3)].item)), cl.clone((yyvsp[(1) - (3)].item)))));
        ;}
    break;

  case 46:
#line 176 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::action((yyvsp[(1) - (2)].item), (yyvsp[(2) - (2)].str)); ;}
    break;

  case 47:
#line 180 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); ;}
    break;

  case 48:
#line 181 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::cons((yyvsp[(1) - (2)].item), (yyvsp[(2) - (2)].item)); ;}
    break;

  case 49:
#line 185 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); ;}
    break;

  case 50:
#line 186 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::condition((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].item)); ;}
    break;

  case 51:
#line 190 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); ;}
    break;

  case 52:
#line 191 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::alternative((yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item)); ;}
    break;

  case 53:
#line 196 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    {
          (yyval.item) = pg::evolve((yyvsp[(1) - (7)].item), _G_system.push_symbol((yyvsp[(3) - (7)].str)),
                          (model::variable_declaration_item*) (yyvsp[(6) - (7)].item), (yyvsp[(4) - (7)].str));
        ;}
    break;

  case 54:
#line 201 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    {
          (yyval.item) = pg::evolve((yyvsp[(1) - (7)].item), _G_system.push_symbol((yyvsp[(3) - (7)].str)),
                          (model::variable_declaration_item*) (yyvsp[(5) - (7)].item), (yyvsp[(7) - (7)].str));
        ;}
    break;

  case 55:
#line 206 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::evolve((yyvsp[(1) - (4)].item), _G_system.push_symbol((yyvsp[(3) - (4)].str)), 0, (yyvsp[(4) - (4)].str)); ;}
    break;

  case 56:
#line 210 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.str) = ""; ;}
    break;

  case 57:
#line 211 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 58:
#line 215 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); ;}
    break;

  case 59:
#line 217 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    {
          model::variable_declaration_item *last = (model::variable_declaration_item*) (yyvsp[(1) - (2)].item);
          while (last->_M_next != 0) {
            last = last->_M_next;
          }
          last->_M_next = (model::variable_declaration_item*) (yyvsp[(2) - (2)].item);
          (yyval.item) = (yyvsp[(1) - (2)].item);
        ;}
    break;

  case 60:
#line 229 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::variable_declaration((yyvsp[(1) - (6)].declaration_type), (yyvsp[(2) - (6)].storage_type), (yyvsp[(3) - (6)].variable_type), false, (yyvsp[(4) - (6)].str), (yyvsp[(6) - (6)].str)); ;}
    break;

  case 61:
#line 231 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::variable_declaration((yyvsp[(1) - (5)].declaration_type), (yyvsp[(2) - (5)].storage_type), model::variable_declaration_item::type_token, false, (yyvsp[(4) - (5)].str), ""); ;}
    break;

  case 62:
#line 233 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::variable_declaration((yyvsp[(1) - (7)].declaration_type), (yyvsp[(2) - (7)].storage_type), (yyvsp[(3) - (7)].variable_type), true, (yyvsp[(5) - (7)].str), (yyvsp[(7) - (7)].str)); ;}
    break;

  case 63:
#line 235 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.item) = pg::variable_declaration((yyvsp[(1) - (6)].declaration_type), (yyvsp[(2) - (6)].storage_type), model::variable_declaration_item::type_token, true, (yyvsp[(5) - (6)].str), ""); ;}
    break;

  case 64:
#line 239 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.declaration_type) = model::variable_declaration_item::declaration_local;     ;}
    break;

  case 65:
#line 240 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.declaration_type) = model::variable_declaration_item::declaration_argument;  ;}
    break;

  case 66:
#line 244 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.storage_type) = model::variable_declaration_item::storage_ast_member;    ;}
    break;

  case 67:
#line 245 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.storage_type) = model::variable_declaration_item::storage_temporary;     ;}
    break;

  case 68:
#line 249 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.variable_type) = model::variable_declaration_item::type_node;             ;}
    break;

  case 69:
#line 250 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"
    { (yyval.variable_type) = model::variable_declaration_item::type_variable;         ;}
    break;


/* Line 1267 of yacc.c.  */
#line 1876 "/home/andreas/KDE-work/4.0/build/kdevelop-pg/kdev-pg/kdev-pg-parser.cc"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 253 "/home/andreas/KDE-work/4.0/kdevelop-pg/kdev-pg/kdev-pg-parser.yy"


