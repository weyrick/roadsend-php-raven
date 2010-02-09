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

#ifndef RPHP_SPLITBUILTINS_H_
#define RPHP_SPLITBUILTINS_H_

#include "rphp/analysis/pBaseTransformer.h"

namespace rphp { namespace AST { namespace Pass {

class Split_Builtins: public pBaseTransformer {

public:
    Split_Builtins(pSourceModule *m):
            pBaseTransformer("Split_Builtins","Splits up multiple arguments to unset/isset into different unset/isset calls", m)
    {
    }

    expr* transform_post_builtin(builtin* n);

};

} } } // namespace

#endif /* RPHP_SPLITBUILTINS_H_ */
