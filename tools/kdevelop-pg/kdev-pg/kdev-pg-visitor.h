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

#ifndef KDEV_PG_VISITOR_H
#define KDEV_PG_VISITOR_H

#include "kdev-pg-ast.h"

struct visitor
{
  virtual ~visitor()
  {}

  virtual void visit_node(model::node *node)
  {
    if (node)
      (this->*_S_table[node->kind])(node);
  }

protected:
  virtual void visit_zero(model::zero_item *) {}
  virtual void visit_plus(model::plus_item *) {}
  virtual void visit_star(model::star_item *) {}
  virtual void visit_symbol(model::symbol_item *) {}
  virtual void visit_action(model::action_item *) {}
  virtual void visit_alternative(model::alternative_item *) {}
  virtual void visit_cons(model::cons_item *) {}
  virtual void visit_evolve(model::evolve_item *) {}
  virtual void visit_try_catch(model::try_catch_item *) {}
  virtual void visit_alias(model::alias_item *) {}
  virtual void visit_terminal(model::terminal_item *) {}
  virtual void visit_nonterminal(model::nonterminal_item *) {}
  virtual void visit_annotation(model::annotation_item *) {}
  virtual void visit_condition(model::condition_item *) {}
  virtual void visit_variable_declaration(model::variable_declaration_item *) {}

private:
  typedef void (visitor::*visitor_fun_ptr)(model::node *item);
  static visitor_fun_ptr _S_table[];
};

#endif // KDEV_PG_VISITOR_H

