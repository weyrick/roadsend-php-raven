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

#include "rphp/analysis/passes/Split_Builtins.h"
#include "rphp/analysis/pSourceModule.h"

namespace rphp { namespace AST { namespace Pass {

/**
 * Transform
 * 			unset($a, $b, $c);
 * into
 * 			unset($a);
 * 			unset($b);
 * 			unset($c);
 *
 * The same transformation is used for echos.
 *
 * Transform
 * 			isset($a, $b, $c)
 * into
 * 			isset($a) && isset($b) && isset($c)
 */
expr* Split_Builtins::transform_post_builtin(builtin* n) {
    if(n->opKind() != builtin::ISSET && n->opKind() != builtin::UNSET && n->opKind() != builtin::ECHO)
        return n;
    // Without arguments, there's nothing to split.
    if(n->numArgs() <= 1)
        return n;
    // This turns one builtin with multiple arguments into a list of the corresponding builtin with only one argument.
    expressionList builtins;

    foreach(stmt* arg, n->children()) {
        //TODO: make this shorter by adding another constructor to builtin.
        expressionList argument;
        argument.push_back(cast<expr>(arg->clone(C_)));
        builtins.push_back(new (C_) builtin(C_, n->opKind(), &argument));
    }
    switch(n->opKind()) {
        case builtin::ECHO:
        case builtin::UNSET:
            return new (C_) exprReduce(C_, &builtins);
        case builtin::ISSET:
            return h_.chainExpressions(&builtins, binaryOp::BOOLEAN_AND);
        default:
            assert(0 && "unreachable!");
            break;
    }
    assert(0 && "unreachable!");
    return NULL;
}


} } } // namespace
