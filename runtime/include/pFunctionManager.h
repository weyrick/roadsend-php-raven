/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
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

#ifndef RPHP_PFUNCTIONMANAGER
#define RPHP_PFUNCTIONMANAGER

#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <ext/hash_map>
#include "pTypeOperators.h"
#include "pFunctionSig.h"

using boost::multi_index_container;
using namespace boost::multi_index;

namespace rphp {

    struct functionEntry {
        pUString canonicalName;
        pFunctionSig* signature;
        functionEntry(pUString name, pFunctionSig* sig) : canonicalName(name.toLower()), signature(sig) { }
    };

    typedef multi_index_container<
      functionEntry,
      indexed_by<
       hashed_unique <
        BOOST_MULTI_INDEX_MEMBER(functionEntry, const pUString, canonicalName)
       >
      >
    > functionRegistryType;

    class pRuntimeEngine;

    class pFunctionManager {

        private:
            pRuntimeEngine* runtime;
            functionRegistryType functionRegistry;

        public:

            pFunctionManager(pRuntimeEngine *r) : runtime(r) { }
            ~pFunctionManager() {
                // TODO delete function signatures
            }

            void registerBuiltin(const pExtBase*, const pUString&, const pFunPointer1&);

            pVar invoke(pUString funName, pVar arg1) {
                functionRegistryType::iterator function = functionRegistry.find(funName.toLower());
                // TODO this needs to throw a runtime error if the function wasn't found
                if (function != functionRegistry.end()) {
                    return (*function).signature->invoke(arg1);
                }
                else {
                    return pNull;
                }
            }

    };

}

#endif /* RPHP_PFUNCTIONMANAGER_ */
