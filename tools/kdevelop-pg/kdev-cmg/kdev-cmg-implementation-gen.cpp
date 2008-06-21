/* This file is part of kdev-cmg
   Copyright (C) 2006 Jakob Petsovits <jpetso@gmx.at>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kdev-cmg-implementation-gen.h"

#include "kdev-cmg.h"
#include "kdev-cmg-string-tools.h"

#include <iostream>


namespace kdevcmg
{

void generate_implementation::operator()()
{
  visit_node(_G_system.ast);
}

void generate_implementation::visit_class_declaration(class_declaration_ast *node)
{
  out << "// ---------------------------------------------------------------------------" << std::endl;
  default_visitor::visit_class_declaration(node);
}

void generate_implementation::visit_code_model_declaration(code_model_declaration_ast *node)
{
  current_class = "CodeModel";
  out << "#define CLASS " << current_class << std::endl
      << "#define BASECLASS KDevelop::CodeModel" << std::endl
      << std::endl

      << "CodeModel::CodeModel( QObject *parent )" << std::endl
      << ": KDevelop::CodeModel( parent )" << std::endl;

  generate_member_initializations __inits(out);
  __inits(node);

  out << "{" << std::endl;

  generate_code __code(out);
  __code(node, code_item::type_constructor);

  out << "}" << std::endl
      << std::endl

      << "CodeModel::~CodeModel()" << std::endl
      << "{" << std::endl;

  __code(node, code_item::type_destructor);

  out << "}" << std::endl

      << "void CodeModel::wipeout()" << std::endl
      << "{" << std::endl
      << "}" << std::endl
      << std::endl

      << "void CodeModel::addCodeItem(CodeModelItem item)" << std::endl
      << "{" << std::endl
      << "beginAppendItem(item);" << std::endl
      << "endAppendItem();" << std::endl
      << "}" << std::endl
      << std::endl

      << "void CodeModel::removeCodeItem(CodeModelItem item)" << std::endl
      << "{" << std::endl
      << "beginRemoveItem(item);" << std::endl
      << "endRemoveItem();" << std::endl
      << "}" << std::endl
      << std::endl;

  default_visitor::visit_code_model_declaration(node);

  out << "#undef CLASS" << std::endl
      << "#undef BASECLASS" << std::endl
      << std::endl;
}

void generate_implementation::visit_root_item(root_item_ast *node)
{
  out << node->item->type->type_name << " CodeModel::"
      << node->item->name->identifier << "() const" << std::endl
      << "{" << std::endl
      << "return _M_" << node->item->name->identifier << ";" << std::endl
      << "}" << std::endl
      << std::endl

      << "KDevelop::ItemCollection *CodeModel::root() const" << std::endl
      << "{" << std::endl
      << "return _M_" << node->item->name->identifier << ";" << std::endl
      << "}" << std::endl
      << std::endl;
}

void generate_implementation::visit_item_declaration(item_declaration_ast *node)
{
  current_class = "_" + node->type->type_name;
  out << "#define CLASS " << current_class << std::endl;

  if (node->type->type_name == "CodeModelItem")
    out << "#define BASECLASS KDevelop::CodeItem" << std::endl;
  else if (node->base_type)
    out << "#define BASECLASS _" << node->base_type->type_name << std::endl;
  else
    out << "#define BASECLASS has_no_base_class!" << std::endl;

  out << std::endl;

  out << current_class << "::" << current_class
      << "(CodeModel *model, int kind)" << std::endl;

  if (node->type->type_name == "CodeModelItem")
    {
      out << ": KDevelop::CodeItem( QString::null, 0 )" << std::endl
          << ", _M_model(model)" << std::endl
          << ", _M_kind(kind)" << std::endl
          << ", _M_startLine(-1)" << std::endl
          << ", _M_startColumn(-1)" << std::endl
          << ", _M_endLine(-1)" << std::endl
          << ", _M_endColumn(-1)" << std::endl;
    }
  else
    {
      if (node->base_type)
        out << ": _" << node->base_type->type_name << "(model, kind)";
    }

  generate_member_initializations __inits(out);
  __inits(node);

  out << "{" << std::endl;

  generate_code __code(out);
  __code(node, code_item::type_constructor);

  out << "}" << std::endl
      << std::endl;

  out << current_class << "::~" << current_class << "()"
      << "{" << std::endl;

  __code(node, code_item::type_destructor);

  out << "}" << std::endl;

  if (node->type->type_name != "CodeModelItem")
    {
      out << node->type->type_name << " " << current_class
          << "::create(CodeModel *model)" << std::endl
          << "{" << std::endl
          << node->type->type_name << " item("
          << "new " << current_class << "(model));" << std::endl
          << "return item;" << std::endl
          << "}" << std::endl
          << std::endl;
    }
  else
    {
      out << "_CodeModelItem *_CodeModelItem::itemAt(int index) const" << std::endl
          << "{" << std::endl
          << "return static_cast<_CodeModelItem*>(KDevelop::ItemCollection::itemAt(index));" << std::endl
          << "}" << std::endl
          << std::endl

          << "CodeModelItem _CodeModelItem::toItem() const" << std::endl
          << "{" << std::endl
          << "return CodeModelItem(const_cast<_CodeModelItem*>(this));" << std::endl
          << "}" << std::endl
          << std::endl

          << "int _CodeModelItem::kind() const" << std::endl
          << "{" << std::endl
          << "return _M_kind;" << std::endl
          << "}" << std::endl
          << std::endl

          << "void _CodeModelItem::setKind(int kind)" << std::endl
          << "{" << std::endl
          << "_M_kind = kind;" << std::endl
          << "}" << std::endl
          << std::endl

          << "KTextEditor::Cursor _CodeModelItem::startPosition() const" << std::endl
          << "{" << std::endl
          << "return KTextEditor::Cursor(_M_startLine, _M_startColumn);" << std::endl
          << "}" << std::endl
          << std::endl

          << "void _CodeModelItem::setStartPosition(const KTextEditor::Cursor& cursor)" << std::endl
          << "{" << std::endl
          << "_M_startLine = cursor.line();" << std::endl
          << "_M_startColumn = cursor.column();" << std::endl
          << "}" << std::endl
          << std::endl

          << "KTextEditor::Cursor _CodeModelItem::endPosition() const" << std::endl
          << "{" << std::endl
          << "return KTextEditor::Cursor(_M_endLine, _M_endColumn);" << std::endl
          << "}" << std::endl
          << std::endl

          << "void _CodeModelItem::setEndPosition(const KTextEditor::Cursor& cursor)" << std::endl
          << "{" << std::endl
          << "_M_endLine = cursor.line();" << std::endl
          << "_M_endColumn = cursor.column();" << std::endl
          << "}" << std::endl
          << std::endl;
    }

  default_visitor::visit_item_declaration(node);

  out << "#undef CLASS" << std::endl
      << "#undef BASECLASS" << std::endl
      << std::endl;
}

void generate_implementation::visit_non_item_type_declaration(non_item_type_declaration_ast *node)
{
  current_class = node->type->type_name;
  out << "#define CLASS " << current_class << std::endl;

  if (node->base_type)
    out << "#define BASECLASS " << node->base_type->type_name << std::endl;
  else
    out << "#define BASECLASS has_no_base_class!" << std::endl;

  out << std::endl;

  default_visitor::visit_non_item_type_declaration(node);

  out << "#undef CLASS" << std::endl
      << "#undef BASECLASS" << std::endl
      << std::endl;
}

void generate_implementation::visit_member_item(member_item_ast *node)
{
  if (!node->is_list)
    {
      // getter
      out << node->type->type_name << " "
          << current_class << "::" << node->name->identifier
          << "() const" << std::endl
          << "{" << std::endl
          << "return _M_" << node->name->identifier << ";" << std::endl
          << "}" << std::endl
          << std::endl;

      // setter
      out << "void "
          << current_class << "::" << string_tools::accessor_name("set", node->name->identifier)
          << "(" << node->type->type_name << " " << node->name->identifier << ")" << std::endl
          << "{" << std::endl
          << "_M_" << node->name->identifier
          << " = " << node->name->identifier << ";" << std::endl
          << "}" << std::endl
          << std::endl;
    }
  else
    {
      bool isChildItem = false;

      if (node->options && node->options->childitem)
        isChildItem = true;

      // getter
      out << node->type->raw_type_name << "List "
          << current_class << "::" << string_tools::plural(node->name->identifier)
          << "() const" << std::endl
          << "{" << std::endl
          << "return _M_" << string_tools::plural(node->name->identifier);

      if (node->options && node->options->hashed)
        out << ".values()";

      out << ";" << std::endl
          << "}" << std::endl
          << std::endl;

      // setter: add
      out << "void " << current_class << "::"
          << string_tools::accessor_name("add", node->name->identifier)
          << "(" << node->type->type_name << " item)" << std::endl
          << "{" << std::endl;

      if (node->options && node->options->hashed_member)
        {
          out << "if ( "
              << "_M_" << string_tools::plural(node->name->identifier) << ".contains("
              << "item->" << node->options->hashed_member->identifier << "()"
              << ") )" << std::endl
              << string_tools::accessor_name("remove", node->name->identifier) << "("
              << "_M_" << string_tools::plural(node->name->identifier) << "["
              << "item->" << node->options->hashed_member->identifier << "()"
              << "]" << ");" << std::endl
              << std::endl;
        }
      else if (node->options && node->options->multihashed_member)
        {
          out << "foreach( _" << node->type->type_name << " *existingItem, " << std::endl
              << "_M_" << string_tools::plural(node->name->identifier) << ".values("
              << "item->" << node->options->multihashed_member->identifier << "()"
              << ") )" << std::endl;

          if (node->options->unique_condition)
            out << "if (!( " << node->options->unique_condition->code
                << " ))" << std::endl;

          out << string_tools::accessor_name("remove", node->name->identifier)
              << "(existingItem);" << std::endl;
        }

      if ( isChildItem )
        out << "model()->beginAppendItem(item, this);" << std::endl;

      out << "_M_" << string_tools::plural(node->name->identifier);

      if (node->options && node->options->hashed_member)
        {
          out << ".insertMulti(item->"
              << node->options->hashed_member->identifier << "()"
              << ", item);" << std::endl;
        }
      else if (node->options && node->options->multihashed_member)
        {
          out << ".insert(item->"
              << node->options->multihashed_member->identifier << "()"
              << ", item);" << std::endl;
        }
      else // not a hashed list
        {
          out << ".append(item);" << std::endl;
        }

      if ( isChildItem )
        out << "model()->endAppendItem();" << std::endl;

      out << "}" << std::endl;

      // setter: remove
      out << "void " << current_class << "::"
          << string_tools::accessor_name("remove", node->name->identifier)
          << "(" << node->type->type_name << " item)" << std::endl
          << "{" << std::endl;

      if (node->options && node->options->multihashed_member)
        {
          std::string hash_type = _G_system.type_of(
            node->options->multihashed_member->identifier, node->type->type_name );

          out << "QMultiHash<" << hash_type << ", " << node->type->type_name << ">::Iterator it"
              << "= _M_" << string_tools::plural(node->name->identifier)
              << ".find(item->name());" << std::endl
              << std::endl;

          out << "while (it != _M_" << string_tools::plural(node->name->identifier) << ".end() "
              << "&& it.key() == item->"
              << node->options->multihashed_member->identifier << "()" << std::endl
              << "&& it.value() != item)" << std::endl
              << "{" << std::endl
              << "++it;" << std::endl
              << "}" << std::endl
              << std::endl;

          out << "if (it != _M_" << string_tools::plural(node->name->identifier) << ".end()"
              << " && it.value() == item)" << std::endl
              << "{" << std::endl;
        }

      if ( isChildItem )
        out << "model()->beginRemoveItem(item);" << std::endl;

      out << "_M_" << string_tools::plural(node->name->identifier);

      if (node->options && node->options->hashed_member)
        {
          out << ".remove(item->"
              << node->options->hashed_member->identifier << "());" << std::endl;
        }
      else if (node->options && node->options->multihashed_member)
        {
          out << ".erase(it);" << std::endl;
        }
      else // not a hashed list
       {
         out << ".removeAt(_M_" << string_tools::plural(node->name->identifier)
             << ".indexOf(item));" << std::endl;
       }

      if ( isChildItem )
        out << "model()->endRemoveItem();" << std::endl;

      if (node->options && node->options->multihashed_member)
        out << "}" << std::endl;

      out << "}" << std::endl;

      // finder
      if (node->options && node->options->hashed_member)
        {
          std::string hash_type = _G_system.type_of(
            node->options->hashed_member->identifier, node->type->type_name );

          out << node->type->type_name << " " << current_class << "::"
              << string_tools::accessor_name("find", node->name->identifier)
              << "(const " << hash_type
              << " &" << node->options->hashed_member->identifier << ")"
              << " const" << std::endl
              << "{" << std::endl
              << "return _M_" << string_tools::plural(node->name->identifier)
              << ".value(" << node->options->hashed_member->identifier << ");" << std::endl
              << "}" << std::endl
              << std::endl;
        }
      else if (node->options && node->options->multihashed_member)
        {
          std::string hash_type = _G_system.type_of(
            node->options->multihashed_member->identifier, node->type->type_name );

          out << node->type->raw_type_name << "List " << current_class << "::"
              << string_tools::accessor_name("find", string_tools::plural(node->name->identifier))
              << "(const " << hash_type
              << " &" << node->options->multihashed_member->identifier << ")"
              << " const" << std::endl
              << "{" << std::endl
              << "return _M_" << string_tools::plural(node->name->identifier)
              << ".values(" << node->options->multihashed_member->identifier << ");" << std::endl
              << "}" << std::endl
              << std::endl;
        }
    }
}

void generate_implementation::visit_code_item(code_item_ast *node)
{
  if (node->type == code_item::type_implementation)
    out << node->code->code;
}


void generate_member_initializations::operator()(ast_node *node)
{
  default_visitor::visit_node(node);
}

void generate_member_initializations::visit_member_item(member_item_ast *node)
{
  if (node->options && node->options->initialization_code)
    {
      std::string variable_name = (node->is_list) ?
        string_tools::plural(node->name->identifier) : node->name->identifier;

      out << std::endl << ", "
          << "_M_" << variable_name << "("
          << node->options->initialization_code->code << ")";
    }
}


void generate_code::operator()(ast_node *node, code_item::item_type_enum code_type)
{
  type = code_type;
  default_visitor::visit_node(node);
}

void generate_code::visit_code_item(code_item_ast *node)
{
  if (node->type == type)
    out << node->code->code;
}

} // end of namespace kdevcmg

