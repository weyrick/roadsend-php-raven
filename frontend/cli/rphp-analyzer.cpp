/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009 Shannon Weyrick <weyrick@roadsend.com>
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

#include "rphp/analysis/pPassManager.h"
#include "rphp/analysis/pSourceModule.h"
#include "rphp/analysis/pSourceFile.h"
#include "rphp/analysis/pLexer.h"

#include "rphp/analysis/passes/DumpAST.h"
#include "rphp/analysis/passes/SimplifyStrings.h"

using namespace llvm;
using namespace rphp;

void rphpVersion(void) {
    std::cout << "Roadsend PHP Source Analyzer" << std::endl;
}


int main( int argc, char* argv[] )
{

    // command line options
    cl::opt<std::string> inputFile(cl::Positional, cl::desc("<input file>"), cl::Required);
    cl::opt<bool> dumpToks ("dump-toks", cl::desc("Dump tokens from lexer"));
    cl::opt<bool> dumpAST ("dump-ast", cl::desc("Dump AST"));

    cl::opt<std::string> passListText ("passes", cl::desc("List of passes to run"));

    cl::opt<std::string> encoding ("encoding",cl::desc("Character encoding of the source file"));

    cl::SetVersionPrinter(&rphpVersion);
    cl::ParseCommandLineOptions(argc, argv, "Roadsend PHP Analyzer");

    // default encoding
    if (encoding.empty())
        encoding = "UTF-8";

    assert(!inputFile.empty() && "empty input file");

    pSourceFileDesc inFile(inputFile, encoding);
    pSourceModule unit(inFile);
    pPassManager passManager;

    if (dumpToks) {
        // no pass, just a token dump
        pSourceFile* source = new pSourceFile(inFile);
        lexer::pLexer l(source);
        l.dumpTokens();
        delete source;
        return 0;
    }
    else if (dumpAST) {
        passManager.addPass<AST::Pass::SimplifyStrings>();
        passManager.addPass<AST::Pass::DumpAST>();
    }
    else if (!passListText.empty()) {
        // custom list of passes
        std::cout << "Custom pass list\n";
    }
    else {
        std::cout << "no action\n";
        return 1;
    }

    // run selected passes
    passManager.run(&unit);

    return 0;

}
