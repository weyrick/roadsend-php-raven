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
    currentBlock_(getGlobalContext()),
    runtimeEngine_(NULL),
    thisFunction_(NULL),
    valueStack_(),
    destructList_(),
    symTable_(),
    // TODO: better way to get int size?
    wordSize_((llvmModule_->getPointerSize() == Module::Pointer64) ? 64 : 32),
    ifRecursionDepth_(0),
    lastSourceLoc_(0)
{

    // This is the entry function of the module if we compile a whole module.
    thisFunction_ = llvmModule_->getFunction(functionSymbol_);
    assert(thisFunction_ && "declare pass didn't initialize this function");

    Function::arg_iterator a = thisFunction_->arg_begin();
    // Skip the retval argument
    a++;
    runtimeEngine_ = a;

    // rest of the function args (if any) go in symTab
    for (; a != thisFunction_->arg_end(); ++a) {
        symTable_[a->getName()] = a;
    }

    // entry block created in declare
    // This sets the start of codegen after all instrs already added by pGenerator.cpp
    currentBlock_.SetInsertPoint(&thisFunction_->getEntryBlock());

    destructList_.push(valueVectorType());

    // The allocBlock should contain all allocas for vars + their constructor calls.
    allocBlock = BasicBlock::Create(getGlobalContext(), "allocBlock", thisFunction_);
    currentBlock_.CreateBr(allocBlock);

    // Go to this at the end of allocBlock.
    // This is the first block wich contains compiled code.
    beginBlock = BasicBlock::Create(getGlobalContext(), "begin", thisFunction_);
    currentBlock_.SetInsertPoint(beginBlock);

}

pCodeGen::~pCodeGen(void) {

    finalize();

}

void pCodeGen::finalize(void) {

    // destruct current stack vars
	// rphp::pVar::~pVar()
    Function* destructor = llvmModule_->getFunction("_ZN4rphp4pVarD1Ev");
    assert(destructor != NULL);

    BasicBlock* end = BasicBlock::Create(getGlobalContext(), "end", thisFunction_);
    // Branch to the last BasicBlock
    currentBlock_.CreateBr(end);
    currentBlock_.SetInsertPoint(end);

    // The destructList is a queue
    // TODO:
    for (valueVectorType::iterator i = destructList_.back().begin(); i != destructList_.back().end(); ++i) {
        currentBlock_.CreateCall(destructor, *i);
    }
    destructList_.pop();
    // create return
    currentBlock_.CreateRet(0);

    // newVarOnStack isn't used anymore, so build it's block end.
    currentBlock_.SetInsertPoint(allocBlock);
    // Jump to the beginBlock.
    currentBlock_.CreateBr(beginBlock);
}


// create a pVar on the stack. this handles construction
// at the start of the current function, and destruction at it's end
Value* pCodeGen::newVarOnStack(const char* name) {
	BasicBlock* oldBlock = currentBlock_.GetInsertBlock();
	currentBlock_.SetInsertPoint(allocBlock);

	Value* pVarTmp = currentBlock_.CreateAlloca(IRHelper_.pVarType(), 0, name);

    // rphp::pVar::pVar()
    Function* constructor = llvmModule_->getFunction("_ZN4rphp4pVarC1Ev");
    assert(constructor != NULL && "couldn't find pVar constructor in IR runtime.");

    currentBlock_.CreateCall(constructor, pVarTmp);

    // Set back the insert point to the old block.
    currentBlock_.SetInsertPoint(oldBlock);

    // Call the destructor of the new pVar at the end of the function.
    destructList_.back().push_back(pVarTmp);
    return pVarTmp;
}

BasicBlock* pCodeGen::visitInOwnBlock(AST::stmt* n, const std::string &Name)
{
	// TODO: remove llvm:: in cpp.
	llvm::BasicBlock* oldBlock = currentBlock_.GetInsertBlock();
	llvm::BasicBlock* block = llvm::BasicBlock::Create(getGlobalContext(), Name, thisFunction_);
	if(n) {
		currentBlock_.SetInsertPoint(block);
		visit(n);
		currentBlock_.SetInsertPoint(oldBlock);
	}
	return block;
}

