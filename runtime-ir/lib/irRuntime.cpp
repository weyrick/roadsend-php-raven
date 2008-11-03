/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
 *                    Thomas Moenicke <tm@php-qt.org>
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

#include <iostream>
#include "irRuntime.h"
#include <unicode/ustream.h>
#include "pTypes.h"

extern "C" {

    // create a new runtime engine
    rphp::pRuntimeEngine* rphp_newRuntimeEngine() {
        rphp::pRuntimeEngine* rt = new rphp::pRuntimeEngine();
        return rt;
    }

    // destroy runtime engine
    void rphp_deleteRuntimeEngine(rphp::pRuntimeEngine* e) {
        delete e;
    }

    // print a c string to the current output buffer in the given runtime
    void rphp_print_cstr(rphp::pRuntimeEngine* e, char* str) {
        e->outputManager->print(rphp::pBString(str));
    }

    // print a pVar, coercing to string
    void rphp_print_pVar(rphp::pRuntimeEngine* e, rphp::pVar v) {
        v.convertToBString();
        e->outputManager->print(v.getBString());
    }

    // create a new pVar from a bString
    rphp::pVar rphp_make_pVar_from_cstr(char* str) {
        return rphp::pVar(rphp::pBString(str));
    }

    // create a new pVar from a pInt
    rphp::pVar rphp_make_pVar_from_pInt(rphp::pInt v) {
        return rphp::pVar(v);
    }

    
    // create a new bool pVar
    rphp::pVar rphp_make_pVar_bool(int v) {
        return (v) ? rphp::pVar(rphp::pTrue) : rphp::pVar(rphp::pFalse);
    }

    rphp::pVar rphp_make_object(char *className) {
        return rphp::pObjectP( new rphp::pObject(className) );
    }
    
    void _make_pVar(void) {
      rphp::pVar p;
      p = 5;
    }

}

