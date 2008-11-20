/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include <unicode/unistr.h>
#include <unicode/schriter.h>
#include <stdio.h>

#include "pSourceFile.h"

namespace rphp { 

pSourceFile::pSourceFile(pFileNameString fName, std::string encoding):
    fileName_(fName),
    encoding_(encoding)
{

    std::ifstream instream(fileName_.c_str());
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << fileName_ << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);

    // TODO: this makes 4 separate buffers during read and conversion. surely there's a faster way.

    // first we get a raw bytewise buffer
    std::string rawBuffer =  std::string(std::istreambuf_iterator<std::string::value_type>(instream.rdbuf()),
                                         std::istreambuf_iterator<std::string::value_type>());

    // charset conversion we leave to UnicodeString
    // note this "pivots" through a 16 bit UChar, but so does the C ucnv_ interface
    UnicodeString ubuffer(rawBuffer.data(), rawBuffer.length(), encoding.c_str());
    if (ubuffer.isBogus()) {
        std::cerr << "Could not perform character conversion in file: " << fileName_ << " from charset: " << encoding << std::endl;
        exit(-1);
    }

    // finally to wchar_t for lexer
    int32_t bsize = ubuffer.countChar32();

    wchar_t* buffer = new wchar_t[bsize];
    int32_t newLength;
    UErrorCode errorCode(U_ZERO_ERROR);
    u_strToWCS(buffer,
                 ubuffer.countChar32(),
                 &newLength,
                 ubuffer.getBuffer(),
                 ubuffer.length(),
                 &errorCode);
    assert(U_SUCCESS(errorCode));

    contents_.assign(buffer, newLength);
    delete buffer;
    
}


} // namespace

