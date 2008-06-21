/* This file is part of kdev-cmg
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

#include "kdev-cmg-forward-declarations-gen.h"

#include "kdev-cmg.h"

#include <iostream>


namespace kdevcmg
{

void generate_forward_declarations::operator()()
{
  out << "// forward declarations" << std::endl;

  mode = class_forwards;
  visit_node(_G_system.ast);
  out << std::endl;

  mode = item_typedef_forwards;
  visit_node(_G_system.ast);
  out << std::endl;

  mode = list_typedef_forwards;
  visit_node(_G_system.ast);
  out << std::endl;

  out << std::endl;
}

void generate_forward_declarations::visit_code_model_declaration(code_model_declaration_ast *node)
{
  if (mode == class_forwards)
    out << "class CodeModel;" << std::endl;
}

void generate_forward_declarations::visit_item_declaration(item_declaration_ast *node)
{
  if (mode == class_forwards)
    {
      out << "class _" << node->type->type_name << ";" << std::endl;
    }

  else if (mode == item_typedef_forwards)
    {
      out << "typedef KDevelop::SharedPtr<_" << node->type->type_name << "> "
          << node->type->type_name << ";" << std::endl;
    }

  else if (mode == list_typedef_forwards)
    {
      out << "typedef QList<" << node->type->type_name << "> "
          << node->type->raw_type_name << "List" << ";" << std::endl;

      if (node->type->type_name == "CodeModelItem")
        {
          out << "typedef QList<CodeModelItem> ItemList;" << std::endl;
        }
    }
}

void generate_forward_declarations::visit_non_item_type_declaration(non_item_type_declaration_ast *node)
{
  if (mode == class_forwards)
    out << "class " << node->type->type_name << ";" << std::endl;

  else if (mode == list_typedef_forwards)
    {
      out << "typedef QList<" << node->type->type_name << "> "
          << node->type->type_name << "List" << ";" << std::endl;
    }
}

} // end of namespace kdevcmg

