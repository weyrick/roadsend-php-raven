/* This file is part of kdev-pg
   Copyright (C) 2005 Roberto Raggi <roberto@kdevelop.org>
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

#include "kdev-pg-default-visitor-bits-gen.h"
#include "kdev-pg.h"
#include <iostream>

void gen_default_visitor_bits_rule::operator()(std::pair<std::string,model::symbol_item*> const &__it)
{
  _M_names.clear();
  _M_variable_declarations.clear();

  model::symbol_item *sym = __it.second;

  bool has_members = false;
  has_member_nodes hms(has_members);
  hms(sym);

  out << "void default_visitor::visit_" << sym->_M_name
      << "(" << sym->_M_name << "_ast *" << (has_members ? "node" : "")
      << ") {" << std::endl;

  world::environment::iterator it = _G_system.env.find(sym);
  while (it != _G_system.env.end())
    {
      model::evolve_item *e = (*it).second;
      if ((*it).first != sym)
        break;

      ++it;

      visit_node(e);
    }

  out << "}" << std::endl << std::endl;
}

void gen_default_visitor_bits_rule::visit_variable_declaration(model::variable_declaration_item *node)
{
  do
  {
    if (node->_M_storage_type != model::variable_declaration_item::storage_ast_member)
      break;

    if (node->_M_variable_type != model::variable_declaration_item::type_node)
      break; // nothing to do

    if (_M_names.find(node->_M_name) != _M_names.end())
      break;

    std::string base_type = std::string(node->_M_type) + "_ast*";

    if (node->_M_is_sequence)
      {
        out << "if (" << "node->" << node->_M_name << "_sequence" << ") {"
            << "const list_node<" << base_type << "> *__it = "
            << "node->" << node->_M_name << "_sequence" << "->to_front()"
            << ", *__end = __it;" << std::endl
            << "do {" << std::endl
            << "visit_node(__it->element);" << std::endl
            << "__it = __it->next;" << std::endl
            << "} while (__it != __end);" << std::endl
            << "}" << std::endl;
      }
    else
      {
        out << "visit_node(" << "node->" << node->_M_name << ")" << ";" << std::endl;
      }

    _M_names.insert(node->_M_name);
    _M_variable_declarations.push_back(node);

  } while(false);

  default_visitor::visit_variable_declaration(node);
}


void has_member_nodes::operator()(model::symbol_item *sym)
{
  has_members = false;

  world::environment::iterator it = _G_system.env.find(sym);
  while (it != _G_system.env.end())
    {
      model::evolve_item *e = (*it).second;
      if ((*it).first != sym)
        break;

      ++it;

      visit_node(e);
    }
}

void has_member_nodes::visit_variable_declaration(model::variable_declaration_item *node)
{
  if (node->_M_storage_type == model::variable_declaration_item::storage_ast_member
      && node->_M_variable_type == model::variable_declaration_item::type_node)
    {
      has_members = true;
    }
}
