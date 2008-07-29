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

#include <iostream>
#include "pExtManager.h"

#include "standard/pStandardExt.h"

namespace rphp {

pExtManager::pExtManager(pRuntimeEngine *r) : runtime(r) {

    // initialize standard library
    pStandardExt* sext = new pStandardExt(r);
    sext->extensionStartup();
    extRegistry.push_back(sext);

}

pExtManager::~pExtManager() {

    // shutdown extensions
    for (registryType::iterator i=extRegistry.begin(); i!=extRegistry.end(); ++i) {
        (*i)->extensionShutdown();
        delete *i;
    }

}

}


