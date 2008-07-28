// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "rphp_parser.h"


#include "phplexer.h"

namespace rphp
  {

  void parser::tokenize( const UnicodeString& contents )
  {
    m_contents = contents;
    Lexer<UnicodeString, UChar32> lexer( this, contents );
    int kind = parser::Token_EOF;

    do
      {
        kind = lexer.nextTokenKind();
        while (kind == parser::Token_WHITESPACE || kind == parser::Token_COMMENT || kind == parser::Token_DOC_COMMENT)
          {
            kind = lexer.nextTokenKind();
          }
        if ( !kind ) // when the lexer returns 0, the end of file is reached
          {
            kind = parser::Token_EOF;
          }
        parser::token_type &t = token_stream->next();
        t.begin = lexer.tokenBegin();
        t.end = lexer.tokenEnd();
        t.kind = kind;
        //if ( m_debug ) qDebug() << kind << tokenText(t.begin,t.end) << t.begin << t.end;
      }
    while ( kind != parser::Token_EOF );

    yylex(); // produce the look ahead token
  }


  UnicodeString parser::tokenText(rint64 begin, rint64 end)
  {
#ifdef PENDING_THOMAS
    // TODO pending
    return m_contents.mid(begin,end-begin+1);
#endif
    return "";
  }


  void parser::reportProblem( parser::ProblemType type, const std::string& message )
  {
    if (type == Error)
      std::cout << "** ERROR:" << message;
    else if (type == Warning)
      std::cout << "** WARNING:" << message;
    else if (type == Info)
      std::cout << "** Info:" << message;
  }

  // custom error recovery
  void parser::yy_expected_token(int kind, std::size_t token, const char* name)
  {
    std::string msg = "Expected token ";
    msg += name;
    reportProblem( parser::Error, msg );
  }

  void parser::yy_expected_symbol(int kind, const char* name)
  {
    rint64 line;
    rint64 col;
    rint64 index = token_stream->index()-1;
    token_type &token = token_stream->token(index);
    std::cout << "token starts at:" << token.begin;
    std::cout << "index is:" << index;
    //    token_stream->start_position(index, &line, &col);
    UnicodeString tokenValue = tokenText(token.begin, token.end);

    std::stringstream msg;
    msg << "Expected symbol "
    << name
    << " (current token: \""
    //        + ( token.kind != 0 ? tokenValue : \"EOF\" )
    + token.kind
    << " at line: "
    << line
    << " col: "
    << col;

    //        reportProblem( parser::Error, msg.str() );

#ifdef PENDING_THOMAS // can be removed

    reportProblem( parser::Error,
                   QString("Expected symbol \"%1\" (current token: \"%2\" [%3] at line: %4 col: %5)")
                   .arg(name)
                   .arg(token.kind != 0 ? tokenValue : "EOF")
                   .arg(token.kind)
                   .arg(line)
                   .arg(col));
#endif
  }

  void parser::setDebug( bool debug )
  {
    m_debug = debug;
  }

  parser::parser_state *parser::copy_current_state()
  {
    parser_state *state = new parser_state();
    state->varExpressionState = m_state.varExpressionState;
    state->varExpressionIsVariable = m_state.varExpressionIsVariable;
    return state;
  }

  void parser::restore_state( parser::parser_state* state)
  {
    m_state.varExpressionState = state->varExpressionState;
    m_state.varExpressionIsVariable = state->varExpressionIsVariable;
  }

} // end of namespace rphp


