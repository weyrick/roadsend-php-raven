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

#ifndef RPHP_PIRHELPER_H_
#define RPHP_PIRHELPER_H_

#include <llvm/ADT/StringRef.h>
#include <unicode/utypes.h>
#include <string>

namespace llvm {
    class FunctionType;
    class Type;
    class Module;
    class Constant;
}

namespace rphp { namespace IR {

/**

    convenience functions for working with IR. it requires a module
    containing c-runtime IR code

 */
class pIRHelper {

    llvm::Module* mod_;

public:

    pIRHelper(llvm::Module* m): mod_(m) { }

    // pointer to the runtime engine
    llvm::Type* runtimeEngineType(void);

    llvm::FunctionType* moduleEntryFunType();
    llvm::FunctionType* moduleInitFunType();
    llvm::FunctionType* pVarBaseFunType();
    
    llvm::Type* pVarType();
    llvm::Type* pVarPointerType();

    llvm::FunctionType* pUserFunction0();
    llvm::FunctionType* pUserFunction1();
    llvm::FunctionType* pUserFunction2();
    llvm::FunctionType* pUserFunction3();
    llvm::FunctionType* pUserFunction4();
    llvm::FunctionType* pUserFunction5();
    
    llvm::Constant* byteArrayConstant(const llvm::StringRef& s);

};


} } // namespace

#endif /* RPHP_PIRHELPER_H_ */
