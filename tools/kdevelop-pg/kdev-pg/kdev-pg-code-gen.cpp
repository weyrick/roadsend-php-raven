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

#include "kdev-pg.h"
#include "kdev-pg-code-gen.h"

#include <list>
#include <stack>
#include <iostream>
#include <cassert>

namespace
{
  void gen_condition(world::node_set const &s, std::ostream &out)
  {
    bool initial = true;
    model::node *item = _G_system.zero();

    world::node_set::iterator it = s.begin();
    while (it != s.end())
      {
        item = *it++;

        if (model::terminal_item *t = node_cast<model::terminal_item*>(item))
          {
            if (!initial)
              out << std::endl << "|| ";

            out << "yytoken == Token_" << t->_M_name;
            initial = false;
          }
      }

    if (initial && is_zero(item))
      out << "true /*epsilon*/";
  }

  void gen_test_condition(model::node *node, std::ostream &out)
  {
    world::node_set s = _G_system.FIRST(node);
    gen_condition(s, out);
  }

  std::string gen_parser_call(model::nonterminal_item *node, int catch_id, std::ostream &out)
  {
    static int __id = 0;
    static char __var[1024];
    char const *symbol_name = node->_M_symbol->_M_name;

    if (_G_system.generate_ast)
      {
        sprintf(__var, "__node_%d", __id++);

        out << symbol_name << "_ast *" << __var << " = 0;" << std::endl
            << "if (!parse_" << symbol_name << "(&" << __var;

        if (node->_M_arguments[0] != '\0') // read: if (_M_arguments != "")
            out << ", " << node->_M_arguments;

        out << "))" << std::endl;
      }
    else
      {
        out << "if (!parse_" << symbol_name << "(" << node->_M_arguments << "))"
            << std::endl;
      }

    if (!catch_id)
      {
        out << "{" << std::endl;

        if (_G_system.need_state_management)
          out <<   "if (!yy_block_errors) {" << std::endl;

        out << "yy_expected_symbol(ast_node::Kind_" << symbol_name
            << ", \"" << symbol_name << "\"" << ");" << std::endl;

        if (_G_system.need_state_management)
          out << "}" << std::endl;

        out << "return false;" << std::endl
            << "}" << std::endl;
      }
    else
      {
        out << "{ goto __catch_" << catch_id << "; }" << std::endl;
      }

    return __var;
  }

  void gen_token_test(model::terminal_item *node, int catch_id, std::ostream &out)
  {
    out << "if (yytoken != Token_" << node->_M_name << ")" << std::endl;
    if (!catch_id)
      {
        out << "{" << std::endl;

        if (_G_system.need_state_management)
          out << "if (!yy_block_errors) {" << std::endl;

        out << "yy_expected_token(yytoken, Token_" << node->_M_name
            << ", \"" << node->_M_description << "\");" << std::endl;

        if (_G_system.need_state_management)
          out << "}" << std::endl;

        out << "return false;" << std::endl
            << "}" << std::endl;
      }
    else
      {
        out << "goto __catch_" << catch_id << ";" << std::endl;
      }
  }

  void gen_recovery(model::node *node, int catch_id, std::ostream &out)
  {
    world::node_set s = _G_system.FOLLOW(node);
    model::node *item = _G_system.zero();

    out << "if (try_start_token_" << catch_id
        << " == token_stream->index() - 1)" << std::endl
        << "yylex();" << std::endl
        << std::endl;

    out << "while (yytoken != Token_EOF";

    world::node_set::iterator it = s.begin();
    while (it != s.end())
      {
        item = *it++;

        if (model::terminal_item *t = node_cast<model::terminal_item*>(item))
          out << std::endl << "&& yytoken != Token_" << t->_M_name;
      }

    out << ")" << std::endl
        << "{ yylex(); }" << std::endl;
  }


} // namespace

void code_generator::operator()(model::node *node)
{
  _M_evolve = node_cast<model::evolve_item*>(node);
  assert(_M_evolve != 0);
  visit_node(node);
}

void code_generator::visit_zero(model::zero_item *node)
{
  // out << " /* nothing to do */" << std::endl;
}

void code_generator::visit_symbol(model::symbol_item *node)
{
  // out << " /* nothing to do */" << std::endl;
}

