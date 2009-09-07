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

#include "rphp/runtime/pVar.h"
#include "rphp/runtime/pOutputBuffer.h"

#include <boost/variant.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <iostream>

using boost::multi_index_container;
using namespace boost::multi_index;

/// custom mutli index key type
namespace rphp {
    typedef boost::variant< pInt, pBString, pUStringP > hKeyVar;
    typedef enum { hKeyInt=0, hKeyBStr=1, hKeyUStr=2  } hKeyType;
}

/// customer key hasher
namespace boost {
    std::size_t hash_value(const rphp::hKeyVar& k);
}

namespace rphp {

/// a visitor for hashing phash keys
class hKeyHasher : public boost::static_visitor<std::size_t> {

public:

    std::size_t operator()(pInt k) const {
        return static_cast<std::size_t>(k);
    }

    std::size_t operator()(const pBString &k) const {
        return boost::hash_value(k);
    }

    std::size_t operator()(const pUStringP &k) const {
        return static_cast<std::size_t>(k->hashCode());
    }

};

/// the container stored in stableHash
struct h_container {

    pVar pData;
    hKeyVar key;

    h_container(pInt k, const pVar& d) : pData(d), key(k) { }
    h_container(const pBString& k, const pVar& d) : pData(d), key(k) { }
    h_container(const pUStringP& k, const pVar& d) : pData(d), key(k) { }

};

/**
 the stableHash container
 a boost.multiindex that stores data with two indexes: hashed and sequenced
*/
typedef multi_index_container<
    // the container structure we store for each item in the hash
    h_container,
    // index definitions: hash and sequence
    indexed_by<
        hashed_unique< member<h_container, hKeyVar, &h_container::key> >,
        sequenced<>
    >
> stableHash;

/// sequenced index accessor
typedef nth_index<stableHash,1>::type seq_index;

/**
* pHash definition
*/
class pHash {

public:

    typedef stableHash::size_type size_type;
    typedef stableHash::iterator iterator;

private:
    /// maximum (signed) integer used as a next key so far
    /// this is signed because the runtime type is too
    pInt maxIntKey_;
    /// the actual stable hash data
    stableHash hashData_;

public:

    /// construct an empty hash
    pHash() : maxIntKey_(0) {
#ifdef RPHP_PVAR_DEBUG
    std::cerr << "pHash(): create" << std::endl;
#endif
    }

    // NOTE: default copy constructor, destructor

#ifdef RPHP_PVAR_DEBUG
    pHash(pHash const& p) : maxIntKey_(p.maxIntKey_), hashData_(p.hashData_) {
        std::cerr << "pHash(): copy construct (full data copy)" << std::endl;
    }

    ~pHash() {
        std::cerr << "pHash(): destruct" << std::endl;
    }
#endif

    // modifiers
    void insert(pInt key, const pVar& data);
    void insert(const pBString& key, const pVar& data);
    void insert(const pUStringP& key, const pVar& data);
    void insert(const pVar& key, const pVar& data);
    void insert(const char* key, const pVar& data) {
        // TODO need to check runtime for default string type
        insert(pBString(key), data);
    }
    void insertNext(const pVar& data);

    size_type remove(pInt key);
    size_type remove(const pBString& key);
    size_type remove(const pUStringP& key);
    size_type remove(const char* key);

    // queries
    size_type size() const { return hashData_.size(); }

    bool keyExists(pInt key) const;
    bool keyExists(const pBString& key) const;
    bool keyExists(const pUStringP& key) const;

    // lookup
    pVar operator[] (pInt key) const;
    pVar operator[] (const pBString& key) const;
    pVar operator[] (const pUStringP& key) const;
    pVar operator[] (const char* key) const;

    // dump of contents
    void varDump(pOutputBuffer*, const pBString& indent) const;

};

// stream interface
std::ostream& operator << (std::ostream& os, const rphp::pHash& h);

} /* namespace rphp */

#endif /* RPHP_PHASH_H_ */
