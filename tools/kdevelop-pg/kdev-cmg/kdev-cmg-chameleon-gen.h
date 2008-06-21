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

#ifndef KDEV_CMG_CHAMELEON_GEN_H
#define KDEV_CMG_CHAMELEON_GEN_H

#include "kdevcmg_default_visitor.h"

#include <string>
#include <deque>


namespace kdevcmg
{

class generate_chameleon : public default_visitor
{
public:
  enum generator_mode {
    declaration,
    implementation
  };

private:
  struct method_signature {
    std::string method_name;
    std::string argument_type;

    method_signature(std::string _method_name, std::string _argument_type)
      : method_name(_method_name), argument_type(_argument_type)
    {}
  };
  typedef std::deque<method_signature> method_signature_list;

  std::ostream &out;
  method_signature_list method_signatures;
  generate_chameleon::generator_mode mode;

public:
  generate_chameleon(std::ostream &o, generate_chameleon::generator_mode gen_mode)
    : out(o), mode(gen_mode)
  {}

  void operator()();

protected:
  virtual void visit_code_model_declaration(code_model_declaration_ast *node);
  virtual void visit_item_declaration(item_declaration_ast *node);
  virtual void visit_non_item_type_declaration(non_item_type_declaration_ast *node);
  virtual void visit_member_item(member_item_ast *node);
};

class generate_chameleon_initializations : public default_visitor
{
  std::ostream &out;

public:
  generate_chameleon_initializations(std::ostream &o)
    : out(o)
  {}

  virtual void operator()();

protected:
  virtual void visit_item_declaration(item_declaration_ast *node);
};

class generate_chameleon_member : public default_visitor
{
  std::ostream &out;
  generate_chameleon::generator_mode mode;

  member_item_ast *_M_member;
  std::string _M_return_type, _M_method_name;
  std::string _M_argument_type, _M_argument_name;
  std::string _M_current_raw_type;

public:
  generate_chameleon_member(std::ostream &o, generate_chameleon::generator_mode gen_mode)
    : out(o), mode(gen_mode)
  {}

  virtual void operator()( member_item_ast *member,
                           std::string return_type, std::string method_name,
                           std::string argument_type, std::string argument_name );

protected:
  virtual void visit_item_declaration(item_declaration_ast *node);
};

} // end of namespace kdevcmg

#endif // KDEV_CMG_CHAMELEON_GEN_H

