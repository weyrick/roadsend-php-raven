// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "fact_visitor.h"

namespace fact
  {

  visitor::parser_fun_t visitor::_S_parser_table[] = {
        reinterpret_cast<parser_fun_t>(&visitor::visit_assignment_statement),
        reinterpret_cast<parser_fun_t>(&visitor::visit_block_statement),
        reinterpret_cast<parser_fun_t>(&visitor::visit_body),
        reinterpret_cast<parser_fun_t>(&visitor::visit_condition),
        reinterpret_cast<parser_fun_t>(&visitor::visit_declaration),
        reinterpret_cast<parser_fun_t>(&visitor::visit_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_function_definition),
        reinterpret_cast<parser_fun_t>(&visitor::visit_if_statement),
        reinterpret_cast<parser_fun_t>(&visitor::visit_mult_expression),
        reinterpret_cast<parser_fun_t>(&visitor::visit_primary),
        reinterpret_cast<parser_fun_t>(&visitor::visit_program),
        reinterpret_cast<parser_fun_t>(&visitor::visit_return_statement),
        reinterpret_cast<parser_fun_t>(&visitor::visit_statement),
        reinterpret_cast<parser_fun_t>(&visitor::visit_variable)
      }; // _S_parser_table[]

} // end of namespace fact


