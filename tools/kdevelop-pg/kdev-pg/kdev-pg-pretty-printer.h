/* This file is part of kdev-pg
   Copyright (C) 2005 Roberto Raggi <roberto@kdevelop.org>

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

#ifndef KDEV_PG_PRETTY_PRINTER_H
#define KDEV_PG_PRETTY_PRINTER_H

#include "kdev-pg-default-visitor.h"

struct pretty_printer: protected default_visitor
{
  std::ostream &out;

  pretty_printer(std::ostream &o): out(o)
  {}

  void operator()(model::node *node)
  {
    visit_node(node);
  }

protected:
  virtual void visit_zero(model::zero_item *node);
  virtual void visit_plus(model::plus_item *node);
  virtual void visit_star(model::star_item *node);
  virtual void visit_symbol(model::symbol_item *node);
  virtual void visit_action(model::action_item *node);
  virtual void visit_alternative(model::alternative_item *node);
  virtual void visit_cons(model::cons_item *node);
  virtual void visit_evolve(model::evolve_item *node);
  virtual void visit_try_catch(model::try_catch_item *node);
  virtual void visit_alias(model::alias_item *node);
  virtual void visit_terminal(model::terminal_item *node);
  virtual void visit_nonterminal(model::nonterminal_item *node);
  virtual void visit_annotation(model::annotation_item *node);
};

#endif // KDEV_PG_PRETTY_PRINTER_H
