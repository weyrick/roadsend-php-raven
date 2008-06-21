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

#include "kdev-pg-first.h"

#include <cassert>

void initialize_FIRST::operator ()(model::node *node)
{
  return visit_node(node);
}

void initialize_FIRST::visit_node(model::node *node)
{
  if (_G_system.find_in_FIRST(node) != _G_system.FIRST_end())
    return ; // nothing to do

  default_visitor::visit_node(node);
}

void initialize_FIRST::visit_zero(model::zero_item *node)
{
  _G_system.FIRST(node).insert(node);
}

void initialize_FIRST::visit_terminal(model::terminal_item *node)
{
  _G_system.FIRST(node).insert(node);
}


next_FIRST::next_FIRST(bool &changed): _M_changed(changed)
{
}

void next_FIRST::operator ()(model::node *node)
{
  assert(node_cast<model::evolve_item*>(node));
  _M_merge_blocked = false;
  _M_merge_zero_blocked = false;
  _M_item = node;
  visit_node(node);
}

bool next_FIRST::block_merge(bool block)
{
  bool old = _M_merge_blocked;
  _M_merge_blocked = block;
  return old;
}

bool next_FIRST::block_zero_merge(bool block)
{
  bool old = _M_merge_zero_blocked;
  _M_merge_zero_blocked = block;
  return old;
}

void next_FIRST::merge(model::node *__dest, model::node *__source, int K)
{
  world::node_set &dest = _G_system.FIRST(__dest, K);
  world::node_set &source = _G_system.FIRST(__source, K);

  for (world::node_set::iterator it = source.begin(); it != source.end(); ++it)
    {
      if (_M_merge_zero_blocked == true && node_cast<model::zero_item*>(*it))
        {
          continue;
        }

      _M_changed |= dest.insert(*it).second;
    }
}

void next_FIRST::visit_node(model::node *node)
{
  default_visitor::visit_node(node);

  if (!_M_merge_blocked)
    {
      merge(_M_item, node);
    }
}

void next_FIRST::visit_zero(model::zero_item *)
{
}

void next_FIRST::visit_terminal(model::terminal_item *)
{
}

void next_FIRST::visit_nonterminal(model::nonterminal_item *node)
{
  default_visitor::visit_nonterminal(node);

  merge(node, node->_M_symbol);
}

void next_FIRST::visit_symbol(model::symbol_item *)
{
}

void next_FIRST::visit_plus(model::plus_item *node)
{
  default_visitor::visit_plus(node);

  merge(node, node->_M_item);
}

void next_FIRST::visit_star(model::star_item *node)
{
  default_visitor::visit_star(node);

  if (_G_system.FIRST(node).insert(_G_system.zero()).second)
    _M_changed = true;

  merge(node, node->_M_item);
}

void next_FIRST::visit_action(model::action_item *node)
{
  default_visitor::visit_action(node);

  merge(node, node->_M_item);
}

void next_FIRST::visit_alternative(model::alternative_item *node)
{
  default_visitor::visit_alternative(node);

  merge(node, node->_M_left);
  merge(node, node->_M_right);
}

void next_FIRST::visit_cons(model::cons_item *node)
{
  bool blocked = block_merge(true);
  visit_node(node->_M_right);
  block_merge(blocked);

  bool zero_blocked = _M_merge_zero_blocked;
  if (!reduces_to_epsilon(node->_M_right))
    zero_blocked = block_zero_merge(true);

  visit_node(node->_M_left);
  merge(node, node->_M_left);

  if (reduces_to_epsilon(node->_M_left))
    {
      visit_node(node->_M_right);
      merge(node, node->_M_right);
    }

  block_zero_merge(zero_blocked);
}

void next_FIRST::visit_evolve(model::evolve_item *node)
{
  visit_node(node->_M_item);
  merge(node, node->_M_item);

  merge(node->_M_symbol, node);
}

void next_FIRST::visit_try_catch(model::try_catch_item *node)
{
  default_visitor::visit_try_catch(node);

  merge(node, node->_M_try_item);

  if (node->_M_catch_item)
    merge(node, node->_M_catch_item);
}

void next_FIRST::visit_alias(model::alias_item *node)
{
  default_visitor::visit_alias(node);
  assert(0); // ### not implemented yet
}

void next_FIRST::visit_annotation(model::annotation_item *node)
{
  default_visitor::visit_annotation(node);

  merge(node, node->_M_item);
}

void next_FIRST::visit_condition(model::condition_item* node)
{
  default_visitor::visit_condition(node);

  merge(node, node->_M_item);
}

void compute_FIRST() // the closure of the FIRST sets
{
  std::for_each(_G_system.rules.begin(), _G_system.rules.end(),
                initialize_FIRST());

  bool changed = true;
  while (changed)
    {
      changed = false;
      std::for_each(_G_system.rules.begin(), _G_system.rules.end(),
                    next_FIRST(changed));
    }
}
