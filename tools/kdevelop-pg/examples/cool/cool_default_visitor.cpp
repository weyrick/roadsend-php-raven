// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "cool_default_visitor.h"

namespace cool
  {

  void default_visitor::visit_additive_expression(additive_expression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<multiplicative_expression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_block_expression(block_expression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<expression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_case_condition(case_condition_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_case_expression(case_expression_ast *node)
  {
    visit_node(node->expression);
    if (node->condition_sequence)
      {
        const list_node<case_condition_ast*> *__it = node->condition_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_class(class_ast *node)
  {
    if (node->feature_sequence)
      {
        const list_node<feature_ast*> *__it = node->feature_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_expression(expression_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_feature(feature_ast *node)
  {
    if (node->formal_sequence)
      {
        const list_node<formal_ast*> *__it = node->formal_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    visit_node(node->expression);
  }

  void default_visitor::visit_formal(formal_ast *)
{}

  void default_visitor::visit_if_expression(if_expression_ast *node)
  {
    visit_node(node->condition);
    visit_node(node->true_expression);
    visit_node(node->false_expression);
  }

  void default_visitor::visit_let_declaration(let_declaration_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_let_expression(let_expression_ast *node)
  {
    if (node->declaration_sequence)
      {
        const list_node<let_declaration_ast*> *__it = node->declaration_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    visit_node(node->body_expression);
  }

  void default_visitor::visit_multiplicative_expression(multiplicative_expression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<postfix_expression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_postfix_expression(postfix_expression_ast *node)
  {
    visit_node(node->base_expression);
    if (node->arguments_sequence)
      {
        const list_node<expression_ast*> *__it = node->arguments_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_primary_expression(primary_expression_ast *node)
  {
    visit_node(node->expression);
    if (node->argument_sequence)
      {
        const list_node<expression_ast*> *__it = node->argument_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    visit_node(node->if_expression);
    visit_node(node->while_expression);
    visit_node(node->block_expression);
    visit_node(node->let_expression);
    visit_node(node->case_expression);
  }

  void default_visitor::visit_program(program_ast *node)
  {
    if (node->klass_sequence)
      {
        const list_node<class_ast*> *__it = node->klass_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_relational_expression(relational_expression_ast *node)
  {
    if (node->expression_sequence)
      {
        const list_node<additive_expression_ast*> *__it = node->expression_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_unary_expression(unary_expression_ast *node)
  {
    visit_node(node->expression);
  }

  void default_visitor::visit_while_expression(while_expression_ast *node)
  {
    visit_node(node->condition);
    visit_node(node->loop_expression);
  }


} // end of namespace cool


