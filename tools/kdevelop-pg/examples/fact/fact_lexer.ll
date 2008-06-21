
%option c++
%option yyclass="fact::Lexer"
%option noyywrap


%{

#define DONT_INCLUDE_FLEXLEXER
#include "fact_lexer.h"

%}


Letter      [a-zA-Z_]
Digit       [0-9]
Whitespace  [ \t\f\n]
%%

{Whitespace}+   /* skip */ ;


 /* operators and seperators */

"("         return parser::Token_LPAREN;
")"         return parser::Token_RPAREN;
"{"         return parser::Token_LBRACE;
"}"         return parser::Token_RBRACE;
","         return parser::Token_COMMA;
";"         return parser::Token_SEMICOLON;
"=="        return parser::Token_EQUAL;
"="         return parser::Token_ASSIGN;
"*"         return parser::Token_STAR;
"-"         return parser::Token_MINUS;


 /* reserved words */

"if"        return parser::Token_IF;
"else"      return parser::Token_ELSE;
"var"       return parser::Token_VAR;
"function"  return parser::Token_FUNCTION;
"return"    return parser::Token_RETURN;


 /* identifiers and number literals */

{Letter}({Letter}|{Digit})*   return parser::Token_IDENTIFIER;
{Digit}+    return parser::Token_NUMBER;


 /* everything else is not a valid lexeme */

.           return parser::Token_INVALID;

%%

namespace fact
{

Lexer::Lexer( parser *parser, char *contents )
{
    restart( parser, contents );
}

void Lexer::restart( parser *parser, char *contents )
{
    m_parser = parser;
    m_locationTable = parser->token_stream->location_table();
    m_contents = contents;
    m_tokenBegin = m_tokenEnd = 0;
    m_currentOffset = 0;

    // check for and ignore the UTF-8 byte order mark
    unsigned char *ucontents = (unsigned char *) m_contents;
    if ( ucontents[0] == 0xEF && ucontents[1] == 0xBB && ucontents[2] == 0xBF )
    {
        m_tokenBegin = m_tokenEnd = 3;
        m_currentOffset = 3;
    }

    yyrestart(NULL);
    BEGIN(INITIAL); // is not set automatically by yyrestart()
}

// reads a character, and returns 1 as the number of characters read
// (or 0 when the end of the string is reached)
int Lexer::LexerInput( char *buf, int /*max_size*/ )
{
    int c = m_contents[ m_currentOffset++ ];

    switch(c)
    {
    case '\r':
        c = '\n'; // only have one single line break character: '\n'
        if ( m_contents[m_currentOffset + 1] == '\n' )
        {
            m_currentOffset++;
            m_tokenEnd++;
        }

        // fall through
    case '\n':
        m_locationTable->newline( m_currentOffset );
        break;

    default:
        break;
    }

    return (c == 0) ? 0 : (buf[0] = c, 1);
}

} // end of namespace fact

