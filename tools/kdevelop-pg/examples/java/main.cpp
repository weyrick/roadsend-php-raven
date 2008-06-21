
#include "java_parser.h"
#include "java_default_visitor.h"
#include "decoder.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace java;

static void usage(char const* argv0);
static bool parse_file(char const* filename, parser::java_compatibility_mode compatibility_mode);


void print_token_environment(java::parser* parser)
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
  parser::java_compatibility_mode compatibility_mode = parser::java15_compatibility;

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

          if (!strcmp("1.5", version)) {
            compatibility_mode = parser::java15_compatibility;
          }
          else if (!strcmp("1.4", version)) {
            compatibility_mode = parser::java14_compatibility;
          }
          else if (!strcmp("1.3", version)) {
            compatibility_mode = parser::java13_compatibility;
          }
          else {
            std::cerr << "Unsupported Java version: " << version << std::endl;
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

bool parse_file(char const *filename, parser::java_compatibility_mode compatibility_mode)
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
  parser java_parser;
  java_parser.set_compatibility_mode(compatibility_mode);
  java_parser.set_token_stream(&token_stream);
  java_parser.set_memory_pool(&memory_pool);

  // 1) tokenize
  java_parser.tokenize(contents);

  // 2) parse
  compilation_unit_ast *ast = 0;
  bool matched = java_parser.parse_compilation_unit(&ast);
  if (matched)
    {
      default_visitor v;
      v.visit_node(ast);
    }
  else
    {
      java_parser.yy_expected_symbol(ast_node::Kind_compilation_unit, "compilation_unit"); // ### remove me
    }

  delete[] contents;

  return matched;
}

static void usage(char const* argv0)
{
  std::cerr << "usage: " << argv0 << " [options] file.java [file2.java ...]"
     << std::endl << std::endl
     << "Options:" << std::endl
     << "  --compatibility=VERSION: Accept files of the given Java version. " << std::endl
     << "                           VERSION is one of 1.3, 1.4 or 1.5, "
     <<                            "default is 1.5." << std::endl;
}
