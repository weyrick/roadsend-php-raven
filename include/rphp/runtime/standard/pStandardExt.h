/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
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

#include "rphp/runtime/pTypes.h"
#include "rphp/runtime/pExtBase.h"

#define STD_EXT_NAME "standard"

namespace rphp {

    class pStandardExt : public pExtBase {

    public:

        pStandardExt(pRuntimeEngine *r) : pExtBase(r, STD_EXT_NAME) { }

        void extensionStartup();
        void extensionShutdown();

        pInt strlen(pVar str);
        pVar strpos(pVar haystack, pVar needle, pVar offset);

    };

}

#endif /* PSTANDARDEXT_H_ */
