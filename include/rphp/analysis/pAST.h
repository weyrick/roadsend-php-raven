/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
   ***** END LICENSE BLOCK *****

 The layout of this AST has taken much inspiration from clang,
 llvm, and kdevelop-pg, among others. Particularly, the base stmt
 class using the bump ptr and iterators are based on the ones in the
 AST library in clang.

 http://clang.llvm.org
 http://kdevelop.org

*/

#ifndef RPHP_PAST_H_
#define RPHP_PAST_H_

#include "rphp/pSourceTypes.h"
#include "rphp/pTypes.h"
#include "rphp/analysis/pParseContext.h"
#include "rphp/runtime/pFunction.h"

#include <vector>
#include <iterator>

#include <llvm/Support/StringPool.h>
#include <llvm/Support/Casting.h>
#include <llvm/ADT/SmallVector.h>

namespace rphp {

using llvm::isa;
using llvm::dyn_cast;

// These define how big our SmallVectors are, which means
// this should be a good average length we expect to parse

// global declaration
#define RPHP_GLOBAL_VECTOR_SIZE 5
#define RPHP_FORMAL_PARAM_VECTOR_SIZE 5

class pSourceModule;

namespace AST {

enum nodeKind {
#define STMT(CLASS, PARENT) CLASS##Kind,
#include "rphp/analysis/astNodes.def"
};

class stmt;

/* statement iterators */
class stmtIteratorBase {
protected:

  stmt** ptr;
  stmtIteratorBase(stmt** s) : ptr(s) {}
  stmtIteratorBase() : ptr(NULL) {}

};


template <typename DERIVED, typename REFERENCE>
class stmtIteratorImpl : public stmtIteratorBase,
                         public std::iterator<std::forward_iterator_tag,
                                              REFERENCE, ptrdiff_t,
                                              REFERENCE, REFERENCE> {
protected:
  stmtIteratorImpl(const stmtIteratorBase& RHS) : stmtIteratorBase(RHS) {}
public:
  stmtIteratorImpl() {}
  stmtIteratorImpl(stmt** s) : stmtIteratorBase(s) {}

  DERIVED& operator++() {
      ++ptr;
      return static_cast<DERIVED&>(*this);
  }

  DERIVED operator++(int) {
    DERIVED tmp = static_cast<DERIVED&>(*this);
    operator++();
    return tmp;
  }

  bool operator==(const DERIVED& RHS) const { return ptr == RHS.ptr; }
  bool operator!=(const DERIVED& RHS) const { return ptr != RHS.ptr; }
  REFERENCE operator*() const { return (REFERENCE) *ptr; }
  REFERENCE operator->() const { return operator*(); }

};

struct stmtIterator : public stmtIteratorImpl<stmtIterator,stmt*&> {
  explicit stmtIterator() : stmtIteratorImpl<stmtIterator,stmt*&>() {}

  stmtIterator(stmt** S) : stmtIteratorImpl<stmtIterator,stmt*&>(S) {}
};

struct constStmtIterator : public stmtIteratorImpl<constStmtIterator,
                                                   const stmt*> {
  explicit constStmtIterator() :
    stmtIteratorImpl<constStmtIterator,const stmt*>() {}

  constStmtIterator(const stmtIterator& RHS) :
    stmtIteratorImpl<constStmtIterator,const stmt*>(RHS) {}
};

// statement base class
class stmt {

    nodeKind kind_;

    pUInt refCount_;

    pUInt startLineNum_;
    pUInt endLineNum_;

protected:
  void* operator new(size_t bytes) throw() {
    assert(0 && "stmt cannot be allocated with regular 'new'.");
    return 0;
  }
  void operator delete(void* data) throw() {
    assert(0 && "stmt cannot be released with regular 'delete'.");
  }

  // overridden by extending classes to perform class specific destruction
  // note you should still always call Stmt::doDestroy
  virtual void doDestroy(pParseContext& C);

  void destroyChildren(pParseContext& C);

public:
    stmt(nodeKind k): kind_(k), refCount_(1), startLineNum_(0), endLineNum_(0) { }

    void destroy(pParseContext& C) {
        assert(refCount_ >= 1);
        if (--refCount_ == 0)
            doDestroy(C);
    }

    stmt *retain() {
      assert(refCount_ >= 1);
      ++refCount_;
      return this;
    }

    virtual ~stmt(void) { }

