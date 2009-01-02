/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
   ***** END LICENSE BLOCK *****
*/

#ifndef RPHP_PLANGLEXERDEF_H_
#define RPHP_PLANGLEXERDEF_H_

//#define BOOST_SPIRIT_LEXERTL_DEBUG
//#define BOOST_SPIRIT_DEBUG

#include <boost/spirit/include/lex_lexer_lexertl.hpp>
#include <string>

#include "rphp/analysis/pSourceTypes.h"
#include "rphp_grammar.h"

namespace rphp { namespace lexer {

using namespace boost::spirit::lex;

template <typename Lexer>
struct rphpLangTokens : lexer_def<Lexer>
{

    typedef token_def<pSourceString, pSourceString::value_type> token_def;

    template <typename Self>
    void def (Self& self)
    {

        // macros
        //void add_pattern (char_type const* state, string_type const& name, string_type const& patterndef)

        // HTML (default) state tokens
        self
            = token_def(L"<\\?|<\\?PHP|<\\?php", T_OPEN_TAG)
            | token_def(L".+|\\n+", T_INLINE_HTML)
            ;

        // PHP state tokens
        self(L"PHP")
            = token_def(L'(', T_LEFTPAREN)
            | token_def(L')', T_RIGHTPAREN)
            | token_def(L'{', T_LEFTCURLY)
            | token_def(L'}', T_RIGHTCURLY)
            | token_def(L'[', T_LEFTSQUARE)
            | token_def(L']', T_RIGHTSQUARE)
            | token_def(L'>', T_GREATER_THAN)
            | token_def(L'<', T_LESS_THAN)
            | token_def(L'=', T_ASSIGN)
            | token_def(L'-', T_MINUS)
            | token_def(L'+', T_PLUS)
            | token_def(L'/', T_DIV)
            | token_def(L'%', T_MOD)
            | token_def(L'*', T_MULT)
            | token_def(L';', T_SEMI)
            | token_def(L',', T_COMMA)
            | token_def(L'.', T_DOT)
            | token_def(L'!', T_NOT)
            | token_def(L'&', T_REF)
            | token_def(L'@', T_AT)
            | token_def(L'?', T_QUESTION)
            | token_def(L':', T_COLON)
            | token_def(L">=", T_GREATER_THAN_OR_EQUAL)
            | token_def(L"<=", T_LESS_THAN_OR_EQUAL)
            | token_def(L"::", T_DBL_COLON)
            | token_def(L"\\+\\+", T_INC)
            | token_def(L"\\-\\-", T_DEC)
            | token_def(L"\\+\\=", T_PLUS_EQUAL)
            | token_def(L"\\-\\=", T_MINUS_EQUAL)
            | token_def(L"\\!\\=", T_NOTEQUAL)
            | token_def(L"\\.\\=", T_DOTEQUAL)
            | token_def(L"==", T_EQUAL)
            | token_def(L"\\!==", T_NOT_IDENTICAL)
            | token_def(L"===", T_IDENTICAL)
            | token_def(L"\\&\\&", T_BOOLEAN_AND)
            | token_def(L"\\|\\|", T_BOOLEAN_OR)
            | token_def(L"\\?>", T_CLOSE_TAG)
            | token_def(L"=>", T_ARROWKEY)
            | token_def(L"->", T_CLASSDEREF)
            | token_def(L"list", T_LIST)
            | token_def(L"or", T_LOGICAL_OR)
            | token_def(L"and", T_LOGICAL_AND)
            | token_def(L"xor", T_LOGICAL_XOR)
            | token_def(L"if", T_IF)
            | token_def(L"for", T_FOR)
            | token_def(L"foreach", T_FOREACH)
            | token_def(L"as", T_AS)
            | token_def(L"exit", T_EXIT)
            | token_def(L"public", T_PUBLIC)
            | token_def(L"private", T_PRIVATE)
            | token_def(L"protected", T_PROTECTED)
            | token_def(L"extends", T_EXTENDS)
            | token_def(L"return", T_RETURN)
            | token_def(L"global", T_GLOBAL)
            | token_def(L"function", T_FUNCTION)
            | token_def(L"isset", T_ISSET)
            | token_def(L"unset", T_UNSET)
            | token_def(L"empty", T_EMPTY)
            | token_def(L"array", T_ARRAY)
            | token_def(L"while", T_WHILE)
            | token_def(L"else", T_ELSE)
            | token_def(L"elseif", T_ELSEIF)
            | token_def(L"echo", T_ECHO)
            | token_def(L"new", T_NEW)
            | token_def(L"var", T_VAR)
            | token_def(L"switch", T_SWITCH)
            | token_def(L"case", T_CASE)
            | token_def(L"break", T_BREAK)
            | token_def(L"continue", T_CONTINUE)
            | token_def(L"default", T_DEFAULT)
            | token_def(L"instanceof", T_INSTANCEOF)
            | token_def(L"class", T_CLASS)
            | token_def(L"clone", T_CLONE)
            | token_def(L"throw", T_THROW)
            | token_def(L"try", T_TRY)
            | token_def(L"catch", T_CATCH)
            | token_def(L"const", T_CONST)
            | token_def(L"static", T_STATIC)
            | token_def(L"include", T_INCLUDE)
            | token_def(L"include_once", T_INCLUDE_ONCE)
            | token_def(L"require", T_REQUIRE)
            | token_def(L"require_once", T_REQUIRE_ONCE)
            | token_def(L"[a-zA-Z_][a-zA-Z0-9_]*", T_IDENTIFIER)
            | token_def(L"\\$[a-zA-Z_][a-zA-Z0-9_]*", T_VARIABLE)
            | token_def(L"[0-9]+", T_LNUMBER)
            | token_def(L"([0-9]*[\\.][0-9]+)|([0-9]+[\\.][0-9]*)", T_DNUMBER)
            | token_def(L"[ \\t\\n]+", T_WHITESPACE)
            | token_def(L"\\/\\*\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/", T_DOC_COMMENT)
            | token_def(L"\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/", T_MULTILINE_COMMENT)
            | token_def(L"\\/\\/.*$", T_SINGLELINE_COMMENT)
            | token_def(L"b*[\"](\\\\\\\"|[^\"])*[\"]", T_DQ_STRING)
            | token_def(L"b*[\'](\\\\\\.|[^\'])*[\']", T_SQ_STRING)
            ;

    }

};

#define T_DQ_DONE        0
#define T_DQ_DQ          1
#define T_DQ_NEWLINE     2
#define T_DQ_PASSTHROUGH 3
#define T_DQ_ESCAPE      4

template <typename Lexer>
struct rphpDQTokens : lexer_def<Lexer>
{

