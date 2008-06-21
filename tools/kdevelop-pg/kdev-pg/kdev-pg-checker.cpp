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

#include "kdev-pg-checker.h"
#include "kdev-pg-pretty-printer.h"

#include <iostream>
#include <iterator>
#include <algorithm>

//uncomment this to see debug output for follow checker
// #define FOLLOW_CHECKER_DEBUG

int problem_summary_printer::_M_first_first_conflict_count = 0;
int problem_summary_printer::_M_first_follow_conflict_count = 0;
int problem_summary_printer::_M_error_count = 0;

void FIRST_FIRST_conflict_checker::operator()(model::node *node)
{
  model::evolve_item *e = node_cast<model::evolve_item*>(node);
  assert(e != 0);
  _M_symbol = e->_M_symbol;
  visit_node(node);
}

void FIRST_FIRST_conflict_checker::visit_alternative(model::alternative_item *node)
{
  default_visitor::visit_alternative(node);

  _M_checked_node = node;
  check(node->_M_left, node->_M_right);
}

void FIRST_FIRST_conflict_checker::check(model::node *left, model::node *right)
{
  world::node_set const &left_first = _G_system.FIRST(left);
  world::node_set const &right_first = _G_system.FIRST(right);

  std::deque<model::node*> U;
  std::set_intersection(left_first.begin(), left_first.end(),
                        right_first.begin(), right_first.end(), std::back_inserter(U));

  if (!U.empty())
    {
      pretty_printer p(std::cerr);
      std::cerr << "** WARNING found FIRST/FIRST conflict in "
                << _M_symbol->_M_name << ":" << std::endl << "\tRule ``";
      p(_M_checked_node);
      //      p(left);
      std::cerr << "''" << std::endl << "\tTerminals [";

      std::deque<model::node*>::iterator it = U.begin();
      while (it != U.end())
        {
          model::node *n = *it++;

          std::cerr << n;
          if (it != U.end())
            std::cerr << ", ";
        }
      std::cerr << "]" << std::endl << std::endl;
      problem_summary_printer::report_first_first_conflict();
    }
}

void FIRST_FIRST_conflict_checker::visit_evolve(model::evolve_item *node)
{
  default_visitor::visit_evolve(node);

  world::environment::iterator it = _G_system.env.find(node->_M_symbol);
  while (it != _G_system.env.end())
    {
      model::symbol_item *sym = (*it).first;
      model::evolve_item *e = (*it).second;
      ++it;

      if (sym != node->_M_symbol || node == e)
        continue;

      _M_checked_node = node;
      check(e, node);
    }
}

void FIRST_FOLLOW_conflict_checker::operator()(model::node *node)
{
  model::evolve_item *e = node_cast<model::evolve_item*>(node);
  assert(e != 0);
  _M_symbol = e->_M_symbol;
  visit_node(node);
}

void FIRST_FOLLOW_conflict_checker::check(model::node *node, model::node *sym)
{
  if (!sym)
    sym = node;

  world::node_set const &first = _G_system.FIRST(node);
  world::node_set const &follow = _G_system.FOLLOW(sym);

  std::deque<model::node*> U;

  std::set_intersection(first.begin(), first.end(),
                        follow.begin(), follow.end(),
                        std::back_inserter(U));

  if (!U.empty())
    {
      pretty_printer p(std::cerr);
      std::cerr << "** WARNING found FIRST/FOLLOW conflict in "
                << _M_symbol->_M_name;
#ifdef FOLLOW_CHECKER_DEBUG
      std::cerr << "(" << (uint*)_M_symbol << ")";
#endif
      std::cerr << ":" << std::endl << "\tRule ``";
      p(node);
#ifdef FOLLOW_CHECKER_DEBUG
      std::cerr << " [[" << (uint*)node << "]]";
#endif
      std::cerr << "''" << std::endl << "\tTerminals [" << std::endl;

      std::deque<model::node*>::iterator it = U.begin();
      while (it != U.end())
        {
          model::node *n = *it++;
          if (is_zero(n))
            continue;

          std::cerr << "\t" << n << ": conflicts with the FIRST set of: ";
          follow_dep_checker(n).check(node);
          if (it != U.end())
            std::cerr << ", ";
          std::cerr << std::endl;
        }
      std::cerr << "\t" << "]" << std::endl << std::endl;
      problem_summary_printer::report_first_follow_conflict();
    }
}

