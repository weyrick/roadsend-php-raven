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

#include "rphp/analysis/passes/Lower_Conditional_Expr.h"
#include "rphp/analysis/pSourceModule.h"

/**
 * This file lowers conditionalExpr into simpler constructs.
 */

namespace rphp { namespace AST { namespace Pass {

/**
 * Transform
 *      ($cond ? $trueexpr : $falseexpr)
 * into an exprReduce containing
 *      if($cond)
 *          $ret = $trueexpr;
 *      else
 *          $ret = $falseexpr;
 *      $ret;
 */
expr* Lower_Conditional_Expr::transform_post_conditionalExpr(conditionalExpr* n) {
    // TODO: unique variable name!
    var* retVar = h_.tempVar("ret");

    // trueexpr assignment
    assignment* trueAssign = new (C_) assignment(retVar, n->trueExpr()->retain(), false);

    // falseexpr assignment
    assignment* falseAssign = new (C_) assignment(retVar->clone(C_), n->falseExpr()->retain(), false);

    // ifStmt
    ifStmt* ifCond = new (C_) ifStmt(C_, n->condition()->retain(), trueAssign, falseAssign);
    statementList statements;
    statements.push_back(ifCond);
    statements.push_back(retVar->clone(C_));

    return new (C_) exprReduce(C_, &statements);
}



} } } // namespace
