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

#ifndef RPHP_PGENERATOR_H_
#define RPHP_PGENERATOR_H_

#include <queue>
#include <string>
#include <llvm/Support/IRBuilder.h>
#include "pASTVisitors.h"
#include "pIRTypes.h"

namespace llvm {
    class Module;
    class Value;
}

namespace rphp {

class pCompileTarget;

class pGenerator: public AST::defaultVisitor {

    llvm::Module* llvmModule;
    pCompileTarget* target;
    pIRTypes IRTypes;

    std::string entryFunctionName;

    llvm::IRBuilder currentBlock;
    llvm::Value* runtimeEngine;
    std::queue<llvm::Value*> valueStack;

    void createEntryPoint(void);

    void doEchoLiteralStringIR(void);

public:

    pGenerator(llvm::Module* m, pCompileTarget* t): llvmModule(m), target(t) {
        createEntryPoint();
    }

    // destructor: we don't own any of our member objects

    void finalize(void);

    // nodes
    void visit_echoStmt(AST::echoStmt*);
    void visit_literalBString(AST::literalBString*);
    void visit_inlineHtml(AST::inlineHtml*);

};

} // namespace

#endif /* RPHP_PGENERATOR_H_ */
