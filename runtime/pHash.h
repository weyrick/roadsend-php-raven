/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PHASH_H_
#define RPHP_PHASH_H_

#include <boost/variant.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <iostream>

#include "pVar.h"

using boost::multi_index_container;
using namespace boost::multi_index;

// custom key type
namespace rphp {
    typedef boost::variant< pInt, pIdentString > hKeyVar;
    typedef enum { hKeyInt, hKeyStr  } hKeyType;
}

// customer key hasher
namespace boost {
    std::size_t hash_value(rphp::hKeyVar const& k);
}

namespace rphp {

// a visitor for hashing phash keys
class hKeyHasher : public boost::static_visitor<std::size_t> {

public:

    std::size_t operator()(const pInt &k) const {
        return static_cast<std::size_t>(k);
    }

    std::size_t operator()(const pIdentString &k) const {
        return boost::hash_value(k);
        //return static_cast<std::size_t>(k.hashCode());
    }

};

// define the container stored in stableHash
struct h_container {

    pVar pData;
    hKeyVar key;

    h_container(const pIdentString k, pVar d) : pData(d), key(k) { }

    h_container(const pInt k, pVar d) : pData(d), key(k) { }

};

// the stableHash container
// a boost.multiindex that stores data with two indexes: hashed and sequenced
typedef multi_index_container<
    // the container structure we store for each item in the hash
    h_container,
    // index definitions: hash and sequence
    indexed_by<
        hashed_unique< member<h_container, hKeyVar, &h_container::key> >,
        sequenced<>
    >
> stableHash;

// sequenced index accessor
typedef nth_index<stableHash,1>::type seq_index;

/**
* pHash definition
*/
class pHash {

    stableHash hashData_;
    pInt maxIntKey_;

public:

    // types
    typedef stableHash::size_type size_type;

    typedef stableHash::iterator iterator;

    // construct/destroy/copy
    pHash() : maxIntKey_(0) {
#ifdef RPHP_PVAR_DEBUG
    std::cerr << "pHash(): create" << std::endl;
#endif
    }

    // default copy construtor, destructor

#ifdef RPHP_PVAR_DEBUG
    pHash(pHash const& p) : maxIntKey_(p.maxIntKey_), hashData_(p.hashData_) {
        std::cerr << "pHash(): copy construct (full data copy)" << std::endl;
    }

    ~pHash() {
        std::cerr << "pHash(): destruct" << std::endl;
    }
#endif

    // modifiers
    void insert(const pIdentString &key, const pVar& data);
    void insert(const pInt &key, const pVar& data);
    void insertNext(const pVar& data);

    size_type remove(const pIdentString &key);
    size_type remove(const pInt &key);

    // queries
    size_type size() const { return hashData_.size(); }
    bool keyExists(const pIdentString &key) const;
    bool keyExists(const pInt &key) const;

    // dump of contents
    void varDump() const;

    // lookup
    pVar operator[] (const pIdentString &key) const;
    pVar operator[] (const pInt &key) const;


};

// stream interface
std::ostream& operator << (std::ostream& os, const rphp::pHash& h);

} /* namespace rphp */

#endif /* RPHP_PHASH_H_ */
