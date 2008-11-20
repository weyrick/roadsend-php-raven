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

#include <llvm/Module.h>
#include <llvm/ModuleProvider.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Bitcode/ReaderWriter.h>

#include <iostream>
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


llvm::Module* pGenSupport::readBitcode(std::string fileName) {

    std::string errMsg;
    llvm::MemoryBuffer* mb = llvm::MemoryBuffer::getFile(fileName.c_str(), &errMsg);
    if (errMsg.length()) {
        // TODO: errors
        std::cerr << "error loading runtime IR file [" << fileName << "]: " << errMsg << std::endl;
        exit(1);
    }

    llvm::ModuleProvider* mp = llvm::getBitcodeModuleProvider(mb, &errMsg);
    if (errMsg.length()) {
        // TODO: errors
        std::cerr << "error parsing bitcode file [" << fileName << "]: " << errMsg << std::endl;
        exit(1);
    }

    llvm::Module* mod =  mp->getModule();

    // caller takes control of module
    mp->releaseModule();
    delete mp;

    return mod;

}

llvm::Module* pGenSupport::getRuntimeIR() {

    // TODO: ouch!
    return readBitcode("../runtime-ir/irRuntime.ir");

}


} // namespace

