/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2010 Cornelius Riemenschneider <c.r1@gmx.de>
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

#include "rphp/analysis/passes/MainFunction.h"
#include "rphp/analysis/pSourceModule.h"

namespace rphp { namespace AST { namespace Pass {

/**
 * Moves all global code except of function and class declarations into a __MAIN__ function.
 * __MAIN__ will later hold the global variables as well.
 */
stmt* MainFunction::transform_pre_block(block *n) {
    // Just run on the first block.
    if(firstBlock)
        return n;
    firstBlock = true;
    statementList mainFunction;
    statementList globalScope;
    foreach(stmt* child, n->children()) {
        if(isa<functionDecl>(child) || isa<classDecl>(child))
            globalScope.push_back(child->retain());
        else
            mainFunction.push_back(child->retain());
    }
    
    block* mainFunctionBody = new (C_) block(C_, &mainFunction);
    signature* mainFunctionSig = new (C_) signature("__MAIN__", C_, NULL);
    functionDecl* mainFunctionDecl = new (C_) functionDecl(mainFunctionSig, mainFunctionBody);
    globalScope.push_back(mainFunctionDecl);
    return new (C_) block(C_, &globalScope);
}

} } } // namespace
