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

#include "pGenSupport.h"
#include "pStandAloneTargets.h"

using namespace llvm;

namespace rphp {

Module* pStandAloneTarget::createStubModule(void) {

     Module *M = new Module(outputFile);

     FunctionType *FT = FunctionType::get(Type::VoidTy, std::vector<const Type*>(),
                                         /*not vararg*/false);

     Function *F = Function::Create(FT, Function::ExternalLinkage, "main", M);

     BasicBlock *BB = BasicBlock::Create("EntryBlock", F);

     // ** STARTUP **
     // startup function type
     FunctionType *runtimeStartupFuncType = FunctionType::get(IRTypes.runtimeEngineType(), std::vector<const Type*>(), false);
     // startup function
     Function *runtimeStartupFunc = Function::Create(runtimeStartupFuncType,
                                                     Function::ExternalLinkage,
                                                     "rphp_newRuntimeEngine",
                                                     M);
     // startup instruction call
     Instruction *runtimeStartInstr = CallInst::Create(runtimeStartupFunc, "runtime");

     // ** entry function call **
     std::vector<const Type*> printSig;
     printSig.push_back(IRTypes.runtimeEngineType());
     Function *entryFunc = Function::Create(IRTypes.moduleEntryFunType(),
                                            Function::ExternalLinkage,
                                            pGenSupport::mangleModuleName(mainFile),
                                            M);
     std::vector<Value*> entryArgs;
     entryArgs.push_back(runtimeStartInstr);
     Instruction* entryInstr = CallInst::Create(entryFunc, entryArgs.begin(), entryArgs.end());


     //  ** SHUTDOWN **
     // argument sig for shutdown function
     std::vector<const Type*> engineSig(1, IRTypes.runtimeEngineType());
     // shutdown function type
     FunctionType *runtimeDeleteFuncType = FunctionType::get(Type::VoidTy, engineSig, false);
     // shutdown function
     Function *runtimeDeleteFunc = Function::Create(runtimeDeleteFuncType,
                                                    Function::ExternalLinkage,
                                                    "rphp_deleteRuntimeEngine",
                                                    M);

     std::vector<Value*> shutdownArgsV;
     shutdownArgsV.push_back(runtimeStartInstr);

     // shutdown instruction call
     Instruction *runtimeDeleteInstr = CallInst::Create(runtimeDeleteFunc, shutdownArgsV.begin(), shutdownArgsV.end());

     BB->getInstList().push_back(runtimeStartInstr);
     BB->getInstList().push_back(entryInstr);
     BB->getInstList().push_back(runtimeDeleteInstr);
     BB->getInstList().push_back(ReturnInst::Create());

     /*
     if (verifyModule(*M, PrintMessageAction)) {
         cerr << "module corrupt" << endl;
         exit(-1);
     }
     else {
         cerr << "module verified" << endl;
     }
     */

     return M;

}

void pStandAloneTarget::execute(void) {

    Module* M = createStubModule();
    pGenSupport::writeBitcode(M, outputFile);
    delete M;

}

} // namespace

