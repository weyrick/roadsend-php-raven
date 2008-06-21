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

#include "kdev-pg-visitor-gen.h"
#include "kdev-pg.h"
#include <iostream>

void generate_visitor::operator()()
{
  out << "class " << _G_system.export_macro << " visitor {" << std::endl
      << "typedef void (visitor::*parser_fun_t)(ast_node *);" << std::endl
      << "static parser_fun_t _S_parser_table[];" << std::endl
      << std::endl
      << "public:" << std::endl
      << "virtual ~visitor() {}" << std::endl;

  out << "virtual void visit_node(ast_node *node) { "
      << "if (node) (this->*_S_parser_table[node->kind - 1000])(node); "
      << "}" << std::endl;

  for (std::map<std::string, model::symbol_item*>::iterator it = _G_system.symbols.begin();
       it != _G_system.symbols.end(); ++it)
    {
      model::symbol_item *sym = (*it).second;
      out << "virtual void visit_" << sym->_M_name
          << "(" << sym->_M_name << "_ast *) {}" << std::endl;
    }

  out << "};" << std::endl;
}

