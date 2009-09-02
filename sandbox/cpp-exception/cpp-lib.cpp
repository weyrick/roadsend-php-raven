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


#include "cpp-lib.h"

#include <iostream>
#include <stdexcept>

mockRuntimeLib::mockRuntimeLib(void) {
    std::cout << "mockRuntimeLib initialized" << std::endl;
}

mockRuntimeLib::~mockRuntimeLib(void) {
    std::cout << "mockRuntimeLib destroyed" << std::endl;
}

void mockRuntimeLib::throwError(const std::string& msg) {

    throw std::runtime_error(msg);

}

/*
mockRuntimeLib lib;
*/

extern "C" {
  void mockError() {
    mockRuntimeLib lib;
    lib.mockError();
  }
}


