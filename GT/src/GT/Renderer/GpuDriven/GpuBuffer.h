#pragma once
#include <glad/glad.h>
#include <cstdint>

namespace GT
{

    class GPUBuffer {
    public:
        GPUBuffer(GLenum type, size_t size, GLenum usage)
            : m_Type(type), m_Size(size) {
            glGenBuffers(1, &m_ID);
            glBindBuffer(type, m_ID);
            glBufferData(type, size, nullptr, usage);
            glBindBuffer(type, 0);
        }

        ~GPUBuffer() {
            if (m_ID) glDeleteBuffers(1, &m_ID);
        }

        // 禁止拷贝，允许移动
        GPUBuffer(const GPUBuffer&) = delete;
        GPUBuffer& operator=(const GPUBuffer&) = delete;

        GPUBuffer(GPUBuffer&& other) noexcept : m_ID(other.m_ID), m_Type(other.m_Type), m_Size(other.m_Size) {
            other.m_ID = 0;
        }

        void Upload(const void* data, size_t size) {
            glBindBuffer(m_Type, m_ID);
            glBufferSubData(m_Type, 0, size, data);
            glBindBuffer(m_Type, 0);
        }

        void Bind() const {
            glBindBuffer(m_Type, m_ID);
        }

        void BindBase(GLenum target, uint32_t index) const {
            glBindBufferBase(target, index, m_ID);
        }

        uint32_t GetID() const { return m_ID; }

    private:
        uint32_t m_ID = 0;
        GLenum m_Type;
        size_t m_Size;
    };
}