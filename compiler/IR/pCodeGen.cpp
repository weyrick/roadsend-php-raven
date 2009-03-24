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

#include "rphp/IR/pCodeGen.h"

#include "rphp/IR/pCompileError.h"
#include "rphp/IR/pIRHelper.h"
#include "rphp/IR/pGenSupport.h"

#include <llvm/Module.h>
#include <llvm/GlobalVariable.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Constants.h>
#include <llvm/Instructions.h>
#include <llvm/Linker.h>

using namespace llvm;

namespace rphp { namespace IR {

pCodeGen::pCodeGen(llvm::Module* mod, const pIdentString& funSym):
    llvmModule_(mod),
    functionSymbol_(funSym),
    IRHelper_(mod),
    currentBlock_(),
    runtimeEngine_(NULL),
    thisFunction_(NULL),
    valueStack_(),
    destructList_(),
    symTable_(),
    // TODO: better way to get int size?
    wordSize_((llvmModule_->getPointerSize() == Module::Pointer64) ? 64 : 32)
{

    thisFunction_ = llvmModule_->getFunction(functionSymbol_);
    assert(thisFunction_ && "declare pass didn't initialize this function");

    Function::arg_iterator a = thisFunction_->arg_begin();
    a++;
    runtimeEngine_ = a;

    // rest of the function args (if any) go in symTab
    for (; a != thisFunction_->arg_end(); ++a) {
        symTable_[a->getName()] = a;
    }
    
    // entry block created in declare
    currentBlock_.SetInsertPoint(&thisFunction_->getEntryBlock());

    destructList_.push(valueVectorType());

}

pCodeGen::~pCodeGen(void) {

    finalize();
    
}

void pCodeGen::finalize(void) {

    // destruct current stack vars
    Function* destructor = llvmModule_->getFunction("_ZN4rphp4pVarD1Ev");
    assert(destructor != NULL);
    for (valueVectorType::iterator i = destructList_.back().begin(); i != destructList_.back().end(); ++i) {
        thisFunction_->getEntryBlock().getInstList().push_back(CallInst::Create(destructor, *i));
    }
    destructList_.pop();
    // create return
    thisFunction_->getEntryBlock().getInstList().push_back(ReturnInst::Create());


}


// create a pVar on the stack. this handles construction
// at the start of the current function, and destruction at it's end
llvm::Value* pCodeGen::newVarOnStack(const char* name) {

    AllocaInst* pVarTmp = new AllocaInst(IRHelper_.pVarType(), 0, name);
    thisFunction_->getEntryBlock().getInstList().push_front(pVarTmp);

    Function* constructor = llvmModule_->getFunction("_ZN4rphp4pVarC1Ev");
    assert(constructor != NULL);

    CallInst* con = CallInst::Create(constructor, pVarTmp);
    thisFunction_->getEntryBlock().getInstList().push_back(con);

    destructList_.back().push_back(pVarTmp);

    return pVarTmp;

}

void pCodeGen::visit_literalString(AST::literalString* n) {

    bool isUnicode = !n->isBinary();
    int32_t finalLen(0);

    Constant* strPtr;
    if (isUnicode) {
        strPtr = IRHelper_.stringConstant(n->getStringVal(), finalLen);
    }
    else {
        // strip unicodeyness
        strPtr = IRHelper_.stringConstant(std::string(n->getStringVal().begin(), n->getStringVal().end()), finalLen);
    }

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack((isUnicode)?"pUStrTmp":"pBStrTmp");

    // convert cstr to pbstring
    Function* f = llvmModule_->getFunction((isUnicode)?"rphp_make_pVar_pUString":"rphp_make_pVar_pBString");
    assert(f != NULL);

    if (isUnicode) {
        currentBlock_.CreateCall3(f, pVarTmp, strPtr, ConstantInt::get(APInt(wordSize_, finalLen)));
    }
    else {
        currentBlock_.CreateCall2(f, pVarTmp, strPtr);
    }

    // push to stack
    valueStack_.push(pVarTmp);

}

void pCodeGen::visit_literalInt(AST::literalInt* n) {

    // TODO: other bases besides 10
    std::string numLiteral(n->getStringVal().begin(), n->getStringVal().end());
    ConstantInt* const_int = ConstantInt::get(
                                APInt(wordSize_,  
                                      numLiteral.data(), 
                                      numLiteral.length(), 
                                      10));

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pIntTmp");
    Function* f = llvmModule_->getFunction("rphp_make_pVar_pInt");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, const_int);

