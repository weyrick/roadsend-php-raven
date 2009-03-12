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
#include <llvm/ModuleProvider.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <llvm/ValueSymbolTable.h>
#include <llvm/Instructions.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/System/Path.h>

#include <iostream>
#include <fstream>

// getenv
#include <stdlib.h>

#include "rphp/IR/pIRHelper.h"
#include "rphp/IR/pCompileError.h"
#include "rphp/IR/pGenSupport.h"

using namespace llvm;

namespace rphp { namespace IR {

std::string pGenSupport::mangleEntryFunctionName(std::string inName) {

    return inName;

}

std::string pGenSupport::mangleUserFunctionName(std::string moduleName, std::string inName) {

    return moduleName+":"+inName;

}

std::string pGenSupport::mangleInitFunctionName(std::string moduleName) {

    return moduleName+":^init";

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

    sys::Path irFile;
    
    char* libPath = getenv("RPHP_IR_PATH");
    if (libPath) {
        irFile.set(libPath);
    }
    else {
        // assume build dir
        irFile.set("../lib");
    }
    irFile.appendComponent("c-runtime.ir");

    if (irFile.exists()) {
        return readBitcode(irFile.toString());
    }
    else {
        throw pCompileError("unable to find c-runtime.ir - please set RPHP_IR_PATH environment variable to point to the directory containing this file.");
    }
    

}


void pGenSupport::createMain(Module *llvmModule, const pIdentString& entryFunctionName) {

    pIRHelper irHelper(llvmModule);
    
    // void main(void)
    FunctionType *FT = FunctionType::get(Type::VoidTy, std::vector<const Type*>(),
                                        /*not vararg*/false);
    Function *main = Function::Create(FT, Function::ExternalLinkage, "main", llvmModule);
    IRBuilder<> block;
    block.SetInsertPoint(BasicBlock::Create("entry", main));
    
    // alloc return value
    AllocaInst* pVarTmp = block.CreateAlloca(irHelper.pVarType(), 0, "retVal");

    // construct return value
    Function* constructor = llvmModule->getFunction("_ZN4rphp4pVarC1Ev");
    assert(constructor != NULL);
    block.CreateCall(constructor, pVarTmp);

    // create runtime
    Function* newRuntime = llvmModule->getFunction("rphp_newRuntimeEngine");
    assert(newRuntime != NULL);
    Value* runtime = block.CreateCall(newRuntime, "runtime");
    
    // call entry function
    Function* entry = llvmModule->getFunction(entryFunctionName);
    assert(entry != NULL);
    block.CreateCall2(entry, pVarTmp, runtime);
    
    // close runtime
    Function* deleteRuntime = llvmModule->getFunction("rphp_deleteRuntimeEngine");
    assert(deleteRuntime != NULL);
    block.CreateCall(deleteRuntime, runtime);
    
    // destruct return value
    Function* destructor = llvmModule->getFunction("_ZN4rphp4pVarD1Ev");
    assert(destructor != NULL);
    block.CreateCall(destructor, pVarTmp);

    // return
    block.CreateRetVoid();
    
    //dumpIR(llvmModule);

    return;

}

void pGenSupport::dumpIR(Module* llvmModule) {

    // dump all generated symbols (globals and functions). this skips all of the
    // imported runtime jazz
    const ValueSymbolTable& sTable = llvmModule->getValueSymbolTable();

    std::string name;
    for(ValueSymbolTable::const_iterator s = sTable.begin(); s != sTable.end(); ++s) {
        name.assign(s->getKeyData());
        if (// functions start with the module identifier name (mangled script file name)
            name == "main" || 
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

