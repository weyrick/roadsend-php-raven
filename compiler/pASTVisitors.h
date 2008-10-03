/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
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
public:
    virtual void visit(stmt*);

    virtual void visit_echoStmt(echoStmt*) = 0;
    virtual void visit_inlineHtml(inlineHtml*) = 0;
    virtual void visit_literalBString(literalBString*) = 0;
    virtual void visit_literalInt(literalInt*) = 0;

};

class defaultVisitor: public baseVisitor {
public:
    virtual void visit_echoStmt(echoStmt*);
    virtual void visit_inlineHtml(inlineHtml*) = 0;
    virtual void visit_literalBString(literalBString*) = 0;
    virtual void visit_literalInt(literalInt*) = 0;

};

class dumpVisitor: public defaultVisitor {
public:
    void visit_echoStmt(echoStmt*);
    void visit_inlineHtml(inlineHtml*);
    void visit_literalBString(literalBString* n);
    void visit_literalInt(literalInt*);

};

} } // namespace

#endif /* RPHP_PASTVISITORS_H_ */
