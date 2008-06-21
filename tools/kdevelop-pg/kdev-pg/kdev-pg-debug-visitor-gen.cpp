/* This file is part of kdev-pg
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

#include "kdev-pg-debug-visitor-gen.h"
#include "kdev-pg.h"
#include <iostream>

void generate_debug_visitor::operator()()
{
  out << "class " << _G_system.export_macro << " debug_visitor: public default_visitor {" << std::endl
      << "public:" << std::endl;

  std::for_each(_G_system.symbols.begin(), _G_system.symbols.end(),
                gen_debug_visitor_rule(out));

  out << "};" << std::endl;
}

void gen_debug_visitor_rule::operator()(std::pair<std::string,
                                          model::symbol_item*> const &__it)
{
  model::symbol_item *sym = __it.second;

  bool has_members = false;
  has_member_nodes hms(has_members);
  hms(sym);

  out << "virtual void visit_" << sym->_M_name
      << "(" << sym->_M_name << "_ast *" << "node"
      << ") {" << std::endl;

  out << "std::cout << \"" << sym->_M_name << "\" << std::endl; " << std::endl;

  out << "default_visitor::visit_" << sym->_M_name
      << "(" << "node"
      << ");" << std::endl;

  out << "}" << std::endl << std::endl;
}
