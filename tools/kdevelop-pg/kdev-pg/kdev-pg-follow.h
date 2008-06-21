/* This file is part of kdev-pg
   Copyright (C) 2005 Roberto Raggi <roberto@kdevelop.org>
   Copyright (C) 2006 Jakob Petsovits <jpetso@gmx.at>
   Copyright (C) 2006 Alexander Dymo <adymo@kdevelop.org>

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

#ifndef KDEV_PG_FOLLOW_H
#define KDEV_PG_FOLLOW_H

#include "kdev-pg.h"
#include "kdev-pg-default-visitor.h"

struct initialize_FOLLOW: protected default_visitor
{
  void operator()(model::node *node);
protected:
  virtual void visit_symbol(model::symbol_item *node);
};

struct next_FOLLOW: protected default_visitor
{
  next_FOLLOW(bool &changed);

  void operator()(model::node *node);

protected:
  void merge(model::node *__dest, world::node_set const &source);
  /**adds dependency between rule @p dep whose FIRST set is added to
  @p dest FOLLOW set*/
  void add_first_to_follow_dep(model::node *dest, model::node *dep);
  /**adds dependency between rule @p dep whose FOLLOW set is added to
  @p dest FOLLOW set*/
  void add_follow_to_follow_dep(model::node *dest, model::node *dep);

  virtual void visit_plus(model::plus_item *node);
  virtual void visit_star(model::star_item *node);
  virtual void visit_action(model::action_item *node);
  virtual void visit_alternative(model::alternative_item *node);
  virtual void visit_cons(model::cons_item *node);
  virtual void visit_evolve(model::evolve_item *node);
  virtual void visit_try_catch(model::try_catch_item *node);
  virtual void visit_annotation(model::annotation_item *node);
  virtual void visit_condition(model::condition_item *node);
  virtual void visit_nonterminal(model::nonterminal_item *node);

private:
  bool &_M_changed;
  model::symbol_item *_M_symbol;
};

void compute_FOLLOW();

#endif // KDEV_PG_FOLLOW_H
