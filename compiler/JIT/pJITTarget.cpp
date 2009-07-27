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

#include "rphp/JIT/pJITTarget.h"

#include "rphp/runtime/pRuntime.h"

#include <llvm/Module.h>
#include <llvm/System/DynamicLibrary.h>
#include <llvm/ModuleProvider.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>

#include <iostream>
#include <string>

using namespace llvm;

namespace rphp {

/*
bool pJIT::executeMain(llvm::Module* M) {

//M->dump();
    ExistingModuleProvider* MP = new ExistingModuleProvider(M);

    std::string errMsg;
    ExecutionEngine* EE = ExecutionEngine::createJIT(MP, &errMsg);
    if (!EE) {
        std::cerr << errMsg << std::endl;
        return false;
    }

    Function* main = MP->getModule()->getFunction("main");
    if (!main) {
        std::cerr << "error: 'main' entry symbol not found" << std::endl;
        delete EE;
        return false;
    }

    EE->runStaticConstructorsDestructors(false);

    std::vector<GenericValue> noargs;
    EE->runFunction(main, noargs);

    EE->runStaticConstructorsDestructors(true);

    delete EE;

    return true;

}
*/

//bool pJIT::executeWithRuntime(Module* M, std::string entryFunction) {
void pJITTarget::execute(void) {

    // we need exception handling for our c++ runtime to work properly
    // therefore we tell llvm JIT to generate the proper dwarf tables
    llvm::ExceptionHandling = true;

    ExistingModuleProvider* MP = new ExistingModuleProvider(llvmModule_);

    std::string errMsg;
    ExecutionEngine* EE = ExecutionEngine::createJIT(MP, &errMsg);
    if (!EE) {
        std::cerr << errMsg << std::endl;
        return;// false;
    }

    // EE now owns MP and M
    Function* main = MP->getModule()->getFunction(entryFunction_);

    if (!main) {
        std::cerr << "error: entry symbol not found: " << entryFunction_ << std::endl;
        delete EE;
        return;// false;
    }

    EE->runStaticConstructorsDestructors(false);

    // JIT magic
    //pRuntimeEngine* r = new pRuntimeEngine();
    void *mainPtr = EE->getPointerToFunction(main);
//    std::cerr << "found main function at: " << mainPtr << std::endl;
    // cast to entry function type (returns void, takes one parameter of runtime engine instance)
    // see pGenerator::createEntryPoint, pIRHelper::moduleEntryFunType
    void (*mainFunc)(pVar*,pRuntimeEngine*) = (void (*)(pVar*,pRuntimeEngine*))mainPtr;
    pVar retVal;
    mainFunc(&retVal, runtime_);

    EE->runStaticConstructorsDestructors(true);

    // FIXME: leak?
    //delete EE;
    //delete r;

    //return true;

}


}

