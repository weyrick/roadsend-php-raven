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

#include <llvm/Module.h>
#include <llvm/GlobalVariable.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Constants.h>
#include <llvm/Instructions.h>

#include <llvm/Linker.h>

#include "pGenerator.h"
#include "pCompileTarget.h"
#include "pIRHelper.h"
#include "pGenSupport.h"

using namespace llvm;

namespace rphp {

pGenerator::pGenerator(llvm::Module* m, pCompileTarget* t): llvmModule(m), target(t), IRHelper(0) {

    loadAndLinkRuntimeIR();
    createEntryPoint();

}
    
pGenerator::~pGenerator(void) {

    delete IRHelper;

}

void pGenerator::loadAndLinkRuntimeIR(void) {

    std::string errMsg;
    Module* irMod = pGenSupport::getRuntimeIR();

    // Linker will take ownership of irMod

    // here we link the irRuntime.ir with our clean module so it already includes
    // definitions of types and functions we need for code generation
    Linker l(llvmModule->getModuleIdentifier()+"_link", llvmModule);
    l.LinkInModule(irMod, &errMsg);
    if (errMsg.length()) {
        // TODO: errors
        std::cerr << "error linking in runtime ir: " << errMsg << std::endl;
        exit(1);
    }

    // take ownership of module so it's not freed
    l.releaseModule();

    // IRHelper knows how to pull types out of the composite module
    IRHelper = new pIRHelper(llvmModule);

}

void pGenerator::createEntryPoint(void) {

    // script top level initialization function
    entryFunctionName = pGenSupport::mangleModuleName(llvmModule->getModuleIdentifier());

    // entry function
    Function *initFun = Function::Create(IRHelper->moduleEntryFunType(),
                                         Function::ExternalLinkage,
                                         entryFunctionName,
                                         llvmModule);

    runtimeEngine = initFun->arg_begin();
    runtimeEngine->setName("rEngine");

    // entry block
    currentBlock.SetInsertPoint(BasicBlock::Create("entry", initFun));

}

void pGenerator::finalize(void) {
    // terminate entry function
    Function *initFun = llvmModule->getFunction(entryFunctionName);
    initFun->getEntryBlock().getInstList().push_back(ReturnInst::Create());
    // DEBUG
    verifyModule(*llvmModule);
}

void pGenerator::visit_literalBString(AST::literalBString* n) {

//     assert(n->getVal().size() > 0);
// 
//     ArrayType* stringLiteralType = ArrayType::get(IntegerType::get(8), n->getVal().size()+1);
// 
//     GlobalVariable* globalStr = new GlobalVariable(
//     /*Type=*/stringLiteralType,
//     /*isConstant=*/true,
//     /*Linkage=*/GlobalValue::InternalLinkage,
//     /*Initializer=*/0, // has initializer, specified below
//     /*Name=*/".pBString",
//     llvmModule);
// 
//     Constant* const_array_7 = ConstantArray::get(n->getVal().c_str(), true);
//     std::vector<Constant*> const_ptr_8_indices;
//     Constant* const_int32_9 = Constant::getNullValue(IntegerType::get(32));
//     const_ptr_8_indices.push_back(const_int32_9);
//     const_ptr_8_indices.push_back(const_int32_9);
//     Constant* litStringRef = ConstantExpr::getGetElementPtr(globalStr, &const_ptr_8_indices[0], const_ptr_8_indices.size() );
// 
//     // Global Variable Definitions
//     globalStr->setInitializer(const_array_7);

    // turn it into a pVar and push
//    valueStack.push(emitVarCreate_pBString(litStringRef));

    ArrayType* stringLiteralType = ArrayType::get(IntegerType::get(8), n->val.size()+1);

    GlobalVariable* gvar_array__str = new GlobalVariable(
    /*Type=*/stringLiteralType,
    /*isConstant=*/true,
    /*Linkage=*/GlobalValue::InternalLinkage,
    /*Initializer=*/0, // has initializer, specified below
    /*Name=*/".str",
    llvmModule);

    // Constant Definitions
    Constant* const_array_7 = ConstantArray::get(n->val.c_str(), true);
    std::vector<Constant*> const_ptr_8_indices;
    Constant* const_int32_9 = Constant::getNullValue(IntegerType::get(32));
    const_ptr_8_indices.push_back(const_int32_9);
    const_ptr_8_indices.push_back(const_int32_9);
    Constant* strPtr = ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_8_indices[0], const_ptr_8_indices.size() );
 
    // Global Variable Definitions
    gvar_array__str->setInitializer(const_array_7);

    // allocate tmp pVar for return value
    Value* pVarTmp = currentBlock.CreateAlloca(IRHelper->pVarType(), 0, "pVarTmp");
    Function* constructor = llvmModule->getFunction("_ZN4rphp4pVarC1Ev");
    assert(constructor != NULL);
    currentBlock.CreateCall(constructor, pVarTmp);

    // convert cstr to pbstring
    Function* f = llvmModule->getFunction("rphp_make_pVar_from_cstr");
    assert(f != NULL);
    currentBlock.CreateCall2(f, pVarTmp, strPtr);

    // push to stack
    valueStack.push(pVarTmp);

}

