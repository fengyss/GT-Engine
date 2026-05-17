#pragma once
#include "gtpch.h"
#include "GT/Core/Base.h"
#include "GPUDriven.h"
#include "GpuBuffer.h"
#include "GraphicsPipeline.h"
#include "DescriptorSet.h"
#include "GT/Renderer/Shader.h"
#include "glad/glad.h"
namespace GT
{

    GPUDrivenRenderer::GPUDrivenRenderer()
        : m_MaxInstances(0), m_CurrentInstanceCount(0) {
    }

    GPUDrivenRenderer::~GPUDrivenRenderer() {
        // 智能指针自动释放 GPU 资源
    }

    bool GPUDrivenRenderer::Initialize(uint32_t maxInstances) {
        m_MaxInstances = maxInstances;
        m_CPUInstanceCache.reserve(maxInstances);

        // 1. 创建 UBO (每帧更新的相机矩阵等)
        m_FrameUBO = CreateScope<GPUBuffer>(
            GL_UNIFORM_BUFFER, sizeof(FrameConstants), GL_DYNAMIC_DRAW
        );

        // 2. 创建 SSBO (存储所有物体的 Transform 和 Bounds)
        m_InstanceSSBO = CreateScope<GPUBuffer>(
            GL_SHADER_STORAGE_BUFFER, sizeof(GPUInstance) * maxInstances, GL_DYNAMIC_DRAW
        );

        // 3. 创建 Draw Indirect Buffer (由 Compute Shader 写入)
        m_DrawCommandBuffer = CreateScope<GPUBuffer>(
            GL_DRAW_INDIRECT_BUFFER, sizeof(DrawIndexedIndirectCommand) * maxInstances, GL_STREAM_READ
        );

        // 4. 创建原子计数器 (用于 Compute Shader 统计可见物体数量)
        uint32_t zero = 0;
        m_AtomicCounterBuffer = CreateScope<GPUBuffer>(
            GL_ATOMIC_COUNTER_BUFFER, sizeof(uint32_t), GL_DYNAMIC_DRAW
        );
        m_AtomicCounterBuffer->Upload(&zero, sizeof(uint32_t));

        // 5. 加载并编译 Shader
        // 注意：这里你需要自己实现 Shader 加载逻辑
        m_CullingComputePipeline = CreateScope<GraphicsPipeline>(GraphicsPipeline::Type::Compute);
        Ref<Shader> Cullingshader = Shader::CreateCompute("");
        m_CullingComputePipeline->AttachShader(Cullingshader->GetRendererID());
        m_GraphicsPipeline = CreateScope<GraphicsPipeline>(GraphicsPipeline::Type::Graphics);
        Ref<Shader> Graphicsshader = Shader::CreateCompute("");
        m_GraphicsPipeline->AttachShader(Graphicsshader->GetRendererID());

        return true;
    }

    void GPUDrivenRenderer::BeginFrame(const glm::mat4& viewProj, const glm::vec3& camPos) {
        // 1. 重置 CPU 端缓存
        m_CPUInstanceCache.clear();
        m_CurrentInstanceCount = 0;

        // 2. 更新 Frame UB 数据
        m_CurrentFrameData.viewProj = viewProj;
        m_CurrentFrameData.cameraPos = glm::vec4(camPos, 1.0f);
        m_CurrentFrameData.frameIndex++; // 用于 TAA / 随机化
        m_FrameUBO->Upload(&m_CurrentFrameData, sizeof(FrameConstants));
    }

    void GPUDrivenRenderer::SubmitInstance(const GPUInstance& instance) {
        if (m_CurrentInstanceCount < m_MaxInstances) {
            m_CPUInstanceCache.push_back(instance);
            m_CurrentInstanceCount++;
        }
    }

    void GPUDrivenRenderer::Render() {
        if (m_CurrentInstanceCount == 0) return;

        // 1. 将 CPU 收集的实例数据上传到 GPU SSBO
        m_InstanceSSBO->Upload(
            m_CPUInstanceCache.data(),
            sizeof(GPUInstance) * m_CurrentInstanceCount
        );

        // 2. 重置原子计数器为 0
        uint32_t zero = 0;
        m_AtomicCounterBuffer->Upload(&zero, sizeof(uint32_t));

        // 3. 绑定全局描述符集 (UBO + SSBO + Textures)
        m_DescriptorSet->Bind();

        // --- 阶段一：GPU 视锥体剔除 (Compute Shader) ---
        m_CullingComputePipeline->Bind(); // glUseProgram(computeProgram)

        // 绑定 SSBO 到 binding = 0, 1, 2...
        m_InstanceSSBO->BindBase(GL_SHADER_STORAGE_BUFFER, 0);
        m_DrawCommandBuffer->BindBase(GL_SHADER_STORAGE_BUFFER, 1);
        m_AtomicCounterBuffer->BindBase(GL_ATOMIC_COUNTER_BUFFER, 2);

        // 分发 Compute Shader (每个线程处理一个实例)
        uint32_t groupSize = (m_CurrentInstanceCount + 63) / 64;
        glDispatchCompute(groupSize, 1, 1);

        // 插入内存屏障：确保所有写入完成，且后续 Draw 能读到
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT);

        // --- 阶段二：间接绘制 (Multi-Draw Indirect) ---
        m_GraphicsPipeline->Bind(); // glUseProgram(graphicsProgram)

        // 绑定顶点数组 (VAO 应该在外部由 GTVertexArray 类绑定)
        // glBindVertexArray(m_GlobalVAO); 

        // 绑定间接绘制缓冲区
        m_DrawCommandBuffer->Bind();

        // 核心 API：间接绘制！
        // 参数：图元类型，Indirect Buffer 的偏移量，绘制命令的数量（这里用原子计数器获取）
        glMultiDrawElementsIndirect(
            GL_TRIANGLES,
            GL_UNSIGNED_INT,
            0,
            m_CurrentInstanceCount, // 最大可能数量，原子计数器会限制实际绘制数量
            sizeof(DrawIndexedIndirectCommand)
        );
    }
}