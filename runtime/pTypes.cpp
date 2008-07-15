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

#include "pTypes.h"

namespace rphp {

    // non destructive cast (explicit copy)
    pVar pVar_castToNumber(const pVar p) {

        pVar r = p;
        boost::apply_visitor(convertToNumber(r), r);
        return r;

    }

    // TODO: belongs in rphp_operators.cpp
    pVar pVar_add(const pVar lhs, const pVar rhs)
    {
        pVar l,r,result;

        pVarType lhs_type = pVar_getType(lhs);
        pVarType rhs_type = pVar_getType(rhs);
        if ( (lhs_type == pVarHashType) && (rhs_type == pVarHashType) ) {
            //std::cout << "fixme: concat hashes" << std::endl;
            result = 0l;
        }
        else {
            // convert to number, then add
            l = pVar_castToNumber(lhs);
            //std::cout << "pVar_add: l is " << l << std::endl;
            r = pVar_castToNumber(rhs);
            //std::cout << "pVar_add: r is " << r << std::endl;
            result = pVar_getVal_int(l) + pVar_getVal_int(r);
            //std::cout << "pVar_add: result is " << result << std::endl;
        }

        return result;
    }


} /* namespace rphp */

