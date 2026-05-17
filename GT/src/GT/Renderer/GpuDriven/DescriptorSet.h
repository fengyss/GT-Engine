#pragma once
#include <vector>
#include <cstdint>
#include "glad/glad.h"
namespace GT
{


    class GPUBuffer;

    class DescriptorSet {
    public:
        enum class Type {
            UniformBuffer,
            ShaderStorageBuffer,
            Sampler2D
        };

        struct Binding {
            uint32_t binding;
            Type type;
            uint32_t stageFlags; // VERTEX | FRAGMENT | COMPUTE
            GPUBuffer* buffer;
            GLuint textureID;
        };

        DescriptorSet();
        ~DescriptorSet();

        void AddBinding(uint32_t binding, Type type, GPUBuffer* buffer);
        void AddBinding(uint32_t binding, GLuint textureID);

        void Build();
        void Bind() const;

    private:
        std::vector<Binding> m_Bindings;
        GLuint m_UBOIndex = 0;
    };
}