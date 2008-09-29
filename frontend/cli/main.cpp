
#include <iostream>
#include <vector>
#include <string>
#include <llvm/Support/CommandLine.h>

#include "pDriver.h"
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

    cl::opt<std::string> outputFile ("o",cl::desc("Output file name"));
    cl::opt<std::string> mainFile ("main-file",cl::desc("Main entry script for stand alone programs"));

    cl::opt<bool> compileModule ("c", cl::desc("Compile a single source file to bitcode object file"));
    cl::opt<bool> linkSA ("link-sa", cl::desc("Link objects to stand alone executable"));

    cl::SetVersionPrinter(&rphpVersion);
    cl::ParseCommandLineOptions(argc, argv, "Roadsend PHP");

    pTarget* target = NULL;
    if (compileModule) {
        target = new pCompileTarget(inputFile, "/");
    }
    else if (linkSA) {
        pStandAloneTarget* saTarget = new pStandAloneTarget(outputFile, mainFile);
        saTarget->addInputFile(inputFile);
        target = saTarget;
    }
    else if (dumpToks) {
        pDriver driver;
        driver.dumpTokens(inputFile);
        return 0;
    }
    else if (dumpAST) {
        pDriver driver;
        driver.dumpAST(inputFile);
        return 0;
    }

    if (!target) {
        std::cout << "nothing to do" << std::endl;
        return 1;
    }

    try {
        target->execute();
    }
    catch (...) {
        std::cerr << "problem executing target" << std::endl;
    }

    delete target;

/*
    if (dumpToks) {
        driver.dumpTokens(inputFile);
    }
    else if (dumpIR) {
        driver.dumpIR(inputFile);
    }
    else if (dumpAST) {
        driver.dumpAST(inputFile);
    }
    else if (iBC) {
        driver.executeBC(inputFile);
    }
    else {
        driver.compileToBC(inputFile);
    }
*/

    /*
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("i", "execute the given file (php source or precompiled bytecode)")
        ("compile-target", po::value<std::string>(), "one of [native, bc, asm]")
        ("dump-toks", "dump tokens from lexer")
        ("dump-ast", "dump AST")
        ("dump-ir", "dump LLVM IR (assembly)")
//        ("optimization", po::value<int>(&opt)->default_value(10), "optimization level")
        ("input-file", po::value< std::vector<std::string> >(), "input file")
        ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
            options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help") || !vm.count("input-file")) {
        showHelp(desc);
        return 1;
    }


    // TODO: make this a map of function pointers, or something nicer.
    if (vm.count("dump-toks")) {
        std::vector<std::string> infiles = vm["input-file"].as< std::vector<std::string> >();
        for (std::vector<std::string>::iterator it = infiles.begin(); it!=infiles.end(); ++it) {
            driver.dumpTokens(*it);
        }
    }
    else if (vm.count("dump-ast")) {
        std::vector<std::string> infiles = vm["input-file"].as< std::vector<std::string> >();
        for (std::vector<std::string>::iterator it = infiles.begin(); it!=infiles.end(); ++it) {
            driver.dumpAST(*it);
        }
    }
    else if (vm.count("dump-ir")) {
        std::vector<std::string> infiles = vm["input-file"].as< std::vector<std::string> >();
        for (std::vector<std::string>::iterator it = infiles.begin(); it!=infiles.end(); ++it) {
            driver.dumpIR(*it);
        }
    }
    else if (vm.count("i")) {
        std::vector<std::string> infiles = vm["input-file"].as< std::vector<std::string> >();
        for (std::vector<std::string>::iterator it = infiles.begin(); it!=infiles.end(); ++it) {
            driver.execute(*it);
        }
    }
    else if (vm.count("compile-target")) {
        std::vector<std::string> infiles = vm["input-file"].as< std::vector<std::string> >();
        for (std::vector<std::string>::iterator it = infiles.begin(); it!=infiles.end(); ++it) {
            if (vm["compile-target"].as<std::string>() == "bc") {
                driver.compileToBC(*it);
            }
            else if (vm["compile-target"].as<std::string>() == "asm") {
                driver.compileToAsm(*it);
            }
            else if (vm["compile-target"].as<std::string>() == "native") {
                driver.compileToNative(*it);
            }
            else {
                std::cerr << "unknown target: " << vm["compile-target"].as<std::string>() << std::endl;
                showHelp(desc);
                return 1;
            }
        }
    }
    else {
        showHelp(desc);
        return 1;
    }
    */

    return 0;

}
