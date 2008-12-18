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

#include "rphp/analysis/pSourceTypes.h"
#include "rphp/runtime/pTypes.h"

namespace rphp { namespace AST {

// NOTE: if you change this, check static dispatch table in pASTVisitors.cpp!
enum nodeKind {
    blockKind,
    echoStmtKind,
    inlineHtmlKind,
    literalStringKind,
    literalIntKind,
    literalFloatKind,
    literalNullKind,
    literalBoolKind,
    assignmentKind,
    varKind,
    functionInvokeKind,
    constructorInvokeKind
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

typedef std::vector<stmt*> statementList;

// a block of statements
class block: public stmt {
public:
    statementList statements;

    block(void): stmt(blockKind) { }
    ~block(void) {
        for (int i=0; i < statements.size(); i++) {
            delete statements[i];
        }
    }
    
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

typedef std::vector<expr*> expressionList;

// literal expression base class
class literalExpr: public expr {

    pSourceString stringVal_;

public:
    literalExpr(nodeKind k): expr(k), stringVal_() { }
    literalExpr(nodeKind k, const pSourceRange& v): expr(k), stringVal_(v.begin(), v.end()) { }
    
    const pSourceString& getStringVal(void) const {
        return stringVal_;
    }

};

// NODE: literal bstring
class literalString: public literalExpr {

    bool isBinary_;

public:
    literalString(bool isBinary): literalExpr(literalStringKind), isBinary_(isBinary) { }
    literalString(const pSourceRange& v, bool isBinary): literalExpr(literalStringKind, v), isBinary_(isBinary) { }
    literalString(const pSourceRange& v, nodeKind k): literalExpr(k, v), isBinary_(false) { }

    bool isBinary(void) const { return isBinary_; }

};

// NODE: literal int
class literalInt: public literalExpr {

public:
    literalInt(const pSourceRange& v): literalExpr(literalIntKind, v) { }

};

// NODE: literal float
class literalFloat: public literalExpr {

public:
    literalFloat(const pSourceRange& v): literalExpr(literalFloatKind, v) { }

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
    inlineHtml(const pSourceRange& v): literalString(v, inlineHtmlKind) { }

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
    var(const pSourceRange& name): expr(varKind), name_(name.begin(), name.end()) { }

    pIdentString name(void) const {
        return name_;
    }

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

// NODE: function invoke
class functionInvoke: public expr {

    pIdentString name_;
    expressionList argList_;

public:
    functionInvoke(const pSourceRange& name, expressionList* argList, nodeKind kind = functionInvokeKind):
        expr(kind),
        name_(name.begin(), name.end()),
        argList_(*argList)
    {
        // free parser's version, which we've copied
        delete argList;
    }
    
    ~functionInvoke(void) {
        for (expressionList::iterator i = argList_.begin(); i != argList_.end(); ++i) {
            delete *i;
        }
    }

    pIdentString name(void) const {
        return name_;
    }
    expressionList& argList(void) { return argList_; }
    const expressionList& argList(void) const { return argList_; }

};

// NODE: constructor invoke
class constructorInvoke: public functionInvoke {

public:
    constructorInvoke(const pSourceRange& name, expressionList* argList):
        functionInvoke(name, argList, constructorInvokeKind)
    {

    }


};


} } // namespace

#endif /* RPHP_PAST_H_ */
