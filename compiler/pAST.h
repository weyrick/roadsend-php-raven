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

// node forwards
class statementNode;

// node list types
typedef std::vector<statementNode*> statementListType;

// base node
struct Node {
    pUInt lineNum;
};

// NODE: treeTop
struct treeTop: public Node {
    statementListType statementList;
};

// NODE: statement
struct statementNode: public Node {

};

// NODE: literal bstring
struct literalBStringNode: public Node {

    pBString val;

};

// NODE: literal int
struct literalIntNode: public Node {

    pInt val;

};

// NODE: literal int
struct echoNode: public Node {

/*
    typedef boost::fusion::vector<std::string> fVectorSig;

    echoNode(const fVectorSig& r): rVal(boost::fusion::at_c<0>(r)) {
        std::cout << "creating echo node" << std::endl;
    }
*/
    typedef pBString parseSig;

    pBString rVal;

    echoNode(const parseSig& r): rVal(r) {
        std::cout << "creating echo node, string is: " << rVal << std::endl;
    }

    // XXX FIXME TODO this never runs, and will leak pBString data
    // because memPool does not run destructors. but, it's fast, right?
    ~echoNode() {
        std::cout << "echo node destructing, string is: " << rVal << std::endl;
    }

};

} } // namespace

#endif /* RPHP_PAST_H_ */
