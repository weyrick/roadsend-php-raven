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

#include <boost/lexical_cast.hpp>

#include "pTypeOperators.h"

/*
   This defines a boost compatible hash function for UnicodeString
*/
U_NAMESPACE_BEGIN
std::size_t hash_value(const rphp::pUString &v) {
    return (std::size_t)v.hashCode();
}
U_NAMESPACE_END

namespace rphp {

void pVar_convertToNumberVisitor::operator()(pBString &v) {
    // TODO: handle floats
    try {
        var = boost::lexical_cast<pInt>(v);
    } catch(boost::bad_lexical_cast &) {
        var = 0l;
    }
}

void pVar_convertToNumberVisitor:: operator()(pUStringP &v) {
    // TODO: do a real conversion here
    // should handle both integers and floats
    var = 0l;
}

pVar pVar_castToNumber(const pVar &v) {
    pVar r(v);
    pVar_convertToNumberVisitor cv(r);
    boost::apply_visitor(cv, r);
    return r;
}

pVar pVar_castToBString(const pVar &v) {
    pVar r(v);
    pVar_convertToBStringVisitor cv(r);
    boost::apply_visitor(cv, r);
    return r;
}

} /* namespace rphp */

