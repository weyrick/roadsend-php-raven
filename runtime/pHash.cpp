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

U_NAMESPACE_BEGIN
    // this is used by the multi_index for hashing unicode strings
    std::size_t hash_value(rphp::pUString const& s) {
        return static_cast<std::size_t>(s.hashCode());
    }
U_NAMESPACE_END

namespace rphp {

    void pHash::insert(const pUString &key, pVarP data) {

        hashData.insert(h_container(key, data));

    }

    void pHash::varDump() {


        std::cout << "array(" << hashData.size() << ") {" << std::endl;

        seq_index& ot = get<1>(hashData);

        for (seq_index::iterator it = ot.begin(); it!=ot.end(); it++) {
            std::cout << "   ['" << (*it).key << "'] => " << *(*it).data << std::endl;
        }

        std::cout << "}" << std::endl;

    }

    std::ostream& operator << (std::ostream& os, const rphp::pHash& h)
    {
        return os << "php_hash:" << std::endl;
    }

} /* namespace rphp */

