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

enum nodeKind {
    echoStmtKind,
    inlineHtmlKind,
    literalBStringKind,
    literalIntKind,
    literalNullKind,
    literalBoolKind
};

// statement base class
struct stmt {

    nodeKind stmtKind;

    pUInt startLineNum;
    pUInt endLineNum;

public:
    stmt(nodeKind k): stmtKind(k) { }
    virtual ~stmt(void) { }

    const nodeKind getKind(void) { return stmtKind; }

};

// declaration base class
struct decl: public stmt {

public:


};

// expression base class
struct expr: public stmt {

public:
    expr(nodeKind k): stmt(k) { }

};

// literal expression base class
struct literalExpr: public expr {

    pBString stringVal;

public:
    literalExpr(nodeKind k): expr(k) { }
    literalExpr(nodeKind k, const pBString& v): expr(k), stringVal(v) { }
    
    const pBString& getStringVal(void) { return stringVal; }

};

// NODE: literal bstring
struct literalBString: public literalExpr {

public:
    literalBString(const pBString& v): literalExpr(literalBStringKind, v) { }
    literalBString(const pBString& v, nodeKind k): literalExpr(k, v) { }

};

// NODE: literal int
struct literalInt: public literalExpr {

public:
    literalInt(const pBString& v): literalExpr(literalIntKind, v) { }

};

// NODE: literal bool
struct literalBool: public literalExpr {

    bool boolVal;

public:
    literalBool(bool v): literalExpr(literalBoolKind), boolVal(v) { }

    bool getBoolVal(void) { return boolVal; }

};

// NODE: literal null
struct literalNull: public literalExpr {

public:
    literalNull(void): literalExpr(literalNullKind) { }

};

// NODE: inline html
struct inlineHtml: public literalBString {

public:
    inlineHtml(const pBString& v): literalBString(v, inlineHtmlKind) { }

};

// NODE: echo statement
struct echoStmt: public stmt {

    expr* rVal;

public:
    echoStmt(expr* v): stmt(echoStmtKind), rVal(v) { }
    ~echoStmt(void) { delete rVal; }

    expr* getRVal(void) { return rVal; }

};


} } // namespace

#endif /* RPHP_PAST_H_ */
