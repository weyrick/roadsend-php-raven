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

#include <boost/shared_ptr.hpp>

#include <vector>
#include "pTypes.h"

namespace rphp { namespace AST {

/* node forwards */
class moduleNode;
typedef boost::shared_ptr<moduleNode> moduleNodeP;

class statementNode;
typedef boost::shared_ptr<statementNode> statementNodeP;

class literalIntNode;
typedef boost::shared_ptr<literalIntNode> literalIntNodeP;

/* node list types */
typedef std::vector<statementNodeP> statementListType;

/* encapsulates a single php "module" (one script) */
class rphpModule {

private:
    std::string originalFileName;
    moduleNodeP moduleAST;
};

/* base node */
struct Node {
    pSourceLocation location;
};

/* NODE: module */
struct moduleNode: public Node {
    statementListType statementList;
};

/* NODE: statement */
struct statementNode: public Node {

};

/* NODE: literal int */
struct literalIntNode: public Node {

    pInt val;

};


} } // namespace

#endif /* RPHP_PAST_H_ */
