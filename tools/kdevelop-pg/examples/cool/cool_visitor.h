// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef cool_VISITOR_H_INCLUDED
#define cool_VISITOR_H_INCLUDED

#include "cool_ast.h"

namespace cool
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
      virtual void visit_additive_expression(additive_expression_ast *)
    {}
      virtual void visit_block_expression(block_expression_ast *)
      {}
      virtual void visit_case_condition(case_condition_ast *)
      {}
      virtual void visit_case_expression(case_expression_ast *)
      {}
      virtual void visit_class(class_ast *)
      {}
      virtual void visit_expression(expression_ast *)
      {}
      virtual void visit_feature(feature_ast *)
      {}
      virtual void visit_formal(formal_ast *)
      {}
      virtual void visit_if_expression(if_expression_ast *)
      {}
      virtual void visit_let_declaration(let_declaration_ast *)
      {}
      virtual void visit_let_expression(let_expression_ast *)
      {}
      virtual void visit_multiplicative_expression(multiplicative_expression_ast *)
      {}
      virtual void visit_postfix_expression(postfix_expression_ast *)
      {}
      virtual void visit_primary_expression(primary_expression_ast *)
      {}
      virtual void visit_program(program_ast *)
      {}
      virtual void visit_relational_expression(relational_expression_ast *)
      {}
      virtual void visit_unary_expression(unary_expression_ast *)
      {}
      virtual void visit_while_expression(while_expression_ast *)
      {}
    }
  ;

} // end of namespace cool

#endif


