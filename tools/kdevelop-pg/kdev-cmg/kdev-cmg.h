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

#ifndef KDEV_CMG_H
#define KDEV_CMG_H

#include "kdevcmg_ast.h"

#include <string>
#include <map>

struct world
{
  typedef std::map<std::string, std::string>     member_type_map;
  typedef std::map<std::string, member_type_map> class_member_map;

  world()
    : ast(0)
  {}

  char *contents;
  kdevcmg::model_ast *ast;
  std::string language;
  std::string declarations;
  std::string implementation;

  void register_type(std::string name, std::string type_name, std::string containing_class)
  {
    member_types[containing_class][name] = type_name;
  }

  void inherit_types(std::string base_class, std::string inheriting_class)
  {
    member_types[inheriting_class] = member_types[base_class];
  }

  std::string type_of(std::string name, std::string containing_class)
  {
    class_member_map::iterator clsit = member_types.find(containing_class);

    if (clsit != member_types.end())
      {
        member_type_map::iterator typeit = (*clsit).second.find(name);

        if (typeit != (*clsit).second.end())
          return (*typeit).second;
      }
    return "";
  }

private:
  class_member_map member_types;
};

extern world _G_system;

#endif // KDEV_CMG_H
