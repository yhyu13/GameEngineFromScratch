#pragma once
#include <new>
#include "IRuntimeModule.hpp"
#include "Allocator.hpp"
#include "EngineException.hpp"

namespace My {
    class MemoryManager : implements IRuntimeModule
    {
    public:
        // Replacement for new 
        template<class T, typename... Arguments>
        T* New(Arguments... parameters) noexcept
        {
            return new (Allocate(sizeof(T))) T(parameters...);
        }

        // Replacement for delete
        template<class T>
        void Delete(T* p) noexcept
        {
            p->~T();
            Free(p, sizeof(T));
        }

    public:
        virtual ~MemoryManager() noexcept {};

        virtual int Initialize() noexcept override;
        virtual void Finalize() noexcept override;
        virtual void Tick() noexcept override;

        // Called in Buffer.hpp
        void* Allocate(size_t size, size_t alignment) noexcept;

        // Replacement for malloc()
        void* Allocate(size_t size) noexcept
        {
            return (size <= kMaxBlockSize) ? (m_pAllocators + m_pBlockSizeLookup[size])->Allocate() 
                : malloc(size);
        }

        // Replacement for free()
        void  Free(void* p, size_t size) noexcept
        {
            (size <= kMaxBlockSize) ? (m_pAllocators + m_pBlockSizeLookup[size])->Free(p) 
                : free(p);
        }
    private:
        static const uint32_t kBlockSizes[];

        static const uint32_t kPageSize;
        static const uint32_t kAlignment;

        // number of elements in the block size array
        static const uint32_t kNumBlockSizes;

        // largest valid block size
        static const uint32_t kMaxBlockSize;

        static size_t*        m_pBlockSizeLookup;
        static Allocator*     m_pAllocators;
    private:
        static Allocator* LookUpAllocator(size_t size) noexcept;
    };
}

namespace My {
    extern MemoryManager*   g_pMemoryManager;

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
        
        template <typename U> struct rebind { typedef std::allocator<U> other;};
        
        pointer allocate(unsigned n)
        {
        return
            reinterpret_cast<T *>
            (g_pMemoryManager->Allocate(sizeof(T) * n));
        }

        void deallocate(pointer p, unsigned n)
        {
        g_pMemoryManager->Free(p, sizeof(T) * n);
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
}