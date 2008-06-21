/* This file is part of kdev-cmg
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

#include "kdev-cmg.h"
#include "kdev-cmg-decoder.h"
#include "kdev-cmg-generate.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace kdevcmg;

world _G_system;

static void usage(char const* argv0);
static bool process_file(char const* filename);


void print_token_environment(parser* parser)
{
    static bool done = false;
    if (done)
      return; // don't print with each call when going up the error path

    decoder dec(parser->token_stream);

    std::size_t current_index = parser->token_stream->index() - 1;
    for (std::size_t i = current_index - 5; i < current_index + 5; i++)
      {
        if (i < 0 || i >= parser->token_stream->size())
          continue;

        if (i == current_index)
          std::cerr << ">>";

        std::cerr << dec.decode_id(i); // print out currently processed token

        if (i == current_index)
          std::cerr << "<<";

        std::cerr << " ";
      }
    std::cerr << std::endl;

    done = true;
}


int main(int argc, char *argv[])
{
  if (argc == 1)
    {
      usage(argv[0]);
      exit(EXIT_FAILURE);
    }

  while (char const *arg = *++argv)
    {
      if (!strncmp(arg, "--language=", 11))
        {
          _G_system.language = arg + 11;
        }
      else if (!strncmp(arg, "--", 2))
        {
          std::cerr << "Unknown option: " << arg << std::endl;
          usage(argv[0]);
          exit(EXIT_FAILURE);
        }
      else
        {
          if(!process_file(arg))
            exit(EXIT_FAILURE);
          else
            exit(EXIT_SUCCESS);
        }
    }

  return EXIT_FAILURE;
}

bool process_file(char const *filename)
{
  std::ifstream filestr(filename);

  if (filestr.is_open())
    {
      std::filebuf *pbuf;
      long size;

      // get pointer to associated buffer object
      pbuf=filestr.rdbuf();

      // get file size using buffer's members
      size=pbuf->pubseekoff(0,std::ios::end,std::ios::in);
      pbuf->pubseekpos(0,std::ios::in);

      // allocate memory to contain file data
      _G_system.contents = new char[size+1];

      // get file data
      pbuf->sgetn(_G_system.contents, size);

      _G_system.contents[size] = '\0';

      filestr.close();
    }
  else
    {
      std::cerr << filename << ": file not found" << std::endl;
      return false;
    }

  parser::token_stream_type token_stream;
  parser::memory_pool_type memory_pool;

  // 0) setup
  parser kdevcmg_parser;
  kdevcmg_parser.set_token_stream(&token_stream);
  kdevcmg_parser.set_memory_pool(&memory_pool);

  // 1) tokenize
  kdevcmg_parser.tokenize();

  // 2) parse
  model_ast *ast = 0;
  bool matched = kdevcmg_parser.parse_model(&ast);
  if (matched)
    {
      _G_system.ast = ast;
      generate_output();
    }
  else
    {
      kdevcmg_parser.yy_expected_symbol(ast_node::Kind_model, "model"); // ### remove me
    }

  delete[] _G_system.contents;
  _G_system.contents = 0;

  return matched;
}

static void usage(char const* argv0)
{
  std::cerr << "usage: " << argv0 << " [options]" << " file1.cm" << std::endl
    << std::endl
    << "Options:" << std::endl
    << "  --language=<lang>: Specify the name of the language." << std::endl
    << "                     Examples for <lang>: cpp, csharp, java, ..."
    << std::endl;
}
