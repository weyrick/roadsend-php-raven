
// This file is meant to be specific to the framework in which the parser
// operates, and is likely to be adapted for different environments.
// Specifically, the error output might not always go to std::cerr,
// but will rather be placed as items inside some listbox.


#include "cool_parser.h"
#include "cool_lexer.h"

#include <iostream>

void print_token_environment(cool::parser* parser);


namespace cool
{

void parser::report_problem( parser::problem_type type, std::string message )
{
  report_problem( type, message.c_str() );
}

void parser::report_problem( parser::problem_type type, const char* message )
{
  if (type == error)
    std::cerr << "** ERROR: " << message << std::endl;
  else if (type == warning)
    std::cerr << "** WARNING: " << message << std::endl;
  else if (type == info)
    std::cerr << "** Info: " << message << std::endl;
}


// custom error recovery
void parser::yy_expected_token(int /*expected*/, std::size_t /*where*/, char const *name)
{
  print_token_environment(this);
  report_problem(
    parser::error,
    std::string("Expected token ``") + name
      //+ "'' instead of ``" + current_token_text
      + "''"
  );
}

void parser::yy_expected_symbol(int /*expected_symbol*/, char const *name)
{
  print_token_environment(this);
  report_problem(
    parser::error,
    std::string("Expected symbol ``") + name
      //+ "'' instead of ``" + current_token_text
      + "''"
  );
}

} // end of namespace cool

