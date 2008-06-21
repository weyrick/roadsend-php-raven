// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "cool_visitor.h"

namespace cool
  {

  visitor::parser_fun_t visitor::_S_parser_table[] = {
        reinterpret_cast<parser_fun_t>(&visitor::visit_additive_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_block_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_case_condition),
        reinterpret_cast<parser_fun_t>(&visitor::visit_case_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_class),
        reinterpret_cast<parser_fun_t>(&visitor::visit_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_feature),
        reinterpret_cast<parser_fun_t>(&visitor::visit_formal),
        reinterpret_cast<parser_fun_t>(&visitor::visit_if_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_let_declaration),
        reinterpret_cast<parser_fun_t>(&visitor::visit_let_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_multiplicative_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_postfix_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_primary_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_program),
        reinterpret_cast<parser_fun_t>(&visitor::visit_relational_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_unary_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_while_expression)
      }; // _S_parser_table[]

} // end of namespace cool


