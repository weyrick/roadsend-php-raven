/* KDevelop PHP Support
 *
 * Copyright 2008 Niko Sams <niko.sams@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef PHPLEXER_H
#define PHPLEXER_H

#include "rphp_ast.h"
#include <stack>
//#include <string>
#include <unistr.h>

namespace rphp
{

class parser;

/**
 * Hand-written Lexer that generates the same tokens as php uses.
 * This includes also a whitespace and comment token.
 *
 * For debugging output can be compared to php-tokens using the
 * test/test-tokenize.php script
 **/
class Lexer {
public:
    Lexer(parser* _parser, const UnicodeString& contents);

    int nextTokenKind();
    rint64 tokenBegin() const;
    rint64 tokenEnd() const;

private:
    UnicodeString m_content;
    parser* m_parser;
    int m_curpos;
    int m_contentSize;
    rint64 m_tokenBegin;
    rint64 m_tokenEnd;

    int state(int deepness = 0) const;
    void pushState(int state);
    void popState();
    void printState();

    bool processVariable(int pos);
    bool isValidVariableIdentifier(const UChar32& it);
    void createNewline( int pos );
    bool isEscapedWithBackslash(const UChar32& it, int curPos, int startPos, int pos);
    bool isHeredocEnd(const UChar32& it, int pos);

    UChar32 lookAt( int pos ){ return m_content.char32At( m_curpos ); }

    std::stack<int> m_state;
    enum State
    {
        ErrorState = -1,
        HtmlState = 0,
        DefaultState = 1,
        String = 2,
        StringVariable = 3,
        StringVariableBracket = 4,
        StringVariableObjectOperator = 5,
        StringVariableCurly = 6,
        StringVarname = 7,
        StringHeredoc = 8,
        StringBacktick = 9
    };
    UnicodeString m_heredocIdentifier;
    int m_haltCompiler;
};

}

#endif

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
