/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PAST_H_
#define RPHP_PAST_H_

#include <vector>

#include "pTypes.h"

namespace rphp { namespace AST {

// NOTE: if you change this, check static dispatch table in pASTVisitors.cpp!
enum nodeKind {
    echoStmtKind,
    inlineHtmlKind,
    literalBStringKind,
    literalIntKind,
    literalNullKind,
    literalBoolKind
};

// statement base class
class stmt {

    nodeKind kind_;

    pUInt startLineNum_;
    pUInt endLineNum_;

public:
    stmt(nodeKind k): kind_(k) { }
    virtual ~stmt(void) { }

    nodeKind getKind(void) const { return kind_; }

};

// declaration base class
class decl: public stmt {

public:


};

// expression base class
class expr: public stmt {

public:
    expr(nodeKind k): stmt(k) { }

};

// literal expression base class
class literalExpr: public expr {

    pBString stringVal_;

public:
    literalExpr(nodeKind k): expr(k) { }
    literalExpr(nodeKind k, const pBString& v): expr(k), stringVal_(v) { }
    
    const pBString& getStringVal(void) const { return stringVal_; }

};

// NODE: literal bstring
class literalBString: public literalExpr {

public:
    literalBString(const pBString& v): literalExpr(literalBStringKind, v) { }
    literalBString(const pBString& v, nodeKind k): literalExpr(k, v) { }

};

// NODE: literal int
class literalInt: public literalExpr {

public:
    literalInt(const pBString& v): literalExpr(literalIntKind, v) { }

};

// NODE: literal bool
class literalBool: public literalExpr {

    bool boolVal_;

public:
    literalBool(bool v): literalExpr(literalBoolKind), boolVal_(v) { }

    bool getBoolVal(void) const { return boolVal_; }

};

// NODE: literal null
class literalNull: public literalExpr {

public:
    literalNull(void): literalExpr(literalNullKind) { }

};

// NODE: inline html
class inlineHtml: public literalBString {

public:
    inlineHtml(const pBString& v): literalBString(v, inlineHtmlKind) { }

};

// NODE: echo statement
class echoStmt: public stmt {

    expr* rVal_;

public:
    echoStmt(expr* v): stmt(echoStmtKind), rVal_(v) { }
    ~echoStmt(void) { delete rVal_; }

    expr* getRVal(void) { return rVal_; }

};


} } // namespace

#endif /* RPHP_PAST_H_ */
