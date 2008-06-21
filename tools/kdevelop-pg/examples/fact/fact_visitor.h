// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef fact_VISITOR_H_INCLUDED
#define fact_VISITOR_H_INCLUDED

#include "fact_ast.h"

namespace fact
  {

  class visitor
    {
      typedef void (visitor::*parser_fun_t)(ast_node *);
      static parser_fun_t _S_parser_table[];

    public:
      virtual ~visitor()
      {}
      virtual void visit_node(ast_node *node)
      {
        if (node)
          (this->*_S_parser_table[node->kind - 1000])(node);
      }
      virtual void visit_assignment_statement(assignment_statement_ast *)
    {}
      virtual void visit_block_statement(block_statement_ast *)
      {}
      virtual void visit_body(body_ast *)
      {}
      virtual void visit_condition(condition_ast *)
      {}
      virtual void visit_declaration(declaration_ast *)
      {}
      virtual void visit_expression(expression_ast *)
      {}
      virtual void visit_function_definition(function_definition_ast *)
      {}
      virtual void visit_if_statement(if_statement_ast *)
      {}
      virtual void visit_mult_expression(mult_expression_ast *)
      {}
      virtual void visit_primary(primary_ast *)
      {}
      virtual void visit_program(program_ast *)
      {}
      virtual void visit_return_statement(return_statement_ast *)
                               {}
                               virtual void visit_statement(statement_ast *)
                               {}
                               virtual void visit_variable(variable_ast *)
                               {}
                             }
                           ;

} // end of namespace fact

#endif


