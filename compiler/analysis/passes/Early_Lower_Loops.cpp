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
    // TODO: Get a unique variable name here!
    // The $firstRun = true part.
    // Get the variable firstRun
    var* firstRun = new (C_) var("firstRun", C_);

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
        //TODO unique variable name!
        var* firstRun = new (C_) var("firstRun", C_);
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


} } } // namespace
