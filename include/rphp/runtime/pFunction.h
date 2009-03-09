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

#ifndef RPHP_PFUNCTION
#define RPHP_PFUNCTION

#include <vector>
#include "rphp/runtime/pSupport.h"
#include "rphp/runtime/pVar.h"

namespace rphp {

class pClass;
class pExtBase;
class pRuntimeEngine;

struct pFunctionParam {

    pIdentString name_;
    pVar defaultValue_;
    bool isOptional_;
    bool isByRef_;
    bool isArray_;      // array type hint
    const pClass* classHint_; // class type hint

public:

    pFunctionParam(void):
                name_(),
                defaultValue_(pNull),
                isOptional_(false),
                isByRef_(false),
                isArray_(false),
                classHint_(NULL)
    {

    }

    const pIdentString& name(void) const { return name_; }
    void setName(const pIdentString& n) { name_ = n; }

    const pVar& defaultValue(void) const { return defaultValue_; }
    void setDefault(const pVar& p) {
        assert(!p.isBoxed() && "default function parameter can't be a reference!");
        defaultValue_ = p;
        isOptional_ = true;
    }

    bool isOptional(void) const { return isOptional_; }
    void setIsOptional(bool v) { isOptional_ = v; }

    bool isByRef(void) const { return isByRef_; }
    void setIsByRef(bool v) { isByRef_ = v; }

    bool isArray(void) const { return isArray_; }
    void setIsArray(bool v) { isArray_ = v; }

    const pClass* classHint(void) const { return classHint_; }
    void setClassHint(const pClass* c) { classHint_ = c; }


};

class pFunction {

public:
    typedef std::vector<pFunctionParam*> paramListType;
    
    typedef enum { 
        pBuiltinFunType, 
        pUserFunType, 
        pBuiltinMethodType, 
        pUserMethodType 
    } pFunType;

private:

    // or parent extension (only builtins)
    const pExtBase* parentExtension_;

    // docComment?

    // signature
    pIdentString name_;
    pFunType funType_;
    pUInt requiredArity_;
    pUInt maxArity_;
    
    // TODO compact flags
    bool isVarArity_;
    
    paramListType paramList_;

    typedef union pFunPointerT {    
        pFunPointer0 funPointer0;
        pFunPointer1 funPointer1;
        pFunPointer2 funPointer2;
        pFunPointer3 funPointer3;
        pFunPointer4 funPointer4;
        pFunPointer5 funPointer5;
        pFunPointerN funPointerN;
        pMethodPointer0 methodPointer0;
        pMethodPointer1 methodPointer1;
        pMethodPointer2 methodPointer2;
        pMethodPointer3 methodPointer3;
        pMethodPointer4 methodPointer4;
        pMethodPointer5 methodPointer5;
        pMethodPointerN methodPointerN;
        pFunPointerT(void): funPointer0(NULL) { }
        pFunPointerT(pFunPointer0 p): funPointer0(p) { }
        pFunPointerT(pFunPointer1 p): funPointer1(p) { }
        pFunPointerT(pFunPointer2 p): funPointer2(p) { }
        pFunPointerT(pFunPointer3 p): funPointer3(p) { }
        pFunPointerT(pFunPointer4 p): funPointer4(p) { }
        pFunPointerT(pFunPointer5 p): funPointer5(p) { }
        pFunPointerT(pFunPointerN p): funPointerN(p) { }
        pFunPointerT(pMethodPointer0 p): methodPointer0(p) { }
        pFunPointerT(pMethodPointer1 p): methodPointer1(p) { }
        pFunPointerT(pMethodPointer2 p): methodPointer2(p) { }
        pFunPointerT(pMethodPointer3 p): methodPointer3(p) { }
        pFunPointerT(pMethodPointer4 p): methodPointer4(p) { }
        pFunPointerT(pMethodPointer5 p): methodPointer5(p) { }
        pFunPointerT(pMethodPointerN p): methodPointerN(p) { }
    };
    
    pFunPointerT funPointer_;

public:

