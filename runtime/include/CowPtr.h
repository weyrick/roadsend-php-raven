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

#include <iostream>

namespace rphp {

template <class T>
class CowPtr
{
    public:
        typedef boost::shared_ptr<T> RefPtr;
 
    private:
        RefPtr m_sp;
 
        void detach()
        {
            std::cout << "detatching" << std::endl;
            T* tmp = m_sp.get();
            if( !( tmp == 0 || m_sp.unique() ) ) {
                m_sp = RefPtr( new T( *tmp ) );
            }
        }
 
    public:
        CowPtr(T* t)
            :   m_sp(t)
        {
            
        }
        CowPtr(const RefPtr& refptr)
            :   m_sp(refptr)
        {
            
        }
        CowPtr(const CowPtr& cowptr)
            :   m_sp(cowptr.m_sp)
        {
            
        }
        CowPtr& operator=(const CowPtr& rhs)
        {
            m_sp = rhs.m_sp; // no need to check for self-assignment with boost::shared_ptr
            return *this;
        }
        const T& operator*() const
        {
            return *m_sp;
        }
        T& operator*()
        {
            detach();
            return *m_sp;
        }
        const T* operator->() const
        {
            return m_sp.operator->();
        }
        T* operator->()
        {
            detach();
            return m_sp.operator->();
        }
};

}

#endif //COWPTR_HPP
