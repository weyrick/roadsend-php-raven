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

#include "kdev-cmg-string-tools.h"

std::string string_tools::accessor_name(const std::string &prefix, const std::string &variable_name)
{
  std::string adapted_variable_name =
    (variable_name.substr(0,2) == "is") ? variable_name.substr(2) : variable_name;

  if (adapted_variable_name[0] >= 'a' && adapted_variable_name[0] <= 'z')
    adapted_variable_name[0] = adapted_variable_name[0] - 'a' + 'A';

  return prefix + adapted_variable_name;
}

std::string string_tools::plural(const std::string &singular)
{
  if (singular[singular.length()-1] == 's')
    return singular + "es";
  else
    return singular + "s";
}

