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

#ifndef KDEV_CMG_IMPLEMENTATION_GEN_H
#define KDEV_CMG_IMPLEMENTATION_GEN_H

#include "kdevcmg_default_visitor.h"


namespace kdevcmg
{

class generate_implementation : public default_visitor
{
  std::ostream &out;
  std::string current_class;

public:
  generate_implementation(std::ostream &o): out(o)
  {}

  void operator ()();

protected:
  virtual void visit_class_declaration(class_declaration_ast *node);
  virtual void visit_code_model_declaration(code_model_declaration_ast *node);
  virtual void visit_root_item(root_item_ast *node);
  virtual void visit_item_declaration(item_declaration_ast *node);
  virtual void visit_non_item_type_declaration(non_item_type_declaration_ast *node);
  virtual void visit_member_item(member_item_ast *node);
  virtual void visit_code_item(code_item_ast *node);
};


class generate_member_initializations : public default_visitor
{
  std::ostream &out;

public:
  generate_member_initializations(std::ostream &o): out(o)
  {}

  void operator ()(ast_node *node);

protected:
  virtual void visit_member_item(member_item_ast *node);
};


class generate_code : public default_visitor
{
  std::ostream &out;
  code_item::item_type_enum type;

public:
  generate_code(std::ostream &o): out(o), type(code_item::type_constructor)
  {}

  void operator ()(ast_node *node, code_item::item_type_enum code_type);

protected:
  virtual void visit_code_item(code_item_ast *node);
};

} // end of namespace kdevcmg

#endif // KDEV_IMPLEMENTATION_GEN_H

