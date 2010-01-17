/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2010 Shannon Weyrick <weyrick@roadsend.com>
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

#include "rphp/analysis/passes/DumpAST.h"
#include "rphp/analysis/pSourceModule.h"

#include "tinyxml.h"

#include <iostream>
#include <unicode/ustream.h>

namespace rphp { namespace AST { namespace Pass {

void DumpAST::pre_run(void) {
    doc_ = new TiXmlDocument();
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc_->LinkEndChild(decl);

    currentElement_ = new TiXmlElement( "PHP_source" );
    currentElement_->SetAttribute("file", module_->fileName());
    doc_->LinkEndChild(currentElement_);
}

void DumpAST::post_run(void) {
    TiXmlPrinter printer;
    printer.SetIndent( "    " );
    doc_->Accept( &printer );
    std::cout << printer.CStr();
    delete doc_;
}

void DumpAST::visit_pre_stmt(stmt* n) {
    TiXmlElement* node = new TiXmlElement(nodeDescTable_[n->kind()]);
    currentElement_->LinkEndChild(node);
    currentElement_ = node;
    if (n->startLineNum() != n->endLineNum()) {
        currentElement_->SetAttribute("start_line", n->startLineNum());
        currentElement_->SetAttribute("end_line", n->endLineNum());
    }
    else if (n->startLineNum() != 0) {
        currentElement_->SetAttribute("line", n->startLineNum());
    }
}

void DumpAST::visit_post_stmt(stmt* n) {
    TiXmlNode* xnode = currentElement_->Parent();
    currentElement_ = static_cast<TiXmlElement*>(xnode);
}

void DumpAST::visit_pre_var(var* n) {
    currentElement_->SetAttribute("id",n->name());
}

void DumpAST::visit_pre_classDecl(classDecl* n) {
    currentElement_->SetAttribute("id",n->name());
}

void DumpAST::visit_pre_literalID(literalID* n) {
    currentElement_->SetAttribute("id",n->name());
}

void DumpAST::visit_pre_assignment(assignment* n) {
    currentElement_->SetAttribute("byRef", (n->byRef() ? "true" : "false") );
}

void DumpAST::visit_pre_methodDecl(methodDecl* n) {
    pUInt flags = n->flags();
    if (flags & memberFlags::ABSTRACT)
        currentElement_->SetAttribute("ABSTRACT", "true");
    if (flags & memberFlags::CONST)
        currentElement_->SetAttribute("CONST", "true");
    if (flags & memberFlags::FINAL)
        currentElement_->SetAttribute("FINAL", "true");
    if (flags & memberFlags::PRIVATE)
        currentElement_->SetAttribute("PRIVATE", "true");
    if (flags & memberFlags::PROTECTED)
        currentElement_->SetAttribute("PROTECTED", "true");
    if (flags & memberFlags::PUBLIC)
        currentElement_->SetAttribute("PUBLIC", "true");
    if (flags & memberFlags::STATIC)
        currentElement_->SetAttribute("STATIC", "true");
}

void DumpAST::visit_pre_propertyDecl(propertyDecl* n) {
    currentElement_->SetAttribute("id",n->name());
    pUInt flags = n->flags();
    if (flags & memberFlags::ABSTRACT)
        currentElement_->SetAttribute("ABSTRACT", "true");
    if (flags & memberFlags::CONST)
        currentElement_->SetAttribute("CONST", "true");
    if (flags & memberFlags::FINAL)
        currentElement_->SetAttribute("FINAL", "true");
    if (flags & memberFlags::PRIVATE)
        currentElement_->SetAttribute("PRIVATE", "true");
    if (flags & memberFlags::PROTECTED)
        currentElement_->SetAttribute("PROTECTED", "true");
    if (flags & memberFlags::PUBLIC)
        currentElement_->SetAttribute("PUBLIC", "true");
    if (flags & memberFlags::STATIC)
        currentElement_->SetAttribute("STATIC", "true");
}


void DumpAST::visit_pre_unaryOp(unaryOp* n)  {
    switch (n->opKind()) {
    case unaryOp::LOGICALNOT:
        currentElement_->SetAttribute("op", "LOGICALNOT");
        break;
    case unaryOp::NEGATIVE:
        currentElement_->SetAttribute("op", "NEGATIVE");
        break;
    case unaryOp::POSITIVE:
        currentElement_->SetAttribute("op", "POSITIVE");
        break;
    }
}

void DumpAST::visit_pre_binaryOp(binaryOp* n)  {
    switch (n->opKind()) {
    case binaryOp::CONCAT:
        currentElement_->SetAttribute("op", "CONCAT");
        break;
    case binaryOp::BOOLEAN_AND:
        currentElement_->SetAttribute("op", "BOOOLEAN_AND");
        break;
    case binaryOp::BOOLEAN_OR:
        currentElement_->SetAttribute("op", "BOOLEAN_OR");
        break;
    case binaryOp::BOOLEAN_XOR:
        currentElement_->SetAttribute("op", "BOOLEAN_XOR");
        break;
    case binaryOp::DIV:
        currentElement_->SetAttribute("op", "DIV");
        break;
    case binaryOp::MOD:
        currentElement_->SetAttribute("op", "MOD");
        break;
    case binaryOp::MULT:
        currentElement_->SetAttribute("op", "MULT");
        break;
    case binaryOp::ADD:
        currentElement_->SetAttribute("op", "ADD");
        break;
    case binaryOp::SUB:
        currentElement_->SetAttribute("op", "SUB");
        break;
    case binaryOp::GREATER_THAN:
        currentElement_->SetAttribute("op", "GREATER_THAN");
        break;
    case binaryOp::LESS_THAN:
        currentElement_->SetAttribute("op", "LESS_THAN");
        break;
    case binaryOp::GREATER_OR_EQUAL:
        currentElement_->SetAttribute("op", "GREATER_OR_EQUAL");
        break;
    case binaryOp::LESS_OR_EQUAL:
        currentElement_->SetAttribute("op", "LESS_OR_EQUAL");
        break;
    case binaryOp::EQUAL:
        currentElement_->SetAttribute("op", "EQUAL");
        break;
    case binaryOp::NOT_EQUAL:
        currentElement_->SetAttribute("op", "NOT_EQUAL");
        break;
    case binaryOp::IDENTICAL:
        currentElement_->SetAttribute("op", "IDENTICAL");
        break;
    case binaryOp::NOT_IDENTICAL:
        currentElement_->SetAttribute("op", "NOT_IDENTICAL");
        break;
    case binaryOp::BIT_OR:
        currentElement_->SetAttribute("op", "BIT_OR");
        break;
    case binaryOp::BIT_AND:
        currentElement_->SetAttribute("op", "BIT_AND");
        break;
    case binaryOp::BIT_XOR:
        currentElement_->SetAttribute("op", "BIT_XOR");
        break;
    case binaryOp::INSTANCEOF:
        currentElement_->SetAttribute("op", "INSTANCEOF");
        break;
    }
}

void DumpAST::visit_pre_literalString(literalString* n)  {
    currentElement_->SetAttribute("type", (n->isBinary() ? "binary" : "utf8"));
    currentElement_->SetAttribute("simple", (n->isSimple() ? "yes" : "no"));
    TiXmlText * text = new TiXmlText( n->getStringVal() );
    currentElement_->LinkEndChild( text );
}

void DumpAST::visit_pre_inlineHtml(inlineHtml* n)  {
    visit_pre_literalString(static_cast<literalString*>(n));
}

void DumpAST::visit_pre_literalInt(literalInt* n)  {
    currentElement_->SetAttribute("value", n->getStringVal());
}

void DumpAST::visit_pre_literalFloat(literalFloat* n)  {
    currentElement_->SetAttribute("value", n->getStringVal());
}

void DumpAST::visit_pre_literalBool(literalBool* n)  {
    currentElement_->SetAttribute("value", (n->getBoolVal() ? "true" : "false") );
}

void DumpAST::visit_pre_literalConstant(literalConstant* n)  {
    currentElement_->SetAttribute("id", n->name());
}

//void DumpAST::visit_pre_functionDecl(functionDecl* n) {
//    std::cout << "# line " << n->startLineNum() << "\n";
//    std::cout << "pre_functionDecl\n";
//    /*
//    showindent();
//    std::cout << "# line " << n->startLineNum() << std::endl;
//    showindent();
//    std::cout << "(function declare: " << n->functionDef()->name() << std::endl;
//
//    indent();
//    showindent();
//    std::cout << "(arguments: " << std::endl;
//    indent();
//    for (pFunction::paramListType::size_type i = 0; i < n->functionDef()->numParams(); i++) {
//        showindent();
//        std::cout << i+1 << ": $" << n->functionDef()->param(i)->name() << std::endl;
//        // TODO more info on params
//    }
//    unindent();
//    showindent();
//    std::cout << ")" << std::endl;
//    unindent();
//
//
//    indent();
//    showindent();
//    std::cout << "(body: " << std::endl;
//
//    indent();
//    visit(n->body());
//    unindent();
//
//    showindent();
//    std::cout << ")" << std::endl;
//    unindent();
//
//    showindent();
//    std::cout << ")" << std::endl;
//    */
//}
//

void DumpAST::visit_pre_literalArray(literalArray* n)  {

    TiXmlElement* element(NULL);
    TiXmlElement* key(NULL);
    TiXmlElement* value(NULL);
    TiXmlElement* arrayNode(currentElement_);

    for (arrayList::reverse_iterator i = n->itemList().rbegin();
        i != n->itemList().rend();
        ++i)
    {
        element = new TiXmlElement("element");
        arrayNode->LinkEndChild(element);
        key = new TiXmlElement("key");
        element->LinkEndChild(key);
        if ((*i).key) {
            currentElement_ = key;
            visit((*i).key);
        }
        else {
            key->SetAttribute("next","true");
        }
        value = new TiXmlElement("value");
        element->LinkEndChild(value);
        currentElement_ = value;
        visit((*i).val);
        currentElement_ = arrayNode;
    }
    currentElement_ = arrayNode;
}


void DumpAST::visit_pre_signature(signature* n) {
    currentElement_->SetAttribute("id",n->name());
    currentElement_->SetAttribute("returnByRef", (n->returnByRef() ? "true" : "false") );
}

void DumpAST::visit_pre_formalParam(formalParam* n) {
    currentElement_->SetAttribute("id",n->name());
    currentElement_->SetAttribute("byRef", (n->byRef() ? "true" : "false") );
}

} } } // namespace

