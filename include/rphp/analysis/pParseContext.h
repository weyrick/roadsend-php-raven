/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2010 Shannon Weyrick <weyrick@roadsend.com>
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
*/

#ifndef RPHP_PPARSECONTEXT_H_
#define RPHP_PPARSECONTEXT_H_

#include "rphp/pSourceTypes.h"

#include <llvm/Support/Allocator.h>
#include <llvm/Support/StringPool.h>
#include <boost/unordered_map.hpp>


namespace rphp {

class pSourceModule;

namespace AST {

class pParseContext {
private:
    typedef boost::unordered_map<const pSourceRange*, pUInt> lineNumMapType;

    /// This data is only valid during parse (creation of AST), not analysis or transforms
    // line number information will be copied into AST
    pUInt currentLineNum_;
    pSourceCharIterator lastNewline_;
    const pSourceRange* lastToken_;
    lineNumMapType tokenLineInfo_;

    /// Maintains memory of IR during entire analysis and code gen phases
    llvm::BumpPtrAllocator allocator_;

    /// String pool for identifiers to use. Also lives through analysis
    llvm::StringPool idPool_;

    // owning source module
    const pSourceModule* owner_;

public:

    pParseContext(const pSourceModule* o):
        currentLineNum_(0),
        lastNewline_(),
        lastToken_(NULL),
        tokenLineInfo_(),
        allocator_(),
        idPool_(),
        owner_(o)
        { }

    // MEMORY POOL
    llvm::BumpPtrAllocator& allocator(void) { return allocator_; }
    void *allocate(size_t size, size_t align = 8) {
        return allocator_.Allocate(size, align);
    }
    void deallocate(void* Ptr) {
        // note this is a NOOP for bumpptr
        allocator_.Deallocate(Ptr);
    }

    llvm::StringPool& idPool(void) { return idPool_; }

    // PARSING
    pUInt currentLineNum() const { return currentLineNum_; }
    void incLineNum(void) { ++currentLineNum_; }
    void incLineNum(pUInt i) { currentLineNum_ +=i; }

    void setLastNewline(pSourceCharIterator i) { lastNewline_ = i; }
    const pSourceCharIterator& lastNewline(void) const { return lastNewline_; }

    void setLastToken(const pSourceRange* i) { lastToken_ = i; }
    const pSourceRange* lastToken(void) const { return lastToken_; }

    void setTokenLine(const pSourceRange* t) {
        tokenLineInfo_[t] = currentLineNum_;
    }

    pUInt getTokenLine(const pSourceRange* t) {
        lineNumMapType::const_iterator i = tokenLineInfo_.find(t);
        if (i == tokenLineInfo_.end())
            return 0;
        else
            return (*i).second;
    }

    void finishParse(void) {
        currentLineNum_ = 0;
        lastToken_ = NULL;
        tokenLineInfo_.clear();
    }

    // PARSE ERROR HANDLER
    void parseError(pSourceRange* r);
    void parseError(pStringRef msg);

};

} } // namespace

// placement allocators for constructing AST nodes. this is modeled after clang
inline void *operator new(size_t bytes, rphp::AST::pParseContext& C, size_t align=8) throw () {
  return C.allocate(bytes, align);
}

inline void operator delete(void *p, rphp::AST::pParseContext& C) throw () {
  C.deallocate(p);
}

inline void *operator new[](size_t bytes, rphp::AST::pParseContext& C, size_t align = 8) throw () {
  return C.allocate(bytes, align);
}

inline void operator delete[](void *p, rphp::AST::pParseContext& C) throw () {
  C.deallocate(p);
}

#endif /* RPHP_PPARSECONTEXT_H_ */
