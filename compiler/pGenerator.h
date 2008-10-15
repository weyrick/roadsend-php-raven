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

#ifndef RPHP_PGENERATOR_H_
#define RPHP_PGENERATOR_H_

#include <queue>
#include <string>
#include <llvm/Support/IRBuilder.h>
#include "pASTVisitors.h"
#include "pIRHelper.h"

namespace llvm {
    class Module;
    class Value;
}

namespace rphp {

class pCompileTarget;

class pGenerator: public AST::defaultVisitor {

public:
    typedef std::vector<llvm::Value*> valueVectorType;

private:
    llvm::Module* llvmModule; // don't own
    pCompileTarget* target; // don't own
    pIRHelper* IRHelper; // own

    std::string entryFunctionName;

    llvm::IRBuilder currentBlock;
    llvm::Value* runtimeEngine; // don't own

    llvm::Function* currentFunction;

    std::queue<llvm::Value*> valueStack;
    std::queue<valueVectorType> destructList;

private:
    void loadAndLinkRuntimeIR(void);
    void createEntryPoint(void);

    llvm::Value* newVarOnStack(void);

    //void emitEchoLiteralString(void);

    // pVar
    /*
    llvm::Value* emitVarCreate(void);
    llvm::Value* emitVarCreate_pBString(llvm::Value*);
    llvm::Value* emitVarCreate_pInt(llvm::Value*);

    void emitVarConstruct(llvm::Value*);
    void emitVarDestruct(llvm::Value*);
    */
    
public:

    pGenerator(llvm::Module* m, pCompileTarget* t);
    ~pGenerator(void);

    void finalize(void);

    // nodes
    void visit_echoStmt(AST::echoStmt*);
    void visit_inlineHtml(AST::inlineHtml*);
    void visit_literalBString(AST::literalBString*);
    void visit_literalInt(AST::literalInt*);

};

} // namespace

#endif /* RPHP_PGENERATOR_H_ */
