/* This file is part of kdev-cmg
   Copyright (C) 2005 Roberto Raggi <roberto@kdevelop.org>

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

#include "kdev-cmg-beautifier.h"
#include "astyle.h"

void format(std::istream &in, std::ostream &out)
{
  astyle::ASFormatter f;

  f.sourceStyle = astyle::STYLE_C;

  f.blockIndent = true;
  f.bracketIndent = false;
  f.indentLength = 2;
  f.indentString = "  ";
  f.minConditionalIndent = f.indentLength * 2;
  f.bracketFormatMode = astyle::BREAK_MODE;
  f.classIndent = false;
  f.switchIndent = false;
  f.namespaceIndent = false;
  f.breakElseIfs = false;
  f.padParen = false;
  f.emptyLineIndent = false;
  f.init(in);

  do
    {
      out << f.nextLine() << std::endl;
    }
  while (f.hasMoreLines());
}