void pCodeGen::updateSourceLocation(const AST::stmt* n) {
    // TODO: it makes maybe more sense to implement this as a pre-visitor for all stmts?
    // if we already emitted for this line, skip it
    if (n->getStartLine() == lastSourceLoc_)
        return;

    // TODO check current pConfig, avoid this if optimizing
    int32_t finalLen(0);
    Constant* filestrPtr;
    // TODO we can cache this here since it will never change
    filestrPtr = IRHelper_.stringConstant(llvmModule_->getModuleIdentifier(), finalLen);

    lastSourceLoc_ = n->getStartLine();

    ConstantInt* lineNo = ConstantInt::get(getGlobalContext(),
            APInt(wordSize_, n->getStartLine()));

    Function* f = llvmModule_->getFunction("rphp_setSourceLocation");
    assert(f != NULL);
    currentBlock_.CreateCall3(f, runtimeEngine_, filestrPtr, lineNo);


}

void pCodeGen::visit_literalString(AST::literalString* n) {

    updateSourceLocation(n);

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
        currentBlock_.CreateCall3(f, pVarTmp, strPtr, ConstantInt::get(getGlobalContext(), APInt(wordSize_, finalLen)));
    }
    else {
        currentBlock_.CreateCall2(f, pVarTmp, strPtr);
    }

    // push to stack
    valueStack_.push(pVarTmp);

}

