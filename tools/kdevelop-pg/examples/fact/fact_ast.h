// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef fact_AST_H_INCLUDED
#define fact_AST_H_INCLUDED

#include "kdev-pg-list.h"


namespace fact
  {

  struct assignment_statement_ast;
  struct block_statement_ast;
  struct body_ast;
  struct condition_ast;
  struct declaration_ast;
  struct expression_ast;
  struct function_definition_ast;
  struct if_statement_ast;
  struct mult_expression_ast;
  struct primary_ast;
  struct program_ast;
  struct return_statement_ast;
  struct statement_ast;
  struct variable_ast;


  struct ast_node
    {
      enum ast_node_kind_enum {
        Kind_assignment_statement = 1000,
        Kind_block_statement = 1001,
        Kind_body = 1002,
        Kind_condition = 1003,
        Kind_declaration = 1004,
        Kind_expression = 1005,
        Kind_function_definition = 1006,
        Kind_if_statement = 1007,
        Kind_mult_expression = 1008,
        Kind_primary = 1009,
        Kind_program = 1010,
        Kind_return_statement = 1011,
        Kind_statement = 1012,
        Kind_variable = 1013,
        AST_NODE_KIND_COUNT
      };

      int kind;
      std::size_t start_token;
      std::size_t end_token;
    };

  struct assignment_statement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_assignment_statement
      };

      std::size_t id;
      expression_ast *expr;
    };

  struct block_statement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_block_statement
      };

      const list_node<statement_ast *> *stmt_sequence;
    };

  struct body_ast: public ast_node
    {
      enum
      {
        KIND = Kind_body
      };

      const list_node<declaration_ast *> *decl_sequence;
      const list_node<statement_ast *> *stmt_sequence;
    };

  struct condition_ast: public ast_node
    {
      enum
      {
        KIND = Kind_condition
      };

      expression_ast *left_expr;
      expression_ast *right_expr;
    };

  struct declaration_ast: public ast_node
    {
      enum
      {
        KIND = Kind_declaration
      };

      variable_ast *var;
    };

  struct expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_expression
      };

      mult_expression_ast *left_expr;
      mult_expression_ast *right_expr;
    };

  struct function_definition_ast: public ast_node
    {
      enum
      {
        KIND = Kind_function_definition
      };

      std::size_t id;
      const list_node<std::size_t > *param_sequence;
      body_ast *body;
    };

  struct if_statement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_if_statement
      };

      condition_ast *cond;
      statement_ast *if_stmt;
      statement_ast *else_stmt;
    };

  struct mult_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_mult_expression
      };

      primary_ast *left_expr;
      primary_ast *right_expr;
    };

  struct primary_ast: public ast_node
    {
      enum
      {
        KIND = Kind_primary
      };

      std::size_t num;
      std::size_t id;
      const list_node<expression_ast *> *argument_sequence;
    };

  struct program_ast: public ast_node
    {
      enum
      {
        KIND = Kind_program
      };

      const list_node<function_definition_ast *> *fun_sequence;
    };

  struct return_statement_ast: public ast_node
                 {
                   enum
                   {
                     KIND = Kind_return_statement
                   };

                   expression_ast *expr;
                 };

  struct statement_ast: public ast_node
    {
      enum
      {
        KIND = Kind_statement
      };

      assignment_statement_ast *assign_stmt;
      if_statement_ast *if_stmt;
      block_statement_ast *block_stmt;
      return_statement_ast *return_stmt;
    };

  struct variable_ast: public ast_node
    {
      enum
      {
        KIND = Kind_variable
      };

      std::size_t id;
    };



} // end of namespace fact

#endif


