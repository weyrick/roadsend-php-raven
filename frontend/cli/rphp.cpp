
#include <exception>
#include <iostream>
#include <vector>
#include <string>
#include <llvm/Support/CommandLine.h>
#include <llvm/System/Path.h>

#include "rphp/IR/pDumpTarget.h"
#include "rphp/IR/pCompileTarget.h"
#include "rphp/IR/pStandAloneTargets.h"
#include "rphp/IR/pCompileAndLinkTarget.h"

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
    cl::opt<bool> dumpToks ("dump-toks", cl::desc("Dump tokens from lexer"));
    cl::opt<bool> dumpIR ("dump-ir", cl::desc("Dump IR "));
    cl::opt<bool> dumpAST ("dump-ast", cl::desc("Dump AST"));
    cl::opt<bool> dumpPre ("dump-pre", cl::desc("Preprocess the source file and dump it to stdout"));
    cl::opt<int> verbosity ("v", cl::desc("Verbosity level (0=Silent/1=Info/2=Full/3+=Debug)"));

    cl::opt<std::string> outputFile ("o",cl::desc("Output file name"));
    cl::opt<std::string> mainFile ("main-file",cl::desc("Main entry script for stand alone programs"));
    cl::opt<std::string> encoding ("encoding",cl::desc("Character encoding of the source file"));

    cl::opt<std::string> libSearchPath ("L",cl::desc("Add directory to linker search path"));

    cl::opt<bool> compileModule ("c", cl::desc("Compile a single source file to bitcode object file"));
    cl::opt<bool> linkSA ("link-sa", cl::desc("Link objects to stand alone executable"));

    cl::SetVersionPrinter(&rphpVersion);
    cl::ParseCommandLineOptions(argc, argv, "Roadsend PHP");

    // default encoding
    if (encoding.empty())
        encoding = "ASCII";


    assert(!inputFile.empty() && "empty input file");

    pSourceFileDesc inFile = boost::make_tuple(inputFile, encoding);

    pTarget* target = NULL;
    if (compileModule) {
        target = new pCompileTarget(inFile/*, "/"*/);
    }
    else if (linkSA) {
        if (outputFile.empty()) {
            std::cerr << "no output file was specified" << std::endl;
            return 1;
        }
        if (mainFile.empty()) {
            std::cerr << "no main file was specified" << std::endl;
            return 1;
        }
        pStandAloneTarget* saTarget = new pStandAloneTarget(outputFile, mainFile);
        saTarget->addInputFile(inputFile);
        if (!libSearchPath.empty())
            saTarget->addLibSearchPath(libSearchPath);
        target = saTarget;
    }
    else if (dumpToks) {
        target = new pDumpTarget(inFile, pDumpTarget::Tokens);
    }
    else if (dumpAST) {
        target = new pDumpTarget(inFile, pDumpTarget::AST);
    }
    else if (dumpIR) {
        target = new pDumpTarget(inFile, pDumpTarget::IR);
    }
    else if (dumpPre) {
        target = new pDumpTarget(inFile, pDumpTarget::Preprocessor);
    }
    else {
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

        pCompileAndLinkTarget* saTarget = new pCompileAndLinkTarget(inFile, /*"/",*/ oFileName);
        if (!libSearchPath.empty())
            saTarget->addLibSearchPath(libSearchPath);
        target = saTarget;
    }

    if (!target) {
        // success
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
        return 1;
    }

    // success
    delete target;
    return 0;

}