void pCodeGen::visit_literalInt(AST::literalInt* n) {

    updateSourceLocation(n);

    // TODO: other bases besides 10
    std::string numLiteral(n->getStringVal().begin(), n->getStringVal().end());
    ConstantInt* const_int = ConstantInt::get(getGlobalContext(),
                                APInt(wordSize_,
                                      StringRef(numLiteral.data(),
						numLiteral.length()),
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

    updateSourceLocation(n);

    std::string numLiteral(n->getStringVal().begin(), n->getStringVal().end());
    ConstantFP* const_float = ConstantFP::get(getGlobalContext(), APFloat(APFloat::IEEEdouble,  numLiteral.c_str()));

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pFloatTmp");

    Function* f = llvmModule_->getFunction("rphp_make_pVar_pFloat");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, const_float);

    // push to stack
    valueStack_.push(pVarTmp);

}


void pCodeGen::visit_literalBool(AST::literalBool* n) {

    updateSourceLocation(n);

    // NOTE even on 64 bit, this is 32 (with g++)
    ConstantInt* cbool = ConstantInt::get(getGlobalContext(), APInt(32,  (n->getBoolVal()) ? "1" : "0", 10));

    // allocate tmp pVar for return value
    Value* pVarTmp = newVarOnStack("pBoolTmp");

    Function* f = llvmModule_->getFunction("rphp_make_pVar_pBool");
    assert(f != NULL);
    currentBlock_.CreateCall2(f, pVarTmp, cbool);

    // push to stack
    valueStack_.push(pVarTmp);

}

void pCodeGen::visit_literalNull(AST::literalNull* n) {

    updateSourceLocation(n);

    Value* pVarTmp = newVarOnStack("pNullTmp");

    Function* f = llvmModule_->getFunction("rphp_make_pVar_pNull");
    assert(f != NULL);
    currentBlock_.CreateCall(f, pVarTmp);

    // push to stack
    valueStack_.push(pVarTmp);
}

void pCodeGen::visit_literalArray(AST::literalArray* n) {

    updateSourceLocation(n);

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

    for (AST::arrayList::reverse_iterator i = n->itemList().rbegin();
        i != n->itemList().rend();
        ++i)
    {
        // gen key (if necessary) and value
        if ((*i).key) {
            visit((*i).key);
            assert(valueStack_.size() && "array key didn't push a value!");
            key = valueStack_.top();
            valueStack_.pop();
        }
        else {
            key = NULL;
        }

        visit((*i).val);
        assert(valueStack_.size() && "array value didn't push a value!");
        val = valueStack_.top();
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

    updateSourceLocation(n);

    visit(n->rVal());
    Value* rVal = valueStack_.top();
    valueStack_.pop();

    Function *f = llvmModule_->getFunction("rphp_print_pVar");
    assert(f != NULL);

    currentBlock_.CreateCall2(f, runtimeEngine_, rVal);
}

void pCodeGen::visit_assignment(AST::assignment* n) {

    updateSourceLocation(n);

    // gen rval
    visit(n->rVal());
    assert(valueStack_.size() && "rVal didn't push a value!");
    Value* rVal = valueStack_.top();
    valueStack_.pop();

    // gen lval
    pIdentString name("lVal");
    if (n->lVal()->getKind() == AST::varKind) {
        AST::var* l = static_cast<AST::var*>(n->lVal());
        name = l->name();
    }
    visit(n->lVal());
    Value* lVal = valueStack_.top();
    valueStack_.pop();

    Function* f = llvmModule_->getFunction("_ZN4rphp4pVaraSERKS0_");
    assert(f != NULL);

    currentBlock_.CreateCall2(f, lVal, rVal, name.c_str());
    //@TODO: push the return to the stack, make the assignement function to adhere php's standards here.
}

void pCodeGen::visit_var(AST::var* n) {

    updateSourceLocation(n);

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

    updateSourceLocation(n);

    Value* retval = newVarOnStack("retval");
    std::vector<Value*> callArgList;
    callArgList.push_back(retval);
    callArgList.push_back(runtimeEngine_);
    int32_t len(0);
    callArgList.push_back(IRHelper_.stringConstant(n->name(),len));

    // visit arguments in reverse order, add to call arg list as we go
    for (AST::expressionList::reverse_iterator i = n->argList().rbegin(); i != n->argList().rend(); ++i) {
        visit(*i);
        callArgList.push_back(valueStack_.top());
        valueStack_.pop();
    }

    currentBlock_.CreateCall(f, callArgList.begin(), callArgList.end());

    valueStack_.push(retval);
}

void pCodeGen::visit_ifStmt(AST::ifStmt* n) {

	// Keep track of the level of recursion, so we can adjust the afterConds accordingly.
	ifRecursionDepth_++;

	// Trueblock
	llvm::BasicBlock* trueBlock = visitInOwnBlock(n->trueBlock(), "trueblock");

	// Falseblock, visitInOwnBlock handles NULLs.
	llvm::BasicBlock* falseBlock = visitInOwnBlock(n->falseBlock(), "falseblock");

	// Condition
	//assert(currentBlock_.GetInsertBlock()->getTerminator() == NULL && "Already a terminator instruction in this BB!");
	visit(n->condition());

	Function* evalTo = llvmModule_->getFunction("rphp_pVar_evalAsBool");
	assert(evalTo != NULL && "Couldn't find the rphp_pVar_evalAsBool function in the runtime");

	assert(!valueStack_.empty() && "No expression for the condition avaible.");
	Value* conditionValue = currentBlock_.CreateCall(evalTo, valueStack_.top());
	valueStack_.pop();

	Value* trueValue = ConstantInt::get(evalTo->getReturnType(), 1);

	llvm::Value* condition = currentBlock_.CreateICmpEQ(conditionValue, trueValue, "if-cmp");
	llvm::BasicBlock* afterCond = BasicBlock::Create(getGlobalContext(), "afterCond", thisFunction_);

	// Branch
	currentBlock_.CreateCondBr(condition, trueBlock, falseBlock);

	// Both blocks jump after there execution to the afterCond block.
	// Don't create branch instructions in already terminated BBs, llvm doesn't like this.
	// This happens when the control flow moves out of these BBs during the execution of the if.
	// Note: LLVM doesn't support nested Blocks, that's why this is necessary at all.
	if(!trueBlock->getTerminator()) {
		currentBlock_.SetInsertPoint(trueBlock);
		currentBlock_.CreateBr(afterCond);
	}
	if(!falseBlock->getTerminator()) {
		currentBlock_.SetInsertPoint(falseBlock);
		currentBlock_.CreateBr(afterCond);
	}

	// Fix up the Control Flow, as when we do an if() in another if, we leave it's BB, and afterwards we get in another BB
	// To which we need to add our branch.
	while(!endingIfBlocks_.empty()) {
		llvm::BasicBlock* block = endingIfBlocks_.top();
		if(!block->getTerminator()) {
			currentBlock_.SetInsertPoint(block);
			currentBlock_.CreateBr(afterCond);
		}
		endingIfBlocks_.pop();
	}
	currentBlock_.SetInsertPoint(afterCond);
	ifRecursionDepth_--;
	if(ifRecursionDepth_)
		endingIfBlocks_.push(afterCond);

}

} } // namespace

