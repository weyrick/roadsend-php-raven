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

#ifndef RPHP_PPARSER_H_
#define RPHP_PPARSER_H_

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <string>

#include "pLexers.h"

using namespace boost::spirit;
using namespace boost::spirit::qi;
using namespace boost::spirit::lex;
using namespace boost::spirit::arg_names;

using namespace boost::phoenix;
using namespace boost::spirit::ascii;
using boost::phoenix::val;
using boost::phoenix::function;

namespace rphp {

struct parse_error_handler_
{
    template <typename, typename, typename>
    struct result { typedef void type; };

    template <typename Iterator>
    void operator()(
        std::string const& what
      , Iterator err_pos, Iterator last) const
    {
        std::cout
            << "Error! Expecting "
            << what                         // what failed?
            << " here: \""
            << std::string(err_pos, last)   // iterators to error-pos, end
            << "\""
            << std::endl
        ;
    }
};

function<parse_error_handler_> const parse_error_handler = parse_error_handler_();

template <typename Iterator, typename Lexer>
struct rphpLangGrammarDef
  : grammar<Iterator, in_state_skipper<typename Lexer::token_set> >
{
    template <typename TokenDef>
    rphpLangGrammarDef(TokenDef const& tok)
      : rphpLangGrammarDef::base_type(program, "program")
    {
        program
            =  +block
            ;

        block
            =   '{' >> *statement >> '}'
            ;

        statement
            =   assignment
            |   if_stmt
            |   while_stmt
            ;

        assignment
            =   (tok.identifier >> '=' >> expression >> ';')
                [
                    std::cout << val("assignment statement to: ") << _1 << "\n"
                ]
            ;

        if_stmt
            =   (   tok.if_ >> '(' >> expression >> ')' >> block
                    >> -(tok.else_ >> block)
                )
                [
                    std::cout << val("if expression: ") << _1 << "\n"
                ]
            ;

        while_stmt
            =   (tok.while_ >> '(' >> expression >> ')' >> block)
                [
                    std::cout << val("while expression: ") << _1 << "\n"
                ]
            ;

        //  since expression has a variant return type accommodating for
        //  std::string and unsigned integer, both possible values may be
        //  returned to the calling rule
        expression
            =   tok.identifier [ _val = _1 ]
            |   tok.variable   [ _val = _1 ]
            |   tok.constant   [ _val = _1 ]
            ;

        //BOOST_SPIRIT_DEBUG_NODE(program);

        program.name("program");
        block.name("block");
        statement.name("statement");
        assignment.name("assignment");
        if_stmt.name("if_stmt");
        while_stmt.name("while_stmt");

        on_error<fail>(program, parse_error_handler(_4, _3, _2));
        /*
        on_error<fail>(block, error_handler(_4, _3, _2));
        on_error<fail>(statement, error_handler(_4, _3, _2));
        on_error<fail>(assignment, error_handler(_4, _3, _2));
        on_error<fail>(if_stmt, error_handler(_4, _3, _2));
        on_error<fail>(while_stmt, error_handler(_4, _3, _2));
        on_error<fail>(expression, error_handler(_4, _3, _2));
        */

    }

    typedef grammar<Iterator, in_state_skipper<typename Lexer::token_set> > base_type;
    typedef typename base_type::skipper_type skipper_type;

    rule<Iterator, skipper_type> program, block, statement;
    rule<Iterator, skipper_type> assignment, if_stmt;
    rule<Iterator, skipper_type> while_stmt;

    //  the expression is the only rule having a return value
    typedef boost::variant<unsigned int, std::string> expression_type;
    rule<Iterator, expression_type(), skipper_type>  expression;
};

// rphp language grammer type
typedef rphpLangGrammarDef<tokIteratorType, lexerEngineType> pLangGrammar;

} // namespace

#endif /* RPHP_PPARSER_H_ */
