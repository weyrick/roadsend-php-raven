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
    virtual void visit(treeTop*) = 0;
    virtual void visit(statementNode*) = 0;
    virtual void visit(echoNode*) = 0;

};

class defaultVisitor: public baseVisitor {
public:
    virtual void visit(treeTop*);
    virtual void visit(statementNode*);
    virtual void visit(echoNode*) = 0;

};

class dumpVisitor: public defaultVisitor {
public:
    void visit(treeTop*);
    void visit(statementNode*);
    void visit(echoNode*);

};

} } // namespace

#endif /* RPHP_PASTVISITORS_H_ */
