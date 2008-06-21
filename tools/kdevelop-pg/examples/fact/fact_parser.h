// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef fact_H_INCLUDED
#define fact_H_INCLUDED

#include "fact_ast.h"
#include "kdev-pg-memory-pool.h"
#include "kdev-pg-allocator.h"
#include "kdev-pg-token-stream.h"

namespace fact
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
        Token_ASSIGN = 1000,
        Token_COMMA = 1001,
        Token_ELSE = 1002,
        Token_EOF = 1003,
        Token_EQUAL = 1004,
        Token_FUNCTION = 1005,
        Token_IDENTIFIER = 1006,
        Token_IF = 1007,
        Token_INVALID = 1008,
        Token_LBRACE = 1009,
        Token_LPAREN = 1010,
        Token_MINUS = 1011,
        Token_NUMBER = 1012,
        Token_RBRACE = 1013,
        Token_RETURN = 1014,
        Token_RPAREN = 1015,
        Token_SEMICOLON = 1016,
        Token_STAR = 1017,
        Token_VAR = 1018,
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

      bool parse_assignment_statement(assignment_statement_ast **yynode);
      bool parse_block_statement(block_statement_ast **yynode);
      bool parse_body(body_ast **yynode);
      bool parse_condition(condition_ast **yynode);
      bool parse_declaration(declaration_ast **yynode);
      bool parse_expression(expression_ast **yynode);
      bool parse_function_definition(function_definition_ast **yynode);
      bool parse_if_statement(if_statement_ast **yynode);
      bool parse_mult_expression(mult_expression_ast **yynode);
      bool parse_primary(primary_ast **yynode);
      bool parse_program(program_ast **yynode);
      bool parse_return_statement(return_statement_ast **yynode);
      bool parse_statement(statement_ast **yynode);
      bool parse_variable(variable_ast **yynode);
    };

} // end of namespace fact

#endif


