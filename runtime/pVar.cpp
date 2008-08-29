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


#include "pVar.h"
#include "pTypeOperators.h"

namespace rphp {

const pVarType pVar::getType() const {
    int w = pVarData_.which();
    if (w == pVarTriStateType_) {
        return (pNull(boost::get<pTriState>(pVarData_))) ? pVarNullType : pVarBoolType;
    }
    else {
        return (pVarType)w;
    }
}

pInt& pVar::convertToInt() {
    pVar_convertToIntVisitor cv(pVarData_);
    boost::apply_visitor(cv, pVarData_);
    return getInt();
}

pBString& pVar::convertToBString() {
    pVar_convertToBStringVisitor cv(pVarData_);
    boost::apply_visitor(cv, pVarData_);
    return getBString();
}

pInt pVar::copyAsInt() const {
    pVar v(*this);
    return v.convertToInt();
}

pBString pVar::copyAsBString() const {
    pVar v(*this);
    return v.convertToBString();
}

std::ostream& operator << (std::ostream& os, const pVar& v)
{
    return os << v.pVarData_ << std::endl;
}

} /* namespace rphp */
