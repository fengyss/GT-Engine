#include "gtpch.h"
#include "Frustum.h"
#include "GT/Renderer/RenderCommand.h"

namespace GT
{
	FrustumCullingPass::FrustumCullingPass()
	{
      /*  m_ComputeShader = Shader::CreateCompute(
            "assets/shaders/FrustumCulling.comp"
        );*/

        m_FrustumBuffer = StorageBuffer::Create(
            sizeof(glm::vec4) * 6,
            nullptr
        );

        m_AABBBuffer = StorageBuffer::Create(
            sizeof(GPUAABB) * 1024,
            nullptr
        );

        m_VisibleBuffer = StorageBuffer::Create(
            sizeof(uint32_t) * 1024,
            nullptr
        );
	}
	void FrustumCullingPass::Execute(const Frustum& frustum, uint32_t count)
	{
        m_FrustumBuffer->SetData(frustum.Planes, sizeof(glm::vec4) * 6);

        m_ComputeShader->Bind();
        m_FrustumBuffer->Bind(0);
        m_AABBBuffer->Bind(1);
        m_VisibleBuffer->Bind(2);

        uint32_t groupCount = (count + 63) / 64;

        RenderCommand::DispatchCompute(groupCount, 1, 1);

        RenderCommand::SetMemoryBarrier(MemoryBarrierType::ShaderStorage);
	}
}

