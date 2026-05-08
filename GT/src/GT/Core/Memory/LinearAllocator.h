// GT/Core/Memory/LinearAllocator.h
#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>

namespace GT {

    class LinearAllocator {
    public:
        explicit LinearAllocator(size_t size);
        ~LinearAllocator();

        // 分配内存
        void* Allocate(size_t size, size_t alignment = 16);

        // 重置（每帧调用一次）
        void Reset();

        // 禁止拷贝
        LinearAllocator(const LinearAllocator&) = delete;
        LinearAllocator& operator=(const LinearAllocator&) = delete;

    private:
        uint8_t* m_Buffer;     // 内存起始地址
        size_t   m_Size;       // 总大小
        size_t   m_Offset;     // 当前偏移量
    };

}