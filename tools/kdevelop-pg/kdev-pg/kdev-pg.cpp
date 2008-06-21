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

#include "kdev-pg.h"
#include "kdev-pg-ast.h"

#include <cassert>
#include <iostream>

model::zero_item *pg::zero()
{
  model::zero_item *node = create_node<model::zero_item>();
  return node;
}

model::plus_item *pg::plus(model::node *item)
{
  model::plus_item *node = create_node<model::plus_item>();
  node->_M_item = item;
  return node;
}

model::star_item *pg::star(model::node *item)
{
  model::star_item *node = create_node<model::star_item>();
  node->_M_item = item;
  return node;
}

model::symbol_item *pg::symbol(char const *name)
{
  model::symbol_item *node = create_node<model::symbol_item>();
  node->_M_name = name;
  return node;
}

model::action_item *pg::action(model::node *item, char const *code)
{
  model::action_item *node = create_node<model::action_item>();
  node->_M_item = item;
  node->_M_code = code;
  return node;
}

model::alternative_item *pg::alternative(model::node *left, model::node *right)
{
  model::alternative_item *node = create_node<model::alternative_item>();
  node->_M_left = left;
  node->_M_right = right;
  return node;
}

model::cons_item *pg::cons(model::node *left, model::node *right)
{
  model::cons_item *node = create_node<model::cons_item>();
  node->_M_left = left;
  node->_M_right = right;
  return node;
}

model::evolve_item *pg::evolve(
    model::node *item, model::symbol_item *symbol,
    model::variable_declaration_item *declarations, char const *code)
{
  model::evolve_item *node = create_node<model::evolve_item>();
  node->_M_item = item;
  node->_M_symbol = symbol;
  node->_M_declarations = declarations;
  node->_M_code = code;
  return node;
}

model::try_catch_item *pg::try_catch(model::node *try_item, model::node *catch_item)
{
  model::try_catch_item *node = create_node<model::try_catch_item>();
  node->_M_try_item = try_item;
  node->_M_catch_item = catch_item;
  node->_M_unsafe = false;
  return node;
}

model::alias_item *pg::alias(char const *code, model::symbol_item *symbol)
{
  model::alias_item *node = create_node<model::alias_item>();
  node->_M_code = code;
  node->_M_symbol = symbol;
  return node;
}

model::terminal_item *pg::terminal(char const *name, char const *description)
{
  model::terminal_item *node = create_node<model::terminal_item>();
  node->_M_name = name;
  node->_M_description = description;
  return node;
}

model::nonterminal_item *pg::nonterminal(model::symbol_item *symbol, char const *arguments)
{
  model::nonterminal_item *node = create_node<model::nonterminal_item>();
  node->_M_symbol = symbol;
  node->_M_arguments = arguments;
  return node;
}

model::annotation_item *pg::annotation(
    char const *name, model::node *item, bool is_sequence,
    model::variable_declaration_item::storage_type_enum storage_type)
{
  model::annotation_item *node = create_node<model::annotation_item>();
  node->_M_item = item;

  model::variable_declaration_item::variable_type_enum variable_type;
  char const *type;

  if (model::terminal_item *t = node_cast<model::terminal_item*>(item))
    {
      variable_type = model::variable_declaration_item::type_token;
      type = t->_M_name;
    }
  else if (model::nonterminal_item *nt = node_cast<model::nonterminal_item*>(item))
    {
      variable_type = model::variable_declaration_item::type_node;
      type = nt->_M_symbol->_M_name;
    }
  else
    {
      assert(0); // ### item must either be a terminal or a nonterminal
    }

  node->_M_declaration =
     pg::variable_declaration(model::variable_declaration_item::declaration_local,
                              storage_type, variable_type, is_sequence, name, type);
  return node;
}

model::condition_item *pg::condition(char const *code, model::node *item)
{
  model::condition_item *node = create_node<model::condition_item>();
  node->_M_code = code;
  node->_M_item = item;
  return node;
}

