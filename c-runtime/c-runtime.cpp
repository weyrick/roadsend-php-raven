/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
 *                         Thomas Moenicke <tm@php-qt.org>
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

#include "c-runtime.h"

#include "rphp/runtime/pErrorManager.h"
#include "rphp/runtime/pFunctionManager.h"
#include "rphp/runtime/pHash.h"
#include "rphp/runtime/pObject.h"
#include "rphp/runtime/pRuntimeError.h"

#include <iostream>
#include <gmp.h>


using namespace rphp;

extern "C" {

    // create a new runtime engine
    pRuntimeEngine* rphp_newRuntimeEngine() {
        pRuntimeEngine* rt = new pRuntimeEngine();
        return rt;
    }

    // destroy runtime engine
    void rphp_deleteRuntimeEngine(pRuntimeEngine* e) {
        delete e;
    }

    // print a c string to the current output buffer in the given runtime
    void rphp_print_cstr(pRuntimeEngine* e, const char* str) {
        e->output << str;
    }

    // print a pVar, coercing to string
    void rphp_print_pVar(pRuntimeEngine* e, pVar v) {
        e->output << v;
    }

    // create a new pVar for a NULL
    pVar rphp_make_pVar_pNull() {
        return pVar(pNull);
    }

    // create a new binary string
    pVar rphp_make_pVar_pBString(const char* str, size_t len) {
        return pVar(pBString(str, len));
    }

    // create a new unicode string
    pVar rphp_make_pVar_pUString(const char* str, size_t len) {
        return pVar(pUStringP(new UnicodeString(str, len, "UTF8")));
    }

    // create a new pVar from a pInt
    pVar rphp_make_pVar_pInt(pInt v) {
        return pVar(v);
    }

    // create a new pVar from serialized pBigInt data
    pVar rphp_make_pVar_pBigInt(const char* intData, size_t len, int neg) {
        // TODO too many temps?
        mpz_t z, nz;
        mpz_init(z);
        mpz_import(z,
                   len, /* size of data */
                   1,  /* most sig 1st */
                   sizeof(char),  /* bytesized */
                   1, /* bigendian */
                   0, /* fullword */
                   (void*)intData);
        if (neg) {
            mpz_init(nz);
            mpz_neg(nz, z);
            mpz_swap(z, nz);
            mpz_clear(nz);
        }
        pBigIntP b(new pBigInt(z));
        mpz_clear(z);
        return pVar(b);
    }

    // create a new pVar from a pFloat
    pVar rphp_make_pVar_pFloat(double v) {
        return pVar(v);
    }

    // create a new bool pVar
    pVar rphp_make_pVar_pBool(int v) {
        return (v) ? pVar(pTrue) : pVar(pFalse);
    }

    pVar rphp_make_pVar_pObject(const char *className) {
        return pObjectP( new pObject(className) );
    }

    pVar rphp_make_pVar_pHash() {
        return pHashP(new pHash());
    }

    int rphp_pVar_evalAsBool(pVar arg) {
    	return arg.evalAsBool();
    }

    void rphp_pHash_insertNext(pVar h, pVar v) {
        h.getHashP()->insertNext(v);
    }

    void rphp_pHash_insert(pVar h, pVar k, pVar v) {
        h.getHashP()->insert(k, v);
    }

    void rphp_registerUserFun0(pRuntimeEngine* r, const char* name, pFunPointer0 p) {
        r->functionManager->registerUser(name, p, 0);
    }

    void rphp_registerUserFun1(pRuntimeEngine* r, const char* name, pFunPointer1 p) {
        r->functionManager->registerUser(name, p, 1);
    }

    void rphp_registerUserFun2(pRuntimeEngine* r, const char* name, pFunPointer2 p) {
        r->functionManager->registerUser(name, p, 2);
    }

    void rphp_registerUserFun3(pRuntimeEngine* r, const char* name, pFunPointer3 p) {
        r->functionManager->registerUser(name, p, 3);
    }

    void rphp_registerUserFun4(pRuntimeEngine* r, const char* name, pFunPointer4 p) {
        r->functionManager->registerUser(name, p, 4);
    }

    void rphp_registerUserFun5(pRuntimeEngine* r, const char* name, pFunPointer5 p) {
        r->functionManager->registerUser(name, p, 5);
    }

    pVar rphp_funCall0(pRuntimeEngine* r, const char* name) {
        return r->functionManager->invoke(name);
    }

    pVar rphp_funCall1(pRuntimeEngine* r, const char* name, const pVar& arg1) {
        return r->functionManager->invoke(name, arg1);
    }

    pVar rphp_funCall2(pRuntimeEngine* r, const char* name, const pVar& arg1, const pVar& arg2) {
        return r->functionManager->invoke(name, arg1, arg2);
    }

    pVar rphp_funCall3(pRuntimeEngine* r, const char* name, const pVar& arg1, const pVar& arg2, const pVar& arg3) {
        return r->functionManager->invoke(name, arg1, arg2, arg3);
    }

    pVar rphp_funCall4(pRuntimeEngine* r, const char* name, const pVar& arg1, const pVar& arg2, const pVar& arg3, const pVar& arg4) {
        return r->functionManager->invoke(name, arg1, arg2, arg3, arg4);
    }

    pVar rphp_funCall5(pRuntimeEngine* r, const char* name, const pVar& arg1, const pVar& arg2, const pVar& arg3, const pVar& arg4, const pVar& arg5) {
        return r->functionManager->invoke(name, arg1, arg2, arg3, arg4, arg5);
    }

    pVar rphp_newCall(pRuntimeEngine* r, const char* className, const pVar& arg1) {
      pVar result = rphp_make_pVar_pObject( className );
      // invoke method call "__construct" on result
      return result;
    }

    void rphp_setSourceLocation(rphp::pRuntimeEngine* r, const char* file, rphp::pInt line) {
       r->errorManager->setSourceLocation(file, line);
    }

    void _make_pVar(void) {
      pVar p;
      p = 5;
    }

    void _assign_pVar(void) {
      pVar l;
      pVar r;
      l = r;
    }

    ////

    void rphp_mainEntry(pVar*, pRuntimeEngine*);

    // stand alone entry point
    // this will be removed from the module base if there is no entry point
    int main(int argc, char** argv) {
        pVar retVal;
        pRuntimeEngine rEngine;
        try {
            rphp_mainEntry(&retVal, &rEngine);
        }
        catch (pRuntimeError& e) {
            return retVal.copyAsInt();
        }
        catch (...) {
            return retVal.copyAsInt();
        }
    }

}

