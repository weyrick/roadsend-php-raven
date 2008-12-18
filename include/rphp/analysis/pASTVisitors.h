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

#include "rphp/analysis/pAST.h"

namespace rphp { namespace AST {

class baseVisitor {
private:
    typedef void (baseVisitor::*dispatchFunction)(stmt *);
    static dispatchFunction dispatchTable_[];

public:
    virtual void visit(stmt*);

    void visit_block(block*);
    virtual void visit_echoStmt(echoStmt*) { }
    virtual void visit_inlineHtml(inlineHtml*) { }
    virtual void visit_literalString(literalString*) { }
    virtual void visit_literalInt(literalInt*) { }
    virtual void visit_literalFloat(literalFloat*) { }
    virtual void visit_literalBool(literalBool*) { }
    virtual void visit_literalNull(literalNull*) { }
    virtual void visit_assignment(assignment*) { }
    virtual void visit_var(var*) { }
    virtual void visit_functionInvoke(functionInvoke*) { }
    virtual void visit_constructorInvoke(constructorInvoke*) { }

};

class dumpVisitor: public baseVisitor {
    int indentLevel_;
    void indent() { indentLevel_ += 2; }
    void unindent() { indentLevel_ -= 2; }
    void showindent();
public:
    dumpVisitor(void): indentLevel_(0) { }

    void visit_echoStmt(echoStmt*);
    void visit_inlineHtml(inlineHtml*);
    void visit_literalString(literalString* n);
    void visit_literalInt(literalInt*);
    void visit_literalFloat(literalFloat*);
    void visit_literalBool(literalBool*);
    void visit_literalNull(literalNull*);
    void visit_assignment(assignment*);
    void visit_var(var*);
    void visit_functionInvoke(functionInvoke*);
    void visit_constructorInvoke(constructorInvoke*);

};

} } // namespace

#endif /* RPHP_PASTVISITORS_H_ */
