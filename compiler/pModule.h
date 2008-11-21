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
#include "pSourceFile.h"
#include "pAST.h"

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

private:
    const pSourceFile* source_;

    // code generation
    AST::statementList ast_;
    llvm::Module* llvmModule_;
    bool llvmModuleOwner_;

    // error reporting. move to parseContext struct/class/tuple?
    pUInt currentLineNum_;
    pSourceCharIterator lastNewline_;
    pSourceRange lastToken_;

public:
    pModule(pSourceFileDesc file);
    ~pModule();

    // INSPECTION
    std::string getEntryFunctionName();
    const pSourceFile* source() const { return source_; }
    const pFileNameString& fileName() const { return source_->fileName(); }
    const char* encoding(void) const { return source_->encoding().c_str(); }

    // GENERATION
    AST::statementList& getAST() { return ast_; }
    void applyVisitor(AST::baseVisitor* v);
    bool lowerToIR(pCompileTarget* target);
    void writeBitcode(pFileNameString fileName);
    void setLLVMModuleOwnership(bool v) { llvmModuleOwner_ = v; }
    llvm::Module* getLLVMModule() { return llvmModule_; }

    // PARSING
    pUInt currentLineNum() const { return currentLineNum_; }
    void incLineNum(void) { currentLineNum_++; }
    void incLineNum(pUInt i) { currentLineNum_ +=i; }

    void setLastNewline(pSourceCharIterator i) { lastNewline_ = i; }
    const pSourceCharIterator& lastNewline(void) const { return lastNewline_; }

    void setLastToken(pSourceRange i) { lastToken_ = i; }
    const pSourceRange& lastToken(void) const { return lastToken_; }

    void parseError(pSourceRange* r);

    // DEBUG
    void dumpAST();
    void dumpIR();


};

} // namespace

#endif /* RPHP_PMODULE_H_ */
