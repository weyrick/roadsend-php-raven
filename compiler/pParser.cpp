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

#include <iostream>

#include "pLexer.h"
#include "pParser.h"
#include "pModule.h"

/* generated rphp_grammar parser interface */
void* rphpParseAlloc(void *(*)(size_t));
void  rphpParse(void *, int, rphp::lexer::tokenPairType*, rphp::pModule*);
void  rphpParseFree(void *, void (*)(void*));
void  rphpParseTrace(FILE *, char *);

namespace rphp { namespace parser {

void parseSourceFile(std::string fileName, pModule* pMod) {

    lexer::pLexer lexer(fileName);

    void* pParser = rphpParseAlloc(malloc);

    // DEBUG
    //rphpParseTrace(stderr, "trace: ");

    for (lexer::tokIteratorType iter = lexer.begin(); iter != lexer.end(); ++iter)
    {
        if ((*iter).id() == 0) {
            std::cerr << "lexical scan error, unexpected: [" << (*iter).value() << "]" << std::endl;
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
            else if ((*iter).id() == T_WHITESPACE) {
                // skip
            }
            else {
                // parse
                rphpParse(pParser, (*iter).id(), &(*iter).value(), pMod);
            }
        }
    }

    // finish parse
    rphpParse(pParser, 0, 0, pMod);

    rphpParseFree(pParser, free);

}


} } // namespace
