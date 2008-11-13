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
#include <string>

#include <llvm/Module.h>
#include <llvm/System/DynamicLibrary.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/ModuleProvider.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/GenericValue.h>

#include "pDriver.h"
#include "pLexer.h"
#include "pParser.h"
#include "pModule.h"

#include "pRuntime.h"

using namespace std;

namespace rphp {

// execute the llvm Module contained in the given pModule
// ir should already have been generated
bool pDriver::executeModule(pModule* pMod) {

    llvm::Module* M = pMod->getLLVMModule();
    assert(M != NULL);

    // create entry stub which creates a runtime instance
    llvm::ExistingModuleProvider* MP = new llvm::ExistingModuleProvider(M);

    string errMsg;
    llvm::ExecutionEngine* EE = llvm::ExecutionEngine::createJIT(MP, &errMsg);
    if (!EE) {
        cerr << errMsg << endl;
        return false;
    }

    // EE now owns MP and M, so tell pModule not to delete
    pMod->setLLVMModuleOwnership(false);

    llvm::Function* main = MP->getModule()->getFunction(pMod->getEntryFunctionName());
    if (!main) {
        cerr << "error: entry symbol not found: " << pMod->getEntryFunctionName() << endl;
        delete EE;
        return false;
    }

    // JIT magic
    pRuntimeEngine* r = new pRuntimeEngine();
    void *mainPtr = EE->getPointerToFunction(main);
    // cast to entry function type (returns void, takes one parameter of runtime engine instance)
    void (*mainFunc)(pRuntimeEngine*) = (void (*)(pRuntimeEngine*))mainPtr;
    mainFunc(r);

    delete r;
    delete EE;

    return true;

}

// execute the given precompiled bitcode file, by calling the main
// function and passing it a runtime instance
bool pDriver::executeBC(string fileName) {

    string errMsg;
    llvm::MemoryBuffer* mb = llvm::MemoryBuffer::getFile(fileName.c_str(), &errMsg);
    if (errMsg.length()) {
        cerr << "error loading file [" << fileName << "]: " << errMsg << endl;
        return false;
    }
    llvm::ModuleProvider* MP = llvm::getBitcodeModuleProvider(mb, &errMsg);
    if (errMsg.length()) {
        cerr << "error parsing bitcode file [" << fileName << "]: " << errMsg << endl;
        return false;
    }

    // MP now owns mb and will delete
    return JITmodule(MP, "main");

}

// run an llvm Module in the JIT
// takes ownership of ModuleProvider
// will call the given entryFunction with no arguments
bool pDriver::JITmodule(llvm::ModuleProvider* MP, std::string entryFunction) {

    string errMsg;
    if (llvm::sys::DynamicLibrary::LoadLibraryPermanently("librphp-runtime.so", &errMsg)) {
        cerr << "error loading runtime library: " << errMsg << endl;
        delete MP;
        return false;
    }

    llvm::ExecutionEngine* EE = llvm::ExecutionEngine::createJIT(MP, &errMsg);
    if (!EE) {
        cerr << errMsg << endl;
        return false;
    }

    // EE now owns MP
    llvm::Function* rphpMain = MP->getModule()->getFunction(entryFunction);
    if (!rphpMain) {
        cerr << "error: entry symbol not found: " << entryFunction << endl;
        MP->getModule()->dump();
        delete EE;
        return false;
    }

    EE->runStaticConstructorsDestructors(false);

    // Call entry function with no arguments:
    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue gv = EE->runFunction(rphpMain, noargs);

    EE->runStaticConstructorsDestructors(true);

    delete EE;
    return true;

}

// lex and then dump tokens from the given source file
void pDriver::dumpTokens(string fileName) {

    lexer::pLexer l(fileName);
    l.dumpTokens();

}

// preprocess the source file, dump to stdout
void pDriver::dumpPre(string fileName) {

    lexer::pLexer l(fileName);
    bool success = l.preprocess();
    std::cout << l.contents();
    if (!success) {
        std::cout << "preprocess failed due to lex error" << std::endl;
    }

}

// dump the parse tree from the given source file
void pDriver::dumpAST(string fileName) {

    pModule m(fileName);
    m.dumpAST();

}

// dump the generated IR code from the given source file
void pDriver::dumpIR(string fileName) {

    pModule m(fileName);
    m.dumpIR();

}

}