void code_generator::visit_nonterminal(model::nonterminal_item *node)
{
  gen_parser_call(node, _M_current_catch_id, out);
}

void code_generator::visit_terminal(model::terminal_item *node)
{
  gen_token_test(node, _M_current_catch_id, out);

  out << "yylex();" << std::endl
      << std::endl;
}

void code_generator::visit_plus(model::plus_item *node)
{
  out << "do {" << std::endl;
  visit_node(node->_M_item);
  out << "} while (";
  gen_test_condition(node, out);
  out << ");" << std::endl;
}

void code_generator::visit_star(model::star_item *node)
{
  out << "while (";
  gen_test_condition(node, out);
  out << ") {" << std::endl;
  visit_node(node->_M_item);
  out << "}" << std::endl;
}

void code_generator::visit_action(model::action_item *node)
{
  default_visitor::visit_action(node);
  out << node->_M_code;
}

void code_generator::visit_condition(model::condition_item *node)
{
  default_visitor::visit_condition(node);
}

void code_generator::visit_alternative(model::alternative_item *node)
{
  std::list<model::node*> top_level_nodes;

  std::stack<model::node*> working_list;
  working_list.push(node->_M_right);
  working_list.push(node->_M_left);

  while (!working_list.empty())
    {
      model::node *n = working_list.top();
      working_list.pop();

      if (model::alternative_item *a = node_cast<model::alternative_item*>(n))
        {
          working_list.push(a->_M_right);
          working_list.push(a->_M_left);
        }
      else
        {
          top_level_nodes.push_back(n);
        }
    }

  std::list<model::node*>::iterator it = top_level_nodes.begin();
  while (it != top_level_nodes.end())
    {
      model::node *n = *it++;
      model::condition_item *cond = node_cast<model::condition_item*>(n);

      out << "if (";

      if (cond)
        out << "(";

      gen_test_condition(n, out);

      if (cond)
        out << ") && (" << cond->_M_code << ")";

      out << ") {" << std::endl;
      visit_node(n);
      out << "}";

      if (it != top_level_nodes.end())
        out << "else ";
      else
        {
          out << "else {" << std::endl;

          if (!_M_current_catch_id)
              out << "return false;" << std::endl;
          else
              out << "goto __catch_" << _M_current_catch_id << ";";

          out << "}" << std::endl;
        }
    }
}

void code_generator::visit_cons(model::cons_item *node)
{
  default_visitor::visit_cons(node);
}

void code_generator::visit_evolve(model::evolve_item *node)
{
  out << "if (";

  model::condition_item *cond = node_cast<model::condition_item*>(node->_M_item);

  if (cond)
    out << "(";

  gen_test_condition(node, out);

  if (reduces_to_epsilon(node->_M_item))
    {
      out << " || ";
      gen_condition(_G_system.FOLLOW(node->_M_symbol), out);
    }

  if (cond)
    out << ") && (" << cond->_M_code << ")";

  out << ") {" << std::endl;

  gen_local_decls gen_locals(out, _M_names);
  gen_locals(node->_M_item);

  out << node->_M_code;

  visit_node(node->_M_item);

  if (node == _G_system.start)
    out << "if (Token_EOF != yytoken) { return false; }" << std::endl;

  out << "}" << std::endl;
}

