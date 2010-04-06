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

#include "rphp/analysis/passes/Lower_Control_Flow.h"
#include "rphp/analysis/pSourceModule.h"

namespace rphp { namespace AST { namespace Pass {

/**
 * TODO: foreach lowering, dynamic break/continues.
 */

/**
 * Foreach lowering is not implemented yet, thus reject foreachs.
 */
stmt* Lower_Control_Flow::transform_pre_forEach(forEach* n) {
    assert(0 && "Foreach lowering is not implemented yet, sorry.");
    return n;
}

/* Transform
 *          if ($x) { y (); }
 *          else { z (); }
 * into
 *          if ($x) goto L1;
 *          else goto L2;
 *      L1:
 *          y ();
 *          goto L3;
 *      L2:
 *          z ();
 *          goto L3;
 *      L3:
 *
 * We only need two labels, since the first edge can be an implict
 * fall-through edge if we negate the condition, but this is more
 * readable and understandable, as it keeps the structure of the original
 * if-else statement. */
stmt* Lower_Control_Flow::transform_post_ifStmt(ifStmt* n) {
    label* L1 = currentFunction_->getNewLabel(C_);
    label* L2 = currentFunction_->getNewLabel(C_);
    label* L3 = currentFunction_->getNewLabel(C_);
    
    branch* br = new (C_) branch(n->condition()->retain(), L1, L2);
    statementList stmts;
    stmts.push_back(br);
    stmts.push_back(L1);
    if(n->trueBlock())
        stmts.push_back(n->trueBlock()->retain());
    stmts.push_back(new (C_) branch(L3));
    stmts.push_back(L2);
    if(n->falseBlock())
        stmts.push_back(n->falseBlock()->retain());
    stmts.push_back(new (C_) branch(L3));
    stmts.push_back(L3);
    
    return new (C_) block(C_, &stmts);
}

/**
 * Remove Lbreak and Lcontinue from the respective stacks.
 */
stmt* Lower_Control_Flow::transform_post_whileStmt(whileStmt* n) {
    breakLabels_.pop_back();
    continueLabels_.pop_back();
    return n;
}

/* Transform
 *          while ($x) { y (); }
 * into
 *      Lcontinue:
 *          y ();
 *          goto Lcontinue
 *      Lbreak:
 *
 * Lcontinue is the label used if you use continue in the loop, Lbreak is the label used if you use break in the loop.
 */
stmt* Lower_Control_Flow::transform_pre_whileStmt(whileStmt* n) {
    assert(cast<literalBool>(n->condition())->getBoolVal() == true && "non-lowered while loop found!");
    
    statementList stmts;
    
    // continue label
    label* Lcontinue = currentFunction_->getNewLabel(C_);
    continueLabels_.push_back(Lcontinue);
    
    stmts.push_back(Lcontinue);
    stmts.push_back(n->body()->retain());
    stmts.push_back(new (C_) branch(Lcontinue));

    label* Lbreak = currentFunction_->getNewLabel(C_);
    breakLabels_.push_back(Lbreak);
    stmts.push_back(Lbreak);

    return new (C_) block(C_, &stmts);
}

/**
 * This function lowers an exit (continue/break) from a loop and generates a branch to the appropriate label.
 * Currently dynamic continue/breaks aren't supported.
 */
stmt* Lower_Control_Flow::lower_exit(std::vector<label*>& labels, expr* depthExpr) {
    pInt depth = -1;
    if(depthExpr == NULL)
        depth = 0;
    else {
        //TODO: what's about literalStrings? normally they evaluate to 0, but to what evaluates '1'?
        assert(!isa<literalString>(depthExpr));
        if(literalInt* depthInt = dyn_cast<literalInt>(depthExpr)) {
            // This possibly throws an exception if int>2^31-1, but that's okay, we don't support breaking on that values anyways :D
            depth = depthInt->getInt();
            //TODO: error/warning on depth < 1
            // negative/zero break is always 1
            if(depth < 1)
                depth = 1;
        }
        //TODO: Dynamic depth
    }
    if(depth == 0)
        depth = 1;
    if(depth != -1)
        //TODO: replace by a better error!
        assert(depth <= (pInt)labels.size() && "you're trying to break too much!");
    assert(depth != -1 && "dynamic depth currently not supported!");
    return new (C_) branch(labels[labels.size() - depth]);
}

stmt* Lower_Control_Flow::transform_post_breakStmt(breakStmt* n) {
    return lower_exit(breakLabels_, n->rVal());
}

stmt* Lower_Control_Flow::transform_post_continueStmt(continueStmt* n) {
    return lower_exit(continueLabels_, n->rVal());
}

// This is needed to get the current function.
stmt* Lower_Control_Flow::transform_pre_functionDecl(functionDecl* n) {
    currentFunction_ = n;
    return n;
}


} } } // namespace