    inline void* operator new(size_t bytes, pParseContext& C, size_t align=8) throw() {
        return C.allocate(bytes, align);
    }
    inline void* operator new(size_t bytes, void* mem) throw() {
      return mem;
    }
    void operator delete(void*, pParseContext&, unsigned) throw() { }
    void operator delete(void*, std::size_t) throw() { }
    void operator delete(void*, void*) throw() { }

    typedef stmtIterator       child_iterator;
    typedef constStmtIterator  const_child_iterator;

    virtual child_iterator child_begin() = 0;
    virtual child_iterator child_end()   = 0;

    const_child_iterator child_begin() const {
      return const_child_iterator(const_cast<stmt*>(this)->child_begin());
    }

    const_child_iterator child_end() const {
      return const_child_iterator(const_cast<stmt*>(this)->child_end());
    }

    nodeKind kind(void) const { return kind_; }

    void setLine(pUInt start) { startLineNum_ = start; endLineNum_ = start; }
    void setLine(pUInt start, pUInt end) { startLineNum_ = start; endLineNum_ = end; }

    pUInt startLineNum(void) const { return startLineNum_; }
    pUInt endLineNum(void) const { return endLineNum_; }

    // LLVM isa<T> and casting support
    static bool classof(const stmt* s) { return true; }

};

typedef std::vector<stmt*> statementList;

// expression base class
class expr: public stmt {

public:
    // see astNodes.def
    static const nodeKind firstExprKind = assignmentKind;
    static const nodeKind lastExprKind = unaryOpKind;

    expr(nodeKind k): stmt(k) { }

    static bool classof(const expr* s) { return true; }
    static bool classof(const stmt* s) {
        return s->kind() >= firstExprKind &&
               s->kind() <= lastExprKind;
    }

};

typedef std::vector<expr*> expressionList;

// a block of statements
class block: public stmt {
    stmt** block_;
    pUInt numStmts_;
public:

    // build a block with a single expression
    block(pParseContext& C, expr* s): stmt(blockKind), block_(0), numStmts_(1) {
        block_ = new (C) stmt*[numStmts_];
        block_[0] = s;
    }

    block(pParseContext& C, const statementList* s): stmt(blockKind), block_(0), numStmts_(s->size()) {
        if (numStmts_) {
            block_ = new (C) stmt*[numStmts_];
            memcpy(block_, &(s->front()), numStmts_ * sizeof(*block_));
        }
    }

    stmt::child_iterator child_begin() { return &block_[0]; }
    stmt::child_iterator child_end() { return &block_[0]+numStmts_; }

    static bool classof(const block* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == blockKind; }

};


// declaration base class
class decl: public stmt {

public:
    // see astNodes.def
    static const nodeKind firstDeclKind = formalParamKind;
    static const nodeKind lastDeclKind = functionDeclKind;

    decl(nodeKind k): stmt(k) { }

    static bool classof(const decl* s) { return true; }
    static bool classof(const stmt* s) {
        return s->kind() >= firstDeclKind &&
               s->kind() <= lastDeclKind;
    }

};

class staticDecl: public decl {

    llvm::PooledStringPtr name_;
    stmt* default_;

public:
    staticDecl(const pSourceRange& name, pParseContext& C, expr* def=NULL):
        decl(staticDeclKind),
        name_(C.idPool().intern(pStringRef(name.begin().base(), (name.end()-name.begin())))),
        default_(def)
    {
    }

    pIdentString name(void) const {
        assert(name_);
        return *name_;
    }

    stmt::child_iterator child_begin() { return &default_; }
    stmt::child_iterator child_end() { return &default_+1; }

    static bool classof(const staticDecl* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == staticDeclKind; }

};


class formalParam: public decl {

    enum { byRefBit=1, arrayHintBit=2 };

    llvm::PooledStringPtr name_;
    llvm::PooledStringPtr classHint_;
    pUInt flags_;
    stmt* default_;

public:
    formalParam(const pSourceRange& name, pParseContext& C, bool ref, expr* def=NULL):
        decl(formalParamKind),
        name_(C.idPool().intern(pStringRef(name.begin().base(), (name.end()-name.begin())))),
        classHint_(),
        flags_(0),
        default_(def)
    {
        if (ref) {
            flags_ ^= byRefBit;
        }
    }

    bool byRef(void) const { return flags_ & byRefBit; }

    bool arrayHint(void) const { return flags_ & arrayHintBit; }

    bool optional(void) const {  return default_ != NULL; }

