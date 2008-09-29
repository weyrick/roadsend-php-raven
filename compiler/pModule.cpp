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

#include "pASTVisitors.h"
#include "pModule.h"
#include "pASTVisitors.h"
#include "pGenerator.h"
#include "pGenSupport.h"

namespace rphp {

pModule::~pModule() {
    // free up statements
    for(astType::iterator s = ast.begin(); s != ast.end(); ++s) {
        delete *s;
    }
    // only if codegen was performed
    delete llvmModule;
}

void pModule::applyVisitor(AST::baseVisitor* v) {
    for(astType::iterator s = ast.begin(); s != ast.end(); ++s) {
        v->visit(*s);
    }
}

void pModule::dumpAST() {

    AST::dumpVisitor v;
    applyVisitor(&v);

}

void pModule::lowerToIR(pCompileTarget* target) {

    assert(llvmModule == NULL);

    llvmModule = new llvm::Module(fileName);
    pGenerator codeGen(llvmModule, target);
    applyVisitor(&codeGen);
    codeGen.finalize();

}

void pModule::writeBitcode(std::string fileName) {

    pGenSupport::writeBitcode(llvmModule, fileName);

}

std::string pModule::getEntryFunctionName() {
    return pGenSupport::mangleModuleName(fileName);
}

} // namespace
