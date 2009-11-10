/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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

#include "rphp/analysis/pParser.h"

#include "rphp/analysis/pLexer.h"
#include "rphp/analysis/pSourceModule.h"

#include <boost/pool/object_pool.hpp>

#include <iostream>

/* generated rphp_grammar parser interface */
void* rphpParseAlloc(void *(*)(size_t));
void  rphpParse(void *, int, rphp::pSourceRange*, rphp::pSourceModule*);
void  rphpParseFree(void *, void (*)(void*));
void  rphpParseTrace(FILE *, char *);

namespace rphp { namespace parser {

void parseSourceFile(pSourceModule* pMod) {

    lexer::pLexer lexer(pMod->source());

    // prepare source buffer for main parse
    lexer.preprocess();

    void* pParser = rphpParseAlloc(malloc);

    // DEBUG
    //rphpParseTrace(stderr, "trace: ");

    // start at begining of source file
    pMod->incLineNum(); // line 1
    pMod->setLastToken(pSourceRange(lexer.sourceBegin(),lexer.sourceBegin()));
    pMod->setLastNewline(lexer.sourceBegin());

    pSourceRange* curRange;
    boost::object_pool<pSourceRange> tokenPool;
    pSourceCharIterator lastNL;
    pUInt nlCnt(0);

    pUInt curID(0);
    std::size_t state(0);
    pSourceCharIterator sourceEnd(lexer.sourceEnd());
    pSourceCharIterator tokStart(lexer.sourceBegin());
    pSourceCharIterator tokEnd(lexer.sourceBegin());

    while ( (curID = rphp_nextLangToken(state, tokEnd, sourceEnd)) ) {

        nlCnt = 0;

        switch (curID) {
            case 0:
                // end of input (success)
                break;
            case T_OPEN_TAG:
            case T_CLOSE_TAG:
                // state change (no parse)
                break;
            case boost::lexer::npos:
                // unmatched token: error
                pMod->parseError(NULL);
                break;
            case T_WHITESPACE:
            case T_INLINE_HTML:
            case T_DOC_COMMENT:
            case T_MULTILINE_COMMENT:
            case T_SINGLELINE_COMMENT:
                // handle newlines
                // O(n)
                for (pSourceCharIterator i = tokStart; i != tokEnd; ++i) {
                    if (*i == '\n') {
                        nlCnt++;
                        lastNL = i;
                    }
                }
                if (nlCnt) {
                    pMod->incLineNum(nlCnt);
                    pMod->setLastNewline(lastNL);
                }
                // only actually parse T_INLINE_HTML, not whitespace
                if (curID == T_INLINE_HTML) {
                    curRange = tokenPool.construct(pSourceRange(tokStart, tokEnd));
                    pMod->setTokenLine(curRange->begin());
                    rphpParse(pParser, curID, curRange, pMod);
                }
                pMod->setLastToken(pSourceRange(tokStart, tokEnd));
                break;
            default:
                // parse
                curRange = tokenPool.construct(pSourceRange(tokStart, tokEnd));
                pMod->setTokenLine(curRange->begin());
                rphpParse(pParser, curID, curRange, pMod);
                pMod->setLastToken(pSourceRange(tokStart, tokEnd));
                break;
        }

        // next token
        tokStart = tokEnd;

    }

    // finish parse
    pMod->finishParse();
    rphpParse(pParser, 0, 0, pMod);
    rphpParseFree(pParser, free);

}


} } // namespace
