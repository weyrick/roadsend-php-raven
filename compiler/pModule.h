/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#include <vector>

namespace llvm {
    class Module;
}

namespace rphp {

namespace AST {
    class stmt;
    class baseVisitor;
}

class pCompileTarget;

class pModule {
public:
    typedef std::vector<AST::stmt*> astType;

private:
    std::string fileName;
    astType ast;
    llvm::Module* llvmModule;
    bool llvmModuleOwner;
    bool defaultUnicode_;

public:
    pModule(std::string name, bool dUnicode = false);
    ~pModule();

    astType& getAST() { return ast; }
    llvm::Module* getLLVMModule() { return llvmModule; }

    std::string getEntryFunctionName();

    void applyVisitor(AST::baseVisitor* v);
    bool lowerToIR(pCompileTarget* target);
    bool writeBitcode(std::string fileName);
    void setLLVMModuleOwnership(bool v) { llvmModuleOwner = v; }

    bool defaultUnicode(void) const { return defaultUnicode_; }
    // XXX this is temporary. called from the parser for now, so
    // that it's not hard coded
    const char* encoding(void) const { return "UTF-16"; }

    void dumpAST();
    void dumpIR();


};

} // namespace

#endif /* RPHP_PMODULE_H_ */
