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

#include <iostream>
#include <vector>
#include "pASTVisitors.h"

namespace rphp { namespace AST {


void defaultVisitor::visit(treeTop* n) {
    for(statementListType::iterator s = n->statementList.begin(); s != n->statementList.end(); ++s) {
        visit(*s);
    }
}

void defaultVisitor::visit(statementNode* n) {
    // fixme
}

void dumpVisitor::visit(statementNode* n) {
    std::cout << "statementNode" << std::endl;
    defaultVisitor::visit(n);
}


} } // namespace

