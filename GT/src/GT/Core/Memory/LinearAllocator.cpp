#include "gtpch.h"
#include "LinearAllocator.h"
#include <cstdlib> // For aligned_alloc or _aligned_malloc
#include <cstring> // For memset if needed
#include "GT/Core/Log.h"
namespace GT {

    LinearAllocator::LinearAllocator(size_t size)
        : m_Size(size), m_Offset(0) {
#if defined(_MSC_VER)
        // Use _aligned_malloc on MSVC
        m_Buffer = (uint8_t*)_aligned_malloc(size, 16);
#else
        // Use posix_memalign on other platforms
        void* ptr = nullptr;
        if (posix_memalign(&ptr, 16, size) != 0) {
            m_Buffer = nullptr;
        } else {
            m_Buffer = (uint8_t*)ptr;
        }
#endif
    }

    LinearAllocator::~LinearAllocator() 
    {
#if defined(_MSC_VER)
        _aligned_free(m_Buffer);
#else
        free(m_Buffer);
#endif
    }

    void* LinearAllocator::Allocate(size_t size, size_t alignment) {
        uintptr_t current = (uintptr_t)m_Buffer + m_Offset;
        uintptr_t aligned = (current + alignment - 1) & ~(alignment - 1);

        size_t padding = aligned - current;

        if (m_Offset + padding + size > m_Size) {
			GT_CORE_ASSERT(false, "LinearAllocator out of memory!");
            return nullptr;
        }

        m_Offset += padding + size;
        return (void*)aligned;
    }

    void LinearAllocator::Reset() {
        m_Offset = 0;
    }

}