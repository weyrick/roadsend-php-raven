
#include <iostream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include "pDriver.h"
#include "pRuntime.h"

namespace po = boost::program_options;

int main( int argc, char* argv[] )
{

    rphp::pRuntimeEngine runtime;
    rphp::pDriver driver;

    //int opt;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
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
        std::cout << "Roadsend PHP" << std::endl;
        std::cout << desc << "\n";
        return 1;
    }


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
    else {
        std::cout << "Roadsend PHP" << std::endl;
        std::cout << desc << "\n";
        return 1;
    }

    return 0;

}