void code_generator::visit_try_catch(model::try_catch_item *node)
{
  static int try_catch_counter = 0;
  int previous_catch_id = set_catch_id(++try_catch_counter);

  if (node->_M_catch_item) // node is a try/rollback block
    {
      out << "bool block_errors_" << _M_current_catch_id
          << " = block_errors(true);" << std::endl;
    }

  out << "std::size_t try_start_token_" << _M_current_catch_id
      << " = token_stream->index() - 1;" << std::endl;

  if (!node->_M_unsafe)
    {
      out << "parser_state *try_start_state_" << _M_current_catch_id
          << " = copy_current_state();" << std::endl;
    }

  out << "{" << std::endl;
  visit_node(node->_M_try_item);
  out << "}" << std::endl;

  if (node->_M_catch_item)
    {
      out << "block_errors(block_errors_" << _M_current_catch_id << ");" << std::endl;
    }

  if (!node->_M_unsafe)
    {
      out << "if (try_start_state_" << _M_current_catch_id << ")" << std::endl
          << "delete try_start_state_" <<  _M_current_catch_id << ";" << std::endl
          << std::endl;
    }

  out << "if (false) // the only way to enter here is using goto" << std::endl
      << "{" << std::endl
      << "__catch_" << _M_current_catch_id << ":" << std::endl;

  if (!node->_M_unsafe)
    {
      out << "if (try_start_state_" << _M_current_catch_id << ")" << std::endl
          << "{" << std::endl
          << "restore_state(try_start_state_" <<  _M_current_catch_id << ");" << std::endl
          << "delete try_start_state_" <<  _M_current_catch_id << ";" << std::endl
          << "}" << std::endl;
    }

  if (!node->_M_catch_item)
    {
      gen_recovery(node, _M_current_catch_id, out);
      set_catch_id(previous_catch_id);
    }
  else
    {
      out << "block_errors(block_errors_" << _M_current_catch_id << ");" << std::endl
          << "rewind(try_start_token_" << _M_current_catch_id << ");" << std::endl
          << std::endl;

      set_catch_id(previous_catch_id);
      visit_node(node->_M_catch_item);
    }

  out << "}" << std::endl
      << std::endl;
}

int code_generator::set_catch_id(int catch_id)
{
  int previous = _M_current_catch_id;
  _M_current_catch_id = catch_id;
  return previous;
}

void code_generator::visit_alias(model::alias_item *node)
{
  assert(0); // ### not implemented yet
}

void code_generator::visit_annotation(model::annotation_item *node)
{
  if (!_G_system.generate_ast)
    {
      // std::cerr << "** WARNING annotation ignored" << std::endl;
      visit_node(node->_M_item);
      return;
    }

  if (model::terminal_item *t = node_cast<model::terminal_item*>(node->_M_item))
    {
      gen_token_test(t, _M_current_catch_id, out);

      if (node->_M_declaration->_M_is_sequence)
        {
          std::string target;

          if (node->_M_declaration->_M_storage_type == model::variable_declaration_item::storage_ast_member)
            target += "(*yynode)->";

          target += node->_M_declaration->_M_name;
          target += "_sequence";

          out << target << " = snoc(" << target << ", "
              << "token_stream->index() - 1, memory_pool);" << std::endl
              << "yylex();" << std::endl
              << std::endl;
        }
      else
        {
          if (node->_M_declaration->_M_storage_type == model::variable_declaration_item::storage_ast_member)
            out << "(*yynode)->";

          out << node->_M_declaration->_M_name
              << " = token_stream->index() - 1;" << std::endl
              << "yylex();" << std::endl
              << std::endl;
        }
    }
  else if (model::nonterminal_item *nt = node_cast<model::nonterminal_item*>(node->_M_item))
    {
      std::string __var = gen_parser_call(nt, _M_current_catch_id, out);

      bool check_start_token = false;
      world::environment::iterator it = _G_system.env.find(nt->_M_symbol);
      while (it != _G_system.env.end())
        {
          model::evolve_item *e = (*it).second;
          if ((*it).first != nt->_M_symbol)
            break;

          ++it;

          model::variable_declaration_item *current_decl = e->_M_declarations;
          while (current_decl)
            {
              if ((current_decl->_M_declaration_type
                   == model::variable_declaration_item::declaration_argument)
                  &&
                  (current_decl->_M_variable_type
                   != model::variable_declaration_item::type_variable))
                {
                  check_start_token = true;
                  break;
                }

              current_decl = current_decl->_M_next;
            }
        }

      if (check_start_token == true)
        {
          check_start_token = false;
          model::variable_declaration_item *current_decl = _M_evolve->_M_declarations;
          while (current_decl)
            {
              if ((current_decl->_M_storage_type
                   == model::variable_declaration_item::storage_temporary)
                  &&
                  (current_decl->_M_variable_type
                   != model::variable_declaration_item::type_variable)
                  &&
                  (current_decl->_M_declaration_type
                   == model::variable_declaration_item::declaration_argument))
                {
                  check_start_token = true;
                  break;
                }

              current_decl = current_decl->_M_next;
            }
        }

      if (check_start_token == true)
        {
          out << "if (" << __var << "->start_token < (*yynode)->start_token)" << std::endl
              << "(*yynode)->start_token = " << __var << "->start_token;" << std::endl;
        }

      std::string target;
      if (node->_M_declaration->_M_storage_type == model::variable_declaration_item::storage_ast_member)
        target += "(*yynode)->";

      target += node->_M_declaration->_M_name;

      if (node->_M_declaration->_M_is_sequence)
        {
          target += "_sequence";

          out << target << " = " << "snoc(" << target << ", "
              << __var << ", memory_pool);" << std::endl
              << std::endl;
        }
      else
        {
          out << target << " = " << __var << ";" << std::endl
              << std::endl;
        }
    }
  else
    assert(0); // ### not supported
}


