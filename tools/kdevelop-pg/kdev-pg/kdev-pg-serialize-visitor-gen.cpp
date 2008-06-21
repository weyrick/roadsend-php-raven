/* This file is part of kdev-pg
   Copyright (C) 2005 Roberto Raggi <roberto@kdevelop.org>
   Copyright (C) 2006 Jakob Petsovits <jpetso@gmx.at>
   Copyright (C) 2006 Adam Treat <treat@kde.org>

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

#include "kdev-pg-serialize-visitor-gen.h"
#include "kdev-pg.h"
#include <iostream>

void generate_serialize_visitor::operator()()
{
  out << "class " << _G_system.export_macro << " serialize: public default_visitor {" << std::endl
      << "public:" << std::endl;

  out << "static void read(kdev_pg_memory_pool *p," << std::endl
      << "ast_node *node, std::ifstream *i) { " << std::endl
      << "serialize(p, node, i); " << std::endl
      << "}" << std::endl << std::endl;

  out << "static void write(ast_node *node, std::ofstream *o) { " << std::endl
      << "serialize(node, o); " << std::endl
      << "}" << std::endl << std::endl;

  out << "private:" << std::endl;
  out << "serialize(kdev_pg_memory_pool *p," << std::endl
      << "ast_node *node, std::ifstream *i) : in(i), out(0) {" << std::endl
      << "memory_pool = p;" << std::endl
      << "if ( !node )" << std::endl
      << "node = create<" << _G_system.start->_M_symbol->_M_name << "_ast>();" << std::endl
      << "visit_node( node );" << std::endl
      << "}" << std::endl << std::endl;

  out << "serialize(ast_node *node, std::ofstream *o) : in(0), out(o) {" << std::endl
      << "visit_node( node );" << std::endl
      << "}" << std::endl << std::endl;

  out << "std::ifstream *in;" << std::endl;
  out << "std::ofstream *out;" << std::endl << std::endl;

  out << "// memory pool" << std::endl
      << "typedef kdev_pg_memory_pool memory_pool_type;" << std::endl
      << "kdev_pg_memory_pool *memory_pool;" << std::endl
      << "template <class T>" << std::endl
      << "inline T *create() {" << std::endl
      << "T *node = new (memory_pool->allocate(sizeof(T))) T();" << std::endl
      << "node->kind = T::KIND;" << std::endl
      << "return node;" << std::endl
      << "}" << std::endl;

  out << "template <class T, class E>" << std::endl
      << "void handle_list_node(const list_node<T> *t, E *e) {" << std::endl
      << "if (in) {" << std::endl

      //list in
      << "bool b;" << std::endl
      << "in->read(reinterpret_cast<char*>(&b), sizeof(bool));" << std::endl
      << "if (b) {" << std::endl
      << "int count;" << std::endl
      << "in->read(reinterpret_cast<char*>(&count), sizeof(int));" << std::endl
      << "for ( int i = 0; i < count; ++i ) {" << std::endl
      << "    e = create<E>();" << std::endl // FIXME: what about token
      << "    t = snoc(t, e, memory_pool);" << std::endl
      << "}" << std::endl
      << "}" << std::endl
      //end list in

      << "} else if (out) {" << std::endl

      //list out
      <<"if (t) {" << std::endl
      << "bool b = true;" << std::endl
      << "out->write(reinterpret_cast<char*>(&b), sizeof(bool));" << std::endl
      << "int c = t->count();" << std::endl
      << "out->write(reinterpret_cast<char*>(&c), sizeof(int));" << std::endl
      << "} else {" << std::endl
      << "bool b = false;" << std::endl
      << "out->write(reinterpret_cast<char*>(&b), sizeof(bool));" << std::endl
      << "}" << std::endl << std::endl
      //end list out

      << "}" << std::endl
      << "}" << std::endl << std::endl;

  out << "template <class T>" << std::endl
      << "void handle_ast_node(T *t) {" << std::endl
      << "if (in) {" << std::endl

      //ast in
      << "bool b;" << std::endl
      << "in->read(reinterpret_cast<char*>(&b), sizeof(bool));" << std::endl
      << "if (b) {" << std::endl
      << "t = create<T>();" << std::endl

      << "in->read(reinterpret_cast<char*>(&t->start_token)," << std::endl
      << "sizeof(std::size_t));" << std::endl
      << "in->read(reinterpret_cast<char*>(&t->end_token)," << std::endl
      << "sizeof(std::size_t));" << std::endl
      << "}" << std::endl
      //end ast in

      << "} else if (out) {" << std::endl

      //ast out
      << "if (t) {" << std::endl
      << "bool b = true;" << std::endl
      << "out->write(reinterpret_cast<char*>(&b), sizeof(bool));" << std::endl
      << "out->write(reinterpret_cast<char*>(&t->start_token)," << std::endl
      << "sizeof(std::size_t));" << std::endl
      << "out->write(reinterpret_cast<char*>(&t->end_token)," << std::endl
      << "sizeof(std::size_t));" << std::endl
      << "} else {" << std::endl
      << "bool b = false;" << std::endl
      << "out->write(reinterpret_cast<char*>(&b), sizeof(bool));" << std::endl
      << "}" << std::endl << std::endl
      //end ast out

      << "}" << std::endl
      << "}" << std::endl << std::endl;

  out << "template <class T>" << std::endl
      << "void handle_variable(T *t) {" << std::endl
      << "if (in) {" << std::endl
      << "in->read( reinterpret_cast<char*>(t), sizeof(T));" << std::endl
      << "} else if (out) {" << std::endl
      << "out->write( reinterpret_cast<char*>(t), sizeof(T));" << std::endl
      << "}" << std::endl
      << "}" << std::endl << std::endl;

  std::for_each(_G_system.symbols.begin(), _G_system.symbols.end(),
                gen_serialize_visitor_rule(out));

  out << "};" << std::endl;
}

void gen_serialize_visitor_rule::operator()(std::pair<std::string,
                                          model::symbol_item*> const &__it)
{
  _M_names.clear();
  _M_variable_declarations.clear();

  model::symbol_item *sym = __it.second;

  bool has_members = false;
  has_member_nodes hms(has_members);
  hms(sym);

  out << "virtual void visit_" << sym->_M_name
      << "(" << sym->_M_name << "_ast *" << "node"
      << ") {" << std::endl;

  world::environment::iterator it = _G_system.env.find(sym);
  while (it != _G_system.env.end())
    {
      model::evolve_item *e = (*it).second;
      if ((*it).first != sym)
        break;

      ++it;

      visit_node(e);
    }

  out << "default_visitor::visit_" << sym->_M_name
      << "(" << "node"
      << ");" << std::endl;

  out << "}" << std::endl << std::endl;
}

void gen_serialize_visitor_rule::visit_variable_declaration(model::variable_declaration_item *node)
{
  do
  {
    if (node->_M_storage_type != model::variable_declaration_item::storage_ast_member)
      break;

    if (_M_names.find(node->_M_name) != _M_names.end())
      break;

    std::string ext =
        ( node->_M_variable_type == model::variable_declaration_item::type_node ?
        "_ast" : "");

    std::string type = std::string(node->_M_type) + ext;
    std::string name = std::string(node->_M_name);

    if (node->_M_variable_type == model::variable_declaration_item::type_token)
      type = "std::size_t";

    if (node->_M_is_sequence)
    {
      out << "{" << std::endl
          << type << " *e = 0;" << std::endl
          << "handle_list_node(node->" << name << "_sequence, e);" << std::endl
          << "}" << std::endl;
    }
    else if (node->_M_variable_type == model::variable_declaration_item::type_node)
    {
      out << "handle_ast_node(node->" << name << ");" << std::endl;
    }
    else if (node->_M_variable_type == model::variable_declaration_item::type_variable
             || node->_M_variable_type == model::variable_declaration_item::type_token)
    {
      out << "handle_variable(&node->" << name << ");" << std::endl;
    }
    else
    {
      assert(0); // every variable type must be supported
    }

    _M_names.insert(node->_M_name);
    _M_variable_declarations.push_back(node);

  } while(false);

  default_visitor::visit_variable_declaration(node);
}
