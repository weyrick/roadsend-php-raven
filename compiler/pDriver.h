/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
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

namespace llvm {
    class Module;
    class ModuleProvider;
}

namespace rphp {

class pModule;

class pDriver {
private:
    std::string readFile(std::string fileName);

    void JITmodule(llvm::ModuleProvider* MP, std::string entryFunction);

public:

    pModule* createModule(std::string fileName);

    void execute(std::string fileName);
    void executeBC(std::string fileName);
    void executePHP(std::string fileName);
    void executeModule(pModule* mod);

    void dumpTokens(std::string fileName);
    void dumpAST(std::string fileName);
//        void dumpIR(std::string fileName);

};

}

#endif /* RPHP_PDRIVER_H_ */
