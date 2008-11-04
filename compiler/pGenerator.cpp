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

pGenerator::pGenerator(llvm::Module* m, pCompileTarget* t): llvmModule_(m), target_(t), IRHelper_(0) {

    loadAndLinkRuntimeIR();
    createEntryPoint();

}
    
pGenerator::~pGenerator(void) {

    delete IRHelper_;

}

void pGenerator::loadAndLinkRuntimeIR(void) {

    std::string errMsg;
    Module* irMod = pGenSupport::getRuntimeIR();

    // Linker will take ownership of irMod

    // here we link the irRuntime.ir with our clean module so it already includes
    // definitions of types and functions we need for code generation
    Linker l(llvmModule_->getModuleIdentifier()+"_link", llvmModule_);
    l.LinkInModule(irMod, &errMsg);
    if (errMsg.length()) {
        // TODO: errors
        std::cerr << "error linking in runtime ir: " << errMsg << std::endl;
        exit(1);
    }

    // take ownership of module so it's not freed
    l.releaseModule();

    // IRHelper knows how to pull types out of the composite module
    IRHelper_ = new pIRHelper(llvmModule_);

}

void pGenerator::createEntryPoint(void) {

    // script top level initialization function
    entryFunctionName_ = pGenSupport::mangleModuleName(llvmModule_->getModuleIdentifier());

    // entry function
    Function *initFun = Function::Create(IRHelper_->moduleEntryFunType(),
                                         Function::ExternalLinkage,
                                         entryFunctionName_,
                                         llvmModule_);

    runtimeEngine_ = initFun->arg_begin();
    runtimeEngine_->setName("rEngine");

    // entry block
    currentBlock_.SetInsertPoint(BasicBlock::Create("entry", initFun));
    currentFunction_ = initFun;

    destructList_.push(valueVectorType());

}

void pGenerator::finalize(void) {
    // terminate entry function
    Function *initFun = llvmModule_->getFunction(entryFunctionName_);
    // destructors in global module space
    Function* destructor = llvmModule_->getFunction("_ZN4rphp4pVarD1Ev");
    assert(destructor != NULL);
    for (valueVectorType::iterator i = destructList_.back().begin(); i != destructList_.back().end(); ++i) {
        currentFunction_->getEntryBlock().getInstList().push_back(CallInst::Create(destructor, *i));
    }
    destructList_.pop();
    // create return
    initFun->getEntryBlock().getInstList().push_back(ReturnInst::Create());
    // DEBUG
    verifyModule(*llvmModule_);
}

// create a pVar on the stack. this handles construction
// at the start of the current function, and destruction at it's end
llvm::Value* pGenerator::newVarOnStack(const char* name) {

    
    AllocaInst* pVarTmp = new AllocaInst(IRHelper_->pVarType(), 0, name);
    currentFunction_->getEntryBlock().getInstList().push_front(pVarTmp);

    Function* constructor = llvmModule_->getFunction("_ZN4rphp4pVarC1Ev");
    assert(constructor != NULL);
    
    CallInst* con = CallInst::Create(constructor, pVarTmp);
    currentFunction_->getEntryBlock().getInstList().push_back(con);

    destructList_.back().push_back(pVarTmp);

    return pVarTmp;

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
//     llvmModule_);
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
//    valueStack_.push(emitVarCreate_pBString(litStringRef));

    ArrayType* stringLiteralType = ArrayType::get(IntegerType::get(8), n->getStringVal().size()+1);

    GlobalVariable* gvar_array__str = new GlobalVariable(
    /*Type=*/stringLiteralType,
    /*isConstant=*/true,
    /*Linkage=*/GlobalValue::InternalLinkage,
    /*Initializer=*/0, // has initializer, specified below
    /*Name=*/".str",
    llvmModule_);

    // Constant Definitions
    Constant* const_array_7 = ConstantArray::get(n->getStringVal().c_str(), true);
    std::vector<Constant*> const_ptr_8_indices;
    Constant* const_int32_9 = Constant::getNullValue(IntegerType::get(32));
    const_ptr_8_indices.push_back(const_int32_9);
    const_ptr_8_indices.push_back(const_int32_9);
    Constant* strPtr = ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_8_indices[0], const_ptr_8_indices.size() );
 
    // Global Variable Definitions
    gvar_array__str->setInitializer(const_array_7);

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pBStrTmp");
    
    // convert cstr to pbstring
    Function* f = llvmModule_->getFunction("rphp_make_pVar_from_cstr");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, strPtr);

    // push to stack
    valueStack_.push(pVarTmp);

}

void pGenerator::visit_literalInt(AST::literalInt* n) {

    // TODO: other bases besides 10
    ConstantInt* const_int = ConstantInt::get(APInt(32,  n->getStringVal(), 10));

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pIntTmp");

    // convert cstr to pbstring
    Function* f = llvmModule_->getFunction("rphp_make_pVar_from_pInt");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, const_int);

    // push to stack
    valueStack_.push(pVarTmp);

}

