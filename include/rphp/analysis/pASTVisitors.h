/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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
    typedef bool (baseVisitor::*childDispatchFunction)(stmt *);

    static dispatchFunction preDispatchTable_[];
    static dispatchFunction postDispatchTable_[];
    static childDispatchFunction childrenDispatchTable_[];

protected:
    static const char* nodeDescTable_[];

public:
    virtual ~baseVisitor(void) { }

    // root dispatch
    void visit(stmt*);
    virtual void visitChildren(stmt*);

    virtual void visit_pre_stmt(stmt* ) { }
    virtual void visit_post_stmt(stmt* ) { }

    virtual void visit_pre_expr(expr* ) { }
    virtual void visit_post_expr(expr* ) { }

    // PRE
#define STMT(CLASS, PARENT) virtual void visit_pre_##CLASS(CLASS *) { }
#include "rphp/analysis/astNodes.def"

    // POST
#define STMT(CLASS, PARENT) virtual void visit_post_##CLASS(CLASS *) { }
#include "rphp/analysis/astNodes.def"

    // CHILDREN
    // for custom children handler, define and return true
#define STMT(CLASS, PARENT) virtual bool visit_children_##CLASS(CLASS *) { return false; }
#include "rphp/analysis/astNodes.def"

};


class dumpVisitor: public baseVisitor {
    int indentLevel_;
    void showindent();
public:
    dumpVisitor(void): indentLevel_(0) { }

    void visit_pre_stmt(stmt*);
    void visit_post_stmt(stmt*);
    void visitChildren(stmt*);

    void visit_pre_var(var*);
    void visit_pre_unaryOp(unaryOp* n);

};

} } // namespace

#endif /* RPHP_PASTVISITORS_H_ */
