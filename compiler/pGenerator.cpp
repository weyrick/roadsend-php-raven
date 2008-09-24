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

#include <llvm/Module.h>
#include <llvm/GlobalVariable.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Constants.h>
#include <llvm/Instructions.h>

#include "pCompileTarget.h"
#include "pGenerator.h"

using namespace llvm;

namespace rphp {

void pGenerator::createEntryFunctionName(const std::string& inName) {

    // TODO: this needs to set an appropriate name based on file name
    // and project root (from target)
    entryFunctionName = inName;

}
    
void pGenerator::createEntryPoint(void) {

    // script top level initialization function
    createEntryFunctionName(llvmModule->getModuleIdentifier());

    // function type: void (*)(void)
    FunctionType *initFunType = FunctionType::get(llvm::Type::VoidTy, /* return type */
                                                  std::vector<const llvm::Type*>() /* arguments */,
                                                  false /*not vararg*/);

    // function
    Function *initFun = Function::Create(initFunType, Function::ExternalLinkage, entryFunctionName, llvmModule);
     
    // entry block
    currentBlock.SetInsertPoint(BasicBlock::Create("EntryBlock", initFun));

}

void pGenerator::visit(AST::treeTop* n) {
    std::cout << "generator: treeTop: " << n->statementList.size() << " top level statements in module" << std::endl;
    AST::defaultVisitor::visit(n);
}

void pGenerator::visit(AST::statementNode* n) {
    std::cout << "generator: statementNode" << std::endl;
    AST::defaultVisitor::visit(n);
}

void pGenerator::visit(AST::echoNode* n) {
    std::cout << "generator: echoNode, string: " << n->rVal << std::endl;
}

} // namespace