    void setClassHint(const pSourceRange& name, pParseContext& C) {
        classHint_ = C.idPool().intern(pStringRef(name.begin().base(), (name.end()-name.begin())));
    }

    pIdentString name(void) const {
        assert(name_);
        return *name_;
    }

    stmt::child_iterator child_begin() { return &default_; }
    stmt::child_iterator child_end() { return &default_+1; }

    static bool classof(const formalParam* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == formalParamKind; }

};

typedef llvm::SmallVector<formalParam*,RPHP_FORMAL_PARAM_VECTOR_SIZE> formalParamList;

class signature: public decl {

    llvm::PooledStringPtr name_;
    stmt** formalParamList_;
    pUInt numParams_;
    bool returnByRef_;

public:
    signature(const pSourceRange& name,
              pParseContext& C,
              const formalParamList* s,
              bool returnByRef=false):
            decl(signatureKind),
            name_(C.idPool().intern(pStringRef(name.begin().base(), (name.end()-name.begin())))),
            formalParamList_(0),
            numParams_(s->size()),
            returnByRef_(returnByRef)
    {
        if (numParams_) {
            formalParamList_ = new (C) stmt*[numParams_];
            memcpy(formalParamList_, &(s->front()), numParams_ * sizeof(*formalParamList_));
        }
    }

    pIdentString name(void) const {
        assert(name_);
        return *name_;
    }

    bool returnByRef(void) const { return returnByRef_; }

    stmt::child_iterator child_begin() { return &formalParamList_[0]; }
    stmt::child_iterator child_end() { return &formalParamList_[0]+numParams_; }

    static bool classof(const signature* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == signatureKind; }

};

// function declaration
class functionDecl: public decl {

    enum { SIG, BODY, END_EXPR };
    stmt* children_[END_EXPR];

public:
    functionDecl(signature* sig, block* body):
        decl(functionDeclKind),
        children_()
    {
        children_[SIG] = sig;
        children_[BODY] = body;
    }

    signature* sig(void) { return static_cast<signature*>(children_[SIG]); }
    block* body(void) { return static_cast<block*>(children_[BODY]); }

    stmt::child_iterator child_begin() { return (stmt**)&children_[0]; }
    stmt::child_iterator child_end() { return (stmt**)&children_[0]+END_EXPR; }

    static bool classof(const functionDecl* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == functionDeclKind; }

};

// if statement
class ifStmt: public stmt {

    enum { CONDITION, TRUEBLOCK, FALSEBLOCK, END_EXPR };
    stmt* children_[END_EXPR];

public:
    ifStmt(expr* cond,
           stmt* trueBlock,
           stmt* falseBlock):
               stmt(ifStmtKind),
               children_() {

        children_[CONDITION] = static_cast<stmt*>(cond);
        children_[TRUEBLOCK] = trueBlock;
        children_[FALSEBLOCK] = falseBlock;

    }

    stmt::child_iterator child_begin() { return (stmt**)&children_[0]; }
    stmt::child_iterator child_end() { return (stmt**)&children_[0]+END_EXPR; }

    expr* condition(void) { return static_cast<expr*>(children_[CONDITION]); }
    stmt* trueBlock(void) { return children_[TRUEBLOCK]; }
    stmt* falseBlock(void) { return children_[FALSEBLOCK]; }

    static bool classof(const ifStmt* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == ifStmtKind; }

};

// foreach statement
class forEach: public stmt {

    enum { RVAL, BODY, END_EXPR };
    stmt* children_[END_EXPR];

    llvm::PooledStringPtr value_;
    llvm::PooledStringPtr key_; // may be empty
    bool byRef_;

public:
    forEach(expr* rVal,
            block* body,
            pParseContext& C,
            const pSourceRange& val,            
            bool byRef,
            const pSourceRange* key=NULL):
    stmt(forEachKind),
    children_(),
    value_(C.idPool().intern(pStringRef(val.begin().base(), (val.end()-val.begin())))),
    key_(),
    byRef_(byRef)
    {

        children_[RVAL] = static_cast<stmt*>(rVal);
        children_[BODY] = static_cast<stmt*>(body);
        if (key) {
            key_ = C.idPool().intern(pStringRef(key->begin().base(), (key->end()-key->begin())));
        }

    }

    stmt::child_iterator child_begin() { return (stmt**)&children_[0]; }
    stmt::child_iterator child_end() { return (stmt**)&children_[0]+END_EXPR; }

