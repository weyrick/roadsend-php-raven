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

#include <llvm/Module.h>
#include <llvm/DerivedTypes.h>

#include <iostream>
#include <string>

#include "pIRHelper.h"

using namespace llvm;

namespace rphp {

Type* pIRHelper::runtimeEngineType() {
    return PointerType::get(mod->getTypeByName("struct.rphp::pRuntimeEngine"), 0);
}

Type* pIRHelper::pVarPointerType() {
    return PointerType::get(mod->getTypeByName("struct.rphp::pVar"), 0);
}

const Type* pIRHelper::pVarType() {
    return mod->getTypeByName("struct.rphp::pVar");
}

// void (*)(pVar*)
FunctionType* pIRHelper::pVarBaseFunType() {

    std::vector<const Type*>FuncTy_52_args;
    FuncTy_52_args.push_back(pVarPointerType());
    FunctionType* FuncTy_52 = FunctionType::get(
    /*Result=*/Type::VoidTy,
    /*Params=*/FuncTy_52_args,
    /*isVarArg=*/false);

    return FuncTy_52;
  
}

FunctionType* pIRHelper::moduleEntryFunType() {

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

