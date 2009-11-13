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

#include "rphp/IR/pCompileTarget.h"

#include "rphp/analysis/pSourceModule.h"
#include "rphp/IR/pGenerator.h"
#include "rphp/IR/pGenSupport.h"

#include <llvm/Module.h>

namespace rphp {

pCompileTarget::~pCompileTarget(void) {
    if (ownModule_)
        delete llvmModule_;
}

void pCompileTarget::execute(void) {

    std::string outputFile;

    log(E_COMPILE_DEBUG, "compiling module ["+inputFile_.fileName()+"] to IR");

    pSourceModule  m(inputFile_);

    IR::pGenerator codeGen(m, context_);
    llvmModule_ = codeGen.getIR();
    entryFunctionName_ = codeGen.entryFunctionName();

    if (createMain_) {
        IR::pGenSupport::createMain(llvmModule_, entryFunctionName_);
    }
    else {
        // TODO remove main from c-runtime base
    }

}

void pCompileTarget::writeToFile(std::string outputFile) {

    log(E_COMPILE_DEBUG, "saving IR from ["+inputFile_.fileName()+"] to output file ["+outputFile+"]");

    if (llvmModule_)
        IR::pGenSupport::writeBitcode(llvmModule_, outputFile);

}

} // namespace

