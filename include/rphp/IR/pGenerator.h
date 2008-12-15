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

#include <boost/unordered_map.hpp>

#include <llvm/Support/IRBuilder.h>

#include "rphp/analysis/pASTVisitors.h"

namespace llvm {
    class Module;
    class Value;
}

namespace rphp {

// TODO: llvm has structures for this. use those?
typedef boost::unordered_map<pIdentString, llvm::Value*> symbolTableType;

class pIRHelper;

class pGenerator: public AST::baseVisitor {

public:
    typedef std::vector<llvm::Value*> valueVectorType;

private:
    llvm::Module* llvmModule_; // won't free
    pIRHelper* IRHelper_; // own

    std::string entryFunctionName_;

    llvm::IRBuilder<> currentBlock_;
    llvm::Value* runtimeEngine_; // don't own

    llvm::Function* currentFunction_;

    std::queue<llvm::Value*> valueStack_;
    std::queue<valueVectorType> destructList_;

    symbolTableType globalSymbols_;

private:
    void loadAndLinkRuntimeIR(void);
    void createEntryPoint(void);

    llvm::Value* newVarOnStack(const char*);
    
public:

    pGenerator(const std::string& moduleName);
    ~pGenerator(void);

    void finalize(void);

    llvm::Module* getIR(void) { return llvmModule_; }
    const std::string& entryFunctionName(void) const { return entryFunctionName_; }

    // nodes
    void visit_echoStmt(AST::echoStmt*);
    void visit_inlineHtml(AST::inlineHtml*);
    void visit_literalString(AST::literalString*);
    void visit_literalInt(AST::literalInt*);
    void visit_literalFloat(AST::literalFloat*);
    void visit_literalBool(AST::literalBool*);
    void visit_literalNull(AST::literalNull*);
    void visit_assignment(AST::assignment*);
    void visit_var(AST::var*);
    void visit_functionInvoke(AST::functionInvoke*);

};

} // namespace

#endif /* RPHP_PGENERATOR_H_ */
