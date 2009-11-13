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

#include "rphp/runtime/pRuntimeEngine.h"

#include "rphp/pConfig.h"
#include "rphp/runtime/pErrorManager.h"
#include "rphp/runtime/pFunctionManager.h"
#include "rphp/runtime/pClassManager.h"
#include "rphp/runtime/pExtManager.h"

namespace rphp {

pRuntimeEngine::pRuntimeEngine(pConfig* c) :
                                   globals_(),
                                   ownConfig_(false),
                                   includeHandler_(NULL),
                                   evalHandler_(NULL),
                                   unicode_(),
                                   errorManager(new pErrorManager(this)),
                                   config(c),
                                   output(this),
                                   functionManager(new pFunctionManager(this)),
                                   classManager(new pClassManager(this)),
                                   extManager(new pExtManager(this))
{

    // handle empty config
    if (!config) {
        config = new pConfig();
        ownConfig_ = true;
    }

    // error handler initialization
    errorManager->startUp();

    // runtime initialization
    extManager->startUp();

}


pRuntimeEngine::~pRuntimeEngine() {

    // runtime shutdown
    delete extManager;
    delete functionManager;
    delete classManager;
    delete errorManager;
    if (ownConfig_)
        delete config;

}

} /* end namespace rphp */

