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

#include "kdev-pg-generate.h"

#include "kdev-pg.h"
#include "kdev-pg-code-gen.h"
#include "kdev-pg-ast-gen.h"
#include "kdev-pg-visitor-gen.h"
#include "kdev-pg-visitor-bits-gen.h"
#include "kdev-pg-default-visitor-gen.h"
#include "kdev-pg-default-visitor-bits-gen.h"
#include "kdev-pg-serialize-visitor-gen.h"
#include "kdev-pg-debug-visitor-gen.h"
#include "kdev-pg-beautifier.h"

#include <iostream>
#include <sstream>
#include <fstream>


void generate_output()
{
  if (_G_system.generate_ast)
  { // generate the ast
    std::stringstream s;

    generate_ast __ast(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#ifndef " << _G_system.language << "_AST_H_INCLUDED" << std::endl
      << "#define " << _G_system.language << "_AST_H_INCLUDED" << std::endl
      << std::endl

      << "#include <kdev-pg-list.h>" << std::endl
      << std::endl;
    if (_G_system.export_macro_header)
      s << "#include <" << _G_system.export_macro_header << ">"
        << std::endl;

    if (_G_system.adapt_to_kdevelop)
      {
        // Replace kdevast.h with something more current,
        // like a new duchain include or whatever is the current trend.
        // s << "#include <kdevast.h>" << std::endl
        //   << std::endl;
      }

    if (_G_system.decl)
      s << _G_system.decl << std::endl;

    s << "namespace " << _G_system.ns << "{" << std::endl
      << std::endl;

    __ast();

    s << std::endl << "} // end of namespace " << _G_system.ns << std::endl
      << std::endl

      << "#endif" << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_ast.h";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }

  { // generate the parser decls
    std::stringstream s;

    generate_parser_decls __decls(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#ifndef " << _G_system.language << "_H_INCLUDED" << std::endl
      << "#define " << _G_system.language << "_H_INCLUDED" << std::endl
      << std::endl;

    if (_G_system.generate_ast)
      {
        s << "#include \"" << _G_system.language << "_ast.h\"" << std::endl
          << "#include <kdev-pg-memory-pool.h>" << std::endl
          << "#include <kdev-pg-allocator.h>" << std::endl;
      }

    if (!strcmp(_G_system.token_stream, "kdev_pg_token_stream"))
      s << "#include <kdev-pg-token-stream.h>" << std::endl;

    s << std::endl;
    if (_G_system.export_macro_header)
      s << "#include <" << _G_system.export_macro_header << ">"
        << std::endl;

    if (_G_system.decl && !_G_system.generate_ast)
      s << _G_system.decl << std::endl;

    s << "namespace " << _G_system.ns << "{" << std::endl
      << std::endl;

    __decls();

    s << std::endl << "} // end of namespace " << _G_system.ns << std::endl
      << std::endl

      << "#endif" << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_parser.h";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }

  if (_G_system.generate_ast)
  { // generate the visitor decls
    std::stringstream s;

    generate_visitor __visitor(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#ifndef " << _G_system.language << "_VISITOR_H_INCLUDED" << std::endl
      << "#define " << _G_system.language << "_VISITOR_H_INCLUDED" << std::endl
      << std::endl

      << "#include \"" << _G_system.language << "_ast.h\"" << std::endl
      << std::endl;
    if (_G_system.export_macro_header)
      s << "#include <" << _G_system.export_macro_header << ">"
        << std::endl;

    s << "namespace " << _G_system.ns << "{" << std::endl
      << std::endl;

    __visitor();

    s << std::endl << "} // end of namespace " << _G_system.ns << std::endl
      << std::endl

      << "#endif" << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_visitor.h";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }

  if (_G_system.generate_ast)
  { // generate the default visitor
    std::stringstream s;

    generate_default_visitor __default_visitor(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#ifndef " << _G_system.language << "_DEFAULT_VISITOR_H_INCLUDED" << std::endl
      << "#define " << _G_system.language << "_DEFAULT_VISITOR_H_INCLUDED" << std::endl
      << std::endl

      << "#include \"" << _G_system.language << "_visitor.h\"" << std::endl
      << std::endl;
    if (_G_system.export_macro_header)
      s << "#include <" << _G_system.export_macro_header << ">"
        << std::endl;

    s << "namespace " << _G_system.ns << "{" << std::endl
      << std::endl;

    __default_visitor();

    s << std::endl << "} // end of namespace " << _G_system.ns << std::endl
      << std::endl

      << "#endif" << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_default_visitor.h";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }

  if (_G_system.gen_serialize_visitor)
  { // generate the serialization visitor
    std::stringstream s;

    generate_serialize_visitor __serialize_visitor(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#ifndef " << _G_system.language << "_SERIALIZATION_H_INCLUDED" << std::endl
      << "#define " << _G_system.language << "_SERIALIZATION_H_INCLUDED" << std::endl
      << std::endl

      << "#include \"" << _G_system.language << "_default_visitor.h\"" << std::endl
      << std::endl;
    if (_G_system.export_macro_header)
      s << "#include <" << _G_system.export_macro_header << ">"
        << std::endl;

    s << "#include <iostream>" << std::endl
      << "#include <fstream>" << std::endl
      << std::endl

      << "namespace " << _G_system.ns << "{" << std::endl
      << std::endl;

    __serialize_visitor();

    s << std::endl << "} // end of namespace " << _G_system.ns << std::endl
      << std::endl;

    s << "#endif" << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_serialize_visitor.h";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }

  if (_G_system.gen_debug_visitor)
  { // generate the debug visitor
    std::stringstream s;

    generate_debug_visitor __debug_visitor(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#ifndef " << _G_system.language << "_DEBUG_VISITOR_H_INCLUDED" << std::endl
      << "#define " << _G_system.language << "_DEBUG_VISITOR_H_INCLUDED" << std::endl
      << std::endl

      << "#include \"" << _G_system.language << "_default_visitor.h\"" << std::endl
      << std::endl;
    if (_G_system.export_macro_header)
      s << "#include <" << _G_system.export_macro_header << ">"
        << std::endl;

    s << "#include <iostream>" << std::endl
      << "#include <fstream>" << std::endl
      << std::endl

      << "namespace " << _G_system.ns << "{" << std::endl
      << std::endl;

    __debug_visitor();

    s << std::endl << "} // end of namespace " << _G_system.ns << std::endl
      << std::endl;

    s << "#endif" << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_debug_visitor.h";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }

  { // generate the parser bits
    std::stringstream s;

    generate_parser_bits __bits(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl;

    s << "#include \"" << _G_system.language << "_parser.h\""
      << std::endl
      << std::endl;

    if (_G_system.bits)
      s << _G_system.bits << std::endl;

    s << "namespace " << _G_system.ns << "{" << std::endl
      << std::endl;

    __bits();

    s << std::endl << "} // end of namespace " << _G_system.ns << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_parser.cpp";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }

  if (_G_system.generate_ast)
  { // generate the visitor bits
    std::stringstream s;

    generate_visitor_bits __visitor_bits(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#include \"" << _G_system.language << "_visitor.h\"" << std::endl
      << std::endl

      << "namespace " << _G_system.ns << "{" << std::endl
      << std::endl;

    __visitor_bits();

    s << std::endl << "} // end of namespace " << _G_system.ns << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_visitor.cpp";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }

  if (_G_system.generate_ast)
  { // generate the default visitor bits
    std::stringstream s;

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#include \"" << _G_system.language << "_default_visitor.h\"" << std::endl
      << std::endl

      << "namespace " << _G_system.ns << "{" << std::endl
      << std::endl;

    std::for_each(_G_system.symbols.begin(), _G_system.symbols.end(),
                  gen_default_visitor_bits_rule(s));

    s << std::endl << "} // end of namespace " << _G_system.ns << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_default_visitor.cpp";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }
}



