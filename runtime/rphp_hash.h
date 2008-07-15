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

// hash function for use with rphp::pUString
U_NAMESPACE_BEGIN
    std::size_t hash_value(rphp::pUString const& s);
U_NAMESPACE_END

using boost::multi_index_container;
using namespace boost::multi_index;

namespace rphp {

    /*
     * defines an interface to a hash table class which implements php style
     * hash table/array semantics
     *
     */

    // container stored in stableHash
    struct h_container {

        pVarP data;
        const pUString key;
        // TODO: php has support for numeric keys

        h_container(const pUString k, pVarP d) : data(d), key(k) { }

    };

    // the stableHash container
    // a boost.multiindex that stores data with two indexes: hashed and sequenced
    typedef multi_index_container<
        // the container structure we store for each item in the hash
        h_container,
        // index definitions: hash and sequence
        indexed_by<
            hashed_unique< member<h_container, const pUString, &h_container::key> >,
            sequenced<>
        >
    > stableHash;

    // sequenced index accessor
    typedef nth_index<stableHash,1>::type seq_index;

    /**
     * pHash definition
     *
     */
    class pHash {

        private:
            stableHash hashData;

        public:

            // types
            typedef stableHash::size_type size_type;

            // construct/destroy/copy
            pHash() { std::cout << "creating fresh pHash" << std::endl; }

            pHash(pHash const& p) {
                std::cout << "pHash copy construct" << std::endl;
                hashData = p.hashData;
            }

            ~pHash() { std::cout << "destroying pHash" << std::endl; }

            // modifiers
            void insert(const pUString &key, pVarP data);

            // size
            const size_type getSize() { return hashData.size(); }

            // dump of contents
            void varDump();

            // lookup
            pVarP operator[] ( const pUString &key ) {
                stableHash::iterator k = hashData.find(key);
                if (k == hashData.end())
                    return pVarP();
                else
                    return (*k).data;
            }


    };

    // stream interface
    std::ostream& operator << (std::ostream& os, const rphp::pHash& h);

} /* namespace rphp */

#endif /* RPHP_HASH_H_ */