void gen_forward_parser_rule::operator()(std::pair<std::string, model::symbol_item*> const &__it)
{
  model::symbol_item *sym = __it.second;
  out << "bool" << " " << "parse_" << sym->_M_name << "(";

  gen_parse_method_signature gen_signature(out, 0);
  gen_signature(sym);

  out << ");" << std::endl;
}

void gen_parser_rule::operator()(std::pair<std::string, model::symbol_item*> const &__it)
{
  _M_names.clear();
  model::symbol_item *sym = __it.second;
  code_generator cg(out, &_M_names);

  out << "bool parser::parse_" << sym->_M_name << "(";

  gen_parse_method_signature gen_signature(out, &_M_names);
  gen_signature(sym);

  out << ")" << std::endl
      << "{" << std::endl;

  if (_G_system.generate_ast)
    {
      out << "*yynode = create<" << sym->_M_name << "_ast" << ">();" << std::endl << std::endl
          << "(*yynode)->start_token = token_stream->index() - 1;" << std::endl
          << std::endl;
    }

  world::environment::iterator it = _G_system.env.find(sym);
  while (it != _G_system.env.end())
    {
      model::evolve_item *e = (*it).second;
      if ((*it).first != sym)
        break;

      ++it;

      gen_local_decls gen_locals(out, &_M_names);
      gen_locals(e->_M_declarations);
    }

  it = _G_system.env.find(sym);
  bool initial = true;
  while (it != _G_system.env.end())
    {
      model::evolve_item *e = (*it).second;
      if ((*it).first != sym)
        break;

      ++it;

      if (!initial)
        out << "else ";

      cg(e);
      initial = false;
    }

  out << "else" << std::endl << "{ return false; }" << std::endl
      << std::endl;


  if (_G_system.generate_ast)
    {
      out << "(*yynode)->end_token = token_stream->index() - 1;" << std::endl
          << std::endl;
    }

  out << "return true;" << std::endl
      << "}" << std::endl
      << std::endl;
}

void gen_local_decls::operator()(model::node *node)
{
  visit_node(node);
}

void gen_local_decls::visit_variable_declaration(model::variable_declaration_item *node)
{
  // normal declarations for local variables
  if (node->_M_storage_type == model::variable_declaration_item::storage_temporary
      && node->_M_declaration_type == model::variable_declaration_item::declaration_local)
    {
      if ((_M_names == 0) || _M_names->find(node->_M_name) == _M_names->end())
        {
          gen_variable_declaration gen_var_decl(out);
          gen_var_decl(node);

          if (node->_M_variable_type == model::variable_declaration_item::type_token
              || node->_M_variable_type == model::variable_declaration_item::type_node
              || node->_M_is_sequence)
            {
              out << " = 0";
            }

          out << ";" << std::endl << std::endl;

          if (_M_names != 0)
            _M_names->insert(node->_M_name);
        }
    }

  // for argument member nodes and tokens, check if their index precedes the current one
  else if (node->_M_storage_type == model::variable_declaration_item::storage_ast_member
           && node->_M_declaration_type == model::variable_declaration_item::declaration_argument
           && _G_system.generate_ast)
    {
      char const *sequence_suffix = (node->_M_is_sequence) ? "_sequence" : "";

      out << "(*yynode)->" << node->_M_name << sequence_suffix << " = "
          << node->_M_name << sequence_suffix << ";" << std::endl;

      if (node->_M_variable_type != model::variable_declaration_item::type_variable)
        {
          std::string argument_start_token = node->_M_name;
          if (node->_M_is_sequence)
            argument_start_token += "_sequence->to_front()->element";

          if (node->_M_variable_type == model::variable_declaration_item::type_node)
            argument_start_token += "->start_token";

          out << "if (";

          // check that the variable is not null
          if (node->_M_variable_type == model::variable_declaration_item::type_node
              || node->_M_is_sequence)
            {
              out << node->_M_name << sequence_suffix << " && ";
            }

          out << argument_start_token << " < (*yynode)->start_token)" << std::endl
              << "(*yynode)->start_token = " << argument_start_token << ";"
            << std::endl << std::endl;
        }
    }

  default_visitor::visit_variable_declaration(node);
}