    typedef token_def<pSourceString, pSourceString::value_type> token_def;

    template <typename Self>
    void def (Self& self)
    {


        self
            = token_def(L"\\\\n", T_DQ_NEWLINE)
            | token_def(L'"', T_DQ_DQ)
            | token_def(L"\\\\\\\"", T_DQ_ESCAPE)
            // FIXME: this is inefficient. does lexertl have an "unmatched"?
            | token_def(L".{1}", T_DQ_PASSTHROUGH)
            ;

    }

};

// token type. this should list all types used as return values in the
// token lexer definition
typedef lexertl_token<pSourceCharIterator> pSourceToken;

// use the lexertl based lexer engine.
typedef lexertl_lexer<pSourceToken> pLexerEngine;

// specialize the grammar tokens with this engine
typedef rphpLangTokens<pLexerEngine> pLangTokens;

// specialize the double quote tokens
typedef rphpDQTokens<pLexerEngine> pPreprocessTokens;

// actual lexer types
typedef boost::spirit::lex::lexer<pLangTokens> pLangLexer;
typedef boost::spirit::lex::lexer<pPreprocessTokens> pPreprocessLexer;

// this is the iterator type exposed by the lexer, which dereferences to
// a token
typedef pLangLexer::iterator_type pTokenIterator;

} } // namespace

#endif /* RPHP_PLANGLEXERDEF_H_ */
