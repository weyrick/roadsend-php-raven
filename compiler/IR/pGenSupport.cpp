/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2012 Shannon Weyrick <weyrick@mozek.us>
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

#include "rphp/IR/pGenSupport.h"

#include "rphp/IR/pIRHelper.h"
#include "rphp/IR/pCompileError.h"

#include <llvm/Module.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <llvm/ValueSymbolTable.h>
#include <llvm/Instructions.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/PathV2.h>
#include <llvm/Support/system_error.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>

#include <iostream>
#include <fstream>
#include <sstream>

// getenv
#include <stdlib.h>

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

    std::string ErrInfo;
    raw_fd_ostream OS(outFile.c_str(), ErrInfo, raw_fd_ostream::F_Binary);
    if (ErrInfo.empty()) {
        WriteBitcodeToFile(m, OS);
    }
    else {
        throw pCompileError("unable to write bitcode file ["+outFile+"]");
    }

}


Module* pGenSupport::readBitcode(std::string fileName, LLVMContext& context) {

    OwningPtr<MemoryBuffer> fileBuf;
    if (error_code ec = MemoryBuffer::getFile(fileName.c_str(), fileBuf)) {
        throw pCompileError("error loading runtime IR file [" + fileName + "]: " + ec.message());
    }

    std::string errMsg;
    Module *module = getLazyBitcodeModule(fileBuf.take(),
                                          getGlobalContext(),
                                          &errMsg);
    if (!module) {
        throw pCompileError("error parsing bitcode file [" + fileName + "]: " + errMsg);
    }

    return module;

}

Module* pGenSupport::getRuntimeIR(LLVMContext &c) {

    SmallString<256> irFile;

    char* libPath = getenv("RPHP_IR_PATH");
    if (libPath) {
        irFile = libPath;
    }
    else {
        // assume build dir
        irFile = "../lib";
    }
    sys::path::append(irFile, "c-runtime.bc");

    if (sys::fs::exists(irFile.str())) {
        return readBitcode(irFile.str(), c);
    }
    else {
        throw pCompileError("unable to find c-runtime.bc - please set RPHP_IR_PATH environment variable to point to the directory containing this file.");
    }


}


void pGenSupport::createMain(Module *llvmModule, const pIdentString& entryFunctionName) {

    pIRHelper irHelper(llvmModule);

    // int rphp_mainEntry(pVar*, pRuntimeEngine*)
    std::vector<const Type*>args;
    args.push_back(irHelper.pVarPointerType()); // retval
    args.push_back(irHelper.runtimeEngineType());
    FunctionType* FT = FunctionType::get(Type::getVoidTy(llvmModule->getContext()),
                                         /*isVarArg=*/false);

    // this matches an extern in c-runtime.cpp
    Function *main = (Function*)llvmModule->getOrInsertFunction("rphp_mainEntry", FT);

    Function::arg_iterator a = main->arg_begin();
    Value* retVal = a;
    (*a).setName("mainRetVal");
    Value* runtime = ++a;
    runtime->setName("rEngine");
    
    // always inline
    main->addFnAttr(Attribute::AlwaysInline);
    
    IRBuilder<> block(llvmModule->getContext());
    block.SetInsertPoint(BasicBlock::Create(llvmModule->getContext(), "entry", main));

    // call entry function
    Function* entry = llvmModule->getFunction(entryFunctionName);
    assert(entry != NULL);
    block.CreateCall2(entry, retVal, runtime);

    // return
    block.CreateRetVoid();

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
            //name == "main" ||
            name.substr(0, llvmModule->getModuleIdentifier().length()) == llvmModule->getModuleIdentifier() ||
            // byte arrays
            name.substr(0, 5) == ".barr"
        ) {
            s->getValue()->dump();
        }
    }

}


} } // namespace

