/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PVAROPERATORS_H_
#define RPHP_PVAROPERATORS_H_

#include "rphp/runtime/pRuntimeTypes.h"
#include "rphp/runtime/pHash.h"
#include "rphp/runtime/pOutputBuffer.h"

namespace rphp {

class pVar_var_dump : public boost::static_visitor<void>
{
private:
    pOutputBuffer* buf_;
    const pVar& var_;
    const pBString& indent_;
public:
    pVar_var_dump(const pVar& v, pOutputBuffer *buf, const pBString& indent=""): buf_(buf), var_(v), indent_(indent) { }

    void operator()(const pTriState &v) const {
        if (pNull(v)) {
            *buf_ << indent_ << "NULL\n";
        }
        else {
            *buf_ << indent_ << "bool(";
            if (v == pTrue)
                *buf_ << "true";
            else
                *buf_ << "false";
            *buf_ << ")\n";
        }
    }

    void operator()(const pInt &v) const {
        *buf_ << indent_ << "int(" << var_ << ")\n";
    }

    void operator()(const pBigIntP &v) const {
        *buf_ << indent_ << "int(" << var_ << ")\n";
    }

    void operator()(const pFloatP &v) const {
        *buf_ << indent_ << "float(" << var_ << ")\n";
    }

    void operator()(const pBString &v) const {
        *buf_ << indent_ << "string(" << pInt(v.length()) << ") \"" << v << "\"\n";
    }

    void operator()(const pUStringP &v) const {
        *buf_ << indent_ << "unicode(" << pInt(v->length()) << ") \"" << v << "\"\n";
    }

    void operator()(const pHashP &v) const {
        v->varDump(buf_, indent_);
    }

    void operator()(const pObjectP &v) const {
        // TODO
        *buf_ << indent_ << "object\n";
    }

    void operator()(const pResourceP &v) const {
        // TODO
        *buf_ << indent_ << "resource\n";
    }

    void operator()(const pVarP &v) const {
    	v->applyVisitor<pVar_var_dump>(buf_);
    }
};


} /* namespace rphp */


#endif /* RPHP_PVAROPERATORS_H_ */
