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

#include "rphp/IR/pStandAloneTargets.h"

#include "rphp/pTargetError.h"

#include <llvm/Module.h>
#include <llvm/System/Path.h>
#include <llvm/Support/SystemUtils.h>
#include <llvm/System/Program.h>

#include <cstring>
#include <iostream>
#include <vector>
#include <string>

namespace rphp {

// from llvm/lib/System/Path.cpp. why isnt it exported?
static void getPathList(const char*path, std::vector<llvm::sys::Path>& Paths) {
  const char* at = path;
  const char* delim = strchr(at, llvm::sys::PathSeparator);
  llvm::sys::Path tmpPath;
  while (delim != 0) {
    std::string tmp(at, size_t(delim-at));
    if (tmpPath.set(tmp))
      if (tmpPath.canRead())
        Paths.push_back(tmpPath);
    at = delim + 1;
    delim = strchr(at, llvm::sys::PathSeparator);
  }

  if (*at != 0)
    if (tmpPath.set(std::string(at)))
      if (tmpPath.canRead())
        Paths.push_back(tmpPath);
}

void pStandAloneTarget::execute(void) {

    log(E_CORE_WARNING, "linking stand alone executable ["+outputFile_+"]");

    // the following is based on code from llvm/tools/llvm-ld.cpp

    std::vector<llvm::sys::Path> rtPaths;
    char* rtPathE = getenv("RPHP_RUNTIME_PATH");
    if (rtPathE) {
        getPathList(rtPathE, rtPaths);
        for (unsigned int i=0; i < rtPaths.size(); i++) {
#if(LLVM_VERSION >= 2007000)
            libSearchPaths_.push_back(rtPaths[i].str());
#else
            libSearchPaths_.push_back(rtPaths[i].toString());
#endif
        }
    }

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
    args.push_back("-disable-opt");
    //args.push_back("-verify-each");
    if (verbosity_ & E_COMPILE_DEBUG)
        args.push_back("-v");
    //
    args.push_back("-lrphp-runtime");

    // XXX osx
#ifdef __APPLE__
    args.push_back("-lstdc++.6");
    args.push_back("-licuuc");
    args.push_back("-licuio");
    args.push_back("-lgmp");
#endif

    args.push_back("-o");
    args.push_back(outputFile_);
    for (std::vector<std::string>::iterator i = inputFiles_.begin(); i != inputFiles_.end(); ++i) {
        args.push_back(*i);
    }

    std::vector<const char *> Args;
    for (unsigned i = 0, e = args.size(); i != e; ++i)
        Args.push_back(args[i].c_str());
    Args.push_back(0);

    std::vector<const char*>::const_iterator I = Args.begin(), E = Args.end();

    if (verbosity_ & E_COMPILE_DEBUG) {
        std::string cline;
        for (; I != E; ++I) {
            if (*I) {
                //cline.push_back('\'');
                cline.append(*I);
                //cline.push_back('\'');
                cline.push_back(' ');
            }
        }
        log(E_COMPILE_DEBUG, cline);
    }

    std::string errMsg;
    int R = llvm::sys::Program::ExecuteAndWait(
        ld, &Args[0], 0, 0, 0, 0, &errMsg);

    if (R != 0) {
        throw pTargetError(errMsg);
    }

}

} // namespace

