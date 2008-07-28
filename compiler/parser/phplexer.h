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
#include <vector>
#include <string>
#include <unicode/unistr.h>

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
	template <typename StringClass, typename CharClass>
	class Lexer {
	public:
		Lexer(parser* _parser, const StringClass& contents);

		int nextTokenKind();
		rint64 tokenBegin() const;
		rint64 tokenEnd() const;

	private:
		StringClass m_content;
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
		bool isValidVariableIdentifier(const CharClass& it);
		void createNewline( int pos );
		bool isEscapedWithBackslash(const CharClass& it, int curPos, int startPos, int pos);
		bool isHeredocEnd(const CharClass& it, int pos);

		CharClass inline lookAt( int pos ){
            return m_content[pos];
        }

		std::vector<int> m_state; // was: QStack<int>
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
		StringClass m_heredocIdentifier;
		int m_haltCompiler;
	};

	// define the two lexers we support: Unicode and std::string based
	typedef Lexer<UnicodeString, UChar32> ULexer;
	typedef Lexer<std::string, char> BLexer;

}

#endif

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
