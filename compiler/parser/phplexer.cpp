/* KDevelop PHP Support
 *
 * Copyright 2008 Niko Sams <niko.sams@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "phplexer.h"

#include "rphp_parser.h"

#include <kdev-pg-location-table.h>
#include <kdev-pg-token-stream.h>

#include <iostream>
#include <string>
#include "unicode/utypes.h"
#include "unicode/uchar.h"

// TODO check cursor position after returning from a method and if they change the former state, or not!
// TODO pos and m_curpos the same?

    namespace rphp
    {

        Lexer::Lexer( parser* _parser, const UnicodeString& content ):
        m_content( content ), m_parser( _parser ),
        m_curpos( 0 ), m_contentSize( m_content.length() ),
        m_tokenBegin( 0 ), m_tokenEnd( 0 ), m_haltCompiler( 0 )
{
    pushState( ErrorState );
    pushState( HtmlState );
}

int Lexer::state(int deepness) const
{

    // was: return m_state[ m_state.size() - deepness - 1 ]; size()
    return m_state[ m_state.size() - deepness - 1 ];
}
void Lexer::printState()
{
    int s = state();
    if (s == ErrorState)
        std::cout << "ErrorState";
    else if (s == HtmlState)
        std::cout << "HtmlState";
    else if (s == DefaultState)
        std::cout << "DefaultState";
    else if (s == String)
        std::cout << "String";
    else if (s == StringVariable)
        std::cout << "StringVariable";
    else if (s == StringVariableBracket)
        std::cout << "StringVariableBracket";
    else if (s == StringVariableObjectOperator)
        std::cout << "StringVariableObjectOperator";
    else if (s == StringVariableCurly)
        std::cout << "StringVariableCurly";
    else if (s == StringVarname)
        std::cout << "StringVarname";
    else if (s == StringHeredoc)
        std::cout << "StringHeredoc";
    else if (s == StringBacktick)
        std::cout << "StringBacktick";
}

void Lexer::pushState( int state )
{
    m_state.push_back( state );
    //m_state.push( state ); which is a QVector::append()
}

void Lexer::popState()
{
    m_state.pop_back();
//    m_state.pop(); which is a QVector::data[ size() - 1 ]
}

int Lexer::nextTokenKind()
{
    int token = parser::Token_INVALID;
    if ( m_curpos >= m_contentSize )
    {
        m_tokenBegin = -1;
        m_tokenEnd = -1;
        return 0;
    }
    int pos = m_curpos;
    #ifdef THOMAS_TEMP_DISABLED
    // TODO temp. disabled code, tokenBegin was not in use
    m_tokenBegin = m_curpos;
    #endif
    switch ( state() )
    {
        case HtmlState:
            if (lookAt( pos ) == '<' && lookAt( pos + 1 ) == '?') {
                token = parser::Token_OPEN_TAG;
                if ( lookAt( pos + 2 ) == '=')
                {
                    token = parser::Token_OPEN_TAG_WITH_ECHO;
                    m_curpos++;
                    pos++;
                }
                // TODO toLower, isSpace
                else if (
                       (lookAt( pos + 2 )  == 'p' || lookAt( pos + 2 ) == 'P')
                    && (lookAt( pos + 3 ) == 'h' || lookAt( pos + 3 ) == 'H')
                    && (lookAt( pos + 4 ) == 'p' || lookAt( pos + 4 ) == 'P')
                    && lookAt( pos + 5 ) == ' ')
                {
                    m_curpos += 4;
                }
                m_curpos++;
                pushState(DefaultState);
            } else {
                token = parser::Token_INLINE_HTML;
                while( m_curpos < m_contentSize )
                {
                    if (lookAt( pos ) == '\n') createNewline(m_curpos);
                    if ( lookAt( pos + 1 ) == '<' && lookAt( pos + 2 ) == '?') {
                        break;
                    }
                    pos++;
                    m_curpos++;
                }
            }
            break;
        case DefaultState:
        case StringVariableCurly:
        {
            // isSpace
            if ( lookAt( pos ) == ' ')
            {
                token = parser::Token_WHITESPACE;
                while (m_curpos < m_contentSize && lookAt( pos ) == ' ') {
                    if ( lookAt( pos ) == '\n') createNewline(m_curpos);
                    pos; // weiterspringen!!!!
                    m_curpos++;
                }
                m_curpos--;
            }
            else if (u_isdigit( lookAt( pos ) )
                    || ( lookAt( pos )  == '.' && u_isdigit( lookAt( pos + 1 ) ) ))
            {
		UnicodeString num;bool hasPoint = false;
                bool hex = false;
                if ( lookAt( pos ) == '0' && ( lookAt( pos + 1 )) == 'x') {
                    pos += 2;
                    m_curpos += 2;
                    hex = true;
                }
                while (m_curpos < m_contentSize && (
                            u_isdigit( lookAt( pos ) )
                        || (!hex && !hasPoint && lookAt( pos ) == '.')
                        || (hex && (u_tolower( lookAt( pos ) ) == 'a' || u_tolower( lookAt( pos ) ) == 'b' ||
                                    u_tolower( lookAt( pos ) ) == 'c' || u_tolower( lookAt( pos ) ) == 'd' ||
                                    u_tolower( lookAt( pos ) ) == 'e' || u_tolower( lookAt( pos ) ) == 'f'))))
                {
                    if (lookAt( pos ) == '.') hasPoint = true;
                    num.append(lookAt( pos ));
                    pos++;
                    m_curpos++;
                }
                if (!hex && u_tolower( lookAt( pos ) ) == 'e' &&
                        (u_isdigit( lookAt( pos ) ) ||
                            ((lookAt( pos + 1 ) == '-' || lookAt( pos + 1 ) == '+') && u_isdigit( lookAt( pos + 2 ) ) )))
                {
                    //exponential number
                    token = parser::Token_DNUMBER;
                    m_curpos++;
                    pos++;
                    if (lookAt( pos ) == '-' || lookAt( pos ) == '+') {
                        pos++;
                        m_curpos++;
                    }
                    while (m_curpos < m_contentSize && ( u_isdigit( lookAt( pos ) ) )) {
                        pos++;
                        m_curpos++;
                    }
                    m_curpos--;
                } else {
                    m_curpos--;
                    if (hasPoint) {
                        token = parser::Token_DNUMBER;
                    } else {
                        #ifdef THOMAS_TEMP_DISABLED
                        // TODO temp. disabled code, toLong()
                        bool ok;
                        //check if string can be converted to long
                        //if we get an overflow use double
                        num.toLong(&ok, hex ? 16 : 10);
                        if (ok) {
                            token = parser::Token_LNUMBER;
                        } else {
                            token = parser::Token_DNUMBER;
                        }
                        #endif
                    }
                }

            }
            else if (processVariable( lookAt( pos ) ))
            {
                token = parser::Token_VARIABLE;
            }
            else if (lookAt( pos ) == '$')
            {
                //when *it was not recognized as variable
                token = parser::Token_DOLLAR;
            }
            else if (lookAt( pos ) == '}')
            {
                token = parser::Token_RBRACE;
                if (state() == StringVariableCurly) {
                    popState();
                }
            }
            else if (lookAt( pos ) == '{')
            {
                token = parser::Token_LBRACE;
                if (state() == StringVariableCurly) {
                    pushState(StringVariableCurly);
                }
            }
            else if (lookAt( pos ) == ')')
            {
                token = parser::Token_RPAREN;
            }
            else if (lookAt( pos ) == '(')
            {
                pos++;
                int _pos = m_curpos + 1;
                while (_pos < m_contentSize && lookAt( pos ) == ' ')
                {
                    pos++;
                    _pos++;
                }
                UnicodeString name;
                while (_pos < m_contentSize && u_isalnum( lookAt( pos ) ))
                {
                    name.append( lookAt( pos ) );
                    pos++;
                    _pos++;
                }
                while (_pos < m_contentSize && lookAt( pos ) == ' ')
                {
                    pos++;
                    _pos++;
                }
                name = name.toLower();
                if (lookAt( pos ) == ')')
                {
                    if (name == "int" || name == "integer")
                    {
                        token = parser::Token_INT_CAST;
                    }
                    else if (name == "real" || name == "double" || name == "float")
                    {
                        token = parser::Token_DOUBLE_CAST;
                    }
                    else if (name == "string")
                    {
                        token = parser::Token_STRING_CAST;
                    }
                    else if (name == "binary")
                    {
                        //as in php
                        token = parser::Token_STRING_CAST;
                    }
                    else if (name == "array")
                    {
                        token = parser::Token_ARRAY_CAST;
                    }
                    else if (name == "object")
                    {
                        token = parser::Token_OBJECT_CAST;
                    }
                    else if (name == "bool" || name == "boolean")
                    {
                        token = parser::Token_BOOL_CAST;
                    }
                    else if (name == "unset")
                    {
                        token = parser::Token_UNSET_CAST;
                    }
                    else
                    {
                        token = parser::Token_LPAREN;
                    }

                    if (token != parser::Token_LPAREN)
                    {
                        m_curpos = _pos;
                    }
                }
                else
                {
                    token = parser::Token_LPAREN;
                }
            }
            else if (lookAt( pos ) == ']')
            {
                token = parser::Token_RBRACKET;
            }
            else if (lookAt( pos ) == '[')
            {
                token = parser::Token_LBRACKET;
            }
            else if (lookAt( pos ) == ',')
            {
                token = parser::Token_COMMA;
            }
            else if (lookAt( pos ) == '@')
            {
                token = parser::Token_AT;
            }
            else if (lookAt( pos ) == '!')
            {
                if (lookAt( pos + 1 ) == '=')
                {
                    m_curpos++;
                    if (lookAt( pos + 2 ) == '=')
                    {
                        m_curpos++;
                        token = parser::Token_IS_NOT_IDENTICAL;
                    }
                    else
                    {
                        token = parser::Token_IS_NOT_EQUAL;
                    }
                }
                else
                {
                    token = parser::Token_BANG;
                }
            }
            else if (lookAt( pos ) == '<')
            {
                if (lookAt( pos + 1 ) == '<')
                {
                    m_curpos++;
                    if (lookAt( pos + 2 ) == '<' && state() != StringVariableCurly)
                    {
                        //HEREDOC string (<<< EOD\nfoo\nEOD;\n)
                        int _pos = 3;
                        while (m_curpos+_pos < m_contentSize &&
                               ( u_isalnum( lookAt( pos + _pos ) ) == ' ' || u_isalnum( lookAt( pos + _pos ) ) == '\t'))
                        {
                            _pos++;
                        }
                        if ( u_isalnum( lookAt( pos + _pos ) ) || lookAt( pos + _pos ) == '_') //identifier must start with a letter
                        {
                            m_heredocIdentifier.remove();
                            while (m_curpos+_pos < m_contentSize &&
                                ( u_isdigit( lookAt( pos + _pos ) ) || u_isalnum( lookAt( pos + _pos ) ) || lookAt( pos + _pos ) == '_'))
                            {
                                m_heredocIdentifier.append( lookAt( pos + _pos ) );
                                _pos++;
                            }
                            if ( lookAt( pos + _pos ) == '\n') {
                                //identifier must be followed by newline, newline is part of HEREDOC token
                                token = parser::Token_START_HEREDOC;
                                pushState(StringHeredoc);
                                m_curpos += _pos-1;
                            }
                        }
                    }

                    if (token != parser::Token_START_HEREDOC)
                    {
                        if (lookAt( pos + 2 ) == '=')
                        {
                            m_curpos++;
                            token = parser::Token_SL_ASSIGN;
                        }
                        else
                        {
                            token = parser::Token_SL;
                        }
                    }
                }
                else if (lookAt( pos + 1 ) == '=')
                {
                    m_curpos++;
                    token = parser::Token_IS_SMALLER_OR_EQUAL;
                }
                else if (lookAt( pos + 1 ) == '>')
                {
                    m_curpos++;
                    token = parser::Token_IS_NOT_EQUAL;
                }
                else
                {
                    token = parser::Token_IS_SMALLER;
                }
            }
            else if (lookAt( pos ) == '>')
            {
                if (lookAt( pos + 1 ) == '>')
                {
                    m_curpos++;
                    if (lookAt( pos + 2 ) == '=')
                    {
                        m_curpos++;
                        token = parser::Token_SR_ASSIGN;
                    }
                    else
                    {
                        token = parser::Token_SR;
                    }
                }
                else if (lookAt( pos + 1 ) == '=')
                {
                    m_curpos++;
                    token = parser::Token_IS_GREATER_OR_EQUAL;
                }
                else
                {
                    token = parser::Token_IS_GREATER;
                }
            }
            else if (lookAt( pos ) == '~')
            {
                token = parser::Token_TILDE;
            }
            else if (lookAt( pos ) == ':')
            {
                if (lookAt( pos + 1 ) == ':') {
                    m_curpos++;
                    token = parser::Token_PAAMAYIM_NEKUDOTAYIM;
                } else {
                    token = parser::Token_COLON;
                }
            }
            else if (lookAt( pos ) == '?')
            {
                if (lookAt( pos + 1 ) == '>')
                {
                    //accept CLOSE_TAG inside StringVariableCurly too, as php does
                    token = parser::Token_CLOSE_TAG;
                    m_curpos++;
                    if (lookAt( pos + 2 ) == '\n') m_curpos++;
                    while (state() != HtmlState) popState();
                }
                else
                {
                    token = parser::Token_QUESTION;
                }
            }
            else if (lookAt( pos ) == '-' && lookAt( pos + 1 ) == '>')
            {
                m_curpos++;
                token = parser::Token_OBJECT_OPERATOR;
                if (isValidVariableIdentifier( lookAt( pos + 2 ) ) ) {
                    pushState(StringVariableObjectOperator);
                }
            }
            else if (lookAt( pos ) == '%')
            {
                if (lookAt( pos + 1 ) == '=') {
                    m_curpos++;
                    token = parser::Token_MOD_ASSIGN;
                } else {
                    token = parser::Token_MOD;
                }
            }
            else if (lookAt( pos ) == '/')
            {
                if (lookAt( pos + 1 ) == '=')
                {
                    m_curpos++;
                    token = parser::Token_DIV_ASSIGN;
                }
                else if (lookAt( pos + 1 ) == '/')
                {
                    //accept COMMENT inside StringVariableCurly too, as php does
                    token = parser::Token_COMMENT;
                    while (m_curpos < m_contentSize && lookAt( pos ) != '\n' &&
                           !(lookAt( pos + 1 ) == '?' && lookAt( pos + 2 ) == '>'))
                    {
                        pos++;
                        m_curpos++;
                    }
                }
                else if (lookAt( pos + 1 ) == '*')
                {
                    //accept COMMENT inside StringVariableCurly too, as php does
                    if (lookAt( pos + 2 ) == '*' && lookAt( pos + 3 ) == ' ')
                    {
                        token = parser::Token_DOC_COMMENT;
                    }
                    else
                    {
                        token = parser::Token_COMMENT;
                    }
                    pos += 2;
                    m_curpos += 2;
                    while (m_curpos < m_contentSize && !(lookAt( pos ) == '*' && lookAt( pos + 1 ) == '/'))
                    {
                        pos++;
                        m_curpos++;
                    }
                    m_curpos++;
                } else {
                    token = parser::Token_DIV;
                }
            }
            else if (lookAt( pos ) == '#')
            {
                //accept COMMENT inside StringVariableCurly too, as php does
                token = parser::Token_COMMENT;
                while (m_curpos < m_contentSize && lookAt( pos ) != '\n')
                {
                    pos++;
                    m_curpos++;
                }
            }
            else if (lookAt( pos ) == '^')
            {
                if (lookAt( pos + 1 ) == '=') {
                    m_curpos++;
                    token = parser::Token_XOR_ASSIGN;
                } else {
                    token = parser::Token_BIT_XOR;
                }
            }
            else if (lookAt( pos ) == '*')
            {
                if (lookAt( pos + 1 ) == '=') {
                    m_curpos++;
                    token = parser::Token_MUL_ASSIGN;
                } else {
                    token = parser::Token_MUL;
                }
            }
            else if (lookAt( pos ) == '|')
            {
                if (lookAt( pos + 1 ) == '|') {
                    m_curpos++;
                    token = parser::Token_BOOLEAN_OR;
                } else if (lookAt( pos + 1 ) == '=') {
                    m_curpos++;
                    token = parser::Token_OR_ASSIGN;
                } else {
                    token = parser::Token_BIT_OR;
                }
            }
            else if (lookAt( pos ) == '&')
            {
                if (lookAt( pos + 1 ) == '&') {
                    m_curpos++;
                    token = parser::Token_BOOLEAN_AND;
                } else if (lookAt( pos + 1 ) == '=') {
                    m_curpos++;
                    token = parser::Token_AND_ASSIGN;
                } else {
                    token = parser::Token_BIT_AND;
                }
            }
            else if (lookAt( pos ) == '+')
            {
                if (lookAt( pos + 1 ) == '+') {
                    m_curpos++;
                    token = parser::Token_INC;
                } else if (lookAt( pos + 1 ) == '=') {
                    m_curpos++;
                    token = parser::Token_PLUS_ASSIGN;
                } else {
                    token = parser::Token_PLUS;
                }
            }
            else if (lookAt( pos ) == '-')
            {
                if (lookAt( pos + 1 ) == '-') {
                    m_curpos++;
                    token = parser::Token_DEC;
                } else if (lookAt( pos + 1 ) == '=') {
                    m_curpos++;
                    token = parser::Token_MINUS_ASSIGN;
                } else {
                    token = parser::Token_MINUS;
                }
            }
            else if (lookAt( pos ) == '.')
            {
                if (lookAt( pos + 1 ) == '=') {
                    m_curpos++;
                    token = parser::Token_CONCAT_ASSIGN;
                } else {
                    token = parser::Token_CONCAT;
                }

            }
            else if (lookAt( pos ) == ';')
            {
                token = parser::Token_SEMICOLON;
            }
            else if (lookAt( pos ) == '\'')
            {
                token = parser::Token_CONSTANT_ENCAPSED_STRING;
                pos++;
                m_curpos++;
                int startPos = m_curpos;
                while (m_curpos < m_contentSize
                        && (lookAt( pos ) != '\'' || isEscapedWithBackslash(lookAt( pos ), m_curpos, startPos, pos)))
                {
                    pos++;
                    m_curpos++;
                }
            }
            else if (lookAt( pos ) == '"')
            {
                pos++;
                int i = 0;
                bool foundVar = false;
                while (m_curpos < m_contentSize
                        && (lookAt( pos ) != '"' || isEscapedWithBackslash(lookAt( pos ), m_curpos+i, m_curpos, pos)))
                {
                    if (lookAt( pos ) == '$'  && !isEscapedWithBackslash(lookAt( pos ), m_curpos+i, m_curpos, pos)
                            && (lookAt( pos + 1 ) == '{'
                                   || (isValidVariableIdentifier( lookAt( pos + 1 ) ) && ! u_isdigit( lookAt( pos + 1 ) )))) {
                        foundVar = true;
                    }
                    pos++;
                    i++;
                }
                if (!foundVar)
                {
                    token = parser::Token_CONSTANT_ENCAPSED_STRING;
                    m_curpos += i + 1;
                }
                else
                {
                    token = parser::Token_DOUBLE_QUOTE;
                    pushState(String);
                }
            }
            else if (lookAt( pos ) == '`')
            {
                token = parser::Token_BACKTICK;
                pushState(StringBacktick);
            }
            else if (lookAt( pos ) == '=')
            {
                if (lookAt( pos + 1 ) == '=')
                {
                    m_curpos++;
                    if (lookAt( pos + 2 ) == '=')
                    {
                        m_curpos++;
                        token = parser::Token_IS_IDENTICAL;
                    }
                    else
                    {
                        token = parser::Token_IS_EQUAL;
                    }
                }
                else if (lookAt( pos + 1 ) == '>')
                {
                    m_curpos++;
                    token = parser::Token_DOUBLE_ARROW;
                }
                else
                {
                    token = parser::Token_ASSIGN;
                }
            }
            else if (isValidVariableIdentifier(lookAt( pos )) && !u_isdigit( lookAt( pos ) ))
            {
		UnicodeString name;
                while (m_curpos < m_contentSize && (isValidVariableIdentifier(lookAt( pos )))) {
                    name.append(lookAt( pos ));
                    pos++;
                    m_curpos++;
                }
                m_curpos--;
                name = name.toLower();
                if (name == "echo") {
                    token = parser::Token_ECHO;
                } else if (name == "include") {
                    token = parser::Token_INCLUDE;
                } else if (name == "include_once") {
                    token = parser::Token_INCLUDE_ONCE;
                } else if (name == "require") {
                    token = parser::Token_REQUIRE;
                } else if (name == "require_once") {
                    token = parser::Token_REQUIRE_ONCE;
                } else if (name == "eval") {
                    token = parser::Token_EVAL;
                } else if (name == "print") {
                    token = parser::Token_PRINT;
                } else if (name == "abstract") {
                    token = parser::Token_ABSTRACT;
                } else if (name == "break") {
                    token = parser::Token_BREAK;
                } else if (name == "case") {
                    token = parser::Token_CASE;
                } else if (name == "catch") {
                    token = parser::Token_CATCH;
                } else if (name == "class") {
                    token = parser::Token_CLASS;
                } else if (name == "const") {
                    token = parser::Token_CONST;
                } else if (name == "continue") {
                    token = parser::Token_CONTINUE;
                } else if (name == "default") {
                    token = parser::Token_DEFAULT;
                } else if (name == "do") {
                    token = parser::Token_DO;
                } else if (name == "else") {
                    token = parser::Token_ELSE;
                } else if (name == "extends") {
                    token = parser::Token_EXTENDS;
                } else if (name == "final") {
                    token = parser::Token_FINAL;
                } else if (name == "for") {
                    token = parser::Token_FOR;
                } else if (name == "if") {
                    token = parser::Token_IF;
                } else if (name == "implements") {
                    token = parser::Token_IMPLEMENTS;
                } else if (name == "instanceof") {
                    token = parser::Token_INSTANCEOF;
                } else if (name == "interface") {
                    token = parser::Token_INTERFACE;
                } else if (name == "new") {
                    token = parser::Token_NEW;
                } else if (name == "private") {
                    token = parser::Token_PRIVATE;
                } else if (name == "protected") {
                    token = parser::Token_PROTECTED;
                } else if (name == "public") {
                    token = parser::Token_PUBLIC;
                } else if (name == "return") {
                    token = parser::Token_RETURN;
                } else if (name == "static") {
                    token = parser::Token_STATIC;
                } else if (name == "switch") {
                    token = parser::Token_SWITCH;
                } else if (name == "throw") {
                    token = parser::Token_THROW;
                } else if (name == "try") {
                    token = parser::Token_TRY;
                } else if (name == "while") {
                    token = parser::Token_WHILE;
                } else if (name == "clone") {
                    token = parser::Token_CLONE;
                } else if (name == "exit" || name == "die") {
                    token = parser::Token_EXIT;
                } else if (name == "elseif") {
                    token = parser::Token_ELSEIF;
                } else if (name == "endif") {
                    token = parser::Token_ENDIF;
                } else if (name == "endwhile") {
                    token = parser::Token_ENDWHILE;
                } else if (name == "endfor") {
                    token = parser::Token_ENDFOR;
                } else if (name == "foreach") {
                    token = parser::Token_FOREACH;
                } else if (name == "endforeach") {
                    token = parser::Token_ENDFOREACH;
                } else if (name == "declare") {
                    token = parser::Token_DECLARE;
                } else if (name == "enddeclare") {
                    token = parser::Token_ENDDECLARE;
                } else if (name == "as") {
                    token = parser::Token_AS;
                } else if (name == "endswitch") {
                    token = parser::Token_ENDSWITCH;
                } else if (name == "function") {
                    token = parser::Token_FUNCTION;
                } else if (name == "use") {
                    token = parser::Token_USE;
                } else if (name == "global") {
                    token = parser::Token_GLOBAL;
                } else if (name == "var") {
                    token = parser::Token_VAR;
                } else if (name == "unset") {
                    token = parser::Token_UNSET;
                } else if (name == "isset") {
                    token = parser::Token_ISSET;
                } else if (name == "empty") {
                    token = parser::Token_EMPTY;
                } else if (name == "__halt_compiler") {
                    token = parser::Token_HALT_COMPILER;
                } else if (name == "list") {
                    token = parser::Token_LIST;
                } else if (name == "array") {
                    token = parser::Token_ARRAY;
                } else if (name == "__class__") {
                    token = parser::Token_CLASS_C;
                } else if (name == "__method__") {
                    token = parser::Token_METHOD_C;
                } else if (name == "__function__") {
                    token = parser::Token_FUNC_C;
                } else if (name == "__line__") {
                    token = parser::Token_LINE;
                } else if (name == "__file__") {
                    token = parser::Token_FILE;
                } else if (name == "or") {
                    token = parser::Token_LOGICAL_OR;
                } else if (name == "and") {
                    token = parser::Token_LOGICAL_AND;
                } else if (name == "xor") {
                    token = parser::Token_LOGICAL_XOR;
                } else {
                    token = parser::Token_STRING;
                }
            }
            break;
        }

        case StringVariable:
        case String:
        case StringHeredoc:
        case StringBacktick:
            if ((state() == String || state(1) == String) && lookAt( pos ) == '"')
            {
                token = parser::Token_DOUBLE_QUOTE;
                if (state() == StringVariable) popState();
                popState();
            }
            else if ((state() == StringBacktick || state(1) == StringBacktick) && lookAt( pos ) == '`')
            {
                token = parser::Token_BACKTICK;
                if (state() == StringVariable) popState();
                popState();
            }
            else if ((state() == StringHeredoc || state(1) == StringHeredoc) && isHeredocEnd(lookAt( pos ), pos))
            {
                token = parser::Token_END_HEREDOC;
                m_curpos += m_heredocIdentifier.length()-1;
                if (state() == StringVariable) popState();
                popState();
            }
            else if (processVariable(lookAt( pos )))
            {
                token = parser::Token_VARIABLE;
                if (state() != StringVariable) pushState(StringVariable);
            }
            else if (state() != StringVariable  && lookAt( pos ) == '$' && lookAt( pos + 1 ) == '{')
            {
                token = parser::Token_DOLLAR_OPEN_CURLY_BRACES;
                m_curpos++;
                pos += 2;
                //check if a valid variable follows
                if ((isValidVariableIdentifier(lookAt( pos )) && ! u_isdigit( lookAt( pos ) )))
                {
                    pushState(StringVarname);
                }

            }
            else if (state() == StringVariable && lookAt( pos ) == '[')
            {
                token = parser::Token_LBRACKET;
                pushState(StringVariableBracket);
            }
            else if (state() != StringVariable && lookAt( pos ) == '{' && lookAt( pos + 1 ) == '$'
                    && (isValidVariableIdentifier( lookAt( pos + 2 ) ) && ! u_isdigit( lookAt( pos + 2 ) ) || lookAt( pos + 2 ) == '{'))
            {
                token = parser::Token_CURLY_OPEN;
                pushState(StringVariableCurly);
            }
            else if (state() == StringVariable
                    && lookAt( pos ) == '-' && lookAt( pos + 1 ) == '>'
                    && isValidVariableIdentifier( lookAt( pos + 2 ) ) && ! u_isdigit( lookAt( pos + 2 ) ))
            {
                token = parser::Token_OBJECT_OPERATOR;
                m_curpos++;
                pushState(StringVariableObjectOperator);
            }
            else
            {
                if (state() == StringVariable) popState();
                token = parser::Token_ENCAPSED_AND_WHITESPACE;
                int startPos = m_curpos;
                while ( m_curpos < m_contentSize )
                {

                    if (!isEscapedWithBackslash(lookAt( pos ), m_curpos, startPos, pos) &&
                         (lookAt( pos ) == '$' && lookAt( pos + 1 ) == '{' ||
                          lookAt( pos ) == '{' && lookAt( pos + 1 ) == '$' ||
                          lookAt( pos ) == '$' && isValidVariableIdentifier( lookAt( pos + 1 ) ) && ! u_isdigit( lookAt( pos + 1 ) )))
                    {
                        //variable is next ${var} or {$var}
                        break;
                    }
                    if (state() == String && lookAt( pos ) == '"'
                        && !isEscapedWithBackslash(lookAt( pos ), m_curpos, startPos, pos))
                    {
                        //end of string
                        break;
                    }
                    if (state() == StringBacktick && lookAt( pos ) == '`'
                        && !isEscapedWithBackslash(lookAt( pos ), m_curpos, startPos, pos))
                    {
                        //end of string
                        break;
                    }

                    if ( lookAt( pos ) == '\n') createNewline(m_curpos);
                    m_curpos++;
                    pos++;

                    if (state() == StringHeredoc && lookAt( pos - 1 ) == '\n')
                    {
                        //check for end of heredoc (\nEOD;\n)
                        if (state() == StringHeredoc && isHeredocEnd(lookAt( pos ), pos)) {
                            break;
                        }
                    }
                }
                m_curpos--;
            }
            break;
        case StringVariableBracket:
            if (lookAt( pos ) == ']')
            {
                token = parser::Token_RBRACKET;
                popState();
                popState();
            }
            else if ( u_isdigit( lookAt( pos ) ))
            {
                token = parser::Token_NUM_STRING;
                while (m_curpos < m_contentSize && u_isdigit( lookAt( pos ) ))
                {
                    pos++;
                    m_curpos++;
                }
                m_curpos--;
            }
            else
            {
                token = parser::Token_STRING;
                while (m_curpos < m_contentSize && (lookAt( pos ) != ']'))
                {
                    if (lookAt( pos ) == '\n') createNewline(m_curpos);
                    pos++;
                    m_curpos++;
                }
                m_curpos--;
            }
            break;
        case StringVariableObjectOperator:
            token = parser::Token_STRING;
            while (m_curpos < m_contentSize && isValidVariableIdentifier(lookAt( pos )))
            {
                pos++;
                m_curpos++;
            }
            m_curpos--;
            popState();
            if (state() == StringVariable) popState();
            break;
        case StringVarname:
            popState();
            pushState(StringVariableCurly);
            token = parser::Token_STRING_VARNAME;
            while (m_curpos < m_contentSize && isValidVariableIdentifier(lookAt( pos )))
            {
                pos++;
                m_curpos++;
            }
            m_curpos--;
            break;
        default:
            token = parser::Token_INVALID;
            break;
    }
    if ( m_curpos >= m_contentSize )
    {
        m_tokenBegin = -1;
        m_tokenEnd = -1;
        return 0;
    }
    m_tokenEnd = m_curpos;
    m_curpos++;

    if (m_haltCompiler) {
        //look for __halt_compiler(); and stop lexer there
        if (m_haltCompiler == 4) {
            token = 0; //EOF
        } else if (token == parser::Token_WHITESPACE || token == parser::Token_COMMENT || token == parser::Token_DOC_COMMENT) {
            //ignore
        } else if (m_haltCompiler == 1 && token == parser::Token_LPAREN) {
            m_haltCompiler++;
        } else if (m_haltCompiler == 2 && token == parser::Token_RPAREN) {
            m_haltCompiler++;
        } else if (m_haltCompiler == 3 && token == parser::Token_SEMICOLON) {
            m_haltCompiler++;
        } else {
            m_haltCompiler = 0;
        }
    }
    if (token == parser::Token_HALT_COMPILER && !m_haltCompiler) {
        m_haltCompiler = 1;
    }
    return token;
}

rint64 Lexer::tokenBegin() const
{
    return m_tokenBegin;
}

rint64 Lexer::tokenEnd() const
{
    return m_tokenEnd;
}

bool Lexer::isHeredocEnd(const UChar32& it, int pos)
{
    int identiferLen = m_heredocIdentifier.length();
    UnicodeString lineStart;
    for (int i = 0; i < identiferLen; i++) {
        if (m_curpos+i >= m_contentSize) break;
        lineStart.append( lookAt( pos + i ) );
    }
    if (lineStart == m_heredocIdentifier &&
        (lookAt( pos + identiferLen ) == '\n'
            || (lookAt( pos + identiferLen ) == ';' &&
                    lookAt( pos + identiferLen + 1 ) == '\n')))
    {
        return true;
    }
    return false;
}

//used for strings, to check if " is escaped (\" is, \\" not)
bool Lexer::isEscapedWithBackslash(const UChar32& it, int curPos, int startPos, int pos)
{
    int cnt = 0;
    pos--;
    while (curPos > startPos && lookAt( pos ) == '\\') {
        cnt++;
        pos--;
    }
    return (cnt % 2) == 1;
}

bool Lexer::processVariable(int pos)
{
    UChar32 c2 = lookAt( pos + 1 );
    if ( lookAt( pos ) == '$' && (isValidVariableIdentifier(c2) && ! u_isdigit( c2 )))
    {
        pos++;
        m_curpos++;
        while (m_curpos < m_contentSize
                && (isValidVariableIdentifier( lookAt( pos ) )))
        {
            pos++;
            m_curpos++;
        }
        m_curpos--;
        return true;
    }
    else
    {
        return false;
    }
}
bool Lexer::isValidVariableIdentifier(const UChar32& it)
{
    // TODO check: it > 0x7f
    return u_isalnum( it ) || u_isdigit( it ) || it == '_' || it > 0x7f;
}

void Lexer::createNewline( int pos )
{
    #ifdef PENDING_THOMAS
    // TODO pending add newline to the tokenStream
    if( m_parser )
        m_parser->tokenStream->locationTable()->newline( pos );
    #endif
}

}

