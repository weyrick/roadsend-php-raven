/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2009 Shannon Weyrick (weyrick@roadsend.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * ***** END LICENSE BLOCK ***** */

#ifndef PSTANDARDEXT_H_
#define PSTANDARDEXT_H_

#include "rphp/runtime/pExtBase.h"

#define STD_EXT_NAME "standard"

namespace rphp {
 namespace ext {
  namespace standard {

    class pStandardExt : public pExtBase {

    public:

        pStandardExt(pRuntimeEngine *r) : pExtBase(r, STD_EXT_NAME) { }

        void extensionStartup();
        void extensionShutdown();

    };

    // standard functions
    void var_dump(RPHP_STDFUNC_ARGS, const pVar& v);
    void strlen(RPHP_STDFUNC_ARGS, const pVar& str);
    void strpos(RPHP_STDFUNC_ARGS, const pVar& haystack, const pVar& needle, const pVar& offset);

    }
  }
}

#endif /* PSTANDARDEXT_H_ */
