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

#include "rphp/analysis/pLexer.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

namespace rphp { namespace lexer {

pLexer::pLexer(const pSourceFile* s):
    langRules_((boost::lexer::regex_flags)(boost::lexer::icase | boost::lexer::dot_not_newline)),
    langState_(),
    lexInput_(NULL),
    dqRules_(),
    dqState_(),
    source_(s),
    // TODO: bad copy here. because we mutate in preprocessor
    contents_(source_->contents()),
    sourceBegin_(contents_.begin()),
    sourceEnd_ (contents_.end())
{

    dqRules_.add("INITIAL", "\\\\n", T_DQ_NEWLINE, ".");
    dqRules_.add("INITIAL", "\\\"", T_DQ_DQ, ".");
    dqRules_.add("INITIAL", "\\\\\\\"", T_DQ_ESCAPE, ".");
    dqRules_.add("INITIAL", "\\$[a-zA-Z_][a-zA-Z0-9_]*", T_DQ_VARIABLE, ".");
    boost::lexer::generator::build (dqRules_, dqState_);

    langRules_.add_state("PHP");

    langRules_.add_macro ("DIGIT", "[0-9]");
    langRules_.add_macro ("OCTALDIGIT", "[0-7]");
    langRules_.add_macro ("HEXDIGIT", "[0-9a-fA-F]");
    langRules_.add_macro ("IDCHARS", "[a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*");

    langRules_.add("INITIAL", "<\\?\\s*|<\\?PHP\\s*", T_OPEN_TAG, "PHP"); // go to PHP state
    langRules_.add("INITIAL", ".+|\\n+", T_INLINE_HTML, ".");

    langRules_.add("PHP", "\\?>", T_CLOSE_TAG, "INITIAL"); // go to HTML state

    langRules_.add("PHP", "\\`", T_TICK, ".");
    langRules_.add("PHP", "\\~", T_TILDE, ".");
    langRules_.add("PHP", "\\(", T_LEFTPAREN, ".");
    langRules_.add("PHP", "\\)", T_RIGHTPAREN, ".");
    langRules_.add("PHP", "\\{", T_LEFTCURLY, ".");
    langRules_.add("PHP", "\\}", T_RIGHTCURLY, ".");
    langRules_.add("PHP", "\\[", T_LEFTSQUARE, ".");
    langRules_.add("PHP", "\\]", T_RIGHTSQUARE, ".");
    langRules_.add("PHP", "\\>", T_GREATER_THAN, ".");
    langRules_.add("PHP", "\\<", T_LESS_THAN, ".");
    langRules_.add("PHP", "\\=", T_ASSIGN, ".");
    langRules_.add("PHP", "\\-", T_MINUS, ".");
    langRules_.add("PHP", "\\+", T_PLUS, ".");
    langRules_.add("PHP", "\\/", T_DIV, ".");
    langRules_.add("PHP", "\\%", T_MOD, ".");
    langRules_.add("PHP", "\\*", T_MULT, ".");
    langRules_.add("PHP", "\\;", T_SEMI, ".");
    langRules_.add("PHP", "\\,", T_COMMA, ".");
    langRules_.add("PHP", "\\|", T_PIPE, ".");
    langRules_.add("PHP", "\\.", T_DOT, ".");
    langRules_.add("PHP", "\\!", T_NOT, ".");
    langRules_.add("PHP", "\\&", T_AND, ".");
    langRules_.add("PHP", "\\@", T_AT, ".");
    langRules_.add("PHP", "\\?", T_QUESTION, ".");
    langRules_.add("PHP", "\\:", T_COLON, ".");
    langRules_.add("PHP", ">=", T_GREATER_THAN_OR_EQUAL, ".");
    langRules_.add("PHP", "<=", T_LESS_THAN_OR_EQUAL, ".");
    langRules_.add("PHP", "<<", T_SL, ".");
    langRules_.add("PHP", ">>", T_SR, ".");
    langRules_.add("PHP", "<<=", T_SL_EQUAL, ".");
    langRules_.add("PHP", ">>=", T_SR_EQUAL, ".");
    langRules_.add("PHP", "::", T_DBL_COLON, ".");
    langRules_.add("PHP", "\\+\\+", T_INC, ".");
    langRules_.add("PHP", "\\-\\-", T_DEC, ".");
    langRules_.add("PHP", "\\^\\=", T_XOR_EQUAL, ".");
    langRules_.add("PHP", "\\|\\=", T_OR_EQUAL, ".");
    langRules_.add("PHP", "\\&\\=", T_AND_EQUAL, ".");
    langRules_.add("PHP", "\\%\\=", T_MOD_EQUAL, ".");
    langRules_.add("PHP", "\\.\\=", T_CONCAT_EQUAL, ".");
    langRules_.add("PHP", "\\/\\=", T_DIV_EQUAL, ".");
    langRules_.add("PHP", "\\*\\=", T_MUL_EQUAL, ".");
    langRules_.add("PHP", "\\+\\=", T_PLUS_EQUAL, ".");
    langRules_.add("PHP", "\\-\\=", T_MINUS_EQUAL, ".");
    langRules_.add("PHP", "\\!\\=", T_NOTEQUAL, ".");
    langRules_.add("PHP", "\\.\\=", T_DOTEQUAL, ".");
    langRules_.add("PHP", "==", T_EQUAL, ".");
    langRules_.add("PHP", "\\!==", T_NOT_IDENTICAL, ".");
    langRules_.add("PHP", "===", T_IDENTICAL, ".");
    langRules_.add("PHP", "\\&\\&", T_BOOLEAN_AND, ".");
    langRules_.add("PHP", "\\|\\|", T_BOOLEAN_OR, ".");
    langRules_.add("PHP", "\\?>", T_CLOSE_TAG, ".");
    langRules_.add("PHP", "=>", T_ARROWKEY, ".");
    langRules_.add("PHP", "->", T_CLASSDEREF, ".");
    langRules_.add("PHP", "list", T_LIST, ".");
    langRules_.add("PHP", "or", T_LOGICAL_OR, ".");
    langRules_.add("PHP", "and", T_LOGICAL_AND, ".");
    langRules_.add("PHP", "xor", T_LOGICAL_XOR, ".");
    langRules_.add("PHP", "if", T_IF, ".");
    langRules_.add("PHP", "for", T_FOR, ".");
    langRules_.add("PHP", "endfor", T_ENDFOR, ".");
    langRules_.add("PHP", "foreach", T_FOREACH, ".");
    langRules_.add("PHP", "endforeach", T_ENDFOREACH, ".");
    langRules_.add("PHP", "interface", T_INTERFACE, ".");
    langRules_.add("PHP", "as", T_AS, ".");
    langRules_.add("PHP", "exit", T_EXIT, ".");
    langRules_.add("PHP", "print", T_PRINT, ".");
    langRules_.add("PHP", "eval", T_EXIT, ".");
    langRules_.add("PHP", "public", T_PUBLIC, ".");
    langRules_.add("PHP", "private", T_PRIVATE, ".");
    langRules_.add("PHP", "protected", T_PROTECTED, ".");
    langRules_.add("PHP", "extends", T_EXTENDS, ".");
    langRules_.add("PHP", "return", T_RETURN, ".");
    langRules_.add("PHP", "global", T_GLOBAL, ".");
    langRules_.add("PHP", "function", T_FUNCTION, ".");
    langRules_.add("PHP", "namespace", T_NAMESPACE, ".");
    langRules_.add("PHP", "isset", T_ISSET, ".");
    langRules_.add("PHP", "unset", T_UNSET, ".");
    langRules_.add("PHP", "empty", T_EMPTY, ".");
    langRules_.add("PHP", "array", T_ARRAY, ".");
    langRules_.add("PHP", "while", T_WHILE, ".");
    langRules_.add("PHP", "endwhile", T_ENDWHILE, ".");
    langRules_.add("PHP", "else", T_ELSE, ".");
    langRules_.add("PHP", "elseif", T_ELSEIF, ".");
    langRules_.add("PHP", "echo", T_ECHO, ".");
    langRules_.add("PHP", "new", T_NEW, ".");
    langRules_.add("PHP", "var", T_VAR, ".");
    langRules_.add("PHP", "switch", T_SWITCH, ".");
    langRules_.add("PHP", "case", T_CASE, ".");
    langRules_.add("PHP", "break", T_BREAK, ".");
    langRules_.add("PHP", "continue", T_CONTINUE, ".");
    langRules_.add("PHP", "default", T_DEFAULT, ".");
    langRules_.add("PHP", "instanceof", T_INSTANCEOF, ".");
    langRules_.add("PHP", "class", T_CLASS, ".");
    langRules_.add("PHP", "clone", T_CLONE, ".");
    langRules_.add("PHP", "throw", T_THROW, ".");
    langRules_.add("PHP", "try", T_TRY, ".");
    langRules_.add("PHP", "catch", T_CATCH, ".");
    langRules_.add("PHP", "goto", T_GOTO, ".");
    langRules_.add("PHP", "const", T_CONST, ".");
    langRules_.add("PHP", "static", T_STATIC, ".");
    langRules_.add("PHP", "include", T_INCLUDE, ".");
    langRules_.add("PHP", "include_once", T_INCLUDE_ONCE, ".");
    langRules_.add("PHP", "require", T_REQUIRE, ".");
    langRules_.add("PHP", "require_once", T_REQUIRE_ONCE, ".");
    langRules_.add("PHP", "__FILE__", T_MAGIC_FILE, ".");
    langRules_.add("PHP", "__LINE__", T_MAGIC_LINE, ".");
    langRules_.add("PHP", "__CLASS__", T_MAGIC_CLASS, ".");
    langRules_.add("PHP", "__METHOD__", T_MAGIC_METHOD, ".");
    langRules_.add("PHP", "__FUNCTION__", T_MAGIC_FUNCTION, ".");
    langRules_.add("PHP", "__NAMESPACE__", T_MAGIC_NS, ".");
    langRules_.add("PHP", "\\((int|integer)\\)", T_INT_CAST, ".");
    langRules_.add("PHP", "\\((real|double|float)\\)", T_FLOAT_CAST, ".");
    langRules_.add("PHP", "\\(string\\)", T_STRING_CAST, ".");
    langRules_.add("PHP", "\\(binary\\)", T_BINARY_CAST, ".");
    langRules_.add("PHP", "\\(unicode\\)", T_UNICODE_CAST, ".");
    langRules_.add("PHP", "\\(array\\)", T_ARRAY_CAST, ".");
    langRules_.add("PHP", "\\(object\\)", T_OBJECT_CAST, ".");
    langRules_.add("PHP", "\\((bool|boolean)\\)", T_BOOL_CAST, ".");
    langRules_.add("PHP", "\\(unset\\)", T_UNSET_CAST, ".");
    langRules_.add("PHP", "{IDCHARS}", T_IDENTIFIER, ".");
    langRules_.add("PHP", "\\${IDCHARS}", T_VARIABLE, ".");
    langRules_.add("PHP", "((0x|0X){HEXDIGIT}+|0{OCTALDIGIT}*|[1-9]{DIGIT}*)", T_LNUMBER, ".");
    langRules_.add("PHP", "([0-9]*[\\.][0-9]+)|([0-9]+[\\.][0-9]*)", T_DNUMBER, ".");
    langRules_.add("PHP", "[ \\t\\n]+", T_WHITESPACE, ".");
    langRules_.add("PHP", "\\/\\*\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/", T_DOC_COMMENT, ".");
    langRules_.add("PHP", "\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/", T_MULTILINE_COMMENT, ".");
    langRules_.add("PHP", "\\/\\/.*$", T_SINGLELINE_COMMENT, ".");
    langRules_.add("PHP", "b*[\"](\\\\\\\"|[^\"])*[\"]", T_DQ_STRING, ".");
    langRules_.add("PHP", "b*[\'](\\\\\\.|[^\'])*[\']", T_SQ_STRING, ".");

    boost::lexer::generator::build (langRules_, langState_);
    lexInput_ = new boost::lexer::iter_input(&langState_, sourceBegin_, sourceEnd_);

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
            boost::lexer::iter_input* preInput = new boost::lexer::iter_input(&dqState_, iter->start, iter->end);
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
                        buffer.push_back('\n');
                        break;
                    case T_DQ_VARIABLE:
                        // replace with concatination
                        buffer.append("'.");
                        buffer.append(dqIter->start, dqIter->end);
                        buffer.append(".'");
                        break;
                    case boost::lexer::npos:
                    default:
                        // passthrough
                        buffer.append(dqIter->start, dqIter->end);
                        break;
                }

            }