namespace rphp
  {

  bool parser::parse_additiveExpression(additiveExpression_ast **yynode)
  {
    *yynode = create<additiveExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        multiplicativeExpression_ast *__node_0 = 0;
        if (!parse_multiplicativeExpression(&__node_0))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_multiplicativeExpression, "multiplicativeExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_0;

        while (yytoken == Token_PLUS
               || yytoken == Token_MINUS
               || yytoken == Token_CONCAT)
          {
            additiveExpressionRest_ast *__node_1 = 0;
            if (!parse_additiveExpressionRest(&__node_1))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_additiveExpressionRest, "additiveExpressionRest");
                  }
                return false;
              }
            (*yynode)->additionalExpression_sequence = snoc((*yynode)->additionalExpression_sequence, __node_1, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_additiveExpressionRest(additiveExpressionRest_ast **yynode)
  {
    *yynode = create<additiveExpressionRest_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_CONCAT)
      {
        if (yytoken == Token_PLUS)
          {
            if (yytoken != Token_PLUS)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_PLUS, "+");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_MINUS)
          {
            if (yytoken != Token_MINUS)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_MINUS, "-");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_CONCAT)
          {
            if (yytoken != Token_CONCAT)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CONCAT, ".");
                  }
                return false;
              }
            yylex();

          }
        else
          {
            return false;
          }
        multiplicativeExpression_ast *__node_2 = 0;
        if (!parse_multiplicativeExpression(&__node_2))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_multiplicativeExpression, "multiplicativeExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_2;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_arrayPairValue(arrayPairValue_ast **yynode)
  {
    *yynode = create<arrayPairValue_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_BIT_AND
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        if (yytoken == Token_NEW
            || yytoken == Token_PRINT
            || yytoken == Token_CLONE
            || yytoken == Token_EXIT
            || yytoken == Token_ISSET
            || yytoken == Token_EMPTY
            || yytoken == Token_LIST
            || yytoken == Token_ARRAY
            || yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_INCLUDE
            || yytoken == Token_INCLUDE_ONCE
            || yytoken == Token_EVAL
            || yytoken == Token_REQUIRE
            || yytoken == Token_REQUIRE_ONCE
            || yytoken == Token_INT_CAST
            || yytoken == Token_DOUBLE_CAST
            || yytoken == Token_STRING_CAST
            || yytoken == Token_ARRAY_CAST
            || yytoken == Token_OBJECT_CAST
            || yytoken == Token_BOOL_CAST
            || yytoken == Token_UNSET_CAST
            || yytoken == Token_DOUBLE_QUOTE
            || yytoken == Token_LPAREN
            || yytoken == Token_AT
            || yytoken == Token_START_HEREDOC
            || yytoken == Token_BACKTICK
            || yytoken == Token_PLUS
            || yytoken == Token_MINUS
            || yytoken == Token_INC
            || yytoken == Token_DEC
            || yytoken == Token_BANG
            || yytoken == Token_TILDE
            || yytoken == Token_DOLLAR
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_VARIABLE
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER
            || yytoken == Token_STRING
            || yytoken == Token_STRING_VARNAME)
          {
            expr_ast *__node_3 = 0;
            if (!parse_expr(&__node_3))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_3;

            if (yytoken == Token_DOUBLE_ARROW)
              {
                if (yytoken != Token_DOUBLE_ARROW)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_DOUBLE_ARROW, "=>");
                      }
                    return false;
                  }
                yylex();

                if (yytoken == Token_NEW
                    || yytoken == Token_PRINT
                    || yytoken == Token_CLONE
                    || yytoken == Token_EXIT
                    || yytoken == Token_ISSET
                    || yytoken == Token_EMPTY
                    || yytoken == Token_LIST
                    || yytoken == Token_ARRAY
                    || yytoken == Token_CLASS_C
                    || yytoken == Token_METHOD_C
                    || yytoken == Token_FUNC_C
                    || yytoken == Token_LINE
                    || yytoken == Token_FILE
                    || yytoken == Token_INCLUDE
                    || yytoken == Token_INCLUDE_ONCE
                    || yytoken == Token_EVAL
                    || yytoken == Token_REQUIRE
                    || yytoken == Token_REQUIRE_ONCE
                    || yytoken == Token_INT_CAST
                    || yytoken == Token_DOUBLE_CAST
                    || yytoken == Token_STRING_CAST
                    || yytoken == Token_ARRAY_CAST
                    || yytoken == Token_OBJECT_CAST
                    || yytoken == Token_BOOL_CAST
                    || yytoken == Token_UNSET_CAST
                    || yytoken == Token_DOUBLE_QUOTE
                    || yytoken == Token_LPAREN
                    || yytoken == Token_AT
                    || yytoken == Token_START_HEREDOC
                    || yytoken == Token_BACKTICK
                    || yytoken == Token_PLUS
                    || yytoken == Token_MINUS
                    || yytoken == Token_INC
                    || yytoken == Token_DEC
                    || yytoken == Token_BANG
                    || yytoken == Token_TILDE
                    || yytoken == Token_DOLLAR
                    || yytoken == Token_CONSTANT_ENCAPSED_STRING
                    || yytoken == Token_VARIABLE
                    || yytoken == Token_DNUMBER
                    || yytoken == Token_LNUMBER
                    || yytoken == Token_STRING
                    || yytoken == Token_STRING_VARNAME)
                  {
                    expr_ast *__node_4 = 0;
                    if (!parse_expr(&__node_4))
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_symbol(ast_node::Kind_expr, "expr");
                          }
                        return false;
                      }
                    (*yynode)->exprValue = __node_4;

                  }
                else if (yytoken == Token_BIT_AND)
                  {
                    if (yytoken != Token_BIT_AND)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_BIT_AND, "&");
                          }
                        return false;
                      }
                    yylex();

                    variable_ast *__node_5 = 0;
                    if (!parse_variable(&__node_5))
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_symbol(ast_node::Kind_variable, "variable");
                          }
                        return false;
                      }
                    (*yynode)->varValue = __node_5;

                  }
                else
                  {
                    return false;
                  }
              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
          }
        else if (yytoken == Token_BIT_AND)
          {
            if (yytoken != Token_BIT_AND)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BIT_AND, "&");
                  }
                return false;
              }
            yylex();

            variable_ast *__node_6 = 0;
            if (!parse_variable(&__node_6))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variable, "variable");
                  }
                return false;
              }
            (*yynode)->variable = __node_6;

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

  bool parser::parse_assignmentExpression(assignmentExpression_ast **yynode)
  {
    *yynode = create<assignmentExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        m_state.varExpressionIsVariable = false;
        conditionalExpression_ast *__node_7 = 0;
        if (!parse_conditionalExpression(&__node_7))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_conditionalExpression, "conditionalExpression");
              }
            return false;
          }
        (*yynode)->conditionalExpression = __node_7;

        if (yytoken == Token_ASSIGN)
          {
            assignmentExpressionEqual_ast *__node_8 = 0;
            if (!parse_assignmentExpressionEqual(&__node_8))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_assignmentExpressionEqual, "assignmentExpressionEqual");
                  }
                return false;
              }
            (*yynode)->assignmentAxpressionEqual = __node_8;

          }
        else if (yytoken == Token_PLUS_ASSIGN
                 || yytoken == Token_MINUS_ASSIGN
                 || yytoken == Token_MUL_ASSIGN
                 || yytoken == Token_DIV_ASSIGN
                 || yytoken == Token_CONCAT_ASSIGN
                 || yytoken == Token_MOD_ASSIGN
                 || yytoken == Token_AND_ASSIGN
                 || yytoken == Token_OR_ASSIGN
                 || yytoken == Token_XOR_ASSIGN
                 || yytoken == Token_SL_ASSIGN
                 || yytoken == Token_SR_ASSIGN)
          {
            if (yytoken == Token_PLUS_ASSIGN)
              {
                if (yytoken != Token_PLUS_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_PLUS_ASSIGN, "+=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_MINUS_ASSIGN)
              {
                if (yytoken != Token_MINUS_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_MINUS_ASSIGN, "-=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_MUL_ASSIGN)
              {
                if (yytoken != Token_MUL_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_MUL_ASSIGN, "*=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_DIV_ASSIGN)
              {
                if (yytoken != Token_DIV_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_DIV_ASSIGN, "/=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_CONCAT_ASSIGN)
              {
                if (yytoken != Token_CONCAT_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_CONCAT_ASSIGN, ".=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_MOD_ASSIGN)
              {
                if (yytoken != Token_MOD_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_MOD_ASSIGN, "%=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_AND_ASSIGN)
              {
                if (yytoken != Token_AND_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_AND_ASSIGN, "&=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_OR_ASSIGN)
              {
                if (yytoken != Token_OR_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_OR_ASSIGN, "|=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_XOR_ASSIGN)
              {
                if (yytoken != Token_XOR_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_XOR_ASSIGN, "^=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_SL_ASSIGN)
              {
                if (yytoken != Token_SL_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_SL_ASSIGN, "<<=");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_SR_ASSIGN)
              {
                if (yytoken != Token_SR_ASSIGN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_SR_ASSIGN, ">>=");
                      }
                    return false;
                  }
                yylex();

              }
            else
              {
                return false;
              }
            assignmentExpressionCheckIfVariable_ast *__node_9 = 0;
            if (!parse_assignmentExpressionCheckIfVariable(&__node_9))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_assignmentExpressionCheckIfVariable, "assignmentExpressionCheckIfVariable");
                  }
                return false;
              }
            assignmentExpression_ast *__node_10 = 0;
            if (!parse_assignmentExpression(&__node_10))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_assignmentExpression, "assignmentExpression");
                  }
                return false;
              }
            (*yynode)->assignmentExpression = __node_10;

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

  bool parser::parse_assignmentExpressionCheckIfVariable(assignmentExpressionCheckIfVariable_ast **yynode)
  {
    *yynode = create<assignmentExpressionCheckIfVariable_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (true /*epsilon*/ || yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_BIT_AND
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {

        if (!m_state.varExpressionIsVariable)
          {
            reportProblem(Error, "Left side is not a variable");
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

  bool parser::parse_assignmentExpressionEqual(assignmentExpressionEqual_ast **yynode)
  {
    *yynode = create<assignmentExpressionEqual_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ASSIGN)
      {
        if (yytoken != Token_ASSIGN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_ASSIGN, "=");
              }
            return false;
          }
        yylex();

        assignmentExpressionCheckIfVariable_ast *__node_11 = 0;
        if (!parse_assignmentExpressionCheckIfVariable(&__node_11))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_assignmentExpressionCheckIfVariable, "assignmentExpressionCheckIfVariable");
              }
            return false;
          }
        if (yytoken == Token_BIT_AND)
          {
            if (yytoken != Token_BIT_AND)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BIT_AND, "&");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_NEW)
              {
                reportProblem(Warning, "=& new foo() is deprecated");
                m_state.varExpressionState = OnlyNewObject;
              }
            else
              {
                m_state.varExpressionState = OnlyVariable;
              }
          }
        else if (true /*epsilon*/)
        {}
        else
          {
            return false;
          }
        assignmentExpression_ast *__node_12 = 0;
        if (!parse_assignmentExpression(&__node_12))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_assignmentExpression, "assignmentExpression");
              }
            return false;
          }
        (*yynode)->assignmentExpression = __node_12;

        m_state.varExpressionState = Normal;
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_assignmentList(assignmentList_ast **yynode)
  {
    *yynode = create<assignmentList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LIST
        || yytoken == Token_COMMA
        || yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING || yytoken == Token_RPAREN)
      {
        assignmentListElement_ast *__node_13 = 0;
        if (!parse_assignmentListElement(&__node_13))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_assignmentListElement, "assignmentListElement");
              }
            return false;
          }
        (*yynode)->element_sequence = snoc((*yynode)->element_sequence, __node_13, memory_pool);

        while (yytoken == Token_COMMA)
          {
            if (yytoken != Token_COMMA)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_COMMA, ",");
                  }
                return false;
              }
            yylex();

            assignmentListElement_ast *__node_14 = 0;
            if (!parse_assignmentListElement(&__node_14))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_assignmentListElement, "assignmentListElement");
                  }
                return false;
              }
            (*yynode)->element_sequence = snoc((*yynode)->element_sequence, __node_14, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_assignmentListElement(assignmentListElement_ast **yynode)
  {
    *yynode = create<assignmentListElement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LIST
        || yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING || yytoken == Token_RPAREN
        || yytoken == Token_COMMA)
      {
        if (yytoken == Token_DOLLAR
            || yytoken == Token_VARIABLE
            || yytoken == Token_STRING)
          {
            variable_ast *__node_15 = 0;
            if (!parse_variable(&__node_15))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variable, "variable");
                  }
                return false;
              }
            (*yynode)->variable = __node_15;

          }
        else if (yytoken == Token_LIST)
          {
            if (yytoken != Token_LIST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LIST, "list");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            assignmentList_ast *__node_16 = 0;
            if (!parse_assignmentList(&__node_16))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_assignmentList, "assignmentList");
                  }
                return false;
              }
            (*yynode)->assignmentList = __node_16;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
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

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_baseVariable(baseVariable_ast **yynode)
  {
    *yynode = create<baseVariable_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING)
      {
        if (yytoken == Token_DOLLAR
            || yytoken == Token_VARIABLE)
          {
            compoundVariableWithSimpleIndirectReference_ast *__node_17 = 0;
            if (!parse_compoundVariableWithSimpleIndirectReference(&__node_17))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_compoundVariableWithSimpleIndirectReference, "compoundVariableWithSimpleIndirectReference");
                  }
                return false;
              }
            (*yynode)->var = __node_17;

            while (yytoken == Token_LBRACKET
                   || yytoken == Token_LBRACE)
              {
                dimListItem_ast *__node_18 = 0;
                if (!parse_dimListItem(&__node_18))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_dimListItem, "dimListItem");
                      }
                    return false;
                  }
                (*yynode)->offsetItems_sequence = snoc((*yynode)->offsetItems_sequence, __node_18, memory_pool);

              }
          }
        else if (yytoken == Token_STRING)
          {
            staticMember_ast *__node_19 = 0;
            if (!parse_staticMember(&__node_19))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_staticMember, "staticMember");
                  }
                return false;
              }
            (*yynode)->staticMember = __node_19;

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

  bool parser::parse_baseVariableWithFunctionCalls(baseVariableWithFunctionCalls_ast **yynode)
  {
    *yynode = create<baseVariableWithFunctionCalls_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING)
      {
        bool block_errors_1 = block_errors(true);
        std::size_t try_start_token_1 = token_stream->index() - 1;
        parser_state *try_start_state_1 = copy_current_state();
        {
          functionCall_ast *__node_20 = 0;
          if (!parse_functionCall(&__node_20))
            {
              goto __catch_1;
            }
          (*yynode)->functionCall = __node_20;

        }
        block_errors(block_errors_1);
        if (try_start_state_1)
          delete try_start_state_1;

        if (false) // the only way to enter here is using goto
          {
          __catch_1:
            if (try_start_state_1)
              {
                restore_state(try_start_state_1);
                delete try_start_state_1;
              }
            block_errors(block_errors_1);
            rewind(try_start_token_1);

            baseVariable_ast *__node_21 = 0;
            if (!parse_baseVariable(&__node_21))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_baseVariable, "baseVariable");
                  }
                return false;
              }
            (*yynode)->baseVariable = __node_21;

          }

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_bitAndExpression(bitAndExpression_ast **yynode)
  {
    *yynode = create<bitAndExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        equalityExpression_ast *__node_22 = 0;
        if (!parse_equalityExpression(&__node_22))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_equalityExpression, "equalityExpression");
              }
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_22, memory_pool);

        while (yytoken == Token_BIT_AND)
          {
            if (yytoken != Token_BIT_AND)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BIT_AND, "&");
                  }
                return false;
              }
            yylex();

            equalityExpression_ast *__node_23 = 0;
            if (!parse_equalityExpression(&__node_23))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_equalityExpression, "equalityExpression");
                  }
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_23, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_bitOrExpression(bitOrExpression_ast **yynode)
  {
    *yynode = create<bitOrExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        bitXorExpression_ast *__node_24 = 0;
        if (!parse_bitXorExpression(&__node_24))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_bitXorExpression, "bitXorExpression");
              }
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_24, memory_pool);

        while (yytoken == Token_BIT_OR)
          {
            if (yytoken != Token_BIT_OR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BIT_OR, "|");
                  }
                return false;
              }
            yylex();

            bitXorExpression_ast *__node_25 = 0;
            if (!parse_bitXorExpression(&__node_25))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_bitXorExpression, "bitXorExpression");
                  }
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_25, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_bitXorExpression(bitXorExpression_ast **yynode)
  {
    *yynode = create<bitXorExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        bitAndExpression_ast *__node_26 = 0;
        if (!parse_bitAndExpression(&__node_26))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_bitAndExpression, "bitAndExpression");
              }
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_26, memory_pool);

        while (yytoken == Token_BIT_XOR)
          {
            if (yytoken != Token_BIT_XOR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BIT_XOR, "^");
                  }
                return false;
              }
            yylex();

            bitAndExpression_ast *__node_27 = 0;
            if (!parse_bitAndExpression(&__node_27))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_bitAndExpression, "bitAndExpression");
                  }
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_27, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_booleanAndExpression(booleanAndExpression_ast **yynode)
  {
    *yynode = create<booleanAndExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        bitOrExpression_ast *__node_28 = 0;
        if (!parse_bitOrExpression(&__node_28))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_bitOrExpression, "bitOrExpression");
              }
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_28, memory_pool);

        while (yytoken == Token_BOOLEAN_AND)
          {
            if (yytoken != Token_BOOLEAN_AND)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BOOLEAN_AND, "&&");
                  }
                return false;
              }
            yylex();

            bitOrExpression_ast *__node_29 = 0;
            if (!parse_bitOrExpression(&__node_29))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_bitOrExpression, "bitOrExpression");
                  }
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_29, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_booleanOrExpression(booleanOrExpression_ast **yynode)
  {
    *yynode = create<booleanOrExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        booleanAndExpression_ast *__node_30 = 0;
        if (!parse_booleanAndExpression(&__node_30))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_booleanAndExpression, "booleanAndExpression");
              }
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_30, memory_pool);

        while (yytoken == Token_BOOLEAN_OR)
          {
            if (yytoken != Token_BOOLEAN_OR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BOOLEAN_OR, "||");
                  }
                return false;
              }
            yylex();

            booleanAndExpression_ast *__node_31 = 0;
            if (!parse_booleanAndExpression(&__node_31))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_booleanAndExpression, "booleanAndExpression");
                  }
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_31, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_caseList(caseList_ast **yynode)
  {
    *yynode = create<caseList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CASE
        || yytoken == Token_DEFAULT || yytoken == Token_ENDSWITCH
        || yytoken == Token_RBRACE)
      {
        while (yytoken == Token_CASE
               || yytoken == Token_DEFAULT)
          {
            case_item_ast *__node_32 = 0;
            if (!parse_case_item(&__node_32))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_case_item, "case_item");
                  }
                return false;
              }
            (*yynode)->caseItems_sequence = snoc((*yynode)->caseItems_sequence, __node_32, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_case_item(case_item_ast **yynode)
  {
    *yynode = create<case_item_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CASE
        || yytoken == Token_DEFAULT)
      {
        if (yytoken == Token_CASE)
          {
            if (yytoken != Token_CASE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CASE, "case");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_33 = 0;
            if (!parse_expr(&__node_33))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_33;

            if (yytoken == Token_COLON)
              {
                if (yytoken != Token_COLON)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COLON, ":");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_SEMICOLON)
              {
                if (yytoken != Token_SEMICOLON)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_SEMICOLON, ";");
                      }
                    return false;
                  }
                yylex();

              }
            else
              {
                return false;
              }
            innerStatementList_ast *__node_34 = 0;
            if (!parse_innerStatementList(&__node_34))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_34;

          }
        else if (yytoken == Token_DEFAULT)
          {
            if (yytoken != Token_DEFAULT)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DEFAULT, "default");
                  }
                return false;
              }
            (*yynode)->def = token_stream->index() - 1;
            yylex();

            if (yytoken == Token_COLON)
              {
                if (yytoken != Token_COLON)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COLON, ":");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_SEMICOLON)
              {
                if (yytoken != Token_SEMICOLON)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_SEMICOLON, ";");
                      }
                    return false;
                  }
                yylex();

              }
            else
              {
                return false;
              }
            innerStatementList_ast *__node_35 = 0;
            if (!parse_innerStatementList(&__node_35))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_35;

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

  bool parser::parse_catch_item(catch_item_ast **yynode)
  {
    *yynode = create<catch_item_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CATCH)
      {
        if (yytoken != Token_CATCH)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_CATCH, "catch");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_LPAREN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_LPAREN, "(");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_STRING)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_STRING, "string");
              }
            return false;
          }
        (*yynode)->catchClass = token_stream->index() - 1;
        yylex();

        if (yytoken != Token_VARIABLE)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_VARIABLE, "variable");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_RPAREN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_RPAREN, ")");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_LBRACE)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_LBRACE, "{");
              }
            return false;
          }
        yylex();

        innerStatementList_ast *__node_36 = 0;
        if (!parse_innerStatementList(&__node_36))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
              }
            return false;
          }
        (*yynode)->statements = __node_36;

        if (yytoken != Token_RBRACE)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_RBRACE, "}");
              }
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

  bool parser::parse_classConstantDeclaration(classConstantDeclaration_ast **yynode)
  {
    *yynode = create<classConstantDeclaration_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_STRING)
      {
        if (yytoken != Token_STRING)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_STRING, "string");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_ASSIGN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_ASSIGN, "=");
              }
            return false;
          }
        yylex();

        staticScalar_ast *__node_37 = 0;
        if (!parse_staticScalar(&__node_37))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_staticScalar, "staticScalar");
              }
            return false;
          }
        (*yynode)->scalar = __node_37;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_classDeclarationStatement(classDeclarationStatement_ast **yynode)
  {
    *yynode = create<classDeclarationStatement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ABSTRACT
        || yytoken == Token_CLASS
        || yytoken == Token_FINAL
        || yytoken == Token_INTERFACE)
      {
        if (yytoken == Token_ABSTRACT
            || yytoken == Token_CLASS
            || yytoken == Token_FINAL)
          {
            if (yytoken == Token_ABSTRACT)
              {
                if (yytoken != Token_ABSTRACT)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_ABSTRACT, "abstract");
                      }
                    return false;
                  }
                (*yynode)->classType = token_stream->index() - 1;
                yylex();

              }
            else if (yytoken == Token_FINAL)
              {
                if (yytoken != Token_FINAL)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_FINAL, "final");
                      }
                    return false;
                  }
                (*yynode)->classType = token_stream->index() - 1;
                yylex();

              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
            if (yytoken != Token_CLASS)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CLASS, "class");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING, "string");
                  }
                return false;
              }
            (*yynode)->name = token_stream->index() - 1;
            yylex();

            if (yytoken == Token_EXTENDS)
              {
                if (yytoken != Token_EXTENDS)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_EXTENDS, "extends");
                      }
                    return false;
                  }
                yylex();

                if (yytoken != Token_STRING)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_STRING, "string");
                      }
                    return false;
                  }
                (*yynode)->extends = token_stream->index() - 1;
                yylex();

              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
            if (yytoken == Token_IMPLEMENTS)
              {
                if (yytoken != Token_IMPLEMENTS)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_IMPLEMENTS, "implements");
                      }
                    return false;
                  }
                yylex();

                if (yytoken != Token_STRING)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_STRING, "string");
                      }
                    return false;
                  }
                (*yynode)->implments_sequence = snoc((*yynode)->implments_sequence, token_stream->index() - 1, memory_pool);
                yylex();

                while (yytoken == Token_COMMA)
                  {
                    if (yytoken != Token_COMMA)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_COMMA, ",");
                          }
                        return false;
                      }
                    yylex();

                    if (yytoken != Token_STRING)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_STRING, "string");
                          }
                        return false;
                      }
                    (*yynode)->implments_sequence = snoc((*yynode)->implments_sequence, token_stream->index() - 1, memory_pool);
                    yylex();

                  }
              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
            if (yytoken != Token_LBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACE, "{");
                  }
                return false;
              }
            yylex();

            while (yytoken == Token_ABSTRACT
                   || yytoken == Token_CONST
                   || yytoken == Token_FINAL
                   || yytoken == Token_PRIVATE
                   || yytoken == Token_PROTECTED
                   || yytoken == Token_PUBLIC
                   || yytoken == Token_STATIC
                   || yytoken == Token_FUNCTION
                   || yytoken == Token_VAR
                   || yytoken == Token_VARIABLE)
              {
                classStatement_ast *__node_38 = 0;
                if (!parse_classStatement(&__node_38))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_classStatement, "classStatement");
                      }
                    return false;
                  }
                (*yynode)->statements_sequence = snoc((*yynode)->statements_sequence, __node_38, memory_pool);

              }
            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_INTERFACE)
          {
            if (yytoken != Token_INTERFACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_INTERFACE, "interface");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING, "string");
                  }
                return false;
              }
            (*yynode)->name = token_stream->index() - 1;
            yylex();

            if (yytoken == Token_EXTENDS)
              {
                if (yytoken != Token_EXTENDS)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_EXTENDS, "extends");
                      }
                    return false;
                  }
                yylex();

                if (yytoken != Token_STRING)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_STRING, "string");
                      }
                    return false;
                  }
                (*yynode)->implments_sequence = snoc((*yynode)->implments_sequence, token_stream->index() - 1, memory_pool);
                yylex();

                while (yytoken == Token_COMMA)
                  {
                    if (yytoken != Token_COMMA)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_COMMA, ",");
                          }
                        return false;
                      }
                    yylex();

                    if (yytoken != Token_STRING)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_STRING, "string");
                          }
                        return false;
                      }
                    (*yynode)->implments_sequence = snoc((*yynode)->implments_sequence, token_stream->index() - 1, memory_pool);
                    yylex();

                  }
              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
            if (yytoken != Token_LBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACE, "{");
                  }
                return false;
              }
            yylex();

            while (yytoken == Token_ABSTRACT
                   || yytoken == Token_CONST
                   || yytoken == Token_FINAL
                   || yytoken == Token_PRIVATE
                   || yytoken == Token_PROTECTED
                   || yytoken == Token_PUBLIC
                   || yytoken == Token_STATIC
                   || yytoken == Token_FUNCTION
                   || yytoken == Token_VAR
                   || yytoken == Token_VARIABLE)
              {
                classStatement_ast *__node_39 = 0;
                if (!parse_classStatement(&__node_39))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_classStatement, "classStatement");
                      }
                    return false;
                  }
                (*yynode)->statements_sequence = snoc((*yynode)->statements_sequence, __node_39, memory_pool);

              }
            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_classNameReference(classNameReference_ast **yynode)
  {
    *yynode = create<classNameReference_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING)
      {
        if ((yytoken == Token_STRING) && ( LA(2).kind != Token_PAAMAYIM_NEKUDOTAYIM ))
          {
            if (yytoken != Token_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING, "string");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_DOLLAR
                 || yytoken == Token_VARIABLE
                 || yytoken == Token_STRING)
          {
            dynamicClassNameReference_ast *__node_40 = 0;
            if (!parse_dynamicClassNameReference(&__node_40))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_dynamicClassNameReference, "dynamicClassNameReference");
                  }
                return false;
              }
            (*yynode)->dynamicClassNameReference = __node_40;

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

  bool parser::parse_classStatement(classStatement_ast **yynode)
  {
    *yynode = create<classStatement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ABSTRACT
        || yytoken == Token_CONST
        || yytoken == Token_FINAL
        || yytoken == Token_PRIVATE
        || yytoken == Token_PROTECTED
        || yytoken == Token_PUBLIC
        || yytoken == Token_STATIC
        || yytoken == Token_FUNCTION
        || yytoken == Token_VAR
        || yytoken == Token_VARIABLE)
      {
        if (yytoken == Token_CONST)
          {
            if (yytoken != Token_CONST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CONST, "const");
                  }
                return false;
              }
            yylex();

            classConstantDeclaration_ast *__node_41 = 0;
            if (!parse_classConstantDeclaration(&__node_41))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_classConstantDeclaration, "classConstantDeclaration");
                  }
                return false;
              }
            (*yynode)->consts = __node_41;

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                classConstantDeclaration_ast *__node_42 = 0;
                if (!parse_classConstantDeclaration(&__node_42))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_classConstantDeclaration, "classConstantDeclaration");
                      }
                    return false;
                  }
                (*yynode)->consts = __node_42;

              }
            if (yytoken != Token_SEMICOLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SEMICOLON, ";");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_VAR)
          {
            if (yytoken != Token_VAR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_VAR, "var ");
                  }
                return false;
              }
            yylex();

            classVariableDeclaration_ast *__node_43 = 0;
            if (!parse_classVariableDeclaration(&__node_43))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_classVariableDeclaration, "classVariableDeclaration");
                  }
                return false;
              }
            if (yytoken != Token_SEMICOLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SEMICOLON, ";");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_ABSTRACT
                 || yytoken == Token_FINAL
                 || yytoken == Token_PRIVATE
                 || yytoken == Token_PROTECTED
                 || yytoken == Token_PUBLIC
                 || yytoken == Token_STATIC
                 || yytoken == Token_FUNCTION
                 || yytoken == Token_VARIABLE)
          {
            while (yytoken == Token_ABSTRACT
                   || yytoken == Token_FINAL
                   || yytoken == Token_PRIVATE
                   || yytoken == Token_PROTECTED
                   || yytoken == Token_PUBLIC
                   || yytoken == Token_STATIC)
              {
                memberModifier_ast *__node_44 = 0;
                if (!parse_memberModifier(&__node_44))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_memberModifier, "memberModifier");
                      }
                    return false;
                  }
                (*yynode)->modifiers = __node_44;

              }
            if (yytoken == Token_VARIABLE)
              {
                classVariableDeclaration_ast *__node_45 = 0;
                if (!parse_classVariableDeclaration(&__node_45))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_classVariableDeclaration, "classVariableDeclaration");
                      }
                    return false;
                  }
                if (yytoken != Token_SEMICOLON)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_SEMICOLON, ";");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_FUNCTION)
              {
                if (yytoken != Token_FUNCTION)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_FUNCTION, "function");
                      }
                    return false;
                  }
                yylex();

                if (yytoken == Token_BIT_AND)
                  {
                    if (yytoken != Token_BIT_AND)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_BIT_AND, "&");
                          }
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
                if (yytoken != Token_STRING)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_STRING, "string");
                      }
                    return false;
                  }
                yylex();

                if (yytoken != Token_LPAREN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_LPAREN, "(");
                      }
                    return false;
                  }
                yylex();

                parameterList_ast *__node_46 = 0;
                if (!parse_parameterList(&__node_46))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_parameterList, "parameterList");
                      }
                    return false;
                  }
                (*yynode)->params = __node_46;

                if (yytoken != Token_RPAREN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_RPAREN, ")");
                      }
                    return false;
                  }
                yylex();

                methodBody_ast *__node_47 = 0;
                if (!parse_methodBody(&__node_47))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_methodBody, "methodBody");
                      }
                    return false;
                  }
                (*yynode)->body = __node_47;

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
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_classVariable(classVariable_ast **yynode)
  {
    *yynode = create<classVariable_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_VARIABLE)
      {
        if (yytoken != Token_VARIABLE)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_VARIABLE, "variable");
              }
            return false;
          }
        (*yynode)->var = token_stream->index() - 1;
        yylex();

        if (yytoken == Token_ASSIGN)
          {
            if (yytoken != Token_ASSIGN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ASSIGN, "=");
                  }
                return false;
              }
            yylex();

            staticScalar_ast *__node_48 = 0;
            if (!parse_staticScalar(&__node_48))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_staticScalar, "staticScalar");
                  }
                return false;
              }
            (*yynode)->value = __node_48;

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

  bool parser::parse_classVariableDeclaration(classVariableDeclaration_ast **yynode)
  {
    *yynode = create<classVariableDeclaration_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_VARIABLE)
      {
        classVariable_ast *__node_49 = 0;
        if (!parse_classVariable(&__node_49))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_classVariable, "classVariable");
              }
            return false;
          }
        (*yynode)->vars = __node_49;

        while (yytoken == Token_COMMA)
          {
            if (yytoken != Token_COMMA)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_COMMA, ",");
                  }
                return false;
              }
            yylex();

            classVariable_ast *__node_50 = 0;
            if (!parse_classVariable(&__node_50))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_classVariable, "classVariable");
                  }
                return false;
              }
            (*yynode)->vars = __node_50;

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_commonScalar(commonScalar_ast **yynode)
  {
    *yynode = create<commonScalar_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER)
      {
        if (yytoken == Token_LNUMBER)
          {
            if (yytoken != Token_LNUMBER)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LNUMBER, "long number");
                  }
                return false;
              }
            yylex();

            (*yynode)->numType = rphp::LongNumber;
          }
        else if (yytoken == Token_DNUMBER)
          {
            if (yytoken != Token_DNUMBER)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DNUMBER, "double number");
                  }
                return false;
              }
            yylex();

            (*yynode)->numType = rphp::DoubleNumber;
          }
        else if (yytoken == Token_CONSTANT_ENCAPSED_STRING)
          {
            if (yytoken != Token_CONSTANT_ENCAPSED_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CONSTANT_ENCAPSED_STRING, "constant encapsed string");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_LINE)
          {
            if (yytoken != Token_LINE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LINE, "__LINE__");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_FILE)
          {
            if (yytoken != Token_FILE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_FILE, "__FILE__");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_CLASS_C)
          {
            if (yytoken != Token_CLASS_C)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CLASS_C, "__CLASS__");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_METHOD_C)
          {
            if (yytoken != Token_METHOD_C)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_METHOD_C, "__METHOD__");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_FUNC_C)
          {
            if (yytoken != Token_FUNC_C)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_FUNC_C, "__FUNCTION__");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_compoundVariable(compoundVariable_ast **yynode)
  {
    *yynode = create<compoundVariable_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE)
      {
        if (yytoken == Token_VARIABLE)
          {
            if (yytoken != Token_VARIABLE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_VARIABLE, "variable");
                  }
                return false;
              }
            (*yynode)->variable = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_DOLLAR)
          {
            if (yytoken != Token_DOLLAR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DOLLAR, "$");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACE, "{");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_51 = 0;
            if (!parse_expr(&__node_51))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_51;

            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_compoundVariableWithSimpleIndirectReference(compoundVariableWithSimpleIndirectReference_ast **yynode)
  {
    *yynode = create<compoundVariableWithSimpleIndirectReference_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE)
      {
        if (yytoken == Token_DOLLAR)
          {
            if (yytoken != Token_DOLLAR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DOLLAR, "$");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_DOLLAR)
              {
                do
                  {
                    if (yytoken != Token_DOLLAR)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_DOLLAR, "$");
                          }
                        return false;
                      }
                    yylex();

                  }
                while (yytoken == Token_DOLLAR);
              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
            if (yytoken == Token_VARIABLE)
              {
                if (yytoken != Token_VARIABLE)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_VARIABLE, "variable");
                      }
                    return false;
                  }
                (*yynode)->var = token_stream->index() - 1;
                yylex();

              }
            else if (yytoken == Token_LBRACE)
              {
                if (yytoken != Token_LBRACE)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_LBRACE, "{");
                      }
                    return false;
                  }
                yylex();

                expr_ast *__node_52 = 0;
                if (!parse_expr(&__node_52))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->expr = __node_52;

                if (yytoken != Token_RBRACE)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_RBRACE, "}");
                      }
                    return false;
                  }
                yylex();

              }
            else
              {
                return false;
              }
          }
        else if (yytoken == Token_VARIABLE)
          {
            if (yytoken != Token_VARIABLE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_VARIABLE, "variable");
                  }
                return false;
              }
            (*yynode)->var = token_stream->index() - 1;
            yylex();

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

  bool parser::parse_conditionalExpression(conditionalExpression_ast **yynode)
  {
    *yynode = create<conditionalExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        booleanOrExpression_ast *__node_53 = 0;
        if (!parse_booleanOrExpression(&__node_53))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_booleanOrExpression, "booleanOrExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_53;

        if (yytoken == Token_QUESTION)
          {
            if (yytoken != Token_QUESTION)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_QUESTION, "?");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_54 = 0;
            if (!parse_expr(&__node_54))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->ifExpression = __node_54;

            if (yytoken != Token_COLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_COLON, ":");
                  }
                return false;
              }
            yylex();

            conditionalExpression_ast *__node_55 = 0;
            if (!parse_conditionalExpression(&__node_55))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_conditionalExpression, "conditionalExpression");
                  }
                return false;
              }
            (*yynode)->elseExpression = __node_55;

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

  bool parser::parse_ctorArguments(ctorArguments_ast **yynode)
  {
    *yynode = create<ctorArguments_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LPAREN || yytoken == Token_INSTANCEOF
        || yytoken == Token_AS
        || yytoken == Token_DOUBLE_ARROW
        || yytoken == Token_SEMICOLON
        || yytoken == Token_RBRACKET
        || yytoken == Token_RPAREN
        || yytoken == Token_RBRACE
        || yytoken == Token_COMMA
        || yytoken == Token_IS_EQUAL
        || yytoken == Token_IS_NOT_EQUAL
        || yytoken == Token_IS_IDENTICAL
        || yytoken == Token_IS_NOT_IDENTICAL
        || yytoken == Token_IS_SMALLER
        || yytoken == Token_IS_GREATER
        || yytoken == Token_IS_SMALLER_OR_EQUAL
        || yytoken == Token_IS_GREATER_OR_EQUAL
        || yytoken == Token_BOOLEAN_OR
        || yytoken == Token_BOOLEAN_AND
        || yytoken == Token_ASSIGN
        || yytoken == Token_PLUS_ASSIGN
        || yytoken == Token_MINUS_ASSIGN
        || yytoken == Token_MUL_ASSIGN
        || yytoken == Token_DIV_ASSIGN
        || yytoken == Token_CONCAT_ASSIGN
        || yytoken == Token_MOD_ASSIGN
        || yytoken == Token_AND_ASSIGN
        || yytoken == Token_OR_ASSIGN
        || yytoken == Token_XOR_ASSIGN
        || yytoken == Token_SL_ASSIGN
        || yytoken == Token_SR_ASSIGN
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_CONCAT
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_QUESTION
        || yytoken == Token_COLON
        || yytoken == Token_BIT_AND
        || yytoken == Token_BIT_OR
        || yytoken == Token_BIT_XOR
        || yytoken == Token_SL
        || yytoken == Token_SR
        || yytoken == Token_MUL
        || yytoken == Token_DIV
        || yytoken == Token_MOD
        || yytoken == Token_LOGICAL_OR
        || yytoken == Token_LOGICAL_AND
        || yytoken == Token_LOGICAL_XOR
        || yytoken == Token_CLOSE_TAG)
      {
        if (yytoken == Token_LPAREN)
          {
            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            functionCallParameterList_ast *__node_56 = 0;
            if (!parse_functionCallParameterList(&__node_56))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_functionCallParameterList, "functionCallParameterList");
                  }
                return false;
              }
            (*yynode)->parameterList = __node_56;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
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

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_declareItem(declareItem_ast **yynode)
  {
    *yynode = create<declareItem_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_STRING)
      {
        if (yytoken != Token_STRING)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_STRING, "string");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_ASSIGN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_ASSIGN, "=");
              }
            return false;
          }
        yylex();

        staticScalar_ast *__node_57 = 0;
        if (!parse_staticScalar(&__node_57))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_staticScalar, "staticScalar");
              }
            return false;
          }
        (*yynode)->scalar = __node_57;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_declareStatement(declareStatement_ast **yynode)
  {
    *yynode = create<declareStatement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_BREAK
        || yytoken == Token_CONTINUE
        || yytoken == Token_DO
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_FOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_COLON
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO)
      {
        if (yytoken == Token_BREAK
            || yytoken == Token_CONTINUE
            || yytoken == Token_DO
            || yytoken == Token_FOR
            || yytoken == Token_IF
            || yytoken == Token_NEW
            || yytoken == Token_RETURN
            || yytoken == Token_STATIC
            || yytoken == Token_SWITCH
            || yytoken == Token_THROW
            || yytoken == Token_TRY
            || yytoken == Token_WHILE
            || yytoken == Token_ECHO
            || yytoken == Token_PRINT
            || yytoken == Token_CLONE
            || yytoken == Token_EXIT
            || yytoken == Token_FOREACH
            || yytoken == Token_DECLARE
            || yytoken == Token_GLOBAL
            || yytoken == Token_UNSET
            || yytoken == Token_ISSET
            || yytoken == Token_EMPTY
            || yytoken == Token_LIST
            || yytoken == Token_ARRAY
            || yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_INCLUDE
            || yytoken == Token_INCLUDE_ONCE
            || yytoken == Token_EVAL
            || yytoken == Token_REQUIRE
            || yytoken == Token_REQUIRE_ONCE
            || yytoken == Token_INT_CAST
            || yytoken == Token_DOUBLE_CAST
            || yytoken == Token_STRING_CAST
            || yytoken == Token_ARRAY_CAST
            || yytoken == Token_OBJECT_CAST
            || yytoken == Token_BOOL_CAST
            || yytoken == Token_UNSET_CAST
            || yytoken == Token_SEMICOLON
            || yytoken == Token_DOUBLE_QUOTE
            || yytoken == Token_LPAREN
            || yytoken == Token_LBRACE
            || yytoken == Token_AT
            || yytoken == Token_START_HEREDOC
            || yytoken == Token_BACKTICK
            || yytoken == Token_PLUS
            || yytoken == Token_MINUS
            || yytoken == Token_INC
            || yytoken == Token_DEC
            || yytoken == Token_BANG
            || yytoken == Token_TILDE
            || yytoken == Token_DOLLAR
            || yytoken == Token_INLINE_HTML
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_VARIABLE
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER
            || yytoken == Token_STRING
            || yytoken == Token_STRING_VARNAME
            || yytoken == Token_OPEN_TAG
            || yytoken == Token_CLOSE_TAG
            || yytoken == Token_OPEN_TAG_WITH_ECHO)
          {
            statement_ast *__node_58 = 0;
            if (!parse_statement(&__node_58))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_statement, "statement");
                  }
                return false;
              }
            (*yynode)->statement = __node_58;

          }
        else if (yytoken == Token_COLON)
          {
            if (yytoken != Token_COLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_COLON, ":");
                  }
                return false;
              }
            yylex();

            innerStatementList_ast *__node_59 = 0;
            if (!parse_innerStatementList(&__node_59))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_59;

            if (yytoken != Token_ENDDECLARE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ENDDECLARE, "enddeclare");
                  }
                return false;
              }
            yylex();

            semicolonOrCloseTag_ast *__node_60 = 0;
            if (!parse_semicolonOrCloseTag(&__node_60))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
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

  bool parser::parse_dimListItem(dimListItem_ast **yynode)
  {
    *yynode = create<dimListItem_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LBRACKET
        || yytoken == Token_LBRACE)
      {
        if (yytoken == Token_LBRACKET)
          {
            if (yytoken != Token_LBRACKET)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACKET, "[");
                  }
                return false;
              }
            yylex();

            dimOffset_ast *__node_61 = 0;
            if (!parse_dimOffset(&__node_61))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_dimOffset, "dimOffset");
                  }
                return false;
              }
            (*yynode)->dimOffset = __node_61;

            if (yytoken != Token_RBRACKET)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACKET, "]");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_LBRACE)
          {
            if (yytoken != Token_LBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACE, "{");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_62 = 0;
            if (!parse_expr(&__node_62))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_62;

            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_dimOffset(dimOffset_ast **yynode)
  {
    *yynode = create<dimOffset_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME || yytoken == Token_RBRACKET)
      {
        if (yytoken == Token_NEW
            || yytoken == Token_PRINT
            || yytoken == Token_CLONE
            || yytoken == Token_EXIT
            || yytoken == Token_ISSET
            || yytoken == Token_EMPTY
            || yytoken == Token_LIST
            || yytoken == Token_ARRAY
            || yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_INCLUDE
            || yytoken == Token_INCLUDE_ONCE
            || yytoken == Token_EVAL
            || yytoken == Token_REQUIRE
            || yytoken == Token_REQUIRE_ONCE
            || yytoken == Token_INT_CAST
            || yytoken == Token_DOUBLE_CAST
            || yytoken == Token_STRING_CAST
            || yytoken == Token_ARRAY_CAST
            || yytoken == Token_OBJECT_CAST
            || yytoken == Token_BOOL_CAST
            || yytoken == Token_UNSET_CAST
            || yytoken == Token_DOUBLE_QUOTE
            || yytoken == Token_LPAREN
            || yytoken == Token_AT
            || yytoken == Token_START_HEREDOC
            || yytoken == Token_BACKTICK
            || yytoken == Token_PLUS
            || yytoken == Token_MINUS
            || yytoken == Token_INC
            || yytoken == Token_DEC
            || yytoken == Token_BANG
            || yytoken == Token_TILDE
            || yytoken == Token_DOLLAR
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_VARIABLE
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER
            || yytoken == Token_STRING
            || yytoken == Token_STRING_VARNAME)
          {
            expr_ast *__node_63 = 0;
            if (!parse_expr(&__node_63))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_63;

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

  bool parser::parse_dynamicClassNameReference(dynamicClassNameReference_ast **yynode)
  {
    *yynode = create<dynamicClassNameReference_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING)
      {
        baseVariable_ast *__node_64 = 0;
        if (!parse_baseVariable(&__node_64))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_baseVariable, "baseVariable");
              }
            return false;
          }
        (*yynode)->baseVariable = __node_64;

        if (yytoken == Token_OBJECT_OPERATOR)
          {
            if (yytoken != Token_OBJECT_OPERATOR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_OBJECT_OPERATOR, "->");
                  }
                return false;
              }
            yylex();

            objectProperty_ast *__node_65 = 0;
            if (!parse_objectProperty(&__node_65))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_objectProperty, "objectProperty");
                  }
                return false;
              }
            (*yynode)->objectProperty = __node_65;

            dynamicClassNameVariableProperties_ast *__node_66 = 0;
            if (!parse_dynamicClassNameVariableProperties(&__node_66))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_dynamicClassNameVariableProperties, "dynamicClassNameVariableProperties");
                  }
                return false;
              }
            (*yynode)->properties = __node_66;

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

  bool parser::parse_dynamicClassNameVariableProperties(dynamicClassNameVariableProperties_ast **yynode)
  {
    *yynode = create<dynamicClassNameVariableProperties_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_OBJECT_OPERATOR || yytoken == Token_INSTANCEOF
        || yytoken == Token_AS
        || yytoken == Token_DOUBLE_ARROW
        || yytoken == Token_SEMICOLON
        || yytoken == Token_RBRACKET
        || yytoken == Token_LPAREN
        || yytoken == Token_RPAREN
        || yytoken == Token_RBRACE
        || yytoken == Token_COMMA
        || yytoken == Token_IS_EQUAL
        || yytoken == Token_IS_NOT_EQUAL
        || yytoken == Token_IS_IDENTICAL
        || yytoken == Token_IS_NOT_IDENTICAL
        || yytoken == Token_IS_SMALLER
        || yytoken == Token_IS_GREATER
        || yytoken == Token_IS_SMALLER_OR_EQUAL
        || yytoken == Token_IS_GREATER_OR_EQUAL
        || yytoken == Token_BOOLEAN_OR
        || yytoken == Token_BOOLEAN_AND
        || yytoken == Token_ASSIGN
        || yytoken == Token_PLUS_ASSIGN
        || yytoken == Token_MINUS_ASSIGN
        || yytoken == Token_MUL_ASSIGN
        || yytoken == Token_DIV_ASSIGN
        || yytoken == Token_CONCAT_ASSIGN
        || yytoken == Token_MOD_ASSIGN
        || yytoken == Token_AND_ASSIGN
        || yytoken == Token_OR_ASSIGN
        || yytoken == Token_XOR_ASSIGN
        || yytoken == Token_SL_ASSIGN
        || yytoken == Token_SR_ASSIGN
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_CONCAT
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_QUESTION
        || yytoken == Token_COLON
        || yytoken == Token_BIT_AND
        || yytoken == Token_BIT_OR
        || yytoken == Token_BIT_XOR
        || yytoken == Token_SL
        || yytoken == Token_SR
        || yytoken == Token_MUL
        || yytoken == Token_DIV
        || yytoken == Token_MOD
        || yytoken == Token_LOGICAL_OR
        || yytoken == Token_LOGICAL_AND
        || yytoken == Token_LOGICAL_XOR
        || yytoken == Token_CLOSE_TAG)
      {
        while (yytoken == Token_OBJECT_OPERATOR)
          {
            dynamicClassNameVariableProperty_ast *__node_67 = 0;
            if (!parse_dynamicClassNameVariableProperty(&__node_67))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_dynamicClassNameVariableProperty, "dynamicClassNameVariableProperty");
                  }
                return false;
              }
            (*yynode)->properties_sequence = snoc((*yynode)->properties_sequence, __node_67, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_dynamicClassNameVariableProperty(dynamicClassNameVariableProperty_ast **yynode)
  {
    *yynode = create<dynamicClassNameVariableProperty_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_OBJECT_OPERATOR)
      {
        if (yytoken != Token_OBJECT_OPERATOR)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_OBJECT_OPERATOR, "->");
              }
            return false;
          }
        yylex();

        objectProperty_ast *__node_68 = 0;
        if (!parse_objectProperty(&__node_68))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_objectProperty, "objectProperty");
              }
            return false;
          }
        (*yynode)->property = __node_68;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_elseSingle(elseSingle_ast **yynode)
  {
    *yynode = create<elseSingle_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ELSE || yytoken == Token_ABSTRACT
        || yytoken == Token_BREAK
        || yytoken == Token_CASE
        || yytoken == Token_CLASS
        || yytoken == Token_CONTINUE
        || yytoken == Token_DEFAULT
        || yytoken == Token_DO
        || yytoken == Token_ELSE
        || yytoken == Token_FINAL
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_INTERFACE
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ELSEIF
        || yytoken == Token_ENDIF
        || yytoken == Token_ENDWHILE
        || yytoken == Token_ENDFOR
        || yytoken == Token_FOREACH
        || yytoken == Token_ENDFOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_ENDDECLARE
        || yytoken == Token_ENDSWITCH
        || yytoken == Token_FUNCTION
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_HALT_COMPILER
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_RBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO
        || yytoken == Token_EOF)
      {
        if (yytoken == Token_ELSE)
          {
            if (yytoken != Token_ELSE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ELSE, "else");
                  }
                return false;
              }
            yylex();

            statement_ast *__node_69 = 0;
            if (!parse_statement(&__node_69))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_statement, "statement");
                  }
                return false;
              }
            (*yynode)->statement = __node_69;

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

  bool parser::parse_elseifList(elseifList_ast **yynode)
  {
    *yynode = create<elseifList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ELSEIF || yytoken == Token_ABSTRACT
        || yytoken == Token_BREAK
        || yytoken == Token_CASE
        || yytoken == Token_CLASS
        || yytoken == Token_CONTINUE
        || yytoken == Token_DEFAULT
        || yytoken == Token_DO
        || yytoken == Token_ELSE
        || yytoken == Token_FINAL
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_INTERFACE
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ELSEIF
        || yytoken == Token_ENDIF
        || yytoken == Token_ENDWHILE
        || yytoken == Token_ENDFOR
        || yytoken == Token_FOREACH
        || yytoken == Token_ENDFOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_ENDDECLARE
        || yytoken == Token_ENDSWITCH
        || yytoken == Token_FUNCTION
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_HALT_COMPILER
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_RBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO
        || yytoken == Token_EOF)
      {
        while (yytoken == Token_ELSEIF)
          {
            elseifListItem_ast *__node_70 = 0;
            if (!parse_elseifListItem(&__node_70))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_elseifListItem, "elseifListItem");
                  }
                return false;
              }
            (*yynode)->elseifListItem_sequence = snoc((*yynode)->elseifListItem_sequence, __node_70, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_elseifListItem(elseifListItem_ast **yynode)
  {
    *yynode = create<elseifListItem_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ELSEIF)
      {
        if (yytoken != Token_ELSEIF)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_ELSEIF, "elseif");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_LPAREN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_LPAREN, "(");
              }
            return false;
          }
        yylex();

        expr_ast *__node_71 = 0;
        if (!parse_expr(&__node_71))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_expr, "expr");
              }
            return false;
          }
        (*yynode)->expr = __node_71;

        if (yytoken != Token_RPAREN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_RPAREN, ")");
              }
            return false;
          }
        yylex();

        statement_ast *__node_72 = 0;
        if (!parse_statement(&__node_72))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_statement, "statement");
              }
            return false;
          }
        (*yynode)->statement = __node_72;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_encaps(encaps_ast **yynode)
  {
    *yynode = create<encaps_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CURLY_OPEN
        || yytoken == Token_DOLLAR_OPEN_CURLY_BRACES
        || yytoken == Token_VARIABLE
        || yytoken == Token_ENCAPSED_AND_WHITESPACE)
      {
        if (yytoken == Token_CURLY_OPEN
            || yytoken == Token_DOLLAR_OPEN_CURLY_BRACES
            || yytoken == Token_VARIABLE)
          {
            encapsVar_ast *__node_73 = 0;
            if (!parse_encapsVar(&__node_73))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_encapsVar, "encapsVar");
                  }
                return false;
              }
            (*yynode)->var = __node_73;

          }
        else if (yytoken == Token_ENCAPSED_AND_WHITESPACE)
          {
            if (yytoken != Token_ENCAPSED_AND_WHITESPACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ENCAPSED_AND_WHITESPACE, "encapsed and whitespace");
                  }
                return false;
              }
            (*yynode)->value = token_stream->index() - 1;
            yylex();

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

  bool parser::parse_encapsList(encapsList_ast **yynode)
  {
    *yynode = create<encapsList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CURLY_OPEN
        || yytoken == Token_DOLLAR_OPEN_CURLY_BRACES
        || yytoken == Token_VARIABLE
        || yytoken == Token_ENCAPSED_AND_WHITESPACE || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_END_HEREDOC
        || yytoken == Token_BACKTICK)
      {
        while (yytoken == Token_CURLY_OPEN
               || yytoken == Token_DOLLAR_OPEN_CURLY_BRACES
               || yytoken == Token_VARIABLE
               || yytoken == Token_ENCAPSED_AND_WHITESPACE)
          {
            encaps_ast *__node_74 = 0;
            if (!parse_encaps(&__node_74))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_encaps, "encaps");
                  }
                return false;
              }
            (*yynode)->encaps_sequence = snoc((*yynode)->encaps_sequence, __node_74, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_encapsVar(encapsVar_ast **yynode)
  {
    *yynode = create<encapsVar_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CURLY_OPEN
        || yytoken == Token_DOLLAR_OPEN_CURLY_BRACES
        || yytoken == Token_VARIABLE)
      {
        if (yytoken == Token_DOLLAR_OPEN_CURLY_BRACES)
          {
            if (yytoken != Token_DOLLAR_OPEN_CURLY_BRACES)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DOLLAR_OPEN_CURLY_BRACES, "${");
                  }
                return false;
              }
            yylex();

            if ((yytoken == Token_STRING_VARNAME) && ( LA(2).kind == Token_LBRACKET))
              {
                if (yytoken != Token_STRING_VARNAME)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_STRING_VARNAME, "string varname");
                      }
                    return false;
                  }
                yylex();

                if (yytoken != Token_LBRACKET)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_LBRACKET, "[");
                      }
                    return false;
                  }
                yylex();

                expr_ast *__node_75 = 0;
                if (!parse_expr(&__node_75))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->expr = __node_75;

                if (yytoken != Token_RBRACKET)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_RBRACKET, "]");
                      }
                    return false;
                  }
                yylex();

                if (yytoken != Token_RBRACE)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_RBRACE, "}");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_NEW
                     || yytoken == Token_PRINT
                     || yytoken == Token_CLONE
                     || yytoken == Token_EXIT
                     || yytoken == Token_ISSET
                     || yytoken == Token_EMPTY
                     || yytoken == Token_LIST
                     || yytoken == Token_ARRAY
                     || yytoken == Token_CLASS_C
                     || yytoken == Token_METHOD_C
                     || yytoken == Token_FUNC_C
                     || yytoken == Token_LINE
                     || yytoken == Token_FILE
                     || yytoken == Token_INCLUDE
                     || yytoken == Token_INCLUDE_ONCE
                     || yytoken == Token_EVAL
                     || yytoken == Token_REQUIRE
                     || yytoken == Token_REQUIRE_ONCE
                     || yytoken == Token_INT_CAST
                     || yytoken == Token_DOUBLE_CAST
                     || yytoken == Token_STRING_CAST
                     || yytoken == Token_ARRAY_CAST
                     || yytoken == Token_OBJECT_CAST
                     || yytoken == Token_BOOL_CAST
                     || yytoken == Token_UNSET_CAST
                     || yytoken == Token_DOUBLE_QUOTE
                     || yytoken == Token_LPAREN
                     || yytoken == Token_AT
                     || yytoken == Token_START_HEREDOC
                     || yytoken == Token_BACKTICK
                     || yytoken == Token_PLUS
                     || yytoken == Token_MINUS
                     || yytoken == Token_INC
                     || yytoken == Token_DEC
                     || yytoken == Token_BANG
                     || yytoken == Token_TILDE
                     || yytoken == Token_DOLLAR
                     || yytoken == Token_CONSTANT_ENCAPSED_STRING
                     || yytoken == Token_VARIABLE
                     || yytoken == Token_DNUMBER
                     || yytoken == Token_LNUMBER
                     || yytoken == Token_STRING
                     || yytoken == Token_STRING_VARNAME)
              {
                expr_ast *__node_76 = 0;
                if (!parse_expr(&__node_76))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->expr = __node_76;

                if (yytoken != Token_RBRACE)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_RBRACE, "}");
                      }
                    return false;
                  }
                yylex();

              }
            else
              {
                return false;
              }
          }
        else if (yytoken == Token_VARIABLE)
          {
            if (yytoken != Token_VARIABLE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_VARIABLE, "variable");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_OBJECT_OPERATOR)
              {
                if (yytoken != Token_OBJECT_OPERATOR)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_OBJECT_OPERATOR, "->");
                      }
                    return false;
                  }
                yylex();

                if (yytoken != Token_STRING)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_STRING, "string");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_LBRACKET)
              {
                if (yytoken != Token_LBRACKET)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_LBRACKET, "[");
                      }
                    return false;
                  }
                yylex();

                encapsVarOffset_ast *__node_77 = 0;
                if (!parse_encapsVarOffset(&__node_77))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_encapsVarOffset, "encapsVarOffset");
                      }
                    return false;
                  }
                (*yynode)->offset = __node_77;

                if (yytoken != Token_RBRACKET)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_RBRACKET, "]");
                      }
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
        else if (yytoken == Token_CURLY_OPEN)
          {
            if (yytoken != Token_CURLY_OPEN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CURLY_OPEN, "curly open");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_78 = 0;
            if (!parse_expr(&__node_78))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_78;

            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_encapsVarOffset(encapsVarOffset_ast **yynode)
  {
    *yynode = create<encapsVarOffset_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_VARIABLE
        || yytoken == Token_NUM_STRING
        || yytoken == Token_STRING)
      {
        if (yytoken == Token_STRING)
          {
            if (yytoken != Token_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING, "string");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_NUM_STRING)
          {
            if (yytoken != Token_NUM_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_NUM_STRING, "num string");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_VARIABLE)
          {
            if (yytoken != Token_VARIABLE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_VARIABLE, "variable");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_equalityExpression(equalityExpression_ast **yynode)
  {
    *yynode = create<equalityExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        relationalExpression_ast *__node_79 = 0;
        if (!parse_relationalExpression(&__node_79))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_relationalExpression, "relationalExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_79;

        while (yytoken == Token_IS_EQUAL
               || yytoken == Token_IS_NOT_EQUAL
               || yytoken == Token_IS_IDENTICAL
               || yytoken == Token_IS_NOT_IDENTICAL)
          {
            equalityExpressionRest_ast *__node_80 = 0;
            if (!parse_equalityExpressionRest(&__node_80))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_equalityExpressionRest, "equalityExpressionRest");
                  }
                return false;
              }
            (*yynode)->additionalExpression_sequence = snoc((*yynode)->additionalExpression_sequence, __node_80, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_equalityExpressionRest(equalityExpressionRest_ast **yynode)
  {
    *yynode = create<equalityExpressionRest_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IS_EQUAL
        || yytoken == Token_IS_NOT_EQUAL
        || yytoken == Token_IS_IDENTICAL
        || yytoken == Token_IS_NOT_IDENTICAL)
      {
        if (yytoken == Token_IS_EQUAL)
          {
            if (yytoken != Token_IS_EQUAL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_IS_EQUAL, "==");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_IS_NOT_EQUAL)
          {
            if (yytoken != Token_IS_NOT_EQUAL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_IS_NOT_EQUAL, "!=");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_IS_IDENTICAL)
          {
            if (yytoken != Token_IS_IDENTICAL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_IS_IDENTICAL, "===");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_IS_NOT_IDENTICAL)
          {
            if (yytoken != Token_IS_NOT_IDENTICAL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_IS_NOT_IDENTICAL, "!==");
                  }
                return false;
              }
            yylex();

          }
        else
          {
            return false;
          }
        relationalExpression_ast *__node_81 = 0;
        if (!parse_relationalExpression(&__node_81))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_relationalExpression, "relationalExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_81;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_expr(expr_ast **yynode)
  {
    *yynode = create<expr_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        logicalOrExpression_ast *__node_82 = 0;
        if (!parse_logicalOrExpression(&__node_82))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_logicalOrExpression, "logicalOrExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_82;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_forExpr(forExpr_ast **yynode)
  {
    *yynode = create<forExpr_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME || yytoken == Token_SEMICOLON
        || yytoken == Token_RPAREN)
      {
        if (yytoken == Token_NEW
            || yytoken == Token_PRINT
            || yytoken == Token_CLONE
            || yytoken == Token_EXIT
            || yytoken == Token_ISSET
            || yytoken == Token_EMPTY
            || yytoken == Token_LIST
            || yytoken == Token_ARRAY
            || yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_INCLUDE
            || yytoken == Token_INCLUDE_ONCE
            || yytoken == Token_EVAL
            || yytoken == Token_REQUIRE
            || yytoken == Token_REQUIRE_ONCE
            || yytoken == Token_INT_CAST
            || yytoken == Token_DOUBLE_CAST
            || yytoken == Token_STRING_CAST
            || yytoken == Token_ARRAY_CAST
            || yytoken == Token_OBJECT_CAST
            || yytoken == Token_BOOL_CAST
            || yytoken == Token_UNSET_CAST
            || yytoken == Token_DOUBLE_QUOTE
            || yytoken == Token_LPAREN
            || yytoken == Token_AT
            || yytoken == Token_START_HEREDOC
            || yytoken == Token_BACKTICK
            || yytoken == Token_PLUS
            || yytoken == Token_MINUS
            || yytoken == Token_INC
            || yytoken == Token_DEC
            || yytoken == Token_BANG
            || yytoken == Token_TILDE
            || yytoken == Token_DOLLAR
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_VARIABLE
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER
            || yytoken == Token_STRING
            || yytoken == Token_STRING_VARNAME)
          {
            expr_ast *__node_83 = 0;
            if (!parse_expr(&__node_83))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->exprs_sequence = snoc((*yynode)->exprs_sequence, __node_83, memory_pool);

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                expr_ast *__node_84 = 0;
                if (!parse_expr(&__node_84))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->exprs_sequence = snoc((*yynode)->exprs_sequence, __node_84, memory_pool);

              }
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

  bool parser::parse_forStatement(forStatement_ast **yynode)
  {
    *yynode = create<forStatement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_BREAK
        || yytoken == Token_CONTINUE
        || yytoken == Token_DO
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_FOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_COLON
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO)
      {
        if (yytoken == Token_BREAK
            || yytoken == Token_CONTINUE
            || yytoken == Token_DO
            || yytoken == Token_FOR
            || yytoken == Token_IF
            || yytoken == Token_NEW
            || yytoken == Token_RETURN
            || yytoken == Token_STATIC
            || yytoken == Token_SWITCH
            || yytoken == Token_THROW
            || yytoken == Token_TRY
            || yytoken == Token_WHILE
            || yytoken == Token_ECHO
            || yytoken == Token_PRINT
            || yytoken == Token_CLONE
            || yytoken == Token_EXIT
            || yytoken == Token_FOREACH
            || yytoken == Token_DECLARE
            || yytoken == Token_GLOBAL
            || yytoken == Token_UNSET
            || yytoken == Token_ISSET
            || yytoken == Token_EMPTY
            || yytoken == Token_LIST
            || yytoken == Token_ARRAY
            || yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_INCLUDE
            || yytoken == Token_INCLUDE_ONCE
            || yytoken == Token_EVAL
            || yytoken == Token_REQUIRE
            || yytoken == Token_REQUIRE_ONCE
            || yytoken == Token_INT_CAST
            || yytoken == Token_DOUBLE_CAST
            || yytoken == Token_STRING_CAST
            || yytoken == Token_ARRAY_CAST
            || yytoken == Token_OBJECT_CAST
            || yytoken == Token_BOOL_CAST
            || yytoken == Token_UNSET_CAST
            || yytoken == Token_SEMICOLON
            || yytoken == Token_DOUBLE_QUOTE
            || yytoken == Token_LPAREN
            || yytoken == Token_LBRACE
            || yytoken == Token_AT
            || yytoken == Token_START_HEREDOC
            || yytoken == Token_BACKTICK
            || yytoken == Token_PLUS
            || yytoken == Token_MINUS
            || yytoken == Token_INC
            || yytoken == Token_DEC
            || yytoken == Token_BANG
            || yytoken == Token_TILDE
            || yytoken == Token_DOLLAR
            || yytoken == Token_INLINE_HTML
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_VARIABLE
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER
            || yytoken == Token_STRING
            || yytoken == Token_STRING_VARNAME
            || yytoken == Token_OPEN_TAG
            || yytoken == Token_CLOSE_TAG
            || yytoken == Token_OPEN_TAG_WITH_ECHO)
          {
            statement_ast *__node_85 = 0;
            if (!parse_statement(&__node_85))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_statement, "statement");
                  }
                return false;
              }
            (*yynode)->statement = __node_85;

          }
        else if (yytoken == Token_COLON)
          {
            if (yytoken != Token_COLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_COLON, ":");
                  }
                return false;
              }
            yylex();

            innerStatementList_ast *__node_86 = 0;
            if (!parse_innerStatementList(&__node_86))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_86;

            if (yytoken != Token_ENDFOR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ENDFOR, "endfor");
                  }
                return false;
              }
            yylex();

            semicolonOrCloseTag_ast *__node_87 = 0;
            if (!parse_semicolonOrCloseTag(&__node_87))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
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

  bool parser::parse_foreachOptionalArg(foreachOptionalArg_ast **yynode)
  {
    *yynode = create<foreachOptionalArg_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOUBLE_ARROW || yytoken == Token_RPAREN)
      {
        if (yytoken == Token_DOUBLE_ARROW)
          {
            if (yytoken != Token_DOUBLE_ARROW)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DOUBLE_ARROW, "=>");
                  }
                return false;
              }
            yylex();

            foreachVariable_ast *__node_88 = 0;
            if (!parse_foreachVariable(&__node_88))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_foreachVariable, "foreachVariable");
                  }
                return false;
              }
            (*yynode)->foreachVariable = __node_88;

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

  bool parser::parse_foreachStatement(foreachStatement_ast **yynode)
  {
    *yynode = create<foreachStatement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_BREAK
        || yytoken == Token_CONTINUE
        || yytoken == Token_DO
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_FOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_COLON
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO)
      {
        if (yytoken == Token_BREAK
            || yytoken == Token_CONTINUE
            || yytoken == Token_DO
            || yytoken == Token_FOR
            || yytoken == Token_IF
            || yytoken == Token_NEW
            || yytoken == Token_RETURN
            || yytoken == Token_STATIC
            || yytoken == Token_SWITCH
            || yytoken == Token_THROW
            || yytoken == Token_TRY
            || yytoken == Token_WHILE
            || yytoken == Token_ECHO
            || yytoken == Token_PRINT
            || yytoken == Token_CLONE
            || yytoken == Token_EXIT
            || yytoken == Token_FOREACH
            || yytoken == Token_DECLARE
            || yytoken == Token_GLOBAL
            || yytoken == Token_UNSET
            || yytoken == Token_ISSET
            || yytoken == Token_EMPTY
            || yytoken == Token_LIST
            || yytoken == Token_ARRAY
            || yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_INCLUDE
            || yytoken == Token_INCLUDE_ONCE
            || yytoken == Token_EVAL
            || yytoken == Token_REQUIRE
            || yytoken == Token_REQUIRE_ONCE
            || yytoken == Token_INT_CAST
            || yytoken == Token_DOUBLE_CAST
            || yytoken == Token_STRING_CAST
            || yytoken == Token_ARRAY_CAST
            || yytoken == Token_OBJECT_CAST
            || yytoken == Token_BOOL_CAST
            || yytoken == Token_UNSET_CAST
            || yytoken == Token_SEMICOLON
            || yytoken == Token_DOUBLE_QUOTE
            || yytoken == Token_LPAREN
            || yytoken == Token_LBRACE
            || yytoken == Token_AT
            || yytoken == Token_START_HEREDOC
            || yytoken == Token_BACKTICK
            || yytoken == Token_PLUS
            || yytoken == Token_MINUS
            || yytoken == Token_INC
            || yytoken == Token_DEC
            || yytoken == Token_BANG
            || yytoken == Token_TILDE
            || yytoken == Token_DOLLAR
            || yytoken == Token_INLINE_HTML
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_VARIABLE
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER
            || yytoken == Token_STRING
            || yytoken == Token_STRING_VARNAME
            || yytoken == Token_OPEN_TAG
            || yytoken == Token_CLOSE_TAG
            || yytoken == Token_OPEN_TAG_WITH_ECHO)
          {
            statement_ast *__node_89 = 0;
            if (!parse_statement(&__node_89))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_statement, "statement");
                  }
                return false;
              }
            (*yynode)->statement = __node_89;

          }
        else if (yytoken == Token_COLON)
          {
            if (yytoken != Token_COLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_COLON, ":");
                  }
                return false;
              }
            yylex();

            innerStatementList_ast *__node_90 = 0;
            if (!parse_innerStatementList(&__node_90))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_90;

            if (yytoken != Token_ENDFOREACH)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ENDFOREACH, "endforeach");
                  }
                return false;
              }
            yylex();

            semicolonOrCloseTag_ast *__node_91 = 0;
            if (!parse_semicolonOrCloseTag(&__node_91))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
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

  bool parser::parse_foreachVariable(foreachVariable_ast **yynode)
  {
    *yynode = create<foreachVariable_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_BIT_AND
        || yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING)
      {
        if (yytoken == Token_DOLLAR
            || yytoken == Token_VARIABLE
            || yytoken == Token_STRING)
          {
            variable_ast *__node_92 = 0;
            if (!parse_variable(&__node_92))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variable, "variable");
                  }
                return false;
              }
            (*yynode)->variable = __node_92;

          }
        else if (yytoken == Token_BIT_AND)
          {
            if (yytoken != Token_BIT_AND)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BIT_AND, "&");
                  }
                return false;
              }
            yylex();

            variable_ast *__node_93 = 0;
            if (!parse_variable(&__node_93))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variable, "variable");
                  }
                return false;
              }
            (*yynode)->variable = __node_93;

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

  bool parser::parse_functionCall(functionCall_ast **yynode)
  {
    *yynode = create<functionCall_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING)
      {
        if (yytoken == Token_STRING)
          {
            if (yytoken != Token_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING, "string");
                  }
                return false;
              }
            (*yynode)->stringFunctionNameOrClass = token_stream->index() - 1;
            yylex();

            if (yytoken == Token_LPAREN)
              {
                if (yytoken != Token_LPAREN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_LPAREN, "(");
                      }
                    return false;
                  }
                yylex();

                functionCallParameterList_ast *__node_94 = 0;
                if (!parse_functionCallParameterList(&__node_94))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_functionCallParameterList, "functionCallParameterList");
                      }
                    return false;
                  }
                (*yynode)->stringParameterList = __node_94;

                if (yytoken != Token_RPAREN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_RPAREN, ")");
                      }
                    return false;
                  }
                yylex();

              }
            else if (yytoken == Token_PAAMAYIM_NEKUDOTAYIM)
              {
                if (yytoken != Token_PAAMAYIM_NEKUDOTAYIM)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_PAAMAYIM_NEKUDOTAYIM, "::");
                      }
                    return false;
                  }
                yylex();

                if (yytoken == Token_STRING)
                  {
                    if (yytoken != Token_STRING)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_STRING, "string");
                          }
                        return false;
                      }
                    (*yynode)->stringFunctionName = token_stream->index() - 1;
                    yylex();

                    if (yytoken != Token_LPAREN)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_LPAREN, "(");
                          }
                        return false;
                      }
                    yylex();

                    functionCallParameterList_ast *__node_95 = 0;
                    if (!parse_functionCallParameterList(&__node_95))
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_symbol(ast_node::Kind_functionCallParameterList, "functionCallParameterList");
                          }
                        return false;
                      }
                    (*yynode)->stringParameterList = __node_95;

                    if (yytoken != Token_RPAREN)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_RPAREN, ")");
                          }
                        return false;
                      }
                    yylex();

                  }
                else if (yytoken == Token_DOLLAR
                         || yytoken == Token_VARIABLE)
                  {
                    variableWithoutObjects_ast *__node_96 = 0;
                    if (!parse_variableWithoutObjects(&__node_96))
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_symbol(ast_node::Kind_variableWithoutObjects, "variableWithoutObjects");
                          }
                        return false;
                      }
                    (*yynode)->varFunctionName = __node_96;

                    if (yytoken != Token_LPAREN)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_LPAREN, "(");
                          }
                        return false;
                      }
                    yylex();

                    functionCallParameterList_ast *__node_97 = 0;
                    if (!parse_functionCallParameterList(&__node_97))
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_symbol(ast_node::Kind_functionCallParameterList, "functionCallParameterList");
                          }
                        return false;
                      }
                    (*yynode)->stringParameterList = __node_97;

                    if (yytoken != Token_RPAREN)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_RPAREN, ")");
                          }
                        return false;
                      }
                    yylex();

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
          }
        else if (yytoken == Token_DOLLAR
                 || yytoken == Token_VARIABLE)
          {
            variableWithoutObjects_ast *__node_98 = 0;
            if (!parse_variableWithoutObjects(&__node_98))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variableWithoutObjects, "variableWithoutObjects");
                  }
                return false;
              }
            (*yynode)->varFunctionName = __node_98;

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            functionCallParameterList_ast *__node_99 = 0;
            if (!parse_functionCallParameterList(&__node_99))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_functionCallParameterList, "functionCallParameterList");
                  }
                return false;
              }
            (*yynode)->varParameterList = __node_99;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_functionCallParameterList(functionCallParameterList_ast **yynode)
  {
    *yynode = create<functionCallParameterList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_BIT_AND
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME || yytoken == Token_RPAREN)
      {
        if (yytoken == Token_NEW
            || yytoken == Token_PRINT
            || yytoken == Token_CLONE
            || yytoken == Token_EXIT
            || yytoken == Token_ISSET
            || yytoken == Token_EMPTY
            || yytoken == Token_LIST
            || yytoken == Token_ARRAY
            || yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_INCLUDE
            || yytoken == Token_INCLUDE_ONCE
            || yytoken == Token_EVAL
            || yytoken == Token_REQUIRE
            || yytoken == Token_REQUIRE_ONCE
            || yytoken == Token_INT_CAST
            || yytoken == Token_DOUBLE_CAST
            || yytoken == Token_STRING_CAST
            || yytoken == Token_ARRAY_CAST
            || yytoken == Token_OBJECT_CAST
            || yytoken == Token_BOOL_CAST
            || yytoken == Token_UNSET_CAST
            || yytoken == Token_DOUBLE_QUOTE
            || yytoken == Token_LPAREN
            || yytoken == Token_AT
            || yytoken == Token_START_HEREDOC
            || yytoken == Token_BACKTICK
            || yytoken == Token_PLUS
            || yytoken == Token_MINUS
            || yytoken == Token_INC
            || yytoken == Token_DEC
            || yytoken == Token_BANG
            || yytoken == Token_BIT_AND
            || yytoken == Token_TILDE
            || yytoken == Token_DOLLAR
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_VARIABLE
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER
            || yytoken == Token_STRING
            || yytoken == Token_STRING_VARNAME)
          {
            functionCallParameterListElement_ast *__node_100 = 0;
            if (!parse_functionCallParameterListElement(&__node_100))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_functionCallParameterListElement, "functionCallParameterListElement");
                  }
                return false;
              }
            (*yynode)->parameters_sequence = snoc((*yynode)->parameters_sequence, __node_100, memory_pool);

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                functionCallParameterListElement_ast *__node_101 = 0;
                if (!parse_functionCallParameterListElement(&__node_101))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_functionCallParameterListElement, "functionCallParameterListElement");
                      }
                    return false;
                  }
                (*yynode)->parameters_sequence = snoc((*yynode)->parameters_sequence, __node_101, memory_pool);

              }
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

  bool parser::parse_functionCallParameterListElement(functionCallParameterListElement_ast **yynode)
  {
    *yynode = create<functionCallParameterListElement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_BIT_AND
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        if (yytoken == Token_BIT_AND)
          {
            if (yytoken != Token_BIT_AND)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BIT_AND, "&");
                  }
                return false;
              }
            yylex();

            variable_ast *__node_102 = 0;
            if (!parse_variable(&__node_102))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variable, "variable");
                  }
                return false;
              }
            (*yynode)->variable = __node_102;

          }
        else if (yytoken == Token_NEW
                 || yytoken == Token_PRINT
                 || yytoken == Token_CLONE
                 || yytoken == Token_EXIT
                 || yytoken == Token_ISSET
                 || yytoken == Token_EMPTY
                 || yytoken == Token_LIST
                 || yytoken == Token_ARRAY
                 || yytoken == Token_CLASS_C
                 || yytoken == Token_METHOD_C
                 || yytoken == Token_FUNC_C
                 || yytoken == Token_LINE
                 || yytoken == Token_FILE
                 || yytoken == Token_INCLUDE
                 || yytoken == Token_INCLUDE_ONCE
                 || yytoken == Token_EVAL
                 || yytoken == Token_REQUIRE
                 || yytoken == Token_REQUIRE_ONCE
                 || yytoken == Token_INT_CAST
                 || yytoken == Token_DOUBLE_CAST
                 || yytoken == Token_STRING_CAST
                 || yytoken == Token_ARRAY_CAST
                 || yytoken == Token_OBJECT_CAST
                 || yytoken == Token_BOOL_CAST
                 || yytoken == Token_UNSET_CAST
                 || yytoken == Token_DOUBLE_QUOTE
                 || yytoken == Token_LPAREN
                 || yytoken == Token_AT
                 || yytoken == Token_START_HEREDOC
                 || yytoken == Token_BACKTICK
                 || yytoken == Token_PLUS
                 || yytoken == Token_MINUS
                 || yytoken == Token_INC
                 || yytoken == Token_DEC
                 || yytoken == Token_BANG
                 || yytoken == Token_TILDE
                 || yytoken == Token_DOLLAR
                 || yytoken == Token_CONSTANT_ENCAPSED_STRING
                 || yytoken == Token_VARIABLE
                 || yytoken == Token_DNUMBER
                 || yytoken == Token_LNUMBER
                 || yytoken == Token_STRING
                 || yytoken == Token_STRING_VARNAME)
          {
            expr_ast *__node_103 = 0;
            if (!parse_expr(&__node_103))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_103;

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

  bool parser::parse_functionDeclarationStatement(functionDeclarationStatement_ast **yynode)
  {
    *yynode = create<functionDeclarationStatement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_FUNCTION)
      {
        if (yytoken != Token_FUNCTION)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_FUNCTION, "function");
              }
            return false;
          }
        yylex();

        if (yytoken == Token_BIT_AND)
          {
            if (yytoken != Token_BIT_AND)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BIT_AND, "&");
                  }
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
        if (yytoken != Token_STRING)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_STRING, "string");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_LPAREN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_LPAREN, "(");
              }
            return false;
          }
        yylex();

        parameterList_ast *__node_104 = 0;
        if (!parse_parameterList(&__node_104))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_parameterList, "parameterList");
              }
            return false;
          }
        (*yynode)->params = __node_104;

        if (yytoken != Token_RPAREN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_RPAREN, ")");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_LBRACE)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_LBRACE, "{");
              }
            return false;
          }
        yylex();

        innerStatementList_ast *__node_105 = 0;
        if (!parse_innerStatementList(&__node_105))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
              }
            return false;
          }
        (*yynode)->statements = __node_105;

        if (yytoken != Token_RBRACE)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_RBRACE, "}");
              }
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

  bool parser::parse_globalVar(globalVar_ast **yynode)
  {
    *yynode = create<globalVar_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE)
      {
        if (yytoken == Token_VARIABLE)
          {
            if (yytoken != Token_VARIABLE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_VARIABLE, "variable");
                  }
                return false;
              }
            (*yynode)->var = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_DOLLAR)
          {
            if (yytoken != Token_DOLLAR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DOLLAR, "$");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_DOLLAR
                || yytoken == Token_VARIABLE
                || yytoken == Token_STRING)
              {
                variable_ast *__node_106 = 0;
                if (!parse_variable(&__node_106))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_variable, "variable");
                      }
                    return false;
                  }
                (*yynode)->dollarVar = __node_106;

              }
            else if (yytoken == Token_LBRACE)
              {
                if (yytoken != Token_LBRACE)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_LBRACE, "{");
                      }
                    return false;
                  }
                yylex();

                expr_ast *__node_107 = 0;
                if (!parse_expr(&__node_107))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->expr = __node_107;

                if (yytoken != Token_RBRACE)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_RBRACE, "}");
                      }
                    return false;
                  }
                yylex();

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
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_innerStatementList(innerStatementList_ast **yynode)
  {
    *yynode = create<innerStatementList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ABSTRACT
        || yytoken == Token_BREAK
        || yytoken == Token_CLASS
        || yytoken == Token_CONTINUE
        || yytoken == Token_DO
        || yytoken == Token_FINAL
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_INTERFACE
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_FOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_FUNCTION
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_HALT_COMPILER
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO || yytoken == Token_CASE
        || yytoken == Token_DEFAULT
        || yytoken == Token_ELSE
        || yytoken == Token_ELSEIF
        || yytoken == Token_ENDIF
        || yytoken == Token_ENDWHILE
        || yytoken == Token_ENDFOR
        || yytoken == Token_ENDFOREACH
        || yytoken == Token_ENDDECLARE
        || yytoken == Token_ENDSWITCH
        || yytoken == Token_RBRACE
        || yytoken == Token_EOF)
      {
        while (yytoken == Token_ABSTRACT
               || yytoken == Token_BREAK
               || yytoken == Token_CLASS
               || yytoken == Token_CONTINUE
               || yytoken == Token_DO
               || yytoken == Token_FINAL
               || yytoken == Token_FOR
               || yytoken == Token_IF
               || yytoken == Token_INTERFACE
               || yytoken == Token_NEW
               || yytoken == Token_RETURN
               || yytoken == Token_STATIC
               || yytoken == Token_SWITCH
               || yytoken == Token_THROW
               || yytoken == Token_TRY
               || yytoken == Token_WHILE
               || yytoken == Token_ECHO
               || yytoken == Token_PRINT
               || yytoken == Token_CLONE
               || yytoken == Token_EXIT
               || yytoken == Token_FOREACH
               || yytoken == Token_DECLARE
               || yytoken == Token_FUNCTION
               || yytoken == Token_GLOBAL
               || yytoken == Token_UNSET
               || yytoken == Token_ISSET
               || yytoken == Token_EMPTY
               || yytoken == Token_HALT_COMPILER
               || yytoken == Token_LIST
               || yytoken == Token_ARRAY
               || yytoken == Token_CLASS_C
               || yytoken == Token_METHOD_C
               || yytoken == Token_FUNC_C
               || yytoken == Token_LINE
               || yytoken == Token_FILE
               || yytoken == Token_INCLUDE
               || yytoken == Token_INCLUDE_ONCE
               || yytoken == Token_EVAL
               || yytoken == Token_REQUIRE
               || yytoken == Token_REQUIRE_ONCE
               || yytoken == Token_INT_CAST
               || yytoken == Token_DOUBLE_CAST
               || yytoken == Token_STRING_CAST
               || yytoken == Token_ARRAY_CAST
               || yytoken == Token_OBJECT_CAST
               || yytoken == Token_BOOL_CAST
               || yytoken == Token_UNSET_CAST
               || yytoken == Token_SEMICOLON
               || yytoken == Token_DOUBLE_QUOTE
               || yytoken == Token_LPAREN
               || yytoken == Token_LBRACE
               || yytoken == Token_AT
               || yytoken == Token_START_HEREDOC
               || yytoken == Token_BACKTICK
               || yytoken == Token_PLUS
               || yytoken == Token_MINUS
               || yytoken == Token_INC
               || yytoken == Token_DEC
               || yytoken == Token_BANG
               || yytoken == Token_TILDE
               || yytoken == Token_DOLLAR
               || yytoken == Token_INLINE_HTML
               || yytoken == Token_CONSTANT_ENCAPSED_STRING
               || yytoken == Token_VARIABLE
               || yytoken == Token_DNUMBER
               || yytoken == Token_LNUMBER
               || yytoken == Token_STRING
               || yytoken == Token_STRING_VARNAME
               || yytoken == Token_OPEN_TAG
               || yytoken == Token_CLOSE_TAG
               || yytoken == Token_OPEN_TAG_WITH_ECHO)
          {
            topStatement_ast *__node_108 = 0;
            if (!parse_topStatement(&__node_108))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_topStatement, "topStatement");
                  }
                return false;
              }
            (*yynode)->statements_sequence = snoc((*yynode)->statements_sequence, __node_108, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_logicalAndExpression(logicalAndExpression_ast **yynode)
  {
    *yynode = create<logicalAndExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        printExpression_ast *__node_109 = 0;
        if (!parse_printExpression(&__node_109))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_printExpression, "printExpression");
              }
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_109, memory_pool);

        while (yytoken == Token_LOGICAL_AND)
          {
            if (yytoken != Token_LOGICAL_AND)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LOGICAL_AND, "logical and");
                  }
                return false;
              }
            yylex();

            printExpression_ast *__node_110 = 0;
            if (!parse_printExpression(&__node_110))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_printExpression, "printExpression");
                  }
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_110, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_logicalOrExpression(logicalOrExpression_ast **yynode)
  {
    *yynode = create<logicalOrExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        logicalXorExpression_ast *__node_111 = 0;
        if (!parse_logicalXorExpression(&__node_111))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_logicalXorExpression, "logicalXorExpression");
              }
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_111, memory_pool);

        while (yytoken == Token_LOGICAL_OR)
          {
            if (yytoken != Token_LOGICAL_OR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LOGICAL_OR, "logical or");
                  }
                return false;
              }
            yylex();

            logicalXorExpression_ast *__node_112 = 0;
            if (!parse_logicalXorExpression(&__node_112))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_logicalXorExpression, "logicalXorExpression");
                  }
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_112, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_logicalXorExpression(logicalXorExpression_ast **yynode)
  {
    *yynode = create<logicalXorExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        logicalAndExpression_ast *__node_113 = 0;
        if (!parse_logicalAndExpression(&__node_113))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_logicalAndExpression, "logicalAndExpression");
              }
            return false;
          }
        (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_113, memory_pool);

        while (yytoken == Token_LOGICAL_XOR)
          {
            if (yytoken != Token_LOGICAL_XOR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LOGICAL_XOR, "logical xor");
                  }
                return false;
              }
            yylex();

            logicalAndExpression_ast *__node_114 = 0;
            if (!parse_logicalAndExpression(&__node_114))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_logicalAndExpression, "logicalAndExpression");
                  }
                return false;
              }
            (*yynode)->expression_sequence = snoc((*yynode)->expression_sequence, __node_114, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_memberModifier(memberModifier_ast **yynode)
  {
    *yynode = create<memberModifier_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ABSTRACT
        || yytoken == Token_FINAL
        || yytoken == Token_PRIVATE
        || yytoken == Token_PROTECTED
        || yytoken == Token_PUBLIC
        || yytoken == Token_STATIC)
      {
        if (yytoken == Token_PUBLIC)
          {
            if (yytoken != Token_PUBLIC)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_PUBLIC, "public");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_PROTECTED)
          {
            if (yytoken != Token_PROTECTED)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_PROTECTED, "protected");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_PRIVATE)
          {
            if (yytoken != Token_PRIVATE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_PRIVATE, "private");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_STATIC)
          {
            if (yytoken != Token_STATIC)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STATIC, "static");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_ABSTRACT)
          {
            if (yytoken != Token_ABSTRACT)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ABSTRACT, "abstract");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_FINAL)
          {
            if (yytoken != Token_FINAL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_FINAL, "final");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_methodBody(methodBody_ast **yynode)
  {
    *yynode = create<methodBody_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_SEMICOLON
        || yytoken == Token_LBRACE)
      {
        if (yytoken == Token_SEMICOLON)
          {
            if (yytoken != Token_SEMICOLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SEMICOLON, ";");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_LBRACE)
          {
            if (yytoken != Token_LBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACE, "{");
                  }
                return false;
              }
            yylex();

            innerStatementList_ast *__node_115 = 0;
            if (!parse_innerStatementList(&__node_115))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_115;

            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_multiplicativeExpression(multiplicativeExpression_ast **yynode)
  {
    *yynode = create<multiplicativeExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        unaryExpression_ast *__node_116 = 0;
        if (!parse_unaryExpression(&__node_116))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_116;

        while (yytoken == Token_MUL
               || yytoken == Token_DIV
               || yytoken == Token_MOD)
          {
            multiplicativeExpression_rest_ast *__node_117 = 0;
            if (!parse_multiplicativeExpression_rest(&__node_117))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_multiplicativeExpression_rest, "multiplicativeExpression_rest");
                  }
                return false;
              }
            (*yynode)->additionalExpression_sequence = snoc((*yynode)->additionalExpression_sequence, __node_117, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_multiplicativeExpression_rest(multiplicativeExpression_rest_ast **yynode)
  {
    *yynode = create<multiplicativeExpression_rest_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_MUL
        || yytoken == Token_DIV
        || yytoken == Token_MOD)
      {
        if (yytoken == Token_MUL)
          {
            if (yytoken != Token_MUL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_MUL, "*");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_DIV)
          {
            if (yytoken != Token_DIV)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DIV, "/");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_MOD)
          {
            if (yytoken != Token_MOD)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_MOD, "%");
                  }
                return false;
              }
            yylex();

          }
        else
          {
            return false;
          }
        unaryExpression_ast *__node_118 = 0;
        if (!parse_unaryExpression(&__node_118))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_118;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_newElseSingle(newElseSingle_ast **yynode)
  {
    *yynode = create<newElseSingle_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ELSE || yytoken == Token_ENDIF)
      {
        if (yytoken == Token_ELSE)
          {
            if (yytoken != Token_ELSE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ELSE, "else");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_COLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_COLON, ":");
                  }
                return false;
              }
            yylex();

            innerStatementList_ast *__node_119 = 0;
            if (!parse_innerStatementList(&__node_119))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_119;

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

  bool parser::parse_newElseifList(newElseifList_ast **yynode)
  {
    *yynode = create<newElseifList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ELSEIF || yytoken == Token_ELSE
        || yytoken == Token_ENDIF)
      {
        while (yytoken == Token_ELSEIF)
          {
            newelseifListItem_ast *__node_120 = 0;
            if (!parse_newelseifListItem(&__node_120))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_newelseifListItem, "newelseifListItem");
                  }
                return false;
              }
            (*yynode)->newElseifListItem_sequence = snoc((*yynode)->newElseifListItem_sequence, __node_120, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_newelseifListItem(newelseifListItem_ast **yynode)
  {
    *yynode = create<newelseifListItem_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ELSEIF)
      {
        if (yytoken != Token_ELSEIF)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_ELSEIF, "elseif");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_LPAREN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_LPAREN, "(");
              }
            return false;
          }
        yylex();

        expr_ast *__node_121 = 0;
        if (!parse_expr(&__node_121))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_expr, "expr");
              }
            return false;
          }
        (*yynode)->expr = __node_121;

        if (yytoken != Token_RPAREN)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_RPAREN, ")");
              }
            return false;
          }
        yylex();

        if (yytoken != Token_COLON)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_COLON, ":");
              }
            return false;
          }
        yylex();

        innerStatementList_ast *__node_122 = 0;
        if (!parse_innerStatementList(&__node_122))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
              }
            return false;
          }
        (*yynode)->statements = __node_122;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_objectDimList(objectDimList_ast **yynode)
  {
    *yynode = create<objectDimList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LBRACE
        || yytoken == Token_STRING)
      {
        variableName_ast *__node_123 = 0;
        if (!parse_variableName(&__node_123))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_variableName, "variableName");
              }
            return false;
          }
        (*yynode)->variableName = __node_123;

        while (yytoken == Token_LBRACKET
               || yytoken == Token_LBRACE)
          {
            dimListItem_ast *__node_124 = 0;
            if (!parse_dimListItem(&__node_124))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_dimListItem, "dimListItem");
                  }
                return false;
              }
            (*yynode)->offsetItems_sequence = snoc((*yynode)->offsetItems_sequence, __node_124, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_objectProperty(objectProperty_ast **yynode)
  {
    *yynode = create<objectProperty_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LBRACE
        || yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING)
      {
        if (yytoken == Token_LBRACE
            || yytoken == Token_STRING)
          {
            objectDimList_ast *__node_125 = 0;
            if (!parse_objectDimList(&__node_125))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_objectDimList, "objectDimList");
                  }
                return false;
              }
            (*yynode)->objectDimList = __node_125;

          }
        else if (yytoken == Token_DOLLAR
                 || yytoken == Token_VARIABLE)
          {
            variableWithoutObjects_ast *__node_126 = 0;
            if (!parse_variableWithoutObjects(&__node_126))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variableWithoutObjects, "variableWithoutObjects");
                  }
                return false;
              }
            (*yynode)->variableWithoutObjects = __node_126;

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

  bool parser::parse_parameter(parameter_ast **yynode)
  {
    *yynode = create<parameter_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ARRAY
        || yytoken == Token_BIT_AND
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING)
      {
        if (yytoken == Token_STRING)
          {
            if (yytoken != Token_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING, "string");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_ARRAY)
          {
            if (yytoken != Token_ARRAY)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ARRAY, "array");
                  }
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
        if (yytoken == Token_BIT_AND)
          {
            if (yytoken != Token_BIT_AND)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BIT_AND, "&");
                  }
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
        if (yytoken != Token_VARIABLE)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_VARIABLE, "variable");
              }
            return false;
          }
        yylex();

        if (yytoken == Token_ASSIGN)
          {
            if (yytoken != Token_ASSIGN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ASSIGN, "=");
                  }
                return false;
              }
            yylex();

            staticScalar_ast *__node_127 = 0;
            if (!parse_staticScalar(&__node_127))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_staticScalar, "staticScalar");
                  }
                return false;
              }
            (*yynode)->defaultValue = __node_127;

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

  bool parser::parse_parameterList(parameterList_ast **yynode)
  {
    *yynode = create<parameterList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ARRAY
        || yytoken == Token_BIT_AND
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING || yytoken == Token_RPAREN)
      {
        if (yytoken == Token_ARRAY
            || yytoken == Token_BIT_AND
            || yytoken == Token_VARIABLE
            || yytoken == Token_STRING)
          {
            parameter_ast *__node_128 = 0;
            if (!parse_parameter(&__node_128))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_parameter, "parameter");
                  }
                return false;
              }
            (*yynode)->params_sequence = snoc((*yynode)->params_sequence, __node_128, memory_pool);

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                parameter_ast *__node_129 = 0;
                if (!parse_parameter(&__node_129))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_parameter, "parameter");
                      }
                    return false;
                  }
                (*yynode)->params_sequence = snoc((*yynode)->params_sequence, __node_129, memory_pool);

              }
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

  bool parser::parse_postprefixOperator(postprefixOperator_ast **yynode)
  {
    *yynode = create<postprefixOperator_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_INC
        || yytoken == Token_DEC)
      {
        if (yytoken == Token_INC)
          {
            if (yytoken != Token_INC)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_INC, "++");
                  }
                return false;
              }
            (*yynode)->op = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_DEC)
          {
            if (yytoken != Token_DEC)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DEC, "--");
                  }
                return false;
              }
            (*yynode)->op = token_stream->index() - 1;
            yylex();

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

  bool parser::parse_printExpression(printExpression_ast **yynode)
  {
    *yynode = create<printExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        while (yytoken == Token_PRINT)
          {
            if (yytoken != Token_PRINT)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_PRINT, "print");
                  }
                return false;
              }
            (*yynode)->print = token_stream->index() - 1;
            yylex();

          }
        assignmentExpression_ast *__node_130 = 0;
        if (!parse_assignmentExpression(&__node_130))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_assignmentExpression, "assignmentExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_130;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_relationalExpression(relationalExpression_ast **yynode)
  {
    *yynode = create<relationalExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        shiftExpression_ast *__node_131 = 0;
        if (!parse_shiftExpression(&__node_131))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_shiftExpression, "shiftExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_131;

        if (yytoken == Token_IS_SMALLER
            || yytoken == Token_IS_GREATER
            || yytoken == Token_IS_SMALLER_OR_EQUAL
            || yytoken == Token_IS_GREATER_OR_EQUAL)
          {
            do
              {
                relationalExpressionRest_ast *__node_132 = 0;
                if (!parse_relationalExpressionRest(&__node_132))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_relationalExpressionRest, "relationalExpressionRest");
                      }
                    return false;
                  }
                (*yynode)->additionalExpression_sequence = snoc((*yynode)->additionalExpression_sequence, __node_132, memory_pool);

              }
            while (yytoken == Token_IS_SMALLER
                   || yytoken == Token_IS_GREATER
                   || yytoken == Token_IS_SMALLER_OR_EQUAL
                   || yytoken == Token_IS_GREATER_OR_EQUAL);
          }
        else if (yytoken == Token_INSTANCEOF)
          {
            if (yytoken != Token_INSTANCEOF)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_INSTANCEOF, "instanceof");
                  }
                return false;
              }
            yylex();

            classNameReference_ast *__node_133 = 0;
            if (!parse_classNameReference(&__node_133))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_classNameReference, "classNameReference");
                  }
                return false;
              }
            (*yynode)->instanceofType = __node_133;

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

  bool parser::parse_relationalExpressionRest(relationalExpressionRest_ast **yynode)
  {
    *yynode = create<relationalExpressionRest_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_IS_SMALLER
        || yytoken == Token_IS_GREATER
        || yytoken == Token_IS_SMALLER_OR_EQUAL
        || yytoken == Token_IS_GREATER_OR_EQUAL)
      {
        if (yytoken == Token_IS_SMALLER)
          {
            if (yytoken != Token_IS_SMALLER)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_IS_SMALLER, "<");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_IS_GREATER)
          {
            if (yytoken != Token_IS_GREATER)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_IS_GREATER, ">");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_IS_SMALLER_OR_EQUAL)
          {
            if (yytoken != Token_IS_SMALLER_OR_EQUAL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_IS_SMALLER_OR_EQUAL, "<=");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_IS_GREATER_OR_EQUAL)
          {
            if (yytoken != Token_IS_GREATER_OR_EQUAL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_IS_GREATER_OR_EQUAL, ">=");
                  }
                return false;
              }
            yylex();

          }
        else
          {
            return false;
          }
        shiftExpression_ast *__node_134 = 0;
        if (!parse_shiftExpression(&__node_134))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_shiftExpression, "shiftExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_134;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_scalar(scalar_ast **yynode)
  {
    *yynode = create<scalar_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        if (yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER)
          {
            commonScalar_ast *__node_135 = 0;
            if (!parse_commonScalar(&__node_135))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_commonScalar, "commonScalar");
                  }
                return false;
              }
            (*yynode)->commonScalar = __node_135;

          }
        else if (yytoken == Token_STRING)
          {
            if (yytoken != Token_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING, "string");
                  }
                return false;
              }
            (*yynode)->string = token_stream->index() - 1;
            yylex();

            if (yytoken == Token_PAAMAYIM_NEKUDOTAYIM)
              {
                if (yytoken != Token_PAAMAYIM_NEKUDOTAYIM)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_PAAMAYIM_NEKUDOTAYIM, "::");
                      }
                    return false;
                  }
                yylex();

                if (yytoken != Token_STRING)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_STRING, "string");
                      }
                    return false;
                  }
                (*yynode)->constname = token_stream->index() - 1;
                yylex();

              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
          }
        else if (yytoken == Token_STRING_VARNAME)
          {
            if (yytoken != Token_STRING_VARNAME)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING_VARNAME, "string varname");
                  }
                return false;
              }
            (*yynode)->varname = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_DOUBLE_QUOTE)
          {
            if (yytoken != Token_DOUBLE_QUOTE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DOUBLE_QUOTE, "\"");
                  }
                return false;
              }
            yylex();

            encapsList_ast *__node_136 = 0;
            if (!parse_encapsList(&__node_136))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_encapsList, "encapsList");
                  }
                return false;
              }
            (*yynode)->encapsList = __node_136;

            if (yytoken != Token_DOUBLE_QUOTE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DOUBLE_QUOTE, "\"");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_START_HEREDOC)
          {
            if (yytoken != Token_START_HEREDOC)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_START_HEREDOC, "start heredoc");
                  }
                return false;
              }
            yylex();

            encapsList_ast *__node_137 = 0;
            if (!parse_encapsList(&__node_137))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_encapsList, "encapsList");
                  }
                return false;
              }
            (*yynode)->encapsList = __node_137;

            if (yytoken != Token_END_HEREDOC)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_END_HEREDOC, "end heredoc");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_semicolonOrCloseTag(semicolonOrCloseTag_ast **yynode)
  {
    *yynode = create<semicolonOrCloseTag_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_SEMICOLON
        || yytoken == Token_CLOSE_TAG)
      {
        if (yytoken == Token_SEMICOLON)
          {
            if (yytoken != Token_SEMICOLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SEMICOLON, ";");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_CLOSE_TAG)
          {
            if (yytoken != Token_CLOSE_TAG)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CLOSE_TAG, "?>");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_shiftExpression(shiftExpression_ast **yynode)
  {
    *yynode = create<shiftExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        additiveExpression_ast *__node_138 = 0;
        if (!parse_additiveExpression(&__node_138))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_additiveExpression, "additiveExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_138;

        while (yytoken == Token_SL
               || yytoken == Token_SR)
          {
            shiftExpressionRest_ast *__node_139 = 0;
            if (!parse_shiftExpressionRest(&__node_139))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_shiftExpressionRest, "shiftExpressionRest");
                  }
                return false;
              }
            (*yynode)->additionalExpression_sequence = snoc((*yynode)->additionalExpression_sequence, __node_139, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_shiftExpressionRest(shiftExpressionRest_ast **yynode)
  {
    *yynode = create<shiftExpressionRest_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_SL
        || yytoken == Token_SR)
      {
        if (yytoken == Token_SL)
          {
            if (yytoken != Token_SL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SL, "<<");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_SR)
          {
            if (yytoken != Token_SR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SR, ">>");
                  }
                return false;
              }
            yylex();

          }
        else
          {
            return false;
          }
        additiveExpression_ast *__node_140 = 0;
        if (!parse_additiveExpression(&__node_140))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_additiveExpression, "additiveExpression");
              }
            return false;
          }
        (*yynode)->expression = __node_140;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_start(start_ast **yynode)
  {
    *yynode = create<start_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ABSTRACT
        || yytoken == Token_BREAK
        || yytoken == Token_CLASS
        || yytoken == Token_CONTINUE
        || yytoken == Token_DO
        || yytoken == Token_FINAL
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_INTERFACE
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_FOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_FUNCTION
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_HALT_COMPILER
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO || yytoken == Token_EOF)
      {
        innerStatementList_ast *__node_141 = 0;
        if (!parse_innerStatementList(&__node_141))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
              }
            return false;
          }
        (*yynode)->statements = __node_141;

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

  bool parser::parse_statement(statement_ast **yynode)
  {
    *yynode = create<statement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_BREAK
        || yytoken == Token_CONTINUE
        || yytoken == Token_DO
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_FOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO)
      {
        if (yytoken == Token_LBRACE)
          {
            if (yytoken != Token_LBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACE, "{");
                  }
                return false;
              }
            yylex();

            innerStatementList_ast *__node_142 = 0;
            if (!parse_innerStatementList(&__node_142))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_142;

            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_IF)
          {
            if (yytoken != Token_IF)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_IF, "if");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_143 = 0;
            if (!parse_expr(&__node_143))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->ifExpr = __node_143;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_COLON)
              {
                if (yytoken != Token_COLON)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COLON, ":");
                      }
                    return false;
                  }
                yylex();

                innerStatementList_ast *__node_144 = 0;
                if (!parse_innerStatementList(&__node_144))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                      }
                    return false;
                  }
                (*yynode)->statements = __node_144;

                newElseifList_ast *__node_145 = 0;
                if (!parse_newElseifList(&__node_145))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_newElseifList, "newElseifList");
                      }
                    return false;
                  }
                newElseSingle_ast *__node_146 = 0;
                if (!parse_newElseSingle(&__node_146))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_newElseSingle, "newElseSingle");
                      }
                    return false;
                  }
                if (yytoken != Token_ENDIF)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_ENDIF, "endif");
                      }
                    return false;
                  }
                yylex();

                semicolonOrCloseTag_ast *__node_147 = 0;
                if (!parse_semicolonOrCloseTag(&__node_147))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                      }
                    return false;
                  }
              }
            else if (yytoken == Token_BREAK
                     || yytoken == Token_CONTINUE
                     || yytoken == Token_DO
                     || yytoken == Token_FOR
                     || yytoken == Token_IF
                     || yytoken == Token_NEW
                     || yytoken == Token_RETURN
                     || yytoken == Token_STATIC
                     || yytoken == Token_SWITCH
                     || yytoken == Token_THROW
                     || yytoken == Token_TRY
                     || yytoken == Token_WHILE
                     || yytoken == Token_ECHO
                     || yytoken == Token_PRINT
                     || yytoken == Token_CLONE
                     || yytoken == Token_EXIT
                     || yytoken == Token_FOREACH
                     || yytoken == Token_DECLARE
                     || yytoken == Token_GLOBAL
                     || yytoken == Token_UNSET
                     || yytoken == Token_ISSET
                     || yytoken == Token_EMPTY
                     || yytoken == Token_LIST
                     || yytoken == Token_ARRAY
                     || yytoken == Token_CLASS_C
                     || yytoken == Token_METHOD_C
                     || yytoken == Token_FUNC_C
                     || yytoken == Token_LINE
                     || yytoken == Token_FILE
                     || yytoken == Token_INCLUDE
                     || yytoken == Token_INCLUDE_ONCE
                     || yytoken == Token_EVAL
                     || yytoken == Token_REQUIRE
                     || yytoken == Token_REQUIRE_ONCE
                     || yytoken == Token_INT_CAST
                     || yytoken == Token_DOUBLE_CAST
                     || yytoken == Token_STRING_CAST
                     || yytoken == Token_ARRAY_CAST
                     || yytoken == Token_OBJECT_CAST
                     || yytoken == Token_BOOL_CAST
                     || yytoken == Token_UNSET_CAST
                     || yytoken == Token_SEMICOLON
                     || yytoken == Token_DOUBLE_QUOTE
                     || yytoken == Token_LPAREN
                     || yytoken == Token_LBRACE
                     || yytoken == Token_AT
                     || yytoken == Token_START_HEREDOC
                     || yytoken == Token_BACKTICK
                     || yytoken == Token_PLUS
                     || yytoken == Token_MINUS
                     || yytoken == Token_INC
                     || yytoken == Token_DEC
                     || yytoken == Token_BANG
                     || yytoken == Token_TILDE
                     || yytoken == Token_DOLLAR
                     || yytoken == Token_INLINE_HTML
                     || yytoken == Token_CONSTANT_ENCAPSED_STRING
                     || yytoken == Token_VARIABLE
                     || yytoken == Token_DNUMBER
                     || yytoken == Token_LNUMBER
                     || yytoken == Token_STRING
                     || yytoken == Token_STRING_VARNAME
                     || yytoken == Token_OPEN_TAG
                     || yytoken == Token_CLOSE_TAG
                     || yytoken == Token_OPEN_TAG_WITH_ECHO)
              {
                statement_ast *__node_148 = 0;
                if (!parse_statement(&__node_148))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_statement, "statement");
                      }
                    return false;
                  }
                (*yynode)->ifStatement = __node_148;

                elseifList_ast *__node_149 = 0;
                if (!parse_elseifList(&__node_149))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_elseifList, "elseifList");
                      }
                    return false;
                  }
                (*yynode)->elseifList = __node_149;

                elseSingle_ast *__node_150 = 0;
                if (!parse_elseSingle(&__node_150))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_elseSingle, "elseSingle");
                      }
                    return false;
                  }
                (*yynode)->elseSingle = __node_150;

              }
            else
              {
                return false;
              }
          }
        else if (yytoken == Token_WHILE)
          {
            if (yytoken != Token_WHILE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_WHILE, "while");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_151 = 0;
            if (!parse_expr(&__node_151))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->whileExpr = __node_151;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            whileStatement_ast *__node_152 = 0;
            if (!parse_whileStatement(&__node_152))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_whileStatement, "whileStatement");
                  }
                return false;
              }
            (*yynode)->whilteStatement = __node_152;

          }
        else if (yytoken == Token_FOR)
          {
            if (yytoken != Token_FOR)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_FOR, "for");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            forExpr_ast *__node_153 = 0;
            if (!parse_forExpr(&__node_153))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_forExpr, "forExpr");
                  }
                return false;
              }
            (*yynode)->forExpr1 = __node_153;

            if (yytoken != Token_SEMICOLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SEMICOLON, ";");
                  }
                return false;
              }
            yylex();

            forExpr_ast *__node_154 = 0;
            if (!parse_forExpr(&__node_154))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_forExpr, "forExpr");
                  }
                return false;
              }
            (*yynode)->forExpr2 = __node_154;

            if (yytoken != Token_SEMICOLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SEMICOLON, ";");
                  }
                return false;
              }
            yylex();

            forExpr_ast *__node_155 = 0;
            if (!parse_forExpr(&__node_155))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_forExpr, "forExpr");
                  }
                return false;
              }
            (*yynode)->forExpr3 = __node_155;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            forStatement_ast *__node_156 = 0;
            if (!parse_forStatement(&__node_156))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_forStatement, "forStatement");
                  }
                return false;
              }
            (*yynode)->forStatement = __node_156;

          }
        else if (yytoken == Token_SWITCH)
          {
            if (yytoken != Token_SWITCH)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SWITCH, "switch");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_157 = 0;
            if (!parse_expr(&__node_157))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->swtichExpr = __node_157;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            switchCaseList_ast *__node_158 = 0;
            if (!parse_switchCaseList(&__node_158))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_switchCaseList, "switchCaseList");
                  }
                return false;
              }
            (*yynode)->switchCaseList = __node_158;

          }
        else if (yytoken == Token_FOREACH)
          {
            if (yytoken != Token_FOREACH)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_FOREACH, "foreach");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            bool block_errors_2 = block_errors(true);
            std::size_t try_start_token_2 = token_stream->index() - 1;
            parser_state *try_start_state_2 = copy_current_state();
            {
              variable_ast *__node_159 = 0;
              if (!parse_variable(&__node_159))
                {
                  goto __catch_2;
                }
              (*yynode)->foreachVar = __node_159;

              if (yytoken != Token_AS)
                goto __catch_2;
              yylex();

              foreachVariable_ast *__node_160 = 0;
              if (!parse_foreachVariable(&__node_160))
                {
                  goto __catch_2;
                }
              (*yynode)->foreachVarAsVar = __node_160;

            }
            block_errors(block_errors_2);
            if (try_start_state_2)
              delete try_start_state_2;

            if (false) // the only way to enter here is using goto
              {
              __catch_2:
                if (try_start_state_2)
                  {
                    restore_state(try_start_state_2);
                    delete try_start_state_2;
                  }
                block_errors(block_errors_2);
                rewind(try_start_token_2);

                expr_ast *__node_161 = 0;
                if (!parse_expr(&__node_161))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->foreachExpr = __node_161;

                if (yytoken != Token_AS)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_AS, "as");
                      }
                    return false;
                  }
                yylex();

                variable_ast *__node_162 = 0;
                if (!parse_variable(&__node_162))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_variable, "variable");
                      }
                    return false;
                  }
                (*yynode)->foreachExprAsVar = __node_162;

              }

            foreachOptionalArg_ast *__node_163 = 0;
            if (!parse_foreachOptionalArg(&__node_163))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_foreachOptionalArg, "foreachOptionalArg");
                  }
                return false;
              }
            (*yynode)->foreachArg = __node_163;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            foreachStatement_ast *__node_164 = 0;
            if (!parse_foreachStatement(&__node_164))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_foreachStatement, "foreachStatement");
                  }
                return false;
              }
            (*yynode)->foreachStatement = __node_164;

          }
        else if (yytoken == Token_DECLARE)
          {
            if (yytoken != Token_DECLARE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DECLARE, "declare");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            declareItem_ast *__node_165 = 0;
            if (!parse_declareItem(&__node_165))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_declareItem, "declareItem");
                  }
                return false;
              }
            (*yynode)->declareItem = __node_165;

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                declareItem_ast *__node_166 = 0;
                if (!parse_declareItem(&__node_166))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_declareItem, "declareItem");
                      }
                    return false;
                  }
                (*yynode)->declareItem = __node_166;

              }
            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            declareStatement_ast *__node_167 = 0;
            if (!parse_declareStatement(&__node_167))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_declareStatement, "declareStatement");
                  }
                return false;
              }
          }
        else if (yytoken == Token_SEMICOLON)
          {
            if (yytoken != Token_SEMICOLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SEMICOLON, ";");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_TRY)
          {
            if (yytoken != Token_TRY)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_TRY, "try");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACE, "{");
                  }
                return false;
              }
            yylex();

            innerStatementList_ast *__node_168 = 0;
            if (!parse_innerStatementList(&__node_168))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_168;

            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

            while (yytoken == Token_CATCH)
              {
                catch_item_ast *__node_169 = 0;
                if (!parse_catch_item(&__node_169))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_catch_item, "catch_item");
                      }
                    return false;
                  }
                (*yynode)->catches_sequence = snoc((*yynode)->catches_sequence, __node_169, memory_pool);

              }
          }
        else if (yytoken == Token_UNSET)
          {
            if (yytoken != Token_UNSET)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_UNSET, "unset");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            variable_ast *__node_170 = 0;
            if (!parse_variable(&__node_170))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variable, "variable");
                  }
                return false;
              }
            (*yynode)->unsetVariables_sequence = snoc((*yynode)->unsetVariables_sequence, __node_170, memory_pool);

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                variable_ast *__node_171 = 0;
                if (!parse_variable(&__node_171))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_variable, "variable");
                      }
                    return false;
                  }
                (*yynode)->unsetVariables_sequence = snoc((*yynode)->unsetVariables_sequence, __node_171, memory_pool);

              }
            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            semicolonOrCloseTag_ast *__node_172 = 0;
            if (!parse_semicolonOrCloseTag(&__node_172))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_NEW
                 || yytoken == Token_PRINT
                 || yytoken == Token_CLONE
                 || yytoken == Token_EXIT
                 || yytoken == Token_ISSET
                 || yytoken == Token_EMPTY
                 || yytoken == Token_LIST
                 || yytoken == Token_ARRAY
                 || yytoken == Token_CLASS_C
                 || yytoken == Token_METHOD_C
                 || yytoken == Token_FUNC_C
                 || yytoken == Token_LINE
                 || yytoken == Token_FILE
                 || yytoken == Token_INCLUDE
                 || yytoken == Token_INCLUDE_ONCE
                 || yytoken == Token_EVAL
                 || yytoken == Token_REQUIRE
                 || yytoken == Token_REQUIRE_ONCE
                 || yytoken == Token_INT_CAST
                 || yytoken == Token_DOUBLE_CAST
                 || yytoken == Token_STRING_CAST
                 || yytoken == Token_ARRAY_CAST
                 || yytoken == Token_OBJECT_CAST
                 || yytoken == Token_BOOL_CAST
                 || yytoken == Token_UNSET_CAST
                 || yytoken == Token_DOUBLE_QUOTE
                 || yytoken == Token_LPAREN
                 || yytoken == Token_AT
                 || yytoken == Token_START_HEREDOC
                 || yytoken == Token_BACKTICK
                 || yytoken == Token_PLUS
                 || yytoken == Token_MINUS
                 || yytoken == Token_INC
                 || yytoken == Token_DEC
                 || yytoken == Token_BANG
                 || yytoken == Token_TILDE
                 || yytoken == Token_DOLLAR
                 || yytoken == Token_CONSTANT_ENCAPSED_STRING
                 || yytoken == Token_VARIABLE
                 || yytoken == Token_DNUMBER
                 || yytoken == Token_LNUMBER
                 || yytoken == Token_STRING
                 || yytoken == Token_STRING_VARNAME)
          {
            expr_ast *__node_173 = 0;
            if (!parse_expr(&__node_173))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_173;

            semicolonOrCloseTag_ast *__node_174 = 0;
            if (!parse_semicolonOrCloseTag(&__node_174))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_DO)
          {
            if (yytoken != Token_DO)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DO, "do");
                  }
                return false;
              }
            yylex();

            statement_ast *__node_175 = 0;
            if (!parse_statement(&__node_175))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_statement, "statement");
                  }
                return false;
              }
            (*yynode)->doStatement = __node_175;

            if (yytoken != Token_WHILE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_WHILE, "while");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_176 = 0;
            if (!parse_expr(&__node_176))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->whilteExpr = __node_176;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            semicolonOrCloseTag_ast *__node_177 = 0;
            if (!parse_semicolonOrCloseTag(&__node_177))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_BREAK)
          {
            if (yytoken != Token_BREAK)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BREAK, "break");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_NEW
                || yytoken == Token_PRINT
                || yytoken == Token_CLONE
                || yytoken == Token_EXIT
                || yytoken == Token_ISSET
                || yytoken == Token_EMPTY
                || yytoken == Token_LIST
                || yytoken == Token_ARRAY
                || yytoken == Token_CLASS_C
                || yytoken == Token_METHOD_C
                || yytoken == Token_FUNC_C
                || yytoken == Token_LINE
                || yytoken == Token_FILE
                || yytoken == Token_INCLUDE
                || yytoken == Token_INCLUDE_ONCE
                || yytoken == Token_EVAL
                || yytoken == Token_REQUIRE
                || yytoken == Token_REQUIRE_ONCE
                || yytoken == Token_INT_CAST
                || yytoken == Token_DOUBLE_CAST
                || yytoken == Token_STRING_CAST
                || yytoken == Token_ARRAY_CAST
                || yytoken == Token_OBJECT_CAST
                || yytoken == Token_BOOL_CAST
                || yytoken == Token_UNSET_CAST
                || yytoken == Token_DOUBLE_QUOTE
                || yytoken == Token_LPAREN
                || yytoken == Token_AT
                || yytoken == Token_START_HEREDOC
                || yytoken == Token_BACKTICK
                || yytoken == Token_PLUS
                || yytoken == Token_MINUS
                || yytoken == Token_INC
                || yytoken == Token_DEC
                || yytoken == Token_BANG
                || yytoken == Token_TILDE
                || yytoken == Token_DOLLAR
                || yytoken == Token_CONSTANT_ENCAPSED_STRING
                || yytoken == Token_VARIABLE
                || yytoken == Token_DNUMBER
                || yytoken == Token_LNUMBER
                || yytoken == Token_STRING
                || yytoken == Token_STRING_VARNAME)
              {
                expr_ast *__node_178 = 0;
                if (!parse_expr(&__node_178))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->breakExpr = __node_178;

              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
            semicolonOrCloseTag_ast *__node_179 = 0;
            if (!parse_semicolonOrCloseTag(&__node_179))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_CONTINUE)
          {
            if (yytoken != Token_CONTINUE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CONTINUE, "continue");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_NEW
                || yytoken == Token_PRINT
                || yytoken == Token_CLONE
                || yytoken == Token_EXIT
                || yytoken == Token_ISSET
                || yytoken == Token_EMPTY
                || yytoken == Token_LIST
                || yytoken == Token_ARRAY
                || yytoken == Token_CLASS_C
                || yytoken == Token_METHOD_C
                || yytoken == Token_FUNC_C
                || yytoken == Token_LINE
                || yytoken == Token_FILE
                || yytoken == Token_INCLUDE
                || yytoken == Token_INCLUDE_ONCE
                || yytoken == Token_EVAL
                || yytoken == Token_REQUIRE
                || yytoken == Token_REQUIRE_ONCE
                || yytoken == Token_INT_CAST
                || yytoken == Token_DOUBLE_CAST
                || yytoken == Token_STRING_CAST
                || yytoken == Token_ARRAY_CAST
                || yytoken == Token_OBJECT_CAST
                || yytoken == Token_BOOL_CAST
                || yytoken == Token_UNSET_CAST
                || yytoken == Token_DOUBLE_QUOTE
                || yytoken == Token_LPAREN
                || yytoken == Token_AT
                || yytoken == Token_START_HEREDOC
                || yytoken == Token_BACKTICK
                || yytoken == Token_PLUS
                || yytoken == Token_MINUS
                || yytoken == Token_INC
                || yytoken == Token_DEC
                || yytoken == Token_BANG
                || yytoken == Token_TILDE
                || yytoken == Token_DOLLAR
                || yytoken == Token_CONSTANT_ENCAPSED_STRING
                || yytoken == Token_VARIABLE
                || yytoken == Token_DNUMBER
                || yytoken == Token_LNUMBER
                || yytoken == Token_STRING
                || yytoken == Token_STRING_VARNAME)
              {
                expr_ast *__node_180 = 0;
                if (!parse_expr(&__node_180))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->continueExpr = __node_180;

              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
            semicolonOrCloseTag_ast *__node_181 = 0;
            if (!parse_semicolonOrCloseTag(&__node_181))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_RETURN)
          {
            if (yytoken != Token_RETURN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RETURN, "return");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_NEW
                || yytoken == Token_PRINT
                || yytoken == Token_CLONE
                || yytoken == Token_EXIT
                || yytoken == Token_ISSET
                || yytoken == Token_EMPTY
                || yytoken == Token_LIST
                || yytoken == Token_ARRAY
                || yytoken == Token_CLASS_C
                || yytoken == Token_METHOD_C
                || yytoken == Token_FUNC_C
                || yytoken == Token_LINE
                || yytoken == Token_FILE
                || yytoken == Token_INCLUDE
                || yytoken == Token_INCLUDE_ONCE
                || yytoken == Token_EVAL
                || yytoken == Token_REQUIRE
                || yytoken == Token_REQUIRE_ONCE
                || yytoken == Token_INT_CAST
                || yytoken == Token_DOUBLE_CAST
                || yytoken == Token_STRING_CAST
                || yytoken == Token_ARRAY_CAST
                || yytoken == Token_OBJECT_CAST
                || yytoken == Token_BOOL_CAST
                || yytoken == Token_UNSET_CAST
                || yytoken == Token_DOUBLE_QUOTE
                || yytoken == Token_LPAREN
                || yytoken == Token_AT
                || yytoken == Token_START_HEREDOC
                || yytoken == Token_BACKTICK
                || yytoken == Token_PLUS
                || yytoken == Token_MINUS
                || yytoken == Token_INC
                || yytoken == Token_DEC
                || yytoken == Token_BANG
                || yytoken == Token_TILDE
                || yytoken == Token_DOLLAR
                || yytoken == Token_CONSTANT_ENCAPSED_STRING
                || yytoken == Token_VARIABLE
                || yytoken == Token_DNUMBER
                || yytoken == Token_LNUMBER
                || yytoken == Token_STRING
                || yytoken == Token_STRING_VARNAME)
              {
                expr_ast *__node_182 = 0;
                if (!parse_expr(&__node_182))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->returnExpr = __node_182;

              }
            else if (true /*epsilon*/)
            {}
            else
              {
                return false;
              }
            semicolonOrCloseTag_ast *__node_183 = 0;
            if (!parse_semicolonOrCloseTag(&__node_183))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_GLOBAL)
          {
            if (yytoken != Token_GLOBAL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_GLOBAL, "global");
                  }
                return false;
              }
            yylex();

            globalVar_ast *__node_184 = 0;
            if (!parse_globalVar(&__node_184))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_globalVar, "globalVar");
                  }
                return false;
              }
            (*yynode)->globalVars_sequence = snoc((*yynode)->globalVars_sequence, __node_184, memory_pool);

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                globalVar_ast *__node_185 = 0;
                if (!parse_globalVar(&__node_185))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_globalVar, "globalVar");
                      }
                    return false;
                  }
                (*yynode)->globalVars_sequence = snoc((*yynode)->globalVars_sequence, __node_185, memory_pool);

              }
            semicolonOrCloseTag_ast *__node_186 = 0;
            if (!parse_semicolonOrCloseTag(&__node_186))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_STATIC)
          {
            if (yytoken != Token_STATIC)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STATIC, "static");
                  }
                return false;
              }
            yylex();

            staticVar_ast *__node_187 = 0;
            if (!parse_staticVar(&__node_187))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_staticVar, "staticVar");
                  }
                return false;
              }
            (*yynode)->staticVars_sequence = snoc((*yynode)->staticVars_sequence, __node_187, memory_pool);

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                staticVar_ast *__node_188 = 0;
                if (!parse_staticVar(&__node_188))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_staticVar, "staticVar");
                      }
                    return false;
                  }
                (*yynode)->staticVars_sequence = snoc((*yynode)->staticVars_sequence, __node_188, memory_pool);

              }
            semicolonOrCloseTag_ast *__node_189 = 0;
            if (!parse_semicolonOrCloseTag(&__node_189))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_ECHO)
          {
            if (yytoken != Token_ECHO)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ECHO, "echo");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_190 = 0;
            if (!parse_expr(&__node_190))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->echoExprs_sequence = snoc((*yynode)->echoExprs_sequence, __node_190, memory_pool);

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                expr_ast *__node_191 = 0;
                if (!parse_expr(&__node_191))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_expr, "expr");
                      }
                    return false;
                  }
                (*yynode)->echoExprs_sequence = snoc((*yynode)->echoExprs_sequence, __node_191, memory_pool);

              }
            semicolonOrCloseTag_ast *__node_192 = 0;
            if (!parse_semicolonOrCloseTag(&__node_192))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_THROW)
          {
            if (yytoken != Token_THROW)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_THROW, "throw");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_193 = 0;
            if (!parse_expr(&__node_193))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->throwExpr = __node_193;

            semicolonOrCloseTag_ast *__node_194 = 0;
            if (!parse_semicolonOrCloseTag(&__node_194))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_CLOSE_TAG)
          {
            if (yytoken != Token_CLOSE_TAG)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CLOSE_TAG, "?>");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_OPEN_TAG)
          {
            if (yytoken != Token_OPEN_TAG)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_OPEN_TAG, "<?");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_OPEN_TAG_WITH_ECHO)
          {
            if (yytoken != Token_OPEN_TAG_WITH_ECHO)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_OPEN_TAG_WITH_ECHO, "<?=");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_195 = 0;
            if (!parse_expr(&__node_195))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_195;

            semicolonOrCloseTag_ast *__node_196 = 0;
            if (!parse_semicolonOrCloseTag(&__node_196))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
                return false;
              }
          }
        else if (yytoken == Token_INLINE_HTML)
          {
            if (yytoken != Token_INLINE_HTML)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_INLINE_HTML, "inline html");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_staticArrayPairValue(staticArrayPairValue_ast **yynode)
  {
    *yynode = create<staticArrayPairValue_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING)
      {
        staticScalar_ast *__node_197 = 0;
        if (!parse_staticScalar(&__node_197))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_staticScalar, "staticScalar");
              }
            return false;
          }
        (*yynode)->val1_sequence = snoc((*yynode)->val1_sequence, __node_197, memory_pool);

        if (yytoken == Token_DOUBLE_ARROW)
          {
            if (yytoken != Token_DOUBLE_ARROW)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DOUBLE_ARROW, "=>");
                  }
                return false;
              }
            yylex();

            staticScalar_ast *__node_198 = 0;
            if (!parse_staticScalar(&__node_198))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_staticScalar, "staticScalar");
                  }
                return false;
              }
            (*yynode)->val2_sequence = snoc((*yynode)->val2_sequence, __node_198, memory_pool);

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

  bool parser::parse_staticMember(staticMember_ast **yynode)
  {
    *yynode = create<staticMember_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_STRING)
      {
        if (yytoken != Token_STRING)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_STRING, "string");
              }
            return false;
          }
        (*yynode)->className = token_stream->index() - 1;
        yylex();

        if (yytoken != Token_PAAMAYIM_NEKUDOTAYIM)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_PAAMAYIM_NEKUDOTAYIM, "::");
              }
            return false;
          }
        yylex();

        variableWithoutObjects_ast *__node_199 = 0;
        if (!parse_variableWithoutObjects(&__node_199))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_variableWithoutObjects, "variableWithoutObjects");
              }
            return false;
          }
        (*yynode)->variable = __node_199;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_staticScalar(staticScalar_ast **yynode)
  {
    *yynode = create<staticScalar_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING)
      {
        if (yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER)
          {
            commonScalar_ast *__node_200 = 0;
            if (!parse_commonScalar(&__node_200))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_commonScalar, "commonScalar");
                  }
                return false;
              }
            (*yynode)->value = __node_200;

          }
        else if (yytoken == Token_STRING)
          {
            if (yytoken != Token_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING, "string");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_PAAMAYIM_NEKUDOTAYIM)
              {
                if (yytoken != Token_PAAMAYIM_NEKUDOTAYIM)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_PAAMAYIM_NEKUDOTAYIM, "::");
                      }
                    return false;
                  }
                yylex();

                if (yytoken != Token_STRING)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_STRING, "string");
                      }
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
        else if (yytoken == Token_PLUS)
          {
            if (yytoken != Token_PLUS)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_PLUS, "+");
                  }
                return false;
              }
            yylex();

            staticScalar_ast *__node_201 = 0;
            if (!parse_staticScalar(&__node_201))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_staticScalar, "staticScalar");
                  }
                return false;
              }
            (*yynode)->plusValue = __node_201;

          }
        else if (yytoken == Token_MINUS)
          {
            if (yytoken != Token_MINUS)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_MINUS, "-");
                  }
                return false;
              }
            yylex();

            staticScalar_ast *__node_202 = 0;
            if (!parse_staticScalar(&__node_202))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_staticScalar, "staticScalar");
                  }
                return false;
              }
            (*yynode)->minusValue = __node_202;

          }
        else if (yytoken == Token_ARRAY)
          {
            if (yytoken != Token_ARRAY)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ARRAY, "array");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_ARRAY
                || yytoken == Token_CLASS_C
                || yytoken == Token_METHOD_C
                || yytoken == Token_FUNC_C
                || yytoken == Token_LINE
                || yytoken == Token_FILE
                || yytoken == Token_PLUS
                || yytoken == Token_MINUS
                || yytoken == Token_CONSTANT_ENCAPSED_STRING
                || yytoken == Token_DNUMBER
                || yytoken == Token_LNUMBER
                || yytoken == Token_STRING)
              {
                staticArrayPairValue_ast *__node_203 = 0;
                if (!parse_staticArrayPairValue(&__node_203))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_staticArrayPairValue, "staticArrayPairValue");
                      }
                    return false;
                  }
                (*yynode)->arrayValues_sequence = snoc((*yynode)->arrayValues_sequence, __node_203, memory_pool);

                while (yytoken == Token_COMMA)
                  {
                    if (yytoken != Token_COMMA)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_COMMA, ",");
                          }
                        return false;
                      }
                    yylex();

                    if (yytoken == Token_RPAREN)
                      {
                        break;
                      }
                    staticArrayPairValue_ast *__node_204 = 0;
                    if (!parse_staticArrayPairValue(&__node_204))
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_symbol(ast_node::Kind_staticArrayPairValue, "staticArrayPairValue");
                          }
                        return false;
                      }
                    (*yynode)->arrayValues_sequence = snoc((*yynode)->arrayValues_sequence, __node_204, memory_pool);

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
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_staticVar(staticVar_ast **yynode)
  {
    *yynode = create<staticVar_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_VARIABLE)
      {
        if (yytoken != Token_VARIABLE)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_VARIABLE, "variable");
              }
            return false;
          }
        yylex();

        if (yytoken == Token_ASSIGN)
          {
            if (yytoken != Token_ASSIGN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ASSIGN, "=");
                  }
                return false;
              }
            yylex();

            staticScalar_ast *__node_205 = 0;
            if (!parse_staticScalar(&__node_205))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_staticScalar, "staticScalar");
                  }
                return false;
              }
            (*yynode)->staticScalar = __node_205;

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

  bool parser::parse_switchCaseList(switchCaseList_ast **yynode)
  {
    *yynode = create<switchCaseList_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LBRACE
        || yytoken == Token_COLON)
      {
        if (yytoken == Token_LBRACE)
          {
            if (yytoken != Token_LBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACE, "{");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_SEMICOLON)
              {
                if (yytoken != Token_SEMICOLON)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_SEMICOLON, ";");
                      }
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
            caseList_ast *__node_206 = 0;
            if (!parse_caseList(&__node_206))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_caseList, "caseList");
                  }
                return false;
              }
            (*yynode)->caseList = __node_206;

            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_COLON)
          {
            if (yytoken != Token_COLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_COLON, ":");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_SEMICOLON)
              {
                if (yytoken != Token_SEMICOLON)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_SEMICOLON, ";");
                      }
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
            caseList_ast *__node_207 = 0;
            if (!parse_caseList(&__node_207))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_caseList, "caseList");
                  }
                return false;
              }
            (*yynode)->caseList = __node_207;

            if (yytoken != Token_ENDSWITCH)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ENDSWITCH, "endswitch");
                  }
                return false;
              }
            yylex();

            semicolonOrCloseTag_ast *__node_208 = 0;
            if (!parse_semicolonOrCloseTag(&__node_208))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
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

  bool parser::parse_topStatement(topStatement_ast **yynode)
  {
    *yynode = create<topStatement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_ABSTRACT
        || yytoken == Token_BREAK
        || yytoken == Token_CLASS
        || yytoken == Token_CONTINUE
        || yytoken == Token_DO
        || yytoken == Token_FINAL
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_INTERFACE
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_FOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_FUNCTION
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_HALT_COMPILER
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO)
      {
        if (yytoken == Token_BREAK
            || yytoken == Token_CONTINUE
            || yytoken == Token_DO
            || yytoken == Token_FOR
            || yytoken == Token_IF
            || yytoken == Token_NEW
            || yytoken == Token_RETURN
            || yytoken == Token_STATIC
            || yytoken == Token_SWITCH
            || yytoken == Token_THROW
            || yytoken == Token_TRY
            || yytoken == Token_WHILE
            || yytoken == Token_ECHO
            || yytoken == Token_PRINT
            || yytoken == Token_CLONE
            || yytoken == Token_EXIT
            || yytoken == Token_FOREACH
            || yytoken == Token_DECLARE
            || yytoken == Token_GLOBAL
            || yytoken == Token_UNSET
            || yytoken == Token_ISSET
            || yytoken == Token_EMPTY
            || yytoken == Token_LIST
            || yytoken == Token_ARRAY
            || yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_INCLUDE
            || yytoken == Token_INCLUDE_ONCE
            || yytoken == Token_EVAL
            || yytoken == Token_REQUIRE
            || yytoken == Token_REQUIRE_ONCE
            || yytoken == Token_INT_CAST
            || yytoken == Token_DOUBLE_CAST
            || yytoken == Token_STRING_CAST
            || yytoken == Token_ARRAY_CAST
            || yytoken == Token_OBJECT_CAST
            || yytoken == Token_BOOL_CAST
            || yytoken == Token_UNSET_CAST
            || yytoken == Token_SEMICOLON
            || yytoken == Token_DOUBLE_QUOTE
            || yytoken == Token_LPAREN
            || yytoken == Token_LBRACE
            || yytoken == Token_AT
            || yytoken == Token_START_HEREDOC
            || yytoken == Token_BACKTICK
            || yytoken == Token_PLUS
            || yytoken == Token_MINUS
            || yytoken == Token_INC
            || yytoken == Token_DEC
            || yytoken == Token_BANG
            || yytoken == Token_TILDE
            || yytoken == Token_DOLLAR
            || yytoken == Token_INLINE_HTML
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_VARIABLE
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER
            || yytoken == Token_STRING
            || yytoken == Token_STRING_VARNAME
            || yytoken == Token_OPEN_TAG
            || yytoken == Token_CLOSE_TAG
            || yytoken == Token_OPEN_TAG_WITH_ECHO)
          {
            statement_ast *__node_209 = 0;
            if (!parse_statement(&__node_209))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_statement, "statement");
                  }
                return false;
              }
            (*yynode)->statement = __node_209;

          }
        else if (yytoken == Token_FUNCTION)
          {
            functionDeclarationStatement_ast *__node_210 = 0;
            if (!parse_functionDeclarationStatement(&__node_210))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_functionDeclarationStatement, "functionDeclarationStatement");
                  }
                return false;
              }
            (*yynode)->functionDeclaration = __node_210;

          }
        else if (yytoken == Token_ABSTRACT
                 || yytoken == Token_CLASS
                 || yytoken == Token_FINAL
                 || yytoken == Token_INTERFACE)
          {
            classDeclarationStatement_ast *__node_211 = 0;
            if (!parse_classDeclarationStatement(&__node_211))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_classDeclarationStatement, "classDeclarationStatement");
                  }
                return false;
              }
            (*yynode)->classDeclaration = __node_211;

          }
        else if (yytoken == Token_HALT_COMPILER)
          {
            if (yytoken != Token_HALT_COMPILER)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_HALT_COMPILER, "halt compiler");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_SEMICOLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_SEMICOLON, ";");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_unaryExpression(unaryExpression_ast **yynode)
  {
    *yynode = create<unaryExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        if (yytoken == Token_MINUS)
          {
            if (yytoken != Token_MINUS)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_MINUS, "-");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_212 = 0;
            if (!parse_unaryExpression(&__node_212))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_212;

          }
        else if (yytoken == Token_PLUS)
          {
            if (yytoken != Token_PLUS)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_PLUS, "+");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_213 = 0;
            if (!parse_unaryExpression(&__node_213))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_213;

          }
        else if (yytoken == Token_BANG)
          {
            if (yytoken != Token_BANG)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BANG, "!");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_214 = 0;
            if (!parse_unaryExpression(&__node_214))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_214;

          }
        else if (yytoken == Token_TILDE)
          {
            if (yytoken != Token_TILDE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_TILDE, "~");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_215 = 0;
            if (!parse_unaryExpression(&__node_215))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_215;

          }
        else if (yytoken == Token_INT_CAST)
          {
            if (yytoken != Token_INT_CAST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_INT_CAST, "int cast");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_216 = 0;
            if (!parse_unaryExpression(&__node_216))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_216;

          }
        else if (yytoken == Token_DOUBLE_CAST)
          {
            if (yytoken != Token_DOUBLE_CAST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_DOUBLE_CAST, "double cast");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_217 = 0;
            if (!parse_unaryExpression(&__node_217))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_217;

          }
        else if (yytoken == Token_STRING_CAST)
          {
            if (yytoken != Token_STRING_CAST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING_CAST, "string cast");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_218 = 0;
            if (!parse_unaryExpression(&__node_218))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_218;

          }
        else if (yytoken == Token_ARRAY_CAST)
          {
            if (yytoken != Token_ARRAY_CAST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ARRAY_CAST, "array cast");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_219 = 0;
            if (!parse_unaryExpression(&__node_219))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_219;

          }
        else if (yytoken == Token_OBJECT_CAST)
          {
            if (yytoken != Token_OBJECT_CAST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_OBJECT_CAST, "object cast");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_220 = 0;
            if (!parse_unaryExpression(&__node_220))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_220;

          }
        else if (yytoken == Token_BOOL_CAST)
          {
            if (yytoken != Token_BOOL_CAST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BOOL_CAST, "bool cast");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_221 = 0;
            if (!parse_unaryExpression(&__node_221))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_221;

          }
        else if (yytoken == Token_UNSET_CAST)
          {
            if (yytoken != Token_UNSET_CAST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_UNSET_CAST, "unset cast");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_222 = 0;
            if (!parse_unaryExpression(&__node_222))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_222;

          }
        else if (yytoken == Token_AT)
          {
            if (yytoken != Token_AT)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_AT, "@");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_223 = 0;
            if (!parse_unaryExpression(&__node_223))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_223;

          }
        else if (yytoken == Token_LIST)
          {
            if (yytoken != Token_LIST)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LIST, "list");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            assignmentList_ast *__node_224 = 0;
            if (!parse_assignmentList(&__node_224))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_assignmentList, "assignmentList");
                  }
                return false;
              }
            (*yynode)->assignmentList = __node_224;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_ASSIGN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ASSIGN, "=");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_225 = 0;
            if (!parse_unaryExpression(&__node_225))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_225;

          }
        else if (yytoken == Token_EXIT)
          {
            if (yytoken != Token_EXIT)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_EXIT, "exit");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_LPAREN)
              {
                if (yytoken != Token_LPAREN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_LPAREN, "(");
                      }
                    return false;
                  }
                yylex();

                if (yytoken == Token_NEW
                    || yytoken == Token_PRINT
                    || yytoken == Token_CLONE
                    || yytoken == Token_EXIT
                    || yytoken == Token_ISSET
                    || yytoken == Token_EMPTY
                    || yytoken == Token_LIST
                    || yytoken == Token_ARRAY
                    || yytoken == Token_CLASS_C
                    || yytoken == Token_METHOD_C
                    || yytoken == Token_FUNC_C
                    || yytoken == Token_LINE
                    || yytoken == Token_FILE
                    || yytoken == Token_INCLUDE
                    || yytoken == Token_INCLUDE_ONCE
                    || yytoken == Token_EVAL
                    || yytoken == Token_REQUIRE
                    || yytoken == Token_REQUIRE_ONCE
                    || yytoken == Token_INT_CAST
                    || yytoken == Token_DOUBLE_CAST
                    || yytoken == Token_STRING_CAST
                    || yytoken == Token_ARRAY_CAST
                    || yytoken == Token_OBJECT_CAST
                    || yytoken == Token_BOOL_CAST
                    || yytoken == Token_UNSET_CAST
                    || yytoken == Token_DOUBLE_QUOTE
                    || yytoken == Token_LPAREN
                    || yytoken == Token_AT
                    || yytoken == Token_START_HEREDOC
                    || yytoken == Token_BACKTICK
                    || yytoken == Token_PLUS
                    || yytoken == Token_MINUS
                    || yytoken == Token_INC
                    || yytoken == Token_DEC
                    || yytoken == Token_BANG
                    || yytoken == Token_TILDE
                    || yytoken == Token_DOLLAR
                    || yytoken == Token_CONSTANT_ENCAPSED_STRING
                    || yytoken == Token_VARIABLE
                    || yytoken == Token_DNUMBER
                    || yytoken == Token_LNUMBER
                    || yytoken == Token_STRING
                    || yytoken == Token_STRING_VARNAME)
                  {
                    expr_ast *__node_226 = 0;
                    if (!parse_expr(&__node_226))
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_symbol(ast_node::Kind_expr, "expr");
                          }
                        return false;
                      }
                    (*yynode)->expression = __node_226;

                  }
                else if (true /*epsilon*/)
                {}
                else
                  {
                    return false;
                  }
                if (yytoken != Token_RPAREN)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_RPAREN, ")");
                      }
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
        else if (yytoken == Token_EVAL)
          {
            if (yytoken != Token_EVAL)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_EVAL, "eval");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_227 = 0;
            if (!parse_expr(&__node_227))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expression = __node_227;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_INCLUDE)
          {
            if (yytoken != Token_INCLUDE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_INCLUDE, "include");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_228 = 0;
            if (!parse_unaryExpression(&__node_228))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_228;

          }
        else if (yytoken == Token_INCLUDE_ONCE)
          {
            if (yytoken != Token_INCLUDE_ONCE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_INCLUDE_ONCE, "include_once");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_229 = 0;
            if (!parse_unaryExpression(&__node_229))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_229;

          }
        else if (yytoken == Token_REQUIRE)
          {
            if (yytoken != Token_REQUIRE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_REQUIRE, "require");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_230 = 0;
            if (!parse_unaryExpression(&__node_230))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_230;

          }
        else if (yytoken == Token_REQUIRE_ONCE)
          {
            if (yytoken != Token_REQUIRE_ONCE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_REQUIRE_ONCE, "require_once");
                  }
                return false;
              }
            yylex();

            unaryExpression_ast *__node_231 = 0;
            if (!parse_unaryExpression(&__node_231))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression, "unaryExpression");
                  }
                return false;
              }
            (*yynode)->unaryExpression = __node_231;

          }
        else if (yytoken == Token_NEW
                 || yytoken == Token_CLONE
                 || yytoken == Token_ISSET
                 || yytoken == Token_EMPTY
                 || yytoken == Token_ARRAY
                 || yytoken == Token_CLASS_C
                 || yytoken == Token_METHOD_C
                 || yytoken == Token_FUNC_C
                 || yytoken == Token_LINE
                 || yytoken == Token_FILE
                 || yytoken == Token_DOUBLE_QUOTE
                 || yytoken == Token_LPAREN
                 || yytoken == Token_START_HEREDOC
                 || yytoken == Token_BACKTICK
                 || yytoken == Token_INC
                 || yytoken == Token_DEC
                 || yytoken == Token_DOLLAR
                 || yytoken == Token_CONSTANT_ENCAPSED_STRING
                 || yytoken == Token_VARIABLE
                 || yytoken == Token_DNUMBER
                 || yytoken == Token_LNUMBER
                 || yytoken == Token_STRING
                 || yytoken == Token_STRING_VARNAME)
          {
            unaryExpression_not_plusminus_ast *__node_232 = 0;
            if (!parse_unaryExpression_not_plusminus(&__node_232))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_unaryExpression_not_plusminus, "unaryExpression_not_plusminus");
                  }
                return false;
              }
            (*yynode)->unaryExpressionNotPlusminus = __node_232;

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

  bool parser::parse_unaryExpression_not_plusminus(unaryExpression_not_plusminus_ast **yynode)
  {
    *yynode = create<unaryExpression_not_plusminus_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        while (yytoken == Token_INC
               || yytoken == Token_DEC)
          {
            postprefixOperator_ast *__node_233 = 0;
            if (!parse_postprefixOperator(&__node_233))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_postprefixOperator, "postprefixOperator");
                  }
                return false;
              }
            (*yynode)->prefixOperator_sequence = snoc((*yynode)->prefixOperator_sequence, __node_233, memory_pool);

          }
        varExpression_ast *__node_234 = 0;
        if (!parse_varExpression(&__node_234))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_varExpression, "varExpression");
              }
            return false;
          }
        (*yynode)->varExpression = __node_234;

        while (yytoken == Token_INC
               || yytoken == Token_DEC)
          {
            postprefixOperator_ast *__node_235 = 0;
            if (!parse_postprefixOperator(&__node_235))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_postprefixOperator, "postprefixOperator");
                  }
                return false;
              }
            (*yynode)->postfixOperator_sequence = snoc((*yynode)->postfixOperator_sequence, __node_235, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_varExpression(varExpression_ast **yynode)
  {
    *yynode = create<varExpression_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        if ((yytoken == Token_DOLLAR
             || yytoken == Token_VARIABLE
             || yytoken == Token_STRING) && ( m_state.varExpressionState == OnlyVariable ))
          {
            m_state.varExpressionState = Normal;
            variable_ast *__node_236 = 0;
            if (!parse_variable(&__node_236))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variable, "variable");
                  }
                return false;
              }
            (*yynode)->variable = __node_236;

          }
        else if ((yytoken == Token_NEW) && ( m_state.varExpressionState == OnlyNewObject ))
          {
            m_state.varExpressionState = Normal;
            varExpressionNewObject_ast *__node_237 = 0;
            if (!parse_varExpressionNewObject(&__node_237))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_varExpressionNewObject, "varExpressionNewObject");
                  }
                return false;
              }
            (*yynode)->newObject = __node_237;

          }
        else if (yytoken == Token_NEW
                 || yytoken == Token_CLONE
                 || yytoken == Token_ISSET
                 || yytoken == Token_EMPTY
                 || yytoken == Token_ARRAY
                 || yytoken == Token_CLASS_C
                 || yytoken == Token_METHOD_C
                 || yytoken == Token_FUNC_C
                 || yytoken == Token_LINE
                 || yytoken == Token_FILE
                 || yytoken == Token_DOUBLE_QUOTE
                 || yytoken == Token_LPAREN
                 || yytoken == Token_START_HEREDOC
                 || yytoken == Token_BACKTICK
                 || yytoken == Token_DOLLAR
                 || yytoken == Token_CONSTANT_ENCAPSED_STRING
                 || yytoken == Token_VARIABLE
                 || yytoken == Token_DNUMBER
                 || yytoken == Token_LNUMBER
                 || yytoken == Token_STRING
                 || yytoken == Token_STRING_VARNAME)
          {
            varExpressionNormal_ast *__node_238 = 0;
            if (!parse_varExpressionNormal(&__node_238))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_varExpressionNormal, "varExpressionNormal");
                  }
                return false;
              }
            (*yynode)->varExpressionNormal = __node_238;

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

  bool parser::parse_varExpressionNewObject(varExpressionNewObject_ast **yynode)
  {
    *yynode = create<varExpressionNewObject_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW)
      {
        if (yytoken != Token_NEW)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_NEW, "new");
              }
            return false;
          }
        yylex();

        classNameReference_ast *__node_239 = 0;
        if (!parse_classNameReference(&__node_239))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_classNameReference, "classNameReference");
              }
            return false;
          }
        (*yynode)->className = __node_239;

        ctorArguments_ast *__node_240 = 0;
        if (!parse_ctorArguments(&__node_240))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_ctorArguments, "ctorArguments");
              }
            return false;
          }
        (*yynode)->ctor = __node_240;

      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_varExpressionNormal(varExpressionNormal_ast **yynode)
  {
    *yynode = create<varExpressionNormal_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_NEW
        || yytoken == Token_CLONE
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_DOLLAR
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME)
      {
        if (yytoken == Token_LPAREN)
          {
            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_241 = 0;
            if (!parse_expr(&__node_241))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expression = __node_241;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_BACKTICK)
          {
            if (yytoken != Token_BACKTICK)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BACKTICK, "`");
                  }
                return false;
              }
            yylex();

            encapsList_ast *__node_242 = 0;
            if (!parse_encapsList(&__node_242))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_encapsList, "encapsList");
                  }
                return false;
              }
            (*yynode)->encapsList = __node_242;

            if (yytoken != Token_BACKTICK)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_BACKTICK, "`");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_CLASS_C
                 || yytoken == Token_METHOD_C
                 || yytoken == Token_FUNC_C
                 || yytoken == Token_LINE
                 || yytoken == Token_FILE
                 || yytoken == Token_DOUBLE_QUOTE
                 || yytoken == Token_START_HEREDOC
                 || yytoken == Token_DOLLAR
                 || yytoken == Token_CONSTANT_ENCAPSED_STRING
                 || yytoken == Token_VARIABLE
                 || yytoken == Token_DNUMBER
                 || yytoken == Token_LNUMBER
                 || yytoken == Token_STRING
                 || yytoken == Token_STRING_VARNAME)
          {
            bool block_errors_3 = block_errors(true);
            std::size_t try_start_token_3 = token_stream->index() - 1;
            parser_state *try_start_state_3 = copy_current_state();
            {
              variable_ast *__node_243 = 0;
              if (!parse_variable(&__node_243))
                {
                  goto __catch_3;
                }
              (*yynode)->variable = __node_243;

              m_state.varExpressionIsVariable = true;
            }
            block_errors(block_errors_3);
            if (try_start_state_3)
              delete try_start_state_3;

            if (false) // the only way to enter here is using goto
              {
              __catch_3:
                if (try_start_state_3)
                  {
                    restore_state(try_start_state_3);
                    delete try_start_state_3;
                  }
                block_errors(block_errors_3);
                rewind(try_start_token_3);

                scalar_ast *__node_244 = 0;
                if (!parse_scalar(&__node_244))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_scalar, "scalar");
                      }
                    return false;
                  }
                (*yynode)->scalar = __node_244;

              }

          }
        else if (yytoken == Token_ARRAY)
          {
            if (yytoken != Token_ARRAY)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ARRAY, "array");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            if (yytoken == Token_NEW
                || yytoken == Token_PRINT
                || yytoken == Token_CLONE
                || yytoken == Token_EXIT
                || yytoken == Token_ISSET
                || yytoken == Token_EMPTY
                || yytoken == Token_LIST
                || yytoken == Token_ARRAY
                || yytoken == Token_CLASS_C
                || yytoken == Token_METHOD_C
                || yytoken == Token_FUNC_C
                || yytoken == Token_LINE
                || yytoken == Token_FILE
                || yytoken == Token_INCLUDE
                || yytoken == Token_INCLUDE_ONCE
                || yytoken == Token_EVAL
                || yytoken == Token_REQUIRE
                || yytoken == Token_REQUIRE_ONCE
                || yytoken == Token_INT_CAST
                || yytoken == Token_DOUBLE_CAST
                || yytoken == Token_STRING_CAST
                || yytoken == Token_ARRAY_CAST
                || yytoken == Token_OBJECT_CAST
                || yytoken == Token_BOOL_CAST
                || yytoken == Token_UNSET_CAST
                || yytoken == Token_DOUBLE_QUOTE
                || yytoken == Token_LPAREN
                || yytoken == Token_AT
                || yytoken == Token_START_HEREDOC
                || yytoken == Token_BACKTICK
                || yytoken == Token_PLUS
                || yytoken == Token_MINUS
                || yytoken == Token_INC
                || yytoken == Token_DEC
                || yytoken == Token_BANG
                || yytoken == Token_BIT_AND
                || yytoken == Token_TILDE
                || yytoken == Token_DOLLAR
                || yytoken == Token_CONSTANT_ENCAPSED_STRING
                || yytoken == Token_VARIABLE
                || yytoken == Token_DNUMBER
                || yytoken == Token_LNUMBER
                || yytoken == Token_STRING
                || yytoken == Token_STRING_VARNAME)
              {
                arrayPairValue_ast *__node_245 = 0;
                if (!parse_arrayPairValue(&__node_245))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_arrayPairValue, "arrayPairValue");
                      }
                    return false;
                  }
                (*yynode)->arrayValues_sequence = snoc((*yynode)->arrayValues_sequence, __node_245, memory_pool);

                while (yytoken == Token_COMMA)
                  {
                    if (yytoken != Token_COMMA)
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_token(yytoken, Token_COMMA, ",");
                          }
                        return false;
                      }
                    yylex();

                    if (yytoken == Token_RPAREN)
                      {
                        break;
                      }
                    arrayPairValue_ast *__node_246 = 0;
                    if (!parse_arrayPairValue(&__node_246))
                      {
                        if (!yy_block_errors)
                          {
                            yy_expected_symbol(ast_node::Kind_arrayPairValue, "arrayPairValue");
                          }
                        return false;
                      }
                    (*yynode)->arrayValues_sequence = snoc((*yynode)->arrayValues_sequence, __node_246, memory_pool);

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
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_ISSET)
          {
            if (yytoken != Token_ISSET)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ISSET, "isset");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            variable_ast *__node_247 = 0;
            if (!parse_variable(&__node_247))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variable, "variable");
                  }
                return false;
              }
            (*yynode)->issetVariable_sequence = snoc((*yynode)->issetVariable_sequence, __node_247, memory_pool);

            while (yytoken == Token_COMMA)
              {
                if (yytoken != Token_COMMA)
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_token(yytoken, Token_COMMA, ",");
                      }
                    return false;
                  }
                yylex();

                variable_ast *__node_248 = 0;
                if (!parse_variable(&__node_248))
                  {
                    if (!yy_block_errors)
                      {
                        yy_expected_symbol(ast_node::Kind_variable, "variable");
                      }
                    return false;
                  }
                (*yynode)->issetVariable_sequence = snoc((*yynode)->issetVariable_sequence, __node_248, memory_pool);

              }
            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_EMPTY)
          {
            if (yytoken != Token_EMPTY)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_EMPTY, "empty");
                  }
                return false;
              }
            yylex();

            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            variable_ast *__node_249 = 0;
            if (!parse_variable(&__node_249))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variable, "variable");
                  }
                return false;
              }
            (*yynode)->emptyVarialbe = __node_249;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
                return false;
              }
            yylex();

          }
        else if (yytoken == Token_NEW)
          {
            varExpressionNewObject_ast *__node_250 = 0;
            if (!parse_varExpressionNewObject(&__node_250))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_varExpressionNewObject, "varExpressionNewObject");
                  }
                return false;
              }
            (*yynode)->newObject = __node_250;

          }
        else if (yytoken == Token_CLONE)
          {
            if (yytoken != Token_CLONE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_CLONE, "clone");
                  }
                return false;
              }
            yylex();

            varExpressionNormal_ast *__node_251 = 0;
            if (!parse_varExpressionNormal(&__node_251))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_varExpressionNormal, "varExpressionNormal");
                  }
                return false;
              }
            (*yynode)->cloneCar = __node_251;

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

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE
        || yytoken == Token_STRING)
      {
        baseVariableWithFunctionCalls_ast *__node_252 = 0;
        if (!parse_baseVariableWithFunctionCalls(&__node_252))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_baseVariableWithFunctionCalls, "baseVariableWithFunctionCalls");
              }
            return false;
          }
        (*yynode)->var = __node_252;

        while (yytoken == Token_OBJECT_OPERATOR)
          {
            variableProperty_ast *__node_253 = 0;
            if (!parse_variableProperty(&__node_253))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_variableProperty, "variableProperty");
                  }
                return false;
              }
            (*yynode)->variableProperties_sequence = snoc((*yynode)->variableProperties_sequence, __node_253, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_variableName(variableName_ast **yynode)
  {
    *yynode = create<variableName_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_LBRACE
        || yytoken == Token_STRING)
      {
        if (yytoken == Token_STRING)
          {
            if (yytoken != Token_STRING)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_STRING, "string");
                  }
                return false;
              }
            (*yynode)->name = token_stream->index() - 1;
            yylex();

          }
        else if (yytoken == Token_LBRACE)
          {
            if (yytoken != Token_LBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LBRACE, "{");
                  }
                return false;
              }
            yylex();

            expr_ast *__node_254 = 0;
            if (!parse_expr(&__node_254))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_expr, "expr");
                  }
                return false;
              }
            (*yynode)->expr = __node_254;

            if (yytoken != Token_RBRACE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RBRACE, "}");
                  }
                return false;
              }
            yylex();

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

  bool parser::parse_variableProperty(variableProperty_ast **yynode)
  {
    *yynode = create<variableProperty_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_OBJECT_OPERATOR)
      {
        if (yytoken != Token_OBJECT_OPERATOR)
          {
            if (!yy_block_errors)
              {
                yy_expected_token(yytoken, Token_OBJECT_OPERATOR, "->");
              }
            return false;
          }
        yylex();

        objectProperty_ast *__node_255 = 0;
        if (!parse_objectProperty(&__node_255))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_objectProperty, "objectProperty");
              }
            return false;
          }
        (*yynode)->objectProperty = __node_255;

        if (yytoken == Token_LPAREN)
          {
            if (yytoken != Token_LPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_LPAREN, "(");
                  }
                return false;
              }
            yylex();

            functionCallParameterList_ast *__node_256 = 0;
            if (!parse_functionCallParameterList(&__node_256))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_functionCallParameterList, "functionCallParameterList");
                  }
                return false;
              }
            (*yynode)->parameterList = __node_256;

            if (yytoken != Token_RPAREN)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_RPAREN, ")");
                  }
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

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_variableWithoutObjects(variableWithoutObjects_ast **yynode)
  {
    *yynode = create<variableWithoutObjects_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_DOLLAR
        || yytoken == Token_VARIABLE)
      {
        compoundVariableWithSimpleIndirectReference_ast *__node_257 = 0;
        if (!parse_compoundVariableWithSimpleIndirectReference(&__node_257))
          {
            if (!yy_block_errors)
              {
                yy_expected_symbol(ast_node::Kind_compoundVariableWithSimpleIndirectReference, "compoundVariableWithSimpleIndirectReference");
              }
            return false;
          }
        (*yynode)->variable = __node_257;

        while (yytoken == Token_LBRACKET
               || yytoken == Token_LBRACE)
          {
            dimListItem_ast *__node_258 = 0;
            if (!parse_dimListItem(&__node_258))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_dimListItem, "dimListItem");
                  }
                return false;
              }
            (*yynode)->offsetItems_sequence = snoc((*yynode)->offsetItems_sequence, __node_258, memory_pool);

          }
      }
    else
      {
        return false;
      }

    (*yynode)->end_token = token_stream->index() - 1;

    return true;
  }

  bool parser::parse_whileStatement(whileStatement_ast **yynode)
  {
    *yynode = create<whileStatement_ast>();

    (*yynode)->start_token = token_stream->index() - 1;

    if (yytoken == Token_BREAK
        || yytoken == Token_CONTINUE
        || yytoken == Token_DO
        || yytoken == Token_FOR
        || yytoken == Token_IF
        || yytoken == Token_NEW
        || yytoken == Token_RETURN
        || yytoken == Token_STATIC
        || yytoken == Token_SWITCH
        || yytoken == Token_THROW
        || yytoken == Token_TRY
        || yytoken == Token_WHILE
        || yytoken == Token_ECHO
        || yytoken == Token_PRINT
        || yytoken == Token_CLONE
        || yytoken == Token_EXIT
        || yytoken == Token_FOREACH
        || yytoken == Token_DECLARE
        || yytoken == Token_GLOBAL
        || yytoken == Token_UNSET
        || yytoken == Token_ISSET
        || yytoken == Token_EMPTY
        || yytoken == Token_LIST
        || yytoken == Token_ARRAY
        || yytoken == Token_CLASS_C
        || yytoken == Token_METHOD_C
        || yytoken == Token_FUNC_C
        || yytoken == Token_LINE
        || yytoken == Token_FILE
        || yytoken == Token_INCLUDE
        || yytoken == Token_INCLUDE_ONCE
        || yytoken == Token_EVAL
        || yytoken == Token_REQUIRE
        || yytoken == Token_REQUIRE_ONCE
        || yytoken == Token_INT_CAST
        || yytoken == Token_DOUBLE_CAST
        || yytoken == Token_STRING_CAST
        || yytoken == Token_ARRAY_CAST
        || yytoken == Token_OBJECT_CAST
        || yytoken == Token_BOOL_CAST
        || yytoken == Token_UNSET_CAST
        || yytoken == Token_SEMICOLON
        || yytoken == Token_DOUBLE_QUOTE
        || yytoken == Token_LPAREN
        || yytoken == Token_LBRACE
        || yytoken == Token_AT
        || yytoken == Token_START_HEREDOC
        || yytoken == Token_BACKTICK
        || yytoken == Token_PLUS
        || yytoken == Token_MINUS
        || yytoken == Token_INC
        || yytoken == Token_DEC
        || yytoken == Token_BANG
        || yytoken == Token_COLON
        || yytoken == Token_TILDE
        || yytoken == Token_DOLLAR
        || yytoken == Token_INLINE_HTML
        || yytoken == Token_CONSTANT_ENCAPSED_STRING
        || yytoken == Token_VARIABLE
        || yytoken == Token_DNUMBER
        || yytoken == Token_LNUMBER
        || yytoken == Token_STRING
        || yytoken == Token_STRING_VARNAME
        || yytoken == Token_OPEN_TAG
        || yytoken == Token_CLOSE_TAG
        || yytoken == Token_OPEN_TAG_WITH_ECHO)
      {
        if (yytoken == Token_BREAK
            || yytoken == Token_CONTINUE
            || yytoken == Token_DO
            || yytoken == Token_FOR
            || yytoken == Token_IF
            || yytoken == Token_NEW
            || yytoken == Token_RETURN
            || yytoken == Token_STATIC
            || yytoken == Token_SWITCH
            || yytoken == Token_THROW
            || yytoken == Token_TRY
            || yytoken == Token_WHILE
            || yytoken == Token_ECHO
            || yytoken == Token_PRINT
            || yytoken == Token_CLONE
            || yytoken == Token_EXIT
            || yytoken == Token_FOREACH
            || yytoken == Token_DECLARE
            || yytoken == Token_GLOBAL
            || yytoken == Token_UNSET
            || yytoken == Token_ISSET
            || yytoken == Token_EMPTY
            || yytoken == Token_LIST
            || yytoken == Token_ARRAY
            || yytoken == Token_CLASS_C
            || yytoken == Token_METHOD_C
            || yytoken == Token_FUNC_C
            || yytoken == Token_LINE
            || yytoken == Token_FILE
            || yytoken == Token_INCLUDE
            || yytoken == Token_INCLUDE_ONCE
            || yytoken == Token_EVAL
            || yytoken == Token_REQUIRE
            || yytoken == Token_REQUIRE_ONCE
            || yytoken == Token_INT_CAST
            || yytoken == Token_DOUBLE_CAST
            || yytoken == Token_STRING_CAST
            || yytoken == Token_ARRAY_CAST
            || yytoken == Token_OBJECT_CAST
            || yytoken == Token_BOOL_CAST
            || yytoken == Token_UNSET_CAST
            || yytoken == Token_SEMICOLON
            || yytoken == Token_DOUBLE_QUOTE
            || yytoken == Token_LPAREN
            || yytoken == Token_LBRACE
            || yytoken == Token_AT
            || yytoken == Token_START_HEREDOC
            || yytoken == Token_BACKTICK
            || yytoken == Token_PLUS
            || yytoken == Token_MINUS
            || yytoken == Token_INC
            || yytoken == Token_DEC
            || yytoken == Token_BANG
            || yytoken == Token_TILDE
            || yytoken == Token_DOLLAR
            || yytoken == Token_INLINE_HTML
            || yytoken == Token_CONSTANT_ENCAPSED_STRING
            || yytoken == Token_VARIABLE
            || yytoken == Token_DNUMBER
            || yytoken == Token_LNUMBER
            || yytoken == Token_STRING
            || yytoken == Token_STRING_VARNAME
            || yytoken == Token_OPEN_TAG
            || yytoken == Token_CLOSE_TAG
            || yytoken == Token_OPEN_TAG_WITH_ECHO)
          {
            statement_ast *__node_259 = 0;
            if (!parse_statement(&__node_259))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_statement, "statement");
                  }
                return false;
              }
            (*yynode)->statement = __node_259;

          }
        else if (yytoken == Token_COLON)
          {
            if (yytoken != Token_COLON)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_COLON, ":");
                  }
                return false;
              }
            yylex();

            innerStatementList_ast *__node_260 = 0;
            if (!parse_innerStatementList(&__node_260))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_innerStatementList, "innerStatementList");
                  }
                return false;
              }
            (*yynode)->statements = __node_260;

            if (yytoken != Token_ENDWHILE)
              {
                if (!yy_block_errors)
                  {
                    yy_expected_token(yytoken, Token_ENDWHILE, "endwhile");
                  }
                return false;
              }
            yylex();

            semicolonOrCloseTag_ast *__node_261 = 0;
            if (!parse_semicolonOrCloseTag(&__node_261))
              {
                if (!yy_block_errors)
                  {
                    yy_expected_symbol(ast_node::Kind_semicolonOrCloseTag, "semicolonOrCloseTag");
                  }
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


} // end of namespace rphp


