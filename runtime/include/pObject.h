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

#include <iostream>
#include <unicode/unistr.h>
#include "pHash.h"

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

class pMethod {
    // method name, case sensitive (as declared)
    // canonical name, lowercased
    // origin class: original defining class pClass*, used in inheritance
    // final, abstract flags
    // function pointer (common class with normal functions?)
};

class pClass {
private:
    pHash properties_;
    pHash methods_; // TODO: this can be unordered map
    pUString name_;
public:
    pClass();
    pHash properties();
        // bitset of class flags: abstract, final, interface, abstract-implied
        // class name, case sensitive (as declared)
    const pUString& getName() const;
    void setName( const pUString& name );
        // canonical name, lowercased
        // list of parent classes (only 1, unless interface)
        // list of interfaces the class implements
        // constructor, destructor, method array
        // declared properties (including visibility, static info)
        // class constants

};

class pObject {
    private:
        pHash properties_; // copied from declared
        const pClass* parentClass_;
        pHash runtimeFunctions_;
        pClass* class_;
        // object instance id
        // hash for properties created on the fly
    public:
        pObject(const pUString& className);

        pHash::size_type getNumProperties() const {
            return properties_.getSize();
        }

};

std::ostream& operator << (std::ostream& os, const rphp::pObject& h);

}

#endif /* RPHP_POBJECT_H_ */
