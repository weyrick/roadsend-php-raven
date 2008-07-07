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
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include <iostream>

using boost::multi_index_container;
using namespace boost::multi_index;

typedef multi_index_container<
  std::string,
  indexed_by<
    hashed_unique<identity<std::string> >,
    sequenced<>
  >
> stableHash;

typedef nth_index<stableHash,1>::type seq_hash;

namespace rphp {

// XXX placeholder
class phash {
    private:
        int size;
        stableHash hashData;
    public:
        phash(int sizevar) : size(sizevar) {
            hashData.insert("foo");
            hashData.insert("bar");
            hashData.insert("baz");
        }

        phash(const phash& h) {
            size = h.size;
        }
        void dump() {
            std::cout << "hash has " << hashData.size() << " elements" << std::endl;
            seq_hash& ot = get<1>(hashData);
            for (seq_hash::iterator it = ot.begin(); it!=ot.end(); it++) {
                std::cout << *it << std::endl;
            }
        }
        int getSize() const { return size; }
        ~phash() { std::cout << "destorying php_hash" << std::endl; }

};

std::ostream& operator << (std::ostream& os, const rphp::phash& h)
{
     return os << "php_hash:" << std::endl;
}

} /* namespace rphp */

#endif /* RPHP_HASH_H_ */
