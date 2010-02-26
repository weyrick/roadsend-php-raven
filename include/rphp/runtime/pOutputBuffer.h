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

#ifndef RPHP_POUTPUTBUFFER_H_
#define RPHP_POUTPUTBUFFER_H_

#include "rphp/runtime/pRuntimeTypes.h"

namespace rphp {

class pOutputBuffer {

public:

    typedef enum { bufTypeBinary, bufTypeUnicode } bufTypeT;

private:

    pBString *bBuffer_;
    UnicodeString *uBuffer_;
    bufTypeT bType_;

public:

    // constructors

    // specify type
    pOutputBuffer(bufTypeT t) : bBuffer_(0), uBuffer_(0), bType_(t) {
        switch (t) {
            case bufTypeBinary:
                bBuffer_ = new pBString();
                break;
            case bufTypeUnicode:
                uBuffer_ = new UnicodeString();
                break;
        }
    }

    // destructor
    ~pOutputBuffer() {
        if (uBuffer_)
            delete uBuffer_;
        if (bBuffer_)
            delete bBuffer_;
    }

    size_t length(void) const {
        switch (bType_) {
            case bufTypeBinary:
                return bBuffer_->length();
            case bufTypeUnicode:
                return uBuffer_->length();
        }
        // unreachable?
        return 0;
    }

    bufTypeT getBufferType() const {
        return bType_;
    }

    const pBString* getBBuffer() const {
        return bBuffer_;
    }

    const UnicodeString* getUBuffer() const {
        return uBuffer_;
    }

    const char* getRawBuffer() const {
        switch (bType_) {
            case bufTypeBinary:
                return bBuffer_->c_str();
            case bufTypeUnicode:
                return (const char*)uBuffer_->getTerminatedBuffer();
        }
        assert(0 && "unreachable!");
    }

    void newline() {
        switch (bType_) {
            case bufTypeBinary:
                bBuffer_->push_back('\n');
                break;
            case bufTypeUnicode:
                uBuffer_->append('\n');
                break;
        }
    }

    void print(const pBString& str) {
        switch (bType_) {
            case bufTypeBinary:
                bBuffer_->append(str);
                break;
            case bufTypeUnicode:
                // TODO: this doesn't seem so efficient. but how often will it be used?
                uBuffer_->append(UnicodeString(str.c_str(), str.length(), US_INV));
                break;
        }
    }

    void print(const pUString& str) {
        if (bType_ == bufTypeBinary) {
            // convert to unicode buffer
            //uBuffer_ = new UnicodeString(str.readonlyICUString());
            uBuffer_ = new UnicodeString(str);
            delete bBuffer_;
        }
        //uBuffer_->append(str.readonlyICUString());
        uBuffer_->append(str);
    }

};

// iostream like interface
pOutputBuffer& operator<< (pOutputBuffer& ob, const char* str);
pOutputBuffer& operator<< (pOutputBuffer& ob, const pInt i);
pOutputBuffer& operator<< (pOutputBuffer& ob, const pVar& var);
pOutputBuffer& operator<< (pOutputBuffer& ob, const pBString& str);
pOutputBuffer& operator<< (pOutputBuffer& ob, const pUString& str);

}

#endif /* RPHP_POUTPUTBUFFER_H_ */
