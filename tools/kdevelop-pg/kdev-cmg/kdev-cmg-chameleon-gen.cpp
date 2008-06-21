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

#include "kdev-cmg-chameleon-gen.h"

#include "kdev-cmg.h"
#include "kdev-cmg-string-tools.h"

#include <iostream>

#define GENERATE_MEMBER(generator, node, return_type, method_name, arg_type, arg_name) \
{ \
  method_signature sig(method_name, arg_type); \
  method_signature_list::iterator it = method_signatures.begin(); \
  bool exists = false; \
  while (it != method_signatures.end()) \
    { \
      if (sig.method_name == (*it).method_name \
          && sig.argument_type == (*it).argument_type) \
        { \
          exists = true; \
        } \
      \
      it++; \
    } \
  if (!exists) \
    { \
      method_signatures.push_back(sig); \
      generator(node, return_type, method_name, arg_type, arg_name); \
      methodCreated = true; \
    } \
}


namespace kdevcmg
{

void generate_chameleon::operator()()
{
  method_signatures.clear();

  if (mode == generate_chameleon::declaration)
    {
      out << "template <class T> struct Nullable" << std::endl
          << "{" << std::endl

          << "bool isNull;" << std::endl
          << "T item;" << std::endl

          << "Nullable() : isNull(true) {}"
          << "Nullable(T _item) : isNull(false), item(_item) {}"

          << "};" << std::endl
          << std::endl;

      out << "/** Don't instantiate this class directly." << std::endl
          << " * Instead, use the wrapper named ModelItemChameleon" << std::endl
          << " * which provides shared pointer abilities." << std::endl
          << " */" << std::endl
          << "class _ModelItemChameleon : public KDevelop::Shared" << std::endl
          << "{" << std::endl

          << "CodeModelItem _M_item;" << std::endl
          << std::endl

          << "public:" << std::endl

          << "CodeModelItem item() { return _M_item; }" << std::endl
          << std::endl

          << "_ModelItemChameleon(CodeModelItem item) : _M_item(item)" << std::endl
          << "{}" << std::endl

          << "~_ModelItemChameleon()" << std::endl
          << "{}" << std::endl
          << std::endl;
    }

  visit_node(_G_system.ast);

  if (mode == generate_chameleon::declaration)
    {
      out << "private:" << std::endl
          << "_ModelItemChameleon(const _ModelItemChameleon &other);" << std::endl
          << "void operator=(const _ModelItemChameleon &other);" << std::endl

          << "};" << std::endl
          << std::endl;

      out << "class ModelItemChameleon : public KDevelop::SharedPtr<_ModelItemChameleon>" << std::endl
          << "{" << std::endl
          << "public:" << std::endl;

      generate_chameleon_initializations __gen_inits(out);
      __gen_inits();

      out << "};" << std::endl
          << std::endl;
    }
}

void generate_chameleon::visit_code_model_declaration(code_model_declaration_ast *node)
{
  // don't descend
}

void generate_chameleon::visit_non_item_type_declaration(non_item_type_declaration_ast *node)
{
  // don't descend
}

void generate_chameleon::visit_item_declaration(item_declaration_ast *node)
{
  default_visitor::visit_item_declaration(node);
}

void generate_chameleon::visit_member_item(member_item_ast *node)
{
  generate_chameleon_member __gen_member(out, mode);

  std::string member_type, member_name;
  std::string return_type, method_name;
  std::string argument_type, argument_name;
  bool methodCreated = false;

  if (!node->is_list)
    {
      // getter
      return_type = node->type->type_name;
      method_name = node->name->identifier;
      argument_type = argument_name = "";
      GENERATE_MEMBER( __gen_member, node, return_type, method_name,
                       argument_type, argument_name );


      // setter
      return_type = "void";
      method_name = string_tools::accessor_name("set", node->name->identifier);
      argument_type = node->type->type_name;
      argument_name = node->name->identifier;
      GENERATE_MEMBER( __gen_member, node, return_type, method_name,
                       argument_type, argument_name );

    }
  else
    {
      // getter
      return_type = node->type->raw_type_name + "List";
      method_name = string_tools::plural(node->name->identifier);
      argument_type = argument_name = "";
      GENERATE_MEMBER( __gen_member, node, return_type, method_name,
                       argument_type, argument_name );


      // setter: add
      return_type = "void";
      method_name = string_tools::accessor_name("add", node->name->identifier);
      argument_type = node->type->type_name;
      argument_name = "item";
      GENERATE_MEMBER( __gen_member, node, return_type, method_name,
                       argument_type, argument_name );


      // setter: remove
      method_name = string_tools::accessor_name("remove", node->name->identifier);
      GENERATE_MEMBER( __gen_member, node, return_type, method_name,
                       argument_type, argument_name );

      // finder
      if (node->options && node->options->hashed_member)
        {
          std::string hash_type = _G_system.type_of(
            node->options->hashed_member->identifier, node->type->type_name );

          return_type = node->type->type_name;
          method_name = string_tools::accessor_name("find", node->name->identifier);
          argument_type = "const " + hash_type + " &";
          argument_name = node->options->hashed_member->identifier;
          GENERATE_MEMBER( __gen_member, node, return_type, method_name,
                           argument_type, argument_name );
        }
      else if (node->options && node->options->multihashed_member)
        {
          std::string hash_type = _G_system.type_of(
            node->options->multihashed_member->identifier, node->type->type_name );

          return_type = node->type->raw_type_name + "List";
          method_name = string_tools::accessor_name("find", string_tools::plural(node->name->identifier));
          argument_type = "const " + hash_type + " &";
          argument_name = node->options->multihashed_member->identifier;
          GENERATE_MEMBER( __gen_member, node, return_type, method_name,
                           argument_type, argument_name );
        }
    }

  if (methodCreated)
    out << std::endl;
}


void generate_chameleon_initializations::operator()()
{
  visit_node(_G_system.ast);
}

void generate_chameleon_initializations::visit_item_declaration(item_declaration_ast *node)
{
  if (node->type->raw_type_name == "Code")
    {
      out << "ModelItemChameleon(CodeModelItem item)" << std::endl
          << " : KDevelop::SharedPtr<_ModelItemChameleon>(new _ModelItemChameleon(item))" << std::endl
          << "{}" << std::endl;
    }
  else
    {
      out << "ModelItemChameleon(" << node->type->type_name << " item)" << std::endl
          << " : KDevelop::SharedPtr<_ModelItemChameleon>(" << std::endl
          << "     new _ModelItemChameleon(model_static_cast<CodeModelItem>(item)) )" << std::endl
          << "{}" << std::endl;
    }
}


void generate_chameleon_member::operator()(
    member_item_ast *member, std::string return_type, std::string method_name,
    std::string argument_type, std::string argument_name )
{
  if (mode == generate_chameleon::implementation)
    {
      _M_member = member;
      _M_return_type = return_type;
      _M_method_name = method_name;
      _M_argument_type = argument_type;
      _M_argument_name = argument_name;
    }

  if (return_type != "void")
    out << "Nullable<";

  out << return_type << " ";

  if (return_type != "void")
    out << "> ";

  if (mode == generate_chameleon::implementation)
    out << "_ModelItemChameleon::";

  out << method_name
      << "(" << argument_type << " " << argument_name << ")"
      // all setters return void, and all getters are const methods:
      << ((return_type == "void") ? "" : " const");

  if (mode == generate_chameleon::declaration)
    {
      out << ";" << std::endl;
    }
  else if (mode == generate_chameleon::implementation)
    {
      out << std::endl
          << "{" << std::endl

          << "switch (_M_item->kind())" << std::endl
          << "{" << std::endl;

      visit_node(_G_system.ast);

      out << "default:" << std::endl
          << "break;" << std::endl
          << "}" << std::endl;

      if (return_type == "void")
        {
          out << "return;" << std::endl;
        }
      else
        {
          out << "return Nullable<" << return_type << ">();" << std::endl;
        }

      out << "}" << std::endl
          << std::endl;
    }
}

void generate_chameleon_member::visit_item_declaration(item_declaration_ast *node)
{
  if (_G_system.type_of(_M_member->name->identifier, node->type->raw_type_name + "ModelItem")
      != _M_member->type->type_name)
    return;

  if (node->type->raw_type_name == "Code")
    return; // _CodeModelItem has no Kind_Code enum value, can't query that

  out << "case _CodeModelItem::Kind_" << node->type->raw_type_name << ":" << std::endl
      << "{" << std::endl;

  if (_M_return_type == "void")
    {
      out << "model_static_cast<" << node->type->raw_type_name << "ModelItem>(_M_item)->"
          << _M_method_name << "(" << _M_argument_name << ");" << std::endl
          << "return;" << std::endl;
    }
  else
    {
      out << "return Nullable<" << _M_return_type << " >("
          << "model_static_cast<" << node->type->raw_type_name << "ModelItem>(_M_item)->"
          << _M_method_name << "(" << _M_argument_name << ")"
          << ");" << std::endl;
    }

  out << "}" << std::endl;
}

} // end of namespace kdevcmg

