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

#ifndef RPHP_PAST_H_
#define RPHP_PAST_H_

#include <vector>
#include <boost/fusion/container.hpp>
#include <boost/fusion/sequence.hpp>

#include "pTypes.h"

namespace rphp { namespace AST {

// base node
struct Node {
    pUInt lineNum;
};

// NODE: literal bstring
struct literalBStringNode: public Node {

    pBString val;

};

// NODE: literal int
struct literalIntNode: public Node {

    pInt val;

};

// NODE: echo
struct echoNode: public Node {

    pBString rVal;

    echoNode(pBString v): rVal(v) {
        std::cout << "creating echoNode(" << v << ")" << std::endl;
    }
    
    ~echoNode() {
        std::cout << "destruct echoNode" << std::endl;
    }

};

// NODE: statement
struct statementNode: public Node {

    echoNode* echoNodeVar;

    statementNode(echoNode* v): echoNodeVar(v) {
        std::cout << "creating statementNode(echoNode*)" << std::endl;
    }

    ~statementNode() {
        std::cout << "destruct statementNode" << std::endl;
        if (echoNodeVar)
            delete echoNodeVar;
    }

};

// node list types
typedef std::vector<statementNode*> statementListType;

// NODE: treeTop
struct treeTop: public Node {
    statementListType statementList;
    ~treeTop(void) {
        for(statementListType::iterator s = statementList.begin(); s != statementList.end(); ++s) {
            delete *s;
        }
    }
};

} } // namespace

#endif /* RPHP_PAST_H_ */
