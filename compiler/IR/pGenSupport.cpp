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

#include "rphp/IR/pGenSupport.h"

#include "rphp/IR/pIRHelper.h"
#include "rphp/IR/pCompileError.h"

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
#include <llvm/CallingConv.h>

#include <iostream>
#include <fstream>

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

    ModuleProvider* mp = getBitcodeModuleProvider(mb, getGlobalContext(), &errMsg);
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
    irFile.appendComponent("c-runtime.bc");

    if (irFile.exists()) {
        return readBitcode(irFile.toString());
    }
    else {
        throw pCompileError("unable to find c-runtime.bc - please set RPHP_IR_PATH environment variable to point to the directory containing this file.");
    }


}


void pGenSupport::createMain(Module *llvmModule, const pIdentString& entryFunctionName) {

    std::vector<const Type*>FuncTy_81_args;
    FunctionType* FuncTy_81 = FunctionType::get(
      /*Result=*/IntegerType::get(getGlobalContext(), 32),
      /*Params=*/FuncTy_81_args,
      /*isVarArg=*/false);

    std::vector<const Type*>FuncTy_5_args;
    FunctionType* FuncTy_5 = FunctionType::get(
      /*Result=*/IntegerType::get(getGlobalContext(), 32),
      /*Params=*/FuncTy_5_args,
      /*isVarArg=*/true);

    Function* func_main = Function::Create(
      /*Type=*/FuncTy_81,
      /*Linkage=*/GlobalValue::ExternalLinkage,
      /*Name=*/"main", llvmModule);
    //func_main->setCallingConv(CallingConv::C);
    AttrListPtr func_main_PAL;
    func_main->setAttributes(func_main_PAL);

    PointerType* PointerTy_0 = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
    PointerType* PointerTy_4 = PointerType::get(FuncTy_5, 0);
    PointerType* PointerTy_27 =PointerType::get(llvmModule->getTypeByName("struct.rphp::pRuntimeEngine"), 0);
    PointerType* PointerTy_39 = PointerType::get(llvmModule->getTypeByName("struct.rphp::pConfig"), 0);
    PointerType* PointerTy_594 = PointerType::get(llvmModule->getTypeByName("struct.icu_3_8::UObject"), 0);
    PointerType* PointerTy_596 = PointerType::get(llvmModule->getTypeByName("struct.rphp::pRuntimeError"), 0);
    const StructType* StructTy_struct_rphp__pVar = (const StructType*)llvmModule->getTypeByName("struct.rphp::pVar");
    const StructType* StructTy_struct___si_class_type_info_pseudo = (const StructType*)llvmModule->getTypeByName("struct.__si_class_type_info_pseudo");
    const StructType* StructTy_struct___class_type_info_pseudo = (const StructType*)llvmModule->getTypeByName("struct.__class_type_info_pseudo");
    const StructType* StructTy_struct___type_info_pseudo = (const StructType*)llvmModule->getTypeByName("struct.__type_info_pseudo");
    const StructType* StructTy_struct_std__type_info = (const StructType*)llvmModule->getTypeByName("struct.std::type_info");
    PointerType* PointerTy_2 = PointerType::get(StructTy_struct_std__type_info, 0);

    ArrayType* ArrayTy_70 = ArrayType::get(PointerTy_4, 0);
    ArrayType* ArrayTy_71 = ArrayType::get(IntegerType::get(getGlobalContext(), 8), 23);

    ConstantInt* const_int32_637 = ConstantInt::get(getGlobalContext(), APInt(32,  StringRef("0"), 10));
    ConstantInt* const_int32_698 = ConstantInt::get(getGlobalContext(), APInt(32,  StringRef("112"), 10));
    ConstantInt* const_int32_649 = ConstantInt::get(getGlobalContext(), APInt(32,  StringRef("-1"), 10));
    ConstantInt* const_int32_643 = ConstantInt::get(getGlobalContext(), APInt(32,  StringRef("1"), 10));

    Function* func__Znwj = llvmModule->getFunction("_Znwj");
    Function* func__ZN4rphp14pRuntimeEngineC1EPNS_7pConfigE = llvmModule->getFunction("_ZN4rphp14pRuntimeEngineC1EPNS_7pConfigE");
    Function* func__ZN4rphp4pVarC1Ev = llvmModule->getFunction("_ZN4rphp4pVarC1Ev");
    Function* func__ZdlPv = llvmModule->getFunction("_ZdlPv");
    Function* func__ZN4rphp4pVaraSEi = llvmModule->getFunction("_ZN4rphp4pVaraSEi");
    Function* func__ZSt9terminatev = llvmModule->getFunction("_ZSt9terminatev");
    Function* func___cxa_begin_catch = llvmModule->getFunction("__cxa_begin_catch");
    Function* func___cxa_end_catch = llvmModule->getFunction("__cxa_end_catch");
    Function* func__ZN4rphp14pRuntimeEngineD1Ev = llvmModule->getFunction("_ZN4rphp14pRuntimeEngineD1Ev");
    Function* func__ZNK4rphp4pVar9copyAsIntEv = llvmModule->getFunction("_ZNK4rphp4pVar9copyAsIntEv");
    Function* func__ZN4rphp4pVarD1Ev = llvmModule->getFunction("_ZN4rphp4pVarD1Ev");
    Function* func__Unwind_Resume_or_Rethrow = llvmModule->getFunction("_Unwind_Resume_or_Rethrow");

    std::vector<const Type*>FuncTy_132_args;
    FunctionType* FuncTy_132 = FunctionType::get(
      /*Result=*/PointerTy_0,
      /*Params=*/FuncTy_132_args,
      /*isVarArg=*/false);

    std::vector<const Type*>FuncTy_134_args;
    FuncTy_134_args.push_back(PointerTy_0);
    FuncTy_134_args.push_back(PointerTy_0);
    FunctionType* FuncTy_134 = FunctionType::get(
      /*Result=*/IntegerType::get(getGlobalContext(), 32),
      /*Params=*/FuncTy_134_args,
      /*isVarArg=*/true);

    std::vector<const Type*>FuncTy_137_args;
    FuncTy_137_args.push_back(PointerTy_0);
    FunctionType* FuncTy_137 = FunctionType::get(
      /*Result=*/IntegerType::get(getGlobalContext(), 32),
      /*Params=*/FuncTy_137_args,
      /*isVarArg=*/false);

    Function* func_llvm_eh_exception = llvmModule->getFunction("llvm.eh.exception");

    Function* func_llvm_eh_selector_i32 = llvmModule->getFunction("llvm.eh.selector.i32");

    Function* func_llvm_eh_typeid_for_i32 = llvmModule->getFunction("llvm.eh.typeid.for.i32");

    Function* func___gxx_personality_v0 = llvmModule->getFunction("__gxx_personality_v0");

    GlobalVariable* gvar_struct__ZTIN4rphp13pRuntimeErrorE = llvmModule->getGlobalVariable("_ZTIN4rphp13pRuntimeErrorE");

    GlobalVariable* gvar_struct__ZTISt9exception = llvmModule->getGlobalVariable("_ZTISt9exception");

    GlobalVariable* gvar_array__ZTVN10__cxxabiv120__si_class_type_infoE = llvmModule->getGlobalVariable("_ZTVN10__cxxabiv120__si_class_type_infoE");

    GlobalVariable* gvar_array__ZTSN4rphp13pRuntimeErrorE = llvmModule->getGlobalVariable("_ZTSN4rphp13pRuntimeErrorE");

    GlobalVariable* gvar_struct__ZTISt13runtime_error = llvmModule->getGlobalVariable("_ZTISt13runtime_error");

    ConstantPointerNull* const_ptr_699 = ConstantPointerNull::get(PointerTy_39);
    ConstantPointerNull* const_ptr_700 = ConstantPointerNull::get(PointerTy_27);
    Constant* const_ptr_701 = ConstantExpr::getCast(Instruction::BitCast, gvar_struct__ZTIN4rphp13pRuntimeErrorE, PointerTy_0);
    Constant* const_ptr_702 = ConstantExpr::getCast(Instruction::BitCast, gvar_struct__ZTISt9exception, PointerTy_0);
    ConstantPointerNull* const_ptr_655 = ConstantPointerNull::get(PointerTy_0);
    Constant* const_ptr_654 = ConstantExpr::getCast(Instruction::BitCast, func___gxx_personality_v0, PointerTy_0);

    Function* func_mainScript = llvmModule->getFunction(entryFunctionName);

    // Function: main (func_main)
    {

      BasicBlock* label_entry_8714 = BasicBlock::Create(getGlobalContext(), "entry",func_main,0);
      BasicBlock* label_invcont_8715 = BasicBlock::Create(getGlobalContext(), "invcont",func_main,0);
      BasicBlock* label_invcont2_8716 = BasicBlock::Create(getGlobalContext(), "invcont2",func_main,0);
      BasicBlock* label_bb_8717 = BasicBlock::Create(getGlobalContext(), "bb",func_main,0);
      BasicBlock* label_bb3_8718 = BasicBlock::Create(getGlobalContext(), "bb3",func_main,0);
      BasicBlock* label_bb4_8719 = BasicBlock::Create(getGlobalContext(), "bb4",func_main,0);
      BasicBlock* label_invcont6 = BasicBlock::Create(getGlobalContext(), "invcont6",func_main,0);
      BasicBlock* label_invcont7_8720 = BasicBlock::Create(getGlobalContext(), "invcont7",func_main,0);
      BasicBlock* label_bb8_8721 = BasicBlock::Create(getGlobalContext(), "bb8",func_main,0);
      BasicBlock* label_invcont11 = BasicBlock::Create(getGlobalContext(), "invcont11",func_main,0);
      BasicBlock* label_bb12_8722 = BasicBlock::Create(getGlobalContext(), "bb12",func_main,0);
      BasicBlock* label_bb13_8723 = BasicBlock::Create(getGlobalContext(), "bb13",func_main,0);
      BasicBlock* label_bb14_8724 = BasicBlock::Create(getGlobalContext(), "bb14",func_main,0);
      BasicBlock* label_invcont16 = BasicBlock::Create(getGlobalContext(), "invcont16",func_main,0);
      BasicBlock* label_invcont17 = BasicBlock::Create(getGlobalContext(), "invcont17",func_main,0);
      BasicBlock* label_bb18_8725 = BasicBlock::Create(getGlobalContext(), "bb18",func_main,0);
      BasicBlock* label_invcont21 = BasicBlock::Create(getGlobalContext(), "invcont21",func_main,0);
      BasicBlock* label_bb22_8726 = BasicBlock::Create(getGlobalContext(), "bb22",func_main,0);
      BasicBlock* label_bb23_8727 = BasicBlock::Create(getGlobalContext(), "bb23",func_main,0);
      BasicBlock* label_bb24_8728 = BasicBlock::Create(getGlobalContext(), "bb24",func_main,0);
      BasicBlock* label_bb25 = BasicBlock::Create(getGlobalContext(), "bb25",func_main,0);
      BasicBlock* label_invcont26 = BasicBlock::Create(getGlobalContext(), "invcont26",func_main,0);
      BasicBlock* label_bb27 = BasicBlock::Create(getGlobalContext(), "bb27",func_main,0);
      BasicBlock* label_invcont28 = BasicBlock::Create(getGlobalContext(), "invcont28",func_main,0);
      BasicBlock* label_bb29 = BasicBlock::Create(getGlobalContext(), "bb29",func_main,0);
      BasicBlock* label_invcont32 = BasicBlock::Create(getGlobalContext(), "invcont32",func_main,0);
      BasicBlock* label_bb33 = BasicBlock::Create(getGlobalContext(), "bb33",func_main,0);
      BasicBlock* label_return_8729 = BasicBlock::Create(getGlobalContext(), "return",func_main,0);
      BasicBlock* label_lpad_8730 = BasicBlock::Create(getGlobalContext(), "lpad",func_main,0);
      BasicBlock* label_lpad37 = BasicBlock::Create(getGlobalContext(), "lpad37",func_main,0);
      BasicBlock* label_lpad41 = BasicBlock::Create(getGlobalContext(), "lpad41",func_main,0);
      BasicBlock* label_lpad45 = BasicBlock::Create(getGlobalContext(), "lpad45",func_main,0);
      BasicBlock* label_lpad49 = BasicBlock::Create(getGlobalContext(), "lpad49",func_main,0);
      BasicBlock* label_lpad53 = BasicBlock::Create(getGlobalContext(), "lpad53",func_main,0);
      BasicBlock* label_lpad57 = BasicBlock::Create(getGlobalContext(), "lpad57",func_main,0);
      BasicBlock* label_lpad61 = BasicBlock::Create(getGlobalContext(), "lpad61",func_main,0);
      BasicBlock* label_lpad65 = BasicBlock::Create(getGlobalContext(), "lpad65",func_main,0);
      BasicBlock* label_lpad69 = BasicBlock::Create(getGlobalContext(), "lpad69",func_main,0);
      BasicBlock* label_ppad_8731 = BasicBlock::Create(getGlobalContext(), "ppad",func_main,0);
      BasicBlock* label_ppad73 = BasicBlock::Create(getGlobalContext(), "ppad73",func_main,0);
      BasicBlock* label_ppad74 = BasicBlock::Create(getGlobalContext(), "ppad74",func_main,0);
      BasicBlock* label_ppad75 = BasicBlock::Create(getGlobalContext(), "ppad75",func_main,0);
      BasicBlock* label_ppad76 = BasicBlock::Create(getGlobalContext(), "ppad76",func_main,0);
      BasicBlock* label_ppad77 = BasicBlock::Create(getGlobalContext(), "ppad77",func_main,0);
      BasicBlock* label_nocatch = BasicBlock::Create(getGlobalContext(), "nocatch",func_main,0);
      BasicBlock* label_ppad79 = BasicBlock::Create(getGlobalContext(), "ppad79",func_main,0);
      BasicBlock* label_ppad80 = BasicBlock::Create(getGlobalContext(), "ppad80",func_main,0);
      BasicBlock* label_ppad81 = BasicBlock::Create(getGlobalContext(), "ppad81",func_main,0);
      BasicBlock* label_Unwind_8732 = BasicBlock::Create(getGlobalContext(), "Unwind",func_main,0);

      // Block entry (label_entry_8714)
      AllocaInst* ptr_retval_8733 = new AllocaInst(IntegerType::get(getGlobalContext(), 32), "retval", label_entry_8714);
      AllocaInst* ptr_save_filt_613 = new AllocaInst(IntegerType::get(getGlobalContext(), 32), "save_filt.613", label_entry_8714);
      AllocaInst* ptr_save_eptr_612 = new AllocaInst(PointerTy_0, "save_eptr.612", label_entry_8714);
      AllocaInst* ptr_save_filt_611 = new AllocaInst(IntegerType::get(getGlobalContext(), 32), "save_filt.611", label_entry_8714);
      AllocaInst* ptr_save_eptr_610 = new AllocaInst(PointerTy_0, "save_eptr.610", label_entry_8714);
      AllocaInst* ptr_save_filt_609 = new AllocaInst(IntegerType::get(getGlobalContext(), 32), "save_filt.609", label_entry_8714);
      AllocaInst* ptr_save_eptr_608 = new AllocaInst(PointerTy_0, "save_eptr.608", label_entry_8714);
      AllocaInst* ptr_save_filt_607 = new AllocaInst(IntegerType::get(getGlobalContext(), 32), "save_filt.607", label_entry_8714);
      AllocaInst* ptr_save_eptr_606 = new AllocaInst(PointerTy_0, "save_eptr.606", label_entry_8714);
      AllocaInst* ptr_e = new AllocaInst(PointerTy_594, "e", label_entry_8714);
      AllocaInst* ptr_e1 = new AllocaInst(PointerTy_596, "e1", label_entry_8714);
      AllocaInst* ptr_rVal = new AllocaInst(StructTy_struct_rphp__pVar, "rVal", label_entry_8714);
      ptr_rVal->setAlignment(4);
      AllocaInst* ptr_pEngine = new AllocaInst(PointerTy_27, "pEngine", label_entry_8714);
      AllocaInst* ptr_8734 = new AllocaInst(IntegerType::get(getGlobalContext(), 32), "", label_entry_8714);
      AllocaInst* ptr_pEngine_278 = new AllocaInst(PointerTy_27, "pEngine.278", label_entry_8714);
      AllocaInst* ptr_8735 = new AllocaInst(PointerTy_594, "", label_entry_8714);
      AllocaInst* ptr_8736 = new AllocaInst(PointerTy_596, "", label_entry_8714);
      AllocaInst* ptr_8737 = new AllocaInst(PointerTy_27, "", label_entry_8714);
      AllocaInst* ptr_eh_exception_8738 = new AllocaInst(PointerTy_0, "eh_exception", label_entry_8714);
      AllocaInst* ptr_eh_selector_8739 = new AllocaInst(IntegerType::get(getGlobalContext(), 32), "eh_selector", label_entry_8714);
      CastInst* int32_alloca_point_8740 = new BitCastInst(const_int32_637, IntegerType::get(getGlobalContext(), 32), "alloca point", label_entry_8714);
      CallInst* ptr_8741 = CallInst::Create(func__Znwj, const_int32_698, "", label_entry_8714);
      ptr_8741->setCallingConv(CallingConv::C);
      ptr_8741->setTailCall(false);AttrListPtr ptr_8741_PAL;
      ptr_8741->setAttributes(ptr_8741_PAL);

      CastInst* ptr_8742 = new BitCastInst(ptr_8741, PointerTy_27, "", label_entry_8714);
       new StoreInst(ptr_8742, ptr_8737, false, label_entry_8714);
      LoadInst* ptr_8744 = new LoadInst(ptr_8737, "", false, label_entry_8714);
      std::vector<Value*> void_8745_params;
      void_8745_params.push_back(ptr_8744);
      void_8745_params.push_back(const_ptr_699);
      InvokeInst *void_8745 = InvokeInst::Create(func__ZN4rphp14pRuntimeEngineC1EPNS_7pConfigE, label_invcont_8715, label_lpad_8730, void_8745_params.begin(), void_8745_params.end(), "", label_entry_8714);
      void_8745->setCallingConv(CallingConv::C);AttrListPtr void_8745_PAL;
      void_8745->setAttributes(void_8745_PAL);


      // Block invcont (label_invcont_8715)
      LoadInst* ptr_8746 = new LoadInst(ptr_8737, "", false, label_invcont_8715);
       new StoreInst(ptr_8746, ptr_pEngine, false, label_invcont_8715);
      CallInst* void_8748 = CallInst::Create(func__ZN4rphp4pVarC1Ev, ptr_rVal, "", label_invcont_8715);
      void_8748->setCallingConv(CallingConv::C);
      void_8748->setTailCall(false);AttrListPtr void_8748_PAL;
      void_8748->setAttributes(void_8748_PAL);

      LoadInst* ptr_8749 = new LoadInst(ptr_pEngine, "", false, label_invcont_8715);
      std::vector<Value*> void_8750_params;
      void_8750_params.push_back(ptr_rVal);
      void_8750_params.push_back(ptr_8749);
      InvokeInst *void_8750 = InvokeInst::Create(func_mainScript, label_invcont2_8716, label_lpad41, void_8750_params.begin(), void_8750_params.end(), "", label_invcont_8715);
      void_8750->setCallingConv(CallingConv::C);AttrListPtr void_8750_PAL;
      void_8750->setAttributes(void_8750_PAL);


      // Block invcont2 (label_invcont2_8716)
      BranchInst::Create(label_bb3_8718, label_invcont2_8716);

      // Block bb (label_bb_8717)
      LoadInst* int32_eh_select_8752 = new LoadInst(ptr_eh_selector_8739, "eh_select", false, label_bb_8717);
       new StoreInst(int32_eh_select_8752, ptr_save_filt_607, false, label_bb_8717);
      LoadInst* ptr_eh_value_8754 = new LoadInst(ptr_eh_exception_8738, "eh_value", false, label_bb_8717);
       new StoreInst(ptr_eh_value_8754, ptr_save_eptr_606, false, label_bb_8717);
      LoadInst* ptr_8756 = new LoadInst(ptr_8737, "", false, label_bb_8717);
      CastInst* ptr_8757 = new BitCastInst(ptr_8756, PointerTy_0, "", label_bb_8717);
      CallInst* void_8758 = CallInst::Create(func__ZdlPv, ptr_8757, "", label_bb_8717);
      void_8758->setCallingConv(CallingConv::C);
      void_8758->setTailCall(false);AttrListPtr void_8758_PAL;
      {
        SmallVector<AttributeWithIndex, 4> Attrs;
        AttributeWithIndex PAWI;
        PAWI.Index = 4294967295U; PAWI.Attrs = 0  | Attribute::NoUnwind;
        Attrs.push_back(PAWI);
        void_8758_PAL = AttrListPtr::get(Attrs.begin(), Attrs.end());

      }
      void_8758->setAttributes(void_8758_PAL);

      LoadInst* ptr_8759 = new LoadInst(ptr_save_eptr_606, "", false, label_bb_8717);
       new StoreInst(ptr_8759, ptr_eh_exception_8738, false, label_bb_8717);
      LoadInst* int32_8761 = new LoadInst(ptr_save_filt_607, "", false, label_bb_8717);
       new StoreInst(int32_8761, ptr_eh_selector_8739, false, label_bb_8717);
      BranchInst::Create(label_Unwind_8732, label_bb_8717);

      // Block bb3 (label_bb3_8718)
      BranchInst::Create(label_bb24_8728, label_bb3_8718);

      // Block bb4 (label_bb4_8719)
      LoadInst* ptr_eh_value5_8765 = new LoadInst(ptr_eh_exception_8738, "eh_value5", false, label_bb4_8719);
      CallInst* ptr_8766 = CallInst::Create(func___cxa_begin_catch, ptr_eh_value5_8765, "", label_bb4_8719);
      ptr_8766->setCallingConv(CallingConv::C);
      ptr_8766->setTailCall(false);AttrListPtr ptr_8766_PAL;
      {
        SmallVector<AttributeWithIndex, 4> Attrs;
        AttributeWithIndex PAWI;
        PAWI.Index = 4294967295U; PAWI.Attrs = 0  | Attribute::NoUnwind;
        Attrs.push_back(PAWI);
        ptr_8766_PAL = AttrListPtr::get(Attrs.begin(), Attrs.end());

      }
      ptr_8766->setAttributes(ptr_8766_PAL);

      CastInst* ptr_8767 = new BitCastInst(ptr_8766, PointerTy_596, "", label_bb4_8719);
       new StoreInst(ptr_8767, ptr_8736, false, label_bb4_8719);
      LoadInst* ptr_8769 = new LoadInst(ptr_8736, "", false, label_bb4_8719);
       new StoreInst(ptr_8769, ptr_e1, false, label_bb4_8719);
      std::vector<Value*> void_8771_params;
      void_8771_params.push_back(ptr_rVal);
      void_8771_params.push_back(const_int32_649);
      InvokeInst *void_8771 = InvokeInst::Create(func__ZN4rphp4pVaraSEi, label_invcont6, label_lpad49, void_8771_params.begin(), void_8771_params.end(), "", label_bb4_8719);
      void_8771->setCallingConv(CallingConv::C);AttrListPtr void_8771_PAL;
      void_8771->setAttributes(void_8771_PAL);


      // Block invcont6 (label_invcont6)
      std::vector<Value*> void_8772_params;
      InvokeInst *void_8772 = InvokeInst::Create(func___cxa_end_catch, label_invcont7_8720, label_lpad45, void_8772_params.begin(), void_8772_params.end(), "", label_invcont6);
      void_8772->setCallingConv(CallingConv::C);AttrListPtr void_8772_PAL;
      void_8772->setAttributes(void_8772_PAL);


      // Block invcont7 (label_invcont7_8720)
      BranchInst::Create(label_bb13_8723, label_invcont7_8720);

      // Block bb8 (label_bb8_8721)
      LoadInst* int32_eh_select9 = new LoadInst(ptr_eh_selector_8739, "eh_select9", false, label_bb8_8721);
       new StoreInst(int32_eh_select9, ptr_save_filt_609, false, label_bb8_8721);
      LoadInst* ptr_eh_value10 = new LoadInst(ptr_eh_exception_8738, "eh_value10", false, label_bb8_8721);
       new StoreInst(ptr_eh_value10, ptr_save_eptr_608, false, label_bb8_8721);
      std::vector<Value*> void_8776_params;
      InvokeInst *void_8776 = InvokeInst::Create(func___cxa_end_catch, label_invcont11, label_lpad53, void_8776_params.begin(), void_8776_params.end(), "", label_bb8_8721);
      void_8776->setCallingConv(CallingConv::C);AttrListPtr void_8776_PAL;
      void_8776->setAttributes(void_8776_PAL);


      // Block invcont11 (label_invcont11)
      LoadInst* ptr_8777 = new LoadInst(ptr_save_eptr_608, "", false, label_invcont11);
       new StoreInst(ptr_8777, ptr_eh_exception_8738, false, label_invcont11);
      LoadInst* int32_8779 = new LoadInst(ptr_save_filt_609, "", false, label_invcont11);
       new StoreInst(int32_8779, ptr_eh_selector_8739, false, label_invcont11);
      BranchInst::Create(label_ppad73, label_invcont11);

      // Block bb12 (label_bb12_8722)
      CallInst* void_8782 = CallInst::Create(func__ZSt9terminatev, "", label_bb12_8722);
      void_8782->setCallingConv(CallingConv::C);
      void_8782->setTailCall(false);AttrListPtr void_8782_PAL;
      {
        SmallVector<AttributeWithIndex, 4> Attrs;
        AttributeWithIndex PAWI;
        PAWI.Index = 4294967295U; PAWI.Attrs = 0  | Attribute::NoReturn | Attribute::NoUnwind;
        Attrs.push_back(PAWI);
        void_8782_PAL = AttrListPtr::get(Attrs.begin(), Attrs.end());

      }
      void_8782->setAttributes(void_8782_PAL);

      new UnreachableInst(getGlobalContext(), label_bb12_8722);

      // Block bb13 (label_bb13_8723)
      BranchInst::Create(label_bb24_8728, label_bb13_8723);

      // Block bb14 (label_bb14_8724)
      LoadInst* ptr_eh_value15 = new LoadInst(ptr_eh_exception_8738, "eh_value15", false, label_bb14_8724);
      CallInst* ptr_8785 = CallInst::Create(func___cxa_begin_catch, ptr_eh_value15, "", label_bb14_8724);
      ptr_8785->setCallingConv(CallingConv::C);
      ptr_8785->setTailCall(false);AttrListPtr ptr_8785_PAL;
      {
        SmallVector<AttributeWithIndex, 4> Attrs;
        AttributeWithIndex PAWI;
        PAWI.Index = 4294967295U; PAWI.Attrs = 0  | Attribute::NoUnwind;
        Attrs.push_back(PAWI);
        ptr_8785_PAL = AttrListPtr::get(Attrs.begin(), Attrs.end());

      }
      ptr_8785->setAttributes(ptr_8785_PAL);

      CastInst* ptr_8786 = new BitCastInst(ptr_8785, PointerTy_594, "", label_bb14_8724);
       new StoreInst(ptr_8786, ptr_8735, false, label_bb14_8724);
      LoadInst* ptr_8788 = new LoadInst(ptr_8735, "", false, label_bb14_8724);
       new StoreInst(ptr_8788, ptr_e, false, label_bb14_8724);
      std::vector<Value*> void_8790_params;
      void_8790_params.push_back(ptr_rVal);
      void_8790_params.push_back(const_int32_649);
      InvokeInst *void_8790 = InvokeInst::Create(func__ZN4rphp4pVaraSEi, label_invcont16, label_lpad61, void_8790_params.begin(), void_8790_params.end(), "", label_bb14_8724);
      void_8790->setCallingConv(CallingConv::C);AttrListPtr void_8790_PAL;
      void_8790->setAttributes(void_8790_PAL);


      // Block invcont16 (label_invcont16)
      std::vector<Value*> void_8791_params;
      InvokeInst *void_8791 = InvokeInst::Create(func___cxa_end_catch, label_invcont17, label_lpad57, void_8791_params.begin(), void_8791_params.end(), "", label_invcont16);
      void_8791->setCallingConv(CallingConv::C);AttrListPtr void_8791_PAL;
      void_8791->setAttributes(void_8791_PAL);


      // Block invcont17 (label_invcont17)
      BranchInst::Create(label_bb23_8727, label_invcont17);

      // Block bb18 (label_bb18_8725)
      LoadInst* int32_eh_select19_8793 = new LoadInst(ptr_eh_selector_8739, "eh_select19", false, label_bb18_8725);
       new StoreInst(int32_eh_select19_8793, ptr_save_filt_611, false, label_bb18_8725);
      LoadInst* ptr_eh_value20 = new LoadInst(ptr_eh_exception_8738, "eh_value20", false, label_bb18_8725);
       new StoreInst(ptr_eh_value20, ptr_save_eptr_610, false, label_bb18_8725);
      std::vector<Value*> void_8796_params;
      InvokeInst *void_8796 = InvokeInst::Create(func___cxa_end_catch, label_invcont21, label_lpad65, void_8796_params.begin(), void_8796_params.end(), "", label_bb18_8725);
      void_8796->setCallingConv(CallingConv::C);AttrListPtr void_8796_PAL;
      void_8796->setAttributes(void_8796_PAL);


      // Block invcont21 (label_invcont21)
      LoadInst* ptr_8797 = new LoadInst(ptr_save_eptr_610, "", false, label_invcont21);
       new StoreInst(ptr_8797, ptr_eh_exception_8738, false, label_invcont21);
      LoadInst* int32_8799 = new LoadInst(ptr_save_filt_611, "", false, label_invcont21);
       new StoreInst(int32_8799, ptr_eh_selector_8739, false, label_invcont21);
      BranchInst::Create(label_ppad73, label_invcont21);

      // Block bb22 (label_bb22_8726)
      CallInst* void_8802 = CallInst::Create(func__ZSt9terminatev, "", label_bb22_8726);
      void_8802->setCallingConv(CallingConv::C);
      void_8802->setTailCall(false);AttrListPtr void_8802_PAL;
      {
        SmallVector<AttributeWithIndex, 4> Attrs;
        AttributeWithIndex PAWI;
        PAWI.Index = 4294967295U; PAWI.Attrs = 0  | Attribute::NoReturn | Attribute::NoUnwind;
        Attrs.push_back(PAWI);
        void_8802_PAL = AttrListPtr::get(Attrs.begin(), Attrs.end());

      }
      void_8802->setAttributes(void_8802_PAL);

      new UnreachableInst(getGlobalContext(), label_bb22_8726);

      // Block bb23 (label_bb23_8727)
      BranchInst::Create(label_bb24_8728, label_bb23_8727);

      // Block bb24 (label_bb24_8728)
      LoadInst* ptr_8805 = new LoadInst(ptr_pEngine, "", false, label_bb24_8728);
       new StoreInst(ptr_8805, ptr_pEngine_278, false, label_bb24_8728);
      LoadInst* ptr_8807 = new LoadInst(ptr_pEngine_278, "", false, label_bb24_8728);
      ICmpInst* int1_8808 = new ICmpInst(*label_bb24_8728, ICmpInst::ICMP_NE, ptr_8807, const_ptr_700, "");
      BranchInst::Create(label_bb25, label_bb27, int1_8808, label_bb24_8728);

      // Block bb25 (label_bb25)
      LoadInst* ptr_8810 = new LoadInst(ptr_pEngine_278, "", false, label_bb25);
      std::vector<Value*> void_8811_params;
      void_8811_params.push_back(ptr_8810);
      InvokeInst *void_8811 = InvokeInst::Create(func__ZN4rphp14pRuntimeEngineD1Ev, label_invcont26, label_lpad37, void_8811_params.begin(), void_8811_params.end(), "", label_bb25);
      void_8811->setCallingConv(CallingConv::C);AttrListPtr void_8811_PAL;
      void_8811->setAttributes(void_8811_PAL);


      // Block invcont26 (label_invcont26)
      LoadInst* ptr_8812 = new LoadInst(ptr_pEngine_278, "", false, label_invcont26);
      CastInst* ptr_8813 = new BitCastInst(ptr_8812, PointerTy_0, "", label_invcont26);
      CallInst* void_8814 = CallInst::Create(func__ZdlPv, ptr_8813, "", label_invcont26);
      void_8814->setCallingConv(CallingConv::C);
      void_8814->setTailCall(false);AttrListPtr void_8814_PAL;
      {
        SmallVector<AttributeWithIndex, 4> Attrs;
        AttributeWithIndex PAWI;
        PAWI.Index = 4294967295U; PAWI.Attrs = 0  | Attribute::NoUnwind;
        Attrs.push_back(PAWI);
        void_8814_PAL = AttrListPtr::get(Attrs.begin(), Attrs.end());

      }
      void_8814->setAttributes(void_8814_PAL);

      BranchInst::Create(label_bb27, label_invcont26);

      // Block bb27 (label_bb27)
      std::vector<Value*> int32_8816_params;
      int32_8816_params.push_back(ptr_rVal);
      InvokeInst *int32_8816 = InvokeInst::Create(func__ZNK4rphp4pVar9copyAsIntEv, label_invcont28, label_lpad37, int32_8816_params.begin(), int32_8816_params.end(), "", label_bb27);
      int32_8816->setCallingConv(CallingConv::C);AttrListPtr int32_8816_PAL;
      int32_8816->setAttributes(int32_8816_PAL);


      // Block invcont28 (label_invcont28)
       new StoreInst(int32_8816, ptr_8734, false, label_invcont28);
      CallInst* void_8818 = CallInst::Create(func__ZN4rphp4pVarD1Ev, ptr_rVal, "", label_invcont28);
      void_8818->setCallingConv(CallingConv::C);
      void_8818->setTailCall(false);AttrListPtr void_8818_PAL;
      void_8818->setAttributes(void_8818_PAL);

      LoadInst* int32_8819 = new LoadInst(ptr_8734, "", false, label_invcont28);
       new StoreInst(int32_8819, ptr_retval_8733, false, label_invcont28);
      BranchInst::Create(label_return_8729, label_invcont28);

      // Block bb29 (label_bb29)
      LoadInst* int32_eh_select30 = new LoadInst(ptr_eh_selector_8739, "eh_select30", false, label_bb29);
       new StoreInst(int32_eh_select30, ptr_save_filt_613, false, label_bb29);
      LoadInst* ptr_eh_value31 = new LoadInst(ptr_eh_exception_8738, "eh_value31", false, label_bb29);
       new StoreInst(ptr_eh_value31, ptr_save_eptr_612, false, label_bb29);
      std::vector<Value*> void_8824_params;
      void_8824_params.push_back(ptr_rVal);
      InvokeInst *void_8824 = InvokeInst::Create(func__ZN4rphp4pVarD1Ev, label_invcont32, label_lpad69, void_8824_params.begin(), void_8824_params.end(), "", label_bb29);
      void_8824->setCallingConv(CallingConv::C);AttrListPtr void_8824_PAL;
      void_8824->setAttributes(void_8824_PAL);


      // Block invcont32 (label_invcont32)
      LoadInst* ptr_8825 = new LoadInst(ptr_save_eptr_612, "", false, label_invcont32);
       new StoreInst(ptr_8825, ptr_eh_exception_8738, false, label_invcont32);
      LoadInst* int32_8827 = new LoadInst(ptr_save_filt_613, "", false, label_invcont32);
       new StoreInst(int32_8827, ptr_eh_selector_8739, false, label_invcont32);
      BranchInst::Create(label_Unwind_8732, label_invcont32);

      // Block bb33 (label_bb33)
      CallInst* void_8830 = CallInst::Create(func__ZSt9terminatev, "", label_bb33);
      void_8830->setCallingConv(CallingConv::C);
      void_8830->setTailCall(false);AttrListPtr void_8830_PAL;
      {
        SmallVector<AttributeWithIndex, 4> Attrs;
        AttributeWithIndex PAWI;
        PAWI.Index = 4294967295U; PAWI.Attrs = 0  | Attribute::NoReturn | Attribute::NoUnwind;
        Attrs.push_back(PAWI);
        void_8830_PAL = AttrListPtr::get(Attrs.begin(), Attrs.end());

      }
      void_8830->setAttributes(void_8830_PAL);

      new UnreachableInst(getGlobalContext(), label_bb33);

      // Block return (label_return_8729)
      LoadInst* int32_retval34 = new LoadInst(ptr_retval_8733, "retval34", false, label_return_8729);
      ReturnInst::Create(getGlobalContext(), int32_retval34, label_return_8729);

      // Block lpad (label_lpad_8730)
      CallInst* ptr_eh_ptr_8833 = CallInst::Create(func_llvm_eh_exception, "eh_ptr", label_lpad_8730);
      ptr_eh_ptr_8833->setCallingConv(CallingConv::C);
      ptr_eh_ptr_8833->setTailCall(false);AttrListPtr ptr_eh_ptr_8833_PAL;
      ptr_eh_ptr_8833->setAttributes(ptr_eh_ptr_8833_PAL);

       new StoreInst(ptr_eh_ptr_8833, ptr_eh_exception_8738, false, label_lpad_8730);
      LoadInst* ptr_eh_ptr35 = new LoadInst(ptr_eh_exception_8738, "eh_ptr35", false, label_lpad_8730);
      std::vector<Value*> int32_eh_select36_params;
      int32_eh_select36_params.push_back(ptr_eh_ptr35);
      int32_eh_select36_params.push_back(const_ptr_654);
      int32_eh_select36_params.push_back(const_ptr_655);
      CallInst* int32_eh_select36 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select36_params.begin(), int32_eh_select36_params.end(), "eh_select36", label_lpad_8730);
      int32_eh_select36->setCallingConv(CallingConv::C);
      int32_eh_select36->setTailCall(false);AttrListPtr int32_eh_select36_PAL;
      int32_eh_select36->setAttributes(int32_eh_select36_PAL);

       new StoreInst(int32_eh_select36, ptr_eh_selector_8739, false, label_lpad_8730);
      BranchInst::Create(label_ppad_8731, label_lpad_8730);

      // Block lpad37 (label_lpad37)
      CallInst* ptr_eh_ptr38 = CallInst::Create(func_llvm_eh_exception, "eh_ptr38", label_lpad37);
      ptr_eh_ptr38->setCallingConv(CallingConv::C);
      ptr_eh_ptr38->setTailCall(false);AttrListPtr ptr_eh_ptr38_PAL;
      ptr_eh_ptr38->setAttributes(ptr_eh_ptr38_PAL);

       new StoreInst(ptr_eh_ptr38, ptr_eh_exception_8738, false, label_lpad37);
      LoadInst* ptr_eh_ptr39 = new LoadInst(ptr_eh_exception_8738, "eh_ptr39", false, label_lpad37);
      std::vector<Value*> int32_eh_select40_params;
      int32_eh_select40_params.push_back(ptr_eh_ptr39);
      int32_eh_select40_params.push_back(const_ptr_654);
      int32_eh_select40_params.push_back(const_ptr_655);
      CallInst* int32_eh_select40 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select40_params.begin(), int32_eh_select40_params.end(), "eh_select40", label_lpad37);
      int32_eh_select40->setCallingConv(CallingConv::C);
      int32_eh_select40->setTailCall(false);AttrListPtr int32_eh_select40_PAL;
      int32_eh_select40->setAttributes(int32_eh_select40_PAL);

       new StoreInst(int32_eh_select40, ptr_eh_selector_8739, false, label_lpad37);
      BranchInst::Create(label_ppad73, label_lpad37);

      // Block lpad41 (label_lpad41)
      CallInst* ptr_eh_ptr42 = CallInst::Create(func_llvm_eh_exception, "eh_ptr42", label_lpad41);
      ptr_eh_ptr42->setCallingConv(CallingConv::C);
      ptr_eh_ptr42->setTailCall(false);AttrListPtr ptr_eh_ptr42_PAL;
      ptr_eh_ptr42->setAttributes(ptr_eh_ptr42_PAL);

       new StoreInst(ptr_eh_ptr42, ptr_eh_exception_8738, false, label_lpad41);
      LoadInst* ptr_eh_ptr43 = new LoadInst(ptr_eh_exception_8738, "eh_ptr43", false, label_lpad41);
      std::vector<Value*> int32_eh_select44_params;
      int32_eh_select44_params.push_back(ptr_eh_ptr43);
      int32_eh_select44_params.push_back(const_ptr_654);
      int32_eh_select44_params.push_back(const_ptr_701);
      int32_eh_select44_params.push_back(const_ptr_702);
      int32_eh_select44_params.push_back(const_ptr_655);
      CallInst* int32_eh_select44 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select44_params.begin(), int32_eh_select44_params.end(), "eh_select44", label_lpad41);
      int32_eh_select44->setCallingConv(CallingConv::C);
      int32_eh_select44->setTailCall(false);AttrListPtr int32_eh_select44_PAL;
      int32_eh_select44->setAttributes(int32_eh_select44_PAL);

       new StoreInst(int32_eh_select44, ptr_eh_selector_8739, false, label_lpad41);
      BranchInst::Create(label_ppad74, label_lpad41);

      // Block lpad45 (label_lpad45)
      CallInst* ptr_eh_ptr46 = CallInst::Create(func_llvm_eh_exception, "eh_ptr46", label_lpad45);
      ptr_eh_ptr46->setCallingConv(CallingConv::C);
      ptr_eh_ptr46->setTailCall(false);AttrListPtr ptr_eh_ptr46_PAL;
      ptr_eh_ptr46->setAttributes(ptr_eh_ptr46_PAL);

       new StoreInst(ptr_eh_ptr46, ptr_eh_exception_8738, false, label_lpad45);
      LoadInst* ptr_eh_ptr47 = new LoadInst(ptr_eh_exception_8738, "eh_ptr47", false, label_lpad45);
      std::vector<Value*> int32_eh_select48_params;
      int32_eh_select48_params.push_back(ptr_eh_ptr47);
      int32_eh_select48_params.push_back(const_ptr_654);
      int32_eh_select48_params.push_back(const_ptr_655);
      CallInst* int32_eh_select48 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select48_params.begin(), int32_eh_select48_params.end(), "eh_select48", label_lpad45);
      int32_eh_select48->setCallingConv(CallingConv::C);
      int32_eh_select48->setTailCall(false);AttrListPtr int32_eh_select48_PAL;
      int32_eh_select48->setAttributes(int32_eh_select48_PAL);

       new StoreInst(int32_eh_select48, ptr_eh_selector_8739, false, label_lpad45);
      BranchInst::Create(label_ppad73, label_lpad45);

      // Block lpad49 (label_lpad49)
      CallInst* ptr_eh_ptr50 = CallInst::Create(func_llvm_eh_exception, "eh_ptr50", label_lpad49);
      ptr_eh_ptr50->setCallingConv(CallingConv::C);
      ptr_eh_ptr50->setTailCall(false);AttrListPtr ptr_eh_ptr50_PAL;
      ptr_eh_ptr50->setAttributes(ptr_eh_ptr50_PAL);

       new StoreInst(ptr_eh_ptr50, ptr_eh_exception_8738, false, label_lpad49);
      LoadInst* ptr_eh_ptr51 = new LoadInst(ptr_eh_exception_8738, "eh_ptr51", false, label_lpad49);
      std::vector<Value*> int32_eh_select52_params;
      int32_eh_select52_params.push_back(ptr_eh_ptr51);
      int32_eh_select52_params.push_back(const_ptr_654);
      int32_eh_select52_params.push_back(const_ptr_655);
      CallInst* int32_eh_select52 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select52_params.begin(), int32_eh_select52_params.end(), "eh_select52", label_lpad49);
      int32_eh_select52->setCallingConv(CallingConv::C);
      int32_eh_select52->setTailCall(false);AttrListPtr int32_eh_select52_PAL;
      int32_eh_select52->setAttributes(int32_eh_select52_PAL);

       new StoreInst(int32_eh_select52, ptr_eh_selector_8739, false, label_lpad49);
      BranchInst::Create(label_ppad75, label_lpad49);

      // Block lpad53 (label_lpad53)
      CallInst* ptr_eh_ptr54 = CallInst::Create(func_llvm_eh_exception, "eh_ptr54", label_lpad53);
      ptr_eh_ptr54->setCallingConv(CallingConv::C);
      ptr_eh_ptr54->setTailCall(false);AttrListPtr ptr_eh_ptr54_PAL;
      ptr_eh_ptr54->setAttributes(ptr_eh_ptr54_PAL);

       new StoreInst(ptr_eh_ptr54, ptr_eh_exception_8738, false, label_lpad53);
      LoadInst* ptr_eh_ptr55 = new LoadInst(ptr_eh_exception_8738, "eh_ptr55", false, label_lpad53);
      std::vector<Value*> int32_eh_select56_params;
      int32_eh_select56_params.push_back(ptr_eh_ptr55);
      int32_eh_select56_params.push_back(const_ptr_654);
      int32_eh_select56_params.push_back(const_int32_643);
      CallInst* int32_eh_select56 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select56_params.begin(), int32_eh_select56_params.end(), "eh_select56", label_lpad53);
      int32_eh_select56->setCallingConv(CallingConv::C);
      int32_eh_select56->setTailCall(false);AttrListPtr int32_eh_select56_PAL;
      int32_eh_select56->setAttributes(int32_eh_select56_PAL);

       new StoreInst(int32_eh_select56, ptr_eh_selector_8739, false, label_lpad53);
      BranchInst::Create(label_ppad76, label_lpad53);

      // Block lpad57 (label_lpad57)
      CallInst* ptr_eh_ptr58 = CallInst::Create(func_llvm_eh_exception, "eh_ptr58", label_lpad57);
      ptr_eh_ptr58->setCallingConv(CallingConv::C);
      ptr_eh_ptr58->setTailCall(false);AttrListPtr ptr_eh_ptr58_PAL;
      ptr_eh_ptr58->setAttributes(ptr_eh_ptr58_PAL);

       new StoreInst(ptr_eh_ptr58, ptr_eh_exception_8738, false, label_lpad57);
      LoadInst* ptr_eh_ptr59 = new LoadInst(ptr_eh_exception_8738, "eh_ptr59", false, label_lpad57);
      std::vector<Value*> int32_eh_select60_params;
      int32_eh_select60_params.push_back(ptr_eh_ptr59);
      int32_eh_select60_params.push_back(const_ptr_654);
      int32_eh_select60_params.push_back(const_ptr_655);
      CallInst* int32_eh_select60 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select60_params.begin(), int32_eh_select60_params.end(), "eh_select60", label_lpad57);
      int32_eh_select60->setCallingConv(CallingConv::C);
      int32_eh_select60->setTailCall(false);AttrListPtr int32_eh_select60_PAL;
      int32_eh_select60->setAttributes(int32_eh_select60_PAL);

       new StoreInst(int32_eh_select60, ptr_eh_selector_8739, false, label_lpad57);
      BranchInst::Create(label_ppad73, label_lpad57);

      // Block lpad61 (label_lpad61)
      CallInst* ptr_eh_ptr62 = CallInst::Create(func_llvm_eh_exception, "eh_ptr62", label_lpad61);
      ptr_eh_ptr62->setCallingConv(CallingConv::C);
      ptr_eh_ptr62->setTailCall(false);AttrListPtr ptr_eh_ptr62_PAL;
      ptr_eh_ptr62->setAttributes(ptr_eh_ptr62_PAL);

       new StoreInst(ptr_eh_ptr62, ptr_eh_exception_8738, false, label_lpad61);
      LoadInst* ptr_eh_ptr63 = new LoadInst(ptr_eh_exception_8738, "eh_ptr63", false, label_lpad61);
      std::vector<Value*> int32_eh_select64_params;
      int32_eh_select64_params.push_back(ptr_eh_ptr63);
      int32_eh_select64_params.push_back(const_ptr_654);
      int32_eh_select64_params.push_back(const_ptr_655);
      CallInst* int32_eh_select64 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select64_params.begin(), int32_eh_select64_params.end(), "eh_select64", label_lpad61);
      int32_eh_select64->setCallingConv(CallingConv::C);
      int32_eh_select64->setTailCall(false);AttrListPtr int32_eh_select64_PAL;
      int32_eh_select64->setAttributes(int32_eh_select64_PAL);

       new StoreInst(int32_eh_select64, ptr_eh_selector_8739, false, label_lpad61);
      BranchInst::Create(label_ppad79, label_lpad61);

      // Block lpad65 (label_lpad65)
      CallInst* ptr_eh_ptr66 = CallInst::Create(func_llvm_eh_exception, "eh_ptr66", label_lpad65);
      ptr_eh_ptr66->setCallingConv(CallingConv::C);
      ptr_eh_ptr66->setTailCall(false);AttrListPtr ptr_eh_ptr66_PAL;
      ptr_eh_ptr66->setAttributes(ptr_eh_ptr66_PAL);

       new StoreInst(ptr_eh_ptr66, ptr_eh_exception_8738, false, label_lpad65);
      LoadInst* ptr_eh_ptr67 = new LoadInst(ptr_eh_exception_8738, "eh_ptr67", false, label_lpad65);
      std::vector<Value*> int32_eh_select68_params;
      int32_eh_select68_params.push_back(ptr_eh_ptr67);
      int32_eh_select68_params.push_back(const_ptr_654);
      int32_eh_select68_params.push_back(const_int32_643);
      CallInst* int32_eh_select68 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select68_params.begin(), int32_eh_select68_params.end(), "eh_select68", label_lpad65);
      int32_eh_select68->setCallingConv(CallingConv::C);
      int32_eh_select68->setTailCall(false);AttrListPtr int32_eh_select68_PAL;
      int32_eh_select68->setAttributes(int32_eh_select68_PAL);

       new StoreInst(int32_eh_select68, ptr_eh_selector_8739, false, label_lpad65);
      BranchInst::Create(label_ppad80, label_lpad65);

      // Block lpad69 (label_lpad69)
      CallInst* ptr_eh_ptr70 = CallInst::Create(func_llvm_eh_exception, "eh_ptr70", label_lpad69);
      ptr_eh_ptr70->setCallingConv(CallingConv::C);
      ptr_eh_ptr70->setTailCall(false);AttrListPtr ptr_eh_ptr70_PAL;
      ptr_eh_ptr70->setAttributes(ptr_eh_ptr70_PAL);

       new StoreInst(ptr_eh_ptr70, ptr_eh_exception_8738, false, label_lpad69);
      LoadInst* ptr_eh_ptr71 = new LoadInst(ptr_eh_exception_8738, "eh_ptr71", false, label_lpad69);
      std::vector<Value*> int32_eh_select72_params;
      int32_eh_select72_params.push_back(ptr_eh_ptr71);
      int32_eh_select72_params.push_back(const_ptr_654);
      int32_eh_select72_params.push_back(const_int32_643);
      CallInst* int32_eh_select72 = CallInst::Create(func_llvm_eh_selector_i32, int32_eh_select72_params.begin(), int32_eh_select72_params.end(), "eh_select72", label_lpad69);
      int32_eh_select72->setCallingConv(CallingConv::C);
      int32_eh_select72->setTailCall(false);AttrListPtr int32_eh_select72_PAL;
      int32_eh_select72->setAttributes(int32_eh_select72_PAL);

       new StoreInst(int32_eh_select72, ptr_eh_selector_8739, false, label_lpad69);
      BranchInst::Create(label_ppad81, label_lpad69);

      // Block ppad (label_ppad_8731)
      BranchInst::Create(label_bb_8717, label_ppad_8731);

      // Block ppad73 (label_ppad73)
      BranchInst::Create(label_bb29, label_ppad73);

      // Block ppad74 (label_ppad74)
      CallInst* int32_eh_typeid = CallInst::Create(func_llvm_eh_typeid_for_i32, const_ptr_701, "eh_typeid", label_ppad74);
      int32_eh_typeid->setCallingConv(CallingConv::C);
      int32_eh_typeid->setTailCall(false);AttrListPtr int32_eh_typeid_PAL;
      int32_eh_typeid->setAttributes(int32_eh_typeid_PAL);

      LoadInst* int32_8866 = new LoadInst(ptr_eh_selector_8739, "", false, label_ppad74);
      ICmpInst* int1_8867 = new ICmpInst(*label_ppad74, ICmpInst::ICMP_EQ, int32_8866, int32_eh_typeid, "");
      BranchInst::Create(label_bb4_8719, label_ppad77, int1_8867, label_ppad74);

      // Block ppad75 (label_ppad75)
      BranchInst::Create(label_bb8_8721, label_ppad75);

      // Block ppad76 (label_ppad76)
      BranchInst::Create(label_bb12_8722, label_ppad76);

      // Block ppad77 (label_ppad77)
      CallInst* int32_eh_typeid78 = CallInst::Create(func_llvm_eh_typeid_for_i32, const_ptr_702, "eh_typeid78", label_ppad77);
      int32_eh_typeid78->setCallingConv(CallingConv::C);
      int32_eh_typeid78->setTailCall(false);AttrListPtr int32_eh_typeid78_PAL;
      int32_eh_typeid78->setAttributes(int32_eh_typeid78_PAL);

      LoadInst* int32_8871 = new LoadInst(ptr_eh_selector_8739, "", false, label_ppad77);
      ICmpInst* int1_8872 = new ICmpInst(*label_ppad77, ICmpInst::ICMP_EQ, int32_8871, int32_eh_typeid78, "");
      BranchInst::Create(label_bb14_8724, label_nocatch, int1_8872, label_ppad77);

      // Block nocatch (label_nocatch)
      BranchInst::Create(label_ppad73, label_nocatch);

      // Block ppad79 (label_ppad79)
      BranchInst::Create(label_bb18_8725, label_ppad79);

      // Block ppad80 (label_ppad80)
      BranchInst::Create(label_bb22_8726, label_ppad80);

      // Block ppad81 (label_ppad81)
      BranchInst::Create(label_bb33, label_ppad81);

      // Block Unwind (label_Unwind_8732)
      LoadInst* ptr_eh_ptr82 = new LoadInst(ptr_eh_exception_8738, "eh_ptr82", false, label_Unwind_8732);
      CallInst* void_8878 = CallInst::Create(func__Unwind_Resume_or_Rethrow, ptr_eh_ptr82, "", label_Unwind_8732);
      void_8878->setCallingConv(CallingConv::C);
      void_8878->setTailCall(false);AttrListPtr void_8878_PAL;
      void_8878->setAttributes(void_8878_PAL);

      new UnreachableInst(getGlobalContext(), label_Unwind_8732);

    }


/*
    pIRHelper irHelper(llvmModule);

    // void main(void)
    FunctionType *FT = FunctionType::get(Type::getVoidTy(getGlobalContext()), std::vector<const Type*>(),
                                        false);
    Function *main = Function::Create(FT, Function::ExternalLinkage, "main", llvmModule);
    IRBuilder<> block(getGlobalContext());
    block.SetInsertPoint(BasicBlock::Create(getGlobalContext(), "entry", main));

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
*/
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

