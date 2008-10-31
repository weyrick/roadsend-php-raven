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

#ifndef RPHP_PASTVISITORS_H_
#define RPHP_PASTVISITORS_H_

#include "pAST.h"

namespace rphp { namespace AST {

class baseVisitor {
private:
    typedef void (baseVisitor::*dispatchFunType)(stmt *);
    static dispatchFunType dispatchTable[];

public:
    virtual void visit(stmt*);

    virtual void visit_echoStmt(echoStmt*) = 0;
    virtual void visit_inlineHtml(inlineHtml*) = 0;
    virtual void visit_literalBString(literalBString*) = 0;
    virtual void visit_literalInt(literalInt*) = 0;
    virtual void visit_literalBool(literalBool*) = 0;
    virtual void visit_literalNull(literalNull*) = 0;

};

class defaultVisitor: public baseVisitor {
public:
    virtual void visit_echoStmt(echoStmt*);
    virtual void visit_inlineHtml(inlineHtml*) = 0;
    virtual void visit_literalBString(literalBString*) = 0;
    virtual void visit_literalInt(literalInt*) = 0;
    virtual void visit_literalBool(literalBool*) = 0;
    virtual void visit_literalNull(literalNull*) = 0;

};

class dumpVisitor: public defaultVisitor {
    int indentLevel;
    void indent() { indentLevel += 2; }
    void unindent() { indentLevel -= 2; }
    void showindent();
public:
    dumpVisitor(void): indentLevel(0) { }

    void visit_echoStmt(echoStmt*);
    void visit_inlineHtml(inlineHtml*);
    void visit_literalBString(literalBString* n);
    void visit_literalInt(literalInt*);
    void visit_literalBool(literalBool*);
    void visit_literalNull(literalNull*);

};

} } // namespace

#endif /* RPHP_PASTVISITORS_H_ */
