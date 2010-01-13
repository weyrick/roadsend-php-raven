/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PTRANSFORMHELPER_H_
#define RPHP_PTRANSFORMHELPER_H_

#include "rphp/analysis/pAST.h"
#include "rphp/analysis/pParseContext.h"

namespace rphp { namespace AST {
class pTransformHelper {
    pParseContext& C;

    literalBool* literalTrue_;
    literalBool* literalFalse_;

public:
    pTransformHelper(pParseContext& C): C(C), literalTrue_(NULL), literalFalse_(NULL)
        {

        }

    // lTrue and lFalse provide an easy way to get a literal true or false in the code.
    literalBool* lTrue() {
        if(literalTrue_) {
            literalTrue_->retain();
        }
        else
            literalTrue_ = new (C) literalBool(true);
        return literalTrue_;
    }
    literalBool* lFalse() {
        if(literalFalse_) {
            literalFalse_->retain();
        }
        else
            literalFalse_ = new (C) literalBool(false);
        return literalFalse_;
    }
    // This method gets several expr*s which are chained together in a list of binaryOp*s.
    // You normally want to use this with binaryOp::BOOLEAN_AND
    // retainExprs allows you do safely delete the source of the expressionList/statementList, for example a block.
    expr* chainExpressions(expressionList* expressions, enum binaryOp::opKind operationType, bool retainExprs = false) {
        assert(!expressions->empty());
        assert(expressions->size() > 1);

        expressionList::iterator it = expressions->begin();
        expr* prevExpr = *it++;
        if(retainExprs)
            prevExpr->retain();
        for(expressionList::iterator end = expressions->end(); it != end; ++it)
            if (retainExprs)
                (*it)->retain();
            prevExpr = new (C) binaryOp(prevExpr, *it, operationType);

        return prevExpr;
    }
    // That function is currently a slightly modified version of the one above with the whole code duplicated. Very bad.
    expr* chainExpressionsFromBlock(stmt::child_range statements, enum binaryOp::opKind operationType, bool retainExprs = false) {
        assert(!statements.empty());
        assert(++statements.begin() != statements.end() && " To chain expressions, you need at least two of them!");

        stmt::child_iterator it = statements.begin();
        expr* prevExpr = cast<expr>(*it++);
        if(retainExprs)
            prevExpr->retain();
        for(stmt::child_iterator end = statements.end(); it != end; ++it) {
            if (retainExprs)
                (*it)->retain();
            prevExpr = new (C) binaryOp(prevExpr, cast<expr>(*it), operationType);
        }
        return prevExpr;
    }
};


} } // namespace

#endif /* RPHP_PTRANSFORMHELPER_H_ */
