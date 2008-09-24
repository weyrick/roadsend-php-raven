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

#ifndef RPHP_PCOMPILETARGET_H_
#define RPHP_PCOMPILETARGET_H_

#include "pTarget.h"

namespace rphp {

// the compile target always translates a single php source file into llvm bitcode
class pCompileTarget: public pTarget {

protected:
    std::string inputFile;
    std::string projectRoot;

    // INT options:
    // compileOptimizationLevel - compiler optimization level
    // verbosityLevel           - informational verbosity level

public:
    pCompileTarget(const std::string& fileName, const std::string& root): inputFile(fileName), projectRoot(root) { }

    virtual void execute(void);

};

} // namespace

#endif /* RPHP_PCOMPILETARGET_H_ */
