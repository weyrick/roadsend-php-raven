/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
 *               2008 Thomas Moenicke <tm@php-qt.org>
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

#ifndef RPHP_PRUNTIME_H_
#define RPHP_PRUNTIME_H_

#include "rphp/runtime/pRuntimeTypes.h"
#include "rphp/runtime/pOutputManager.h"

#include <boost/unordered_map.hpp>

namespace rphp {

class pConfig;
class pExtManager;
class pErrorManager;
class pFunctionManager;
class pClassManager;

typedef boost::unordered_map<pIdentString, pVarP> globalRegistryType;

class pRuntimeEngine {

    // global data:
    // --> $GLOBAL and other superglobal symbol table, argc, argv,
    globalRegistryType globals_;

    // --> constants (with define()). support builtins (PATH_SEPARATOR, etc) and interface for dynamic verisons

    // include files: include paths, all files included

    // runtime reset functionality (for page resets)

    // maintenance: startup, shutdown hooks. signal/slots?

    bool ownConfig_;

    // runtime handlers
    pIncludeHandlerFun includeHandler_;
    pEvalHandlerFun evalHandler_;

public:

    pRuntimeEngine(pConfig* c = NULL);
    ~pRuntimeEngine();

    // error manager
    pErrorManager* errorManager;

    // runtime configuration
    pConfig* config;

    // output buffering management
    pOutputManager output;

    // function manager
    // --> store list of available functions, including builtins (from extension manager) which stay on page reset,
    //     and the currently defined via php code, which are reset each page
    // --> interface for new function definition
    pFunctionManager* functionManager;

    // class manager
    // --> similar to function manager, but stores builtin and currently defined classes
    // --> interface for new class definition
    pClassManager* classManager;

    // extension manager
    // --> for loading and registering dynamic extensions (pcre, mysql, etc) and their associated functions, classes
    pExtManager* extManager;


};

} /* end namespace rphp */

#endif /* RPHP_PRUNTIME_H_ */
