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

#include <llvm/Module.h>
#include <llvm/Linker.h>
#include <llvm/Support/IRBuilder.h>

#include "rphp/IR/pCompileError.h"
#include "rphp/IR/pGenerator.h"
#include "rphp/IR/pGenSupport.h"
#include "rphp/IR/pDeclare.h"
#include "rphp/IR/pCodeGen.h"

using namespace llvm;

namespace rphp { namespace IR {

pGenerator::pGenerator(pSourceModule& mod):
    llvmModule_(new Module(mod.fileName())),
    sourceModule_(mod),
    entryFunctionName_(),
    IRHelper_(llvmModule_)
{

    loadAndLinkRuntimeIR();
    createEntryPoint();
    runPasses();
    
}

void pGenerator::runPasses() {

    pDeclare* declarePass = new pDeclare(llvmModule_);
    sourceModule_.applyVisitor(declarePass);
    delete declarePass;

    AST::statementList& topStmts = sourceModule_.getAST();
    pCodeGen* codeGenPass;

    // first codegen all declarations (i.e. functions and methods)
    for (AST::statementList::iterator i = topStmts.begin();
         i != topStmts.end();
         ++i) {
        if ((*i)->getKind() == AST::functionDeclKind) {
            AST::functionDecl* f = static_cast<AST::functionDecl*>(*i);
            codeGenPass = new pCodeGen(llvmModule_, f->functionDef()->name());
            codeGenPass->visit(f);
            delete codeGenPass;
        }
    }

    // now global
    codeGenPass = new pCodeGen(llvmModule_, entryFunctionName_);
    sourceModule_.applyVisitor(codeGenPass);
    delete codeGenPass;

}

void pGenerator::loadAndLinkRuntimeIR(void) {

    std::string errMsg;
    Module* irMod = pGenSupport::getRuntimeIR();

    // Linker will take ownership of irMod

    // here we link the irRuntime.ir with our clean module so it already includes
    // definitions of types and functions we need for code generation
    Linker l(llvmModule_->getModuleIdentifier()+"_link", llvmModule_);
    l.LinkInModule(irMod, &errMsg);
    if (errMsg.length()) {
        throw pCompileError("error linking in runtime IR [" + errMsg + "]");
    }

    // take ownership of module so it's not freed
    l.releaseModule();

}

void pGenerator::createEntryPoint(void) {

    // script top level initialization function
    entryFunctionName_ = pGenSupport::mangleModuleName(llvmModule_->getModuleIdentifier());

    // entry function
    Function *initFun = Function::Create(IRHelper_.moduleEntryFunType(),
                                         Function::ExternalLinkage,
                                         entryFunctionName_,
                                         llvmModule_);

    initFun->arg_begin()->setName("rEngine");

    // entry block
    //IRBuilder<> block;
    //block.SetInsertPoint(BasicBlock::Create("entry", initFun));

    // TODO module initialization: register classes, functions
    

}


} } // namespace

