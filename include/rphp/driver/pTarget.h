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
#include <iostream>

namespace rphp {

/**
    A target is a configurable purpose created in the frontend
    it conveys the user's wishes to the driver
 */
class pTarget {

protected:
    // hash table for generic options
    boost::unordered_map<std::string, std::string> stringOptions_;
    boost::unordered_map<std::string, int> intOptions_;

    int verbosity_;

public:

    static const int logQuiet = 0;
    static const int logInfo  = 1;
    static const int logFull  = 2;
    static const int logDebug = 3;

    pTarget(void): stringOptions_(),
                   intOptions_(),
                   verbosity_(0) { }

    virtual void execute(void) = 0;

    int verbosity(void) { return verbosity_; }
    void setVerbosity(int v) { verbosity_ = v; }

    void setOption(std::string k, std::string v) {
        stringOptions_[k] = v;
    }

    void setOption(std::string k, int v) {
        intOptions_[k] = v;
    }

    /**
        log a message at the given verbosity level
     */
    void log(int level, std::string msg) {
        if (verbosity_ >= level) {
            std::cerr << ">> " << msg << std::endl << std::flush;
        }
    }

};

} // namespace

#endif /* RPHP_TARGET_H_ */
