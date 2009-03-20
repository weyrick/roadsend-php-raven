/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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


#include "rphp/runtime/pObject.h"

#include <iostream>

namespace rphp {

std::ostream& operator << (std::ostream& os, const rphp::pObject& h) {
    return os << "pobject" << std::endl;
}

pObject::pObject( const pIdentString& className )
: properties_()
{
    // find pClass in class hash table and associate it with the object
    //  class_ = rphp::pRuntime::getClass( className );
    // copy properties
    properties_ = class_->properties();
}

}


