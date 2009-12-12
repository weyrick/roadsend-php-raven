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

#include "rphp/IR/pDumpTarget.h"

#include "rphp/analysis/pSourceModule.h"
#include "rphp/analysis/pLexer.h"
//#include "rphp/IR/pGenerator.h"
//#include "rphp/IR/pGenSupport.h"

#include <llvm/Module.h>
#include <llvm/Analysis/Verifier.h>

#include <iostream>

namespace rphp {

void pDumpTarget::execute(void) {

    switch (type_) {
        case Tokens:
            dumpTokens();
            break;
            /*
        case Preprocessor:
            dumpPre();
            break;
            */
        case AST:
            dumpAST();
            break;
        case IR:
            dumpIR();
            break;
    }

}

// lex and then dump tokens from the given source file
void pDumpTarget::dumpTokens(void) {

    pSourceFile* source = new pSourceFile(sourceFile_);
    lexer::pLexer l(source);
    l.dumpTokens();
    delete source;

}

// preprocess the source file, dump to stdout
/*
void pDumpTarget::dumpPre(void) {

    pSourceFile* source = new pSourceFile(sourceFile_);
    lexer::pLexer l(source);
    bool success = l.preprocess();
    if (!success) {
        std::cout << "preprocess failed due to lexer error" << std::endl;
    }
    else {
        std::cout << l.contents();
    }
    delete source;

}
*/

// dump the parse tree from the given source file
void pDumpTarget::dumpAST(void) {

    pSourceModule m(sourceFile_);
    m.dumpAST();

}

// dump the generated IR code from the given source file
void pDumpTarget::dumpIR(void) {
/*
    pSourceModule  m(sourceFile_);
    IR::pGenerator codeGen(m, llvm::getGlobalContext());
    llvm::Module* ir = codeGen.getIR();
    IR::pGenSupport::dumpIR(ir);
    std::string errMsg;
    if (llvm::verifyModule(*ir, llvm::ReturnStatusAction, &errMsg)) {
        std::cout << "*** IR verify failed: " << errMsg << std::endl;
    }
    delete ir;
*/
}

} // namespace

