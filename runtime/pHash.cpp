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

#include <iostream>
#include "pHash.h"


namespace boost {
    std::size_t hash_value(rphp::hKeyVar const& k) {
        return boost::apply_visitor(rphp::hKeyHasher(), k);
    }
}

namespace rphp {
    
    void pHash::insert(const pUString &key, pVarP data) {
        // TODO check numeric string, set maxIntKey accordingly
        hashData.insert(h_container(key, data));
    }

    void pHash::insert(const pInt &key, pVarP data) {
        if (key > maxIntKey)
            maxIntKey = key+1;
        hashData.insert(h_container(key, data));
    }
    
    void pHash::insertNext(pVarP data) {
        hashData.insert(h_container(maxIntKey++, data));
    }
    
    void pHash::varDump() {


        std::cout << "array(" << hashData.size() << ") {" << std::endl;

        seq_index& ot = get<1>(hashData);

        hKeyType kType;
        
        for (seq_index::iterator it = ot.begin(); it!=ot.end(); it++) {
            kType = boost::apply_visitor(rphp::hKeyGetType(), (*it).key);
            if (kType == hKeyInt)
                std::cout << "   [" << (*it).key << "] => " << *(*it).pData << std::endl;
            else
                std::cout << "   ['" << (*it).key << "'] => " << *(*it).pData << std::endl;
        }

        std::cout << "}" << std::endl;

    }

    std::ostream& operator << (std::ostream& os, const rphp::pHash& h)
    {
        return os << "php_hash:" << std::endl;
    }

} /* namespace rphp */

