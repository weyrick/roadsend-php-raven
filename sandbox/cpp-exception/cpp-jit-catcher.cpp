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


#include <iostream>
#include <string>

using namespace llvm;

ExecutionEngine *executionEngine;


// copy & paste by the llvm JIT tutorial.

Module* makeLLVMModule() {
    llvm::ExceptionHandling = true;

	// Module Construction
	Module* mod = new Module("cppe-mod");

	return mod;
}

Function* createCallFunc(Module* mod)
{
	// Create the function which we JIT and call.
	Constant* c = mod->getOrInsertFunction("cppe-func", Type::VoidTy, NULL);
	Function* func = cast<Function>(c);

	// Get the type of the function in the runtime, mockError();
	std::vector<const Type*>RuntimeFuncArgs;
	FunctionType* RuntimeFuncType = FunctionType::get(
			/*Result=*/Type::VoidTy,
			/*Params=*/RuntimeFuncArgs,
			/*isVarArg=*/false);

	// Get the type for the pointer to it.
	PointerType* RuntimeFuncTypePtr = PointerType::get(RuntimeFuncType, 0);

	// Get a constant for the address of the function in the runtime, this is currently x86_64 specific!!
	ConstantInt* addressConstant = ConstantInt::get(APInt(64, (uint64_t)sys::DynamicLibrary::SearchForAddressOfSymbol("mockError")));
	// Cast the address constant to a funcptr
	Constant* funcAddress = ConstantExpr::getCast(Instruction::IntToPtr, addressConstant, RuntimeFuncTypePtr);

	// IR setup
	BasicBlock* entry = BasicBlock::Create("entry", func);
	IRBuilder<> builder(entry);

	// Call the runtime func.
	builder.CreateCall(funcAddress);

	// Terminator instruction
	builder.CreateRetVoid();
	return func;
}

int main()
{
	// Load the cpp mock runtime
	sys::DynamicLibrary::LoadLibraryPermanently("./libcpp-lib.so");

	Module* mod = makeLLVMModule();
	executionEngine = ExecutionEngine::create(mod);
	Function* execFunc = createCallFunc(mod);

	executionEngine->runStaticConstructorsDestructors(false);
	verifyModule(*mod, PrintMessageAction);

	// JIT and exec the generated function-
	void* funcPtr = executionEngine->getPointerToFunction(execFunc);
	void (*func)() = (void (*)())funcPtr;
	
	try {
	func();
	}
	catch(...)
	{
		std::cerr<<"exception catched"<<std::endl;
	}
	executionEngine->runStaticConstructorsDestructors(true);

	return 0;
}
