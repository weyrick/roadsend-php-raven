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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include "rphp/analysis/pLexer.h"

namespace rphp { namespace lexer {

pLexer::pLexer(const pSourceFile* s):
    langRules_((boost::lexer::regex_flags)(boost::lexer::icase | boost::lexer::dot_not_newline)),
    langState_(),
    lexInput_(NULL),
    dqRules_(),
    dqState_(),
    source_(s),
    // TODO: does this copy, or do copy on write? source_ is const
    contents_(source_->contents()),
    sourceBegin_(contents_.begin()),
    sourceEnd_ (contents_.end())
{

    dqRules_.add(L"INITIAL", L"\\\\n", T_DQ_NEWLINE, L".");
    dqRules_.add(L"INITIAL", L"\\\"", T_DQ_DQ, L".");
    dqRules_.add(L"INITIAL", L"\\\\\\\"", T_DQ_ESCAPE, L".");
    boost::lexer::wgenerator::build (dqRules_, dqState_);

    langRules_.add_state(L"PHP");

    langRules_.add(L"INITIAL", L"<\\?|<\\?PHP", T_OPEN_TAG, L"PHP"); // go to PHP state
    langRules_.add(L"INITIAL", L".+|\\n+", T_INLINE_HTML, L".");

    langRules_.add(L"PHP", L"\\?>", T_CLOSE_TAG, L"INITIAL"); // go to HTML state

    langRules_.add(L"PHP", L"\\(", T_LEFTPAREN, L".");
    langRules_.add(L"PHP", L"\\)", T_RIGHTPAREN, L".");
    langRules_.add(L"PHP", L"\\{", T_LEFTCURLY, L".");
    langRules_.add(L"PHP", L"\\}", T_RIGHTCURLY, L".");
    langRules_.add(L"PHP", L"\\[", T_LEFTSQUARE, L".");
    langRules_.add(L"PHP", L"\\]", T_RIGHTSQUARE, L".");
    langRules_.add(L"PHP", L"\\>", T_GREATER_THAN, L".");
    langRules_.add(L"PHP", L"\\<", T_LESS_THAN, L".");
    langRules_.add(L"PHP", L"\\=", T_ASSIGN, L".");
    langRules_.add(L"PHP", L"\\-", T_MINUS, L".");
    langRules_.add(L"PHP", L"\\+", T_PLUS, L".");
    langRules_.add(L"PHP", L"\\/", T_DIV, L".");
    langRules_.add(L"PHP", L"\\%", T_MOD, L".");
    langRules_.add(L"PHP", L"\\*", T_MULT, L".");
    langRules_.add(L"PHP", L"\\;", T_SEMI, L".");
    langRules_.add(L"PHP", L"\\,", T_COMMA, L".");
    langRules_.add(L"PHP", L"\\.", T_DOT, L".");
    langRules_.add(L"PHP", L"\\!", T_NOT, L".");
    langRules_.add(L"PHP", L"\\&", T_REF, L".");
    langRules_.add(L"PHP", L"\\@", T_AT, L".");
    langRules_.add(L"PHP", L"\\?", T_QUESTION, L".");
    langRules_.add(L"PHP", L"\\:", T_COLON, L".");
    langRules_.add(L"PHP", L">=", T_GREATER_THAN_OR_EQUAL, L".");
    langRules_.add(L"PHP", L"<=", T_LESS_THAN_OR_EQUAL, L".");
    langRules_.add(L"PHP", L"::", T_DBL_COLON, L".");
    langRules_.add(L"PHP", L"\\+\\+", T_INC, L".");
    langRules_.add(L"PHP", L"\\-\\-", T_DEC, L".");
    langRules_.add(L"PHP", L"\\+\\=", T_PLUS_EQUAL, L".");
    langRules_.add(L"PHP", L"\\-\\=", T_MINUS_EQUAL, L".");
    langRules_.add(L"PHP", L"\\!\\=", T_NOTEQUAL, L".");
    langRules_.add(L"PHP", L"\\.\\=", T_DOTEQUAL, L".");
    langRules_.add(L"PHP", L"==", T_EQUAL, L".");
    langRules_.add(L"PHP", L"\\!==", T_NOT_IDENTICAL, L".");
    langRules_.add(L"PHP", L"===", T_IDENTICAL, L".");
    langRules_.add(L"PHP", L"\\&\\&", T_BOOLEAN_AND, L".");
    langRules_.add(L"PHP", L"\\|\\|", T_BOOLEAN_OR, L".");
    langRules_.add(L"PHP", L"\\?>", T_CLOSE_TAG, L".");
    langRules_.add(L"PHP", L"=>", T_ARROWKEY, L".");
    langRules_.add(L"PHP", L"->", T_CLASSDEREF, L".");
    langRules_.add(L"PHP", L"list", T_LIST, L".");
    langRules_.add(L"PHP", L"or", T_LOGICAL_OR, L".");
    langRules_.add(L"PHP", L"and", T_LOGICAL_AND, L".");
    langRules_.add(L"PHP", L"xor", T_LOGICAL_XOR, L".");
    langRules_.add(L"PHP", L"if", T_IF, L".");
    langRules_.add(L"PHP", L"for", T_FOR, L".");
    langRules_.add(L"PHP", L"foreach", T_FOREACH, L".");
    langRules_.add(L"PHP", L"as", T_AS, L".");
    langRules_.add(L"PHP", L"exit", T_EXIT, L".");
    langRules_.add(L"PHP", L"public", T_PUBLIC, L".");
    langRules_.add(L"PHP", L"private", T_PRIVATE, L".");
    langRules_.add(L"PHP", L"protected", T_PROTECTED, L".");
    langRules_.add(L"PHP", L"extends", T_EXTENDS, L".");
    langRules_.add(L"PHP", L"return", T_RETURN, L".");
    langRules_.add(L"PHP", L"global", T_GLOBAL, L".");
    langRules_.add(L"PHP", L"function", T_FUNCTION, L".");
    langRules_.add(L"PHP", L"isset", T_ISSET, L".");
    langRules_.add(L"PHP", L"unset", T_UNSET, L".");
    langRules_.add(L"PHP", L"empty", T_EMPTY, L".");
    langRules_.add(L"PHP", L"array", T_ARRAY, L".");
    langRules_.add(L"PHP", L"while", T_WHILE, L".");
    langRules_.add(L"PHP", L"else", T_ELSE, L".");
    langRules_.add(L"PHP", L"elseif", T_ELSEIF, L".");
    langRules_.add(L"PHP", L"echo", T_ECHO, L".");
    langRules_.add(L"PHP", L"new", T_NEW, L".");
    langRules_.add(L"PHP", L"var", T_VAR, L".");
    langRules_.add(L"PHP", L"switch", T_SWITCH, L".");
    langRules_.add(L"PHP", L"case", T_CASE, L".");
    langRules_.add(L"PHP", L"break", T_BREAK, L".");
    langRules_.add(L"PHP", L"continue", T_CONTINUE, L".");
    langRules_.add(L"PHP", L"default", T_DEFAULT, L".");
    langRules_.add(L"PHP", L"instanceof", T_INSTANCEOF, L".");
    langRules_.add(L"PHP", L"class", T_CLASS, L".");
    langRules_.add(L"PHP", L"clone", T_CLONE, L".");
    langRules_.add(L"PHP", L"throw", T_THROW, L".");
    langRules_.add(L"PHP", L"try", T_TRY, L".");
    langRules_.add(L"PHP", L"catch", T_CATCH, L".");
    langRules_.add(L"PHP", L"const", T_CONST, L".");
    langRules_.add(L"PHP", L"static", T_STATIC, L".");
    langRules_.add(L"PHP", L"include", T_INCLUDE, L".");
    langRules_.add(L"PHP", L"include_once", T_INCLUDE_ONCE, L".");
    langRules_.add(L"PHP", L"require", T_REQUIRE, L".");
    langRules_.add(L"PHP", L"require_once", T_REQUIRE_ONCE, L".");
    langRules_.add(L"PHP", L"[a-zA-Z_][a-zA-Z0-9_]*", T_IDENTIFIER, L".");
    langRules_.add(L"PHP", L"\\$[a-zA-Z_][a-zA-Z0-9_]*", T_VARIABLE, L".");
    langRules_.add(L"PHP", L"[0-9]+", T_LNUMBER, L".");
    langRules_.add(L"PHP", L"([0-9]*[\\.][0-9]+)|([0-9]+[\\.][0-9]*)", T_DNUMBER, L".");
    langRules_.add(L"PHP", L"[ \\t\\n]+", T_WHITESPACE, L".");
    langRules_.add(L"PHP", L"\\/\\*\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/", T_DOC_COMMENT, L".");
    langRules_.add(L"PHP", L"\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/", T_MULTILINE_COMMENT, L".");
    langRules_.add(L"PHP", L"\\/\\/.*$", T_SINGLELINE_COMMENT, L".");
    langRules_.add(L"PHP", L"b*[\"](\\\\\\\"|[^\"])*[\"]", T_DQ_STRING, L".");
    langRules_.add(L"PHP", L"b*[\'](\\\\\\.|[^\'])*[\']", T_SQ_STRING, L".");

    boost::lexer::wgenerator::build (langRules_, langState_);
    lexInput_ = new boost::lexer::iter_winput(&langState_, sourceBegin_, sourceEnd_);

    // dump the state machine
    //boost::lexer::wgenerator::minimise(langState_);
    //boost::lexer::wdebug::dump(langState_, std::wcout);

}

bool pLexer::preprocess(void) {

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

        curID = iter->id;
        if ( (curID == boost::lexer::npos) ||
             (curID == 0) ) {
            // eoi or lex error. stop preprocessor and let the main parser
            // show a nice error if necessary
            success = (curID == 0);
            rewrote = true;
            break;
        }
        else if ( (curID == T_OPEN_TAG) ||
                  (curID == T_CLOSE_TAG) ) {
            buffer.append(iter->start, iter->end);
        }
        else if (curID == T_DQ_STRING) {

            rewrote = true;

            // we now use the preprocessor lexer to find the tokens within
            // this double quoted string
            boost::lexer::iter_winput* preInput = new boost::lexer::iter_winput(&dqState_, iter->start, iter->end);
            for (pTokenIterator dqIter = preInput->begin();
                                dqIter != preInput->end();
                                ++dqIter)
            {
                // iterate over DQ tokens
                switch(dqIter->id) {
                    case T_DQ_DONE:
                        goto endOfDQ; // omg!
                    case T_DQ_ESCAPE:
                        // replace with non escaped dq
                        buffer.push_back('"');
                        break;
                    case T_DQ_DQ:
                        // replace with single
                        buffer.push_back('\'');
                        break;
                    case T_DQ_NEWLINE:
                        // replace newline escape sequence with literal newline
                        buffer.push_back(RPHP_NEWLINE);
                        break;
                    case boost::lexer::npos:
                    default:
                        // passthrough
                        buffer.append(dqIter->start, dqIter->end);
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
            buffer.append(iter->start, iter->end);
        }

    }

    // save changes
    if (rewrote) {
        contents_.swap(buffer);
    }

    sourceBegin_ = contents_.begin();
    sourceEnd_ = contents_.end();
    delete lexInput_;
    lexInput_ = new boost::lexer::iter_winput(&langState_, sourceBegin_, sourceEnd_);

    return success;

}

pTokenIterator pLexer::tokBegin(void) {
    return lexInput_->begin();
}

pTokenIterator pLexer::tokEnd(void) {
    return lexInput_->end();
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
        if ( (iter->id == boost::lexer::npos) ||
             (iter->id == 0) ) {
            // no match or eoi
            break;
        }
        else {
            // matched
            // skip plain newlines in html state
            val.str(L"");
            if (iter->id != T_WHITESPACE)
                val << pSourceString(iter->start, iter->end);
            if ((iter->state == 0) && (val.str() == L"\n"))
                continue;
            tokID = getTokenDescription(iter->id);
            if (tokID.size() == 0)
                tokID = val.str();
            std::wcout << val.str() << L" " << tokID << std::endl;
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
        case T_INSTANCEOF:
            return L"T_INSTANCEOF";
        case T_FOREACH:
            return L"T_FOREACH";
        case T_FOR:
            return L"T_FOR";
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
        case T_NOT_IDENTICAL:
            return L"T_IS_NOT_IDENTICAL";
        case T_DBL_COLON:
            return L"T_DOUBLE_COLON";
        case T_INC:
            return L"T_INC";
        case T_DEC:
            return L"T_DEC";
        case T_EXIT:
            return L"T_EXIT";
        case T_SWITCH:
            return L"T_SWITCH";
        case T_CASE:
            return L"T_CASE";
        case T_BREAK:
            return L"T_BREAK";
        case T_CONTINUE:
            return L"T_CONTINUE";
        case T_DEFAULT:
            return L"T_DEFAULT";
        case T_CLONE:
            return L"T_CLONE";
        case T_TRY:
            return L"T_TRY";
        case T_CATCH:
            return L"T_CATCH";
        case T_THROW:
            return L"T_THROW";
        case T_STATIC:
            return L"T_STATIC";
        case T_CONST:
            return L"T_CONST";
        case T_GREATER_THAN_OR_EQUAL:
            return L"T_IS_GREATER_OR_EQUAL";
        case T_LESS_THAN_OR_EQUAL:
            return L"T_IS_SMALLER_OR_EQUAL";
        case T_LOGICAL_OR:
            return L"T_LOGICAL_OR";
        case T_LOGICAL_XOR:
            return L"T_LOGICAL_XOR";
        case T_LOGICAL_AND:
            return L"T_LOGICAL_AND";
        case T_PLUS_EQUAL:
            return L"T_PLUS_EQUAL";
        case T_MINUS_EQUAL:
            return L"T_MINUS_EQUAL";
    }
    return L"";

}


} } // namespace

