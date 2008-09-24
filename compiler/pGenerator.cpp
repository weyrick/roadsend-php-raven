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
#include "pGenerator.h"

namespace rphp {

void pGenerator::visit(AST::treeTop* n) {
    std::cout << "generator: treeTop: " << n->statementList.size() << " top level statements in module" << std::endl;
    AST::defaultVisitor::visit(n);
}

void pGenerator::visit(AST::statementNode* n) {
    std::cout << "generator: statementNode" << std::endl;
    AST::defaultVisitor::visit(n);
}

void pGenerator::visit(AST::echoNode* n) {
    std::cout << "generator: echoNode, string: " << n->rVal << std::endl;
}

} // namespace

