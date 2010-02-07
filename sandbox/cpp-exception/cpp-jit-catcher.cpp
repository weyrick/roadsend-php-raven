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
#include <llvm/CallingConv.h>
#include <llvm/Function.h>
#include <llvm/Module.h>
#include <llvm/PassManager.h>
#include <llvm/System/DynamicLibrary.h>
#include <llvm/ModuleProvider.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Support/ManagedStatic.h>

#include <iostream>
#include <string>
#include <stdexcept>

using namespace llvm;


Function* createCallFunc(Module* mod)
{
	// Create the function which we JIT and call.
	Constant* c = mod->getOrInsertFunction("cppe-func", Type::getVoidTy(getGlobalContext()), NULL);
	Function* func = cast<Function>(c);

	// Get the type of the function in the runtime, mockError();
	std::vector<const Type*>RuntimeFuncArgs;
	FunctionType* RuntimeFuncType = FunctionType::get(
			/*Result=*/Type::getVoidTy(getGlobalContext()),
			/*Params=*/RuntimeFuncArgs,
			/*isVarArg=*/false);

	// Get the type for the pointer to it.
	PointerType* RuntimeFuncTypePtr = PointerType::get(RuntimeFuncType, 0);

	// Get a constant for the address of the function in the runtime, this is currently x86_64 specific!!
	//ConstantInt* addressConstant = ConstantInt::get(getGlobalContext(), APInt(64, (uint64_t)sys::DynamicLibrary::SearchForAddressOfSymbol("mockError")));
	// Cast the address constant to a funcptr
	//Constant* funcAddress = ConstantExpr::getCast(Instruction::IntToPtr, addressConstant, RuntimeFuncTypePtr);

	Constant* c2 = mod->getOrInsertFunction("mockError", Type::getVoidTy(getGlobalContext()), NULL);
	Function* errFunc = cast<Function>(c2);

	// IR setup
	BasicBlock* entry = BasicBlock::Create(getGlobalContext(), "entry", func);
	IRBuilder<> builder(entry);

	// Call the runtime func.
	builder.CreateCall(errFunc);

	// Terminator instruction
	builder.CreateRetVoid();
	return func;
}

int main()
{

    ExecutionEngine *executionEngine;

	InitializeNativeTarget();
	llvm::DwarfExceptionHandling = true;
	llvm::UnwindTablesMandatory = true;
#if (LLVM_VERSION >= 2007000)
        llvm::JITEmitDebugInfo = true;
#endif

	// Load the cpp mock runtime
	/*
	std::string lib = std::string("./libcpp-lib")+std::string(LTDL_SHLIB_EXT);
	if (sys::DynamicLibrary::LoadLibraryPermanently(lib.data())) {
	  std::cout << "unable to find libcpp-lib\n";
	  return 1;
	}
	*/

	Module* mod = new Module("cppe-mod", getGlobalContext());
	Function* execFunc = createCallFunc(mod);
	verifyModule(*mod, PrintMessageAction);
	mod->dump();

	executionEngine = EngineBuilder(mod).create();
	if (!executionEngine) {
	  std::cout << "JIT CREATION FAILED\n";
	  return 1;
	}

	executionEngine->runStaticConstructorsDestructors(false);

	// JIT and exec the generated function-
	void* funcPtr = executionEngine->getPointerToFunction(execFunc);
	if (!funcPtr) {
	  std::cout << "unable to find entry function\n";
	  return 1;
	}

	// voodoo
	void (*func)() = (void (*)())funcPtr;
	
	try {
	  func();
	}
	catch(std::runtime_error& e)
	{
	  std::cerr << "exception caught: " << e.what() <<std::endl;
	}
	executionEngine->runStaticConstructorsDestructors(true);

	llvm_shutdown();
	return 0;
}

