// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "fact_parser.h"


#include "fact_lexer.h"


namespace fact
  {

  void parser::tokenize( char *contents )
  {
    Lexer lexer( this, contents );

    int kind = parser::Token_EOF;
    do
      {
        kind = lexer.yylex();
        //std::cerr << lexer.YYText() << std::endl; //" "; // debug output

        if ( !kind ) // when the lexer returns 0, the end of file is reached
          kind = parser::Token_EOF;

        parser::token_type &t = this->token_stream->next();
        t.kind = kind;
        t.begin = lexer.tokenBegin();
        t.end = lexer.tokenEnd();
        t.text = contents;
      }
    while ( kind != parser::Token_EOF );

    this->yylex(); // produce the look ahead token
  }

} // end of namespace fact


namespace fact
  {

  bool parser::parse_assignment_statement(assignment_statement_ast **yynode)
  {
    *yynode = create<assignment_statement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IDENTIFIER)
      {
        if (yytoken != Token_IDENTIFIER)
          {
            yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
            return false;
          }
        (*yynode)->id = token_stream->index() - 1;
        yylex();

        if (yytoken != Token_ASSIGN)
          {
            yy_expected_token(yytoken, Token_ASSIGN, "=");
            return false;
          }
        yylex();

        expression_ast *__node_0 = 0;
        if (!parse_expression(&__node_0))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->expr = __node_0;

        if (yytoken != Token_SEMICOLON)
          {
            yy_expected_token(yytoken, Token_SEMICOLON, ";");
            return false;
          }
        yylex();

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_block_statement(block_statement_ast **yynode)
  {
    *yynode = create<block_statement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LBRACE)
      {
        if (yytoken != Token_LBRACE)
          {
            yy_expected_token(yytoken, Token_LBRACE, "{");
            return false;
          }
        yylex();

        while (yytoken == Token_IF
               || yytoken == Token_RETURN
               || yytoken == Token_LBRACE
               || yytoken == Token_IDENTIFIER)
          {
            statement_ast *__node_1 = 0;
            if (!parse_statement(&__node_1))
              {
                yy_expected_symbol(ast_node::Kind_statement, "statement");
                return false;
              }
            (*yynode)->stmt_sequence = snoc((*yynode)->stmt_sequence, __node_1, memory_pool);

          }
        if (yytoken != Token_RBRACE)
          {
            yy_expected_token(yytoken, Token_RBRACE, "}");
            return false;
          }
        yylex();

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_body(body_ast **yynode)
  {
    *yynode = create<body_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LBRACE)
      {
        if (yytoken != Token_LBRACE)
          {
            yy_expected_token(yytoken, Token_LBRACE, "{");
            return false;
          }
        yylex();

        while (yytoken == Token_VAR)
          {
            declaration_ast *__node_2 = 0;
            if (!parse_declaration(&__node_2))
              {
                yy_expected_symbol(ast_node::Kind_declaration, "declaration");
                return false;
              }
            (*yynode)->decl_sequence = snoc((*yynode)->decl_sequence, __node_2, memory_pool);

          }
        while (yytoken == Token_IF
               || yytoken == Token_RETURN
               || yytoken == Token_LBRACE
               || yytoken == Token_IDENTIFIER)
          {
            statement_ast *__node_3 = 0;
            if (!parse_statement(&__node_3))
              {
                yy_expected_symbol(ast_node::Kind_statement, "statement");
                return false;
              }
            (*yynode)->stmt_sequence = snoc((*yynode)->stmt_sequence, __node_3, memory_pool);

          }
        if (yytoken != Token_RBRACE)
          {
            yy_expected_token(yytoken, Token_RBRACE, "}");
            return false;
          }
        yylex();

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_condition(condition_ast **yynode)
  {
    *yynode = create<condition_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IDENTIFIER
        || yytoken == Token_NUMBER)
      {
        expression_ast *__node_4 = 0;
        if (!parse_expression(&__node_4))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->left_expr = __node_4;

        if (yytoken != Token_EQUAL)
          {
            yy_expected_token(yytoken, Token_EQUAL, "==");
            return false;
          }
        yylex();

        expression_ast *__node_5 = 0;
        if (!parse_expression(&__node_5))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->right_expr = __node_5;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_declaration(declaration_ast **yynode)
  {
    *yynode = create<declaration_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_VAR)
      {
        if (yytoken != Token_VAR)
          {
            yy_expected_token(yytoken, Token_VAR, "var");
            return false;
          }
        yylex();

        variable_ast *__node_6 = 0;
        if (!parse_variable(&__node_6))
          {
            yy_expected_symbol(ast_node::Kind_variable, "variable");
            return false;
          }
        (*yynode)->var = __node_6;

        while (yytoken == Token_COMMA)
          {
            if (yytoken != Token_COMMA)
              {
                yy_expected_token(yytoken, Token_COMMA, ",");
                return false;
              }
            yylex();

            variable_ast *__node_7 = 0;
            if (!parse_variable(&__node_7))
              {
                yy_expected_symbol(ast_node::Kind_variable, "variable");
                return false;
              }
            (*yynode)->var = __node_7;

          }
        if (yytoken != Token_SEMICOLON)
          {
            yy_expected_token(yytoken, Token_SEMICOLON, ";");
            return false;
          }
        yylex();

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_expression(expression_ast **yynode)
  {
    *yynode = create<expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IDENTIFIER
        || yytoken == Token_NUMBER)
      {
        mult_expression_ast *__node_8 = 0;
        if (!parse_mult_expression(&__node_8))
          {
            yy_expected_symbol(ast_node::Kind_mult_expression, "mult_expression");
            return false;
          }
        (*yynode)->left_expr = __node_8;

        while (yytoken == Token_MINUS)
          {
            if (yytoken != Token_MINUS)
              {
                yy_expected_token(yytoken, Token_MINUS, "-");
                return false;
              }
            yylex();

            mult_expression_ast *__node_9 = 0;
            if (!parse_mult_expression(&__node_9))
              {
                yy_expected_symbol(ast_node::Kind_mult_expression, "mult_expression");
                return false;
              }
            (*yynode)->right_expr = __node_9;

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_function_definition(function_definition_ast **yynode)
  {
    *yynode = create<function_definition_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_FUNCTION)
      {
        if (yytoken != Token_FUNCTION)
          {
            yy_expected_token(yytoken, Token_FUNCTION, "function");
            return false;
          }
        yylex();

        if (yytoken != Token_IDENTIFIER)
          {
            yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
            return false;
          }
        (*yynode)->id = token_stream->index() - 1;
        yylex();

        if (yytoken != Token_LPAREN)
          {
            yy_expected_token(yytoken, Token_LPAREN, "(");
            return false;
          }
        yylex();

        if (yytoken == Token_IDENTIFIER)
          {
            if (yytoken != Token_IDENTIFIER)
              {
                yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                return false;
              }
            (*yynode)->param_sequence = snoc((*yynode)->param_sequence, token_stream->index() - 1, memory_pool);
            yylex();

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    yy_expected_token(yytoken, Token_COMMA, ",");
                    return false;
                  }
                yylex();

                if (yytoken != Token_IDENTIFIER)
                  {
                    yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                    return false;
                  }
                (*yynode)->param_sequence = snoc((*yynode)->param_sequence, token_stream->index() - 1, memory_pool);
                yylex();

              }
          }
        else if (true /*epsilon*/)
        {}
        else
          {
            return false;
          }
        if (yytoken != Token_RPAREN)
          {
            yy_expected_token(yytoken, Token_RPAREN, ")");
            return false;
          }
        yylex();

        body_ast *__node_10 = 0;
        if (!parse_body(&__node_10))
          {
            yy_expected_symbol(ast_node::Kind_body, "body");
            return false;
          }
        (*yynode)->body = __node_10;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_if_statement(if_statement_ast **yynode)
  {
    *yynode = create<if_statement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IF)
      {
        if (yytoken != Token_IF)
          {
            yy_expected_token(yytoken, Token_IF, "if");
            return false;
          }
        yylex();

        if (yytoken != Token_LPAREN)
          {
            yy_expected_token(yytoken, Token_LPAREN, "(");
            return false;
          }
        yylex();

        condition_ast *__node_11 = 0;
        if (!parse_condition(&__node_11))
          {
            yy_expected_symbol(ast_node::Kind_condition, "condition");
            return false;
          }
        (*yynode)->cond = __node_11;

        if (yytoken != Token_RPAREN)
          {
            yy_expected_token(yytoken, Token_RPAREN, ")");
            return false;
          }
        yylex();

        statement_ast *__node_12 = 0;
        if (!parse_statement(&__node_12))
          {
            yy_expected_symbol(ast_node::Kind_statement, "statement");
            return false;
          }
        (*yynode)->if_stmt = __node_12;

        if (yytoken == Token_ELSE)
          {
            if (yytoken != Token_ELSE)
              {
                yy_expected_token(yytoken, Token_ELSE, "else");
                return false;
              }
            yylex();

            statement_ast *__node_13 = 0;
            if (!parse_statement(&__node_13))
              {
                yy_expected_symbol(ast_node::Kind_statement, "statement");
                return false;
              }
            (*yynode)->else_stmt = __node_13;

          }
        else if (true /*epsilon*/)
        {}
        else
          {
            return false;
          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_mult_expression(mult_expression_ast **yynode)
  {
    *yynode = create<mult_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IDENTIFIER
        || yytoken == Token_NUMBER)
      {
        primary_ast *__node_14 = 0;
        if (!parse_primary(&__node_14))
          {
            yy_expected_symbol(ast_node::Kind_primary, "primary");
            return false;
          }
        (*yynode)->left_expr = __node_14;

        while (yytoken == Token_STAR)
          {
            if (yytoken != Token_STAR)
              {
                yy_expected_token(yytoken, Token_STAR, "*");
                return false;
              }
            yylex();

            primary_ast *__node_15 = 0;
            if (!parse_primary(&__node_15))
              {
                yy_expected_symbol(ast_node::Kind_primary, "primary");
                return false;
              }
            (*yynode)->right_expr = __node_15;

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_primary(primary_ast **yynode)
  {
    *yynode = create<primary_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IDENTIFIER
        || yytoken == Token_NUMBER)
      {
        if (yytoken == Token_NUMBER)
          {
            if (yytoken != Token_NUMBER)
              {
                yy_expected_token(yytoken, Token_NUMBER, "integer literal");
                return false;
              }
            (*yynode)->num = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_IDENTIFIER)
          {
            if (yytoken != Token_IDENTIFIER)
              {
                yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                return false;
              }
            (*yynode)->id = token_stream->index() - 1;
            yylex();

            if (yytoken == Token_LPAREN)
              {
                if (yytoken != Token_LPAREN)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                    return false;
                  }
                yylex();

                expression_ast *__node_16 = 0;
                if (!parse_expression(&__node_16))
                  {
                    yy_expected_symbol(ast_node::Kind_expression, "expression");
                    return false;
                  }
                (*yynode)->argument_sequence = snoc((*yynode)->argument_sequence, __node_16, memory_pool);

                while (yytoken == Token_COMMA)
                  {
                    if (yytoken != Token_COMMA)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                        return false;
                      }
                    yylex();

                    expression_ast *__node_17 = 0;
                    if (!parse_expression(&__node_17))
                      {
                        yy_expected_symbol(ast_node::Kind_expression, "expression");
                        return false;
                      }
                    (*yynode)->argument_sequence = snoc((*yynode)->argument_sequence, __node_17, memory_pool);

                  }
                if (yytoken != Token_RPAREN)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                    return false;
                  }
                yylex();

              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
          }
        else
          {
            return false;
          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_program(program_ast **yynode)
  {
    *yynode = create<program_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_FUNCTION || yytoken == Token_EOF)
      {
        while (yytoken == Token_FUNCTION)
          {
            function_definition_ast *__node_18 = 0;
            if (!parse_function_definition(&__node_18))
              {
                yy_expected_symbol(ast_node::Kind_function_definition, "function_definition");
                return false;
              }
            (*yynode)->fun_sequence = snoc((*yynode)->fun_sequence, __node_18, memory_pool);

          }
        if (Token_EOF != yytoken)
          {
            return false;
          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_return_statement(return_statement_ast **yynode)
                           {
                             *yynode = create<return_statement_ast>();

                             (*yynode)->start_token = token_stream->index() - 1;

                             if (yytoken == Token_RETURN)
                               {
                                 if (yytoken != Token_RETURN)
                                   {
                                     yy_expected_token(yytoken, Token_RETURN, "return");
                                     return false;
                                   }
                                 yylex();

                                 expression_ast *__node_19 = 0;
                                 if (!parse_expression(&__node_19))
                                   {
                                     yy_expected_symbol(ast_node::Kind_expression, "expression");
                                     return false;
                                   }
                                 (*yynode)->expr = __node_19;

                                 if (yytoken != Token_SEMICOLON)
                                   {
                                     yy_expected_token(yytoken, Token_SEMICOLON, ";");
                                     return false;
                                   }
                                 yylex();

                               }
                             else
                               {
                                 return false;
                               }

                             (*yynode)->end_token = token_stream->index() - 1;

                             return true;
                           }

                           bool parser::parse_statement(statement_ast **yynode)
                           {
                             *yynode = create<statement_ast>();

                             (*yynode)->start_token = token_stream->index() - 1;

                             if (yytoken == Token_IF
                                 || yytoken == Token_RETURN
                                 || yytoken == Token_LBRACE
                                 || yytoken == Token_IDENTIFIER)
                               {
                                 if (yytoken == Token_IDENTIFIER)
                                   {
                                     assignment_statement_ast *__node_20 = 0;
                                     if (!parse_assignment_statement(&__node_20))
                                       {
                                         yy_expected_symbol(ast_node::Kind_assignment_statement, "assignment_statement");
                                         return false;
                                       }
                                     (*yynode)->assign_stmt = __node_20;

                                   }
                                 else if (yytoken == Token_IF)
                                   {
                                     if_statement_ast *__node_21 = 0;
                                     if (!parse_if_statement(&__node_21))
                                       {
                                         yy_expected_symbol(ast_node::Kind_if_statement, "if_statement");
                                         return false;
                                       }
                                     (*yynode)->if_stmt = __node_21;

                                   }
                                 else if (yytoken == Token_LBRACE)
                                   {
                                     block_statement_ast *__node_22 = 0;
                                     if (!parse_block_statement(&__node_22))
                                       {
                                         yy_expected_symbol(ast_node::Kind_block_statement, "block_statement");
                                         return false;
                                       }
                                     (*yynode)->block_stmt = __node_22;

                                   }
                                 else if (yytoken == Token_RETURN)
                                   {
                                     return_statement_ast *__node_23 = 0;
                                     if (!parse_return_statement(&__node_23))
                                       {
                                         yy_expected_symbol(ast_node::Kind_return_statement, "return_statement");
                                         return false;
                                       }
                                     (*yynode)->return_stmt = __node_23;

                                   }
                                 else
                                   {
                                     return false;
                                   }
                               }
                             else
                               {
                                 return false;
                               }

                             (*yynode)->end_token = token_stream->index() - 1;

                             return true;
                           }

                           bool parser::parse_variable(variable_ast **yynode)
                           {
                             *yynode = create<variable_ast>();

                             (*yynode)->start_token = token_stream->index() - 1;

                             if (yytoken == Token_IDENTIFIER)
                               {
                                 if (yytoken != Token_IDENTIFIER)
                                   {
                                     yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                                     return false;
                                   }
                                 (*yynode)->id = token_stream->index() - 1;
                                 yylex();

                               }
                             else
                               {
                                 return false;
                               }

                             (*yynode)->end_token = token_stream->index() - 1;

                             return true;
                           }


                         } // end of namespace fact


