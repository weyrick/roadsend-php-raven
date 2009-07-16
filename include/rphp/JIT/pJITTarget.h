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

#ifndef RPHP_PJITTARGET_H_
#define RPHP_PJITTARGET_H_

#include "rphp/pTarget.h"

#include <string>

namespace llvm {
    class Module;
}

namespace rphp {

class pRuntimeEngine;

class pJITTarget: public pTarget {

    // does not own runtime or module
    pRuntimeEngine* runtime_;
    llvm::Module* llvmModule_;
    std::string entryFunction_;

public:
    pJITTarget(pRuntimeEngine* engine, llvm::Module* M, std::string entryFunction):
        runtime_(engine),
        llvmModule_(M),
        entryFunction_(entryFunction) { }

    virtual void execute(void);

    //bool executeWithRuntime(llvm::Module* M, std::string entryFunction);
    //bool executeMain(llvm::Module* M);

};

}

#endif /* RPHP_PJIT_H_ */
