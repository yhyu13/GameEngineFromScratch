#include <stddef.h>
#include <stdint.h>
#include <mutex>

#define T_SAFE 0

namespace My {

    struct BlockHeader {
        // union-ed with data
        BlockHeader* pNext;
    };

    struct PageHeader {
        PageHeader* pNext;
        BlockHeader* Blocks() {
                return reinterpret_cast<BlockHeader*>(this + 1);
        }
    };

    class Allocator {
        public:
                // debug patterns
                static const uint8_t PATTERN_ALIGN = 0xFC;
                static const uint8_t PATTERN_ALLOC = 0xFD;
                static const uint8_t PATTERN_FREE  = 0xFE;

                Allocator() noexcept;
                explicit Allocator(size_t data_size, size_t page_size, size_t alignment) noexcept;
                ~Allocator() noexcept;

                // disable copy & assignment
                Allocator(const Allocator& clone) = delete;
                Allocator &operator=(const Allocator &rhs) = delete;

                // resets the allocator to a new configuration
                void Reset(size_t data_size, size_t page_size, size_t alignment) noexcept;

                // alloc and free blocks
                void* Allocate() noexcept;
                void  Free(void* p) noexcept;
                void  FreeAll() noexcept;
        private:
#if defined(_DEBUG)
                // fill a free page with debug patterns
                void FillFreePage(PageHeader* pPage) noexcept;

                // fill a block with debug patterns
                void FillFreeBlock(BlockHeader* pBlock) noexcept;

                // fill an allocated block with debug patterns
                void FillAllocatedBlock(BlockHeader* pBlock) noexcept;
#endif

                // gets the next block
                BlockHeader* NextBlock(BlockHeader* pBlock) noexcept;

                // the page list
                PageHeader* m_pPageList;

                // the free block list
                BlockHeader* m_pFreeList;

                size_t      m_szDataSize;
                size_t      m_szPageSize;
                size_t      m_szAlignmentSize;
                size_t      m_szBlockSize;
                size_t      m_nBlocksPerPage;

                // statistics
                uint32_t    m_nPages;
                uint32_t    m_nBlocks;
                uint32_t    m_nFreeBlocks;

#if T_SAFE
                std::mutex  mtx;
#endif
    };
}

