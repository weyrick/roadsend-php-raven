/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2010 Cornelius Riemenschneider <c.r1@gmx.de>
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

#ifndef RPHP_MAINFUNCTION_H_
#define RPHP_MAINFUNCTION_H_

#include "rphp/analysis/pBaseTransformer.h"

namespace rphp { namespace AST { namespace Pass {

class MainFunction: public pBaseTransformer {
    bool firstBlock;
    
public:
    MainFunction(pSourceModule *m):
            pBaseTransformer("MainFunction","Moves all global code in a __MAIN__ function", m),
            firstBlock(false)
    {
    }

    stmt* transform_pre_block(block* n);

};

} } } // namespace

#endif /* RPHP_MAINFUNCTION_H_ */
