// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef cool_AST_H_INCLUDED
#define cool_AST_H_INCLUDED

#include "kdev-pg-list.h"


namespace cool
  {

  struct additive_expression_ast;
  struct block_expression_ast;
  struct case_condition_ast;
  struct case_expression_ast;
  struct class_ast;
  struct expression_ast;
  struct feature_ast;
  struct formal_ast;
  struct if_expression_ast;
  struct let_declaration_ast;
  struct let_expression_ast;
  struct multiplicative_expression_ast;
  struct postfix_expression_ast;
  struct primary_expression_ast;
  struct program_ast;
  struct relational_expression_ast;
  struct unary_expression_ast;
  struct while_expression_ast;


  struct ast_node
    {
      enum ast_node_kind_enum {
        Kind_additive_expression = 1000,
        Kind_block_expression = 1001,
        Kind_case_condition = 1002,
        Kind_case_expression = 1003,
        Kind_class = 1004,
        Kind_expression = 1005,
        Kind_feature = 1006,
        Kind_formal = 1007,
        Kind_if_expression = 1008,
        Kind_let_declaration = 1009,
        Kind_let_expression = 1010,
        Kind_multiplicative_expression = 1011,
        Kind_postfix_expression = 1012,
        Kind_primary_expression = 1013,
        Kind_program = 1014,
        Kind_relational_expression = 1015,
        Kind_unary_expression = 1016,
        Kind_while_expression = 1017,
        AST_NODE_KIND_COUNT
      };

      int kind;
      std::size_t start_token;
      std::size_t end_token;
    };

  struct additive_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_additive_expression
      };

      const list_node<multiplicative_expression_ast *> *expression_sequence;
      std::size_t op;
    };

  struct block_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_block_expression
      };

      const list_node<expression_ast *> *expression_sequence;
    };

  struct case_condition_ast: public ast_node
    {
      enum
      {
        KIND = Kind_case_condition
      };

      std::size_t name;
      std::size_t type;
      expression_ast *expression;
    };

  struct case_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_case_expression
      };

      expression_ast *expression;
      const list_node<case_condition_ast *> *condition_sequence;
    };

  struct class_ast: public ast_node
    {
      enum
      {
        KIND = Kind_class
      };

      std::size_t type;
      std::size_t base_type;
      const list_node<feature_ast *> *feature_sequence;
    };

  struct expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_expression
      };

      relational_expression_ast *expression;
    };

  struct feature_ast: public ast_node
    {
      enum
      {
        KIND = Kind_feature
      };

      std::size_t name;
      const list_node<formal_ast *> *formal_sequence;
      std::size_t type;
      expression_ast *expression;
    };

  struct formal_ast: public ast_node
    {
      enum
      {
        KIND = Kind_formal
      };

      std::size_t name;
      std::size_t type;
    };

  struct if_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_if_expression
      };

      expression_ast *condition;
      expression_ast *true_expression;
      expression_ast *false_expression;
    };

  struct let_declaration_ast: public ast_node
    {
      enum
      {
        KIND = Kind_let_declaration
      };

      std::size_t name;
      std::size_t type;
      expression_ast *expression;
    };

  struct let_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_let_expression
      };

      const list_node<let_declaration_ast *> *declaration_sequence;
      expression_ast *body_expression;
    };

  struct multiplicative_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_multiplicative_expression
      };

      const list_node<postfix_expression_ast *> *expression_sequence;
      std::size_t op;
    };

  struct postfix_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_postfix_expression
      };

      unary_expression_ast *base_expression;
      std::size_t at_type;
      std::size_t name;
      const list_node<expression_ast *> *arguments_sequence;
    };

  struct primary_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_primary_expression
      };

      std::size_t name;
      expression_ast *expression;
      const list_node<expression_ast *> *argument_sequence;
      std::size_t variable;
      std::size_t integer_literal;
      std::size_t string_literal;
      std::size_t true_literal;
      std::size_t false_literal;
      std::size_t new_type;
      if_expression_ast *if_expression;
      while_expression_ast *while_expression;
      block_expression_ast *block_expression;
      let_expression_ast *let_expression;
      case_expression_ast *case_expression;
    };

  struct program_ast: public ast_node
    {
      enum
      {
        KIND = Kind_program
      };

      const list_node<class_ast *> *klass_sequence;
    };

  struct relational_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_relational_expression
      };

      const list_node<additive_expression_ast *> *expression_sequence;
      std::size_t op;
    };

  struct unary_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_unary_expression
      };

      std::size_t op;
      primary_expression_ast *expression;
    };

  struct while_expression_ast: public ast_node
    {
      enum
      {
        KIND = Kind_while_expression
      };

      expression_ast *condition;
      expression_ast *loop_expression;
    };



} // end of namespace cool

#endif


