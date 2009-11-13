/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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

#include <cassert>

#include "rphp/pTypes.h"
#include "rphp/IR/pCompileTarget.h"
#include "rphp/IR/pStandAloneTargets.h"

namespace rphp {

class pCompileAndLinkTarget : public pStandAloneTarget {

    pCompileTarget* cTarget_;

public:
    pCompileAndLinkTarget(const pSourceFileDesc& sourceFile,
                          //const std::string& root,
                          const std::string& outName):
                            pStandAloneTarget(outName, sourceFile.fileName()),
                            cTarget_(new pCompileTarget(sourceFile/*, root*/))
    {
        assert(cTarget_ && "compile target was empty");
        assert(!cTarget_->getInputFileName().empty() && "input file was empty");
    }

    ~pCompileAndLinkTarget(void) { delete cTarget_; }

    virtual void execute(void);

};

} // namespace

#endif /* RPHP_PCOMPILEANDLINKTARGET_H_ */
