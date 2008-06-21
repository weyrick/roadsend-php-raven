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

#ifndef KDEV_PG_AST_H
#define KDEV_PG_AST_H

#include "kdev-pg-allocator.h"

#define PG_NODE(k) \
    enum { node_kind = node_kind_##k };

// the kdev-pg calculus
namespace model
{

  enum node_kind_enum {
    node_kind_item = 0,
    node_kind_zero = 1,
    node_kind_plus = 2,
    node_kind_star = 3,
    node_kind_symbol = 4,
    node_kind_action = 5,
    node_kind_alternative = 6,
    node_kind_cons = 7,
    node_kind_evolve = 8,
    node_kind_try_catch = 9,
    node_kind_alias = 10,
    node_kind_terminal = 11,
    node_kind_nonterminal = 12,
    node_kind_annotation = 13,
    node_kind_condition = 14,
    node_kind_variable_declaration = 15,

    node_kind_LAST
  };

  struct node
  {
    PG_NODE(item)

    int kind;
  };

  struct zero_item: public node
  {
    PG_NODE(zero)
  };

  struct plus_item: public node
  {
    PG_NODE(plus)

    node *_M_item;
  };

  struct star_item: public node
  {
    PG_NODE(star)

    node *_M_item;
  };

  struct symbol_item: public node
  {
    PG_NODE(symbol)

    char const *_M_name;
  };

  struct action_item: public node
  {
    PG_NODE(action)

    node *_M_item;
    char const *_M_code;
  };

  struct alternative_item: public node
  {
    PG_NODE(alternative)

    node *_M_left;
    node *_M_right;
  };

  struct cons_item: public node
  {
    PG_NODE(cons)

    node *_M_left;
    node *_M_right;
  };

  struct try_catch_item: public node
  {
    PG_NODE(try_catch)

    node *_M_try_item;
    node *_M_catch_item; // contains 0 for "catch(recover)"
    bool _M_unsafe;
  };

  struct alias_item: public node
  {
    PG_NODE(alias)

    char const *_M_code;
    symbol_item *_M_symbol;
  };

  struct terminal_item: public node
  {
    PG_NODE(terminal)

    char const *_M_name;
    char const *_M_description;
  };

  struct nonterminal_item: public node
  {
    PG_NODE(nonterminal)

    symbol_item *_M_symbol;
    char const *_M_arguments;
  };

  struct variable_declaration_item: public node
  {
    PG_NODE(variable_declaration)

    enum declaration_type_enum {
      declaration_argument,
      declaration_local,
    };

    enum storage_type_enum {
      storage_ast_member,
      storage_temporary,
    };

    enum variable_type_enum {
      type_node,
      type_token,
      type_variable,
    };

    char const *_M_type;
    char const *_M_name;

    declaration_type_enum _M_declaration_type;
    storage_type_enum     _M_storage_type;
    variable_type_enum    _M_variable_type;
    bool                  _M_is_sequence;

    variable_declaration_item *_M_next;
  };

  struct annotation_item: public node
  {
    PG_NODE(annotation)

    node *_M_item;
    variable_declaration_item *_M_declaration;
  };

  struct condition_item: public node
  {
    PG_NODE(condition)

    char const *_M_code;
    node *_M_item;
  };

  struct evolve_item: public node
  {
    PG_NODE(evolve)

    node *_M_item;
    symbol_item *_M_symbol;
    variable_declaration_item *_M_declarations;
    char const *_M_code;
  };

} // namespace model


// configuration stuff outside the model
namespace settings
{

  enum node_kind_enum {
    node_kind_member = 30,

    node_kind_LAST
  };

  struct member_item: public model::node
  {
    PG_NODE(member)

    enum member_kind_enum {
      public_declaration    = 1,
      protected_declaration = 2,
      private_declaration   = 4,
      constructor_code      = 8,
      destructor_code       = 16,
    };

    member_kind_enum _M_member_kind;
    char const *_M_code;
  };

} // namespace settings


template <class _Tp>
_Tp node_cast(model::node *item)
{
  if (static_cast<_Tp>(0)->node_kind == item->kind)
    return static_cast<_Tp>(item);

  return 0;
}

extern kdev_pg_allocator<char> _G_memory_pool;

template <class _Tp>
_Tp *create_node()
{
  _Tp *node = reinterpret_cast<_Tp*>(_G_memory_pool.allocate(sizeof(_Tp)));
  node->kind = _Tp::node_kind;
  return node;
}

#endif // KDEV_PG_AST_H
