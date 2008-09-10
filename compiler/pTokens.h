/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
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

#ifndef RPHP_PTOKENS_H_
#define RPHP_PTOKENS_H_

namespace rphp {

typedef enum {

    T_VARIABLE = 1000,
    T_WHITESPACE,
    T_ML_COMMENT,
    T_SL_COMMENT,
    T_ECHO,
    T_OPEN_TAG,
    T_CLOSE_TAG,
    T_LNUMBER,
    T_INLINE_HTML,
    T_IF,
    T_ELSE,
    T_WHILE,
    T_IDENTIFIER,
    T_CONSTANT_ENCAPSED_STRING,
    T_SEMI

} languageTokenIDType;

}

#endif /* RPHP_PTOKENS_H_ */
