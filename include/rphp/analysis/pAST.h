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

namespace rphp {

class pSourceModule;

namespace AST {

// NOTE: if you change this, check static dispatch table in pASTVisitors.cpp!
enum nodeKind {
    blockKind,
    functionDeclKind,
    ifStmtKind,
    echoStmtKind,
    inlineHtmlKind,
    literalStringKind,
    literalIntKind,
    literalFloatKind,
    literalNullKind,
    literalBoolKind,
    literalArrayKind,
    logicalNotKind,
    assignmentKind,
    varKind,
    functionInvokeKind,
    constructorInvokeKind,
    emptyStmtKind,
    unaryArithmeticOpKind
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

    nodeKind getKind(void) const { return kind_; }

    pUInt getStartLine(void) const { return startLineNum_; }

    void setLine(pUInt start) { startLineNum_ = start; endLineNum_ = start; }
    void setLine(pUInt start, pUInt end) { startLineNum_ = start; endLineNum_ = end; }

    pUInt startLineNum(void) const { return startLineNum_; }
    pUInt endLineNum(void) const { return endLineNum_; }

};

typedef std::vector<stmt*> statementList;

// a block of statements
class block: public stmt {
    stmt** block_;
    pUInt numStmts_;
public:

    block(pParseContext& C, const statementList* s): stmt(blockKind), block_(0), numStmts_(s->size()) {
        if (numStmts_) {
            block_ = new (C) stmt*[numStmts_];
            memcpy(block_, &(s->front()), numStmts_ * sizeof(*block_));
        }
    }

    stmt::child_iterator child_begin() { return &block_[0]; }
    stmt::child_iterator child_end() { return &block_[0]+numStmts_; }

};


// declaration base class
class decl: public stmt {

public:
    decl(nodeKind k): stmt(k) { }

};

// expression base class
class expr: public stmt {

public:
    expr(nodeKind k): stmt(k) { }

};

typedef std::vector<expr*> expressionList;

// function declaration
class functionDecl: public decl {

    pFunction* functionDef_;
    block* body_;

public:

    functionDecl(pFunction* def, block* body):
        decl(functionDeclKind),
        functionDef_(def),
        body_(body) { }

    virtual void doDestroy(pParseContext& C) {
        delete functionDef_;
        stmt::doDestroy(C);
    }

    const pFunction* functionDef(void) const { return functionDef_; }
    block* body(void) { return body_; }

    stmt::child_iterator child_begin() { return (stmt**)&body_; }
    stmt::child_iterator child_end() { return (stmt**)&body_+1; }

};

// if statement
class ifStmt: public stmt {

    enum { CONDITION, TRUEBLOCK, FALSEBLOCK, END_EXPR };
    stmt* children_[END_EXPR];

public:
    ifStmt(expr* cond,
           block* trueBlock,
           block* falseBlock):
               stmt(ifStmtKind),
               children_() {

        children_[CONDITION] = static_cast<stmt*>(cond);
        children_[TRUEBLOCK] = static_cast<stmt*>(trueBlock);
        children_[FALSEBLOCK] = static_cast<stmt*>(falseBlock);

    }

    stmt::child_iterator child_begin() { return (stmt**)&children_[0]; }
    stmt::child_iterator child_end() { return (stmt**)&children_[0]+END_EXPR; }

    expr* condition(void) { return static_cast<expr*>(children_[CONDITION]); }
    block* trueBlock(void) { return static_cast<block*>(children_[TRUEBLOCK]); }
    block* falseBlock(void) { return static_cast<block*>(children_[FALSEBLOCK]); }

};

// literal expression base class
class literalExpr: public expr {

    pSourceString stringVal_;

public:
    literalExpr(nodeKind k): expr(k), stringVal_() { }
    literalExpr(nodeKind k, const pSourceRange& v): expr(k), stringVal_(v.begin(), v.end()) { }

    const pSourceString& getStringVal(void) const {
        return stringVal_;
    }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

};

// NODE: literal bstring
class literalString: public literalExpr {

    bool isBinary_;

public:
    literalString(bool isBinary): literalExpr(literalStringKind), isBinary_(isBinary) { }
    literalString(const pSourceRange& v, bool isBinary): literalExpr(literalStringKind, v), isBinary_(isBinary) { }
    literalString(const pSourceRange& v, nodeKind k): literalExpr(k, v), isBinary_(false) { }

    bool isBinary(void) const { return isBinary_; }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

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

};

// NODE: literal float
class literalFloat: public literalExpr {

public:
    literalFloat(const pSourceRange& v): literalExpr(literalFloatKind, v) { }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

};

// NODE: literal bool
class literalBool: public literalExpr {

    bool boolVal_;

public:
    literalBool(bool v): literalExpr(literalBoolKind), boolVal_(v) { }

