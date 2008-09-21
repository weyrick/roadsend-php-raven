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
#include "pASTVisitors.h"

/* generated rphp_grammar parser interface */
void* rphpParseAlloc(void *(*)(size_t));
void  rphpParse(void *, int, rphp::lexer::tokenPairType*, rphp::AST::treeTop*);
void  rphpParseFree(void *, void (*)(void*));
void  rphpParseTrace(FILE *, char *);

namespace rphp { namespace parser {

pModuleP pParser::compileToAST(std::string fileName) {

    pModuleP pMod(new pModule(fileName));
    lexer::pLexer lexer(fileName);

    void* pParser = rphpParseAlloc(malloc);
    AST::treeTop* ast = pMod->getTreeTop();

    // DEBUG
    //rphpParseTrace(stderr, "trace: ");

    for (lexer::tokIteratorType iter = lexer.begin(); iter != lexer.end(); ++iter)
    {
        if ((*iter).id() == 0) {
            // if we didn't match, we switch to state 1 which is our skip_toks (i.e. whitespace, comments)
            iter.set_state(1);
            // if we still haven't matched, then we have a lexer error or end of input
            if ((*iter).id() == 0) {
                std::cerr << "lexical scan error" << std::endl;
                exit(-1);
            }
            // always switch back
            iter.set_state(0);
        }
        else {
            // matched valid token
            rphpParse(pParser, (*iter).id(), &(*iter).value(), ast);
        }
    }

    // finish parse
    rphpParse(pParser, 0, 0, ast);

    rphpParseFree(pParser, free);

    return pMod;

}

void pParser::dumpAST(std::string fileName) {

    pModuleP m = compileToAST(fileName);

    AST::dumpVisitor visitor;
    visitor.visit(m->getTreeTop());
    
}


} } // namespace
