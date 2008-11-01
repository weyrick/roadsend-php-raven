/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PCOMPILEANDLINKTARGET_H_
#define RPHP_PCOMPILEANDLINKTARGET_H_

#include "pCompileTarget.h"
#include "pStandAloneTargets.h"

namespace rphp {

class pCompileAndLinkTarget : public pStandAloneTarget {

    pCompileTarget* cTarget_;

public:
    pCompileAndLinkTarget(const std::string& fileName,
                          const std::string& root,
                          const std::string& outName): pStandAloneTarget(outName, fileName), cTarget_(new pCompileTarget(fileName, root)) { }

    ~pCompileAndLinkTarget(void) { delete cTarget_; }

    virtual void execute(void);

};

} // namespace

#endif /* RPHP_PCOMPILEANDLINKTARGET_H_ */