    expr* rVal(void) { return static_cast<expr*>(children_[RVAL]); }
    block* body(void) { return static_cast<block*>(children_[BODY]); }

    bool hasKey(void) const { return key_; }

    pIdentString key(void) const {
        if (key_)
            return *key_;
        else
            return pIdentString();
    }

    pIdentString value(void) const {
        assert(value_);
        return *value_;
    }

    static bool classof(const forEach* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == forEachKind; }

};

typedef llvm::SmallVector<stmt*,RPHP_GLOBAL_VECTOR_SIZE> globalItemList;

// global
class globalStmt: public stmt {
    stmt** varList_;
    pUInt numVars_;
public:

    globalStmt(pParseContext& C, const globalItemList* s): stmt(globalStmtKind), varList_(0), numVars_(s->size()) {
        if (numVars_) {
            varList_ = new (C) stmt*[numVars_];
            memcpy(varList_, &(s->front()), numVars_ * sizeof(*varList_));
        }
    }

    stmt::child_iterator child_begin() { return &varList_[0]; }
    stmt::child_iterator child_end() { return &varList_[0]+numVars_; }

    static bool classof(const globalStmt* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == globalStmtKind; }

};

// type cast
class typeCast: public expr {
public:
    enum castKindType { STRING, BINARY, UNICODE, INT, REAL, BOOL, UNSET, ARRAY, OBJECT };

private:
    expr* rVal_;
    castKindType castKind_;

public:
    typeCast(castKindType kind, expr* rVal): expr(typeCastKind), rVal_(rVal), castKind_(kind)
    {

    }

    stmt::child_iterator child_begin() { return reinterpret_cast<stmt**>(&rVal_); }
    stmt::child_iterator child_end() { return reinterpret_cast<stmt**>(&rVal_+1); }

    expr* rVal(void) { return rVal_; }
    castKindType castKind(void) const { return castKind_; }

    static bool classof(const typeCast* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == typeCastKind; }

};

// return statement
class returnStmt: public stmt {
    expr* rVal_;
public:
    returnStmt(expr* rVal): stmt(returnStmtKind), rVal_(rVal)
    {

    }

    stmt::child_iterator child_begin() { return reinterpret_cast<stmt**>(&rVal_); }
    stmt::child_iterator child_end() { return reinterpret_cast<stmt**>(&rVal_+1); }

    expr* rVal(void) { return rVal_; }

    static bool classof(const returnStmt* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == returnStmtKind; }

};

// builtins: exit, isset, unset, empty
class builtin: public expr {
public:
    enum opKind { EXIT, ISSET, UNSET, EMPTY };

private:
    stmt** children_;
    pUInt numChildren_;
    opKind opKind_;

public:

    builtin(pParseContext& C, opKind op, const expressionList* s=NULL):
            expr(builtinKind),
            children_(0),
            numChildren_(0),
            opKind_(op)
    {
        if (s) {
            numChildren_ = s->size();
            children_ = new (C) stmt*[numChildren_];
            memcpy(children_, &(s->front()), numChildren_ * sizeof(*children_));
        }
    }

    stmt::child_iterator child_begin() { return &children_[0]; }
    stmt::child_iterator child_end() { return &children_[0]+numChildren_; }

    static bool classof(const builtin* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == builtinKind; }

};

// literal expression base class
class literalExpr: public expr {

protected:
    pSourceString stringVal_;

public:

    // see astNodes.def
    static const nodeKind firstLiteralKind = literalStringKind;
    static const nodeKind lastLiteralKind = inlineHtmlKind;

    literalExpr(nodeKind k): expr(k), stringVal_() { }
    literalExpr(nodeKind k, const pSourceRange& v): expr(k), stringVal_(v.begin(), v.end()) { }

    virtual const pSourceString& getStringVal(void) const {
        return stringVal_;
    }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

    static bool classof(const literalExpr* s) { return true; }
    static bool classof(const stmt* s) {
        return s->kind() >= firstLiteralKind &&
               s->kind() <= lastLiteralKind;
    }

};

// NODE: literal bstring
class literalString: public literalExpr {

    bool isBinary_;
    bool isSimple_; // i.e., single quoted

    // this is storage for a string that was artificially created
    // during a pass and therefore doesn't exist in the original source
    pSourceString* artificial_;

public:

    // empty source string
    literalString(bool isBinary):
            literalExpr(literalStringKind),
            isBinary_(isBinary),
            isSimple_(true),
            artificial_(0) { }

