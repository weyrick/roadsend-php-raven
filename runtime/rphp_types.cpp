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

#include "rphp_types.h"

namespace rphp {

// non destructive cast (explicit copy)
pvar pvar_castToNumber(const pvar p) {

    pvar r = p;
    boost::apply_visitor(convertToNumber(r), r);
    return r;

}

// TODO: belongs in rphp_operators.cpp
pvar pvar_add(const pvar lhs, const pvar rhs)
{
    pvar l,r,result;

    pvarType lhs_type = pvar_getType(lhs);
    pvarType rhs_type = pvar_getType(rhs);
    if ( (lhs_type == PVAR_HASH) && (rhs_type == PVAR_HASH) ) {
        //std::cout << "fixme: concat hashes" << std::endl;
        result = 0l;
    }
    else {
        // convert to number, then add
        l = pvar_castToNumber(lhs);
        //std::cout << "pvar_add: l is " << l << std::endl;
        r = pvar_castToNumber(rhs);
        //std::cout << "pvar_add: r is " << r << std::endl;
        result = pvar_getVal_int(l) + pvar_getVal_int(r);
        //std::cout << "pvar_add: result is " << result << std::endl;
    }

    return result;
}


} /* namespace rphp */

