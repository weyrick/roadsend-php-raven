/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009-2010 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PDECLARE_H_
#define RPHP_PDECLARE_H_

#include "rphp/analysis/pBaseVisitor.h"
#include "rphp/IR/pIRHelper.h"

namespace llvm {
    class Module;
    class Function;
}

namespace rphp { namespace IR {

class pDeclare: public AST::pBaseVisitor {

private:
    llvm::Module* llvmModule_; // don't own
    llvm::Function* initFunction_; // don't own
    pIRHelper IRHelper_;

public:

    pDeclare(pSourceModule* sm, llvm::Module* mod, llvm::Function* initFun):
            pBaseVisitor("IR declare","Generate IR for top level declarations", sm),
            llvmModule_(mod),
            initFunction_(initFun),
            IRHelper_(mod) { }

    // nodes
    void visit_pre_functionDecl(AST::functionDecl* n);

};

} } // namespace

#endif /* RPHP_PDECLARE_H_ */
