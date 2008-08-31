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

#ifndef RPHP_PTYPEOPERATIONS_H_
#define RPHP_PTYPEOPERATIONS_H_

#include "pTypes.h"
#include "pHash.h"
#include "pObject.h"

namespace rphp {

/* a visitor for converting to bool (triState), in place */
class pVar_convertToBoolVisitor : public boost::static_visitor<void> {
    protected:
        pVarDataType &var;

    public:
        pVar_convertToBoolVisitor(pVarDataType &v) : var(v) { }

        void operator()(pTriState &v)  {
            (pNull(v)) ? var = pFalse : var = v;
        }

        void operator()(pInt &v) {
            (v) ? var = pTrue : var = pFalse;
        }

        void operator()(pFloat &v) {
            (v) ? var = pTrue : var = pFalse;
        }

        void operator()(pBString &v) {
            (v.empty()) ? var = pFalse : var = pTrue;
        }

        void operator()(pUStringP &v) {
            (v->isEmpty()) ? var = pFalse : var = pTrue;
        }

        void operator()(pHashP &v) {
            (v->getSize()) ? var = pTrue : var = pFalse;
        }

        void operator()(pObjectP &v) {
            (v->getNumProperties()) ? var = pTrue : var = pFalse;
        }

        void operator()(pResourceP &v) {
            var = pTrue;
        }

        void operator()(pVarP &v) {
    // TODO:unbox
    //boost::apply_visitor(pVar_convertToNumberVisitor(*r), *r);
        }

};

    
/* a visitor for converting to a php number (long or float), in place */
class pVar_convertToIntVisitor : public boost::static_visitor<void> {
protected:
    pVarDataType &var;

public:
    pVar_convertToIntVisitor(pVarDataType &v) : var(v) { }

    void operator()(pTriState &v)  {
            (v) ? var = 1l : var = 0l;
    }

    void operator()(pInt &v) { /* nothing */ }
    
    void operator()(pFloat &v) {
        var = (pInt)v;
    }

    void operator()(pBString &v);

    void operator()(pUStringP &v);

    void operator()(pHashP &v) {
        var = (pInt)v->getSize();
    }

    void operator()(pObjectP &v) {
        var = (pInt)v->getNumProperties();
    }

    void operator()(pResourceP &v) {
        // TODO: return static resource count
        var = 0l;
    }

    void operator()(pVarP &v) {
        // unbox
        //boost::apply_visitor(pVar_convertToNumberVisitor(*r), *r);
    }

};

class pVar_convertToBStringVisitor : public boost::static_visitor<void> {
protected:
    pVarDataType &var;

public:
    pVar_convertToBStringVisitor(pVarDataType &v) : var(v) { }

    void operator()(pTriState &v) {
            (v) ? var = pBString("1") : var = pBString("0");
    }

    void operator()(pInt &v) {
        // TODO: real conversion
        var = pBString("some pInt");
    }

    void operator()(pFloat &v) {
        // TODO: real conversion
        var = pBString("some pFloat");
    }

    void operator()(pBString &v) { /* nothing */ }

    void operator()(pUStringP &v) {
        // TODO
    }

    void operator()(pHashP &v) {
        var = pBString("array");
    }

    void operator()(pObjectP &v) {
        // TODO: toString
        var = pBString("object");
    }

    void operator()(pResourceP &v) {
        var = pBString("resource");
    }

    void operator()(pVarP &v) const {
        // TODO
        // unbox
        //boost::apply_visitor(convertToNumber(*r), *r);
    }

};

} /* namespace rphp */


#endif /* RPHP_PTYPEOPERATIONS_H_ */
