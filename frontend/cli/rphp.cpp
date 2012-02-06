/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009-2012 Shannon Weyrick <weyrick@mozek.us>
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

#include <exception>
#include <iostream>
#include <vector>
#include <string>
#include <llvm/Support/CommandLine.h>
//#include <llvm/Support/PathV2.h>

#include "rphp/pConfig.h"
#include "rphp/IR/pDumpTarget.h"
#include "rphp/IR/pCompileTarget.h"
#include "rphp/IR/pStandAloneTargets.h"
#include "rphp/IR/pCompileAndLinkTarget.h"
#include "rphp/runtime/pRuntimeError.h"
//#include "rphp/JIT/pCachedJIT.h"

using namespace llvm;
using namespace rphp;

void rphpVersion(void) {
    // TODO get version info from runtime
    std::cout << "Roadsend PHP" << std::endl;
}


void consoleNoticeHandler(pUInt level, pMsgString msg) {
    std::cerr << "]] " << msg << std::endl;
}

int main( int argc, char* argv[] )
{

    // command line options
    cl::opt<std::string> inputFile(cl::Positional, cl::desc("<input file>"), cl::Required);
    cl::opt<bool> dumpIR ("dump-ir", cl::desc("Dump IR "));
    cl::opt<int> verbosity ("v", cl::desc("Verbosity level (0=Silent/1=Info/2=Full/3+=Debug)"));

    cl::opt<bool> iSF ("f", cl::desc("Execute source file immediately"));

    cl::opt<std::string> outputFile ("o",cl::desc("Output file name"));
    cl::opt<std::string> mainFile ("main-file",cl::desc("Main entry script for stand alone programs"));

    cl::opt<std::string> encoding ("encoding",cl::desc("Character encoding of the source file"), cl::init("UTF-8"));
    cl::opt<std::string> outputEncoding("output-encoding",cl::desc("Character encoding of final output"));

    cl::opt<std::string> libSearchPath ("L",cl::desc("Add directory to linker search path"));

    cl::opt<bool> compileModule ("c", cl::desc("Compile a single source file to bitcode object file"));
    cl::opt<bool> linkSA ("link-sa", cl::desc("Link objects to stand alone executable"));

    cl::SetVersionPrinter(&rphpVersion);
    cl::ParseCommandLineOptions(argc, argv, "Roadsend PHP");

    assert(!inputFile.empty() && "empty input file");

    pConfig* config = new pConfig();
    // TODO: read php.ini type file, do command line options
    // XXX temporary
    if (!outputEncoding.empty())
        config->set("outputEncoding", outputEncoding);

    pSourceFileDesc inFile(inputFile, encoding);

    // JIT
    /*
    if (iSF) {
        pCachedJIT engine(config);

        try {
            if (verbosity > 0)
                engine.setVerbosity(E_ALL);
            engine.cacheAndJITFileOnDisk(inFile);
        }
        catch (pRuntimeError& e) {
            // runtime errors go to output buffer by default, so we don't display here
            delete config;
            return 1;
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            delete config;
            return 1;
        }

        // success
        delete config;
        return 0;
    }
    */


    // AOT COMPILER
    pTarget* target = NULL;
    /*
    if (compileModule) {
        target = new pCompileTarget(inFile);
    }
    else if (linkSA) {
        if (outputFile.empty()) {
            std::cerr << "no output file was specified" << std::endl;
            delete config;
            return 1;
        }
        if (mainFile.empty()) {
            std::cerr << "no main file was specified" << std::endl;
            delete config;
            return 1;
        }
        pStandAloneTarget* saTarget = new pStandAloneTarget(outputFile, mainFile);
        saTarget->addInputFile(inputFile);
        if (!libSearchPath.empty())
            saTarget->addLibSearchPath(libSearchPath);
        target = saTarget;
    }

    else*/
    if (dumpIR) {
        target = new pDumpTarget(inFile, pDumpTarget::IR);
    }
    else {
        /*
        // default: compile and link single php script to native binary
        std::string oFileName = outputFile;
        if (oFileName.empty()) {
            // try to whip something up
            sys::Path oFileP(inputFile);
            oFileP.eraseSuffix();
            if (oFileP.isValid())
#if(LLVM_VERSION >= 2007000)
                oFileName = oFileP.str();
#else
                oFileName = oFileP.toString();
#endif
            else
                oFileName = "a.out";
        }

        assert(!oFileName.empty() && "empty output file");

        pCompileAndLinkTarget* saTarget = new pCompileAndLinkTarget(inFile, oFileName);
        if (!libSearchPath.empty())
            saTarget->addLibSearchPath(libSearchPath);
        target = saTarget;
        */
    }

    if (!target) {
        // success
        delete config;
        return 0;
    }

    try {
        if (verbosity > 0)
            target->setVerbosity(E_ALL);
        target->setNotifyHandler(&consoleNoticeHandler);
        // TODO: debug
        //target->setVerbosity(2);
        target->execute();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        delete target;
        delete config;
        return 1;
    }

    // success
    delete target;
    delete config;
    return 0;

}