    // normal source string
    literalString(const pSourceRange& v, bool isBinary):
            literalExpr(literalStringKind, v),
            isBinary_(isBinary),
            isSimple_(true),
            artificial_(0) { }

    // extending string (inline html)
    literalString(const pSourceRange& v, nodeKind k):
            literalExpr(k, v),
            isBinary_(false),
            isSimple_(true),
            artificial_(0) { }

    // artificial constructor (creates storage space)
    literalString(pStringRef r):
            literalExpr(literalStringKind),
            isBinary_(true),
            isSimple_(true),
            artificial_(new pSourceString(r)) { }

    ~literalString(void) {
        if (artificial_)
            delete artificial_;
    }

    bool isBinary(void) const { return isBinary_; }

    void setIsSimple(bool s)  {
        isSimple_ = s;
    }

    void setStringVal(pStringRef s) {
        if (artificial_)
            delete artificial_;
        artificial_ = new pSourceString(s);
    }

    const pSourceString& getStringVal(void) const {
        if (artificial_)
            return *artificial_;
        else
            return stringVal_;
    }

    bool isSimple(void) const { return isSimple_; }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

    static bool classof(const literalString* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == literalStringKind; }

};

// NODE: literal int
class literalInt: public literalExpr {

    bool negative_;

public:
    literalInt(const pSourceRange& v): literalExpr(literalIntKind, v), negative_(false) { }

    bool negative(void) const { return negative_; }
    void setNegative(bool n) { negative_ = n; }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

    static bool classof(const literalInt* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == literalIntKind; }

};

// NODE: literal float
class literalFloat: public literalExpr {

public:
    literalFloat(const pSourceRange& v): literalExpr(literalFloatKind, v) { }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

    static bool classof(const literalFloat* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == literalFloatKind; }

};

// NODE: literal bool
class literalBool: public literalExpr {

    bool boolVal_;

public:
    literalBool(bool v): literalExpr(literalBoolKind), boolVal_(v) { }

    bool getBoolVal(void) const { return boolVal_; }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

    static bool classof(const literalBool* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == literalBoolKind; }

};

// array items
struct arrayItem {
    expr* key;
    expr* val;
    bool isRef;
    arrayItem(expr* k, expr* v, bool r):
     key(k),
     val(v),
     isRef(r)
     { }
};

typedef std::vector<arrayItem> arrayList;

// NODE: literal array
class literalArray: public literalExpr {

    arrayList itemList_;

public:
    literalArray(arrayList* items):
        literalExpr(literalArrayKind),
        itemList_(*items) // copy
        { }

    virtual void doDestroy(pParseContext& C) {
        for (arrayList::iterator i = itemList_.begin(); i != itemList_.end(); ++i) {
            if ((*i).key) {
                (*i).key->destroy(C);
            }
            (*i).val->destroy(C);
        }
        stmt::doDestroy(C);
    }

    arrayList& itemList(void) { return itemList_; }
    const arrayList& itemList(void) const { return itemList_; }

    static bool classof(const literalArray* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == literalArrayKind; }

};


// NODE: inline html
class inlineHtml: public literalString {

public:
    inlineHtml(const pSourceRange& v): literalString(v, inlineHtmlKind) { }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

    static bool classof(const inlineHtml* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == inlineHtmlKind; }

};


// NODE: literal null
class literalNull: public literalExpr {

public:
    literalNull(void): literalExpr(literalNullKind) { }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

    static bool classof(const literalNull* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == literalNullKind; }

};

// NODE: echo statement
class echoStmt: public stmt {

    expr* rVal_;

public:
    echoStmt(expr* v): stmt(echoStmtKind), rVal_(v) { }

    stmt::child_iterator child_begin() { return reinterpret_cast<stmt**>(&rVal_); }
    stmt::child_iterator child_end() { return reinterpret_cast<stmt**>(&rVal_+1); }

    expr* rVal(void) { return rVal_; }

    static bool classof(const echoStmt* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == echoStmtKind; }

};

// NODE: var
class var: public expr {

    llvm::PooledStringPtr name_;

    // children_[0] is always target, which may be null. the rest will be array indices
    // numChildren_ is always 1 + number of indices
    stmt** children_;
    pUInt numChildren_;

public:
    var(const pSourceRange& name, pParseContext& C, expr* target = NULL):
        expr(varKind),
        name_(C.idPool().intern(pStringRef(name.begin().base(), (name.end()-name.begin())))),
        children_(NULL),
        numChildren_(1)
    {
        children_ = new (C) stmt*[1];
        children_[0] = target;
    }

