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


pLexer::pLexer(pFilenameString fName): tokens_(), lexer_(tokens_), fileName_(fName) {

    std::ifstream instream(fileName_.c_str());
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << fileName_ << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);
    contents_ =  sourceStringType(std::istreambuf_iterator<sourceStringType::value_type>(instream.rdbuf()),
                                  std::istreambuf_iterator<sourceStringType::value_type>());

    sourceBegin_ = contents_.begin();
    sourceEnd_ = contents_.end();
    
}

bool pLexer::preprocess(void) {

    pPreprocessTokens preTokens;
    pPreprocessLexer preLexer(preTokens);
    bool rewrote = false;

    sourceStringType buffer;
    buffer.reserve(contents_.capacity());

    /*

        The idea of the preprocessor is to prepare the underlying source buffer
        for processing by the main language parser. This mostly means converting
        interpolated double quoted strings to their non-interpolated single quoted
        string counterparts, by using concatenation and replacing escape
        sequences with their literal equivalents
        
    */
    pUInt curID = 0;
    for (tokIteratorType iter = tokBegin(); iter != tokEnd(); ++iter) {

        curID = (*iter).id();
        if (curID == 0) {
            // lex error. stop preprocessor and let the main parser
            // show a nice error
            sourceBegin_ = contents_.begin();
            sourceEnd_ = contents_.end();
            return false;
        }
        else if (curID == T_OPEN_TAG) {
            // go to php
            iter.set_state(1);
            buffer.append((*iter).value().begin(), (*iter).value().end());
        }
        else if (curID == T_CLOSE_TAG) {
            // go to html
            iter.set_state(0);
            buffer.append((*iter).value().begin(), (*iter).value().end());
        }
        else if (curID == T_DQ_STRING) {

            rewrote = true;

            // we now use the preprocessor lexer to find the tokens within
            // this double quoted string
            sourceIteratorType dqStart((*iter).value().begin()),
                               dqEnd((*iter).value().end());
            for (tokIteratorType dqIter = preLexer.begin(dqStart, dqEnd);
                 dqIter != preLexer.end();
                 ++dqIter)
            {
                // iterate over DQ tokens
                switch((*dqIter).id()) {
                    case T_DQ_DONE:
                        goto endOfDQ; // omg!
                    case T_DQ_DQ:
                        // replace with single
                        buffer.append("\'");
                        break;
                    case T_DQ_NEWLINE:
                        // replace newline escape sequence with literal newline
                        buffer.append(1, RPHP_NEWLINE);
                        break;
                    case T_DQ_PASSTHROUGH:
                    default:
                        // passthrough
                        buffer.append((*dqIter).value().begin(), (*dqIter).value().end());
                        break;
                }
            
            }

            endOfDQ:
            ;
            // exit processing DQ string

        }
        else if (curID == T_SINGLELINE_COMMENT || curID == T_MULTILINE_COMMENT) {
            // strip
            rewrote = true;
        }
        else {
            // pass through
            buffer.append((*iter).value().begin(), (*iter).value().end());
        }
    
    }

    // save changes
    if (rewrote) {
        contents_.swap(buffer);
        sourceBegin_ = contents_.begin();
        sourceEnd_ = contents_.end();
    }

    return true;
    
}

tokIteratorType pLexer::tokBegin(void) {
    return lexer_.begin(sourceBegin_, sourceEnd_);
}

tokIteratorType pLexer::tokEnd(void) {
    return lexer_.end();
}

const sourceIteratorType pLexer::sourceBegin(void) const {
    return sourceBegin_;
}

const sourceIteratorType pLexer::sourceEnd(void) const {
    return sourceEnd_;
}

void pLexer::dumpTokens(void) {

    // TODO: unicode
    std::string tokID;
    std::stringstream val;

    for (tokIteratorType iter = tokBegin(); iter != tokEnd(); ++iter)
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

// note, these are designed to line up with zend token names
const char* pLexer::getTokenDescription(const std::size_t t) const {

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
        case T_DNUMBER:
            return "T_DNUMBER";
        case T_IF:
            return "T_IF";
        case T_ELSE:
            return "T_ELSE";
        case T_WHILE:
            return "T_WHILE";
        case T_IDENTIFIER:
            return "T_STRING";
        case T_DQ_STRING:
        case T_SQ_STRING:
            return "T_CONSTANT_ENCAPSED_STRING";
        case T_SINGLELINE_COMMENT:
        case T_MULTILINE_COMMENT:
            return "T_COMMENT";
    }
    return "";

}


} } // namespace

