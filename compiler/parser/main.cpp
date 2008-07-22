/*
 * main.cpp
 *
 *  Created on: Jul 22, 2008
 *      Author: thomasmoenicke
 */

#include "rphp_parser.h"
#include "phplexer.h"

int main( int argc, char* argv[] )
{
    const UnicodeString content( "print();" );

    rphp::parser parser;
    rphp::Lexer lexer( &parser, content );
}
