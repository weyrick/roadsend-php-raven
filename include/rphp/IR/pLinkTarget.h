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

#ifndef RPHP_PLINKTARGET_H_
#define RPHP_PLINKTARGET_H_

#include <string>
#include <vector>
#include "rphp/pTarget.h"

namespace llvm {
    class Module;
}

namespace rphp {

// link targets will create various native binaries from one or more compiled bitcode files
class pLinkTarget : public pTarget {

protected:
    std::string outputFile_;
    std::vector<std::string> inputFiles_;
    std::vector<std::string> libSearchPaths_;

    // link options (static, dynamic)

public:
    pLinkTarget(std::string outFile): outputFile_(outFile) { }

    void addInputFile(std::string f) {
        inputFiles_.push_back(f);
    }

    void addLibSearchPath(std::string f) {
        libSearchPaths_.push_back(f);
    }

    const std::string& getOutputFileName(void) const { return outputFile_; }

};

} // namespace

#endif /* RPHP_PLINKTARGET_H_ */