            endOfDQ:
            delete preInput;
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
    lexInput_ = new boost::lexer::iter_input(&langState_, sourceBegin_, sourceEnd_);

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

    std::string tokID;
    std::stringstream val;

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
            val.str("");
            if (iter->id != T_WHITESPACE)
                val << pSourceString(iter->start, iter->end);
            if ((iter->state == 0) && (val.str() == "\n"))
                continue;
            tokID = getTokenDescription(iter->id);
            if (tokID.size() == 0)
                tokID = val.str();
            std::cout << val.str() << " " << tokID << std::endl;
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
            return "T_INLINE_HTM";
        case T_ECHO:
            return "T_ECHO";
        case T_ARROWKEY:
            return "T_DOUBLE_ARROW";
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
        case T_ELSEIF:
            return "T_ELSEIF";
        case T_WHILE:
            return "T_WHILE";
        case T_ENDWHILE:
            return "T_ENDWHILE";
        case T_NEW:
            return "T_NEW";
        case T_ARRAY:
            return "T_ARRAY";
        case T_IDENTIFIER:
            return "T_STRING";
        case T_DQ_STRING:
        case T_SQ_STRING:
            return "T_CONSTANT_ENCAPSED_STRING";
        case T_SINGLELINE_COMMENT:
        case T_MULTILINE_COMMENT:
            return "T_COMMENT";
        case T_DOC_COMMENT:
            return "T_DOC_COMMENT";
        case T_GLOBAL:
            return "T_GLOBA";
        case T_FUNCTION:
            return "T_FUNCTION";
        case T_EMPTY:
            return "T_EMPTY";
        case T_BOOLEAN_AND:
            return "T_BOOLEAN_AND";
        case T_BOOLEAN_OR:
            return "T_BOOLEAN_OR";
        case T_EQUAL:
            return "T_IS_EQUA";
        case T_ISSET:
            return "T_ISSET";
        case T_UNSET:
            return "T_UNSET";
        case T_VAR:
            return "T_VAR";
        case T_CLASS:
            return "T_CLASS";
        case T_CLASSDEREF:
            return "T_OBJECT_OPERATOR";
        case T_INSTANCEOF:
            return "T_INSTANCEOF";
        case T_FOREACH:
            return "T_FOREACH";
        case T_FOR:
            return "T_FOR";
        case T_ENDFOREACH:
            return "T_ENDFOREACH";
        case T_ENDFOR:
            return "T_ENDFOR";
        case T_AS:
            return "T_AS";
        case T_RETURN:
            return "T_RETURN";
        case T_DOTEQUAL:
            return "T_CONCAT_EQUA";
        case T_LIST:
            return "T_LIST";
        case T_EXTENDS:
            return "T_EXTENDS";
        case T_PUBLIC:
            return "T_PUBLIC";
        case T_PRIVATE:
            return "T_PRIVATE";
        case T_PROTECTED:
            return "T_PROTECTED";
        case T_NOTEQUAL:
            return "T_IS_NOT_EQUA";
        case T_INCLUDE:
            return "T_INCLUDE";
        case T_INCLUDE_ONCE:
            return "T_INCLUDE_ONCE";
        case T_REQUIRE:
            return "T_REQUIRE";
        case T_REQUIRE_ONCE:
            return "T_REQUIRE_ONCE";
        case T_IDENTICAL:
            return "T_IS_IDENTICA";
        case T_NOT_IDENTICAL:
            return "T_IS_NOT_IDENTICA";
        case T_DBL_COLON:
            return "T_DOUBLE_COLON";
        case T_INC:
            return "T_INC";
        case T_DEC:
            return "T_DEC";
        case T_EXIT:
            return "T_EXIT";
        case T_SWITCH:
            return "T_SWITCH";
        case T_ENDSWITCH:
            return "T_ENDSWITCH";
        case T_CASE:
            return "T_CASE";
        case T_BREAK:
            return "T_BREAK";
        case T_CONTINUE:
            return "T_CONTINUE";
        case T_DEFAULT:
            return "T_DEFAULT";
        case T_CLONE:
            return "T_CLONE";
        case T_TRY:
            return "T_TRY";
        case T_CATCH:
            return "T_CATCH";
        case T_THROW:
            return "T_THROW";
        case T_STATIC:
            return "T_STATIC";
        case T_CONST:
            return "T_CONST";
        case T_GREATER_THAN_OR_EQUAL:
            return "T_IS_GREATER_OR_EQUA";
        case T_LESS_THAN_OR_EQUAL:
            return "T_IS_SMALLER_OR_EQUA";
        case T_LOGICAL_OR:
            return "T_LOGICAL_OR";
        case T_LOGICAL_XOR:
            return "T_LOGICAL_XOR";
        case T_LOGICAL_AND:
            return "T_LOGICAL_AND";
        case T_PLUS_EQUAL:
            return "T_PLUS_EQUA";
        case T_MINUS_EQUAL:
            return "T_MINUS_EQUA";
        case T_SR:
            return "T_SR";
        case T_SL:
            return "T_S";
        case T_SR_EQUAL:
            return "T_SR_EQUA";
        case T_SL_EQUAL:
            return "T_SL_EQUA";
        case T_EVAL:
            return "T_EVA";
        case T_XOR_EQUAL:
            return "T_XOR_EQUA";
        case T_OR_EQUAL:
            return "T_OR_EQUA";
        case T_AND_EQUAL:
            return "T_AND_EQUA";
        case T_MOD_EQUAL:
            return "T_MOD_EQUA";
        case T_CONCAT_EQUAL:
            return "T_CONCAT_EQUA";
        case T_DIV_EQUAL:
            return "T_DIV_EQUA";
        case T_MUL_EQUAL:
            return "T_MUL_EQUA";
        case T_NAMESPACE:
            return "T_NAMESPACE";
        case T_INT_CAST:
            return "T_INT_CAST";
        case T_FLOAT_CAST:
            return "T_DOUBLE_CAST";
        case T_STRING_CAST:
            return "T_STRING_CAST";
        case T_UNICODE_CAST:
            return "T_UNICODE_CAST";
        case T_BINARY_CAST:
            return "T_BINARY_CAST";
        case T_ARRAY_CAST:
            return "T_ARRAY_CAST";
        case T_OBJECT_CAST:
            return "T_OBJECT_CAST";
        case T_UNSET_CAST:
            return "T_UNSET_CAST";
        case T_BOOL_CAST:
            return "T_BOOL_CAST";
        case T_MAGIC_FILE:
            return "T_FILE";
        case T_MAGIC_LINE:
            return "T_LINE";
        case T_MAGIC_CLASS:
            return "T_CLASS_C";
        case T_MAGIC_FUNCTION:
            return "T_FUNCTION_C";
        case T_MAGIC_METHOD:
            return "T_METHOD_C";
        case T_MAGIC_NS:
            return "T_NS_C";
        case T_PRINT:
            return "T_PRINT";
        case T_INTERFACE:
            return "T_INTERFACE";

    }
    return "";

}


} } // namespace