    var(const pSourceRange& name, pParseContext& C, expressionList* indices, expr* target = NULL):
        expr(varKind),
        name_(C.idPool().intern(pStringRef(name.begin().base(), (name.end()-name.begin())))),
        children_(NULL),
        numChildren_(1+indices->size())
    {
        children_ = new (C) stmt*[numChildren_];
        children_[0] = target;
        if (numChildren_ > 1) {
            memcpy(children_+1, &(indices->front()), (numChildren_-1) * sizeof(stmt*));
        }
    }

    pIdentString name(void) const {
        assert(name_);
        return *name_;
    }

    expr* target(void) {
        assert((children_[0] == NULL || isa<expr>(children_[0])) && "unknown object in target");
        return static_cast<expr*>(children_[0]);
    }

    stmt::child_iterator child_begin() { return &children_[0]; }
    stmt::child_iterator child_end() { return &children_[0]+numChildren_; }

    pUInt numIndices(void) const { return numChildren_-1; }

    stmt::child_iterator indices_begin() { return &children_[1]; }
    stmt::child_iterator indices_end() { return &children_[1]+(numChildren_-1); }

    static bool classof(const var* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == varKind; }

};

// NODE: assignment
class assignment: public expr {

    enum { LVAL, RVAL, END_EXPR };
    stmt* children_[END_EXPR];
    bool isRef_;

public:
    assignment(expr* lVal, expr* rVal, bool r): expr(assignmentKind), children_(), isRef_(r)
    {
        children_[LVAL] = static_cast<stmt*>(lVal);
        children_[RVAL] = static_cast<stmt*>(rVal);
    }

    expr* lVal(void) { return static_cast<expr*>(children_[LVAL]); }
    expr* rVal(void) { return static_cast<expr*>(children_[RVAL]); }

    bool isRef(void) const { return isRef_; }

    stmt::child_iterator child_begin() { return (stmt**)&children_[0]; }
    stmt::child_iterator child_end() { return (stmt**)&children_[0]+END_EXPR; }

    static bool classof(const assignment* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == assignmentKind; }

};

// NODE: op assignment
class opAssignment: public expr {

public:
    enum opKind { CONCAT,
                  REF };

private:
    enum { LVAL, RVAL, END_EXPR };
    stmt* children_[END_EXPR];
    opKind opKind_;

public:
    opAssignment(expr* lVal, expr* rVal, opKind op): expr(assignmentKind), children_(), opKind_(op)
    {
        children_[LVAL] = static_cast<stmt*>(lVal);
        children_[RVAL] = static_cast<stmt*>(rVal);
    }

    expr* lVal(void) { return static_cast<expr*>(children_[LVAL]); }
    expr* rVal(void) { return static_cast<expr*>(children_[RVAL]); }

    opKind opKind(void) const { return opKind_; }

    stmt::child_iterator child_begin() { return (stmt**)&children_[0]; }
    stmt::child_iterator child_end() { return (stmt**)&children_[0]+END_EXPR; }

    static bool classof(const opAssignment* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == opAssignmentKind; }

};

// NODE: function invoke
class functionInvoke: public expr {

    llvm::PooledStringPtr name_;

    // children_[0] is always target, which may be null. the rest will be array indices
    // numChildren_ is always 1 + number of indices
    stmt** children_;
    pUInt numChildren_;

public:
    functionInvoke(const pSourceRange& name, pParseContext& C, expressionList* argList, expr* target = NULL):
        expr(functionInvokeKind),
        name_(C.idPool().intern(pStringRef(name.begin().base(), (name.end()-name.begin())))),
        children_(NULL),
        numChildren_(1+argList->size())
    {
        children_ = new (C) stmt*[numChildren_];
        children_[0] = target;
        if (numChildren_ > 1) {
            memcpy(children_+1, &(argList->front()), (numChildren_-1) * sizeof(stmt*));
        }
    }

    pIdentString name(void) const {
        assert(name_);
        return *name_;
    }

    expr* target(void) {
        assert((children_[0] == NULL || isa<expr>(children_[0])) && "unknown object in target");
        return static_cast<expr*>(children_[0]);
    }

    pUInt numArgs(void) const { return numChildren_-1; }

