
#include <exception>
#include <iostream>
#include <vector>
#include <string>
#include <llvm/Support/CommandLine.h>

#include "pDriver.h"
#include "pInterpretTarget.h"
#include "pCompileTarget.h"
#include "pStandAloneTargets.h"
#include "pCompileAndLinkTarget.h"

using namespace llvm;
using namespace rphp;

void rphpVersion(void) {
    // TODO get version info from runtime
    std::cout << "Roadsend PHP" << std::endl;
}

int main( int argc, char* argv[] )
{

    // command line options
    cl::opt<std::string> inputFile(cl::Positional, cl::desc("<input file>"), cl::Required);
    cl::opt<bool> dumpToks ("dump-toks", cl::desc("Dump tokens from lexer"));
    cl::opt<bool> dumpIR ("dump-ir", cl::desc("Dump IR "));
    cl::opt<bool> dumpAST ("dump-ast", cl::desc("Dump AST"));
    cl::opt<bool> dumpPre ("dump-pre", cl::desc("Preprocess the source file and dump it to stdout"));
    cl::opt<bool> iBC ("i", cl::desc("Interpret bytecode"));
    cl::opt<bool> iSF ("f", cl::desc("Execute source file immediately"));

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

    pDriver driver;
    pTarget* target = NULL;
    if (compileModule) {
        target = new pCompileTarget(inputFile, "/");
    }
    else if (iSF) {
        target = new pInterpretTarget(inputFile, "/");
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
        try {
            driver.dumpTokens(boost::make_tuple(inputFile, encoding));
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        return 0;
    }
    else if (dumpAST) {
        try {
            driver.dumpAST(boost::make_tuple(inputFile, encoding));
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        return 0;
    }
    else if (dumpIR) {
        try {
            driver.dumpIR(boost::make_tuple(inputFile, encoding));
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        return 0;
    }
    else if (dumpPre) {
        try {
            driver.dumpPre(boost::make_tuple(inputFile, encoding));
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        return 0;
    }
    else {
        // default: compile and link single php script to native binary
        std::string oFile = outputFile;
        if (oFile.empty()) {
            if (inputFile.find_first_of('.')) {
                oFile = inputFile.substr(0, inputFile.find_first_of('.'));
            }
            else {
                oFile = inputFile+".exe";
            }
        }
        pCompileAndLinkTarget* saTarget = new pCompileAndLinkTarget(inputFile, "/", oFile);
        if (!libSearchPath.empty())
            saTarget->addLibSearchPath(libSearchPath);
        target = saTarget;
    }

    if (!target) {
        // success
        return 0;
    }

    try {
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
