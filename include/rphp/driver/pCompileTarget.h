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

#ifndef RPHP_PCOMPILETARGET_H_
#define RPHP_PCOMPILETARGET_H_

#include "rphp/runtime/pTypes.h"
#include "rphp/driver/pTarget.h"

namespace rphp {

// the compile target always translates a single php source file into llvm bitcode
class pCompileTarget: public pTarget {

protected:
    pSourceFileDesc inputFile_;
    std::string projectRoot_;
    bool createMain_;

    // INT options:
    // compileOptimizationLevel - compiler optimization level
    // verbosityLevel           - informational verbosity level

public:
    pCompileTarget(const pSourceFileDesc& fileName, const std::string& root): inputFile_(fileName), projectRoot_(root), createMain_(false) { }

    virtual void execute(void);

    const std::string& getInputFileName(void) const { return inputFile_.get<0>(); }
    const std::string& getProjectRoot(void) const { return projectRoot_; }

    bool createMain(void) const { return createMain_; }
    void setCreateMain(bool v) { createMain_ = v; }

};

} // namespace

#endif /* RPHP_PCOMPILETARGET_H_ */
