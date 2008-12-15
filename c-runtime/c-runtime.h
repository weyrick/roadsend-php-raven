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

#ifndef RPHP_IRRUNTIME_H_
#define RPHP_IRRUNTIME_H_

#include "rphp/runtime/pRuntime.h"

extern "C" {

    // create a new runtime engine
    rphp::pRuntimeEngine* rphp_newRuntimeEngine();

    // destroy runtime engine
    void rphp_deleteRuntimeEngine(rphp::pRuntimeEngine*);

    // debug
    // print to runtime output buffer
    void rphp_print_cstr(rphp::pRuntimeEngine*, const char* str);
    
    void rphp_print_pVar(rphp::pRuntimeEngine* e, rphp::pVar v);
    //

    // pVar creation
    rphp::pVar rphp_make_pVar_pBString(const char* str);

    rphp::pVar rphp_make_pVar_pUString(const char* str, size_t len);

    rphp::pVar rphp_make_pVar_pInt(rphp::pInt v);

    rphp::pVar rphp_make_pVar_pFloat(rphp::pFloat v);

    rphp::pVar rphp_make_pVar_pBool(int v);

    rphp::pVar rphp_make_pVar_pObject(const char* name );
    //

    // function invocation
    rphp::pVar rphp_funCall0(rphp::pRuntimeEngine* r, const char* name);
    rphp::pVar rphp_funCall1(rphp::pRuntimeEngine* r, const char* name, rphp::pVar arg1);
    //rphp::pVar rphp_funCall2(rphp::pRuntimeEngine* r, const char* name, rphp::pVar arg1);
    rphp::pVar rphp_funCall3(rphp::pRuntimeEngine* r, const char* name, rphp::pVar arg1, rphp::pVar arg2, rphp::pVar arg3);
    //rphp::pVar rphp_funCall4(rphp::pRuntimeEngine* r, const char* name, rphp::pVar arg1);
    //rphp::pVar rphp_funCall5(rphp::pRuntimeEngine* r, const char* name, rphp::pVar arg1);
    rphp::pVar rphp_newCall(rphp::pRuntimeEngine* r, const char* className, rphp::pVar arg1);
    //
    
    void _make_pVar(void);
    void _assign_pVar(void);
                                        
}

#endif /* RPHP_IRRUNTIME_H_ */
