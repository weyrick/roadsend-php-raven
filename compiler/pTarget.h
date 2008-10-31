/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PTARGET_H_
#define RPHP_PTARGET_H_

#include <boost/unordered_map.hpp>
#include <string>

namespace rphp {

// A target is a configurable purpose created in the frontend
// it conveys the users wishes to the driver
class pTarget {

protected:
    // hash table for generic options
    boost::unordered_map<std::string, std::string> stringOptions_;
    boost::unordered_map<std::string, int> intOptions_;

public:

    virtual void execute(void) = 0;

    void setOption(std::string k, std::string v) {
        stringOptions_[k] = v;
    }

    void setOption(std::string k, int v) {
        intOptions_[k] = v;
    }

};

} // namespace

#endif /* RPHP_TARGET_H_ */
