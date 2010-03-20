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
;; This program is distributed in the hope that it will be u    seful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
   ***** END LICENSE BLOCK *****
*/

#include "rphp/analysis/pSourceModule.h"
#include "rphp/analysis/pSourceFile.h"

#include "rphp/analysis/pBaseVisitor.h"
#include "rphp/analysis/pBaseTransformer.h"
#include "rphp/analysis/pParser.h"

namespace rphp {

pSourceModule::pSourceModule(const pSourceFileDesc& file):
    source_(new pSourceFile(file)),
    ast_(NULL),
    context_(this)
{


}

void pSourceModule::parse(bool debug=false) {

    parser::parseSourceFile(this,debug);

}

pSourceModule::~pSourceModule() {
    // cleanup AST
    ast_->destroy(context_);
    delete source_;
}

const pFileNameString& pSourceModule::fileName() const {
    return source_->fileName();
}

const char* pSourceModule::encoding(void) const {
    return source_->encoding().value().c_str();
}

void pSourceModule::setAST(const AST::statementList* list) {
    if (ast_)
        ast_->destroy(context_);
    ast_ = new (context_) AST::block(context_, list);
}

void pSourceModule::applyVisitor(AST::pBaseVisitor* v) {
    assert(ast_);
    v->visit(ast_);
}

void pSourceModule::applyTransform(AST::pBaseTransformer* t) {
    assert(ast_);
    ast_ = cast<AST::block>(t->transform(ast_));
}

} // namespace
