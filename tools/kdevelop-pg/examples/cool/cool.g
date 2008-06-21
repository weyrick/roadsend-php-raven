
------------------------------------------------------------
-- Parser class members
------------------------------------------------------------

%parserclass (public declaration)
[:
  /**
   * Transform the raw input into tokens.
   * When this method returns, the parser's token stream has been filled
   * and any parse_*() method can be called.
   */
  void tokenize( char *contents );

  enum problem_type {
      error,
      warning,
      info
  };
  void report_problem( parser::problem_type type, const char* message );
  void report_problem( parser::problem_type type, std::string message );
:]


------------------------------------------------------------
-- List of defined tokens
------------------------------------------------------------

-- keywords:
%token CLASS ("class"), INHERITS ("inherits"), NEW ("new"),
       IF ("if"), THEN ("then"), ELSE ("else"), FI ("fi"), WHILE ("while"),
       LOOP ("loop"), POOL ("pool"), LET ("let"), IN ("in"),
       CASE ("case"), OF ("of"), ESAC ("esac") ;;

-- seperators:
%token LPAREN ("("), RPAREN (")"), LBRACE ("{"), RBRACE ("}"), SEMICOLON (";"),
       COMMA (","), DOT ("."), AT ("@") ;;

-- operators:
%token PLUS ("+"), MINUS ("-"), STAR ("*"), SLASH ("/"), EQUAL ("="),
       LESS_EQUAL ("<="), LESS ("<"), COLON (":"), ARROW_LEFT ("<-"),
       ARROW_RIGHT ("=>"), TILDE ("~"), NOT ("not"), ISVOID ("isvoid") ;;

-- literals and identifiers:
%token IDENTIFIER ("identifier"), TYPE ("type specification"),
       INTEGER ("integer literal"), STRING ("string literal"),
       TRUE ("true"), FALSE ("false") ;;

-- token that makes the parser fail in any case:
%token INVALID ("invalid token") ;;



------------------------------------------------------------
-- Start of the actual grammar
------------------------------------------------------------

   (#klass=class SEMICOLON)*
-> program ;;

   CLASS type=TYPE (INHERITS base_type=TYPE | 0) LBRACE (#feature=feature SEMICOLON)* RBRACE
-> class ;;

   name=IDENTIFIER COLON type=TYPE
-> formal ;;

 ( ?[: LA(2).kind == Token_LPAREN :]
   name=IDENTIFIER LPAREN (#formal=formal @ COMMA | 0) RPAREN
   COLON type=TYPE LBRACE expression=expression RBRACE
 |
   name=IDENTIFIER COLON type=TYPE (ARROW_LEFT expression=expression | 0)
 )
-> feature ;;

 (
   ?[: LA(2).kind == Token_ARROW_LEFT :]
   name=IDENTIFIER ARROW_LEFT expression=expression                  -- assignment
 |
   ?[: LA(2).kind == Token_LPAREN :]
   name=IDENTIFIER LPAREN (#argument=expression @ COMMA | 0) RPAREN  -- dispatch
 |
   variable=IDENTIFIER
 | integer_literal=INTEGER
 | string_literal=STRING
 | true_literal=TRUE
 | false_literal=FALSE
 | NEW new_type=TYPE
 | LPAREN expression=expression RPAREN
 | if_expression=if_expression
 | while_expression=while_expression
 | block_expression=block_expression
 | let_expression=let_expression
 | case_expression=case_expression
 )
-> primary_expression ;;

   op=TILDE  expression=primary_expression -- ^tilde_expression
 | op=NOT    expression=primary_expression -- ^not_expression
 | op=ISVOID expression=primary_expression -- ^isvoid_expression
 |           expression=primary_expression
-> unary_expression ;;

   base_expression=unary_expression
   (AT at_type=TYPE DOT name=IDENTIFIER LPAREN (#arguments=expression @ COMMA | 0) RPAREN
   |                DOT name=IDENTIFIER LPAREN (#arguments=expression @ COMMA | 0) RPAREN
   )*
-> postfix_expression ;;

   #expression=postfix_expression @ (op=STAR | op=SLASH)
-> multiplicative_expression ;;

   #expression=multiplicative_expression @ (op=PLUS | op=MINUS)
-> additive_expression ;;

   #expression=additive_expression @ (op=EQUAL | op=LESS_EQUAL | op=LESS)
-> relational_expression ;;

   IF condition=expression THEN true_expression=expression ELSE false_expression=expression FI
-> if_expression ;;

   WHILE condition=expression LOOP loop_expression=expression POOL
-> while_expression ;;

   LBRACE (#expression=expression SEMICOLON)* RBRACE
-> block_expression ;;

   LET #declaration=let_declaration @ COMMA IN body_expression=expression
-> let_expression ;;

   CASE expression=expression OF (#condition=case_condition SEMICOLON)* ESAC
-> case_expression ;;

   expression=relational_expression
-> expression ;;

   name=IDENTIFIER COLON type=TYPE (ARROW_LEFT expression=expression | 0)
-> let_declaration ;;

   name=IDENTIFIER COLON type=TYPE ARROW_RIGHT expression=expression
-> case_condition ;;




-----------------------------------------------------------------
-- Code segments copied to the implementation (.cpp) file.
-- If existent, kdevelop-pg's current syntax requires this block
-- to occur at the end of the file.
-----------------------------------------------------------------

[:
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

:]
