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

#include "pCompileTarget.h"
#include "pGenerator.h"

using namespace llvm;

namespace rphp {

void pGenerator::createEntryFunctionName(const std::string& inName) {

    // TODO: this needs to set an appropriate name based on file name
    // and project root (from target)
    entryFunctionName = inName;

}

void pGenerator::getTypes(void) {

    // runtime engine pointer
    rEngineType = PointerType::get(Type::Int8Ty,0);

}

void pGenerator::createEntryPoint(void) {

    // script top level initialization function
    createEntryFunctionName(llvmModule->getModuleIdentifier());

    // entry function takes pointer to runtime instance
    std::vector<const Type*> efArgs;
    efArgs.push_back(rEngineType);

    // entry function type: void (*)(void)
    FunctionType *initFunType = FunctionType::get(Type::VoidTy, /* return type */
                                                  efArgs, /* arguments */
                                                  false /*not vararg*/);

    // entry function
    Function *initFun = Function::Create(initFunType, Function::ExternalLinkage, entryFunctionName, llvmModule);

    runtimeEngine = initFun->arg_begin();
    runtimeEngine->setName("rEngine");

    // entry block
    currentBlock.SetInsertPoint(BasicBlock::Create("entry", initFun));

}

void pGenerator::finalize(void) {
    // terminate entry function
    Function *initFun = llvmModule->getFunction(entryFunctionName);
    initFun->getEntryBlock().getInstList().push_back(ReturnInst::Create());
}

void pGenerator::visit(AST::treeTop* n) {
    AST::defaultVisitor::visit(n);
}

void pGenerator::visit(AST::statementNode* n) {
    AST::defaultVisitor::visit(n);
}

void pGenerator::visit(AST::echoNode* n) {

    ArrayType* ArrayTy_0 = ArrayType::get(IntegerType::get(8), 12);
    PointerType* PointerTy_4 = PointerType::get(IntegerType::get(8), 0);
    GlobalVariable* gvar_array__str = new GlobalVariable(
    /*Type=*/ArrayTy_0,
    /*isConstant=*/true,
    /*Linkage=*/GlobalValue::InternalLinkage,
    /*Initializer=*/0, // has initializer, specified below
    /*Name=*/".str",
    llvmModule);

    // Constant Definitions
    Constant* const_array_7 = ConstantArray::get("hello world", true);
    std::vector<Constant*> const_ptr_8_indices;
    Constant* const_int32_9 = Constant::getNullValue(IntegerType::get(32));
    const_ptr_8_indices.push_back(const_int32_9);
    const_ptr_8_indices.push_back(const_int32_9);
    Constant* const_ptr_8 = ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_8_indices[0], const_ptr_8_indices.size() );
    UndefValue* const_int32_10 = UndefValue::get(IntegerType::get(32));

    // Global Variable Definitions
    gvar_array__str->setInitializer(const_array_7);

    // argument sig for print function
    std::vector<const Type*> printSig;
    printSig.push_back(rEngineType);
    printSig.push_back(PointerTy_4);

    // print function type
    FunctionType *printFuncType = FunctionType::get(Type::VoidTy, printSig, false);

    // print function
    Constant *printFunc = llvmModule->getOrInsertFunction("rphp_print_cstr", printFuncType);

    currentBlock.CreateCall2(printFunc, runtimeEngine, const_ptr_8);


}

} // namespace

