/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2010 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_DUMPSTATS_H_
#define RPHP_DUMPSTATS_H_

#include "rphp/analysis/pAST.h"
#include "rphp/analysis/pBaseVisitor.h"

namespace rphp { namespace AST { namespace Pass {

class DumpStats: public pBaseVisitor {

public:
    DumpStats(pSourceModule* m):
            pBaseVisitor("Statistics Dump","Dump parse and memory statistics", m)
            { }

    void post_run(void);

};

} } } // namespace

#endif /* RPHP_DUMPAST_H_ */
