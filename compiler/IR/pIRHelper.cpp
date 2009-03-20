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

#include "rphp/IR/pIRHelper.h"

#include <llvm/Module.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Constants.h>

#include <unicode/schriter.h>
#include <unicode/unistr.h>
#include <unicode/ustring.h>

#include <iostream>

using namespace llvm;

namespace rphp { namespace IR {

Type* pIRHelper::runtimeEngineType() {
    return PointerType::get(mod_->getTypeByName("struct.rphp::pRuntimeEngine"), 0);
}

Type* pIRHelper::pVarPointerType() {
    return PointerType::get(mod_->getTypeByName("struct.rphp::pVar"), 0);
}

const Type* pIRHelper::pVarType() {
    return mod_->getTypeByName("struct.rphp::pVar");
}

// void (*)(pVar*)
FunctionType* pIRHelper::pVarBaseFunType() {

    std::vector<const Type*>args;
    args.push_back(pVarPointerType());
    FunctionType* funType = FunctionType::get(
    /*Result=*/Type::VoidTy,
    /*Params=*/args,
    /*isVarArg=*/false);

    return funType;

}

// pVar* (*)(pVar*, pRuntimeEngine*)
FunctionType* pIRHelper::pUserFunction0() {

    std::vector<const Type*>args;
    args.push_back(pVarPointerType()); // retval
    args.push_back(runtimeEngineType());
    FunctionType* funType = FunctionType::get(
    /*Result=*///pVarPointerType(),
    Type::VoidTy,
    /*Params=*/args,
    /*isVarArg=*/false);

    return funType;

}

// pVar* (*)(pVar*, pRuntimeEngine*, pVar*)
FunctionType* pIRHelper::pUserFunction1() {

    std::vector<const Type*>args;
    args.push_back(pVarPointerType()); // retval
    args.push_back(runtimeEngineType());
    args.push_back(pVarPointerType());
    FunctionType* funType = FunctionType::get(
    /*Result=*///pVarPointerType(),
    Type::VoidTy,
    /*Params=*/args,
    /*isVarArg=*/false);

    return funType;

}

// pVar* (*)(pVar*, pRuntimeEngine*, pVar*, pVar*)
FunctionType* pIRHelper::pUserFunction2() {

    std::vector<const Type*>args;
    args.push_back(pVarPointerType()); // retval
    args.push_back(runtimeEngineType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    FunctionType* funType = FunctionType::get(
    /*Result=*///pVarPointerType(),
    Type::VoidTy,
    /*Params=*/args,
    /*isVarArg=*/false);

    return funType;

}

// pVar* (*)(pVar*, pRuntimeEngine*, pVar*, pVar*, pVar*)
FunctionType* pIRHelper::pUserFunction3() {

    std::vector<const Type*>args;
    args.push_back(pVarPointerType()); // retval
    args.push_back(runtimeEngineType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    FunctionType* funType = FunctionType::get(
    /*Result=*///pVarPointerType(),
    Type::VoidTy,
    /*Params=*/args,
    /*isVarArg=*/false);

    return funType;

}

// pVar* (*)(pVar*, pRuntimeEngine*, pVar*, pVar*, pVar*, pVar*)
FunctionType* pIRHelper::pUserFunction4() {

    std::vector<const Type*>args;
    args.push_back(pVarPointerType()); // retval
    args.push_back(runtimeEngineType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    FunctionType* funType = FunctionType::get(
    /*Result=*///pVarPointerType(),
    Type::VoidTy,
    /*Params=*/args,
    /*isVarArg=*/false);

    return funType;

}

// pVar* (*)(pVar*, pRuntimeEngine*, pVar*, pVar*, pVar*, pVar*, pVar*)
FunctionType* pIRHelper::pUserFunction5() {

    std::vector<const Type*>args;
    args.push_back(pVarPointerType()); // retval
    args.push_back(runtimeEngineType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    args.push_back(pVarPointerType());
    FunctionType* funType = FunctionType::get(
    /*Result=*///pVarPointerType(),
    Type::VoidTy,
    /*Params=*/args,
    /*isVarArg=*/false);

    return funType;

}

// funsig is same as pUserFunction0
// TODO: extra params for env, args?
FunctionType* pIRHelper::moduleEntryFunType() {

    // entry function type: void (*)(pVar*, pRuntimeEngine*)
    std::vector<const Type*> efArgs;
    efArgs.push_back(pVarPointerType()); // retval
    efArgs.push_back(runtimeEngineType());
    return FunctionType::get(Type::VoidTy, /* return type */
                             efArgs, /* arguments */
                             false /*not vararg*/);

}

FunctionType* pIRHelper::moduleInitFunType() {

    // entry function type: void (*)(pRuntimeEngine*)
    std::vector<const Type*> efArgs;
    efArgs.push_back(runtimeEngineType());
    return FunctionType::get(Type::VoidTy, /* return type */
                             efArgs, /* arguments */
                             false /*not vararg*/);

}


llvm::Constant* pIRHelper::stringConstant(const std::string& s, int32_t& finalLen) {

    finalLen = s.length()+1;

    // global value creation
    ArrayType* byteArrayType = ArrayType::get(IntegerType::get(8), finalLen);
    GlobalVariable* gVarStr = new GlobalVariable(
                                    /*Type=*/byteArrayType,
                                    /*isConstant=*/true,
                                    /*Linkage=*/GlobalValue::InternalLinkage,
                                    /*Initializer=*/0, // has initializer, specified below
                                    /*Name=*/".bstr",
                                    mod_);

    // constant definition
    Constant* constArray = ConstantArray::get(s, true);
    gVarStr->setInitializer(constArray);

    // get pointer to global str
    std::vector<Constant*> indices;
    Constant* nullC = Constant::getNullValue(IntegerType::get(32));
    indices.push_back(nullC);
    indices.push_back(nullC);

    return ConstantExpr::getGetElementPtr(gVarStr, &indices[0], indices.size() );


}

llvm::Constant* pIRHelper::stringConstant(const std::wstring& s, int32_t& finalLen) {

    UErrorCode errorCode(U_ZERO_ERROR);
    int32_t destLength=0;
    u_strFromWCS(0, 0, &destLength, s.data(), s.length(), &errorCode);
    assert(!(U_FAILURE(errorCode) && errorCode != U_BUFFER_OVERFLOW_ERROR));
    errorCode = U_ZERO_ERROR;
    std::vector<UChar> buffer(destLength);
    u_strFromWCS(&buffer[0], buffer.size(), &destLength, s.data(), s.length(), &errorCode);
    assert(!U_FAILURE(errorCode));
    UnicodeString ucnv(&buffer[0], buffer.size());

    // byte representation
    std::string ustr;
    StringCharacterIterator iter(ucnv);
    for (UChar c = iter.first(); c != CharacterIterator::DONE; c = iter.next()) {
        // big endian. we convert the other way by specifying UTF-16BE
        // during construction of UnicodeString
        ustr.push_back(c & 0xFF00);
        ustr.push_back(c & 0x00FF);
    }
    finalLen = ustr.length();

    // global value creation
    ArrayType* byteArrayType = ArrayType::get(IntegerType::get(8), finalLen);
    GlobalVariable* gVarStr = new GlobalVariable(
                                    /*Type=*/byteArrayType,
                                    /*isConstant=*/true,
                                    /*Linkage=*/GlobalValue::InternalLinkage,
                                    /*Initializer=*/0, // has initializer, specified below
                                    /*Name=*/".ustr",
                                    mod_);

    // constant definition
    Constant* constArray = ConstantArray::get(ustr, false);
    gVarStr->setInitializer(constArray);

    // get pointer to global str
    std::vector<Constant*> indices;
    Constant* nullC = Constant::getNullValue(IntegerType::get(32));
    indices.push_back(nullC);
    indices.push_back(nullC);

    return ConstantExpr::getGetElementPtr(gVarStr, &indices[0], indices.size() );

}


} }  // namespace