void follow_dep_checker::check(model::node *node)
{
  //avoid cyclical follow dependency check
  if (_M_visited.find(node) != _M_visited.end())
    return;
  _M_visited.insert(node);

  world::follow_dep &D = _G_system.FOLLOW_DEP(node);
  world::node_set FD = D.first;
  world::node_set FLD = D.second;
  pretty_printer p(std::cerr);
#ifdef FOLLOW_CHECKER_DEBUG
  std::cerr << "[["; p(node); std::cerr << " | " << (uint*)node << "]] ";
  std::cerr << "{" << node->kind << "}";
#endif
  for (world::node_set::const_iterator it = FD.begin(); it != FD.end(); ++it)
    {
      world::node_set first = _G_system.FIRST(*it);
#ifdef FOLLOW_CHECKER_DEBUG
      std::cerr << " <iterating first ";
      for (world::node_set::const_iterator fit = first.begin(); fit != first.end(); ++fit)
      {
        p(*fit);
        std::cerr << " ";
      }
      std::cerr << " >";
#endif
      if (first.find(_M_terminal) != first.end())
      {
        std::cerr << std::endl << "            ";
        p(*it);
#ifdef FOLLOW_CHECKER_DEBUG
        std::cerr << " ( in \"";
        p(node);
        std::cerr << " \" )";
#endif
        std::cerr << ", ";
      }
    }
 for (world::node_set::const_iterator it = FLD.begin(); it != FLD.end(); ++it)
  {
      world::node_set first = _G_system.FIRST(*it);
#ifdef FOLLOW_CHECKER_DEBUG
      std::cerr << std::endl << "\t\t" << "in ";
      p(*it);
      std::cerr << std::endl;
#endif
      check(*it);
  }
}

void FIRST_FOLLOW_conflict_checker::visit_alternative(model::alternative_item *node)
{
  default_visitor::visit_alternative(node);

  if (is_zero(node->_M_right))
    return;

  if (reduces_to_epsilon(node))
    check(node);
}

void FIRST_FOLLOW_conflict_checker::visit_cons(model::cons_item *node)
{
  default_visitor::visit_cons(node);

  if (reduces_to_epsilon(node))
    check(node);
}

void FIRST_FOLLOW_conflict_checker::visit_plus(model::plus_item *node)
{
  default_visitor::visit_plus(node);

  if (reduces_to_epsilon(node))
    check(node);
}

void FIRST_FOLLOW_conflict_checker::visit_star(model::star_item *node)
{
  default_visitor::visit_star(node);

  check(node);
}

void undefined_symbol_checker::operator()(model::node *node)
{
  model::evolve_item *e = node_cast<model::evolve_item*>(node);
  assert(e != 0);
  _M_symbol = e->_M_symbol;
  visit_node(node);
}

void undefined_symbol_checker::visit_symbol(model::symbol_item *node)
{
  if (_G_system.env.count(node) == 0)
    {
      std::cerr << "** ERROR Undefined symbol ``" << node->_M_name << "'' in "
                << _M_symbol->_M_name << std::endl;
      problem_summary_printer::report_error();
    }
}

void undefined_symbol_checker::visit_variable_declaration(model::variable_declaration_item *node)
{
  if (node->_M_variable_type != model::variable_declaration_item::type_node)
    return;

  model::symbol_item *sym;

  std::string name = node->_M_type;
  world::symbol_set::iterator it = _G_system.symbols.find(name);
  if (it == _G_system.symbols.end())
    {
      std::cerr << "** ERROR Undefined symbol ``" << name
                << "'' (rule parameter declaration) in "
                << _M_symbol->_M_name << std::endl;
      problem_summary_printer::report_error();
      return;
    }
  else
    sym = (*it).second;

  if (_G_system.env.count(sym) == 0)
    {
      std::cerr << "** ERROR Undefined symbol ``" << node->_M_name
                << "'' (rule parameter declaration) in "
                << _M_symbol->_M_name << std::endl;
      problem_summary_printer::report_error();
    }
}

void undefined_token_checker::operator()(model::node *node)
{
  model::evolve_item *e = node_cast<model::evolve_item*>(node);
  assert(e != 0);
  _M_symbol = e->_M_symbol;
  visit_node(node);
}

void undefined_token_checker::visit_terminal(model::terminal_item *node)
{
  std::string name = node->_M_name;
  if (_G_system.terminals.find(name) == _G_system.terminals.end())
    {
      std::cerr << "** ERROR Undefined token ``" << node->_M_name << "'' in "
                << _M_symbol->_M_name << std::endl;
      problem_summary_printer::report_error();
    }
}

void empty_FIRST_checker::operator()(model::node *node)
{
  visit_node(node);
}

void empty_FIRST_checker::visit_symbol(model::symbol_item *node)
{
  if (_G_system.FIRST(node).empty())
    {
      std::cerr << "** ERROR Empty FIRST set for ``" << node->_M_name
                << "''" << std::endl;
      problem_summary_printer::report_error();
    }
}

void problem_summary_printer::operator()()
{
  std::cerr << (_M_first_first_conflict_count + _M_first_follow_conflict_count)
            << " conflicts total: " << _M_first_follow_conflict_count
            << " FIRST/FOLLOW conflicts, " << _M_first_first_conflict_count
            << " FIRST/FIRST conflicts." << std::endl;

  if (_M_error_count > 0)
    {
      std::cerr << _M_error_count << " fatal errors found, exiting."
                << std::endl;
      exit(EXIT_FAILURE);
    }
}

void problem_summary_printer::report_first_first_conflict()
{
  _M_first_first_conflict_count++;
}

void problem_summary_printer::report_first_follow_conflict()
{
  _M_first_follow_conflict_count++;
}

void problem_summary_printer::report_error()
{
  _M_error_count++;
}

// kate: space-indent on; indent-width 2; tab-width 2; show-tabs on;
