/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PRUNTIMEERROR_H_
#define RPHP_PRUNTIMEERROR_H_

#include <string>
#include <stdexcept>

namespace rphp {

class pRuntimeError : public std::runtime_error {

    std::string msg_;

public:
    pRuntimeError(const std::string& msg):
        std::runtime_error(""),
        msg_(msg) { }

    pRuntimeError(const std::wstring& msg):
        std::runtime_error(""),
        msg_(msg.begin(), msg.end()) { }

    ~pRuntimeError(void) throw() { }

    const char* what() const throw() {
        std::string fullMsg;
        fullMsg = "runtime error: " + msg_ + " on line 0 of xxx";
        return fullMsg.c_str();
    }

};

} // namespace

#endif /* RPHP_PRUNTIMEERROR_H_ */
