/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
   ***** END LICENSE BLOCK *****
*/

#include "rphp/analysis/pSourceFile.h"

#include "rphp/analysis/pParseError.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include <unicode/ustring.h>
#include <unicode/unistr.h>
#include <unicode/schriter.h>
#include <stdio.h>

namespace rphp { 

pSourceFile::pSourceFile(const pSourceFileDesc& file):
    file_(file)
{

    std::ifstream instream(file_.fileName().c_str());
    if (!instream.is_open()) {
        throw pParseError("couldn't open file [" + file_.fileName() + "]");
    }
    instream.unsetf(std::ios::skipws);

    std::istreambuf_iterator<std::string::value_type> startPos(instream.rdbuf());
    std::istreambuf_iterator<std::string::value_type> endPos;

    // note this fuzzy matches on encoding aliases
    if (file_.encoding().is88591OrUTF8()) {

        // basic case: already iso-8859-1 or utf8
        // skip possible UTF8 byte-order mark (BOM)
        if (((unsigned char)*startPos++ == 0xEF) &&
            ((unsigned char)*startPos++ == 0xBB) &&
            ((unsigned char)*startPos++ == 0xBF)) {
            /* noop */
        }
        else {
            // no BOM: start from beginning
            instream.seekg(0);
        }

        contents_.assign(startPos, endPos);

    }
    else {    

        // charset conversion from arbitrary to utf8
        std::string rawBuffer(startPos, endPos);

        // charset conversion we leave to UnicodeString
        // note this "pivots" through a 16 bit UChar, but so does the C ucnv_ interface
        UnicodeString ubuffer(rawBuffer.data(), rawBuffer.length(), file_.encoding().value().c_str());
        if (ubuffer.isBogus()) {
            throw pParseError("could not perform character conversion in file [" + file_.fileName() + "] from codepage [" + file_.encoding().value() + "]");
        }

        ubuffer.toUTF8String(contents_);
        
    }
    
}


} // namespace

