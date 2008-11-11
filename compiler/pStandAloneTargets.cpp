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

#include <iostream>
#include <vector>
#include <string>

#include <llvm/Module.h>
#include <llvm/GlobalVariable.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Constants.h>
#include <llvm/Instructions.h>

#include <llvm/Support/SystemUtils.h>
#include <llvm/System/Program.h>

#include "pGenSupport.h"
#include "pStandAloneTargets.h"

using namespace llvm;

namespace rphp {

Module* pStandAloneTarget::createStubModule(void) {

     Module *M = new Module(outputFile_);

     FunctionType *FT = FunctionType::get(Type::VoidTy, std::vector<const Type*>(),
                                         /*not vararg*/false);

     Function *F = Function::Create(FT, Function::ExternalLinkage, "main", M);

     BasicBlock *BB = BasicBlock::Create("EntryBlock", F);

     // ** STARTUP **
     // startup function type
     FunctionType *runtimeStartupFuncType = FunctionType::get(IRHelper_->runtimeEngineType(), std::vector<const Type*>(), false);
     // startup function
     Function *runtimeStartupFunc = Function::Create(runtimeStartupFuncType,
                                                     Function::ExternalLinkage,
                                                     "rphp_newRuntimeEngine",
                                                     M);
     // startup instruction call
     Instruction *runtimeStartInstr = CallInst::Create(runtimeStartupFunc, "runtime");

     // ** entry function call **
     std::vector<const Type*> printSig;
     printSig.push_back(IRHelper_->runtimeEngineType());
     Function *entryFunc = Function::Create(IRHelper_->moduleEntryFunType(),
                                            Function::ExternalLinkage,
                                            pGenSupport::mangleModuleName(mainFile_),
                                            M);
     std::vector<Value*> entryArgs;
     entryArgs.push_back(runtimeStartInstr);
     Instruction* entryInstr = CallInst::Create(entryFunc, entryArgs.begin(), entryArgs.end());


     //  ** SHUTDOWN **
     // argument sig for shutdown function
     std::vector<const Type*> engineSig(1, IRHelper_->runtimeEngineType());
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
    // TODO: outfile nameing
    pGenSupport::writeBitcode(M, outputFile_+".bc");
    delete M;

    // the following is based on code from llvm/tools/llvm-ld.cpp

    // link to native using llvm-ld
    sys::Path ld = sys::Program::FindProgramByName("llvm-ld");
    if (ld.isEmpty()) {
        // TODO: error handling
        std::cerr << "unable to link: llvm-ld not found" << std::endl;
        return;
    }

    std::vector<std::string> args;
    args.push_back("llvm-ld");
    for (std::vector<std::string>::iterator i = libSearchPaths_.begin(); i != libSearchPaths_.end(); ++i) {
        args.push_back("-L"+(*i));
    }
    args.push_back("-native");
    // TODO: opt flags -- full debug for now
    args.push_back("-disable-opt");
    args.push_back("-verify-each");
    args.push_back("-v");
    //
    args.push_back("-lrphp-runtime");
    args.push_back("-o");
    args.push_back(outputFile_);
    args.push_back(outputFile_+".bc");
    for (std::vector<std::string>::iterator i = inputFiles_.begin(); i != inputFiles_.end(); ++i) {
        args.push_back(*i);
    }

    std::vector<const char *> Args;
    for (unsigned i = 0, e = args.size(); i != e; ++i)
        Args.push_back(args[i].c_str());
    Args.push_back(0);

    std::vector<const char*>::const_iterator I = Args.begin(), E = Args.end();
    for (; I != E; ++I)
        if (*I)
        std::cout << "'" << *I << "'" << " ";
    std::cout << "\n" << std::flush;

    std::string errMsg;
    int R = sys::Program::ExecuteAndWait(
        ld, &Args[0], 0, 0, 0, 0, &errMsg);

    if (R != 0) {
        // TODO: error handling
        std::cerr << errMsg << std::endl;
        return;
    }

}

} // namespace

