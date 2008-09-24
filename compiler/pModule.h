/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
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

#ifndef RPHP_PMODULE_H_
#define RPHP_PMODULE_H_

#include "pAST.h"

namespace llvm {
    class Module;
}

namespace rphp {

class pCompileTarget;

class pModule {

private:
    std::string fileName;
    AST::treeTop* ast;
    llvm::Module* llvmModule;

public:
    pModule(std::string name): fileName(name), ast(new AST::treeTop()), llvmModule(NULL)
    {

    }

    ~pModule();

    AST::treeTop* getTreeTop() { return ast; }

    void lowerToIR(pCompileTarget* target);
    void writeBitcode(std::string fileName);
    void dumpAST();


};

} // namespace

#endif /* RPHP_PMODULE_H_ */
