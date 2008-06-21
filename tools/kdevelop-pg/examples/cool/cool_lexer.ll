
%option c++
%option yyclass="cool::Lexer"
%option noyywrap


%{

#define DONT_INCLUDE_FLEXLEXER
#include "cool_lexer.h"

%}


Whitespace  [ \t\f\n]

%x IN_BLOCKCOMMENT

%%

 /* whitespace, newlines and comments */

{Whitespace}+   /* skip */ ;
"--".*          /* line comments, skip */ ;

"(*"                    BEGIN(IN_BLOCKCOMMENT);
<IN_BLOCKCOMMENT>{
"*)"            BEGIN(INITIAL);
<<EOF>>         return parser::Token_EOF;
[\n]            /* skip */ ;
.               /* skip */ ;
}


 /* seperators */

"("             return parser::Token_LPAREN;
")"             return parser::Token_RPAREN;
"{"             return parser::Token_LBRACE;
"}"             return parser::Token_RBRACE;
";"             return parser::Token_SEMICOLON;
","             return parser::Token_COMMA;
"."             return parser::Token_DOT;
"@"             return parser::Token_AT;


 /* operators */

"+"             return parser::Token_PLUS;
"-"             return parser::Token_MINUS;
"*"             return parser::Token_STAR;
"/"             return parser::Token_SLASH;
"="             return parser::Token_EQUAL;
"<="            return parser::Token_LESS_EQUAL;
"<"             return parser::Token_LESS;
":"             return parser::Token_COLON;
"<-"            return parser::Token_ARROW_LEFT;
"=>"            return parser::Token_ARROW_RIGHT;
"~"             return parser::Token_TILDE;
"not"           return parser::Token_NOT;
"isvoid"        return parser::Token_ISVOID;


 /* reserved words */

"case"          return parser::Token_CASE;
"esac"          return parser::Token_ESAC;
"class"         return parser::Token_CLASS;
"Class"         return parser::Token_CLASS;
"else"          return parser::Token_ELSE;
"false"         return parser::Token_FALSE;
"if"            return parser::Token_IF;
"in"            return parser::Token_IN;
"fi"            return parser::Token_FI;
"inherits"      return parser::Token_INHERITS;
"let"           return parser::Token_LET;
"loop"          return parser::Token_LOOP;
"new"           return parser::Token_NEW;
"of"            return parser::Token_OF;
"pool"          return parser::Token_POOL;
"then"          return parser::Token_THEN;
"true"          return parser::Token_TRUE;
"while"         return parser::Token_WHILE;


 /* literals */

"\""([^"\\]|\\.)*"\""   return parser::Token_STRING;

[A-Z][a-zA-Z0-9_]*      return parser::Token_TYPE;
[a-z_][a-zA-Z0-9_]*     return parser::Token_IDENTIFIER;
[0-9]+          return parser::Token_INTEGER;


 /* everything else is not a valid lexeme */

.               return parser::Token_INVALID;

%%

namespace cool
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

} // end of namespace cool

