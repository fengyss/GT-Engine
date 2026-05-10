#pragma once

#include "GT/Core/Base.h"

#include <string>
#include <vector>
#include <map>

namespace GT {

    // 骨骼关节 (Joint)
    struct Bone
    {
        std::string Name;
        uint32_t Index;
        uint32_t ParentIndex; // 根节点通常为 -1 或 0
        glm::mat4 InverseBindPose; // 逆绑定矩阵 (用于将顶点从模型空间变换到骨骼空间)
        glm::mat4 LocalTransform;  // 相对于父节点的变换
        glm::mat4 GlobalTransform; // 世界空间变换 (每帧计算)
    };

    // 关键帧数据结构
    struct KeyPosition { float Time; glm::vec3 Value; };
    struct KeyRotation { float Time; glm::quat Value; };
    struct KeyScale { float Time; glm::vec3 Value; };

    // 单个骨骼的动画通道
    class BoneAnimation
    {
    public:
        BoneAnimation(const std::string& name, uint32_t boneIndex)
            : m_Name(name), m_BoneIndex(boneIndex) {
        }

        // 根据时间戳插值计算当前的 Transform
        void CalculatePose(float animationTime, glm::mat4& outTransform) const;

        // 辅助函数：查找关键帧索引
        static uint32_t FindKeyIndex(float animationTime, const std::vector<float>& times);

    private:
        std::string m_Name;
        uint32_t m_BoneIndex;
        std::vector<KeyPosition> m_Positions;
        std::vector<KeyRotation> m_Rotations;
        std::vector<KeyScale>    m_Scales;
    };

    // 动画剪辑 (Animation Clip)
    class AnimationClip
    {
    public:
        AnimationClip(const std::string& name, float duration, float ticksPerSecond)
            : m_Name(name), m_Duration(duration), m_TicksPerSecond(ticksPerSecond) {
        }

        void AddBoneAnimation(Ref<BoneAnimation> boneAnim) { m_BoneAnimations.push_back(boneAnim); }

        float GetDurationInTicks() const { return m_Duration; }
        float GetTicksPerSecond() const { return m_TicksPerSecond; }
        const std::vector<Ref<BoneAnimation>>& GetBoneAnimations() const { return m_BoneAnimations; }

    private:
        std::string m_Name;
        float m_Duration;
        float m_TicksPerSecond;
        std::vector<Ref<BoneAnimation>> m_BoneAnimations;
    };

}