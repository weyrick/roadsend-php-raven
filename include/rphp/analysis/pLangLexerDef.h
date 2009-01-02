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
            | token_def(L'=', T_ASSIGN)
            | token_def(L';', T_SEMI)
            | token_def(L',', T_COMMA)
            | token_def(L'!', T_NOT)
            | token_def(L'&', T_REF)
            | token_def(L"\\?>", T_CLOSE_TAG)
            | token_def(L"=>", T_ARROWKEY)
            | token_def(L"if", T_IF)
            | token_def(L"array", T_ARRAY)
            | token_def(L"while", T_WHILE)
            | token_def(L"else", T_ELSE)
            | token_def(L"echo", T_ECHO)
            | token_def(L"new", T_NEW)
            | token_def(L"[a-zA-Z_][a-zA-Z0-9_]*", T_IDENTIFIER)
            | token_def(L"\\$[a-zA-Z_][a-zA-Z0-9_]*", T_VARIABLE)
            | token_def(L"[0-9]+", T_LNUMBER)
            | token_def(L"([0-9]*[\\.][0-9]+)|([0-9]+[\\.][0-9]*)", T_DNUMBER)
            | token_def(L"[ \\t\\n]+", T_WHITESPACE)
            | token_def(L"\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/", T_MULTILINE_COMMENT)
            | token_def(L"\\/\\/.*$", T_SINGLELINE_COMMENT)
            | token_def(L"b*[\"][^\"]*[\"]", T_DQ_STRING)
            | token_def(L"b*[\'][^\']*[\']", T_SQ_STRING)
            ;

    }

};

#define T_DQ_DONE        0
#define T_DQ_DQ          1
#define T_DQ_NEWLINE     2
#define T_DQ_PASSTHROUGH 3

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
