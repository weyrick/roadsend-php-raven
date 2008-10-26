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

#ifndef RPHP_IRRUNTIME_H_
#define RPHP_IRRUNTIME_H_

#include "pRuntime.h"

extern "C" {

    // create a new runtime engine
    rphp::pRuntimeEngine* rphp_newRuntimeEngine();

    // destroy runtime engine
    void rphp_deleteRuntimeEngine(rphp::pRuntimeEngine*);

    // print to runtime output buffer
    void rphp_print_cstr(rphp::pRuntimeEngine*, char* str);
    
    void rphp_print_pVar(rphp::pRuntimeEngine* e, rphp::pVar v);

    rphp::pVar rphp_make_pVar_from_cstr(char* str);

    rphp::pVar rphp_make_pVar_from_pInt(rphp::pInt v);
    
    rphp::pVar rphp_make_pVar_bool(int v);

    void _make_pVar(void);
                                        
}

#endif /* RPHP_IRRUNTIME_H_ */
