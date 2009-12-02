/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PSOURCEMODULE_H_
#define RPHP_PSOURCEMODULE_H_

#include "rphp/pSourceTypes.h"
#include "rphp/analysis/pAST.h"
#include "rphp/analysis/pParseContext.h"

namespace rphp {

class pSourceFile;

namespace AST {
    class stmt;
    class baseVisitor;
}

class pSourceModule {

private:
    const pSourceFile* source_;
    AST::statementList ast_;
    AST::pParseContext context_;

public:
    pSourceModule(const pSourceFileDesc& file);
    ~pSourceModule();

    // INSPECTION
    const pSourceFile* source() const { return source_; }
    const pFileNameString& fileName() const;
    const char* encoding(void) const;

    const AST::pParseContext& context(void) const { return context_; }
    AST::pParseContext& context(void) { return context_; }

    // AST TRAVERSAL
    AST::statementList& getAST() { return ast_; }
    void setAST(const AST::statementList* list) { ast_.assign(list->begin(), list->end()); }
    void applyVisitor(AST::baseVisitor* v);

    // DEBUG
    void dumpAST();

};

} // namespace

#endif /* RPHP_PSOURCEMODULE_H_ */
