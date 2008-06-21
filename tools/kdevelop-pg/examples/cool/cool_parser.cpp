// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "cool_parser.h"


#include "cool_lexer.h"


namespace cool
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

} // end of namespace cool


namespace cool
  {

  bool parser::parse_additive_expression(additive_expression_ast **yynode)
  {
    *yynode = create<additive_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_IF
        || yytoken == Token_WHILE
        || yytoken == Token_LET
        || yytoken == Token_CASE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_TILDE
        || yytoken == Token_NOT
        || yytoken == Token_ISVOID
        || yytoken == Token_IDENTIFIER
        || yytoken == Token_INTEGER
        || yytoken == Token_STRING
        || yytoken == Token_TRUE
        || yytoken == Token_FALSE)
      {
        multiplicative_expression_ast *__node_0 = 0;
        if (!parse_multiplicative_expression(&__node_0))
          {
            yy_expected_symbol(ast_node::Kind_multiplicative_expression, "multiplicative_expression");
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_0, memory_pool);

        while (yytoken == Token_PLUS
               || yytoken == Token_MINUS)
          {
            if (yytoken == Token_PLUS)
              {
                if (yytoken != Token_PLUS)
                  {
                    yy_expected_token(yytoken, Token_PLUS, "+");
                    return false;
                  }
                (*yynode)->op = token_stream->index() - 1;
                yylex();

              }
            else if (yytoken == Token_MINUS)
              {
                if (yytoken != Token_MINUS)
                  {
                    yy_expected_token(yytoken, Token_MINUS, "-");
                    return false;
                  }
                (*yynode)->op = token_stream->index() - 1;
                yylex();

              }
            else
              {
                return false;
              }
            multiplicative_expression_ast *__node_1 = 0;
            if (!parse_multiplicative_expression(&__node_1))
              {
                yy_expected_symbol(ast_node::Kind_multiplicative_expression, "multiplicative_expression");
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_1, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_block_expression(block_expression_ast **yynode)
  {
    *yynode = create<block_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LBRACE)
      {
        if (yytoken != Token_LBRACE)
          {
            yy_expected_token(yytoken, Token_LBRACE, "{");
            return false;
          }
        yylex();

        while (yytoken == Token_NEW
               || yytoken == Token_IF
               || yytoken == Token_WHILE
               || yytoken == Token_LET
               || yytoken == Token_CASE
               || yytoken == Token_LPAREN
               || yytoken == Token_LBRACE
               || yytoken == Token_TILDE
               || yytoken == Token_NOT
               || yytoken == Token_ISVOID
               || yytoken == Token_IDENTIFIER
               || yytoken == Token_INTEGER
               || yytoken == Token_STRING
               || yytoken == Token_TRUE
               || yytoken == Token_FALSE)
          {
            expression_ast *__node_2 = 0;
            if (!parse_expression(&__node_2))
              {
                yy_expected_symbol(ast_node::Kind_expression, "expression");
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_2, memory_pool);

            if (yytoken != Token_SEMICOLON)
              {
                yy_expected_token(yytoken, Token_SEMICOLON, ";");
                return false;
              }
            yylex();

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

  bool parser::parse_case_condition(case_condition_ast **yynode)
  {
    *yynode = create<case_condition_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IDENTIFIER)
      {
        if (yytoken != Token_IDENTIFIER)
          {
            yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
            return false;
          }
        (*yynode)->name = token_stream->index() - 1;
        yylex();

        if (yytoken != Token_COLON)
          {
            yy_expected_token(yytoken, Token_COLON, ":");
            return false;
          }
        yylex();

        if (yytoken != Token_TYPE)
          {
            yy_expected_token(yytoken, Token_TYPE, "type specification");
            return false;
          }
        (*yynode)->type = token_stream->index() - 1;
        yylex();

        if (yytoken != Token_ARROW_RIGHT)
          {
            yy_expected_token(yytoken, Token_ARROW_RIGHT, "=>");
            return false;
          }
        yylex();

        expression_ast *__node_3 = 0;
        if (!parse_expression(&__node_3))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->expression = __node_3;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_case_expression(case_expression_ast **yynode)
  {
    *yynode = create<case_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CASE)
      {
        if (yytoken != Token_CASE)
          {
            yy_expected_token(yytoken, Token_CASE, "case");
            return false;
          }
        yylex();

        expression_ast *__node_4 = 0;
        if (!parse_expression(&__node_4))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->expression = __node_4;

        if (yytoken != Token_OF)
          {
            yy_expected_token(yytoken, Token_OF, "of");
            return false;
          }
        yylex();

        while (yytoken == Token_IDENTIFIER)
          {
            case_condition_ast *__node_5 = 0;
            if (!parse_case_condition(&__node_5))
              {
                yy_expected_symbol(ast_node::Kind_case_condition, "case_condition");
                return false;
              }
            (*yynode)->condition_sequence = snoc((*yynode)->condition_sequence, __node_5, memory_pool);

            if (yytoken != Token_SEMICOLON)
              {
                yy_expected_token(yytoken, Token_SEMICOLON, ";");
                return false;
              }
            yylex();

          }
        if (yytoken != Token_ESAC)
          {
            yy_expected_token(yytoken, Token_ESAC, "esac");
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

  bool parser::parse_class(class_ast **yynode)
  {
    *yynode = create<class_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CLASS)
      {
        if (yytoken != Token_CLASS)
          {
            yy_expected_token(yytoken, Token_CLASS, "class");
            return false;
          }
        yylex();

        if (yytoken != Token_TYPE)
          {
            yy_expected_token(yytoken, Token_TYPE, "type specification");
            return false;
          }
        (*yynode)->type = token_stream->index() - 1;
        yylex();

        if (yytoken == Token_INHERITS)
          {
            if (yytoken != Token_INHERITS)
              {
                yy_expected_token(yytoken, Token_INHERITS, "inherits");
                return false;
              }
            yylex();

            if (yytoken != Token_TYPE)
              {
                yy_expected_token(yytoken, Token_TYPE, "type specification");
                return false;
              }
            (*yynode)->base_type = token_stream->index() - 1;
            yylex();

          }
        else if (true /*epsilon*/)
        {}
        else
          {
            return false;
          }
        if (yytoken != Token_LBRACE)
          {
            yy_expected_token(yytoken, Token_LBRACE, "{");
            return false;
          }
        yylex();

        while (yytoken == Token_IDENTIFIER)
          {
            feature_ast *__node_6 = 0;
            if (!parse_feature(&__node_6))
              {
                yy_expected_symbol(ast_node::Kind_feature, "feature");
                return false;
              }
            (*yynode)->feature_sequence = snoc((*yynode)->feature_sequence, __node_6, memory_pool);

            if (yytoken != Token_SEMICOLON)
              {
                yy_expected_token(yytoken, Token_SEMICOLON, ";");
                return false;
              }
            yylex();

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

  bool parser::parse_expression(expression_ast **yynode)
  {
    *yynode = create<expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_IF
        || yytoken == Token_WHILE
        || yytoken == Token_LET
        || yytoken == Token_CASE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_TILDE
        || yytoken == Token_NOT
        || yytoken == Token_ISVOID
        || yytoken == Token_IDENTIFIER
        || yytoken == Token_INTEGER
        || yytoken == Token_STRING
        || yytoken == Token_TRUE
        || yytoken == Token_FALSE)
      {
        relational_expression_ast *__node_7 = 0;
        if (!parse_relational_expression(&__node_7))
          {
            yy_expected_symbol(ast_node::Kind_relational_expression, "relational_expression");
            return false;
          }
        (*yynode)->expression = __node_7;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_feature(feature_ast **yynode)
  {
    *yynode = create<feature_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IDENTIFIER)
      {
        if ((yytoken == Token_IDENTIFIER) && ( LA(2).kind == Token_LPAREN ))
          {
            if (yytoken != Token_IDENTIFIER)
              {
                yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                return false;
              }
            (*yynode)->name = token_stream->index() - 1;
            yylex();

            if (yytoken != Token_LPAREN)
              {
                yy_expected_token(yytoken, Token_LPAREN, "(");
                return false;
              }
            yylex();

            if (yytoken == Token_IDENTIFIER)
              {
                formal_ast *__node_8 = 0;
                if (!parse_formal(&__node_8))
                  {
                    yy_expected_symbol(ast_node::Kind_formal, "formal");
                    return false;
                  }
                (*yynode)->formal_sequence = snoc((*yynode)->formal_sequence, __node_8, memory_pool);

                while (yytoken == Token_COMMA)
                  {
                    if (yytoken != Token_COMMA)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                        return false;
                      }
                    yylex();

                    formal_ast *__node_9 = 0;
                    if (!parse_formal(&__node_9))
                      {
                        yy_expected_symbol(ast_node::Kind_formal, "formal");
                        return false;
                      }
                    (*yynode)->formal_sequence = snoc((*yynode)->formal_sequence, __node_9, memory_pool);

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

            if (yytoken != Token_COLON)
              {
                yy_expected_token(yytoken, Token_COLON, ":");
                return false;
              }
            yylex();

            if (yytoken != Token_TYPE)
              {
                yy_expected_token(yytoken, Token_TYPE, "type specification");
                return false;
              }
            (*yynode)->type = token_stream->index() - 1;
            yylex();

            if (yytoken != Token_LBRACE)
              {
                yy_expected_token(yytoken, Token_LBRACE, "{");
                return false;
              }
            yylex();

            expression_ast *__node_10 = 0;
            if (!parse_expression(&__node_10))
              {
                yy_expected_symbol(ast_node::Kind_expression, "expression");
                return false;
              }
            (*yynode)->expression = __node_10;

            if (yytoken != Token_RBRACE)
              {
                yy_expected_token(yytoken, Token_RBRACE, "}");
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_IDENTIFIER)
          {
            if (yytoken != Token_IDENTIFIER)
              {
                yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                return false;
              }
            (*yynode)->name = token_stream->index() - 1;
            yylex();

            if (yytoken != Token_COLON)
              {
                yy_expected_token(yytoken, Token_COLON, ":");
                return false;
              }
            yylex();

            if (yytoken != Token_TYPE)
              {
                yy_expected_token(yytoken, Token_TYPE, "type specification");
                return false;
              }
            (*yynode)->type = token_stream->index() - 1;
            yylex();

            if (yytoken == Token_ARROW_LEFT)
              {
                if (yytoken != Token_ARROW_LEFT)
                  {
                    yy_expected_token(yytoken, Token_ARROW_LEFT, "<-");
                    return false;
                  }
                yylex();

                expression_ast *__node_11 = 0;
                if (!parse_expression(&__node_11))
                  {
                    yy_expected_symbol(ast_node::Kind_expression, "expression");
                    return false;
                  }
                (*yynode)->expression = __node_11;

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

  bool parser::parse_formal(formal_ast **yynode)
  {
    *yynode = create<formal_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IDENTIFIER)
      {
        if (yytoken != Token_IDENTIFIER)
          {
            yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
            return false;
          }
        (*yynode)->name = token_stream->index() - 1;
        yylex();

        if (yytoken != Token_COLON)
          {
            yy_expected_token(yytoken, Token_COLON, ":");
            return false;
          }
        yylex();

        if (yytoken != Token_TYPE)
          {
            yy_expected_token(yytoken, Token_TYPE, "type specification");
            return false;
          }
        (*yynode)->type = token_stream->index() - 1;
        yylex();

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_if_expression(if_expression_ast **yynode)
  {
    *yynode = create<if_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IF)
      {
        if (yytoken != Token_IF)
          {
            yy_expected_token(yytoken, Token_IF, "if");
            return false;
          }
        yylex();

        expression_ast *__node_12 = 0;
        if (!parse_expression(&__node_12))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->condition = __node_12;

        if (yytoken != Token_THEN)
          {
            yy_expected_token(yytoken, Token_THEN, "then");
            return false;
          }
        yylex();

        expression_ast *__node_13 = 0;
        if (!parse_expression(&__node_13))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->true_expression = __node_13;

        if (yytoken != Token_ELSE)
          {
            yy_expected_token(yytoken, Token_ELSE, "else");
            return false;
          }
        yylex();

        expression_ast *__node_14 = 0;
        if (!parse_expression(&__node_14))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->false_expression = __node_14;

        if (yytoken != Token_FI)
          {
            yy_expected_token(yytoken, Token_FI, "fi");
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

  bool parser::parse_let_declaration(let_declaration_ast **yynode)
  {
    *yynode = create<let_declaration_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IDENTIFIER)
      {
        if (yytoken != Token_IDENTIFIER)
          {
            yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
            return false;
          }
        (*yynode)->name = token_stream->index() - 1;
        yylex();

        if (yytoken != Token_COLON)
          {
            yy_expected_token(yytoken, Token_COLON, ":");
            return false;
          }
        yylex();

        if (yytoken != Token_TYPE)
          {
            yy_expected_token(yytoken, Token_TYPE, "type specification");
            return false;
          }
        (*yynode)->type = token_stream->index() - 1;
        yylex();

        if (yytoken == Token_ARROW_LEFT)
          {
            if (yytoken != Token_ARROW_LEFT)
              {
                yy_expected_token(yytoken, Token_ARROW_LEFT, "<-");
                return false;
              }
            yylex();

            expression_ast *__node_15 = 0;
            if (!parse_expression(&__node_15))
              {
                yy_expected_symbol(ast_node::Kind_expression, "expression");
                return false;
              }
            (*yynode)->expression = __node_15;

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

  bool parser::parse_let_expression(let_expression_ast **yynode)
  {
    *yynode = create<let_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LET)
      {
        if (yytoken != Token_LET)
          {
            yy_expected_token(yytoken, Token_LET, "let");
            return false;
          }
        yylex();

        let_declaration_ast *__node_16 = 0;
        if (!parse_let_declaration(&__node_16))
          {
            yy_expected_symbol(ast_node::Kind_let_declaration, "let_declaration");
            return false;
          }
        (*yynode)->declaration_sequence = snoc((*yynode)->declaration_sequence, __node_16, memory_pool);

        while (yytoken == Token_COMMA)
          {
            if (yytoken != Token_COMMA)
              {
                yy_expected_token(yytoken, Token_COMMA, ",");
                return false;
              }
            yylex();

            let_declaration_ast *__node_17 = 0;
            if (!parse_let_declaration(&__node_17))
              {
                yy_expected_symbol(ast_node::Kind_let_declaration, "let_declaration");
                return false;
              }
            (*yynode)->declaration_sequence = snoc((*yynode)->declaration_sequence, __node_17, memory_pool);

          }
        if (yytoken != Token_IN)
          {
            yy_expected_token(yytoken, Token_IN, "in");
            return false;
          }
        yylex();

        expression_ast *__node_18 = 0;
        if (!parse_expression(&__node_18))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->body_expression = __node_18;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_multiplicative_expression(multiplicative_expression_ast **yynode)
  {
    *yynode = create<multiplicative_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_IF
        || yytoken == Token_WHILE
        || yytoken == Token_LET
        || yytoken == Token_CASE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_TILDE
        || yytoken == Token_NOT
        || yytoken == Token_ISVOID
        || yytoken == Token_IDENTIFIER
        || yytoken == Token_INTEGER
        || yytoken == Token_STRING
        || yytoken == Token_TRUE
        || yytoken == Token_FALSE)
      {
        postfix_expression_ast *__node_19 = 0;
        if (!parse_postfix_expression(&__node_19))
          {
            yy_expected_symbol(ast_node::Kind_postfix_expression, "postfix_expression");
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_19, memory_pool);

        while (yytoken == Token_STAR
               || yytoken == Token_SLASH)
          {
            if (yytoken == Token_STAR)
              {
                if (yytoken != Token_STAR)
                  {
                    yy_expected_token(yytoken, Token_STAR, "*");
                    return false;
                  }
                (*yynode)->op = token_stream->index() - 1;
                yylex();

              }
            else if (yytoken == Token_SLASH)
              {
                if (yytoken != Token_SLASH)
                  {
                    yy_expected_token(yytoken, Token_SLASH, "/");
                    return false;
                  }
                (*yynode)->op = token_stream->index() - 1;
                yylex();

              }
            else
              {
                return false;
              }
            postfix_expression_ast *__node_20 = 0;
            if (!parse_postfix_expression(&__node_20))
              {
                yy_expected_symbol(ast_node::Kind_postfix_expression, "postfix_expression");
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_20, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_postfix_expression(postfix_expression_ast **yynode)
  {
    *yynode = create<postfix_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_IF
        || yytoken == Token_WHILE
        || yytoken == Token_LET
        || yytoken == Token_CASE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_TILDE
        || yytoken == Token_NOT
        || yytoken == Token_ISVOID
        || yytoken == Token_IDENTIFIER
        || yytoken == Token_INTEGER
        || yytoken == Token_STRING
        || yytoken == Token_TRUE
        || yytoken == Token_FALSE)
      {
        unary_expression_ast *__node_21 = 0;
        if (!parse_unary_expression(&__node_21))
          {
            yy_expected_symbol(ast_node::Kind_unary_expression, "unary_expression");
            return false;
          }
        (*yynode)->base_expression = __node_21;

        while (yytoken == Token_DOT
               || yytoken == Token_AT)
          {
            if (yytoken == Token_AT)
              {
                if (yytoken != Token_AT)
                  {
                    yy_expected_token(yytoken, Token_AT, "@");
                    return false;
                  }
                yylex();

                if (yytoken != Token_TYPE)
                  {
                    yy_expected_token(yytoken, Token_TYPE, "type specification");
                    return false;
                  }
                (*yynode)->at_type = token_stream->index() - 1;
                yylex();

                if (yytoken != Token_DOT)
                  {
                    yy_expected_token(yytoken, Token_DOT, ".");
                    return false;
                  }
                yylex();

                if (yytoken != Token_IDENTIFIER)
                  {
                    yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                    return false;
                  }
                (*yynode)->name = token_stream->index() - 1;
                yylex();

                if (yytoken != Token_LPAREN)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                    return false;
                  }
                yylex();

                if (yytoken == Token_NEW
                    || yytoken == Token_IF
                    || yytoken == Token_WHILE
                    || yytoken == Token_LET
                    || yytoken == Token_CASE
                    || yytoken == Token_LPAREN
                    || yytoken == Token_LBRACE
                    || yytoken == Token_TILDE
                    || yytoken == Token_NOT
                    || yytoken == Token_ISVOID
                    || yytoken == Token_IDENTIFIER
                    || yytoken == Token_INTEGER
                    || yytoken == Token_STRING
                    || yytoken == Token_TRUE
                    || yytoken == Token_FALSE)
                  {
                    expression_ast *__node_22 = 0;
                    if (!parse_expression(&__node_22))
                      {
                        yy_expected_symbol(ast_node::Kind_expression, "expression");
                        return false;
                      }
                    (*yynode)->arguments_sequence = snoc((*yynode)->arguments_sequence, __node_22, memory_pool);

                    while (yytoken == Token_COMMA)
                      {
                        if (yytoken != Token_COMMA)
                          {
                            yy_expected_token(yytoken, Token_COMMA, ",");
                            return false;
                          }
                        yylex();

                        expression_ast *__node_23 = 0;
                        if (!parse_expression(&__node_23))
                          {
                            yy_expected_symbol(ast_node::Kind_expression, "expression");
                            return false;
                          }
                        (*yynode)->arguments_sequence = snoc((*yynode)->arguments_sequence, __node_23, memory_pool);

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

              }
            else if (yytoken == Token_DOT)
              {
                if (yytoken != Token_DOT)
                  {
                    yy_expected_token(yytoken, Token_DOT, ".");
                    return false;
                  }
                yylex();

                if (yytoken != Token_IDENTIFIER)
                  {
                    yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                    return false;
                  }
                (*yynode)->name = token_stream->index() - 1;
                yylex();

                if (yytoken != Token_LPAREN)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                    return false;
                  }
                yylex();

                if (yytoken == Token_NEW
                    || yytoken == Token_IF
                    || yytoken == Token_WHILE
                    || yytoken == Token_LET
                    || yytoken == Token_CASE
                    || yytoken == Token_LPAREN
                    || yytoken == Token_LBRACE
                    || yytoken == Token_TILDE
                    || yytoken == Token_NOT
                    || yytoken == Token_ISVOID
                    || yytoken == Token_IDENTIFIER
                    || yytoken == Token_INTEGER
                    || yytoken == Token_STRING
                    || yytoken == Token_TRUE
                    || yytoken == Token_FALSE)
                  {
                    expression_ast *__node_24 = 0;
                    if (!parse_expression(&__node_24))
                      {
                        yy_expected_symbol(ast_node::Kind_expression, "expression");
                        return false;
                      }
                    (*yynode)->arguments_sequence = snoc((*yynode)->arguments_sequence, __node_24, memory_pool);

                    while (yytoken == Token_COMMA)
                      {
                        if (yytoken != Token_COMMA)
                          {
                            yy_expected_token(yytoken, Token_COMMA, ",");
                            return false;
                          }
                        yylex();

                        expression_ast *__node_25 = 0;
                        if (!parse_expression(&__node_25))
                          {
                            yy_expected_symbol(ast_node::Kind_expression, "expression");
                            return false;
                          }
                        (*yynode)->arguments_sequence = snoc((*yynode)->arguments_sequence, __node_25, memory_pool);

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

              }
            else
              {
                return false;
              }
          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_primary_expression(primary_expression_ast **yynode)
  {
    *yynode = create<primary_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_IF
        || yytoken == Token_WHILE
        || yytoken == Token_LET
        || yytoken == Token_CASE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_IDENTIFIER
        || yytoken == Token_INTEGER
        || yytoken == Token_STRING
        || yytoken == Token_TRUE
        || yytoken == Token_FALSE)
      {
        if ((yytoken == Token_IDENTIFIER) && ( LA(2).kind == Token_ARROW_LEFT ))
          {
            if (yytoken != Token_IDENTIFIER)
              {
                yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                return false;
              }
            (*yynode)->name = token_stream->index() - 1;
            yylex();

            if (yytoken != Token_ARROW_LEFT)
              {
                yy_expected_token(yytoken, Token_ARROW_LEFT, "<-");
                return false;
              }
            yylex();

            expression_ast *__node_26 = 0;
            if (!parse_expression(&__node_26))
              {
                yy_expected_symbol(ast_node::Kind_expression, "expression");
                return false;
              }
            (*yynode)->expression = __node_26;

          }
        else if ((yytoken == Token_IDENTIFIER) && ( LA(2).kind == Token_LPAREN ))
          {
            if (yytoken != Token_IDENTIFIER)
              {
                yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                return false;
              }
            (*yynode)->name = token_stream->index() - 1;
            yylex();

            if (yytoken != Token_LPAREN)
              {
                yy_expected_token(yytoken, Token_LPAREN, "(");
                return false;
              }
            yylex();

            if (yytoken == Token_NEW
                || yytoken == Token_IF
                || yytoken == Token_WHILE
                || yytoken == Token_LET
                || yytoken == Token_CASE
                || yytoken == Token_LPAREN
                || yytoken == Token_LBRACE
                || yytoken == Token_TILDE
                || yytoken == Token_NOT
                || yytoken == Token_ISVOID
                || yytoken == Token_IDENTIFIER
                || yytoken == Token_INTEGER
                || yytoken == Token_STRING
                || yytoken == Token_TRUE
                || yytoken == Token_FALSE)
              {
                expression_ast *__node_27 = 0;
                if (!parse_expression(&__node_27))
                  {
                    yy_expected_symbol(ast_node::Kind_expression, "expression");
                    return false;
                  }
                (*yynode)->argument_sequence = snoc((*yynode)->argument_sequence, __node_27, memory_pool);

                while (yytoken == Token_COMMA)
                  {
                    if (yytoken != Token_COMMA)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                        return false;
                      }
                    yylex();

                    expression_ast *__node_28 = 0;
                    if (!parse_expression(&__node_28))
                      {
                        yy_expected_symbol(ast_node::Kind_expression, "expression");
                        return false;
                      }
                    (*yynode)->argument_sequence = snoc((*yynode)->argument_sequence, __node_28, memory_pool);

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

          }
        else if (yytoken == Token_IDENTIFIER)
          {
            if (yytoken != Token_IDENTIFIER)
              {
                yy_expected_token(yytoken, Token_IDENTIFIER, "identifier");
                return false;
              }
            (*yynode)->variable = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_INTEGER)
          {
            if (yytoken != Token_INTEGER)
              {
                yy_expected_token(yytoken, Token_INTEGER, "integer literal");
                return false;
              }
            (*yynode)->integer_literal = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_STRING)
          {
            if (yytoken != Token_STRING)
              {
                yy_expected_token(yytoken, Token_STRING, "string literal");
                return false;
              }
            (*yynode)->string_literal = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_TRUE)
          {
            if (yytoken != Token_TRUE)
              {
                yy_expected_token(yytoken, Token_TRUE, "true");
                return false;
              }
            (*yynode)->true_literal = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_FALSE)
          {
            if (yytoken != Token_FALSE)
              {
                yy_expected_token(yytoken, Token_FALSE, "false");
                return false;
              }
            (*yynode)->false_literal = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_NEW)
          {
            if (yytoken != Token_NEW)
              {
                yy_expected_token(yytoken, Token_NEW, "new");
                return false;
              }
            yylex();

            if (yytoken != Token_TYPE)
              {
                yy_expected_token(yytoken, Token_TYPE, "type specification");
                return false;
              }
            (*yynode)->new_type = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_LPAREN)
          {
            if (yytoken != Token_LPAREN)
              {
                yy_expected_token(yytoken, Token_LPAREN, "(");
                return false;
              }
            yylex();

            expression_ast *__node_29 = 0;
            if (!parse_expression(&__node_29))
              {
                yy_expected_symbol(ast_node::Kind_expression, "expression");
                return false;
              }
            (*yynode)->expression = __node_29;

            if (yytoken != Token_RPAREN)
              {
                yy_expected_token(yytoken, Token_RPAREN, ")");
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_IF)
          {
            if_expression_ast *__node_30 = 0;
            if (!parse_if_expression(&__node_30))
              {
                yy_expected_symbol(ast_node::Kind_if_expression, "if_expression");
                return false;
              }
            (*yynode)->if_expression = __node_30;

          }
        else if (yytoken == Token_WHILE)
          {
            while_expression_ast *__node_31 = 0;
            if (!parse_while_expression(&__node_31))
              {
                yy_expected_symbol(ast_node::Kind_while_expression, "while_expression");
                return false;
              }
            (*yynode)->while_expression = __node_31;

          }
        else if (yytoken == Token_LBRACE)
          {
            block_expression_ast *__node_32 = 0;
            if (!parse_block_expression(&__node_32))
              {
                yy_expected_symbol(ast_node::Kind_block_expression, "block_expression");
                return false;
              }
            (*yynode)->block_expression = __node_32;

          }
        else if (yytoken == Token_LET)
          {
            let_expression_ast *__node_33 = 0;
            if (!parse_let_expression(&__node_33))
              {
                yy_expected_symbol(ast_node::Kind_let_expression, "let_expression");
                return false;
              }
            (*yynode)->let_expression = __node_33;

          }
        else if (yytoken == Token_CASE)
          {
            case_expression_ast *__node_34 = 0;
            if (!parse_case_expression(&__node_34))
              {
                yy_expected_symbol(ast_node::Kind_case_expression, "case_expression");
                return false;
              }
            (*yynode)->case_expression = __node_34;

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

    if (yytoken == Token_CLASS || yytoken == Token_EOF)
      {
        while (yytoken == Token_CLASS)
          {
            class_ast *__node_35 = 0;
            if (!parse_class(&__node_35))
              {
                yy_expected_symbol(ast_node::Kind_class, "class");
                return false;
              }
            (*yynode)->klass_sequence = snoc((*yynode)->klass_sequence, __node_35, memory_pool);

            if (yytoken != Token_SEMICOLON)
              {
                yy_expected_token(yytoken, Token_SEMICOLON, ";");
                return false;
              }
            yylex();

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

  bool parser::parse_relational_expression(relational_expression_ast **yynode)
  {
    *yynode = create<relational_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_IF
        || yytoken == Token_WHILE
        || yytoken == Token_LET
        || yytoken == Token_CASE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_TILDE
        || yytoken == Token_NOT
        || yytoken == Token_ISVOID
        || yytoken == Token_IDENTIFIER
        || yytoken == Token_INTEGER
        || yytoken == Token_STRING
        || yytoken == Token_TRUE
        || yytoken == Token_FALSE)
      {
        additive_expression_ast *__node_36 = 0;
        if (!parse_additive_expression(&__node_36))
          {
            yy_expected_symbol(ast_node::Kind_additive_expression, "additive_expression");
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_36, memory_pool);

        while (yytoken == Token_EQUAL
               || yytoken == Token_LESS_EQUAL
               || yytoken == Token_LESS)
          {
            if (yytoken == Token_EQUAL)
              {
                if (yytoken != Token_EQUAL)
                  {
                    yy_expected_token(yytoken, Token_EQUAL, "=");
                    return false;
                  }
                (*yynode)->op = token_stream->index() - 1;
                yylex();

              }
            else if (yytoken == Token_LESS_EQUAL)
              {
                if (yytoken != Token_LESS_EQUAL)
                  {
                    yy_expected_token(yytoken, Token_LESS_EQUAL, "<=");
                    return false;
                  }
                (*yynode)->op = token_stream->index() - 1;
                yylex();

              }
            else if (yytoken == Token_LESS)
              {
                if (yytoken != Token_LESS)
                  {
                    yy_expected_token(yytoken, Token_LESS, "<");
                    return false;
                  }
                (*yynode)->op = token_stream->index() - 1;
                yylex();

              }
            else
              {
                return false;
              }
            additive_expression_ast *__node_37 = 0;
            if (!parse_additive_expression(&__node_37))
              {
                yy_expected_symbol(ast_node::Kind_additive_expression, "additive_expression");
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_37, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_unary_expression(unary_expression_ast **yynode)
  {
    *yynode = create<unary_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_IF
        || yytoken == Token_WHILE
        || yytoken == Token_LET
        || yytoken == Token_CASE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_TILDE
        || yytoken == Token_NOT
        || yytoken == Token_ISVOID
        || yytoken == Token_IDENTIFIER
        || yytoken == Token_INTEGER
        || yytoken == Token_STRING
        || yytoken == Token_TRUE
        || yytoken == Token_FALSE)
      {
        if (yytoken == Token_TILDE)
          {
            if (yytoken != Token_TILDE)
              {
                yy_expected_token(yytoken, Token_TILDE, "~");
                return false;
              }
            (*yynode)->op = token_stream->index() - 1;
            yylex();

            primary_expression_ast *__node_38 = 0;
            if (!parse_primary_expression(&__node_38))
              {
                yy_expected_symbol(ast_node::Kind_primary_expression, "primary_expression");
                return false;
              }
            (*yynode)->expression = __node_38;

          }
        else if (yytoken == Token_NOT)
          {
            if (yytoken != Token_NOT)
              {
                yy_expected_token(yytoken, Token_NOT, "not");
                return false;
              }
            (*yynode)->op = token_stream->index() - 1;
            yylex();

            primary_expression_ast *__node_39 = 0;
            if (!parse_primary_expression(&__node_39))
              {
                yy_expected_symbol(ast_node::Kind_primary_expression, "primary_expression");
                return false;
              }
            (*yynode)->expression = __node_39;

          }
        else if (yytoken == Token_ISVOID)
          {
            if (yytoken != Token_ISVOID)
              {
                yy_expected_token(yytoken, Token_ISVOID, "isvoid");
                return false;
              }
            (*yynode)->op = token_stream->index() - 1;
            yylex();

            primary_expression_ast *__node_40 = 0;
            if (!parse_primary_expression(&__node_40))
              {
                yy_expected_symbol(ast_node::Kind_primary_expression, "primary_expression");
                return false;
              }
            (*yynode)->expression = __node_40;

          }
        else if (yytoken == Token_NEW
                 || yytoken == Token_IF
                 || yytoken == Token_WHILE
                 || yytoken == Token_LET
                 || yytoken == Token_CASE
                 || yytoken == Token_LPAREN
                 || yytoken == Token_LBRACE
                 || yytoken == Token_IDENTIFIER
                 || yytoken == Token_INTEGER
                 || yytoken == Token_STRING
                 || yytoken == Token_TRUE
                 || yytoken == Token_FALSE)
          {
            primary_expression_ast *__node_41 = 0;
            if (!parse_primary_expression(&__node_41))
              {
                yy_expected_symbol(ast_node::Kind_primary_expression, "primary_expression");
                return false;
              }
            (*yynode)->expression = __node_41;

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

  bool parser::parse_while_expression(while_expression_ast **yynode)
  {
    *yynode = create<while_expression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_WHILE)
      {
        if (yytoken != Token_WHILE)
          {
            yy_expected_token(yytoken, Token_WHILE, "while");
            return false;
          }
        yylex();

        expression_ast *__node_42 = 0;
        if (!parse_expression(&__node_42))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->condition = __node_42;

        if (yytoken != Token_LOOP)
          {
            yy_expected_token(yytoken, Token_LOOP, "loop");
            return false;
          }
        yylex();

        expression_ast *__node_43 = 0;
        if (!parse_expression(&__node_43))
          {
            yy_expected_symbol(ast_node::Kind_expression, "expression");
            return false;
          }
        (*yynode)->loop_expression = __node_43;

        if (yytoken != Token_POOL)
          {
            yy_expected_token(yytoken, Token_POOL, "pool");
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


} // end of namespace cool


