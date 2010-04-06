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

#ifndef RPHP_LOWERCONTROLFLOW_H
#define RPHP_LOWERCONTROLFLOW_H

#include "rphp/analysis/pBaseTransformer.h"

namespace rphp { namespace AST { namespace Pass {

class Lower_Control_Flow: public pBaseTransformer {

    functionDecl* currentFunction_;
    std::vector<label*> breakLabels_;
    std::vector<label*> continueLabels_;
    
public:
    Lower_Control_Flow(pSourceModule *m):
            pBaseTransformer("Lower_Control_Flow","Lowers the control flow.", m),
            currentFunction_(0)
    {
    }

    stmt* transform_post_ifStmt(ifStmt* n);
    
    stmt* transform_pre_whileStmt(whileStmt* n);
    stmt* transform_post_whileStmt(whileStmt* n);
    
    stmt* transform_post_breakStmt(breakStmt* n);
    stmt* transform_post_continueStmt(continueStmt* n);

    stmt* transform_pre_forEach(forEach* n);


    stmt* transform_pre_functionDecl(functionDecl* n);
    
private:
    stmt* lower_exit(std::vector<label*>& labels, expr* depthExpr);

};

} } } // namespace

#endif /* RPHP_LOWERCONTROLFLOW_H */
