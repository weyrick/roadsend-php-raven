/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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


#include "rphp/JIT/pCachedJIT.h"

#include "rphp/pConfig.h"
#include "rphp/runtime/pRuntimeEngine.h"
#include "rphp/runtime/pErrorManager.h"
#include "rphp/IR/pCompileTarget.h"
#include "rphp/JIT/pJITTarget.h"

#include <llvm/Module.h>
#include <boost/bind.hpp>
#include <string>


namespace rphp {

pCachedJIT::pCachedJIT(pConfig* config):
    pTarget(config),
    runtime_(new pRuntimeEngine(config))
{

    // bind the notify handler to our runtime's error manager
    setNotifyHandler(boost::bind(&pErrorManager::notify, runtime_->errorManager, _1, _2));

    // XXX temporary
    std::string outputEncoding(config->get("outputEncoding"));
    if (!outputEncoding.empty()) {
        runtime_->unicode().setOutputEncoding(outputEncoding);
    }

}


pCachedJIT::~pCachedJIT(void) {
    delete runtime_;
}

void pCachedJIT::cacheAndJITFileOnDisk(const pSourceFileDesc& sourceFile) {

    pCompileTarget cTarget(sourceFile);
    cTarget.configureWith(this);
    cTarget.execute();

    llvm::Module* m = cTarget.releaseModuleOwnership();

    // TODO: caching mechanism. no problem.

    pJITTarget engine(runtime_, m, cTarget.entryFunctionName());

    engine.configureWith(this);

    // note: this may throw a runtime error, but we allow that to be caught higher up
    engine.execute();

    // segfault? who owns this?
    //delete m;


}

}

