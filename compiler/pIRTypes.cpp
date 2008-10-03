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

#include <llvm/DerivedTypes.h>
#include "pIRTypes.h"

using namespace llvm;

namespace rphp {

Type* pIRTypes::runtimeEngineType() {
    return PointerType::get(Type::Int8Ty,0);
}

Type* pIRTypes::pVarPointerType() {
  
    return PointerType::get(pVarType(), 0);

}

Type* pIRTypes::pVarType() {

    std::vector<const Type*>StructTy_struct_boost__align__a4_fields;
    ArrayType* ArrayTy_1 = ArrayType::get(IntegerType::get(32), 1);

    StructTy_struct_boost__align__a4_fields.push_back(ArrayTy_1);
    StructType* StructTy_struct_boost__align__a4 = StructType::get(StructTy_struct_boost__align__a4_fields, /*isPacked=*/true);
    //mod->addTypeName("struct.boost::align::a4", StructTy_struct_boost__align__a4);

    std::vector<const Type*>StructTy_struct_boost__aligned_storage_8u_4u__fields;
    std::vector<const Type*>StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u__fields;
    std::vector<const Type*>StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u___data_t_fields;
    ArrayType* ArrayTy_2 = ArrayType::get(IntegerType::get(8), 8);

    StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u___data_t_fields.push_back(ArrayTy_2);
    StructType* StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u___data_t = StructType::get(StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u___data_t_fields, /*isPacked=*/false);
    //mod->addTypeName("struct.boost::detail::aligned_storage::aligned_storage_imp<8u,4u>::data_t", StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u___data_t);

    StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u__fields.push_back(StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u___data_t);
    StructType* StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u_ = StructType::get(StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u__fields, /*isPacked=*/true);
    //mod->addTypeName("struct.boost::detail::aligned_storage::aligned_storage_imp<8u,4u>", StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u_);

    StructTy_struct_boost__aligned_storage_8u_4u__fields.push_back(StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u_);
    StructType* StructTy_struct_boost__aligned_storage_8u_4u_ = StructType::get(StructTy_struct_boost__aligned_storage_8u_4u__fields, /*isPacked=*/true);
    //mod->addTypeName("struct.boost::aligned_storage<8u,4u>", StructTy_struct_boost__aligned_storage_8u_4u_);

    //mod->addTypeName("struct.boost::detail::aligned_storage::aligned_storage_imp<8u,4u>", StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u_);
    //mod->addTypeName("struct.boost::detail::aligned_storage::aligned_storage_imp<8u,4u>::data_t", StructTy_struct_boost__detail__aligned_storage__aligned_storage_imp_8u_4u___data_t);

    std::vector<const Type*> StructTy_struct_boost__shared_ptr_rphp__pVar__fields;
    std::vector<const Type*> StructTy_struct_rphp__pVar_fields;

    std::vector<const Type*> variant_fields;
    variant_fields.push_back(IntegerType::get(32)); // which
    variant_fields.push_back(StructTy_struct_boost__aligned_storage_8u_4u_); // data

    StructType* variant_struct = StructType::get(variant_fields, /*isPacked=*/true);
    //mod->addTypeName("struct.variant", variant_struct);

    StructTy_struct_rphp__pVar_fields.push_back(variant_struct);
    StructType* StructTy_struct_rphp__pVar = StructType::get(StructTy_struct_rphp__pVar_fields, /*isPacked=*/true);
    //mod->addTypeName("struct.pVar", StructTy_struct_rphp__pVar);

    return StructTy_struct_rphp__pVar;

}

// void (*)(pVar*)
FunctionType* pIRTypes::pVarBaseFunType() {

    std::vector<const Type*>FuncTy_52_args;
    FuncTy_52_args.push_back(pVarPointerType());
    FunctionType* FuncTy_52 = FunctionType::get(
    /*Result=*/Type::VoidTy,
    /*Params=*/FuncTy_52_args,
    /*isVarArg=*/false);

    return FuncTy_52;
  
}

FunctionType* pIRTypes::moduleEntryFunType() {

    if (moduleEntryFunTypeC)
        return moduleEntryFunTypeC;

    // entry function type: void (*)(pRuntimeEngine*)
    std::vector<const Type*> efArgs;
    efArgs.push_back(runtimeEngineType());
    moduleEntryFunTypeC = FunctionType::get(Type::VoidTy, /* return type */
                                           efArgs, /* arguments */
                                           false /*not vararg*/);

    return moduleEntryFunTypeC;

}


} // namespace

