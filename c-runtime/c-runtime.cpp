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
#include <unicode/ustream.h>

#include "rphp/runtime/pTypes.h"
#include "c-runtime.h"

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
    void rphp_print_cstr(rphp::pRuntimeEngine* e, const char* str) {
        e->outputManager->print(rphp::pBString(str));
    }

    // print a pVar, coercing to string
    void rphp_print_pVar(rphp::pRuntimeEngine* e, rphp::pVar v) {
        v.convertToString();
        if (v.isUString())
            e->outputManager->print(v.getUString());
        else
            e->outputManager->print(v.getBString());
    }

    // create a new binary string
    rphp::pVar rphp_make_pVar_pBString(const char* str) {
        return rphp::pVar(rphp::pBString(str));
    }
    
    // create a new unicode string
    rphp::pVar rphp_make_pVar_pUString(const char* str, size_t len) {
        // our code generator always generates UTF-16BE
        return rphp::pVar(UnicodeString(str, len, "UTF-16BE"));
    }

    // create a new pVar from a pInt
    rphp::pVar rphp_make_pVar_pInt(rphp::pInt v) {
        return rphp::pVar(v);
    }

    // create a new pVar from a pFloat
    rphp::pVar rphp_make_pVar_pFloat(rphp::pFloat v) {
        return rphp::pVar(v);
    }
    
    // create a new bool pVar
    rphp::pVar rphp_make_pVar_pBool(int v) {
        return (v) ? rphp::pVar(rphp::pTrue) : rphp::pVar(rphp::pFalse);
    }

    rphp::pVar rphp_make_pVar_pObject(const char *className) {
        return rphp::pObjectP( new rphp::pObject(className) );
    }

    /*
    rphp::pVar rphp_funCall1(rphp::pRuntimeEngine* r, const char* name) {
        return r->functionManager->invoke(name);
    }
    */

    rphp::pVar rphp_funCall1(rphp::pRuntimeEngine* r, const char* name, rphp::pVar arg1) {
        return r->functionManager->invoke(name, arg1);
    }

    rphp::pVar rphp_funCall3(rphp::pRuntimeEngine* r, const char* name, rphp::pVar arg1, rphp::pVar arg2, rphp::pVar arg3) {
        return r->functionManager->invoke(name, arg1, arg2, arg3);
    }
    
    rphp::pVar rphp_newCall(rphp::pRuntimeEngine* r, const char* className, rphp::pVar arg1) {
      rphp::pVar result = rphp_make_pVar_pObject( className );
      // invoke method call "__construct" on result
      return result;
    }

    void _make_pVar(void) {
      rphp::pVar p;
      p = 5;
    }

    void _assign_pVar(void) {
      rphp::pVar l;
      rphp::pVar r;
      l = r;
    }

}

