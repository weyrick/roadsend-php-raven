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


#include "rphp/runtime/pHash.h"

#include "rphp/runtime/pVarOperators.h"

#include <iostream>

namespace boost {
std::size_t hash_value(rphp::hKeyVar const& k) {
    return boost::apply_visitor(rphp::hKeyHasher(), k);
}
}

namespace rphp {

void pHash::insert(pInt key, const pVar& data) {
    if (key > maxIntKey_)
        maxIntKey_ = key+1;
    h_container dataKeyed(key, data);
    stableHash::iterator k = hashData_.find(key);
    if(k == hashData_.end())
    	hashData_.insert(dataKeyed);
    else
    	assert(hashData_.replace(k, dataKeyed));
}

void pHash::insert(const pBString& key, const pVar& data) {
    // TODO check numeric string, set maxIntKey accordingly
    h_container dataKeyed(key, data);
    stableHash::iterator k = hashData_.find(key);
    if(k == hashData_.end())
    	hashData_.insert(dataKeyed);
    else
    	assert(hashData_.replace(k, dataKeyed));
}

void pHash::insert(const pUStringP& key, const pVar& data) {
    // TODO check numeric string, set maxIntKey accordingly
    h_container dataKeyed(key, data);
    stableHash::iterator k = hashData_.find(key);
    if(k == hashData_.end())
    	hashData_.insert(dataKeyed);
    else
    	assert(hashData_.replace(k, dataKeyed));
}

void pHash::insert(const pVar& key, const pVar& data) {
    // TODO proper semantics for key conversion
	if(key.isInt())
		insert(key.getInt(), data);
	else if(key.isBool())
		insert(pInt(key.evalAsBool()), data);
	else if(key.isBString())
		insert(key.getBString(), data);
	else if(key.isUString())
		insert(key.getUStringP(), data);
	if(!key.isInt() && !key.isBool() && !key.isBString() && !key.isUString())
		assert(0 && "unsupported type for array key detected!");
    //hashData_.insert(h_container(key.copyAsBString(), data));
}

void pHash::insertNext(const pVar& data) {
	pInt key(maxIntKey_++);
    h_container dataKeyed(key, data);
    stableHash::iterator k = hashData_.find(key);
    if(k == hashData_.end())
    	hashData_.insert(dataKeyed);
    else
    	assert(hashData_.replace(k, dataKeyed));
}

pHash::size_type pHash::remove(pInt key) {
    return hashData_.erase(key);
}

pHash::size_type pHash::remove(const pBString& key) {
    return hashData_.erase(key);
}

pHash::size_type pHash::remove(const pUStringP& key) {
    return hashData_.erase(key);
}

pHash::size_type pHash::remove(const char* key) {
    return hashData_.erase(pBString(key));
}

// query
bool pHash::keyExists(pInt key) const {
    stableHash::iterator k = hashData_.find(key);
    return (k != hashData_.end());
}

bool pHash::keyExists(const pBString& key) const {
    stableHash::iterator k = hashData_.find(key);
    return (k != hashData_.end());
}

bool pHash::keyExists(const pUStringP& key) const {
    stableHash::iterator k = hashData_.find(key);
    return (k != hashData_.end());
}

// lookup
pVar pHash::operator[] (pInt key) const {
    stableHash::iterator k = hashData_.find(key);
    if (k == hashData_.end())
        return pNull;
    else
        return (*k).pData;
}

pVar pHash::operator[] (const pBString& key) const {
    stableHash::iterator k = hashData_.find(key);
    if (k == hashData_.end())
        return pNull;
    else
        return (*k).pData;
}

pVar pHash::operator[] (const pUStringP& key) const {
    stableHash::iterator k = hashData_.find(key);
    if (k == hashData_.end())
        return pNull;
    else
        return (*k).pData;
}

pVar pHash::operator[] (const char* key) const {
    stableHash::iterator k = hashData_.find(pBString(key));
    if (k == hashData_.end())
        return pNull;
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
    hKeyType kType;

    for (seq_index::iterator it = ot.begin(); it!=ot.end(); it++) {
        kType = (hKeyType)(*it).key.which();
        if (kType == hKeyInt) {
            *buf << newIndent << "[" << (*it).key << "]=>\n";
        }
        else {
            if (kType == hKeyUStr)
                *buf << newIndent << "[u\"" << (*it).key << "\"]=>\n";
            else
                *buf << newIndent << "[\"" << (*it).key << "\"]=>\n";
        }
        (*it).pData.applyVisitor<pVar_var_dump>(buf, newIndent);
    }

    *buf << indent << "}\n";

}

} /* namespace rphp */

