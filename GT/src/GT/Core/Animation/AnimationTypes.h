#pragma once

#include "GT/Core/Base.h"

#include <string>
#include <vector>
#include <map>

namespace GT {

    // 2D 动画帧 (用于帧动画)
    struct SpriteFrame
    {
        //Texture2D TextureHandle; // 指向 Texture2D 的句柄
        glm::vec2   UVOffset;      // 纹理偏移 (如果是图集)
        glm::vec2   UVSize;        // 纹理尺寸
        glm::vec2   Pivot;         // 轴心点 (用于旋转)
    };
    struct SpriteRegion
    {
        glm::vec2 UVOffset; // 左下角 UV
        glm::vec2 UVSize;   // UV 宽高
        glm::vec2 PixelOffset; // 像素偏移 (用于 9-patch 或精确碰撞)
        glm::vec2 PixelSize;
    };

    // 动画剪辑 (Animation Clip)
    // 可以是帧动画，也可以是骨骼动画的引用
    class AnimationClip
    {
    public:
        AnimationClip(const std::string& name, float duration)
            : m_Name(name), m_Duration(duration), m_Loop(true) {
        }
        bool ImportSpriteSheet(const std::filesystem::path& texPath);

        // 帧动画接口
        //void AddFrame(const SpriteFrame& frame, float duration)
        //{
        //    m_Frames.push_back(frame);
        //    m_FrameDurations.push_back(duration);
        //}
        void AddFrame(const SpriteRegion& frame, float duration = 0.0f);

        // 骨骼动画接口 (可选)
        //void SetSkeletonData(RefHandle<> skeletonHandle) { m_SkeletonHandle = skeletonHandle; }

        const std::vector<float>& GetFrameDurations() const { return m_FrameDurations; }
        const std::vector<SpriteRegion>& GetFrames() const { return m_Frames; }
        float GetTotalDuration() const { return m_Duration; }
        bool IsLooping() { return m_Loop; }

    private:
        std::string m_Name;
        float m_Duration;
        bool  m_Loop;

        float count = 0;
        float FrameDuration;

        // 帧动画数据
        //std::vector<SpriteFrame> m_Frames;
        std::vector<SpriteRegion> m_Frames;
        std::vector<float>       m_FrameDurations;

        // 骨骼动画数据 (2D)
        //RefHandle<> m_SkeletonHandle;
    };

}