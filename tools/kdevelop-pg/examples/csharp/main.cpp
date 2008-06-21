
#include "csharp_parser.h"
#include "csharp_default_visitor.h"
#include "decoder.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace csharp;

static void usage(char const* argv0);
static bool parse_file(char const* filename, parser::csharp_compatibility_mode compatibility_mode);


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

        std::cerr << dec.decode_string(i); // print out currently processed token

        if (i == current_index)
          std::cerr << "<<";

        std::cerr << " ";
      }
    std::cerr << std::endl;

    done = true;
}


int main(int argc, char *argv[])
{
  parser::csharp_compatibility_mode compatibility_mode = parser::csharp20_compatibility;

  if (argc == 1)
    {
      usage(argv[0]);
      exit(EXIT_FAILURE);
    }
  while (char const *arg = *++argv)
    {
      if (!strncmp(arg, "--compatibility=", 16))
        {
          char const* version = arg + 16;

          if (!strcmp("1.0", version)) {
            compatibility_mode = parser::csharp10_compatibility;
          }
          else if (!strcmp("2.0", version)) {
            compatibility_mode = parser::csharp20_compatibility;
          }
          else {
            std::cerr << "Unsupported C# version: " << version << std::endl;
            usage(argv[0]);
            exit(EXIT_FAILURE);
          }
        }
      else if (!strncmp(arg, "--", 2))
        {
          std::cerr << "Unknown option: " << arg << std::endl;
          usage(argv[0]);
          exit(EXIT_FAILURE);
        }
      else if(!parse_file(arg, compatibility_mode))
        {
          exit(EXIT_FAILURE);
        }
    }

  return EXIT_SUCCESS;
}

bool parse_file(char const *filename, parser::csharp_compatibility_mode compatibility_mode)
{
  char *contents;
  std::ifstream filestr(filename);

  if (filestr.is_open())
    {
      std::filebuf *pbuf;
      long size;

      // get pointer to associated buffer object
      pbuf = filestr.rdbuf();

      // get file size using buffer's members
      size = pbuf->pubseekoff(0,std::ios::end,std::ios::in);
      pbuf->pubseekpos(0,std::ios::in);

      // allocate memory to contain file data
      contents = new char[size+1];

      // get file data
      pbuf->sgetn(contents, size);

      contents[size] = '\0';

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
  parser csharp_parser;
  csharp_parser.set_compatibility_mode(compatibility_mode);
  csharp_parser.set_token_stream(&token_stream);
  csharp_parser.set_memory_pool(&memory_pool);

  // 1) tokenize
  csharp_parser.tokenize(contents);

  // 2) parse
  compilation_unit_ast *ast = 0;
  bool matched = csharp_parser.parse_compilation_unit(&ast);
  if (matched)
    {
      default_visitor v;
      v.visit_node(ast);
    }
  else
    {
      csharp_parser.yy_expected_symbol(ast_node::Kind_compilation_unit, "compilation_unit"); // ### remove me
    }

  delete[] contents;

  return matched;
}

static void usage(char const* argv0)
{
  std::cerr << "usage: " << argv0 << " [options] file.cs [file2.cs ...]"
     << std::endl << std::endl
     << "Options:" << std::endl
     << "  --compatibility=VERSION: Accept files of the given C# version. " << std::endl
     << "                           VERSION is one of 1.0 or 2.0, "
     <<                            "default is 2.0." << std::endl;
}
