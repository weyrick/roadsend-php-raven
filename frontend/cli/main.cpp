
#include <iostream>
#include <vector>
#include <string>
#include <llvm/Support/CommandLine.h>

#include "pDriver.h"
#include "pInterpretTarget.h"
#include "pCompileTarget.h"
#include "pStandAloneTargets.h"

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
    cl::opt<bool> iBC ("i", cl::desc("Interpret bytecode"));
    cl::opt<bool> iSF ("f", cl::desc("Execute source file immediately"));

    cl::opt<std::string> outputFile ("o",cl::desc("Output file name"));
    cl::opt<std::string> mainFile ("main-file",cl::desc("Main entry script for stand alone programs"));

    cl::opt<bool> compileModule ("c", cl::desc("Compile a single source file to bitcode object file"));
    cl::opt<bool> linkSA ("link-sa", cl::desc("Link objects to stand alone executable"));

    cl::SetVersionPrinter(&rphpVersion);
    cl::ParseCommandLineOptions(argc, argv, "Roadsend PHP");

    pDriver driver;
    pTarget* target = NULL;
    if (compileModule) {
        target = new pCompileTarget(inputFile, "/");
    }
    else if (iSF) {
        target = new pInterpretTarget(inputFile, "/");
    }
    else if (linkSA) {
        pStandAloneTarget* saTarget = new pStandAloneTarget(outputFile, mainFile);
        saTarget->addInputFile(inputFile);
        target = saTarget;
    }
    else if (dumpToks) {
        driver.dumpTokens(inputFile);
        return 0;
    }
    else if (dumpAST) {
        driver.dumpAST(inputFile);
        return 0;
    }
    else if (dumpIR) {
        driver.dumpIR(inputFile);
    }

    if (!target) {
        return 0;
    }

    try {
        target->execute();
    }
    catch (...) {
        std::cerr << "problem executing target" << std::endl;
    }

    delete target;

    return 0;

}
