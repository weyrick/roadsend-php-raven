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

#include "rphp/analysis/pLexer.h"

namespace rphp { namespace lexer {

pLexer::pLexer(const pSourceFile* s):
    tokens_(),
    lexer_(tokens_),
    source_(s),
    // TODO: does this copy, or do copy on write? source_ is const
    contents_(source_->contents()),
    sourceBegin_(contents_.begin()),
    sourceEnd_ (contents_.end())
{
    
}

bool pLexer::preprocess(void) {

    pPreprocessTokens preTokens;
    pPreprocessLexer preLexer(preTokens);
    bool rewrote = false;
    bool success = true;

    pSourceString buffer;
    buffer.reserve(contents_.capacity());

    /*

        The idea of the preprocessor is to prepare the underlying source buffer
        for processing by the main language parser. This mostly means converting
        interpolated double quoted strings to their non-interpolated single quoted
        string counterparts, by using concatenation and replacing escape
        sequences with their literal equivalents
        
    */
    pUInt curID = 0;
    for (pTokenIterator iter = tokBegin(); iter != tokEnd(); ++iter) {

        curID = (*iter).id();
        if (curID == 0) {
            // lex error. stop preprocessor and let the main parser
            // show a nice error
            success = false;
            rewrote = true;
            break;
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
            pSourceCharIterator dqStart((*iter).value().begin()),
                                dqEnd((*iter).value().end());
            for (pTokenIterator dqIter = preLexer.begin(dqStart, dqEnd);
                                dqIter != preLexer.end();
                                ++dqIter)
            {
                // iterate over DQ tokens
                switch((*dqIter).id()) {
                    case T_DQ_DONE:
                        goto endOfDQ; // omg!
                    case T_DQ_DQ:
                        // replace with single
                        buffer.push_back('\'');
                        break;
                    case T_DQ_NEWLINE:
                        // replace newline escape sequence with literal newline
                        buffer.push_back(RPHP_NEWLINE);
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
        else if (curID == T_SINGLELINE_COMMENT ||
                 curID == T_MULTILINE_COMMENT ||
                 curID == T_DOC_COMMENT
                 ) {
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
    }
    
    sourceBegin_ = contents_.begin();
    sourceEnd_ = contents_.end();

    return success;
    
}

pTokenIterator pLexer::tokBegin(void) {
    return lexer_.begin(sourceBegin_, sourceEnd_);
}

pTokenIterator pLexer::tokEnd(void) {
    return lexer_.end();
}

const pSourceCharIterator pLexer::sourceBegin(void) const {
    return sourceBegin_;
}

const pSourceCharIterator pLexer::sourceEnd(void) const {
    return sourceEnd_;
}

void pLexer::dumpTokens(void) {

    std::wstring tokID;
    std::wstringstream val;

    for (pTokenIterator iter = tokBegin(); iter != tokEnd(); ++iter)
    {
        if ((*iter).id() == 0) {
            // no match
            break;
        }
        else {
            // matched
            // skip plain newlines in html state
            val.str(L"");
            if ((*iter).id() != T_WHITESPACE)
                val << (*iter).value();
            if (((*iter).state() == 0) && (val.str() == L"\n"))
                continue;
            tokID = getTokenDescription((*iter).id());
            if (tokID.size() == 0)
                tokID = val.str();
            std::wcout << val.str() << L" " << tokID << std::endl;

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
const wchar_t* pLexer::getTokenDescription(const std::size_t t) const {

    switch (t) {
        case T_VARIABLE:
            return L"T_VARIABLE";
        case T_WHITESPACE:
            return L"T_WHITESPACE";
        case T_INLINE_HTML:
            return L"T_INLINE_HTML";
        case T_ECHO:
            return L"T_ECHO";
        case T_ARROWKEY:
            return L"T_DOUBLE_ARROW";
        case T_OPEN_TAG:
            return L"T_OPEN_TAG";
        case T_CLOSE_TAG:
            return L"T_CLOSE_TAG";
        case T_LNUMBER:
            return L"T_LNUMBER";
        case T_DNUMBER:
            return L"T_DNUMBER";
        case T_IF:
            return L"T_IF";
        case T_ELSE:
            return L"T_ELSE";
        case T_ELSEIF:
            return L"T_ELSEIF";
        case T_WHILE:
            return L"T_WHILE";
        case T_NEW:
            return L"T_NEW";
        case T_ARRAY:
            return L"T_ARRAY";
        case T_IDENTIFIER:
            return L"T_STRING";
        case T_DQ_STRING:
        case T_SQ_STRING:
            return L"T_CONSTANT_ENCAPSED_STRING";
        case T_SINGLELINE_COMMENT:
        case T_MULTILINE_COMMENT:
            return L"T_COMMENT";
        case T_DOC_COMMENT:
            return L"T_DOC_COMMENT";
        case T_GLOBAL:
            return L"T_GLOBAL";
        case T_FUNCTION:
            return L"T_FUNCTION";
        case T_EMPTY:
            return L"T_EMPTY";
        case T_BOOLEAN_AND:
            return L"T_BOOLEAN_AND";
        case T_BOOLEAN_OR:
            return L"T_BOOLEAN_OR";
        case T_EQUAL:
            return L"T_IS_EQUAL";
        case T_ISSET:
            return L"T_ISSET";
        case T_UNSET:
            return L"T_UNSET";
        case T_VAR:
            return L"T_VAR";
        case T_CLASS:
            return L"T_CLASS";
        case T_CLASSDEREF:
            return L"T_OBJECT_OPERATOR";
        case T_FOREACH:
            return L"T_FOREACH";
        case T_AS:
            return L"T_AS";
        case T_RETURN:
            return L"T_RETURN";
        case T_DOTEQUAL:
            return L"T_CONCAT_EQUAL";
        case T_LIST:
            return L"T_LIST";
        case T_EXTENDS:
            return L"T_EXTENDS";
        case T_PUBLIC:
            return L"T_PUBLIC";
        case T_PRIVATE:
            return L"T_PRIVATE";
        case T_PROTECTED:
            return L"T_PROTECTED";
        case T_NOTEQUAL:
            return L"T_IS_NOT_EQUAL";
        case T_INCLUDE:
            return L"T_INCLUDE";
        case T_INCLUDE_ONCE:
            return L"T_INCLUDE_ONCE";
        case T_REQUIRE:
            return L"T_REQUIRE";
        case T_REQUIRE_ONCE:
            return L"T_REQUIRE_ONCE";
        case T_IDENTICAL:
            return L"T_IS_IDENTICAL";
    }
    return L"";

}


} } // namespace

