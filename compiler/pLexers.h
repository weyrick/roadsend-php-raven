/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
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

#ifndef RPHP_PLEXERS_H_
#define RPHP_PLEXERS_H_

//#define BOOST_SPIRIT_LEXERTL_DEBUG
//#define BOOST_SPIRIT_DEBUG

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexer_lexertl.hpp>
#include <string>

#include "pTokens.h"

namespace rphp {

using namespace boost::spirit;
using namespace boost::spirit::lex;

template <typename Lexer>
struct rphpLangTokens : lexer_def<Lexer>
{
    typedef typename Lexer::token_set token_set;

    template <typename Self>
    void def (Self& self)
    {

        identifier = "[a-zA-Z_][a-zA-Z0-9_]*";
        variable = ("\\$[a-zA-Z_][a-zA-Z0-9_]*", T_VARIABLE);
        constant = "[0-9]+";

        if_ = ("if", T_IF);
        while_ = "while";
        else_ = "else";

        skip_toks
           = token_def<std::string>("[ \\t\\n]+")
           | token_def<std::string>("\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/")
           | token_def<std::string>("\\/\\/.*$");

        // associate the tokens and the token set with the lexer
        self += token_def<>('(') | ')' | '{' | '}' | '=' | ';' | constant;
        self += if_ | while_ | else_ | identifier | variable;

        // whitespace tokens in WS lexer state
        self("WS") = skip_toks;

    }

    // TOKEN defs
    // NOTE: any type used as a return value here must also be defined in the vector
    // that defines our langTokenType (see below)

    // these tokens have no value
    token_def<omitted> if_, while_, else_;

    // tokens with string value
    token_def<std::string> identifier, variable;

    // tokens with int value
    token_def<unsigned int> constant;

    // token set to be used as the skip parser (whitespace and comments)
    token_set skip_toks;
};

// iterator type used to expose the underlying input stream
typedef std::string::iterator sourceIteratorType;

// token type. this should list all types used as return values in the
// token lexer definition
typedef lexertl_token<
    sourceIteratorType, boost::mpl::vector<unsigned int, std::string>
> languageTokenType;

// Here we use the lexertl based lexer engine.
typedef lexertl_lexer<languageTokenType> lexerEngineType;

// specialize the tokens with this engine
typedef rphpLangTokens<lexerEngineType> pLangTokens;

// actual lexer type
typedef lexer<pLangTokens> pLangLexer;

// this is the iterator type exposed by the lexer, which dereferences to
// a token
typedef lexer<pLangTokens>::iterator_type tokIteratorType;


} // namespace

#endif /* RPHP_PLEXERS_H_ */
