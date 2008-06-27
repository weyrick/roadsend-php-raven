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

#ifndef RPHP_OBJECT_H_
#define RPHP_OBJECT_H_

#include "rphp_hash.h"
#include <iostream>

namespace rphp {

// XXX placeholder
class php_object {
    private:
        php_hash properties;
    public:
        php_object() : properties(5) { }

};


std::ostream& operator << (std::ostream& os, const rphp::php_object& h)
{
    return os << "php_object" << std::endl;
}

}

#endif /* RPHP_OBJECT_H_ */
