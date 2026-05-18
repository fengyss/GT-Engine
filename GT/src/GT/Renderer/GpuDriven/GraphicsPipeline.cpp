#pragma once
#include "gtpch.h"
#include "GraphicsPipeline.h"
#include <fstream>
#include <sstream>

namespace GT
{

    GraphicsPipeline::GraphicsPipeline(Type type)
        : m_Type(type) {
    }

    GraphicsPipeline::~GraphicsPipeline() {
        if (m_Program)
            glDeleteProgram(m_Program);
    }



    void GraphicsPipeline::Bind() const {
        glUseProgram(m_Program);

        if (m_Type == Type::Graphics) {
            if (m_DepthEnabled) glEnable(GL_DEPTH_TEST);
            else glDisable(GL_DEPTH_TEST);

            if (m_BlendEnabled) glEnable(GL_BLEND);
            else glDisable(GL_BLEND);

            if (m_CullEnabled) glEnable(GL_CULL_FACE);
            else glDisable(GL_CULL_FACE);
        }
    }

    void GraphicsPipeline::Unbind() const {
        glUseProgram(0);
    }
}