void pGenerator::visit_literalFloat(AST::literalFloat* n) {

    ConstantFP* const_float = ConstantFP::get(APFloat(APFloat::IEEEdouble,  n->getStringVal().c_str()));

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pFloatTmp");

    // convert cstr to pbstring
    Function* f = llvmModule_->getFunction("rphp_make_pVar_from_pFloat");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, const_float);

    // push to stack
    valueStack_.push(pVarTmp);

}


void pGenerator::visit_literalBool(AST::literalBool* n) {

//     GlobalVariable* globalInt = new GlobalVariable(
//     /*Type=*/IntegerType::get(32),
//     /*isConstant=*/false,
//     /*Linkage=*/GlobalValue::InternalLinkage,
//     /*Initializer=*/0, // has initializer, specified below
//     /*Name=*/".pBool",
//     llvmModule_);

    ConstantInt* cbool = ConstantInt::get(APInt(32,  (n->getBoolVal()) ? "1" : "0", 10));

//    globalInt->setInitializer(const_int32);

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pBoolTmp");

    // convert cstr to pbstring
    Function* f = llvmModule_->getFunction("rphp_make_pVar_bool");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, cbool);

    // push to stack
    valueStack_.push(pVarTmp);

}

void pGenerator::visit_literalNull(AST::literalNull* n) {
    
    Value* pVarTmp = newVarOnStack("pNullTmp");

    // push to stack
    valueStack_.push(pVarTmp);

}

void pGenerator::visit_inlineHtml(AST::inlineHtml* n) {

    ArrayType* stringLiteralType = ArrayType::get(IntegerType::get(8), n->getStringVal().size()+1);

    GlobalVariable* gvar_array__str = new GlobalVariable(
    /*Type=*/stringLiteralType,
    /*isConstant=*/true,
    /*Linkage=*/GlobalValue::InternalLinkage,
    /*Initializer=*/0, // has initializer, specified below
    /*Name=*/".str",
    llvmModule_);

    // Constant Definitions
    Constant* const_array_7 = ConstantArray::get(n->getStringVal().c_str(), true);
    std::vector<Constant*> const_ptr_8_indices;
    Constant* const_int32_9 = Constant::getNullValue(IntegerType::get(32));
    const_ptr_8_indices.push_back(const_int32_9);
    const_ptr_8_indices.push_back(const_int32_9);
    Constant* strPtr = ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_8_indices[0], const_ptr_8_indices.size() );
 
    // Global Variable Definitions
    gvar_array__str->setInitializer(const_array_7);

    Function *f = llvmModule_->getFunction("rphp_print_cstr");
    assert(f != NULL);

    currentBlock_.CreateCall2(f, runtimeEngine_, strPtr);

}

void pGenerator::visit_echoStmt(AST::echoStmt* n) {

    visit(n->getRVal());
    Value* rVal = valueStack_.back();
    valueStack_.pop();

    Function *f = llvmModule_->getFunction("rphp_print_pVar");
    assert(f != NULL);

    currentBlock_.CreateCall2(f, runtimeEngine_, rVal);

}

// assumes literal is on top of stack
// void pGenerator::emitEchoLiteralString(void) {
// 
//     Value* rVal = valueStack_.back();
//     valueStack_.pop();
// 
//     // argument sig for print function
//     std::vector<const Type*> printSig;
//     printSig.push_back(IRHelper_->runtimeEngineType());
//     printSig.push_back(IRHelper_->pVarPointerType());
// 
//     // print function type
//     FunctionType *printFuncType = FunctionType::get(Type::VoidTy, printSig, false);
// 
//     // print function
//     //Constant *printFunc = llvmModule_->getOrInsertFunction("rphp_print_pvar", printFuncType);
// 
//     //currentBlock_.CreateCall2(printFunc, runtimeEngine, rVal);
// 
// }

// allocate a pVar on the stack in the current block, return pVar*
// llvm::Value* emitVarCreate() {
// 
//     /*
//     AllocaInst* pVar_p = new AllocaInst(IRHelper_->pVarType(), "pVarBString", currentBlock_.getInsertBlock());
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
//     printSig.push_back(IRHelper_->charPointerType(), 0);
//     FunctionType *fFuncType = FunctionType::get(IRHelper_->pVarType(), fSig, false);
//     Constant *Func = llvmModule_->getOrInsertFunction("rphp_create_pVar_pBString", fFuncType);
// 
//     return currentBlock_.CreateCall2(printFunc, runtimeEngine, rVal);
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
//     /*Type=*/IRHelper_->pVarBaseFunType(),
//     /*Linkage=*/GlobalValue::LinkOnceLinkage ,
//     /*Name=*/"_ZN4rphp4pVarC1Ev", llvmModule_);
// 
//     currentBlock_.CreateCall(pVarConstructFun, v);
// 
// }
// 
// 
// // call pVar::~pVar(pVar*)
// void pGenerator::emitVarDestruct(llvm::Value* v) {
// 
//     Function* pVarDestructFun = Function::Create(
//     /*Type=*/IRHelper_->pVarBaseFunType(),
//     /*Linkage=*/GlobalValue::LinkOnceLinkage ,
//     /*Name=*/"_ZN4rphp4pVarD1Ev", llvmModule_);
// 
//     currentBlock_.CreateCall(pVarDestructFun, v);
// 
// }


} // namespace

