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

#include <llvm/Module.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <fstream>

#include "pGenSupport.h"

namespace rphp {

std::string pGenSupport::mangleModuleName(std::string inName) {

    // TODO: mangle
    return inName;

}

bool pGenSupport::writeBitcode(llvm::Module* m, std::string outFile) {

    assert(m != NULL);
    assert(outFile.length() > 0);

    // TODO: real error handling
    std::ofstream OS(outFile.c_str(), std::ios_base::out|std::ios::trunc|std::ios::binary);
    if (!OS.fail()) {
        llvm::WriteBitcodeToFile(m, OS);
        return true;
    }
    else {
        return false;
    }

}


} // namespace

