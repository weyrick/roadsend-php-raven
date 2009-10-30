/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
 *               2008 Thomas Moenicke <tm@php-qt.org>
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

#ifndef RPHP_POBJECT_H_
#define RPHP_POBJECT_H_

#include "rphp/runtime/pHash.h"
#include "rphp/runtime/pRuntimeTypes.h"

#include <boost/unordered_map.hpp>
#include <iostream>
#include <vector>

/*
; PHP5 case sensitivity:
;  - class names are NOT case sensitive
;  - method names are NOT case sensitive
;  - properties ARE case sensitive
;
; regardless of whether it is case sensitive, each item must
; know the case it was defined as, because it's shown that way
; by various reporting functions
*/

namespace rphp {

class pFunction;
class pClass;

class pMethod {

    const static int FINAL    = 0x01;
    const static int ABSTRACT = 0x02;

    pIdentString name_;          // method name, case sensitive (as declared)
                                 // canonical name, lowercased XXX needed?
    pClass* definingClass_;      // origin class: original defining class pClass*, used in inheritance
    boost::uint_fast8_t flags_;  // final, abstract flags
    pFunction* signature_;       // function pointer

};

typedef boost::unordered_map<pIdentString, pMethod*> methodRegistryType;

class pClass {
private:

    const static int ABSTRACT  = 0x01;
    const static int FINAL     = 0x02;
    const static int INTERFACE = 0x03;
    const static int ABSTRACT_IMPLIED = 0x04;

    pHash properties_;                // declared properties
    methodRegistryType methods_;      // declared methods
    pIdentString name_;               // class name, case sensitive (as declared)
    boost::uint_fast8_t flags_;       // class flags: abstract, final, interface, abstract-implied
    std::vector<pClass*> extends_;    // list of parent classes (only 1, unless interface)
    std::vector<pClass*> implements_; // list of interfaces the class implements
    pHash constants_;                 // class constants

public:
    pClass();
    pHash properties();
    const pIdentString& name() const;
    void setName( const pIdentString& name );

};

class pObject {
    private:
        pHash properties_; // copied from declared
        pHash runtimeProperties_; // hash for properties created on the fly
        pClass* class_;  // which class we instantiate
        pUInt instance_; // object instance id
    public:
        pObject(const pIdentString& className);

        pHash::size_type numProperties() const {
            return properties_.size() + runtimeProperties_.size();
        }

};

std::ostream& operator << (std::ostream& os, const rphp::pObject& h);

}

#endif /* RPHP_POBJECT_H_ */
