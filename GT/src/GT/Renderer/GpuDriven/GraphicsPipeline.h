#pragma once
#include <string>
#include <unordered_map>

typedef unsigned int GLuint;

namespace GT
{
    class GraphicsPipeline {
    public:
        enum class Type {
            Graphics,
            Compute
        };

        GraphicsPipeline(Type type);
        ~GraphicsPipeline();

        // Shader 管理
        void AttachShader(GLuint program) { m_Program = program; };

        // 固定管线状态（Graphics 专用）
        void SetDepthTest(bool enable) { m_DepthEnabled = enable; };
        void SetBlend(bool enable) { m_BlendEnabled = enable; };
        void SetCullFace(bool enable) { m_CullEnabled = enable; };

        // 绑定
        void Bind() const;
        void Unbind() const;

        inline GLuint GetProgram() const { return m_Program; }

    private:
        Type    m_Type;
        GLuint  m_Program = 0;
        GLuint  m_VAO = 0;

        bool    m_DepthEnabled = true;
        bool    m_BlendEnabled = false;
        bool    m_CullEnabled = true;
    };
}