model::variable_declaration_item *pg::variable_declaration(
      model::variable_declaration_item::declaration_type_enum declaration_type,
      model::variable_declaration_item::storage_type_enum     storage_type,
      model::variable_declaration_item::variable_type_enum    variable_type,
      bool is_sequence, char const* name, char const *type)
{
  model::variable_declaration_item *node = create_node<model::variable_declaration_item>();
  node->_M_name = name;
  node->_M_type = type;
  node->_M_declaration_type = declaration_type;
  node->_M_storage_type     = storage_type;
  node->_M_variable_type    = variable_type;
  node->_M_is_sequence      = is_sequence;
  node->_M_next = 0;
  return node;
}

settings::member_item *pg::member(settings::member_item::member_kind_enum kind, char const *code)
{
  settings::member_item *node = create_node<settings::member_item>();
  node->_M_member_kind = kind;
  node->_M_code = code;
  return node;
}

std::ostream &operator << (std::ostream &out, model::node const *__node)
{
  model::node *node = const_cast<model::node*>(__node);

  if (node_cast<model::zero_item*>(node))
    return (out << "0");
  else if (model::symbol_item *s = node_cast<model::symbol_item *>(node))
    return (out << s->_M_name);
  else if (model::terminal_item *t = node_cast<model::terminal_item *>(node))
    return (out << t->_M_name);
  else if (model::annotation_item *a = node_cast<model::annotation_item *>(node))
    return (out << ((a->_M_declaration->_M_is_sequence) ? "#" : "")
                << a->_M_declaration->_M_name
                << ((a->_M_declaration->_M_storage_type
                     == model::variable_declaration_item::storage_temporary) ? ":" : "=")
                << a->_M_item);
#if 0

  else
    if (model::evolve_item *e = node_cast<model::evolve_item *>(node))
      return (out << "evolve:" << e->_M_symbol->_M_name);
#endif

  assert(0);
  return out;
}

bool reduces_to_epsilon(model::node *node)
{
  if (model::cons_item *c = node_cast<model::cons_item*>(node))
    {
      return reduces_to_epsilon(c->_M_left) && reduces_to_epsilon(c->_M_right);
    }
  else if (model::alternative_item *a = node_cast<model::alternative_item*>(node))
    {
      return reduces_to_epsilon(a->_M_left) || reduces_to_epsilon(a->_M_right);
    }
  else if (model::action_item *a = node_cast<model::action_item*>(node))
    {
      return reduces_to_epsilon(a->_M_item);
    }
  else if (model::condition_item *c = node_cast<model::condition_item*>(node))
    {
      return reduces_to_epsilon(c->_M_item);
    }
  else if (model::try_catch_item *t = node_cast<model::try_catch_item*>(node))
    {
      return reduces_to_epsilon(t->_M_try_item)
             || (t->_M_catch_item && reduces_to_epsilon(t->_M_catch_item));
    }
  else if (model::annotation_item *a = node_cast<model::annotation_item*>(node))
    {
      return reduces_to_epsilon(a->_M_item);
    }
  else if (model::nonterminal_item *n = node_cast<model::nonterminal_item*>(node))
    {
      return reduces_to_epsilon(n->_M_symbol);
    }
  else if (model::symbol_item *s = node_cast<model::symbol_item*>(node))
    {
      return _G_system.FIRST(s).find(_G_system.zero()) != _G_system.FIRST(s).end(); // hmm
    }
  else if (model::plus_item *p = node_cast<model::plus_item*>(node))
    {
      return reduces_to_epsilon(p->_M_item);
    }
  else if (node_cast<model::star_item*>(node))
    {
      return true;
    }
  else if (node_cast<model::zero_item*>(node))
    {
      return true;
    }

  return false;
}

bool is_zero(model::node *node)
{
  if (model::action_item *a = node_cast<model::action_item*>(node))
    {
      return is_zero(a->_M_item);
    }
  else if (model::condition_item *c = node_cast<model::condition_item*>(node))
    {
      return is_zero(c->_M_item);
    }
  else if (model::annotation_item *a = node_cast<model::annotation_item*>(node))
    {
      return is_zero(a->_M_item);
    }
  else if (model::plus_item *p = node_cast<model::plus_item*>(node))
    {
      return is_zero(p->_M_item);
    }
  else if (model::star_item *s = node_cast<model::star_item*>(node))
    {
      return is_zero(s->_M_item);
    }
  else if (node_cast<model::zero_item*>(node))
    {
      return true;
    }

  return false;
}

