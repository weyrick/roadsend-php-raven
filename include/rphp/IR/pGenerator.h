/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2010 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PGENERATOR_H_
#define RPHP_PGENERATOR_H_

#include "rphp/analysis/pSourceModule.h"
#include "rphp/analysis/pBaseVisitor.h"
#include "rphp/IR/pIRHelper.h"

#include <llvm/LLVMContext.h>
#include <string>

namespace llvm {
    class Module;
    class Function;
}

namespace rphp { namespace IR {

class pGenerator {

private:
    llvm::Module* llvmModule_; // won't free
    pSourceModule* sourceModule_;

    /// entry point of execution into this module
    llvm::Function* entryFunction_;
    /// startup procedures such as registering user defined functions and classes. called upon entry.
    llvm::Function* initFunction_;
    pIRHelper IRHelper_;

public:
    pGenerator(pSourceModule* mod, llvm::LLVMContext& c);

    void loadAndLinkRuntimeIR(void);
    void createEntryPoint(void);
    void runPasses();

    llvm::Module* getIR(void) {  return llvmModule_; }

    const pIdentString entryFunctionName(void) const;

};

} } // namespace

#endif /* RPHP_PGENERATOR_H_ */
