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

#ifndef RPHP_PGENSUPPORT_H_
#define RPHP_PGENSUPPORT_H_

#include "rphp/pTypes.h"

#include <string>
#include <llvm/LLVMContext.h>

namespace llvm {
    class Module;
}

namespace rphp { namespace IR {

class pGenSupport {

public:
    static std::string mangleEntryFunctionName(std::string moduleName);
    static std::string mangleInitFunctionName(std::string moduleName);
    static std::string mangleUserFunctionName(std::string moduleName, std::string inName);
    static void writeBitcode(llvm::Module* m, std::string outFile);
    static llvm::Module* readBitcode(std::string fileName, llvm::LLVMContext& c);
    static llvm::Module* getRuntimeIR(llvm::LLVMContext& c);
    static void createMain(llvm::Module *m, const pIdentString& entryFunctionName);
    static void dumpIR(llvm::Module* llvmModule);

};

} } // namespace

#endif /* RPHP_PGENSUPPORT_H_ */
