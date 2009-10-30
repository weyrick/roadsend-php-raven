/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2009 Shannon Weyrick <weyrick@roadsend.com>
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


#include "rphp/runtime/pErrorManager.h"

#include "rphp/runtime/pRuntimeEngine.h"
#include "rphp/runtime/pRuntimeError.h"

#include <iostream>

namespace rphp {

namespace errorManager {

void defaultErrorHandler(RPHP_STDFUNC_ARGS, const pVar& errno, const pVar& errmsg, const pVar& file, const pVar& line, const pVar& context) {

    pBString errTitle;
    switch (errno.copyAsInt()) {
    case E_PARSE:
        errTitle = "Parse error";
        break;
    case E_ERROR:
    case E_CORE_ERROR:
    case E_USER_ERROR:
    case E_COMPILE_ERROR:
        errTitle = "Fatal error";
        break;
    case E_RECOVERABLE_ERROR:
        errTitle = "Catchable fatal error";
        break;
    case E_WARNING:
    case E_CORE_WARNING:
    case E_USER_WARNING:
    case E_COMPILE_WARNING:
        errTitle = "Warning";
        break;
    case E_NOTICE:
    case E_USER_NOTICE:
        errTitle = "Notice";
        break;
    case E_STRICT:
        errTitle = "Strict notice";
        break;
    case E_DEPRECATED:
    case E_USER_DEPRECATED:
        errTitle = "Deprecation notice";
        break;
    case E_COMPILE_DEBUG:
        errTitle = "Compiler debug notice";
        break;
    case E_RUNTIME_DEBUG:
        errTitle = "Runtime debug notice";
        break;
    }
    runtime->output << errTitle << ": " << errmsg << " in " << file << " on line " << line << "\n";

}

void defaultExceptionHandler(RPHP_STDFUNC_ARGS, const pVar& obj) {

    // XXX

}

}

void pErrorManager::startUp(void) {

    errorHandler_ = &errorManager::defaultErrorHandler;
    exceptionHandler_ = &errorManager::defaultExceptionHandler;

}

void pErrorManager::notify(pUInt level, const pMsgString& msg) {

    if (errorHandler_) {
        pVar retval;
        errorHandler_(&retval, runtime_, (pInt)level, msg, currentFile_, currentLine_, pNull);
    }
    else {
        std::cerr << msg << std::endl;
    }

}

void pErrorManager::fatalError(const pMsgString& msg) {
    fatalState_ = true;
    notify(E_ERROR, msg);
    throw pRuntimeError(msg);
}


}


