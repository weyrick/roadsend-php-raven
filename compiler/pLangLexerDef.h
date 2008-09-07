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

#ifndef RPHP_PLANGLEXERDEF_H_
#define RPHP_PLANGLEXERDEF_H_

//#define BOOST_SPIRIT_LEXERTL_DEBUG
//#define BOOST_SPIRIT_DEBUG

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexer_lexertl.hpp>
#include <string>

#include "pTokens.h"

namespace rphp { namespace lexer {

using namespace boost::spirit;
using namespace boost::spirit::lex;

template <typename Lexer>
struct rphpLangTokens : lexer_def<Lexer>
{
    typedef typename Lexer::token_set token_set;

    template <typename Self>
    void def (Self& self)
    {

        if_ = token_def<omitted>("if", T_IF);
        while_ = token_def<omitted>("while", T_WHILE);
        else_ = token_def<omitted>("else", T_ELSE);
        echo = token_def<omitted>("echo", T_ECHO);

        identifier = token_def<std::string>("[a-zA-Z_][a-zA-Z0-9_]*", T_IDENTIFIER);

        //dqstring = token_def<omitted>("\"([^\"\\\\]|\\\\.)*\"", T_CONSTANT_ENCAPSED_STRING);
        dqstring = token_def<std::string>("[\"][^\"]*[\"]", T_CONSTANT_ENCAPSED_STRING);
        variable = token_def<std::string>("\\$[a-zA-Z_][a-zA-Z0-9_]*", T_VARIABLE);
        lnumber = token_def<unsigned int>("[0-9]+", T_LNUMBER);

        opentag = token_def<omitted>("<\\?", T_OPEN_TAG);
        closetag = token_def<omitted>("\\?>", T_CLOSE_TAG);

        skip_toks
           = token_def<std::string>("[ \\t\\n]+", T_WHITESPACE)
           | token_def<std::string>("\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/", T_ML_COMMENT)
           | token_def<std::string>("\\/\\/.*$", T_SL_COMMENT);

        // associate the tokens and the token set with the lexer
        self += token_def<>('(') | ')' | '{' | '}' | '=' | ';';
        self += if_ | while_ | else_ | echo | opentag | closetag;
        self += identifier | variable | lnumber | dqstring;

        // whitespace tokens in WS lexer state
        self("WS") = skip_toks | opentag | closetag;

    }

    // TOKEN defs
    // NOTE: any type used as a return value here must also be defined in the vector
    // that defines our langTokenType (see below)

    // these tokens have no value
    token_def<omitted> if_, while_, else_, echo, opentag, closetag;

    // tokens with string value
    token_def<std::string> identifier, variable, dqstring;

    // tokens with int value
    // TODO: change this to pInt
    token_def<unsigned int> lnumber;

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
typedef boost::spirit::lex::lexer<pLangTokens> pLangLexer;

// this is the iterator type exposed by the lexer, which dereferences to
// a token
typedef boost::spirit::lex::lexer<pLangTokens>::iterator_type tokIteratorType;

} } // namespace

#endif /* RPHP_PLANGLEXERDEF_H_ */
