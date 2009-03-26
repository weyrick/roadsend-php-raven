/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
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

// adapted from http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-on-write

#ifndef COWPTR_HPP
#define COWPTR_HPP

#include <boost/shared_ptr.hpp>

#ifdef RPHP_PVAR_DEBUG
#include <iostream>
#endif

namespace rphp {

/**

    @brief An udderly userful templated Copy-On-Write container. Moo!

    Implements copy-on-write by means of a boost::shared_ptr.
    If a non-const method is invoked, the object is copied.

*/
template <class T>
class CowPtr {
public:
    typedef boost::shared_ptr<T> RefPtr;

private:
    RefPtr sPtr_;

    void detach() {
        T* tmp = sPtr_.get();
        if( !( tmp == 0 || sPtr_.unique() ) ) {
#ifdef RPHP_PVAR_DEBUG
            std::cerr << "COW: detach, full copy" << std::endl;
#endif
            sPtr_ = RefPtr( new T( *tmp ) );
        }
    }

public:
    CowPtr(T* t)
        :   sPtr_(t) {
#ifdef RPHP_PVAR_DEBUG
        std::cerr << "COW: create from T*" << std::endl;
#endif
    }
    CowPtr(const RefPtr& refptr)
        :   sPtr_(refptr) {
#ifdef RPHP_PVAR_DEBUG
        std::cerr << "COW: create from shared_ptr<T>" << std::endl;
#endif
    }
    CowPtr(const CowPtr& cowptr)
        :   sPtr_(cowptr.sPtr_)  {
#ifdef RPHP_PVAR_DEBUG
        std::cerr << "COW: copy construct" << std::endl;
#endif
    }

    CowPtr& operator=(const CowPtr& rhs) {
#ifdef RPHP_PVAR_DEBUG
        std::cerr << "COW: shallow assign" << std::endl;
#endif
        sPtr_ = rhs.sPtr_; // no need to check for self-assignment with boost::shared_ptr
        return *this;
    }

    bool operator==(const CowPtr& rhs) const {
        return (sPtr_ == rhs.sPtr_);
    }

    const T& operator*() const {
        return *sPtr_;
    }

    T& operator*() {
        detach();
        return *sPtr_;
    }

    const T* operator->() const {
        return sPtr_.operator->();
    }

    T* operator->() {
        detach();
        return sPtr_.operator->();
    }
    
    long use_count() const {
        return sPtr_.use_count();
    }
    
};

}

#endif //COWPTR_HPP
