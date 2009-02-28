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

#include <llvm/Module.h>
#include <llvm/ModuleProvider.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <llvm/ValueSymbolTable.h>

#include <iostream>
#include <fstream>

#include "rphp/IR/pIRHelper.h"
#include "rphp/IR/pCompileError.h"
#include "rphp/IR/pGenSupport.h"

using namespace llvm;

namespace rphp { namespace IR {

std::string pGenSupport::mangleModuleName(std::string inName) {

    // TODO: mangle
    return inName;

}

std::string pGenSupport::mangleFunctionName(std::string moduleName, std::string inName) {

    // TODO: mangle
    return moduleName+"_"+inName;

}

void pGenSupport::writeBitcode(Module* m, std::string outFile) {

    assert(m != NULL);
    assert(outFile.length() > 0);

    std::ofstream OS(outFile.c_str(), std::ios_base::out|std::ios::trunc|std::ios::binary);
    if (!OS.fail()) {
        WriteBitcodeToFile(m, OS);
    }
    else {
        throw pCompileError("unable to write bitcode file ["+outFile+"]");
    }

}


Module* pGenSupport::readBitcode(std::string fileName) {

    std::string errMsg;
    MemoryBuffer* mb = MemoryBuffer::getFile(fileName.c_str(), &errMsg);
    if (errMsg.length()) {
        throw pCompileError("error loading runtime IR file [" + fileName + "]: " + errMsg);
    }

    ModuleProvider* mp = getBitcodeModuleProvider(mb, &errMsg);
    if (errMsg.length()) {
        throw pCompileError("error parsing bitcode file [" + fileName + "]: " + errMsg);
    }

    Module* mod =  mp->getModule();

    // caller takes control of module
    mp->releaseModule();
    delete mp;

    return mod;

}

Module* pGenSupport::getRuntimeIR() {

    // TODO: needs to be a command line option with a good default
    return readBitcode("../lib/c-runtime.ir");

}


Module* pGenSupport::createStandAloneStubModule(const std::string& stubName, const std::string& mainModuleName) {

    pIRHelper irHelper(getRuntimeIR());
    Module *M = new Module(stubName);

    FunctionType *FT = FunctionType::get(Type::VoidTy, std::vector<const Type*>(),
                                        /*not vararg*/false);

    Function *F = Function::Create(FT, Function::ExternalLinkage, "main", M);

    BasicBlock *BB = BasicBlock::Create("EntryBlock", F);

    // ** STARTUP **
    // startup function type
    FunctionType *runtimeStartupFuncType = FunctionType::get(irHelper.runtimeEngineType(), std::vector<const Type*>(), false);
    // startup function
    Function *runtimeStartupFunc = Function::Create(runtimeStartupFuncType,
                                                    Function::ExternalLinkage,
                                                    "rphp_newRuntimeEngine",
                                                    M);
    // startup instruction call
    Instruction *runtimeStartInstr = CallInst::Create(runtimeStartupFunc, "runtime");

    // ** entry function call **
    std::vector<const Type*> printSig;
    printSig.push_back(irHelper.runtimeEngineType());
    Function *entryFunc = Function::Create(irHelper.moduleEntryFunType(),
                                        Function::ExternalLinkage,
                                        mangleModuleName(mainModuleName),
                                        M);
    std::vector<Value*> entryArgs;
    entryArgs.push_back(runtimeStartInstr);
    Instruction* entryInstr = CallInst::Create(entryFunc, entryArgs.begin(), entryArgs.end());


    //  ** SHUTDOWN **
    // argument sig for shutdown function
    std::vector<const Type*> engineSig(1, irHelper.runtimeEngineType());
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

void pGenSupport::dumpIR(Module* llvmModule) {

    // dump all generated symbols (globals and functions). this skips all of the
    // imported runtime jazz
    const ValueSymbolTable& sTable = llvmModule->getValueSymbolTable();

    std::string name;
    for(ValueSymbolTable::const_iterator s = sTable.begin(); s != sTable.end(); ++s) {
        name.assign(s->getKeyData());
        if (// functions start with the module identifier name (mangled script file name)
            name.substr(0, llvmModule->getModuleIdentifier().length()) == llvmModule->getModuleIdentifier() ||
            // global literal strings
            name.substr(0, 5) == ".bstr" ||
            name.substr(0, 5) == ".ustr"
        ) {
            s->getValue()->dump();
        }
    }

}


} } // namespace

