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

#include "rphp/analysis/passes/Early_Lower_Loops.h"
#include "rphp/analysis/pSourceModule.h"

namespace rphp { namespace AST { namespace Pass {

/**
 * Transform
 *      while($cond) {
 *          doStuff();
 *      }
 * into
 *      while(true) {
 *          if($cond) {
 *              doStuff();
 *          }
 *          else
 *              break;
 *      }
 */
stmt* Early_Lower_Loops::transform_post_whileStmt(whileStmt* n) {
    if(literalBool* cond = dyn_cast<literalBool>(n->condition()))
            if(cond->getBoolVal() == true)
                return n;
    // break
    breakStmt* breakLoop = new (C_) breakStmt(NULL);

    // now the if
    ifStmt* ifStatement = new (C_) ifStmt(C_, n->condition()->retain(), n->body()->retain(), breakLoop);

    //the new while
    whileStmt* newWhile = new (C_) whileStmt(C_, h_.lTrue(), ifStatement);
    return newWhile;
}

/* Transform
 *      do { doStuff(); continue; }
 *      while ($cond)
 * into
 *      $firstRun = true;
 *      while (true)
 *      {
 *          if ($firstRun)
 *              $firstRun = false;
 *          else if (!$cond)
 *              break;
 *
 *          doStuff();
 *          continue;
 *      }
 *
 *  This is a little odd, but it accounts for the continue statement, which is
 *  difficult to handle otherwise.
 */
stmt* Early_Lower_Loops::transform_post_doStmt(doStmt* n) {
    // The $firstRun = true part.
    // Get the variable firstRun
    var* firstRun = h_.tempVar("firstRun");

    // Do the assignment $firstRun = true;
    assignment* firstRunTrueAssignment = new (C_) assignment(firstRun, h_.lTrue(), false);

    // The new while loop
    // Inside the while loop, we got else if(!$cond) getToOuterBlock(); else break;
    breakStmt* breakInLoop = new (C_) breakStmt(NULL);
    // Don't use n->body() here because we want to fall out of this block, else the loop transformation won't work
    // because for the first iteration, we wouldn't get into our loop body. This saves one logicalNot, btw ;)
    ifStmt* elseIf = new (C_) ifStmt(C_, n->condition()->retain(), NULL, breakInLoop);

    // The first if
    // $firstRun = false;
    assignment* firstRunFalseAssignement = new (C_) assignment(firstRun->clone(C_), h_.lFalse(), false);
    // Now the if ($firstRun)
    ifStmt* ifFirstRun = new (C_) ifStmt(C_, firstRun->clone(C_), firstRunFalseAssignement, elseIf);

    // The new while body then looks like this: ifStmt (which links to the elseIf); oldBody;
    statementList newWhileBodyStatements;
    newWhileBodyStatements.push_back(ifFirstRun);
    // TODO: Ideally this wouldn't be a block but something codegen wouldn't turn into a block
    newWhileBodyStatements.push_back(n->body()->retain());
    block* newWhileBody = new (C_) block(C_, &newWhileBodyStatements);

    // the new while(true) newWhileBody
    whileStmt* newWhile = new (C_) whileStmt(C_, h_.lTrue(), newWhileBody);

    statementList newWholeBlockStatements;
    newWholeBlockStatements.push_back(firstRunTrueAssignment);
    newWholeBlockStatements.push_back(newWhile);
    block* wholeBlock = new (C_) block(C_, &newWholeBlockStatements);

    return wholeBlock;
}

/* Transform
 *          for (i = 0; i < N; i++) { y(); continue; }
 * into
 *          i = 0;
 *          $first = true;
 *          while (true)
 *          {
 *              if ($first) $first = false;
 *              else $i++; // only performed after first iteration
 *
 *              if (i < N) ;
 *              else break;
 *
 *              y();
 *          }
 */
stmt* Early_Lower_Loops::transform_post_forStmt(forStmt* n) {
    statementList newWholeStatements;
    // put the init before the loop
    if(n->init())
        newWholeStatements.push_back(n->init()->retain());

    statementList newWhileBodyStatements;

    if(n->increment()) {
        var* firstRun = h_.tempVar("firstRun");
        assignment* firstRunTrueAssignement = new (C_) assignment(firstRun, h_.lTrue(), false);

        newWholeStatements.push_back(firstRunTrueAssignement);

        // The first if
        assignment* firstRunFalseAssignment = new (C_) assignment(firstRun->clone(C_), h_.lFalse(), false);

        ifStmt* ifFirstRunElseInc = new (C_) ifStmt(C_, firstRun->clone(C_), firstRunFalseAssignment, n->increment()->retain());
        newWhileBodyStatements.push_back(ifFirstRunElseInc);
    }

    // The 2nd if which ends the loop
    if(n->condition()) {
        // the condition could be a block, for example when your condition is $x != 4, $y != 3
        // This needs to be lowered into ($x != 4) && ($y != 3) which is an expression, not a stmt like now.
        // The final condition
        expr* cond = NULL;
        if(block* conditionBlock = dyn_cast<block>(n->condition()))
            cond = h_.chainExpressionsFromBlock(conditionBlock->children(), binaryOp::BOOLEAN_AND);
        else
            cond = cast<expr>(n->condition()->retain());
        
        breakStmt* breakInLoop = new (C_) breakStmt(NULL);
        ifStmt* ifCond = new (C_) ifStmt(C_, cond, NULL, breakInLoop);
        newWhileBodyStatements.push_back(ifCond);
    }
    newWhileBodyStatements.push_back(n->body()->retain());
    block* newWhileBody = new (C_) block(C_, &newWhileBodyStatements);
    whileStmt* newWhileLoop = new (C_) whileStmt(C_, h_.lTrue(), newWhileBody);

    newWholeStatements.push_back(newWhileLoop);
    block* newReplacementBlock = new (C_) block(C_, &newWholeStatements);
    return newReplacementBlock;
}

/**
 * Currently the AST node for forEach's is weird, so i can't implement this transformation.
 */
stmt* Early_Lower_Loops::transform_post_forEach(forEach* n) {
    assert(0 && "Foreach early_lower_loops is not implemented yet.");
    return n;
}


/* A switch statement is not an easy thing to replace, due to myriads of corner
 * cases, mostly involving fall-through.
 *
 * Break and continue are supported by wrapping the statements in a do {..}
 * while (0) loop. break $x is also supported by this.
 *
 * There can be multiple default statements. In this case, only the last one
 * can be matched. But other case statements can fall-through into a default
 * which doesnt match.
 *
 * Fall-through edges are tricky. Because there may be a break, and its
 * non-trivial (in the general case) to find out if theres a break, so we
 * always let code fall-through. If there is a break, it will catch. If not,
 * check that we have matched already.
 *
 * All blocks are added in order, so they may fall-through. However, we should
 * not evaluate the condition for a fall-through, as it may have side-effects.
 *
 * The last default block gets all subsequent code blocks added to it. This
 * leads to some code duplication, but we'll let the optimizer deal with it.
 * 
 * Convert
 *      switch (expr)
 *      {
 *          case expr1:
 *              x1 ();
 *
 *          default:
 *              xD1;
 *              break;
 *
 *          case expr2:
 *              x2 ();
 *              break;
 *          ...
 *          default: // only the last default counts
 *              xD2 ();
 *
 *          case expr3:
 *              x3 ();
 *      }
 *
 *  into
 *      val = expr;
 *      matched = false;
 *      do
 *      {
 *          // case expr1
 *          if (!matched)
 *          {
 *              val1 = expr1;
 *              if (val == val1)
 *                  matched = true;
 *          }
 *          if (matched)
 *              x1 ();
 *
 *
 *          // default - put the code for the default, in case of fall-through
 *          // (even though a later default overrides a possible match)
 *          if (matched)
 *              xD1 ();
 *
 *
 *          // case expr2
 *          if (!matched)
 *          {
 *              val2 = expr2;
 *              if (val == val2)
 *                  matched = true;
 *          }
 *          if (matched)
 *          {
 *              x2 ();
 *              break; // this will break the outer loop
 *          }
 *
 *
 *          // case expr3
 *          if (!matched)
 *          {
 *              val3 = expr3;
 *              if (val == val3)
 *                  matched = true;
 *          }
 *
 *          // default
 *      if (matched)
 *              xD1 ();
 *
 *          if (matched)
 *          {
 *              x3 ();
 *          }
 *
 *          // default with actual matching
 *          if (!matched)
 *          {
 *              matched = true;
 *          }
 *          xD ();
 *
 *          if (!matched)
 *          {
 *              xD2 ();
 *              x3 ();
 *          }
 *      } while (0)
 *
 *  Use a pre_switchStmt so that the do_while can be lowered in the post_do.
 */
stmt* Early_Lower_Loops::transform_pre_switchStmt(switchStmt* n) {
    statementList loweredStmts;
    
    // switch_val = expr
    var* switchVal = h_.tempVar("switch_val");
    assignment* switchValAssignment = new (C_) assignment(switchVal, n->rVal()->retain(), false /*ref*/);
    loweredStmts.push_back(switchValAssignment);
    
    // matched = false
    var* matchedVal = h_.tempVar("matched");
    assignment* matchedValInit = new (C_) assignment(matchedVal, h_.lFalse(), false /*ref*/);
    loweredStmts.push_back(matchedValInit);
    
    // All stmt*s inside the do... while(false) loop
    statementList loopStmts;
    
    foreach(stmt* caseStmtStmt, n->caseBlock()->children()) {
        assert(isa<switchCase>(caseStmtStmt) && "Statement inside of switch block isn't a case");
        switchCase* caseStmt = cast<switchCase>(caseStmtStmt);
        // TODO: is this legal php?
        assert(caseStmt->condition() && "Case has no expression");
        
        // Build the condition evaluating block
        // matched == false
        binaryOp* matchedFalseComparison = new (C_) binaryOp(matchedVal->clone(C_), h_.lFalse(), binaryOp::IDENTICAL);
        
        // Evaluate the condition of this case if we've not already matched a case.
        statementList evaluateConditionStmts;

        var* caseCondition = h_.tempVar("caseCondition");
        assignment* caseConditionInit = new (C_) assignment(caseCondition, caseStmt->condition()->retain(), false /*ref*/);
        evaluateConditionStmts.push_back(caseConditionInit);

        // if (caseCondition == switch_val) matched = true;
        binaryOp* caseConditionSwitchValComparison = new (C_) binaryOp(caseCondition->clone(C_), switchVal->clone(C_), binaryOp::EQUAL);
        assignment* matchedTrueAssign = new (C_) assignment(matchedVal->clone(C_), h_.lTrue(), false/*ref*/);
        ifStmt* ifCaseConditionMatches = new (C_) ifStmt(C_, caseConditionSwitchValComparison, matchedTrueAssign, NULL);
        evaluateConditionStmts.push_back(ifCaseConditionMatches);
        block* evaluateCondition = new (C_) block(C_, &evaluateConditionStmts);
        
        // put together the whole case expression evaluation
        ifStmt* ifNotMatchedEvaluateCondition = new (C_) ifStmt(C_, matchedFalseComparison, evaluateCondition, NULL);
        loopStmts.push_back(ifNotMatchedEvaluateCondition);
        
        // Build the actual doing block of the case in case we matched.
        ifStmt* ifMatchedDoStuff = new (C_) ifStmt(C_, matchedVal->clone(C_), caseStmt->body()->retain(), NULL);
        loopStmts.push_back(ifMatchedDoStuff);
    }
    // Build the do ... while(false) loop
    block* wrapperLoopBody = new (C_) block(C_, &loopStmts);
    doStmt* wrapperLoop = new (C_) doStmt(C_, h_.lFalse(), wrapperLoopBody);
    loweredStmts.push_back(wrapperLoop);
    
    block* switchReplacement = new (C_) block(C_, &loweredStmts);
    return switchReplacement;
}

} } } // namespace
