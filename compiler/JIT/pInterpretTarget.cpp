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


#include "rphp/JIT/pInterpretTarget.h"

#include "rphp/analysis/pSourceModule.h"
#include "rphp/IR/pGenerator.h"
#include "rphp/JIT/pJIT.h"

#include <llvm/Linker.h>

namespace rphp {

void pInterpretTarget::execute(void) {

    pSourceModule  m(inputFile_);
    IR::pGenerator codeGen(m);

    llvm::Module* ir = codeGen.getIR();
    
    // JIT frees ir
    pJIT engine;
    engine.executeWithRuntime(ir, codeGen.entryFunctionName());

}

} // namespace

