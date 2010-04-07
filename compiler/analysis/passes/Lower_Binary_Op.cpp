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
;;
;; Code derived from:
;; phc -- the open source PHP compiler
;; Copyright (c) 2005-2007, Edsko de Vries, John Gilbert and Paul Biggar.
;; All rights reserved.
;; Originally distributed under the BSD license
;;
 ***** END LICENSE BLOCK *****
 */

#include "rphp/analysis/passes/Lower_Binary_Op.h"
#include "rphp/analysis/pSourceModule.h"

/**
 * This file lowers binaryOp::AND, binaryOp::OR and binaryOp::XOR into simpler constructs.
 * This also replaces >= and > with <= or <.
 */

namespace rphp { namespace AST { namespace Pass {

/**
 * Transform
 *      $a && $b
 * into
 *      ($a ? (bool)$b : false)
 *
 * Transform
 *      $a || $b
 * into
 *      ($a ? true : (bool)$b)
 *
 * Transform
 *      $a xor $b
 * into
 *      ((($a && !$b) || ($b && !$a)) ? true : false)

 * This is in turn then feeded to the transforms above.
 * The conditionalExpr's generated are then lowered by the Lower_Conditional_Expr pass.
 *
 * Transform
 *      $a >= $b
 * into
 *      $b <= $a
 *
 * Transform
 *      $a > $b
 * into
 *      $b < $a
 */
expr* Lower_Binary_Op::transform_pre_binaryOp(binaryOp* n) {
    switch(n->opKind()) {
        case binaryOp::BOOLEAN_AND: {
            // cast rVal to bool
            typeCast* boolCast = new (C_) typeCast(typeCast::BOOL, n->rVal()->retain());
            return new (C_) conditionalExpr(n->lVal()->retain(), boolCast, h_.lFalse());
        }
        case binaryOp::BOOLEAN_OR: {
            // cast rVal to bool
            typeCast* boolCast = new (C_) typeCast(typeCast::BOOL, n->rVal()->retain());
            return new (C_) conditionalExpr(n->lVal()->retain(), h_.lTrue(), boolCast);
        }
        case binaryOp::BOOLEAN_XOR: {
            // $a && !$b
            // negated rVal
            unaryOp* negatedRVal = new (C_) unaryOp(n->rVal()->retain(), unaryOp::LOGICALNOT);
            // The left part of the overall condition
            binaryOp* leftCondition = new (C_) binaryOp(n->lVal()->retain(), negatedRVal, binaryOp::BOOLEAN_AND);

            // $b && !$a
            // Attention: we can't use retain here because we reuse the existing node already above!
            unaryOp* negatedLVal = new (C_) unaryOp(n->lVal()->clone(C_), unaryOp::LOGICALNOT);
            // The right part of the overall condition
            binaryOp* rightCondition = new (C_) binaryOp(n->rVal()->clone(C_), negatedLVal, binaryOp::BOOLEAN_AND);

            // The whole condition
            binaryOp* condition = new (C_) binaryOp(leftCondition, rightCondition, binaryOp::BOOLEAN_OR);
            return new (C_) conditionalExpr(condition, h_.lTrue(), h_.lFalse());
        }
        case binaryOp::GREATER_OR_EQUAL:
        case binaryOp::GREATER_THAN:
            //TODO: file+line numbers?
            return new (C_) binaryOp(n->rVal()->retain(),
                             n->lVal()->retain(),
                             (n->opKind() == binaryOp::GREATER_THAN) ? binaryOp::LESS_THAN : binaryOp::LESS_OR_EQUAL);
        default:
            return n;
    }
    assert(0 && "unreachable!");
    return n;
}

} } } // namespace
