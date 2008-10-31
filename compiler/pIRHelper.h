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

#ifndef RPHP_PIRHELPER_H_
#define RPHP_PIRHELPER_H_

namespace llvm {
    class FunctionType;
    class Type;
    class Module;
}

namespace rphp {

class pIRHelper {

    llvm::FunctionType* moduleEntryFunTypeC_;
    llvm::Module* mod_;

public:

    pIRHelper(llvm::Module* m): moduleEntryFunTypeC_(0), mod_(m) { }

    // pointer to the runtime engine
    llvm::Type* runtimeEngineType(void);

    // entry function for a single compiled php module
    llvm::FunctionType* moduleEntryFunType(void);

    llvm::FunctionType* pVarBaseFunType();
    const llvm::Type* pVarType();
    llvm::Type* pVarPointerType();

};


} // namespace

#endif /* RPHP_PIRHELPER_H_ */
