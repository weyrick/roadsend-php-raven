
#include <iostream>
#include <vector>
#include <string>
#include <llvm/Support/CommandLine.h>
#include "pDriver.h"
#include "pRuntime.h"

using namespace llvm;

void rphpVersion(void) {
    // TODO get version info from runtime
    std::cout << "Roadsend PHP" << std::endl;
}

int main( int argc, char* argv[] )
{

    rphp::pRuntimeEngine runtime;
    rphp::pDriver driver;

    // command line options
    cl::opt<std::string> inputFile(cl::Positional, cl::desc("<input file>"), cl::Required);
    cl::opt<bool> dumpToks ("dump-toks", cl::desc("Dump tokens from lexer"));

    cl::SetVersionPrinter(&rphpVersion);
    cl::ParseCommandLineOptions(argc, argv, "Roadsend PHP");

    if (dumpToks) {
        driver.dumpTokens(inputFile);
    }

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
