#include "gtpch.h"
#include "DescriptorSet.h"
#include "GPUBuffer.h"
#include <glad/glad.h>

namespace GT
{


    DescriptorSet::DescriptorSet() {}
    DescriptorSet::~DescriptorSet() {}

    void DescriptorSet::AddBinding(uint32_t binding, Type type, GPUBuffer* buffer) {
        m_Bindings.push_back({ binding, type, 0, buffer, 0 });
    }

    void DescriptorSet::AddBinding(uint32_t binding, GLuint textureID) {
        m_Bindings.push_back({ binding, Type::Sampler2D, 0, nullptr, textureID });
    }

    void DescriptorSet::Build() {
        // OpenGL 4.3+：Uniform Block Binding
        for (auto& b : m_Bindings) {
            if (b.type == Type::UniformBuffer && b.buffer) {
                glUniformBlockBinding(
                    m_UBOIndex,
                    glGetUniformBlockIndex(m_UBOIndex, ("UBO_" + std::to_string(b.binding)).c_str()),
                    b.binding
                );
            }
        }
    }

    void DescriptorSet::Bind() const {
        for (const auto& b : m_Bindings) {
            switch (b.type) {
            case Type::UniformBuffer:
                b.buffer->BindBase(GL_UNIFORM_BUFFER, b.binding);
                break;
            case Type::ShaderStorageBuffer:
                b.buffer->BindBase(GL_SHADER_STORAGE_BUFFER, b.binding);
                break;
            case Type::Sampler2D:
                glActiveTexture(GL_TEXTURE0 + b.binding);
                glBindTexture(GL_TEXTURE_2D, b.textureID);
                break;
            }
        }
    }
    
}