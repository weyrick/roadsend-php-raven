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

#include "kdev-pg-pretty-printer.h"

#include <list>
#include <stack>
#include <iostream>
#include <cassert>

void pretty_printer::visit_zero(model::zero_item *node)
{
  out << "0";
}

void pretty_printer::visit_plus(model::plus_item *node)
{
  out << "(";
  visit_node(node->_M_item);
  out << ")";
  out << "+";
}

void pretty_printer::visit_star(model::star_item *node)
{
  out << "(";
  visit_node(node->_M_item);
  out << ")";
  out << "*";
}

void pretty_printer::visit_symbol(model::symbol_item *node)
{
  out << node->_M_name;
}

void pretty_printer::visit_action(model::action_item *node)
{
  visit_node(node->_M_item);
}

void pretty_printer::visit_alternative(model::alternative_item *node)
{
  std::list<model::node*> top_level_nodes;

  std::stack<model::node*> working_list;
  working_list.push(node->_M_right);
  working_list.push(node->_M_left);

  while (!working_list.empty())
    {
      model::node *n = working_list.top();
      working_list.pop();

      if (model::alternative_item *a = node_cast<model::alternative_item*>(n))
        {
          working_list.push(a->_M_right);
          working_list.push(a->_M_left);
        }
      else
        {
          top_level_nodes.push_back(n);
        }
    }

  bool initial = true;

  out << "(";
  std::list<model::node*>::iterator it = top_level_nodes.begin();
  while (it != top_level_nodes.end())
    {
      if (!initial)
        out << " | ";

      model::node *n = *it++;
      visit_node(n);
      initial = false;
    }
  out << ")";
}

void pretty_printer::visit_cons(model::cons_item *node)
{
  visit_node(node->_M_left);
  out << " ";
  visit_node(node->_M_right);
}

void pretty_printer::visit_evolve(model::evolve_item *node)
{
  visit_node(node->_M_item);
  out << " -> ";
  visit_node(node->_M_symbol);
}

void pretty_printer::visit_try_catch(model::try_catch_item *node)
{
  out << "try/" << (node->_M_catch_item ? "rollback" : "recover") << "(";
  visit_node(node->_M_try_item);
  out << ")";

  if (node->_M_catch_item)
    {
      out << " catch(";
      visit_node(node->_M_catch_item);
      out << ")";
    }
}

void pretty_printer::visit_alias(model::alias_item *node)
{
  assert(0); // ### not implemented yet
}

void pretty_printer::visit_terminal(model::terminal_item *node)
{
  out << node->_M_name;
}

void pretty_printer::visit_nonterminal(model::nonterminal_item *node)
{
  visit_node(node->_M_symbol);
}

void pretty_printer::visit_annotation(model::annotation_item *node)
{
  out << ((node->_M_declaration->_M_is_sequence) ? "#" : "")
      << node->_M_declaration->_M_name
      << ((node->_M_declaration->_M_storage_type
           == model::variable_declaration_item::storage_temporary) ? ":" : "=");
  visit_node(node->_M_item);
}
