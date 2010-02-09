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
;;
;; Code derived from:
;; phc -- the open source PHP compiler
;; Copyright (c) 2005-2007, Edsko de Vries, John Gilbert and Paul Biggar.
;; All rights reserved.
;; Originally distributed under the BSD license
;;
 ***** END LICENSE BLOCK *****
 */


#ifndef RPHP_LOWERBINARYOP_H
#define RPHP_LOWERBINARYOP_H

#include "rphp/analysis/pBaseTransformer.h"

namespace rphp { namespace AST { namespace Pass {

class Lower_Binary_Op: public pBaseTransformer {

public:
    Lower_Binary_Op(pSourceModule *m):
            pBaseTransformer("Lower_Binary_Op","Lowers binary operators", m)
    {
    }

    expr* transform_pre_binaryOp(binaryOp* n);

};

} } } // namespace

#endif /* RPHP_LOWERBINARYOP_H */
