/*
References:

https://zhuanlan.zhihu.com/p/29092979
http://allenchou.net/2013/05/memory-management-part-2-of-3-c-style-interface/
http://allenchou.net/2013/05/memory-management-part-3-of-3-stl-compatible-allocators/

*/

/* Replaced new and delete with our allocator.

Example:
    // allocate memory
    unsigned *i = MemoryManager.New<unsigned>();
    A *a = MemoryManager.New<A>(42, "is the answer");
    
    // manipulate memory
    *i = 0u;
    
    // free memory
    MemoryManager.Delete(i);
    MemoryManager.Delete(a);
*/

/* Replaced STL default allocator to our ones.

Example:
    std::vector<int, MemoryManager.allocator<int>> myVector;
    myVector.push_back(1);
    myVector.push_back(2);
    myVector.push_back(3);
    
    std::list<int, MemoryManager.allcator<int>> myList;
    myList.push_back(1);
    myList.push_back(2);
    myList.push_back(3);
*/

#pragma once
#include "IRuntimeModule.hpp"
#include "Allocator.hpp"
#include <new>

namespace My {
    class MemoryManager : implements IRuntimeModule
    {
    public:
        template<typename T, typename... Args>
        T* New(Args&&... args)
        {
            return new (Allocate(sizeof(T))) T(args...);
        }

        template<typename T>
        void Delete(T *p)
        {
            reinterpret_cast<T*>(p)->~T();
            Free(p, sizeof(T));
        }

        /*STL-Compatible Allocators------------------------------*/
        template <typename T>
        class allocator
        {
        public:
            
            typedef T value_type;
            typedef T *pointer;
            typedef T &reference;
            typedef const T *const_pointer;
            typedef const T &const_reference;
            typedef unsigned size_type;
            typedef unsigned difference_type;

            template <typename U>
            struct rebind
            {
                typedef allocator<U> other;
            };
            
            pointer allocate(unsigned n)
            {
                return
                reinterpret_cast<T *>
                (Allocate(sizeof(T) * n));
            }
        
            void deallocate(pointer p, unsigned n)
            {
                Free(p, sizeof(T) * n);
            }
        
            void construct(pointer p, const_reference clone)
            {
                new (p) T(clone);
            }
        
            void destroy(pointer p)
            {
                p->~T();
            }
        
            pointer address(reference x) const
            {
                return &x;
            }
        
            const_pointer address(const_reference x) const
            {
                return &x;
            }
        
            bool operator==(const allocator &rhs)
            {
                return true;
            }
        
            bool operator!=(const allocator &rhs)
            {
                return !operator==(rhs);
            }
        };

    public:
        virtual ~MemoryManager() {}

        virtual int Initialize();
        virtual void Finalize();
        virtual void Tick();

        void* Allocate(size_t size);
        void  Free(void* p, size_t size);
    private:
        static size_t*        m_pBlockSizeLookup;
        static Allocator*     m_pAllocators;
    private:
        static Allocator* LookUpAllocator(size_t size);
    };
}

