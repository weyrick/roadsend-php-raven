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

#include "kdev-pg-clone-tree.h"
#include "kdev-pg.h"

void clone_tree::visit_zero(model::zero_item *node)
{
  _M_temps.push(node);
}

void clone_tree::visit_symbol(model::symbol_item *node)
{
  _M_temps.push(node);
}

void clone_tree::visit_terminal(model::terminal_item *node)
{
  _M_temps.push(node);
}

void clone_tree::visit_nonterminal(model::nonterminal_item *node)
{
  _M_temps.push(pg::nonterminal(node->_M_symbol, node->_M_arguments));
}

void clone_tree::visit_plus(model::plus_item *node)
{
  visit_node(node->_M_item);

  model::node *item = _M_temps.top();
  _M_temps.pop();

  _M_temps.push(pg::plus(item));
}

void clone_tree::visit_star(model::star_item *node)
{
  visit_node(node->_M_item);

  model::node *item = _M_temps.top();
  _M_temps.pop();

  _M_temps.push(pg::star(item));
}

void clone_tree::visit_action(model::action_item *node)
{
  visit_node(node->_M_item);

  model::node *item = _M_temps.top();
  _M_temps.pop();

  _M_temps.push(pg::action(item, node->_M_code));
}

void clone_tree::visit_alternative(model::alternative_item *node)
{
  visit_node(node->_M_left);

  model::node *left = _M_temps.top();
  _M_temps.pop();

  visit_node(node->_M_right);

  model::node *right = _M_temps.top();
  _M_temps.pop();

  _M_temps.push(pg::alternative(left, right));
}

void clone_tree::visit_cons(model::cons_item *node)
{
  visit_node(node->_M_left);

  model::node *left = _M_temps.top();
  _M_temps.pop();

  visit_node(node->_M_right);

  model::node *right = _M_temps.top();
  _M_temps.pop();

  _M_temps.push(pg::cons(left, right));
}

void clone_tree::visit_evolve(model::evolve_item *node)
{
  visit_node(node->_M_item);

  model::node *item = _M_temps.top();
  _M_temps.pop();

  _M_temps.push(pg::evolve(item, node->_M_symbol, node->_M_declarations, node->_M_code));
}

void clone_tree::visit_try_catch(model::try_catch_item *node)
{
  visit_node(node->_M_try_item);

  model::node *try_item = _M_temps.top();
  _M_temps.pop();

  model::node *catch_item = 0;

  if (node->_M_catch_item)
    {
      visit_node(node->_M_catch_item);

      catch_item = _M_temps.top();
      _M_temps.pop();
    }

  _M_temps.push(pg::try_catch(try_item, catch_item));
}

void clone_tree::visit_alias(model::alias_item *node)
{
  assert(0); // ### not implemented yet
}

void clone_tree::visit_annotation(model::annotation_item *node)
{
  visit_node(node->_M_item);

  model::node *item = _M_temps.top();
  _M_temps.pop();

  _M_temps.push(pg::annotation(node->_M_declaration->_M_name, item,
                               node->_M_declaration->_M_is_sequence,
                               node->_M_declaration->_M_storage_type));
}

model::node *clone_tree::clone(model::node *node)
{
  _M_temps = std::stack<model::node*>();
  visit_node(node);
  model::node *n = _M_temps.top();
  _M_temps.pop();
  assert(_M_temps.empty());
  return n;
}
