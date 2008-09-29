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

#ifndef RPHP_PIRTYPES_H_
#define RPHP_PIRTYPES_H_

#include <vector>

namespace llvm {
    class FunctionType;
    class Type;
}

namespace rphp {

class pIRTypes {

    llvm::FunctionType* moduleEntryFunTypeC;

public:

    pIRTypes(void): moduleEntryFunTypeC(0) { }

    // pointer to the runtime engine
    llvm::Type* runtimeEngineType(void);

    // entry function for a single compiled php module
    llvm::FunctionType* moduleEntryFunType(void);

};


} // namespace

#endif /* RPHP_PIRTYPES_H_ */
