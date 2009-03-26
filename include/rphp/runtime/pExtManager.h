/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PEXTMANAGER
#define RPHP_PEXTMANAGER

#include <deque>
#include <string>

namespace rphp {

class pRuntimeEngine;
class pExtBase;

/**
 
 @brief Manages the extensions available to the runtime
 
 */
class pExtManager {
public:
    typedef std::deque<pExtBase*> extRegistryType;

private:
    pRuntimeEngine* runtime_;
    extRegistryType extRegistry_;

public:
    pExtManager(pRuntimeEngine *r): runtime_(r) { }
    ~pExtManager();

    /// run upon creation of the runtime object to initialize the extensions
    void startUp();

};

}

#endif /* RPHP_PEXTMANAGER_ */
