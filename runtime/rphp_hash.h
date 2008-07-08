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

#ifndef RPHP_HASH_H_
#define RPHP_HASH_H_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include <iostream>

#include "rphp_pvar.h"

using boost::multi_index_container;
using namespace boost::multi_index;

namespace rphp {

struct _dataContainer {

    pvar data;
    bstring key;
    bool isNumKey;

    _dataContainer(bstring k, pvar d) : data(d), key(k), isNumKey(false) {
        // TODO: set isNumKey based on isNumeric check on key
        // we will need this to emulate their numeric keys
    }

};

// a boost.multiindex that stores data with two indexes: hashed and sequenced
typedef multi_index_container<
  _dataContainer,
  indexed_by<
    hashed_unique< member<_dataContainer, bstring, &_dataContainer::key> >,
    sequenced<>
  >
> stableHash;

// sequenced index accessor
typedef nth_index<stableHash,1>::type seq_index;

class phash {
    private:
        stableHash hashData;
    public:
        
        phash() {
            hashData.insert(_dataContainer("foo", pvar(pint(5))));
            hashData.insert(_dataContainer("bar", pvar(bstring("some string val"))));
            hashData.insert(_dataContainer("baz", pvar(pfloat(5.3212))));
        }

        void insert(const bstring &key, pvar data) {

            hashData.insert(_dataContainer(key, data));

        }

        void varDump() {
            std::cout << "array(" << hashData.size() << ") {" << std::endl;
            seq_index& ot = get<1>(hashData);
            for (seq_index::iterator it = ot.begin(); it!=ot.end(); it++) {
                std::cout << "   ['" << (*it).key << "'] => " << (*it).data << std::endl;
            }
            std::cout << "}" << std::endl;
        }
        
        int getSize() const { return hashData.size(); }
        
        ~phash() { std::cout << "destorying php_hash" << std::endl; }

};

std::ostream& operator << (std::ostream& os, const rphp::phash& h)
{
     return os << "php_hash:" << std::endl;
}

} /* namespace rphp */

#endif /* RPHP_HASH_H_ */
