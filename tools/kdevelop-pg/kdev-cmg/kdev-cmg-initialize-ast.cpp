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

#include "kdev-cmg-initialize-ast.h"

#include "kdev-cmg.h"

#include <iostream>


namespace kdevcmg
{

void initialize_ast::operator()()
{
  mode = gather_types;
  _M_item_type_names.insert("Code");
  while (changed)
    {
      changed = false;
      visit_node(_G_system.ast);
    }

  mode = modify_ast;
  visit_node(_G_system.ast);

  mode = register_types;
  visit_node(_G_system.ast);
}

void initialize_ast::visit_type_declaration(type_declaration_ast *node)
{
  if (mode == gather_types && !node->type->is_item_type && node->base_type)
    {
      string_set::iterator it = _M_item_type_names.find(node->base_type->raw_type_name);

      if (it != _M_item_type_names.end())
        {
          changed = true;
          _M_item_type_names.insert(node->type->raw_type_name);
          _M_base_item_type_names.insert(node->base_type->raw_type_name);
          node->type->is_item_type = true;
        }
    }

  else if (mode == modify_ast)
    {
      if (node->type->is_item_type == false)
        {
          node->item_declaration = 0;
        }
      else
        {
          node->non_item_type_declaration = 0;

          string_set::iterator it = _M_base_item_type_names.find(node->type->raw_type_name);

          if (it == _M_base_item_type_names.end())
            node->item_declaration->is_base_class = false;
          else
            node->item_declaration->is_base_class = true;
        }

      // get rid of the own copy of the child nodes, they are now to be
      // processed from either item_declaration or non_item_type_declaration.
      node->type = 0;
      node->base_type = 0;
      node->item_sequence = 0;
    }

  default_visitor::visit_type_declaration(node);
}

void initialize_ast::visit_item_declaration(item_declaration_ast *node)
{
  current_class = node->type->type_name;

  if (mode == register_types)
    {
      if (node->base_type)
        _G_system.inherit_types(node->base_type->type_name, node->type->type_name);
    }

  default_visitor::visit_item_declaration(node);
}

void initialize_ast::visit_non_item_type_declaration(non_item_type_declaration_ast *node)
{
  current_class = node->type->type_name;

  default_visitor::visit_non_item_type_declaration(node);
}

void initialize_ast::visit_member_item(member_item_ast *node)
{
  if (mode == register_types)
    {
      _G_system.register_type(
        node->name->identifier, node->type->type_name, current_class );
    }

  default_visitor::visit_member_item(node);
}

void initialize_ast::visit_type(type_ast *node)
{
  if (mode == gather_types)
    {
      if (node->raw_type_name == "Code")
        node->is_item_type = true;
    }
  else if (mode == modify_ast)
    {
      string_set::iterator it = _M_item_type_names.find(node->raw_type_name);

      if (it == _M_item_type_names.end())
        {
          node->type_name = node->raw_type_name;
        }
      else
        {
          node->type_name = node->raw_type_name + "ModelItem";
          node->is_item_type = true;
        }
    }
}

} // end of namespace kdevcmg

