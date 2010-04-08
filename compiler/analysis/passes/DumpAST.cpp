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


void DumpAST::visitOrNullChild(stmt* n) {
    if (n)
        visit(n);
    else
        doNullChild();
}

void DumpAST::doComment(const char* c) {
    TiXmlComment* node = new TiXmlComment(c);
    currentElement_->LinkEndChild(node);
}

void DumpAST::doNullChild(void) {
    TiXmlElement* node = new TiXmlElement("NULL");
    currentElement_->LinkEndChild(node);
}

void DumpAST::visit_pre_var(var* n) {
    currentElement_->SetAttribute("id",n->name());
    if (n->indirectionCount()) {
        currentElement_->SetAttribute("indirectionCount",n->indirectionCount());
    }
}

void DumpAST::visit_pre_classDecl(classDecl* n) {

    currentElement_->SetAttribute("id",n->name());
    TiXmlElement* sub, *node;

    switch (n->classType()) {
    case classDecl::ABSTRACT:
        currentElement_->SetAttribute("type","ABSTRACT");
        break;
    case classDecl::FINAL:
        currentElement_->SetAttribute("type","FINAL");
        break;
    case classDecl::IFACE:
        currentElement_->SetAttribute("type","INTERFACE");
        break;
    case classDecl::NORMAL:
        currentElement_->SetAttribute("type","NORMAL");
        break;
    }

    if (n->implementsCount()) {

        sub = new TiXmlElement("implements");
        for (AST::idList::iterator i = n->implements_begin();
             i != n->implements_end();
             ++i)
        {
            // synthesize literal id nodes here
            node = new TiXmlElement("literalID");
            node->SetAttribute("id", *(*i));
            sub->LinkEndChild(node);
        }
        currentElement_->LinkEndChild(sub);

    }
    else {
        doComment("implements none");
    }

    if (n->extendsCount()) {

        sub = new TiXmlElement("extends");
        for (AST::idList::iterator i = n->extends_begin();
             i != n->extends_end();
             ++i)
        {
            // synthesize literal id nodes here
            node = new TiXmlElement("literalID");
            node->SetAttribute("id", *(*i));
            sub->LinkEndChild(node);
        }
        currentElement_->LinkEndChild(sub);

    }
    else {
        doComment("extends none");
    }


}

void DumpAST::visit_pre_literalID(literalID* n) {
    currentElement_->SetAttribute("id",n->name());
}

void DumpAST::visit_pre_assignment(assignment* n) {
    currentElement_->SetAttribute("byRef", (n->byRef() ? "true" : "false") );
}

bool DumpAST::visit_children_staticDecl(staticDecl* n) {
    doComment("default value");
    visitOrNullChild(n->defaultExpr());
    doComment("variable list");
    visitChildren(n->var_begin(), n->var_end());
    return true;
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

bool DumpAST::visit_children_methodDecl(methodDecl* n) {
    visit(n->sig());
    doComment("method body");
    visitOrNullChild(n->body());
    return true;
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
    case unaryOp::BITWISENOT:
        currentElement_->SetAttribute("op", "BITWISENOT");
        break;
    }
}

void DumpAST::visit_pre_builtin(builtin* n)  {
    switch (n->opKind()) {
    case builtin::CLONE:
        currentElement_->SetAttribute("op", "CLONE");
        break;
    case builtin::ECHO:
        currentElement_->SetAttribute("op", "ECHO");
        break;
    case builtin::EMPTY:
        currentElement_->SetAttribute("op", "EMPTY");
        break;
    case builtin::EXIT:
        currentElement_->SetAttribute("op", "EXIT");
        break;
    case builtin::ISSET:
        currentElement_->SetAttribute("op", "ISSET");
        break;
    case builtin::UNSET:
        currentElement_->SetAttribute("op", "UNSET");
        break;
    case builtin::PRINT:
        currentElement_->SetAttribute("op", "PRINT");
        break;
    case builtin::THROW:
        currentElement_->SetAttribute("op", "THROW");
        break;
    case builtin::IGNORE_WARNING:
        currentElement_->SetAttribute("op", "IGNORE_WARNING");
        break;
    case builtin::INCLUDE:
        currentElement_->SetAttribute("op", "INCLUDE");
        break;
    case builtin::INCLUDE_ONCE:
        currentElement_->SetAttribute("op", "INCLUDE_ONCE");
        break;
    case builtin::REQUIRE:
        currentElement_->SetAttribute("op", "REQUIRE");
        break;
    case builtin::REQUIRE_ONCE:
        currentElement_->SetAttribute("op", "REQUIRE_ONCE");
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
    case binaryOp::SHIFT_LEFT:
        currentElement_->SetAttribute("op", "SHIFT_LEFT");
        break;
    case binaryOp::SHIFT_RIGHT:
        currentElement_->SetAttribute("op", "SHIFT_RIGHT");
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
    currentElement_->SetAttribute("arrayHint", (n->arrayHint() ? "true" : "false") );
    pIdentString hint = n->classHint();
    if (hint.length())
        currentElement_->SetAttribute("classHint",hint);
}

bool DumpAST::visit_children_formalParam(formalParam* n) {
    doComment("default value");
    visitOrNullChild(n->defaultExpr());
    return true;
}

void DumpAST::visit_pre_label(label* n) {
    currentElement_->SetAttribute("id", n->labelNo());
}

void DumpAST::visit_pre_branch(branch* n) {
    currentElement_->SetAttribute("trueLabel", n->trueLabel());
    currentElement_->SetAttribute("falseLabel", n->falseLabel());
}

void DumpAST::visit_pre_typeCast(typeCast* n) {
    switch(n->castKind()) {

        case typeCast::STRING:
            currentElement_->SetAttribute("castKind", "string");
            break;
        case typeCast::BINARY:
            currentElement_->SetAttribute("castKind", "binary");
            break;
        case typeCast::UNICODE:
            currentElement_->SetAttribute("castKind", "unicode");
            break;
        case typeCast::INT:
            currentElement_->SetAttribute("castKind", "int");
            break;
        case typeCast::REAL:
            currentElement_->SetAttribute("castKind", "float");
            break;
        case typeCast::BOOL:
            currentElement_->SetAttribute("castKind", "bool");
            break;
        case typeCast::UNSET:
            currentElement_->SetAttribute("castKind", "unset");
            break;
        case typeCast::ARRAY:
            currentElement_->SetAttribute("castKind", "array");
            break;
        case typeCast::OBJECT:
            currentElement_->SetAttribute("castKind", "object");
            break;
    }

}

} } } // namespace