void gen_parse_method_signature::operator()(model::symbol_item* sym)
{
  if (_G_system.generate_ast)
    {
      out << sym->_M_name << "_ast **yynode";
      first_parameter = false;
    }

  world::environment::iterator it = _G_system.env.find(sym);
  if (it != _G_system.env.end())
    {
      // this creates the method signature using just the first of
      // possibly multiple rules with the same name.
      model::evolve_item *e = (*it).second;
      if (e->_M_declarations)
        visit_node(e->_M_declarations);
    }
}

void gen_parse_method_signature::visit_variable_declaration(model::variable_declaration_item *node)
{
  if (node->_M_declaration_type == model::variable_declaration_item::declaration_argument)
    {
      if (_G_system.generate_ast || (node->_M_storage_type != model::variable_declaration_item::storage_ast_member))
        {
          if (first_parameter)
            first_parameter = false;
          else
            out << ", ";

          gen_variable_declaration gen_var_decl(out);
          gen_var_decl(node);

          if (_M_names != 0)
            _M_names->insert(node->_M_name);
        }
    }

  default_visitor::visit_variable_declaration(node);
}

void gen_variable_declaration::operator()(model::variable_declaration_item *node)
{
  if (node->_M_is_sequence)
    out << "const list_node<";

  if (node->_M_variable_type == model::variable_declaration_item::type_token)
    out << "std::size_t ";
  else if (node->_M_variable_type == model::variable_declaration_item::type_node)
    out << node->_M_type << "_ast *";
  else if (node->_M_variable_type == model::variable_declaration_item::type_variable)
    out << node->_M_type << " ";
  else
    assert(0); // ### not supported

  if (node->_M_is_sequence)
    out << "> *";

  out << node->_M_name;

  if (node->_M_is_sequence)
    out << "_sequence";
}

void gen_token::operator()(std::pair<std::string, model::terminal_item*> const &__it)
{
  model::terminal_item *t = __it.second;
  out << "Token_" << t->_M_name << " = " << _M_token_value++ << "," << std::endl;
}

void gen_member_code::operator()(settings::member_item* m)
{
  if ((_M_kind_mask & m->_M_member_kind) != 0)
    {
      if (m->_M_member_kind == settings::member_item::public_declaration)
        out << "public:" << std::endl;
      else if (m->_M_member_kind == settings::member_item::protected_declaration)
        out << "protected:" << std::endl;
      else if (m->_M_member_kind == settings::member_item::private_declaration)
        out << "private:" << std::endl;

      out << m->_M_code << std::endl;
    }
}

