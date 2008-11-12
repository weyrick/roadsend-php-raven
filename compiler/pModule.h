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

#include "pCompilerTypes.h"

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
    pFilenameString filename_;
    astType ast_;
    llvm::Module* llvmModule_;
    bool llvmModuleOwner_;
    bool defaultUnicode_;

    // error reporting
    pUInt currentLineNum_;
    sourceIteratorType lastNewline_;
    sourceRangeType lastToken_;

public:
    pModule(pFilenameString name, bool dUnicode = false);
    ~pModule();

    astType& getAST() { return ast_; }
    llvm::Module* getLLVMModule() { return llvmModule_; }

    std::string getEntryFunctionName();

    const pFilenameString& filename() { return filename_; }

    void applyVisitor(AST::baseVisitor* v);
    bool lowerToIR(pCompileTarget* target);
    bool writeBitcode(pFilenameString filename);
    void setLLVMModuleOwnership(bool v) { llvmModuleOwner_ = v; }

    // during parse
    bool defaultUnicode(void) const { return defaultUnicode_; }
    // XXX this is temporary. called from the parser for now, so
    // that it's not hard coded
    const char* encoding(void) const { return "UTF-16"; }

    pUInt currentLineNum() const { return currentLineNum_; }
    void incLineNum(void) { currentLineNum_++; }
    void incLineNum(pUInt i) { currentLineNum_ +=i; }

    void setLastNewline(sourceIteratorType i) { lastNewline_ = i; }
    const sourceIteratorType& lastNewline(void) const { return lastNewline_; }

    void setLastToken(sourceRangeType i) { lastToken_ = i; }
    const sourceRangeType& lastToken(void) const { return lastToken_; }

    void dumpAST();
    void dumpIR();


};

} // namespace

#endif /* RPHP_PMODULE_H_ */
