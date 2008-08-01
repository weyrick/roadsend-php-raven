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

#ifndef RPHP_PTYPES_H_
#define RPHP_PTYPES_H_

#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>

#include "pVar.h"
#include "pHash.h"
#include "pObject.h"

U_NAMESPACE_BEGIN
    // boost hash function for pUString
    std::size_t hash_value(rphp::pUString const& k);
U_NAMESPACE_END

namespace rphp {

    // types used in the runtime which aren't related to pVar (i.e. that aren't builtin php types)

    // note, pUInt is not a base PHP type (all PHP numbers are signed)
    typedef unsigned long pUInt;

    // source locations: filename/linenum
    typedef boost::tuple<const pUString, const pUInt> pSourceLocation;

    // source locations: filename/startlinenum/endlinenum
    typedef boost::tuple<const pUString, const pUInt, const pUInt> pSourceStartEndLocation;

    // a visitor for determining type of pVar
    class pVarTypeChecker : public boost::static_visitor<pVarType> {

    public:

        pVarType operator()(const pTriState &h) const {
            return (pNull(h)) ? pVarNullType : pVarBoolType;
        }

        pVarType operator()(const pInt &i) const {
            return pVarIntType;
        }

        pVarType operator()(const pFloat &i) const {
            return pVarFloatType;
        }

        pVarType operator()(const pBString &str) const {
            return pVarBStringType;
        }

        pVarType operator()(const pUStringP &str) const {
            return pVarUStringType;
        }

        pVarType operator()(const pHashP &h) const {
            return pVarHashType;
        }

        pVarType operator()(const pObjectP &h) const {
            return pVarObjectType;
        }

        pVarType operator()(const pVarRef &p) const {
            return pVarRefType;
        }

    };


    // a visitor for converting to a number (long or float)
    class convertToNumber : public boost::static_visitor<void> {
    protected:
        pVar &var;

    public:
        convertToNumber(pVar &v) : var(v) { }

        void operator()(pTriState &h) const {
                (h) ? var = 1l : var = 0l;
        }

        void operator()(pInt &a) const {
            // nothing, already numeric
        }

        void operator()(pFloat &i) const {
            // nothing, already numeric
        }

        void operator()(pBString &a) const {
            // TODO: handle floats
            try {
                var = boost::lexical_cast<long>(a);
            } catch(boost::bad_lexical_cast &) {
                var = 0l;
            }
        }

        void operator()(pUStringP &a) const {
            // TODO: do a real conversion here
            // should handle both integers and floats
            var = 0l;
        }

        void operator()(pHashP &h) const {
            var = (pInt)h->getSize();
        }

        void operator()(pObjectP &h) const {
            var = (pInt)h->getNumProperties();
        }

        void operator()(pVarRef &r) const {
            // unbox
            //boost::apply_visitor(convertToNumber(*r), *r);
        }

    };

    // a visitor for converting to a string
    class convertToBString : public boost::static_visitor<void> {
    protected:
        pVar &var;

    public:
        convertToBString(pVar &v) : var(v) { }

        void operator()(pTriState &h) const {
                (h) ? var = pBString("1") : var = pBString("0");
        }

        void operator()(pInt &a) const {
            // TODO: real conversion
            var = pBString("some pInt");
        }

        void operator()(pFloat &i) const {
            // TODO: real conversion
            var = pBString("some pFloat");
        }

        void operator()(pBString &a) const {
            // nothing
        }

        void operator()(pUStringP &a) const {
            // TODO
        }

        void operator()(pHashP &h) const {
            var = pBString("array");
        }

        void operator()(pObjectP &h) const {
            // TODO: toString
            var = pBString("object");
        }

        void operator()(pVarRef &r) const {
            // TODO
            // unbox
            //boost::apply_visitor(convertToNumber(*r), *r);
        }

    };

    /*
     * convenience accessors
     *
     */

    // convert to number (in place)
    inline pVarType pVar_getType(const pVar &p) {
        return boost::apply_visitor(pVarTypeChecker(), p);
    }

    // convert to number (in place)
    inline void pVar_convertToNumber(pVar &p) {
        boost::apply_visitor(convertToNumber(p), p);
    }

    // get the boolean value of a pVar. does NOT convert so pVar
    // must already be a pTriState
    inline pTriState pVar_getVal_pBool(const pVar &p) {
            return boost::get<rphp::pTriState>(p);
    }

    inline pInt pVar_getVal_pInt(const pVar &p) {
            return boost::get<pInt>(p);
    }

    inline pBString pVar_getVal_pBString(const pVar &p) {
            return boost::get<pBString>(p);
    }

    inline pUStringP pVar_getVal_pUString(const pVar &p) {
            return boost::get<pUStringP>(p);
    }

    inline pVarRef pVar_getVal_pRef(const pVar &p) {
            return boost::get<pVarRef>(p);
    }

    /*
     * type conversions
     *
     */
    pVar pVar_castToNumber(const pVar p);
    pVar pVar_castToBString(const pVar p);

    pVar pVar_add(const pVar lhs, const pVar rhs);

} /* namespace rphp */


#endif /* RPHP_PTYPES_H_ */
