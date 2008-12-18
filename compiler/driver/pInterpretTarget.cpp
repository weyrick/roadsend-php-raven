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

#include <llvm/Linker.h>

#include "rphp/driver/pTargetError.h"
#include "rphp/driver/pInterpretTarget.h"
#include "rphp/analysis/pSourceModule.h"
#include "rphp/IR/pGenerator.h"
#include "rphp/IR/pGenSupport.h"
#include "rphp/JIT/pJIT.h"

namespace rphp {

void pInterpretTarget::execute(void) {

    pSourceModule  m(inputFile_);
    pGenerator codeGen(inputFile_);
    m.applyVisitor(&codeGen);

    llvm::Module* ir = codeGen.getIR();
    llvm::Module* stub = pGenSupport::createStandAloneStubModule("stub", codeGen.entryFunctionName());

    std::string errMsg;
    llvm::Linker l("stub_link", stub);
    l.LinkInModule(ir, &errMsg);
    if (errMsg.length()) {
        throw pTargetError("error linking in runtime IR [" + errMsg + "]");
    }

    // take ownership of module so it's not freed
    l.releaseModule();

    // JIT frees ir
    pJIT engine;
    engine.executeMain(stub);

}

} // namespace