    /// for builtin functions of arity 1-5
    template <typename funPointerType>
    pFunction(const pExtBase* e, const pIdentString& f, funPointerType fun, pUInt arity) :
        parentExtension_(e),
        name_(f),
        funType_(pBuiltinFunType),
        requiredArity_(arity),
        maxArity_(arity),
        isVarArity_(false),
        paramList_(),
        funPointer_(fun)
    {
        for (pUInt i=0; i < arity; i++)
            paramList_.push_back(new pFunctionParam());
    }

    /// for user functions of arity 1-5
    template <typename funPointerType>
    pFunction(const pIdentString& f, funPointerType fun, pUInt arity):
        parentExtension_(NULL),
        name_(f),
        funType_(pUserFunType),
        requiredArity_(arity),
        maxArity_(arity),
        isVarArity_(false),
        paramList_(),
        funPointer_(fun)    
    {
        for (pUInt i=0; i < arity; i++)
            paramList_.push_back(new pFunctionParam());
    }

    /// used in analysis phase as data structure only, no function pointer and won't be called
    pFunction(const pIdentString& f, pFunType t):
        parentExtension_(NULL),
        name_(f),
        funType_(t),
        requiredArity_(0),
        maxArity_(0),
        isVarArity_(false),
        paramList_(),
        funPointer_()
    {

    }

    ~pFunction(void) {
        foreach (paramListType::value_type i, paramList_) {
            delete i;
        }
    }

    void setRequiredArity(pUInt a) { requiredArity_ = a; }
    void setMaxArity(pUInt a) { maxArity_ = a; }
    void setFunType(pFunType t) { funType_ = t; }

    const pIdentString& name(void) const { return name_; }
    const pIdentString canonicalName(void) const { return toLowerCopy(name_); }
    const pExtBase* parentExtension(void) const { return parentExtension_; }
    pFunType funType(void) const { return funType_; }
    pUInt requiredArity(void) const { return requiredArity_; }
    pUInt maxArity(void) const { return maxArity_; }
    bool isVarArity(void) const { return isVarArity_; }

    // takes ownership
    void setParamList(const paramListType& p) { 
        maxArity_ = requiredArity_ = p.size();
        paramList_ = p; 
    }

    paramListType::size_type numParams() const { return paramList_.size(); }

    // param access
    pFunctionParam* param(pUInt i) {
        if (i < paramList_.size()) {
            return paramList_[i];
        }
        else {
            return NULL;
        }
    }
    const pFunctionParam* param(pUInt i) const {
        if (i < paramList_.size()) {
            return paramList_[i];
        }
        else {
            return NULL;
        }
    }

    // invocation
    // FIXME: this is too simplistic, it has to setup default values
    pVar invoke(pRuntimeEngine* runtime) { 
        pVar ret;
        funPointer_.funPointer0(&ret, runtime);
        return ret;
    }
    pVar invoke(pRuntimeEngine* runtime, pVar arg1) { 
        pVar ret;
        funPointer_.funPointer1(&ret, runtime, arg1);
        return ret;
    }
    pVar invoke(pRuntimeEngine* runtime, pVar arg1, pVar arg2) { 
        pVar ret;
        funPointer_.funPointer2(&ret, runtime, arg1, arg2);
        return ret;
    }
    pVar invoke(pRuntimeEngine* runtime, pVar arg1, pVar arg2, pVar arg3) { 
        pVar ret;
        funPointer_.funPointer3(&ret, runtime, arg1, arg2, arg3);
        return ret;
    }
    pVar invoke(pRuntimeEngine* runtime, pVar arg1, pVar arg2, pVar arg3, pVar arg4) { 
        pVar ret;
        funPointer_.funPointer4(&ret, runtime, arg1, arg2, arg3, arg4);
        return ret;
    }
    pVar invoke(pRuntimeEngine* runtime, pVar arg1, pVar arg2, pVar arg3, pVar arg4, pVar arg5) { 
        pVar ret;
        funPointer_.funPointer5(&ret, runtime, arg1, arg2, arg3, arg4, arg5);
        return ret;
    }
    //pVar invokeN(pVar arg1) { /* fixme */ }

};

}

#endif /* RPHP_PFUNCTION */
