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

#include "rphp/pTypes.h"

namespace rphp {

class pConfig;

/**
    A target is a configurable purpose created in the frontend
    it conveys the user's wishes to a driver. it is configured by
    a pConfig object, and can emit warnings and notices
 */
class pTarget {

protected:

    pNotifyEmitFun notifyHandler_;
    pConfig* config_;
    pUInt verbosity_;

    /**
        log a message at the given verbosity level
     */
    void log(pUInt level, pMsgString msg) {
        if (notifyHandler_ && (verbosity_ & level)) {
            notifyHandler_(level, msg);
        }
    }

public:

    pTarget(pConfig* c): notifyHandler_(NULL), config_(c), verbosity_(0) { }

    pTarget(void): notifyHandler_(NULL),
                   config_(NULL),
                   verbosity_(0) { }

    virtual ~pTarget(void) { }

    virtual void execute(void) = 0;

    pUInt verbosity(void) const { return verbosity_; }

    void setVerbosity(pUInt v) { verbosity_ = v; }
    void setVerbosity(const pTarget* t) { verbosity_ = t->verbosity_; }

    const pConfig* config(void) const { return config_; }

    void setConfig(pConfig* c) { config_ = c; }
    void setConfig(const pTarget* t) { config_ = t->config_; }

    void configureWith(const pTarget* t) {
        notifyHandler_ = t->notifyHandler_;
        verbosity_ = t->verbosity_;
        config_ = t->config_;
    }

    void setNotifyHandler(pNotifyEmitFun f) { notifyHandler_ = f; }

};

} // namespace

#endif /* RPHP_TARGET_H_ */
