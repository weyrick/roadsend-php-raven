// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "fact_default_visitor.h"

namespace fact
  {

  void default_visitor::visit_assignment_statement(assignment_statement_ast *node)
  {
    visit_node(node->expr);
  }

  void default_visitor::visit_block_statement(block_statement_ast *node)
  {
    if (node->stmt_sequence)
      {
        const list_node<statement_ast*> *__it = node->stmt_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_body(body_ast *node)
  {
    if (node->decl_sequence)
      {
        const list_node<declaration_ast*> *__it = node->decl_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
    if (node->stmt_sequence)
      {
        const list_node<statement_ast*> *__it = node->stmt_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_condition(condition_ast *node)
  {
    visit_node(node->left_expr);
    visit_node(node->right_expr);
  }

  void default_visitor::visit_declaration(declaration_ast *node)
  {
    visit_node(node->var);
  }

  void default_visitor::visit_expression(expression_ast *node)
  {
    visit_node(node->left_expr);
    visit_node(node->right_expr);
  }

  void default_visitor::visit_function_definition(function_definition_ast *node)
  {
    visit_node(node->body);
  }

  void default_visitor::visit_if_statement(if_statement_ast *node)
  {
    visit_node(node->cond);
    visit_node(node->if_stmt);
    visit_node(node->else_stmt);
  }

  void default_visitor::visit_mult_expression(mult_expression_ast *node)
  {
    visit_node(node->left_expr);
    visit_node(node->right_expr);
  }

  void default_visitor::visit_primary(primary_ast *node)
  {
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
  }

  void default_visitor::visit_program(program_ast *node)
  {
    if (node->fun_sequence)
      {
        const list_node<function_definition_ast*> *__it = node->fun_sequence->to_front(), *__end = __it;
        do
          {
            visit_node(__it->element);
            __it = __it->next;
          }
        while (__it != __end);
      }
  }

  void default_visitor::visit_return_statement(return_statement_ast *node)
                                    {
                                      visit_node(node->expr);
                                    }

                                    void default_visitor::visit_statement(statement_ast *node)
                                    {
                                      visit_node(node->assign_stmt);
                                      visit_node(node->if_stmt);
                                      visit_node(node->block_stmt);
                                      visit_node(node->return_stmt);
                                    }

                                    void default_visitor::visit_variable(variable_ast *)
                                    {}


                                  } // end of namespace fact


