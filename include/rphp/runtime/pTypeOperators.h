/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#include <boost/lexical_cast.hpp>

#include "rphp/runtime/pRuntimeTypes.h"
#include "rphp/runtime/pHash.h"
#include "rphp/runtime/pObject.h"

namespace rphp {

/* a visitor for converting to bool (triState), in place */
class pVar_convertToBoolVisitor : public boost::static_visitor<void> {

protected:
    pVarDataType &var_;

public:
    pVar_convertToBoolVisitor(pVarDataType &v) : var_(v) { }

    void operator()(const pTriState &v)  {
        (pNull(v)) ? var_ = pFalse : var_ = v;
    }

    void operator()(const pInt &v) {
        (v) ? var_ = pTrue : var_ = pFalse;
    }

    void operator()(const pFloat &v) {
        (v) ? var_ = pTrue : var_ = pFalse;
    }

    void operator()(const pBString &v) {
        (v.empty()) ? var_ = pFalse : var_ = pTrue;
    }

    void operator()(const pUStringP &v) {
        (v->isEmpty()) ? var_ = pFalse : var_ = pTrue;
    }

    void operator()(const pHashP &v) {
        (v->size()) ? var_ = pTrue : var_ = pFalse;
    }

    void operator()(const pObjectP &v) {
        (v->numProperties()) ? var_ = pTrue : var_ = pFalse;
    }

    void operator()(const pResourceP &v) {
        var_ = pTrue;
    }

    void operator()(const pVarP &v) {
        v->convertToBool();
    }

};


/* a visitor for converting to a php number (long or float), in place */
class pVar_convertToIntVisitor : public boost::static_visitor<void> {
protected:
    pVarDataType &var_;

public:
    pVar_convertToIntVisitor(pVarDataType &v) : var_(v) { }

    void operator()(const pTriState &v)  {
            (v) ? var_ = 1l : var_ = 0l;
    }

    void operator()(const pInt &v) { /* nothing */ }

    void operator()(const pFloat &v) {
        var_ = (pInt)v;
    }

    void operator()(const pBString &v);

    void operator()(const pUStringP &v);

    void operator()(const pHashP &v) {
        var_ = (pInt)v->size();
    }

    void operator()(const pObjectP &v) {
        var_ = (pInt)v->numProperties();
    }

    void operator()(const pResourceP& v) {
        // TODO: return static resource count
        var_ = 0l;
    }

    void operator()(const pVarP &v) {
        v->convertToInt();
    }

};

class pVar_convertToBStringVisitor : public boost::static_visitor<void> {
protected:
    pVarDataType &var_;

public:
    pVar_convertToBStringVisitor(pVarDataType &v) : var_(v) { }

    void operator()(const pTriState &v) {
        if (pNull(v)) {
            var_ = pBString("");
        }
        else {
            (v) ? var_ = pBString("1") : var_ = pBString("0");
        }
    }

    void operator()(const pInt &v) {
        using boost::lexical_cast;
        using boost::bad_lexical_cast;
        try {
            var_ = pBString(lexical_cast<pBString>(v));
        }
        catch(bad_lexical_cast &) {
            var_ = pBString("0");
        }
    }

    void operator()(const pFloat &v) {
        using boost::lexical_cast;
        using boost::bad_lexical_cast;
        try {
            var_ = pBString(lexical_cast<pBString>(v));
        }
        catch(bad_lexical_cast &) {
            var_ = pBString("0.0");
        }
    }

    void operator()(const pBString &v) { /* nothing */ }

    void operator()(const pUStringP &v) {
        // TODO this converts to ASCII. is that what we want?
        //UnicodeString ustr = v.readonlyICUString();
        UErrorCode err(U_ZERO_ERROR);
        int32_t bufSize = v->extract((char*)NULL, 0, NULL, err);
        if (bufSize) {
            err = U_ZERO_ERROR;
            char *buf = (char*)malloc(++bufSize);
            v->extract(buf, bufSize, NULL, err);
            if (U_SUCCESS(err)) {
                var_ = pBString(buf);
            }
            else {
                // TODO runtime error?
                var_ = pBString();
            }
            free(buf);
            return;
        }
        var_ = pBString();
    }

    void operator()(const pHashP &v) {
        var_ = pBString("array");
    }

    void operator()(const pObjectP &v) {
        // TODO: toString
        var_ = pBString("object");
    }

    void operator()(const pResourceP &v) {
        var_ = pBString("resource");
    }

    void operator()(const pVarP &v) const {
        v->convertToBString();
    }

};

} /* namespace rphp */


#endif /* RPHP_PTYPEOPERATIONS_H_ */
