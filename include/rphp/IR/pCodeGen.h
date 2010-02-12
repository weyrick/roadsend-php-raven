/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009-2010 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PCODEGEN_H_
#define RPHP_PCODEGEN_H_

#include <queue>
#include <string>
#include <stack>

#include <boost/unordered_map.hpp>

#include <llvm/Support/IRBuilder.h>

#include "rphp/IR/pIRHelper.h"
#include "rphp/analysis/pBaseVisitor.h"

namespace llvm {
    class Module;
    class Value;
}

namespace rphp { namespace IR {

// TODO: llvm has structures for this. use those?
typedef boost::unordered_map<pIdentString, llvm::Value*> symbolTableType;

class pIRHelper;

class pCodeGen: public AST::pBaseVisitor {

public:
    typedef std::vector<llvm::Value*> valueVectorType;

private:
    llvm::Module* llvmModule_; // won't free
    pIdentString functionSymbol_;
    pIRHelper IRHelper_;

    llvm::IRBuilder<> currentBlock_;
    llvm::Value* runtimeEngine_; // don't own

    llvm::BasicBlock* allocBlock;
    llvm::BasicBlock* beginBlock;
    std::stack<llvm::BasicBlock*> endingIfBlocks_;
    llvm::Function* thisFunction_;

    std::stack<llvm::Value*> valueStack_;
    //TODO: The destructList_ currently contains one vector of Value*'s for destruction, evaluate
    // if we still need a queue for that or if one vector is enough.
    std::queue<valueVectorType> destructList_;

    symbolTableType symTable_;

    unsigned int wordSize_;
    unsigned int ifRecursionDepth_;

    pUInt lastSourceLoc_;

    void updateSourceLocation(const AST::stmt*);

    llvm::Value* newVarOnStack(pStringRef);
    llvm::BasicBlock* visitInOwnBlock(AST::stmt* n, const std::string &Name = "");
public:
    pCodeGen(pSourceModule* m, llvm::Module* mod, const pIdentString& funSym);
    ~pCodeGen(void);

    void finalize(void);

    // nodes
    void visit_pre_builtin(AST::builtin*);
    //void visit_inlineHtml(AST::inlineHtml*);
    void visit_pre_literalString(AST::literalString*);
    void visit_pre_literalInt(AST::literalInt*);
    void visit_pre_literalFloat(AST::literalFloat*);
    void visit_pre_literalBool(AST::literalBool*);
    void visit_pre_literalNull(AST::literalNull*);
    void visit_pre_literalArray(AST::literalArray*);
    void visit_pre_assignment(AST::assignment*);
    void visit_pre_var(AST::var*);
    void visit_pre_functionInvoke(AST::functionInvoke*);
    void visit_pre_ifStmt(AST::ifStmt*);
    void visit_pre_unaryOp(AST::unaryOp* n);

};

} } // namespace

#endif /* RPHP_PCODEGEN_H_ */
