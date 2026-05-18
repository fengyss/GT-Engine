#pragma once
namespace GT
{

    class GPUBuffer;
    class GraphicsPipeline;
    class DescriptorSet;

    struct alignas(16) FrameConstants {
        glm::mat4 viewProj;
        glm::vec4 cameraPos;
        uint32_t frameIndex;
        uint32_t totalInstances;
    };

    struct GPUMesh {
        uint32_t vertexOffset;   // 在大数组中的起始顶点索引
        uint32_t indexOffset;    // 在大数组中的起始索引偏移
        uint32_t indexCount;     // 这个 Meshlet 有多少个索引
    };

    struct alignas(16) GPUInstance {
        glm::mat4 worldMatrix;

        glm::vec4 boundsCenter;  // 世界空间的包围盒中心
        glm::vec4 boundsExtents; // 包围盒大小

        uint32_t meshID;         // 指向 GPUMeshlet
        uint32_t materialID;     // 指向材质数据
        uint32_t _pad[2];        // 内存对齐
    };
    struct DrawIndexedIndirectCommand {
        uint32_t indexCount;
        uint32_t instanceCount;
        uint32_t firstIndex;
        int32_t  baseVertex;
        uint32_t baseInstance;
    };
	class GPUDrivenRenderer
	{
    public:
        GPUDrivenRenderer();
        ~GPUDrivenRenderer();

        // 初始化 GPU 资源
        bool Initialize(uint32_t maxInstances = 100000);

        // 每帧更新（CPU端收集数据）
        void BeginFrame(const glm::mat4& viewProj, const glm::vec3& camPos);

        // 提交一个可渲染实例
        void SubmitInstance(const GPUInstance& instance);

        // 执行 GPU 剔除并渲染
        void Render();

    private:
        // GPU 资源句柄
        Scope<GPUBuffer> m_FrameUBO;
        Scope<GPUBuffer> m_InstanceSSBO;
        Scope<GPUBuffer> m_DrawCommandBuffer;
        Scope<GPUBuffer> m_AtomicCounterBuffer;

        // 管线状态
        Scope<GraphicsPipeline> m_CullingComputePipeline;
        Scope<GraphicsPipeline> m_GraphicsPipeline;

        // 描述符集 (绑定 UBO/SSBO/Textures)
        Scope<DescriptorSet> m_DescriptorSet;

        // CPU 端暂存数据
        std::vector<GPUInstance> m_CPUInstanceCache;
        FrameConstants m_CurrentFrameData;

        uint32_t m_MaxInstances;
        uint32_t m_CurrentInstanceCount;
	};
}


