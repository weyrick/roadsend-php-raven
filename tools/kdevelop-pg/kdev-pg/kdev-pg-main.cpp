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
#include "kdev-pg-ast.h"
#include "kdev-pg-visitor.h"
#include "kdev-pg-default-visitor.h"
#include "kdev-pg-pretty-printer.h"
#include "kdev-pg-environment.h"
#include "kdev-pg-first.h"
#include "kdev-pg-follow.h"
#include "kdev-pg-checker.h"
#include "kdev-pg-generate.h"

#include <iterator>
#include <algorithm>
#include <memory>

#include <iostream>
#include <fstream>

#include <cassert>
#include <cstdio>
#include <cstdlib>

int yyparse();

void usage()
{
  std::cerr << "usage: kdev-pg --output=<name> file.g" << std::endl;
  exit(EXIT_SUCCESS);
}

struct debug_rule
{
  std::ostream &out;

  debug_rule(std::ostream &o): out(o)
  {}

  void operator()(model::node *node)
  {
    model::evolve_item *e = node_cast<model::evolve_item*>(node);

    out << std::endl;
    pretty_printer p(out);
    p(e);

    bool initial = true;
    world::node_set::iterator it;

    out << std::endl;
    out << " FIRST:[";
    for (it = _G_system.FIRST(e).begin(); it != _G_system.FIRST(e).end(); it++)
      {
        if (!initial)
          out << ",";

        p(*it);
        initial = false;
      }
    out << "]";

    initial = true;

    out << std::endl;
    out << " FOLLOW:[";
    for (it = _G_system.FOLLOW(e->_M_symbol).begin();
         it != _G_system.FOLLOW(e->_M_symbol).end(); it++)
      {
        if (!initial)
          out << ",";

        p(*it);
        initial = false;
      }
    out << "]";
    out << std::endl;
  }
};

int main(int, char *argv[])
{
  bool dump_terminals = false;
  bool dump_symbols = false;
  bool debug_rules = false;

  file = stdin;

  while (char const *arg = *++argv)
    {
      if (!strncmp(arg, "--output=", 9))
        {
          _G_system.language = arg + 9;
        }
      else if (!strncmp(arg,"--namespace=",12))
        {
          _G_system.ns = arg + 12;
	}
      else if (!strcmp("--no-ast", arg))
        {
          _G_system.generate_ast = false;
        }
      else if (!strcmp("--serialize-visitor", arg))
        {
          _G_system.gen_serialize_visitor = true;
        }
      else if (!strcmp("--debug-visitor", arg))
        {
          _G_system.gen_debug_visitor = true;
        }
      else if (!strcmp("--help", arg))
        {
          usage();
        }
      else if (!strcmp("--terminals", arg))
        {
          dump_terminals = true;
        }
      else if (!strcmp("--symbols", arg))
        {
          dump_symbols = true;
        }
      else if (!strcmp("--rules", arg))
        {
          debug_rules = true;
        }
      else if (!strncmp(arg, "--adapt-to=", 11))
        {
          char const *adapt_to = arg + 11;
          _G_system.adapt_to_kdevelop = (strcmp("kdevelop", adapt_to) == 0);
        }
      else if (file == stdin)
        {
          file = fopen(arg, "r");

          if (!file)
            {
              std::cerr << "kdev-pg: file ``" << arg
                        << "'' not found!" << std::endl;
              file = stdin;
            }
        }
      else
        {
          std::cerr << "kdev-pg: unknown option ``" << arg << "''"
                    << std::endl;
        }
    }

  if( !_G_system.ns )
    _G_system.ns = _G_system.language;

  yyparse();

  fclose(file);

  std::for_each(_G_system.rules.begin(), _G_system.rules.end(),
                initialize_environment());

  compute_FIRST();
  compute_FOLLOW();

  std::for_each(_G_system.rules.begin(), _G_system.rules.end(),
                FIRST_FOLLOW_conflict_checker());

  std::for_each(_G_system.rules.begin(), _G_system.rules.end(),
                FIRST_FIRST_conflict_checker());

  std::for_each(_G_system.rules.begin(), _G_system.rules.end(),
                empty_FIRST_checker());

  std::for_each(_G_system.rules.begin(), _G_system.rules.end(),
                undefined_symbol_checker());

  std::for_each(_G_system.rules.begin(), _G_system.rules.end(),
                undefined_token_checker());

  problem_summary_printer()();

  if (dump_terminals || dump_symbols || debug_rules)
    {
      if(dump_terminals)
        {
          std::ofstream ft("kdev-pg-terminals", std::ios_base::out | std::ios_base::trunc );
          for (world::terminal_set::iterator it = _G_system.terminals.begin();
               it != _G_system.terminals.end(); ++it)
            {
              ft << (*it).first << std::endl;
            }
        }
      if (dump_symbols)
        { 
          std::ofstream st("kdev-pg-symbols", std::ios_base::out | std::ios_base::trunc );
          for (world::symbol_set::iterator it = _G_system.symbols.begin();
               it != _G_system.symbols.end(); ++it)
            {
              st << (*it).first << std::endl;
            }
        }
      if (debug_rules)
        {
          std::ofstream rt("kdev-pg-rules", std::ios_base::out | std::ios_base::trunc );
          std::for_each(_G_system.rules.begin(), _G_system.rules.end(),
                        debug_rule(rt));
        }
    }
  else if (!_G_system.language)
    {
      usage();
    }

  generate_output();

  return EXIT_SUCCESS;
}

