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

#ifndef RPHP_PCREATENODE_H_
#define RPHP_PCREATENODE_H_

#include <boost/spirit/include/support_unused.hpp>

#include "pModule.h"
#include "pAST.h"

namespace rphp {

// This is a generic functor for creating AST nodes
// using a given module's memory pool

template <class T>
struct pCreateNode {

private:
    pModuleP& pMod;

public:
    typedef T* result_type;

    pCreateNode(pModuleP& p): pMod(p) { }

    T* operator()(typename T::parseSig v, boost::spirit::unused_type, boost::spirit::unused_type) const {
        return new (pMod->getMemPool().allocate(sizeof(T))) T(v);
    }

};

} // namespace

#endif /* RPHP_PCREATENODE_H_ */