    bool getBoolVal(void) const { return boolVal_; }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

};

// array items
struct arrayItem {
    expr* key;
    expr* val;
    arrayItem(expr* k, expr* v):
     key(k),
     val(v) { }
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

};


// NODE: literal null
class literalNull: public literalExpr {

public:
    literalNull(void): literalExpr(literalNullKind) { }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

};

// NODE: logical not
class logicalNot: public expr {

    expr* rVal_;

public:
    logicalNot(expr* rVal): expr(logicalNotKind), rVal_(rVal) { }

    expr* rVal(void) { return rVal_; }

    stmt::child_iterator child_begin() { return reinterpret_cast<stmt**>(&rVal_); }
    stmt::child_iterator child_end() { return reinterpret_cast<stmt**>(&rVal_+1); }


};

// NODE: inline html
class inlineHtml: public literalString {

public:
    inlineHtml(const pSourceRange& v): literalString(v, inlineHtmlKind) { }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

};

// NODE: echo statement
class echoStmt: public stmt {

    expr* rVal_;

public:
    echoStmt(expr* v): stmt(echoStmtKind), rVal_(v) { }

    stmt::child_iterator child_begin() { return reinterpret_cast<stmt**>(&rVal_); }
    stmt::child_iterator child_end() { return reinterpret_cast<stmt**>(&rVal_+1); }

    expr* rVal(void) { return rVal_; }

};

// NODE: var
class var: public expr {

    llvm::PooledStringPtr name_;

public:
    var(const pSourceRange& name, pParseContext& C):
        expr(varKind),
        name_(C.idPool().intern(llvm::StringRef(name.begin().base(), (name.end()-name.begin())))) { }

    pIdentString name(void) const {
        assert(name_);
        return *name_;
    }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

};

// NODE: assignment
class assignment: public expr {

    enum { LVAL, RVAL, END_EXPR };
    stmt* children_[END_EXPR];

public:
    assignment(expr* lVal, expr* rVal): expr(assignmentKind), children_()
    {
        children_[LVAL] = static_cast<stmt*>(lVal);
        children_[RVAL] = static_cast<stmt*>(rVal);
    }

    expr* lVal(void) { return static_cast<expr*>(children_[LVAL]); }
    expr* rVal(void) { return static_cast<expr*>(children_[RVAL]); }

    stmt::child_iterator child_begin() { return (stmt**)&children_[0]; }
    stmt::child_iterator child_end() { return (stmt**)&children_[0]+END_EXPR; }

};

// NODE: function invoke
class functionInvoke: public expr {

    llvm::PooledStringPtr name_;
    expressionList argList_;

public:
    functionInvoke(const pSourceRange& name, pParseContext& C, expressionList* argList, nodeKind kind = functionInvokeKind):
        expr(kind),
        name_(C.idPool().intern(llvm::StringRef(name.begin().base(), (name.end()-name.begin())))),
        argList_(*argList) // copy
    {

    }

    virtual void doDestroy(pParseContext& C) {
        for (expressionList::iterator i = argList_.begin(); i != argList_.end(); ++i) {
            (*i)->destroy(C);
        }
        stmt::doDestroy(C);
    }

    pIdentString name(void) const {
        assert(name_);
        return *name_;
    }
    expressionList& argList(void) { return argList_; }
    const expressionList& argList(void) const { return argList_; }

    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

};

// NODE: constructor invoke
class constructorInvoke: public functionInvoke {

public:
    constructorInvoke(const pSourceRange& name, pParseContext& C, expressionList* argList):
        functionInvoke(name, C, argList, constructorInvokeKind)
    {

    }


};

// NOP statement such as ;;
class emptyStmt: public stmt {

public:
	emptyStmt() : stmt(emptyStmtKind) {}


    stmt::child_iterator child_begin() { return child_iterator(); }
    stmt::child_iterator child_end() { return child_iterator(); }

};

// NODE: unary arithmetic operator
class unaryArithmeticOp: public expr {

    expr* rVal_;
    bool negative_;

public:
    unaryArithmeticOp(expr* rVal, bool n): expr(unaryArithmeticOpKind), rVal_(rVal), negative_(n) {
        // if our expression is a simple literal int, flag its sign
        if (rVal_->getKind() == literalIntKind)
            static_cast<literalInt*>(rVal)->setNegative(n);
    }

    expr* rVal(void) { return rVal_; }

    stmt::child_iterator child_begin() { return reinterpret_cast<stmt**>(&rVal_); }
    stmt::child_iterator child_end() { return reinterpret_cast<stmt**>(&rVal_+1); }

    bool negative(void) { return negative_; }

};


} } // namespace

#endif /* RPHP_PAST_H_ */
