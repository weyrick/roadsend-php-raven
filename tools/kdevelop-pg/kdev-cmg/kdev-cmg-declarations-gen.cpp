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

#include "kdev-cmg-declarations-gen.h"

#include "kdev-cmg.h"
#include "kdev-cmg-string-tools.h"

#include <iostream>


namespace kdevcmg
{

void generate_declarations::operator()()
{
  out << "#define DECLARE_MODEL_NODE(k) \\" << std::endl
      << "enum { __node_kind = Kind_##k }; \\" << std::endl
      << "typedef KDevelop::SharedPtr<k##ModelItem> Pointer;" << std::endl
      << std::endl

      << "#define ITEM(item) item##ModelItem" << std::endl
      << "#define LIST(item) item##List" << std::endl
      << std::endl

      << "template <class _Target, class _Source>" << std::endl
      << "_Target model_static_cast(_Source item)" << std::endl
      << "{" << std::endl
      << "typedef typename _Target::Type * _Target_pointer;" << std::endl
      << std::endl
      << "_Target ptr = static_cast<_Target_pointer>(item.data());" << std::endl
      << "return ptr;" << std::endl
      << "}" << std::endl
      << std::endl;

  visit_node(_G_system.ast);

  out << "template <class _Target, class _Source>" << std::endl
      << "_Target model_safe_cast(_Source item)" << std::endl
      << "{" << std::endl
      << "typedef typename _Target::Type * _Target_pointer;" << std::endl
      << "typedef typename _Source::Type * _Source_pointer;" << std::endl
      << std::endl
      << "_Source_pointer source = item.data();" << std::endl
      << "if (source && source->kind() == _Target_pointer(0)->__node_kind)" << std::endl
      << "{" << std::endl
      << "_Target ptr(static_cast<_Target_pointer>(source));" << std::endl
      << "return ptr;" << std::endl
      << "}" << std::endl
      << std::endl
      << "return _Target();" << std::endl
      << "}" << std::endl
      << std::endl

      << "template <typename _Target, typename _Source>" << std::endl
      << "_Target model_dynamic_cast(_Source item)" << std::endl
      << "{" << std::endl
      << "typedef typename _Target::Type * _Target_pointer;" << std::endl
      << "typedef typename _Source::Type * _Source_pointer;" << std::endl
      << std::endl
      << "_Source_pointer source = item.data();" << std::endl
      << "if (source && (source->kind() == _Target_pointer(0)->__node_kind" << std::endl
      << "|| (_Target_pointer(0)->__node_kind <= int(_CodeModelItem::KindMask)" << std::endl
      << "&& ((source->kind() & _Target_pointer(0)->__node_kind)" << std::endl
      << "== _Target_pointer(0)->__node_kind))))" << std::endl
      << "{" << std::endl
      << "_Target ptr(static_cast<_Target_pointer>(source));" << std::endl
      << "return ptr;" << std::endl
      << "}" << std::endl
      << std::endl
      << "return _Target();" << std::endl
      << "}" << std::endl
      << std::endl;
}

void generate_declarations::visit_code_model_declaration(code_model_declaration_ast *node)
{
  out << "class CodeModel : public KDevelop::CodeModel" << std::endl
      << "{" << std::endl

      << "public:" << std::endl
      << "CodeModel( QObject *parent = 0 );" << std::endl
      << "virtual ~CodeModel();" << std::endl
      << std::endl

      << "template <class _Target> _Target create()" << std::endl
      << "{" << std::endl
      << "typedef typename _Target::Type _Target_type;" << std::endl
      << std::endl
      << "_Target result = _Target_type::create(this);" << std::endl
      << "return result;" << std::endl
      << "}" << std::endl
      << std::endl

      << "void addCodeItem(CodeModelItem item);" << std::endl
      << "void removeCodeItem(CodeModelItem item);" << std::endl
      << std::endl
      << "void wipeout();" << std::endl
      << std::endl;

  default_visitor::visit_code_model_declaration(node);

  out << "private:" << std::endl
      << "CodeModel(const CodeModel &other);" << std::endl
      << "void operator=(const CodeModel &other);" << std::endl

      << "};" << std::endl
      << std::endl;
}

void generate_declarations::visit_root_item(root_item_ast *node)
{
  out << "public:" << std::endl
      << node->item->type->type_name << " "
      << node->item->name->identifier << "() const;" << std::endl
      << "KDevelop::ItemCollection *root() const;" << std::endl
      << std::endl

      << "private:" << std::endl
      << node->item->type->type_name << " "
      << "_M_" << node->item->name->identifier << ";" << std::endl
      << std::endl;
}

void generate_declarations::visit_item_declaration(item_declaration_ast *node)
{
  out << "class _" << node->type->type_name;

  if (node->type->type_name == "CodeModelItem")
    out << " : public KDevelop::CodeItem";
  else if (node->base_type)
    out << " : public _" << node->base_type->type_name;

  out << std::endl
      << "{" << std::endl;

  if (node->type->type_name == "CodeModelItem")
    {
      generate_code_model_item_decls __code_model_item_decls(out);
      __code_model_item_decls();
    }
  else
    {
      out << "public:" << std::endl
          << "DECLARE_MODEL_NODE(" << node->type->raw_type_name << ")" << std::endl
          << std::endl
          << "static " << node->type->type_name << " create(CodeModel *model);" << std::endl
          << "virtual ~_" << node->type->type_name << "();" << std::endl
          << std::endl;
    }

  default_visitor::visit_item_declaration(node);

  generate_member_items __items(out);
  __items(node);

  if (node->type->type_name == "CodeModelItem")
    {
      out << "protected:" << std::endl
          << "_" << node->type->type_name
          << "(CodeModel *model, int kind);" << std::endl

          << "void setKind(int kind);" << std::endl;
    }
  else
    {
      out << "protected:" << std::endl
          << "_" << node->type->type_name
          << "(CodeModel *model, int kind = __node_kind);" << std::endl;
    }

  out << std::endl

      << "private:" << std::endl
      << "_" << node->type->type_name
      << "(const _" << node->type->type_name << " &other);" << std::endl
      << "void operator="
      << "(const _" << node->type->type_name << " &other);" << std::endl

      << "};" << std::endl
      << std::endl;
}

void generate_declarations::visit_non_item_type_declaration(non_item_type_declaration_ast *node)
{
  out << "class " << node->type->type_name;

  if (node->base_type)
    out << " : public " << node->base_type->type_name;

  out << std::endl
      << "{" << std::endl;

  default_visitor::visit_non_item_type_declaration(node);

  generate_member_items __items(out);
  __items(node);

  out << "};" << std::endl
      << std::endl;
}

void generate_declarations::visit_code_item(code_item_ast *node)
{
  switch (node->type)
  {
  case code_item::type_public_declaration:
    out << "public:";
    break;
  case code_item::type_protected_declaration:
    out << "protected:";
    break;
  case code_item::type_private_declaration:
    out << "private:";
    break;
  default:
    return;
  }
  out << node->code->code;
}


void generate_member_items::operator()(ast_node *node)
{
  out << "public:" << std::endl;

  mode = generate_accessors;
  default_visitor::visit_node(node);

  break_line = false;
  mode = generate_inlines;
  default_visitor::visit_node(node);

  if (break_line)
    out << std::endl;

  out << "private:" << std::endl;

  mode = generate_variable_declarations;
  default_visitor::visit_node(node);
}

void generate_member_items::visit_member_item(member_item_ast *node)
{
  if (mode == generate_accessors)
    {
      if (!node->is_list)
        {
          // getter
          out << node->type->type_name << " "
              << node->name->identifier << "() const;" << std::endl;

          // setter
          out << "void " << string_tools::accessor_name("set", node->name->identifier)
              << "(" << node->type->type_name << " " << node->name->identifier << ");"
              << std::endl;
        }
      else
        {
          // getter
          out << node->type->raw_type_name << "List "
              << string_tools::plural(node->name->identifier) << "() const;"
              << std::endl;

          // setter: add
          out << "void " << string_tools::accessor_name("add", node->name->identifier)
              << "(" << node->type->type_name << " item);" << std::endl;

          // setter: remove
          out << "void " << string_tools::accessor_name("remove", node->name->identifier)
              << "(" << node->type->type_name << " item);" << std::endl;

          // finder
          if (node->options && node->options->hashed_member)
            {
              std::string hash_type = _G_system.type_of(
                node->options->hashed_member->identifier, node->type->type_name );

              out << node->type->type_name << " "
                  << string_tools::accessor_name("find", node->name->identifier)
                  << "(const " << hash_type
                  << " &" << node->options->hashed_member->identifier << ")"
                  << " const;" << std::endl;
            }
          else if (node->options && node->options->multihashed_member)
            {
              std::string hash_type = _G_system.type_of(
                node->options->multihashed_member->identifier, node->type->type_name );

              out << node->type->raw_type_name << "List "
                  << string_tools::accessor_name("find", string_tools::plural(node->name->identifier))
                  << "(const " << hash_type
                  << " &" << node->options->multihashed_member->identifier << ")"
                  << " const;" << std::endl;
            }
        }

      out << std::endl;
    }

  if (mode == generate_inlines && node->options && node->options->hashed)
    {
      std::string hash_type;

      out << "inline ";

      if (node->options->hashed_member)
        {
          hash_type = _G_system.type_of(
            node->options->hashed_member->identifier, node->type->type_name );

          out << "QHash";
        }
      else // if (node->options->multihashed_member)
        {
          hash_type = _G_system.type_of(
            node->options->multihashed_member->identifier, node->type->type_name );

          out << "QMultiHash";
        }

      out << "<" << hash_type << ", " << node->type->type_name << "> "
          << node->name->identifier << "Map() const" << std::endl
          << "{" << std::endl
          << "return _M_" << string_tools::plural(node->name->identifier) << ";" << std::endl
          << "}" << std::endl;
    }

  if (mode == generate_variable_declarations)
    {
      std::string variable_name = (node->is_list) ?
        string_tools::plural(node->name->identifier) : node->name->identifier;

      if (node->options && node->options->hashed_member)
        {
          std::string hash_type = _G_system.type_of(
            node->options->hashed_member->identifier, node->type->type_name );

          out << "QHash<" << hash_type << ", ";
        }
      else if (node->options && node->options->multihashed_member)
        {
          std::string hash_type = _G_system.type_of(
            node->options->multihashed_member->identifier, node->type->type_name );

          out << "QMultiHash<" << hash_type << ", ";
        }

      if (node->options && node->options->hashed)
        out << node->type->type_name << "> ";
      else if (!node->is_list)
        out << node->type->type_name << " ";
      else
        out << node->type->raw_type_name + "List ";

      out << "_M_" << variable_name << ";" << std::endl;
    }
}


void generate_code_model_item_decls::operator()()
{
  out << "public:" << std::endl;
  visit_node(_G_system.ast); // generate "enum Kind"

  out << "public:" << std::endl
      << "virtual ~_CodeModelItem();" << std::endl
      << std::endl
      << "virtual _CodeModelItem *itemAt(int index) const;" << std::endl
      << std::endl

      << "int kind() const;" << std::endl
      << std::endl
      << "KTextEditor::Cursor startPosition() const;" << std::endl
      << "void setStartPosition(const KTextEditor::Cursor& cursor);" << std::endl
      << std::endl
      << "KTextEditor::Cursor endPosition() const;" << std::endl
      << "void setEndPosition(const KTextEditor::Cursor& cursor);" << std::endl
      << std::endl
      << "QTime timestamp() const { return QTime(); }" << std::endl
      << std::endl
      << "inline CodeModel *model() const { return _M_model; }" << std::endl
      << std::endl
      << "CodeModelItem toItem() const;" << std::endl
      << std::endl

      << "private:" << std::endl
      << "CodeModel *_M_model;" << std::endl
      << "int _M_kind;" << std::endl
      << "int _M_startLine;" << std::endl
      << "int _M_startColumn;" << std::endl
      << "int _M_endLine;" << std::endl
      << "int _M_endColumn;" << std::endl
      << std::endl;
}

void generate_code_model_item_decls::visit_model(model_ast *node)
{
  bitcounter = 0;

  out << "enum Kind" << std::endl
      << "{" << std::endl

      << "/* These are bit-flags resembling inheritance */" << std::endl;

  mode = declare_inherited;
  default_visitor::visit_model(node);

  out << "KindMask = (1 <<" << bitcounter << ") - 1," << std::endl
      << std::endl

      << "/* These are for classes that are not inherited from */" << std::endl
      << "FirstKind = 1 <<" << bitcounter << "," << std::endl;

  bitcounter++;

  mode = declare_non_inherited;
  default_visitor::visit_model(node);

  out << "};" << std::endl;
}

void generate_code_model_item_decls::visit_item_declaration(item_declaration_ast *node)
{
  if (node->type->type_name == "CodeModelItem")
    return;
  if (mode == declare_inherited && !node->is_base_class)
    return;
  if (mode == declare_non_inherited && node->is_base_class)
    return;

  out << "Kind_" << node->type->raw_type_name << " = 1";

  if (bitcounter != 0)
    {
      out << " <<" << bitcounter;

      if (node->base_type->type_name != "CodeModelItem")
        out << " /*| Kind_" << node->base_type->raw_type_name << "*/";
    }

  bitcounter++;
  out << "," << std::endl;
}

} // end of namespace kdevcmg

