/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009 Shannon Weyrick <weyrick@roadsend.com>
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
#include <llvm/DerivedTypes.h>
#include <llvm/Constants.h>
#include <llvm/Instructions.h>
#include <llvm/Support/IRBuilder.h>

#include "rphp/IR/pCompileError.h"
#include "rphp/IR/pDeclare.h"
#include "rphp/IR/pGenSupport.h"

using namespace llvm;

namespace rphp { namespace IR {


void pDeclare::visit_functionDecl(AST::functionDecl* n) {

    // TODO declare pVar parameters as required in functionDecl
    
    // entry function
    Function *userFun = Function::Create(IRHelper_.pUserFunction0(),
                                        Function::ExternalLinkage,
                                        pGenSupport::mangleUserFunctionName(llvmModule_->getModuleIdentifier(),
                                                                            n->functionDef()->name()),
                                        llvmModule_);

    Function::arg_iterator a = userFun->arg_begin();
    (*a).setName("funRetVal");
    Value* runtime = ++a;
    runtime->setName("rEngine");

    // entry block
    BasicBlock::Create("entry", userFun);

    // MODULE INITIALIZATION 
    
    // entry block
    IRBuilder<> block;
    block.SetInsertPoint(&initFunction_->getEntryBlock());

    Function* registerFun;
    switch (n->functionDef()->maxArity()) {
        case 0:
            registerFun = llvmModule_->getFunction("rphp_registerUserFun0");
            break;
    }
    
    assert(registerFun != NULL && "function declare doesn't support requested arity");
    
    int len;
    block.CreateCall3(registerFun, 
                      &(*initFunction_->arg_begin())/* runtime */, 
                      IRHelper_.stringConstant(n->functionDef()->name(),len), 
                      userFun);
    
}

} } // namespace

