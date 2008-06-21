// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef fact_DEFAULT_VISITOR_H_INCLUDED
#define fact_DEFAULT_VISITOR_H_INCLUDED

#include "fact_visitor.h"

namespace fact
  {

  class default_visitor: public visitor
    {
    public:
      virtual void visit_assignment_statement(assignment_statement_ast *node);
      virtual void visit_block_statement(block_statement_ast *node);
      virtual void visit_body(body_ast *node);
      virtual void visit_condition(condition_ast *node);
      virtual void visit_declaration(declaration_ast *node);
      virtual void visit_expression(expression_ast *node);
      virtual void visit_function_definition(function_definition_ast *node);
      virtual void visit_if_statement(if_statement_ast *node);
      virtual void visit_mult_expression(mult_expression_ast *node);
      virtual void visit_primary(primary_ast *node);
      virtual void visit_program(program_ast *node);
      virtual void visit_return_statement(return_statement_ast *node);
      virtual void visit_statement(statement_ast *node);
      virtual void visit_variable(variable_ast *node);
    };

} // end of namespace fact

#endif


