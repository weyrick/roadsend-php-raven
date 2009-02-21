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

#ifndef RPHP_PFUNCTION
#define RPHP_PFUNCTION

#include <vector>
#include "rphp/runtime/pSupport.h"
#include "rphp/runtime/pVar.h"

namespace rphp {

class pClass;
class pExtBase;

typedef enum { pBuiltinFunType, pUserFunType } pFunType;

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

private:

    // or parent extension (only builtins)
    const pExtBase* parentExtension_;

    // docComment?

    // signature
    pIdentString name_;
    pFunType funType_;
    pUInt requiredArity_;
    pUInt maxArity_;
    bool isVarArity_;

    paramListType paramList_;

    const pFunPointer1 funPointer1_;
    const pFunPointer2 funPointer2_;
    const pFunPointer3 funPointer3_;
    const pFunPointer4 funPointer4_;
    const pFunPointer5 funPointer5_;
    const pFunPointerN funPointerN_;

public:

    // standard builtin function: one argument
    pFunction(const pExtBase* e, const pIdentString& f, const pFunPointer1& fun) :
        parentExtension_(e),
        name_(f),
        funType_(pBuiltinFunType),
        requiredArity_(1),
        maxArity_(1),
        isVarArity_(false),
        paramList_(),
        funPointer1_(fun)
    {
        // initalize parameters for arity len 1
        paramList_.push_back(new pFunctionParam());
    }

    pFunction(const pExtBase* e, const pIdentString& f, const pFunPointer3& fun) :
        parentExtension_(e),
        name_(f),
        funType_(pBuiltinFunType),
        requiredArity_(3),
        maxArity_(3),
        isVarArity_(false),
        paramList_(),
        funPointer3_(fun)    {
        // initalize parameters for arity len 2
        paramList_.push_back(new pFunctionParam());
        paramList_.push_back(new pFunctionParam());
        paramList_.push_back(new pFunctionParam());
    }

    pFunction(const pIdentString& f):
        parentExtension_(NULL),
        name_(f),
        funType_(pUserFunType),
        requiredArity_(0),
        maxArity_(0),
        isVarArity_(false),
        paramList_() {

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
    const pUInt& requiredArity(void) const { return requiredArity_; }
    const pUInt& maxArity(void) const { return maxArity_; }
    bool isVarArity(void) const { return isVarArity_; }

    // takes ownership
    void setParamList(const paramListType& p) { paramList_ = p; }

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
    pVar invoke(pVar arg1) { return funPointer1_(arg1); }
    pVar invoke(pVar arg1, pVar arg2) { return funPointer2_(arg1, arg2); }
    pVar invoke(pVar arg1, pVar arg2, pVar arg3) { return funPointer3_(arg1,arg2,arg3); }
    pVar invoke(pVar arg1, pVar arg2, pVar arg3, pVar arg4) { return funPointer4_(arg1,arg2,arg3,arg4); }
    pVar invoke(pVar arg1, pVar arg2, pVar arg3, pVar arg4, pVar arg5) { return funPointer5_(arg1,arg2,arg3,arg4,arg5); }
    //pVar invokeN(pVar arg1) { /* fixme */ }

};

}

#endif /* RPHP_PFUNCTION */
