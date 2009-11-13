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

#ifndef RPHP_PCACHEDJIT_H_
#define RPHP_PCACHEDJIT_H_

#include "rphp/pTarget.h"
#include "rphp/pTypes.h"
#include "rphp/pSourceTypes.h"

namespace rphp {

class pRuntimeEngine;

/**
 *
 * this class is given a config, and creates a runtime instance based on it.
 * it can then be given filenames to interpret from disk. these are compiled
 * to IR, cached, then JITed. the runtime may be reset between requests.
 *
 */
class pCachedJIT: public pTarget {

    pRuntimeEngine* runtime_;

public:
    pCachedJIT(pConfig* config);
    ~pCachedJIT(void);

    void execute(void) { }

    void cacheAndJITFileOnDisk(const pSourceFileDesc& sourceFile);

};

}

#endif /* RPHP_PCACHEDJIT_H_ */