    // push to stack
    valueStack_.push(pVarTmp);

}

void pCodeGen::visit_literalFloat(AST::literalFloat* n) {

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


void pCodeGen::visit_literalBool(AST::literalBool* n) {

    // NOTE even on 64 bit, this is 32 (with g++)
    ConstantInt* cbool = ConstantInt::get(APInt(32,  (n->getBoolVal()) ? "1" : "0", 1, 10));

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pBoolTmp");

    Function* f = llvmModule_->getFunction("rphp_make_pVar_pBool");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, cbool);

    // push to stack
    valueStack_.push(pVarTmp);

}

void pCodeGen::visit_literalNull(AST::literalNull* n) {

    Value* pVarTmp = newVarOnStack("pNullTmp");

    // push to stack
    valueStack_.push(pVarTmp);

}

void pCodeGen::visit_literalArray(AST::literalArray* n) {

    Value* pHashTmp = newVarOnStack("pHashTmp");

    Function* f = llvmModule_->getFunction("rphp_make_pVar_pHash");
    assert(f != NULL);
    currentBlock_.CreateCall(f, pHashTmp);

    Function* hash_insertNext = llvmModule_->getFunction("rphp_pHash_insertNext");
    assert(hash_insertNext != NULL);
    Function* hash_insert = llvmModule_->getFunction("rphp_pHash_insert");
    assert(hash_insert != NULL);

    // TODO: references?
    // TODO: optimize: if it's a big array, insert more than one at a time

    Value* key;
    Value* val;

    for (AST::arrayList::const_reverse_iterator i = n->itemList().rbegin();
        i != n->itemList().rend();
        ++i)
    {

        // gen key (if necessary) and value
        if ((*i).key) {
            visit((*i).key);
            assert(valueStack_.size() && "array key didn't push a value!");
            key = valueStack_.back();
            valueStack_.pop();
        }
        else {
            key = NULL;
        }

        visit((*i).val);
        assert(valueStack_.size() && "array value didn't push a value!");
        val = valueStack_.back();
        valueStack_.pop();

        // insert
        if (key) {
            currentBlock_.CreateCall3(hash_insert, pHashTmp, key, val);
        }
        else {
            currentBlock_.CreateCall2(hash_insertNext, pHashTmp, val);
        }

    }

    // push to stack
    valueStack_.push(pHashTmp);

}

void pCodeGen::visit_inlineHtml(AST::inlineHtml* n) {

    Function *f = llvmModule_->getFunction("rphp_print_cstr");
    assert(f != NULL);

    int32_t finalLen(0);
    currentBlock_.CreateCall2(f, runtimeEngine_, IRHelper_.stringConstant(n->getStringVal(), finalLen));

}

void pCodeGen::visit_echoStmt(AST::echoStmt* n) {

    visit(n->rVal());
    Value* rVal = valueStack_.back();
    valueStack_.pop();

    Function *f = llvmModule_->getFunction("rphp_print_pVar");
    assert(f != NULL);

    currentBlock_.CreateCall2(f, runtimeEngine_, rVal);

}

void pCodeGen::visit_assignment(AST::assignment* n) {

    // gen rval
    visit(n->rVal());
    assert(valueStack_.size() && "rVal didn't push a value!");
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

void pCodeGen::visit_var(AST::var* n) {

    symbolTableType::iterator sym = symTable_.find(n->name());
    if (sym == symTable_.end()) {
        Value* v = newVarOnStack(n->name().c_str());
        symTable_[n->name()] = v;
        valueStack_.push(v);
    }
    else {
        valueStack_.push((*sym).second);
    }


}

void pCodeGen::visit_functionInvoke(AST::functionInvoke* n) {

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
    assert(f != NULL && " visit_functionInvoke couldn't find appropriate rphp_funCall in c-runtime");

    Value* retval = newVarOnStack("retval");
    std::vector<Value*> callArgList;
    callArgList.push_back(retval);
    callArgList.push_back(runtimeEngine_);
    int32_t len(0);
    callArgList.push_back(IRHelper_.stringConstant(n->name(),len));

    // visit arguments in reverse order, add to call arg list as we go
    for (AST::expressionList::reverse_iterator i = n->argList().rbegin(); i != n->argList().rend(); ++i) {
        visit(*i);
        callArgList.push_back(valueStack_.back());
        valueStack_.pop();
    }

    currentBlock_.CreateCall(f, callArgList.begin(), callArgList.end());

    valueStack_.push(retval);

}

} } // namespace

