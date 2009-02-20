/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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
#include <unicode/ustream.h>

#include "rphp/runtime/pHash.h"
#include "rphp/runtime/pVarOperators.h"

namespace boost {
std::size_t hash_value(rphp::hKeyVar const& k) {
    return boost::apply_visitor(rphp::hKeyHasher(), k);
}
}

namespace rphp {

void pHash::insert(const pVar& key, const pVar& data) {
    // TODO proper semantics for key conversion
    hashData_.insert(h_container(key.copyAsBString(), data));
}

void pHash::insert(const pIdentString &key, const pVar& data) {
    // TODO check numeric string, set maxIntKey accordingly
    hashData_.insert(h_container(key, data));
}

void pHash::insert(const pInt &key, const pVar& data) {
    if (key > maxIntKey_)
        maxIntKey_ = key+1;
    hashData_.insert(h_container(key, data));
}

void pHash::insertNext(const pVar& data) {
    hashData_.insert(h_container(maxIntKey_++, data));
}

pHash::size_type pHash::remove(const pIdentString &key) {
    return hashData_.erase(key);
}

pHash::size_type pHash::remove(const pInt &key) {
    return hashData_.erase(key);
}


// query
bool pHash::keyExists(const pIdentString &key) const {
    stableHash::iterator k = hashData_.find(key);
    return (k != hashData_.end());
}

bool pHash::keyExists(const pInt &key) const {
    stableHash::iterator k = hashData_.find(key);
    return (k != hashData_.end());
}

// lookup
pVar pHash::operator[] ( const pIdentString &key ) const {
    stableHash::iterator k = hashData_.find(key);
    if (k == hashData_.end())
        return pVar(); // pNull
    else
        return (*k).pData;
}

pVar pHash::operator[] ( const pInt &key ) const {
    stableHash::iterator k = hashData_.find(key);
    if (k == hashData_.end())
        return pVar(); // pNull
    else
        return (*k).pData;
}

// output
std::ostream& operator << (std::ostream& os, const rphp::pHash& h)
{
    return os << "php_hash:" << std::endl;
}

void pHash::varDump(pOutputBuffer* buf, const pBString& indent) const {

    *buf << indent << "array(" << hashData_.size() << ") {\n";

    const seq_index& ot = get<1>(hashData_);

    pBString newIndent(indent+"  ");

    for (seq_index::iterator it = ot.begin(); it!=ot.end(); it++) {
        if ((*it).key.which() == hKeyInt) {
            *buf << newIndent << "[" << (*it).key << "]=>\n";
        }
        else {
            *buf << newIndent << "['" << (*it).key << "']=>\n";
        }
        (*it).pData.applyVisitor<pVar_var_dump>(buf, newIndent);
    }

    *buf << indent << "}\n";

}

} /* namespace rphp */

