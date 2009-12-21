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

#ifndef RPHP_PPASS_H_
#define RPHP_PPASS_H_

#include<string>

namespace rphp {

class pSourceModule;

namespace AST {

class pPass {

private:
    // no copy constructor
    pPass(const pPass& p) { }

protected:
    std::string passName_;
    std::string passDesc_;

    pSourceModule* module_;

public:

    pPass(const char* n, const char* d, pSourceModule* m): passName_(n), passDesc_(d), module_(m) { }

    virtual ~pPass(void) { }

    virtual void run(void) = 0;


};

} } // namespace

#endif /* RPHP_PPASS_H_ */
