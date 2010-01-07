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
#include "rphp/analysis/pSourceFile.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

namespace rphp { namespace lexer {

pLexer::pLexer(const pSourceFile* s):
    source_(s),
    sourceBegin_(source_->contents().begin()),
    sourceEnd_ (source_->contents().end())
{



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

    pUInt curID(0);
    std::size_t state(0), newState(0), uniqueID(0);
    pSourceCharIterator tokStart(sourceBegin_);
    pSourceCharIterator tokEnd(sourceBegin_);
    while ( (curID = rphp_nextLangToken(newState, tokEnd, sourceEnd_, uniqueID)) )
    {

        if (curID == 0) {
            // end of input
            break;
        }
        else if (curID == boost::lexer::npos) {
            // if state is HTML, collect characters for INLINE HTML token
            if (state == 0) {                
                while ((*tokEnd != '<') && (tokEnd != sourceEnd_)) {
                    tokEnd++;
                }
                std::cout << pSourceString(tokStart, tokEnd) << " " << getTokenDescription(T_INLINE_HTML) << std::endl;
            }
            else {
                // unmatched character in PHP state
                break;
            }
        }
        else {
            // matched
            // skip plain newlines in html state
            val.str("");
            if (curID != T_WHITESPACE)
                val << pSourceString(tokStart, tokEnd);
            if ((state == 0) && (val.str() == "\n"))
                continue;
            tokID = getTokenDescription(curID);
            if (tokID.size() == 0)
                tokID = val.str();
            std::cout << val.str() << " " << tokID << std::endl;
        }

        // next token
        tokStart = tokEnd;
        state = newState;

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
            return "T_GLOBAL";
        case T_FUNCTION:
            return "T_FUNCTION";
        case T_EMPTY:
            return "T_EMPTY";
        case T_BOOLEAN_AND:
            return "T_BOOLEAN_AND";
        case T_BOOLEAN_OR:
            return "T_BOOLEAN_OR";
        case T_EQUAL:
            return "T_IS_EQUAL";
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
            /*
        case T_INCLUDE:
            return "T_INCLUDE";
        case T_INCLUDE_ONCE:
            return "T_INCLUDE_ONCE";
        case T_REQUIRE:
            return "T_REQUIRE";
        case T_REQUIRE_ONCE:
            return "T_REQUIRE_ONCE";
            */
        case T_IDENTICAL:
            return "T_IS_IDENTICAL";
        case T_NOT_IDENTICAL:
            return "T_IS_NOT_IDENTICAL";
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
        case T_GREATER_OR_EQUAL:
            return "T_IS_GREATER_OR_EQUAL";
        case T_LESS_OR_EQUAL:
            return "T_IS_SMALLER_OR_EQUAL";
        case T_BOOLEAN_OR_LIT:
            return "T_LOGICAL_OR";
        case T_BOOLEAN_XOR_LIT:
            return "T_LOGICAL_XOR";
        case T_BOOLEAN_AND_LIT:
            return "T_LOGICAL_AND";
        case T_PLUS_EQUAL:
            return "T_PLUS_EQUAL";
        case T_MINUS_EQUAL:
            return "T_MINUS_EQUAL";
        case T_SR:
            return "T_SR";
        case T_SL:
            return "T_S";
        case T_SR_EQUAL:
            return "T_SR_EQUAL";
        case T_SL_EQUAL:
            return "T_SL_EQUAL";
        case T_EVAL:
            return "T_EVA";
        case T_XOR_EQUAL:
            return "T_XOR_EQUAL";
        case T_OR_EQUAL:
            return "T_OR_EQUAL";
        case T_AND_EQUAL:
            return "T_AND_EQUAL";
        case T_MOD_EQUAL:
            return "T_MOD_EQUAL";
        case T_CONCAT_EQUAL:
            return "T_CONCAT_EQUAL";
        case T_DIV_EQUAL:
            return "T_DIV_EQUAL";
        case T_MUL_EQUAL:
            return "T_MUL_EQUAL";
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