    stmt::child_iterator child_begin() { return &children_[0]; }
    stmt::child_iterator child_end() { return &children_[0]+numChildren_; }

    stmt::child_iterator args_begin() { return &children_[1]; }
    stmt::child_iterator args_end() { return &children_[1]+(numChildren_-1); }

    static bool classof(const functionInvoke* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == functionInvokeKind; }

};

// NOP statement such as ;;
class emptyStmt: public stmt {

public:
	emptyStmt() : stmt(emptyStmtKind) {}


    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

    static bool classof(const emptyStmt* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == emptyStmtKind; }

};

// NODE: unary operator
class unaryOp: public expr {

public:
    enum opKind { NEGATIVE, POSITIVE, LOGICALNOT };

private:
    expr* rVal_;
    opKind opKind_;

public:

    unaryOp(expr* rVal, opKind k): expr(unaryOpKind), rVal_(rVal), opKind_(k) {
        if (opKind_ == NEGATIVE) {
            // if our expression is a simple literal int, flag its sign
            if (literalInt* i = dyn_cast<literalInt>(rVal))
                i->setNegative(true);
        }
    }

    expr* rVal(void) { return rVal_; }

    stmt::child_iterator child_begin() { return reinterpret_cast<stmt**>(&rVal_); }
    stmt::child_iterator child_end() { return reinterpret_cast<stmt**>(&rVal_+1); }

    opKind opKind(void) const { return opKind_; }

    static bool classof(const unaryOp* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == unaryOpKind; }

};

// NODE: pre operator
class preOp: public expr {

public:
    enum opKind { INC, DEC };

private:
    expr* rVal_;
    opKind opKind_;

public:

    preOp(expr* rVal, opKind k): expr(preOpKind), rVal_(rVal), opKind_(k)
    {
    }

    expr* rVal(void) { return rVal_; }

    stmt::child_iterator child_begin() { return reinterpret_cast<stmt**>(&rVal_); }
    stmt::child_iterator child_end() { return reinterpret_cast<stmt**>(&rVal_+1); }

    opKind opKind(void) const { return opKind_; }

    static bool classof(const preOp* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == preOpKind; }

};

// NODE: post operator
class postOp: public expr {

public:
    enum opKind { INC, DEC };

private:
    expr* rVal_;
    opKind opKind_;

public:

    postOp(expr* rVal, opKind k): expr(postOpKind), rVal_(rVal), opKind_(k)
    {
    }

    expr* rVal(void) { return rVal_; }

    stmt::child_iterator child_begin() { return reinterpret_cast<stmt**>(&rVal_); }
    stmt::child_iterator child_end() { return reinterpret_cast<stmt**>(&rVal_+1); }

    opKind opKind(void) const { return opKind_; }

    static bool classof(const postOp* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == postOpKind; }

};


// NODE: binary operator
class binaryOp: public expr {

public:
    enum opKind { CONCAT,
                  BOOLEAN_AND,
                  BOOLEAN_OR,
                  DIV,
                  MOD,
                  MULT,
                  ADD,
                  SUB,
                  GREATER_THAN,
                  LESS_THAN,
                  GREATER_OR_EQUAL,
                  LESS_OR_EQUAL,
                  EQUAL,
                  NOT_EQUAL,
                  IDENTICAL,
                  NOT_IDENTICAL,
                  BIT_OR,
                  BIT_AND,
                  BIT_XOR
              };

private:
    enum { LVAL, RVAL, END_EXPR };
    stmt* children_[END_EXPR];
    opKind opKind_;

public:

    binaryOp(expr* lVal, expr* rVal, opKind k): expr(binaryOpKind), children_(), opKind_(k)
    {
        children_[LVAL] = static_cast<stmt*>(lVal);
        children_[RVAL] = static_cast<stmt*>(rVal);
    }

    expr* lVal(void) { return static_cast<expr*>(children_[LVAL]); }
    expr* rVal(void) { return static_cast<expr*>(children_[RVAL]); }

    stmt::child_iterator child_begin() { return (stmt**)&children_[0]; }
    stmt::child_iterator child_end() { return (stmt**)&children_[0]+END_EXPR; }

    static bool classof(const binaryOp* s) { return true; }
    static bool classof(const stmt* s) { return s->kind() == binaryOpKind; }

    opKind opKind(void) const { return opKind_; }

};


} } // namespace

#endif /* RPHP_PAST_H_ */
