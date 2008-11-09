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
    literalStringKind,
    literalIntKind,
    literalFloatKind,
    literalNullKind,
    literalBoolKind,
    assignmentKind,
    varKind
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
    literalExpr(nodeKind k): expr(k), stringVal_() { }
    literalExpr(nodeKind k, const pBString& v): expr(k), stringVal_(v) { }
    
    const pBString& getStringVal(void) const { return stringVal_; }

};

// NODE: literal bstring
class literalString: public literalExpr {

    UnicodeString uStringVal_;
    bool isUnicode_;

public:
    literalString(const pBString& v): literalExpr(literalStringKind, v), uStringVal_(), isUnicode_(false) { }
    literalString(const pBString& v, nodeKind k): literalExpr(k, v), uStringVal_(), isUnicode_(false) { }

    literalString(const UnicodeString& v): literalExpr(literalStringKind), uStringVal_(v), isUnicode_(true) { }
    literalString(const UnicodeString& v, nodeKind k): literalExpr(k), uStringVal_(v), isUnicode_(true) { }

    const UnicodeString& getUStringVal(void) const { return uStringVal_; }

    // getTerminatedBuffer may mutate, so we do it here then return a const
    const UChar* getUStringBuf(void) {
        uStringVal_.getTerminatedBuffer();
        return uStringVal_.getBuffer();
    }

    bool isUnicode(void) const { return isUnicode_; }

};

// NODE: literal int
class literalInt: public literalExpr {

public:
    literalInt(const pBString& v): literalExpr(literalIntKind, v) { }

};

// NODE: literal float
class literalFloat: public literalExpr {

public:
    literalFloat(const pBString& v): literalExpr(literalFloatKind, v) { }

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
class inlineHtml: public literalString {

public:
    inlineHtml(const pBString& v): literalString(v, inlineHtmlKind) { }
    inlineHtml(const UnicodeString& v): literalString(v, inlineHtmlKind) { }

};

// NODE: echo statement
class echoStmt: public stmt {

    expr* rVal_;

public:
    echoStmt(expr* v): stmt(echoStmtKind), rVal_(v) { }
    ~echoStmt(void) { delete rVal_; }

    expr* rVal(void) { return rVal_; }

};

// NODE: var
class var: public expr {

    pIdentString name_;

public:
    var(const pIdentString& name): expr(varKind), name_(name) { }

    const pIdentString& name(void) const { return name_; }

};

// NODE: assignment
class assignment: public expr {

    expr* lVal_;
    expr* rVal_;

public:
    assignment(expr* lVal, expr* rVal): expr(assignmentKind), lVal_(lVal), rVal_(rVal) { }
    ~assignment(void) {
        delete lVal_;
        delete rVal_;
    }

    expr* lVal(void) { return lVal_; }
    expr* rVal(void) { return rVal_; }

};

} } // namespace

#endif /* RPHP_PAST_H_ */
