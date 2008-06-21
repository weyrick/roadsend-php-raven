// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef cool_H_INCLUDED
#define cool_H_INCLUDED

#include "cool_ast.h"
#include "kdev-pg-memory-pool.h"
#include "kdev-pg-allocator.h"
#include "kdev-pg-token-stream.h"

namespace cool
  {

  class parser
    {
    public:
      typedef kdev_pg_token_stream token_stream_type;
      typedef kdev_pg_token_stream::token_type token_type;
      kdev_pg_token_stream *token_stream;
      int yytoken;

      inline token_type LA(std::size_t k = 1) const
        {
          return token_stream->token(token_stream->index() - 1 + k - 1);
        }
      inline int yylex()
      {
        return (yytoken = token_stream->next_token());
      }
      inline void rewind(std::size_t index)
      {
        token_stream->rewind(index);
        yylex();
      }

      // token stream
      void set_token_stream(kdev_pg_token_stream *s)
      {
        token_stream = s;
      }

      // error handling
      void yy_expected_symbol(int kind, char const *name);
      void yy_expected_token(int kind, std::size_t token, char const *name);

      bool yy_block_errors;
      inline bool block_errors(bool block)
      {
        bool previous = yy_block_errors;
        yy_block_errors = block;
        return previous;
      }

      // memory pool
      typedef kdev_pg_memory_pool memory_pool_type;

      kdev_pg_memory_pool *memory_pool;
      void set_memory_pool(kdev_pg_memory_pool *p)
      {
        memory_pool = p;
      }
      template <class T>
      inline T *create()
      {
        T *node = new (memory_pool->allocate(sizeof(T))) T();
        node->kind = T::KIND;
        return node;
      }

      enum token_type_enum
      {
        Token_ARROW_LEFT = 1000,
        Token_ARROW_RIGHT = 1001,
        Token_AT = 1002,
        Token_CASE = 1003,
        Token_CLASS = 1004,
        Token_COLON = 1005,
        Token_COMMA = 1006,
        Token_DOT = 1007,
        Token_ELSE = 1008,
        Token_EOF = 1009,
        Token_EQUAL = 1010,
        Token_ESAC = 1011,
        Token_FALSE = 1012,
        Token_FI = 1013,
        Token_IDENTIFIER = 1014,
        Token_IF = 1015,
        Token_IN = 1016,
        Token_INHERITS = 1017,
        Token_INTEGER = 1018,
        Token_INVALID = 1019,
        Token_ISVOID = 1020,
        Token_LBRACE = 1021,
        Token_LESS = 1022,
        Token_LESS_EQUAL = 1023,
        Token_LET = 1024,
        Token_LOOP = 1025,
        Token_LPAREN = 1026,
        Token_MINUS = 1027,
        Token_NEW = 1028,
        Token_NOT = 1029,
        Token_OF = 1030,
        Token_PLUS = 1031,
        Token_POOL = 1032,
        Token_RBRACE = 1033,
        Token_RPAREN = 1034,
        Token_SEMICOLON = 1035,
        Token_SLASH = 1036,
        Token_STAR = 1037,
        Token_STRING = 1038,
        Token_THEN = 1039,
        Token_TILDE = 1040,
        Token_TRUE = 1041,
        Token_TYPE = 1042,
        Token_WHILE = 1043,
        token_type_size
      }; // token_type_enum

      // user defined declarations:
    public:

      /**
       * Transform the raw input into tokens.
       * When this method returns, the parser's token stream has been filled
       * and any parse_*() method can be called.
       */
      void tokenize( char *contents );

      enum problem_type {
        error,
        warning,
        info
      };
      void report_problem( parser::problem_type type, const char* message );
      void report_problem( parser::problem_type type, std::string message );


    public:
      parser()
      {
        memory_pool = 0;
        token_stream = 0;
        yytoken = Token_EOF;
        yy_block_errors = false;
      }

      virtual ~parser()
      {}

      bool parse_additive_expression(additive_expression_ast **yynode);
      bool parse_block_expression(block_expression_ast **yynode);
      bool parse_case_condition(case_condition_ast **yynode);
      bool parse_case_expression(case_expression_ast **yynode);
      bool parse_class(class_ast **yynode);
      bool parse_expression(expression_ast **yynode);
      bool parse_feature(feature_ast **yynode);
      bool parse_formal(formal_ast **yynode);
      bool parse_if_expression(if_expression_ast **yynode);
      bool parse_let_declaration(let_declaration_ast **yynode);
      bool parse_let_expression(let_expression_ast **yynode);
      bool parse_multiplicative_expression(multiplicative_expression_ast **yynode);
      bool parse_postfix_expression(postfix_expression_ast **yynode);
      bool parse_primary_expression(primary_expression_ast **yynode);
      bool parse_program(program_ast **yynode);
      bool parse_relational_expression(relational_expression_ast **yynode);
      bool parse_unary_expression(unary_expression_ast **yynode);
      bool parse_while_expression(while_expression_ast **yynode);
    };

} // end of namespace cool

#endif


