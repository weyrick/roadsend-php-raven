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

#ifndef KDEV_PG_DEFAULT_VISITOR_BITS_GEN_H
#define KDEV_PG_DEFAULT_VISITOR_BITS_GEN_H

#include "kdev-pg-default-visitor.h"

#include <set>
#include <list>
#include <string>

class gen_default_visitor_bits_rule: protected default_visitor
{
  std::ostream &out;
  std::set<std::string> _M_names;
  std::list<model::variable_declaration_item*> _M_variable_declarations;

public:
  gen_default_visitor_bits_rule(std::ostream &o): out(o)
  {}

  void operator()(std::pair<std::string, model::symbol_item*> const &__it);

protected:
  virtual void visit_variable_declaration(model::variable_declaration_item *node);
};

class has_member_nodes: protected default_visitor
{
  bool &has_members;

public:
  has_member_nodes(bool &result): has_members(result)
  {}

  void operator()(model::symbol_item *sym);

protected:
  virtual void visit_variable_declaration(model::variable_declaration_item *node);
};


#endif // KDEV_PG_DEFAULT_VISITOR_BITS_GEN_H
