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
#include "rphp/analysis/pTempVar.h"

namespace rphp { namespace AST {
class pTransformHelper {
    pParseContext& C_;
    pTempVar t_;

public:
    pTransformHelper(pParseContext& C): C_(C), t_(C) {}

    // lTrue and lFalse provide an easy way to get a literal true or false in the code.
    literalBool* lTrue() {
        return new (C_) literalBool(true);
    }
    literalBool* lFalse() {
        return new (C_) literalBool(false);
    }
    // This method gets several expr*s which are chained together in a list of binaryOp*s.
    // You normally want to use this with binaryOp::BOOLEAN_AND
    // This function copys all expr's it's using clone().
    expr* chainExpressions(const expressionList* expressions, enum binaryOp::opKind operationType) {
        assert(!expressions->empty());
        assert(expressions->size() > 1);

        expressionList::const_iterator it = expressions->begin();
        expr* prevExpr = (*it)->clone(C_);
        ++it;
        for(expressionList::const_iterator end = expressions->end(); it != end; ++it)
            prevExpr = new (C_) binaryOp(prevExpr, (*it)->clone(C_), operationType);

        return prevExpr;
    }
    // That function is currently a slightly modified version of the one above with the whole code duplicated. Very bad.
    expr* chainExpressionsFromBlock(stmt::const_child_range statements, enum binaryOp::opKind operationType) {
        assert(!statements.empty());
        assert(++statements.begin() != statements.end() && " To chain expressions, you need at least two of them!");

        stmt::const_child_iterator it = statements.begin();
        expr* prevExpr = cast<expr>(it->clone(C_));
        ++it;
        for(stmt::const_child_iterator end = statements.end(); it != end; ++it)
            prevExpr = new (C_) binaryOp(prevExpr, cast<expr>(it->clone(C_)), operationType);

        return prevExpr;
    }
    var* tempVar(pStringRef name) {
        return t_.getTempVar(name);
    }
};


} } // namespace

#endif /* RPHP_PTRANSFORMHELPER_H_ */
