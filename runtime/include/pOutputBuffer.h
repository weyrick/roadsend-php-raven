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

#ifndef RPHP_POUTPUTBUFFER_H_
#define RPHP_POUTPUTBUFFER_H_

#include "pTypes.h"

namespace rphp {

    class pOutputBuffer {

        // default size of a new output buffer
        // note that for unicode strings, memory space is 2*defBufSize
        static const std::size_t defBufSize = 512;

        typedef enum { bufTypeBinary, bufTypeUnicode } bufTypeT;

        private:
            
            pUString *uBuffer;
            pBString *bBuffer;
            bufTypeT bType;

        public:

            // constructors

            // default builds a unicode buffer, default size
            pOutputBuffer() : uBuffer(new pUString(defBufSize,' ',0)), bType(bufTypeUnicode) { }

            // specify type, default size
            pOutputBuffer(bufTypeT t) : bType(t) {
                switch (t) {
                    case bufTypeBinary:
                        bBuffer = new pBString(defBufSize,' ');
                        break;
                    case bufTypeUnicode:
                        uBuffer = new pUString(defBufSize,' ',0);
                        break;
                }
            }

            // specify type, size
            pOutputBuffer(bufTypeT t, std::size_t s) : bType(t) {
                switch (t) {
                    case bufTypeBinary:
                        bBuffer = new pBString(s,' ');
                        break;
                    case bufTypeUnicode:
                        uBuffer = new pUString(s,' ',0);
                        break;
                }
            }

            // destructor
            ~pOutputBuffer() {
                if (uBuffer)
                    delete uBuffer;
                if (bBuffer)
                    delete bBuffer;
            }

    };

}

#endif /* RPHP_POUTPUTBUFFER_H_ */
