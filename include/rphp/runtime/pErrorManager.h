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

#ifndef RPHP_PERRORMANAGER
#define RPHP_PERRORMANAGER

#include "rphp/runtime/pRuntimeTypes.h"

namespace rphp {

class pRuntimeEngine;


/**

 @brief Error handling, including stack tracing and warning/notice handlers

 */
class pErrorManager {

private:
    pRuntimeEngine* runtime_;
    pUInt verbosity_;
    bool fatalState_;
    pInt currentLine_;
    pFileNameString currentFile_;
    pFunPointer5 errorHandler_;
    pFunPointer1 exceptionHandler_;

public:

    pErrorManager(pRuntimeEngine *r) :
        runtime_(r),
        verbosity_(E_ALL),
        fatalState_(false),
        currentLine_(0),
        currentFile_("unknown"),
        errorHandler_(NULL),
        exceptionHandler_(NULL)
    {

    }

    void startUp(void);

    void setSourceLocation(const pFileNameString& file, pInt line) {
        currentFile_ = file;
        currentLine_ = line;
    }

    // this matches pNotifyEmitFun
    void notify(pUInt level, const pMsgString& msg);

    // fatal error
    void fatalError(const pMsgString& msg);

    // XXX exception throw

    // throw php notice
    void notice(const pMsgString& msg) {
        notify(E_NOTICE, msg);
    }

    // throw php warning
    void warning(const pMsgString& msg) {
        notify(E_WARNING, msg);
    }

    // throw php recoverable error
    void recoverableError(const pMsgString& msg) {
        notify(E_RECOVERABLE_ERROR, msg);
    }

};

}

#endif /* RPHP_PERRORMANAGER_ */
