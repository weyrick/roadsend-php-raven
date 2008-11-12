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

#include <iostream>

#include "pLexer.h"
#include "pParser.h"
#include "pModule.h"

/* generated rphp_grammar parser interface */
void* rphpParseAlloc(void *(*)(size_t));
void  rphpParse(void *, int, rphp::sourceRangeType*, rphp::pModule*);
void  rphpParseFree(void *, void (*)(void*));
void  rphpParseTrace(FILE *, char *);

namespace rphp { namespace parser {

void parseSourceFile(std::string fileName, pModule* pMod) {

    lexer::pLexer lexer(fileName);

    void* pParser = rphpParseAlloc(malloc);

    // DEBUG
    //rphpParseTrace(stderr, "trace: ");

    // start at begining of source file
    pMod->incLineNum(); // line 1
    pMod->setLastToken(sourceRangeType(lexer.sourceBegin(),lexer.sourceBegin()));
    pMod->setLastNewline(lexer.sourceBegin()); 

    for (lexer::tokIteratorType iter = lexer.tokBegin(); iter != lexer.tokEnd(); ++iter)
    {
        if ((*iter).id() == 0) {
            // we want to show the source line we are parsing, up to the error
            // it will be lastNewline+1 up until the lastToken.end() + 1
            sourceStringType errorLine(pMod->lastNewline()+1, pMod->lastToken().end()+1);
            sourceStringType errorChar(pMod->lastToken().end(), pMod->lastToken().end()+1);
            std::cerr << errorLine << std::endl;
            // arrow
            std::cerr << sourceStringType((pMod->lastToken().end()+1)-(pMod->lastNewline()+1)-1,' ') << "^" << std::endl;
            std::cerr << "parse error, unexpected: ["
                      << errorChar
                      << "] in "
                      << pMod->filename()
                      << " on line "
                      << pMod->currentLineNum()
                      <<  std::endl;
            exit(-1);
        }
        else {
            // matched valid token. either switch state, or pass to parser
            if ((*iter).id() == T_OPEN_TAG) {
                // go to php
                iter.set_state(1);
            }
            else if ((*iter).id() == T_CLOSE_TAG) {
                // go to html
                iter.set_state(0);
            }
            else if ((*iter).id() == T_WHITESPACE || (*iter).id() == T_INLINE_HTML) {
                // count newlines, save last one, skip
                sourceIteratorType lastNL;
                pUInt nlCnt(0);
                // O(n) for each token
                for (sourceIteratorType i = (*iter).value().begin(); i != (*iter).value().end(); ++i) {
                    if (*i == '\n') {
                        nlCnt++;
                        lastNL = i;
                    }                    
                }
                if (nlCnt) {
                    pMod->incLineNum(nlCnt);
                    pMod->setLastNewline(lastNL);
                }
                // allow actually parse T_INLINE_HTML, not whitespace
                if ((*iter).id() == T_INLINE_HTML)
                    rphpParse(pParser, (*iter).id(), &(*iter).value(), pMod);
                pMod->setLastToken((*iter).value());
            }
            else {
                // parse
                rphpParse(pParser, (*iter).id(), &(*iter).value(), pMod);
                pMod->setLastToken((*iter).value());
            }
        }
    }

    // finish parse
    rphpParse(pParser, 0, 0, pMod);

    rphpParseFree(pParser, free);

}


} } // namespace
