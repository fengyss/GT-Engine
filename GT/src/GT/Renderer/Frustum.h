#pragma once
#include"GT/Renderer/Buffer.h"
#include "GT/Core/Asset/Asset.h"
#include "GT/Renderer/Shader.h"
namespace GT
{
    struct Plane
    {
        glm::vec3 Normal;
        float Distance;
    };

    struct Frustum
    {
        Plane Planes[6]; // Left, Right, Bottom, Top, Near, Far
    };

    struct GPUAABB
    {
        alignas(16) glm::vec3 Min;
        alignas(16) glm::vec3 Max;
    };

    class FrustumCullingPass
    {
    public:
        FrustumCullingPass();
        void Execute(const Frustum& frustum, uint32_t count);

        Ref<StorageBuffer> GetVisibleBuffer() { return m_VisibleBuffer; }
        Ref<StorageBuffer> GetAABBBuffer() { return m_AABBBuffer; }

    private:
        Shader m_ComputeShader;
        Ref<StorageBuffer> m_FrustumBuffer;
        Ref<StorageBuffer> m_VisibleBuffer;
        Ref<StorageBuffer> m_AABBBuffer;
        unsigned int m_RendererID = 0;
    };


    static Frustum ExtractFrustum(const glm::mat4& vp)
    {
        Frustum f;

        // ×ó
        f.Planes[0].Normal.x = vp[0][3] + vp[0][0];
        f.Planes[0].Normal.y = vp[1][3] + vp[1][0];
        f.Planes[0].Normal.z = vp[2][3] + vp[2][0];
        f.Planes[0].Distance = vp[3][3] + vp[3][0];

        // ÓÒ
        f.Planes[1].Normal.x = vp[0][3] - vp[0][0];
        f.Planes[1].Normal.y = vp[1][3] - vp[1][0];
        f.Planes[1].Normal.z = vp[2][3] - vp[2][0];
        f.Planes[1].Distance = vp[3][3] - vp[3][0];

        // ÏÂ
        f.Planes[2].Normal.x = vp[0][3] + vp[0][1];
        f.Planes[2].Normal.y = vp[1][3] + vp[1][1];
        f.Planes[2].Normal.z = vp[2][3] + vp[2][1];
        f.Planes[2].Distance = vp[3][3] + vp[3][1];

        // ÉÏ
        f.Planes[3].Normal.x = vp[0][3] - vp[0][1];
        f.Planes[3].Normal.y = vp[1][3] - vp[1][1];
        f.Planes[3].Normal.z = vp[2][3] - vp[2][1];
        f.Planes[3].Distance = vp[3][3] - vp[3][1];

        // ½ü
        f.Planes[4].Normal.x = vp[0][2];
        f.Planes[4].Normal.y = vp[1][2];
        f.Planes[4].Normal.z = vp[2][2];
        f.Planes[4].Distance = vp[3][2];

        // Ô¶
        f.Planes[5].Normal.x = vp[0][3] - vp[0][2];
        f.Planes[5].Normal.y = vp[1][3] - vp[1][2];
        f.Planes[5].Normal.z = vp[2][3] - vp[2][2];
        f.Planes[5].Distance = vp[3][3] - vp[3][2];

        for (auto& p : f.Planes)
        {
            float mag = glm::length(p.Normal);
            p.Normal /= mag;
            p.Distance /= mag;
        }

        return f;
    }

};