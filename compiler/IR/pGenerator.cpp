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

#include "rphp/IR/pCompileError.h"
#include "rphp/IR/pGenerator.h"
#include "rphp/IR/pIRHelper.h"
#include "rphp/IR/pGenSupport.h"

using namespace llvm;

namespace rphp {

pGenerator::pGenerator(const std::string& moduleName):
    llvmModule_(new Module(moduleName)),
    IRHelper_(NULL),
    entryFunctionName_(),
    currentBlock_(),
    runtimeEngine_(NULL),
    currentFunction_(NULL),
    valueStack_(),
    destructList_(),
    globalSymbols_()
{

    loadAndLinkRuntimeIR();
    createEntryPoint();

}
    
pGenerator::~pGenerator(void) {

    // won't free llvmModule!
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
        throw pCompileError("error linking in runtime IR [" + errMsg + "]");
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

void pGenerator::visit_literalString(AST::literalString* n) {

    bool isUnicode = !n->isBinary();
    int32_t finalLen(0);
    
    Constant* strPtr;
    if (isUnicode) {
        strPtr = IRHelper_->stringConstant(n->getStringVal(), finalLen);
    }
    else {
        // strip unicodeyness
        strPtr = IRHelper_->stringConstant(std::string(n->getStringVal().begin(), n->getStringVal().end()), finalLen);
    }
 
    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack((isUnicode)?"pUStrTmp":"pBStrTmp");
    
    // convert cstr to pbstring
    Function* f = llvmModule_->getFunction((isUnicode)?"rphp_make_pVar_pUString":"rphp_make_pVar_pBString");
    assert(f != NULL);

    if (isUnicode) {
        currentBlock_.CreateCall3(f, pVarTmp, strPtr, ConstantInt::get(APInt(32, finalLen)));
    }
    else {
        currentBlock_.CreateCall2(f, pVarTmp, strPtr);
    }

    // push to stack
    valueStack_.push(pVarTmp);

}

void pGenerator::visit_literalInt(AST::literalInt* n) {

    // TODO: other bases besides 10
    std::string numLiteral(n->getStringVal().begin(), n->getStringVal().end());
    ConstantInt* const_int = ConstantInt::get(APInt(32,  numLiteral.data(), numLiteral.length(), 10));

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pIntTmp");

    Function* f = llvmModule_->getFunction("rphp_make_pVar_pInt");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, const_int);

    // push to stack
    valueStack_.push(pVarTmp);

}

void pGenerator::visit_literalFloat(AST::literalFloat* n) {

    std::string numLiteral(n->getStringVal().begin(), n->getStringVal().end());
    ConstantFP* const_float = ConstantFP::get(APFloat(APFloat::IEEEdouble,  numLiteral.c_str()));

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pFloatTmp");

    Function* f = llvmModule_->getFunction("rphp_make_pVar_pFloat");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, const_float);

    // push to stack
    valueStack_.push(pVarTmp);

}


void pGenerator::visit_literalBool(AST::literalBool* n) {

    ConstantInt* cbool = ConstantInt::get(APInt(32,  (n->getBoolVal()) ? "1" : "0", 1, 10));

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pBoolTmp");

    Function* f = llvmModule_->getFunction("rphp_make_pVar_pBool");
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

    Function *f = llvmModule_->getFunction("rphp_print_cstr");
    assert(f != NULL);

    int32_t finalLen(0);
    currentBlock_.CreateCall2(f, runtimeEngine_, IRHelper_->stringConstant(n->getStringVal(), finalLen));

}

void pGenerator::visit_echoStmt(AST::echoStmt* n) {

    visit(n->rVal());
    Value* rVal = valueStack_.back();
    valueStack_.pop();

    Function *f = llvmModule_->getFunction("rphp_print_pVar");
    assert(f != NULL);

    currentBlock_.CreateCall2(f, runtimeEngine_, rVal);

}

void pGenerator::visit_assignment(AST::assignment* n) {

    // gen rval
    visit(n->rVal());
    Value* rVal = valueStack_.back();
    valueStack_.pop();

    // gen lval
    pIdentString name("lVal");
    if (n->lVal()->getKind() == AST::varKind) {
        AST::var* l = static_cast<AST::var*>(n->lVal());
        name = l->name();
    }
    visit(n->lVal());
    Value* lVal = valueStack_.back();
    valueStack_.pop();

    Function* f = llvmModule_->getFunction("_ZN4rphp4pVaraSERKS0_");
    assert(f != NULL);

    currentBlock_.CreateCall2(f, lVal, rVal, name.c_str());

}

void pGenerator::visit_var(AST::var* n) {

    symbolTableType::iterator sym = globalSymbols_.find(n->name());
    if (sym == globalSymbols_.end()) {
        Value* v = newVarOnStack(n->name().c_str());
        globalSymbols_[n->name()] = v;
        valueStack_.push(v);
    }
    else {
        valueStack_.push((*sym).second);
    }
    

}

void pGenerator::visit_functionInvoke(AST::functionInvoke* n) {

    Function *f;
    switch (n->argList().size()) {
        case 0:
            f = llvmModule_->getFunction("rphp_funCall0");
        break;
        case 1:
            f = llvmModule_->getFunction("rphp_funCall1");
        break;
        case 2:
            f = llvmModule_->getFunction("rphp_funCall2");
        break;
        case 3:
            f = llvmModule_->getFunction("rphp_funCall3");
        break;
        case 4:
            f = llvmModule_->getFunction("rphp_funCall4");
        break;
        case 5:
            f = llvmModule_->getFunction("rphp_funCall5");
        break;
        default:
            f = llvmModule_->getFunction("rphp_funCallN");
        break;
    }
    assert(f != NULL);

    Value* retval = newVarOnStack("retval");
    std::vector<Value*> callArgList;
    callArgList.push_back(retval);
    callArgList.push_back(runtimeEngine_);
    int32_t len(0);
    callArgList.push_back(IRHelper_->stringConstant(n->name(),len));
    
    // visit arguments in reverse order, add to call arg list as we go
    for (AST::expressionList::reverse_iterator i = n->argList().rbegin(); i != n->argList().rend(); ++i) {
        visit(*i);
        callArgList.push_back(valueStack_.back());
        valueStack_.pop();
    }
    
    Value *result = currentBlock_.CreateCall(f, callArgList.begin(), callArgList.end());

    valueStack_.push(retval);

}

} // namespace

