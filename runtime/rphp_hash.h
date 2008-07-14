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

// hash function for rphp::ustring
namespace icu_3_8 {
    std::size_t hash_value(rphp::ustring const& s);
}

namespace rphp {

// container we store in our stable hash
struct h_container {

    pvarP data;
    const ustring key;
    bool isNumKey;

    h_container(const ustring k, pvarP d) : data(d), key(k), isNumKey(false) {
        // TODO: set isNumKey based on isNumeric check on key
        // we will need this to emulate their numeric keys
    }

};

// a boost.multiindex that stores data with two indexes: hashed and sequenced
typedef multi_index_container<
  // the container structure we store for each item in the hash
  h_container,
  // index definitions: hash and sequence
  indexed_by<
    hashed_unique< member<h_container, const ustring, &h_container::key> >,
    sequenced<>
  >
> stableHash;

// sequenced index accessor
typedef nth_index<stableHash,1>::type seq_index;

class phash {
    private:
        stableHash hashData;
    public:

        typedef stableHash::size_type size_type;
        
        phash() { std::cout << "creating fresh phash" << std::endl; }

        phash(phash const& p) {
            std::cout << "phash copy construct" << std::endl;
            hashData = p.hashData;
        }

        void insert(const ustring &key, pvarP data);

        void varDump();
        
        const size_type getSize() { return hashData.size(); }
        
        pvarP operator[] ( const ustring &key ) {
            stableHash::iterator k = hashData.find(key);
            if (k == hashData.end())
                return pvarP();
            else
                return (*k).data;
        }

        
        ~phash() { std::cout << "destroying phash" << std::endl; }

};

std::ostream& operator << (std::ostream& os, const rphp::phash& h);

} /* namespace rphp */

#endif /* RPHP_HASH_H_ */
