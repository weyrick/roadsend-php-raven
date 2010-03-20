/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009 Shannon Weyrick <weyrick@roadsend.com>
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

#include "rphp/analysis/passes/SimplifyStrings.h"
#include "rphp/analysis/pSourceModule.h"
#include "rphp/analysis/lexer/consts.hpp"

// NOTE: these are generated during build, and so live
// in the build directory (not source directory)
#include "rphp_grammar.h" // token defines
#include "rphp_dq_lexer.h"

namespace rphp { namespace AST { namespace Pass {


/**
 * Simplify double quoted strings by parsing them according to double quote
 * rules and creating child nodes concatenated together
 *
 * We always return a simple string or binaryOp here.
 * lVal of the concat operation is always a simple string or variable, while
 * the right can descend to further concats.
 *
 */
expr* SimplifyStrings::simplifyString(pSourceCharIterator in_b, pSourceCharIterator in_e) {

    pUInt dqID(0);
    std::size_t dqUniqueID(0);
    pSourceString buffer;

    expr* returnNode(0);
    literalString* s(0);

    // we now use the dq lexer to find the tokens within
    // this double quoted string
    pSourceCharIterator dqTokStart(in_b);
    pSourceCharIterator dqTokEnd(in_b);
    while ( (dqID = rphp_nextDQToken(dqTokEnd, in_e, dqUniqueID)) )
    {

        // iterate over DQ tokens
        switch(dqID) {
        case T_DQ_DONE:
            goto endOfDQ; // omg!
        case T_DQ_ESCAPE:
            // replace with non escaped dq
            buffer.push_back('"');
            break;
            /*
        case T_DQ_DQ:
            // replace with single
            buffer.push_back('\'');
            break;
            */
        case T_DQ_NEWLINE:
            // replace newline escape sequence with literal newline
            buffer.push_back('\n');
            break;
        case T_DQ_VARIABLE:
            // replace with concatenation
            //std::cout << std::string(dqTokStart, dqTokEnd) << "\n";
            if (buffer.empty()) {
                // if the buffer is empty, then the lVal of out concat is var
                // and the rVal is the result of a recurse on the rest of the inbuf (without variable)
                AST::var* v = new (C_) AST::var(pSourceRange(dqTokStart+1, dqTokEnd), C_);
                if(dqTokEnd == in_e)
                    returnNode = v;
                else
                    returnNode = new (C_) AST::binaryOp(v, simplifyString(dqTokEnd, in_e), binaryOp::CONCAT);
                goto endOfDQ;
            }
            else {
                // if the buffer is not empty, the lVal is the buffer (already simple) and rVal
                // is the result of a recurse on the rest of the inbuf (with variable)
                s = new (C_) AST::literalString(buffer);
                s->setIsSimple(true);
                returnNode = new (C_) AST::binaryOp(s, simplifyString(dqTokStart, in_e), binaryOp::CONCAT);
                goto endOfDQ;
            }

            break;
        case boost::lexer::npos:
        default:
            // passthrough
            buffer.append(dqTokStart, dqTokEnd);
            break;
        }

        dqTokStart = dqTokEnd;

    }

    // if we get here, then we have a simple string left in buffer. make that the return value as a literal
    // note that this will copy and store buffer
    s = new (C_) AST::literalString(buffer);
    s->setIsSimple(true);
    returnNode = s;

    endOfDQ:

    return returnNode;

}

expr* SimplifyStrings::transform_post_literalString(literalString* n) {

    expr* node;

    // only work on non-simple strings
    if (!n->isSimple()) {
        node = simplifyString(n->getStringVal().begin(), n->getStringVal().end());
    }
    else {
        // no transform
        node = n;
    }

    return node;

}

} } } // namespace

