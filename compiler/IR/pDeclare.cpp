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

#include "rphp/IR/pCompileError.h"
#include "rphp/IR/pDeclare.h"
#include "rphp/IR/pGenSupport.h"

using namespace llvm;

namespace rphp { namespace IR {


void pDeclare::visit_functionDecl(AST::functionDecl* n) {

    // TODO declare pVar parameters as required in functionDecl
    
    // entry function
    Function *fun = Function::Create(IRHelper_.moduleEntryFunType(),
                                     Function::ExternalLinkage,
                                     pGenSupport::mangleFunctionName(llvmModule_->getModuleIdentifier(),
                                                                     n->functionDef()->name()),
                                     llvmModule_);

    fun->arg_begin()->setName("rEngine");

    // entry block
    //BasicBlock::Create("entry", fun);
    
    // TODO exit block?

}


} } // namespace

