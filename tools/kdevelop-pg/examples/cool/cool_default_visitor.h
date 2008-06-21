// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef cool_DEFAULT_VISITOR_H_INCLUDED
#define cool_DEFAULT_VISITOR_H_INCLUDED

#include "cool_visitor.h"

namespace cool
  {

  class default_visitor: public visitor
    {
    public:
      virtual void visit_additive_expression(additive_expression_ast *node);
      virtual void visit_block_expression(block_expression_ast *node);
      virtual void visit_case_condition(case_condition_ast *node);
      virtual void visit_case_expression(case_expression_ast *node);
      virtual void visit_class(class_ast *node);
      virtual void visit_expression(expression_ast *node);
      virtual void visit_feature(feature_ast *node);
      virtual void visit_formal(formal_ast *node);
      virtual void visit_if_expression(if_expression_ast *node);
      virtual void visit_let_declaration(let_declaration_ast *node);
      virtual void visit_let_expression(let_expression_ast *node);
      virtual void visit_multiplicative_expression(multiplicative_expression_ast *node);
      virtual void visit_postfix_expression(postfix_expression_ast *node);
      virtual void visit_primary_expression(primary_expression_ast *node);
      virtual void visit_program(program_ast *node);
      virtual void visit_relational_expression(relational_expression_ast *node);
      virtual void visit_unary_expression(unary_expression_ast *node);
      virtual void visit_while_expression(while_expression_ast *node);
    };

} // end of namespace cool

#endif


