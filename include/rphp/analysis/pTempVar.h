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

#ifndef RPHP_PTEMPVAR_H_
#define RPHP_PTEMPVAR_H_

#include "rphp/analysis/pAST.h"

#include <sstream>

namespace rphp {

class pTempVar {
    pUInt nameCount_;
    AST::pParseContext& C_;

public:
    pTempVar(AST::pParseContext& C) : nameCount_(0), C_(C) {}
    
    std::string getTempName(pStringRef name) {
        std::stringstream ss;
        ss << "." << name.str() << nameCount_++;
        return ss.str();
    }
    
    AST::var* getTempVar(pStringRef name) {
        //TODO: Add a facility for release builds which doesn't care about name for speed.
        return new(C_) AST::var(getTempName(name), C_);
    }
};

} // namespace

#endif /* RPHP_PTEMPVAR_H_ */
