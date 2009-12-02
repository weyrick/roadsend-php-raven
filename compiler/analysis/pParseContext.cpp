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


#include "rphp/analysis/pParseContext.h"
#include "rphp/analysis/pSourceModule.h"
#include "rphp/analysis/pSourceFile.h"

#include <iostream>

namespace rphp {

namespace AST {

void pParseContext::parseError(pSourceRange* r) {

    // show the line the error occured on
    // if it was a lex error, we show 1 character. if it was a parse error,
    // we show up the length of the problem token
    pUInt probsize;
    pSourceString problem;
    if (r) {
        probsize = (*r).end()-(*r).begin();
        problem.append((*r).begin(), (*r).end());
    }
    else {
        probsize = 1;
        problem.append(lastToken_->end(), lastToken_->end()+1);
    }

    pSourceCharIterator eLineStart(lastNewline_+1);
    pSourceCharIterator eLineStop(lastToken_->end()+probsize);

    // try to take eLineStop to next new line
    if (eLineStop < owner_->source()->contents().end()) {
        while ( (*eLineStop != '\n') && (eLineStop < (owner_->source()->contents().end())) ) {
            ++eLineStop;
        }
    }

    pSourceString errorLine;
    if (eLineStop > eLineStart)
        errorLine.append(eLineStart, eLineStop);

    // error line with arrow
    if (!errorLine.empty()) {
        std::cerr << errorLine << std::endl;
        std::cerr << pSourceString((lastToken_->end()+1)-(lastNewline_+1)-1,'-') << "^" << std::endl;
    }

    // message
    std::cerr << "parse error: unexpected '"
               << problem
               << "' in ";
    std::cerr << owner_->source()->fileName();
    std::cerr  << " on line "
               << currentLineNum_
               <<  std::endl;

    // what now?
    exit(-1);

}


} } // namespace

