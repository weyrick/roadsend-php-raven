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

#include "kdev-cmg-generate.h"

#include "kdev-cmg.h"
#include "kdev-cmg-beautifier.h"
#include "kdev-cmg-initialize-ast.h"
#include "kdev-cmg-forward-declarations-gen.h"
#include "kdev-cmg-declarations-gen.h"
#include "kdev-cmg-implementation-gen.h"
#include "kdev-cmg-chameleon-gen.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace kdevcmg;


void generate_output()
{
  if (_G_system.language.empty())
    {
      std::cout << "In order to get the codemodel generated," << std::endl
                << "please specify the --language=blah option." << std::endl;
      return;
    }

  { // fill in missing AST members
    initialize_ast __init;
    __init();
  }

  { // generate the forward declarations
    std::stringstream s;

    generate_forward_declarations __forward_decls(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#ifndef " << _G_system.language << "_CODEMODEL_FWD_H" << std::endl
      << "#define " << _G_system.language << "_CODEMODEL_FWD_H" << std::endl
      << std::endl

      << "#include \"kdevsharedptr.h\"" << std::endl
      << std::endl
      << "#include <QtCore/QList>" << std::endl
      << std::endl;

    s << "namespace " << _G_system.language << "{" << std::endl
      << std::endl;

    __forward_decls();

    s << std::endl << "} // end of namespace " << _G_system.language << std::endl
      << std::endl;

    s << "#endif" << std::endl << std::endl;

    std::string oname = _G_system.language;
    oname += "_codemodel_fwd.h";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }


  { // generate the declarations
    std::stringstream s;

    generate_declarations __decls(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#ifndef " << _G_system.language << "_CODEMODEL_H" << std::endl
      << "#define " << _G_system.language << "_CODEMODEL_H" << std::endl
      << std::endl

      << "#include \"kdevcodemodel.h\"" << std::endl
      << std::endl
      << "#include \"" << _G_system.language << "_codemodel_fwd.h\"" << std::endl
      << std::endl
      << "#include <QtCore/QTime>" << std::endl
      << "#include <QtCore/QHash>" << std::endl
      << "#include <QtCore/QList>" << std::endl
      << std::endl
      << "#include <ktexteditor/cursor.h>" << std::endl
      << std::endl;

    s << _G_system.declarations << std::endl;

    s << "namespace " << _G_system.language << "{" << std::endl
      << std::endl;

    __decls();

    s << std::endl << "} // end of namespace " << _G_system.language << std::endl
      << std::endl;

    s << "#endif" << std::endl << std::endl;

    std::string oname = _G_system.language;
    oname += "_codemodel.h";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }


  { // generate the chameleon declarations
    std::stringstream s;

    generate_chameleon __chameleon_decls(s, generate_chameleon::declaration);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#ifndef " << _G_system.language << "_CODEMODEL_CHAMELEON_H" << std::endl
      << "#define " << _G_system.language << "_CODEMODEL_CHAMELEON_H" << std::endl
      << std::endl

      << "#include \"" << _G_system.language << "_codemodel.h\"" << std::endl
      << std::endl;

    s << "namespace " << _G_system.language << "{" << std::endl
      << std::endl;

    __chameleon_decls();

    s << std::endl << "} // end of namespace " << _G_system.language << std::endl
      << std::endl;

    s << "#endif" << std::endl << std::endl;

    std::string oname = _G_system.language;
    oname += "_codemodel_chameleon.h";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }


  { // generate the implementation
    std::stringstream s;

    generate_implementation __implementation(s);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#include \"" << _G_system.language << "_codemodel.h\"" << std::endl
      << std::endl;

    s << "namespace " << _G_system.language << "{" << std::endl
      << std::endl;

    s << _G_system.implementation << std::endl;

    __implementation();

    s << std::endl << "} // end of namespace " << _G_system.language << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_codemodel.cpp";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }


  { // generate the chameleon implementation
    std::stringstream s;

    generate_chameleon __chameleon_implementation(s, generate_chameleon::implementation);

    s << "// THIS FILE IS GENERATED" << std::endl
      << "// WARNING! All changes made in this file will be lost!" << std::endl
      << std::endl

      << "#include \"" << _G_system.language << "_codemodel_chameleon.h\"" << std::endl
      << std::endl;

    s << "namespace " << _G_system.language << "{" << std::endl
      << std::endl;

    s << _G_system.implementation << std::endl;

    __chameleon_implementation();

    s << std::endl << "} // end of namespace " << _G_system.language << std::endl
      << std::endl;

    std::string oname = _G_system.language;
    oname += "_codemodel_chameleon.cpp";

    std::ofstream ofile;
    ofile.open(oname.c_str(), std::ios::out);
    format(s, ofile);
  }
}

