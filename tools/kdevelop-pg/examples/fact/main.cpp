
#include "fact_parser.h"
#include "fact_default_visitor.h"
#include "decoder.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace fact;

static void usage(char const* argv0);
static bool parse_file(char const* filename);


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
      /*if (!strncmp(arg, "--option=", 9))
        {
          char const* option = arg + 9;

          std::cerr << "--option=" << option
                    << " has been given!" << std::endl;
        }
      else */
      if (!strncmp(arg, "--", 2))
        {
          std::cerr << "Unknown option: " << arg << std::endl;
          usage(argv[0]);
          exit(EXIT_FAILURE);
        }
      else if(!parse_file(arg))
        {
          exit(EXIT_FAILURE);
        }
    }

  return EXIT_SUCCESS;
}

bool parse_file(char const *filename)
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
  parser fact_parser;
  fact_parser.set_token_stream(&token_stream);
  fact_parser.set_memory_pool(&memory_pool);

  // 1) tokenize
  fact_parser.tokenize(contents);

  // 2) parse
  program_ast *ast = 0;
  bool matched = fact_parser.parse_program(&ast);
  if (matched)
    {
      default_visitor v;
      v.visit_node(ast);
    }
  else
    {
      fact_parser.yy_expected_symbol(ast_node::Kind_program, "program"); // ### remove me
    }

  delete[] contents;

  return matched;
}

static void usage(char const* argv0)
{
  std::cerr << "usage: " << argv0 /*<< " [options]"*/ << " file1.f [file2.f...]"
    << std::endl; /*<< std::endl
    << "Options:" << std::endl
    << "  --option=BLA: Do BLAH while parsing." << std::endl
    << "                BLAH is one of FOO, BAR or KUNG, default is FOO."
    << std::endl;
    */
}
