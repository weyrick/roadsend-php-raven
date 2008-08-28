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

#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>

// including this file includes all rphp base types
#include "pVar.h"
#include "pHash.h"
#include "pObject.h"
#include "pResource.h"

namespace rphp {

    // note, pUInt is not a base PHP type (all PHP numbers are signed)
    typedef unsigned long pUInt;

    // source locations: filename/linenum
    typedef boost::tuple<const pUString, const pUInt> pSourceLocation;

    // source locations: filename/startlinenum/endlinenum
    typedef boost::tuple<const pUString, const pUInt, const pUInt> pSourceStartEndLocation;

    // php function signatures
    typedef boost::function<pVar (pVar)> pFunPointer1;
    
} /* namespace rphp */


#endif /* RPHP_PTYPES_H_ */
