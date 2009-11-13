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

#include "rphp/runtime/pRuntimeTypes.h"
#include "rphp/runtime/pHash.h"
#include "rphp/runtime/pObject.h"

#include <boost/lexical_cast.hpp>

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

    void operator()(const pBigIntP &v) {
        (*v != 0) ? var_ = pTrue : var_ = pFalse;
    }

    void operator()(const pFloatP &v) {
        (*v != 0) ? var_ = pTrue : var_ = pFalse;
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

    void operator()(const pBigIntP &v) {
        var_ = v->get_si();
    }

    void operator()(const pFloatP &v) {
        var_ = v->get_si();
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

    void operator()(const pBigIntP &v) {
        var_ = pBString(v->get_str());
    }

    void operator()(const pFloatP &v);

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

class pVar_convertToUStringVisitor : public boost::static_visitor<void> {
protected:
    pVarDataType &var_;

public:
    pVar_convertToUStringVisitor(pVarDataType &v) : var_(v) { }

    void operator()(const pTriState &v) {
        if (pNull(v)) {
            var_ = pUStringP(new pUString(""));
        }
        else {
            (v) ? var_ = pUStringP(new pUString("1")) : var_ = pUStringP(new pUString("0"));
        }
    }

    void operator()(const pInt &v) {
        // FIXME
        var_ = pUStringP(new pUString("pInt conversion to pUString"));
    }

    void operator()(const pBigIntP &v) {
        // FIXME
        var_ = pUStringP(new pUString("pBigIntP conversion to pUString"));
    }

    void operator()(const pFloatP &v) {
        // FIXME
        var_ = pUStringP(new pUString("pFloatP conversion to pUString"));
    }

    void operator()(const pBString &v) {
        // FIXME
        var_ = pUStringP(new pUString("pBString conversion to pUString"));
    }

    void operator()(const pUStringP &v) {
        /* none */
    }

    void operator()(const pHashP &v) {
        var_ = pUStringP(new pUString("array"));
    }

    void operator()(const pObjectP &v) {
        // TODO: toString
        var_ = pUStringP(new pUString("object"));
    }

    void operator()(const pResourceP &v) {
        var_ = pUStringP(new pUString("resource"));
    }

    void operator()(const pVarP &v) const {
        v->convertToUString();
    }

};


/* a visitor for converting to array, in place */
class pVar_convertToHashVisitor : public boost::static_visitor<void> {

protected:
    pVarDataType &var_;

public:
    pVar_convertToHashVisitor(pVarDataType &v) : var_(v) { }

    void operator()(const pTriState &v)  {
    	pHash* hash = new pHash;
    	if(!pNull(v))
    		hash->insertNext(v);
        var_ = pHashP(hash);
    }

    void operator()(const pInt &v) {
        pHash* hash = new pHash;
        hash->insertNext(v);
        var_ = pHashP(hash);
    }

    void operator()(const pBigIntP &v) {
        pHash* hash = new pHash;
        // TODO overflow?
        hash->insertNext(v->get_si());
        var_ = pHashP(hash);
    }

    void operator()(const pFloatP &v) {
        pHash* hash = new pHash;
        hash->insertNext(v->get_si());
        var_ = pHashP(hash);
   }

    void operator()(const pBString &v) {
        pHash* hash = new pHash;
        hash->insertNext(v);
        var_ = pHashP(hash);
    }

    void operator()(const pUStringP &v) {
        pHash* hash = new pHash;
        hash->insertNext(v);
        var_ = pHashP(hash);
    }

    void operator()(const pHashP &v) {
    	/* none */
    }

    void operator()(const pObjectP &v) {
    	// TODO: needs array_merge like behaviour + name mangling in the pObject strucure.
    	//throw pRuntimeError("cast of an object to an array not implemented yet!");
    }

    void operator()(const pResourceP &v) {
    	pHash* hash = new pHash;
    	hash->insertNext(v);
    	var_ = pHashP(hash);
    }

    void operator()(const pVarP &v) {
        v->convertToHash();
    }

};


} /* namespace rphp */


#endif /* RPHP_PTYPEOPERATIONS_H_ */
