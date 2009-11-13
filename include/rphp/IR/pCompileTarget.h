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

#ifndef RPHP_PCOMPILETARGET_H_
#define RPHP_PCOMPILETARGET_H_

#include "rphp/pTypes.h"
#include "rphp/pSourceTypes.h"
#include "rphp/pTarget.h"

#include <llvm/LLVMContext.h>

namespace llvm {
    class Module;
}

namespace rphp {

// the compile target always translates a single php source file into llvm bitcode
class pCompileTarget: public pTarget {

protected:
    llvm::Module *llvmModule_;
    llvm::LLVMContext context_;
    pSourceFileDesc inputFile_;
    std::string projectRoot_;
    pIdentString entryFunctionName_;
    bool createMain_;
    bool ownModule_;

    // INT options:
    // compileOptimizationLevel - compiler optimization level
    // verbosityLevel           - informational verbosity level

public:
    pCompileTarget(const pSourceFileDesc& fileName/*, const std::string& root*/):
        llvmModule_(NULL),
        context_(),
        inputFile_(fileName),
        projectRoot_(/*root*/),
        entryFunctionName_(),
        createMain_(false),
        ownModule_(true) { }

    ~pCompileTarget(void);

    virtual void execute(void);

    const std::string& getInputFileName(void) const { return inputFile_.fileName(); }
    const std::string& getProjectRoot(void) const { return projectRoot_; }

    void writeToFile(std::string outputFile);

    llvm::Module* releaseModuleOwnership(void) {
        ownModule_ = false;
        return llvmModule_;
    }

    llvm::LLVMContext& getLLVMContext(void) {
        return context_;
    }

    bool createMain(void) const { return createMain_; }
    void setCreateMain(bool v) { createMain_ = v; }

    const pIdentString& entryFunctionName(void) const {
        return entryFunctionName_;
    }

};

} // namespace

#endif /* RPHP_PCOMPILETARGET_H_ */
