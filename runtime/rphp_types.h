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

#ifndef RPHP_TYPES_H_
#define RPHP_TYPES_H_

#include "rphp_pvar.h"

namespace rphp {

// a visitor for determining type of pvar
class pvarTypeChecker : public boost::static_visitor<pvarType>
{
public:

    pvarType operator()(const p3state &h) const {
        return (h == rphp::Null) ? PVAR_NULL : PVAR_BOOL;
    }

	pvarType operator()(const long &i) const {
        return PVAR_LONG;
    }

    pvarType operator()(const double &i) const {
        return PVAR_DOUBLE;
    }

    pvarType operator()(const bstring &str) const {
        return PVAR_BSTRING;
    }

    pvarType operator()(const ustring &str) const {
        return PVAR_USTRING;
    }

    pvarType operator()(const php_hash &h) const {
        return PVAR_HASH;
    }

    pvarType operator()(const php_object &h) const {
        return PVAR_OBJ;
    }

};



// a visitor for converting to a number (long or float)
class convertToNumber : public boost::static_visitor<void>
{
protected:
	pvar &var;
public:
	convertToNumber(pvar &v) : var(v) {}

	void operator()(const p3state &h) const {
		(h == rphp::True) ? var = 1l : var = 0l;
	}

    void operator()(const long &a) const {
		// nothing, already numeric
    }

    void operator()(const double &i) const {
		// nothing, already numeric
    }

    void operator()(const bstring &a) const {
    	// TODO: handle floats
		try {
		  var = boost::lexical_cast<long>(a);
		} catch(boost::bad_lexical_cast &) {
		  var = 0l;
		}
    }

    void operator()(const ustring &a) const {
    	// TODO: do a real conversion here
    	// should handle both integers and floats
    	var = 0l;
    }

    void operator()(const php_hash &h) const {
		var = (long)h.getSize();
    }

    void operator()(const php_object &h) const {
    	var = 0l;
    }

};

// convert to number (in place)
inline pvarType pvar_getType(const pvar &p) {
    return boost::apply_visitor(pvarTypeChecker(), p);
}

// convert to number (in place)
inline void pvar_convertToNumber(pvar &p) {
    boost::apply_visitor(convertToNumber(p), p);
}

// get the boolean value of a pvar. does NOT convert so pvar
// must already be a p3state
inline p3state pvar_getVal_bool(const pvar &p) {
	return boost::get<rphp::p3state>(p);
}

inline long pvar_getVal_long(const pvar &p) {
	return boost::get<long>(p);
}


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
        result = pvar_getVal_long(l) + pvar_getVal_long(r);
        //std::cout << "pvar_add: result is " << result << std::endl;
    }

    return result;
}


} /* namespace rphp */


#endif /* RPHP_TYPES_H_ */