void generate_parser_decls::operator()()
{
  out << "class " << _G_system.export_macro << " parser {"
      << "public:" << std::endl
      << "typedef " << _G_system.token_stream << " token_stream_type;" << std::endl
      << "typedef " << _G_system.token_stream << "::token_type token_type;" << std::endl
      << _G_system.token_stream << " *token_stream;" << std::endl
      << "int yytoken;" << std::endl
      << std::endl
      << "inline token_type LA(std::size_t k = 1) const" << std::endl
      << "{ return token_stream->token(token_stream->index() - 1 + k - 1); }"
      << std::endl
      << "inline int yylex() {" << std::endl
      << "return (yytoken = token_stream->next_token());" << std::endl
      << "}" << std::endl
      << "inline void rewind(std::size_t index) {" << std::endl
      << "token_stream->rewind(index);" << std::endl
      << "yylex();" << std::endl
      << "}" << std::endl
      << std::endl;

  out << "// token stream" << std::endl
      << "void set_token_stream(" << _G_system.token_stream << " *s)" << std::endl
      << "{ token_stream = s; }" << std::endl
      << std::endl;

  out << "// error handling" << std::endl
      << "void yy_expected_symbol(int kind, char const *name);" << std::endl
      << "void yy_expected_token(int kind, std::size_t token, char const *name);" << std::endl
      << std::endl
      << "bool yy_block_errors;" << std::endl
      << "inline bool block_errors(bool block) {" << std::endl
      << "bool previous = yy_block_errors;" << std::endl
      << "yy_block_errors = block;" << std::endl
      << "return previous;" << std::endl
      << "}" << std::endl;

  out << std::endl;

    if (_G_system.generate_ast)
      {
        out << "// memory pool" << std::endl
            << "typedef kdev_pg_memory_pool memory_pool_type;" << std::endl
            << std::endl
            << "kdev_pg_memory_pool *memory_pool;" << std::endl
            << "void set_memory_pool(kdev_pg_memory_pool *p)" << std::endl
            << "{ memory_pool = p; }" << std::endl
            << "template <class T>" << std::endl
            << "inline T *create()" << std::endl
            << "{" << std::endl
            << "T *node = new (memory_pool->allocate(sizeof(T))) T();" << std::endl
            << "node->kind = T::KIND;" << std::endl
            << "return node;" << std::endl
            << "}" << std::endl << std::endl;
      }


  out << "enum token_type_enum" << std::endl << "{" << std::endl;
  std::for_each(_G_system.terminals.begin(), _G_system.terminals.end(), gen_token(out));
  out << "token_type_size" << std::endl
      << "}; // token_type_enum" << std::endl
      << std::endl;


  if (_G_system.parserclass_members.declarations.empty() == false)
    {
      out << "// user defined declarations:" << std::endl;
      std::for_each(_G_system.parserclass_members.declarations.begin(),
                    _G_system.parserclass_members.declarations.end(),
                    gen_member_code(out, settings::member_item::public_declaration
                                         | settings::member_item::protected_declaration
                                         | settings::member_item::private_declaration)
                   );
      out << std::endl << "public:" << std::endl;
    }

  if (_G_system.need_state_management)
    {
      out << "// The copy_current_state() and restore_state() methods are only declared" << std::endl
          << "// if you are using try blocks in your grammar, and have to be" << std::endl
          << "// implemented by yourself, and you also have to define a" << std::endl
          << "// \"struct parser_state\" inside a %parserclass directive." << std::endl
          << std::endl
          << "// This method should create a new parser_state object and return it," << std::endl
          << "// or return 0 if no state variables need to be saved." << std::endl
          << "parser_state *copy_current_state();" << std::endl
          << std::endl
          << "// This method is only called for parser_state objects != 0" << std::endl
          << "// and should restore the parser state given as argument." << std::endl
          << "void restore_state(parser_state *state);" << std::endl;
    }

  out << "parser() {" << std::endl;
  if (_G_system.generate_ast)
    {
      out << "memory_pool = 0;" << std::endl;
    }

  out << "token_stream = 0;" << std::endl
      << "yytoken = Token_EOF;" << std::endl
      << "yy_block_errors = false;" << std::endl;

  if (_G_system.parserclass_members.constructor_code.empty() == false)
    {
      out << std::endl << "// user defined constructor code:" << std::endl;
      std::for_each(_G_system.parserclass_members.constructor_code.begin(),
                    _G_system.parserclass_members.constructor_code.end(),
                    gen_member_code(out, settings::member_item::constructor_code));
    }

  out << "}" << std::endl << std::endl;

  out << "virtual ~parser() {";

  if (_G_system.parserclass_members.destructor_code.empty() == false)
    {
      out << std::endl
          << "// user defined destructor code:" << std::endl;
      std::for_each(_G_system.parserclass_members.destructor_code.begin(),
                    _G_system.parserclass_members.destructor_code.end(),
                    gen_member_code(out, settings::member_item::destructor_code));
    }

  out << "}" << std::endl << std::endl;

  std::for_each(_G_system.symbols.begin(), _G_system.symbols.end(), gen_forward_parser_rule(out));

  out << "};" << std::endl;
}

void generate_parser_bits::operator()()
{
  std::for_each(_G_system.symbols.begin(), _G_system.symbols.end(), gen_parser_rule(out));
}

