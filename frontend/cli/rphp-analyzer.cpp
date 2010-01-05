/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009-2010 Shannon Weyrick <weyrick@roadsend.com>
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
#include <llvm/System/Path.h>

#include <boost/algorithm/string.hpp>

#include "rphp/analysis/pPassManager.h"
#include "rphp/analysis/pSourceModule.h"
#include "rphp/analysis/pSourceFile.h"
#include "rphp/analysis/pLexer.h"

#include "rphp/analysis/passes/DumpAST.h"
#include "rphp/analysis/passes/DumpStats.h"
#include "rphp/analysis/passes/SimplifyStrings.h"

using namespace llvm;
using namespace rphp;

void rphpVersion(void) {
    std::cout << "Roadsend PHP Source Analyzer" << std::endl;
}


int main( int argc, char* argv[] )
{

    // command line options
    cl::list<std::string> inputFiles(cl::Positional, cl::desc("<input files>"), cl::OneOrMore);

    cl::opt<bool> dumpToks ("dump-toks", cl::desc("Dump tokens from lexer"));
    cl::opt<bool> dumpAST ("dump-ast", cl::desc("Dump AST"));
    cl::opt<bool> debugParse ("debug-parse", cl::desc("Debug output from parser"));

    cl::opt<std::string> passListText ("passes", cl::desc("List of passes to run"));

    cl::opt<std::string> encoding ("encoding",cl::desc("Character encoding of the source file"), cl::init("UTF-8"));

    cl::SetVersionPrinter(&rphpVersion);
    cl::ParseCommandLineOptions(argc, argv, "Roadsend PHP Analyzer");

    pSourceModule* unit(0);

    for (unsigned i = 0; i != inputFiles.size(); ++i) {

        pSourceFileDesc inFile(inputFiles[i], encoding);

        if (dumpToks) {
            // no pass, just a token dump
            pSourceFile source(inFile);
            lexer::pLexer l(&source);
            l.dumpTokens();
        }

        try {
            // catch parse errors
            unit = new pSourceModule(inFile);
            unit->parse(debugParse);
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            continue;
        }

        pPassManager passManager(unit);

        if (dumpAST) {
            passManager.addPass<AST::Pass::SimplifyStrings>();
            passManager.addPass<AST::Pass::DumpAST>();
            passManager.addPass<AST::Pass::DumpStats>();
        }
        else if (!passListText.empty()) {
            // custom list of passes
            std::vector<std::string> passes;
            boost::split(passes, passListText, boost::is_any_of(","));
            for (std::vector<std::string>::iterator i = passes.begin();
            i != passes.end();
            ++i) {
                if (*i == "dumpast") {
                    passManager.addPass<AST::Pass::DumpAST>();
                }
                else if (*i == "simplifystrings") {
                    passManager.addPass<AST::Pass::SimplifyStrings>();
                }
            }
        }
        else {
            std::cout << "no action\n";
            return 1;
        }

        try {
            // run selected passes
            passManager.run();
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }

        // free for next input file
        delete unit;

    } // input file loop

    return 0;

}
