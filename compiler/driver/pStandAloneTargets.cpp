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

#include <iostream>
#include <vector>
#include <string>

#include <llvm/Module.h>
#include <llvm/Support/SystemUtils.h>
#include <llvm/System/Program.h>

#include "rphp/driver/pTargetError.h"
#include "rphp/driver/pStandAloneTargets.h"

namespace rphp {

void pStandAloneTarget::execute(void) {

    llvm::Module* M = IR::pGenSupport::createStandAloneStubModule(outputFile_, mainFile_);
    // TODO: outfile nameing
    std::string stubOutFile(outputFile_+"-driver.bc");
    log(logInfo, "writing stand alone binary stub bitcode ["+stubOutFile+"]");
    IR::pGenSupport::writeBitcode(M, stubOutFile);
    delete M;

    log(logInfo, "linking stand alone executable ["+outputFile_+"]");

    // the following is based on code from llvm/tools/llvm-ld.cpp

    // link to native using llvm-ld
    llvm::sys::Path ld = llvm::sys::Program::FindProgramByName("llvm-ld");
    if (ld.isEmpty()) {
        throw pTargetError("unable to link: llvm-ld not found");
    }

    std::vector<std::string> args;
    args.push_back("llvm-ld");
    for (std::vector<std::string>::iterator i = libSearchPaths_.begin(); i != libSearchPaths_.end(); ++i) {
        args.push_back("-L"+(*i));
    }
    args.push_back("-native");
    // TODO: opt flags
    //args.push_back("-disable-opt");
    //args.push_back("-verify-each");
    if (verbosity() > logInfo)
        args.push_back("-v");
    //
    args.push_back("-lrphp-runtime");
    args.push_back("-o");
    args.push_back(outputFile_);
    args.push_back(stubOutFile);
    for (std::vector<std::string>::iterator i = inputFiles_.begin(); i != inputFiles_.end(); ++i) {
        args.push_back(*i);
    }

    std::vector<const char *> Args;
    for (unsigned i = 0, e = args.size(); i != e; ++i)
        Args.push_back(args[i].c_str());
    Args.push_back(0);

    std::vector<const char*>::const_iterator I = Args.begin(), E = Args.end();

    if (verbosity() >= logFull) {
        std::string cline;
        for (; I != E; ++I) {
            if (*I) {
                //cline.push_back('\'');
                cline.append(*I);
                //cline.push_back('\'');
                cline.push_back(' ');
            }
        }
        log(logFull, cline);
    }
    
    std::string errMsg;
    int R = llvm::sys::Program::ExecuteAndWait(
        ld, &Args[0], 0, 0, 0, 0, &errMsg);

    if (R != 0) {
        throw pTargetError(errMsg);
    }

}

} // namespace

