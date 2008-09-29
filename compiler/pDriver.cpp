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
//
#include <fstream>
#include <iostream>
#include <string>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <boost/algorithm/string.hpp>

// JIT, bitcode
#include "llvm/Module.h"
#include "llvm/System/DynamicLibrary.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/ModuleProvider.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/ModuleProvider.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/GenericValue.h"

#include "pLexer.h"
#include "pParser.h"
#include "pDriver.h"
#include "pModule.h"

#include "pRuntime.h"

using namespace std;

namespace rphp {


/**
 * execute given file -- php source or bytecode
 */
void pDriver::execute(string fileName) {
    //cout << "executing file: " << fileName << endl;
    // TODO: determine file type, then run executeBC or executePHP
    executeBC(fileName);
}

void pDriver::executeModule(pModule* mod) {

    llvm::Module* M = mod->getLLVMModule();
    assert(M != NULL);

    // create entry stub which creates a runtime instance

    llvm::ExistingModuleProvider* MP = new llvm::ExistingModuleProvider(M);
    //JITmodule(MP, mod->getEntryFunctionName());

    string errMsg;
    llvm::ExecutionEngine* EE = llvm::ExecutionEngine::createJIT(MP, &errMsg);
    if (!EE) {
        cerr << errMsg << endl;
        return;
    }

    // EE now owns MP

    llvm::Function* main = MP->getModule()->getFunction(mod->getEntryFunctionName());
    if (!main) {
        cerr << "error: entry symbol not found: " << mod->getEntryFunctionName() << endl;
        MP->getModule()->dump();
        delete EE;
        return;
    }

    // JIT magic
    pRuntimeEngine* r = new pRuntimeEngine();
    void *mainPtr = EE->getPointerToFunction(main);
    // cast to entry function type (returns void, takes one parameter of runtime engine instance)
    void (*mainFunc)(pRuntimeEngine*) = (void (*)(pRuntimeEngine*))mainPtr;
    mainFunc(r);
    delete r;

    delete EE;

}

/**
 * execute precompiled bytecode
 */
void pDriver::executeBC(string fileName) {

    // Now we create the JIT.
    string errMsg;
    llvm::MemoryBuffer* mb = llvm::MemoryBuffer::getFile(fileName.c_str(), &errMsg);
    if (errMsg.length()) {
        cerr << "error loading file [" << fileName << "]: " << errMsg << endl;
        return;
    }
    llvm::ModuleProvider* MP = llvm::getBitcodeModuleProvider(mb, &errMsg);
    if (errMsg.length()) {
        cerr << "error parsing bitcode file [" << fileName << "]: " << errMsg << endl;
        return;
    }

    // MP now owns mb and will delete
    JITmodule(MP, "main");

}

void pDriver::JITmodule(llvm::ModuleProvider* MP, std::string entryFunction) {

    string errMsg;
    if (llvm::sys::DynamicLibrary::LoadLibraryPermanently("librphp-runtime.so", &errMsg)) {
        cerr << "error loading runtime library: " << errMsg << endl;
        return;
    }

    //MP->getModule()->setDataLayout("e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128");
    //MP->getModule()->setTargetTriple("x86_64-pc-linux-gnu");

    //llvm::ExecutionEngine* EE = llvm::ExecutionEngine::create(MP, false);
    llvm::ExecutionEngine* EE = llvm::ExecutionEngine::createJIT(MP, &errMsg);
    if (!EE) {
        cerr << errMsg << endl;
        return;
    }

    // EE now owns MP

    llvm::Function* rphpMain = MP->getModule()->getFunction(entryFunction);
    if (!rphpMain) {
        cerr << "error: entry symbol not found: " << entryFunction << endl;
        MP->getModule()->dump();
        delete EE;
        return;
    }

    EE->runStaticConstructorsDestructors(false);

    // Call the rphp_main function with no arguments:
    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue gv = EE->runFunction(rphpMain, noargs);

    EE->runStaticConstructorsDestructors(true);

    delete EE;

}

/**
 * compile, then execute PHP source
 */
void pDriver::executePHP(string fileName) {
    cout << "compile and execute php file: " << fileName << endl;
}


/**
 * compile to llvm assembly
 */
// void pDriver::compileToAsm(string fileName) {
//     cout << "compiling " << fileName << " to llvm assembly " << endl;
// }

/**
 * compile to native binary
 */
// void pDriver::compileToNative(string fileName) {
//     cout << "compiling " << fileName << " to native binary " << endl;
// }

/**
 *  compile the given file to an pModule, which the caller then owns and must delete
 */
pModule* pDriver::createModule(string fileName) {

    return parser::parseSourceFile(fileName);

}


void pDriver::dumpTokens(string fileName) {

    lexer::pLexer l(fileName);
    l.dumpTokens();

}

void pDriver::dumpAST(string fileName) {

    pModule* m = parser::parseSourceFile(fileName);
    m->dumpAST();
    delete m;

}


}


