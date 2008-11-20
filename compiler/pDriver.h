/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PDRIVER_H_
#define RPHP_PDRIVER_H_

#include <string>
#include "pCompilerTypes.h"

namespace llvm {
    class ModuleProvider;
}

namespace rphp {

class pModule;

class pDriver {

    bool JITmodule(llvm::ModuleProvider* MP, std::string entryFunction);

public:

    bool executeBC(std::string fileName);
    bool executeModule(pModule* pMod);

    void dumpTokens(pSourceFileDesc file);
    void dumpAST(pSourceFileDesc file);
    void dumpIR(pSourceFileDesc file);
    void dumpPre(pSourceFileDesc file);

};

}

#endif /* RPHP_PDRIVER_H_ */
