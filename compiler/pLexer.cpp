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

#include "pLexer.h"

namespace rphp { namespace lexer {


pLexer::pLexer(std::string fName): tokens(), lexer(tokens), fileName(fName) {

    std::ifstream instream(fileName.c_str());
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << fileName << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);
    contents =  std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
                            std::istreambuf_iterator<char>());

    sourceBegin = contents.begin();
    sourceEnd = contents.end();
    
}

tokIteratorType pLexer::begin(void) {

    return lexer.begin(sourceBegin, sourceEnd);

}

tokIteratorType pLexer::end(void) {

    return lexer.end();

}


void pLexer::dumpTokens(void) {

    std::string tokID;
    std::stringstream val;

    for (tokIteratorType iter = begin(); iter != end(); ++iter)
    {
        if ((*iter).id() == 0) {
            // no match
            break;
        }
        else {
            // matched
            // skip plain newlines in html state
            val.str("");
            if ((*iter).id() != T_WHITESPACE)
                val << (*iter).value();
            if (((*iter).state() == 0) && (val.str() == "\n"))
                continue;
            tokID = getTokenDescription((*iter).id());
            if (tokID.size() == 0)
                tokID = val.str();
            std::cout << val.str() << " " << tokID << std::endl;

            if ((*iter).id() == T_OPEN_TAG) {
                // go to php
                iter.set_state(1);
            }
            else if ((*iter).id() == T_CLOSE_TAG) {
                // go to html
                iter.set_state(0);
            }
        }
    }


}

const char* pLexer::getTokenDescription(const std::size_t t) {

    switch (t) {
        case T_VARIABLE:
            return "T_VARIABLE";
        case T_WHITESPACE:
            return "T_WHITESPACE";
        case T_INLINE_HTML:
            return "T_INLINE_HTML";
        case T_ECHO:
            return "T_ECHO";
        case T_OPEN_TAG:
            return "T_OPEN_TAG";
        case T_CLOSE_TAG:
            return "T_CLOSE_TAG";
        case T_LNUMBER:
            return "T_LNUMBER";
        case T_IF:
            return "T_IF";
        case T_ELSE:
            return "T_ELSE";
        case T_WHILE:
            return "T_WHILE";
        case T_IDENTIFIER:
            return "T_STRING";
        case T_CONSTANT_ENCAPSED_STRING:
            return "T_CONSTANT_ENCAPSED_STRING";
    }
    return "";

}


} } // namespace

