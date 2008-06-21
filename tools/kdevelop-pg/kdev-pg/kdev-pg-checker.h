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

#ifndef KDEV_PG_CHECKER_H
#define KDEV_PG_CHECKER_H

#include "kdev-pg.h"
#include "kdev-pg-default-visitor.h"

struct empty_FIRST_checker: protected default_visitor
{
  void operator()(model::node *node);

protected:
  virtual void visit_symbol(model::symbol_item *node);
};

struct FIRST_FIRST_conflict_checker: protected default_visitor
{
  void operator()(model::node *node);

protected:
  void check(model::node *left, model::node *right);

  virtual void visit_evolve(model::evolve_item *node);
  virtual void visit_alternative(model::alternative_item *node);

private:
  model::symbol_item *_M_symbol;
  model::node *_M_checked_node;
};

struct FIRST_FOLLOW_conflict_checker: protected default_visitor
{
  void operator()(model::node *node);

protected:
  void check(model::node *node, model::node *sym = 0);

  virtual void visit_alternative(model::alternative_item *node);
  virtual void visit_cons(model::cons_item *node);
  virtual void visit_plus(model::plus_item *node);
  virtual void visit_star(model::star_item *node);

private:
  model::symbol_item *_M_symbol;
};

struct follow_dep_checker
{
  follow_dep_checker(model::node *terminal): _M_terminal(terminal) {}
  void check(model::node *n);

private:
  model::node *_M_terminal;
  world::node_set _M_visited;
};

struct undefined_symbol_checker: protected default_visitor
{
  void operator()(model::node *node);

protected:
  virtual void visit_symbol(model::symbol_item *node);
  virtual void visit_variable_declaration(model::variable_declaration_item *node);

private:
  model::symbol_item *_M_symbol;
};

struct undefined_token_checker: protected default_visitor
{
  void operator()(model::node *node);

protected:
  virtual void visit_terminal(model::terminal_item *node);

private:
  model::symbol_item *_M_symbol;
};

struct problem_summary_printer
{
  void operator()();

  static void report_first_first_conflict();
  static void report_first_follow_conflict();
  static void report_error();

private:
  static int _M_first_first_conflict_count;
  static int _M_first_follow_conflict_count;
  static int _M_error_count;
};

#endif // KDEV_PG_CHECKER_H

// kate: space-indent on; indent-width 2; tab-width 2; show-tabs on;