void pGenerator::visit_literalInt(AST::literalInt* n) {

//     GlobalVariable* globalInt = new GlobalVariable(
//     /*Type=*/IntegerType::get(32),
//     /*isConstant=*/false,
//     /*Linkage=*/GlobalValue::InternalLinkage,
//     /*Initializer=*/0, // has initializer, specified below
//     /*Name=*/".pInt",
//     llvmModule);
// 
//     // TODO: others bases besides 10
//     ConstantInt* const_int32 = ConstantInt::get(APInt(32,  n->getVal(), 10));
// 
//     globalInt->setInitializer(const_int32);
// 
//     valueStack.push(emitVarCreate_pInt(globalInt));

}

void pGenerator::visit_inlineHtml(AST::inlineHtml* n) {

    ArrayType* stringLiteralType = ArrayType::get(IntegerType::get(8), n->val.size()+1);

    GlobalVariable* gvar_array__str = new GlobalVariable(
    /*Type=*/stringLiteralType,
    /*isConstant=*/true,
    /*Linkage=*/GlobalValue::InternalLinkage,
    /*Initializer=*/0, // has initializer, specified below
    /*Name=*/".str",
    llvmModule);

    // Constant Definitions
    Constant* const_array_7 = ConstantArray::get(n->val.c_str(), true);
    std::vector<Constant*> const_ptr_8_indices;
    Constant* const_int32_9 = Constant::getNullValue(IntegerType::get(32));
    const_ptr_8_indices.push_back(const_int32_9);
    const_ptr_8_indices.push_back(const_int32_9);
    Constant* strPtr = ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_8_indices[0], const_ptr_8_indices.size() );
 
    // Global Variable Definitions
    gvar_array__str->setInitializer(const_array_7);

    Function *f = llvmModule->getFunction("rphp_print_cstr");
    assert(f != NULL);

    currentBlock.CreateCall2(f, runtimeEngine, strPtr);

}

void pGenerator::visit_echoStmt(AST::echoStmt* n) {

    visit(n->getRVal());
    Value* rVal = valueStack.back();
    valueStack.pop();

    Function *f = llvmModule->getFunction("rphp_print_pVar");
    assert(f != NULL);

    currentBlock.CreateCall2(f, runtimeEngine, rVal);

}

// assumes literal is on top of stack
// void pGenerator::emitEchoLiteralString(void) {
// 
//     Value* rVal = valueStack.back();
//     valueStack.pop();
// 
//     // argument sig for print function
//     std::vector<const Type*> printSig;
//     printSig.push_back(IRHelper->runtimeEngineType());
//     printSig.push_back(IRHelper->pVarPointerType());
// 
//     // print function type
//     FunctionType *printFuncType = FunctionType::get(Type::VoidTy, printSig, false);
// 
//     // print function
//     //Constant *printFunc = llvmModule->getOrInsertFunction("rphp_print_pvar", printFuncType);
// 
//     //currentBlock.CreateCall2(printFunc, runtimeEngine, rVal);
// 
// }

// allocate a pVar on the stack in the current block, return pVar*
// llvm::Value* emitVarCreate() {
// 
//     /*
//     AllocaInst* pVar_p = new AllocaInst(IRHelper->pVarType(), "pVarBString", currentBlock.getInsertBlock());
//     pVar_p->setAlignment(4);
//     emitVarConstruct(pVar_p);
//     
//     return pVar_p;
//     */
// 
// }

// create a new pVar on the stack and assign a pBString to it
// return pVar*
// llvm::Value* pGenerator::emitVarCreate_pBString(llvm::Value*) {
// 
//     /*
//     Value* pVar_p = emitVarCreate();
//     rphp_create_pVar_pBString
//     return pVar_p;
//     
//     // call runtime to create pVar from char*
//     // TODO: inline this
//     std::vector<const Type*> fSig;
//     printSig.push_back(IRHelper->charPointerType(), 0);
//     FunctionType *fFuncType = FunctionType::get(IRHelper->pVarType(), fSig, false);
//     Constant *Func = llvmModule->getOrInsertFunction("rphp_create_pVar_pBString", fFuncType);
// 
//     return currentBlock.CreateCall2(printFunc, runtimeEngine, rVal);
//     */
//     
// }

// create a new pVar on the stack and assign a pBString to it
// return pVar*
// llvm::Value* pGenerator::emitVarCreate_pInt(llvm::Value*) {
// 
//     
// }

// // call pVar::pVar(pVar*)
// void pGenerator::emitVarConstruct(llvm::Value* v) {
// 
//     Function* pVarConstructFun = Function::Create(
//     /*Type=*/IRHelper->pVarBaseFunType(),
//     /*Linkage=*/GlobalValue::LinkOnceLinkage ,
//     /*Name=*/"_ZN4rphp4pVarC1Ev", llvmModule);
// 
//     currentBlock.CreateCall(pVarConstructFun, v);
// 
// }
// 
// 
// // call pVar::~pVar(pVar*)
// void pGenerator::emitVarDestruct(llvm::Value* v) {
// 
//     Function* pVarDestructFun = Function::Create(
//     /*Type=*/IRHelper->pVarBaseFunType(),
//     /*Linkage=*/GlobalValue::LinkOnceLinkage ,
//     /*Name=*/"_ZN4rphp4pVarD1Ev", llvmModule);
// 
//     currentBlock.CreateCall(pVarDestructFun, v);
// 
// }


} // namespace

