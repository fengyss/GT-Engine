#include "gtpch.h"
//#include "AnimationSystem.h"
//#include "GT/Scene/Components.h"
//#include "GT/Assets/AssetsManager.h"
//
//namespace GT {
//
//    void Animation2DSystem::OnUpdate(Scene* scene, Timestep ts)
//    {
//        // 遍历所有 2D 动画组件
//        auto view = scene->Reg().view<Animator2DComponent, SpriteRendererComponent>();
//
//        for (auto entity : view)
//        {
//            auto [animator, sprite] = view.get<Animator2DComponent, SpriteRendererComponent>(entity);
//
//            Ref<AnimationClip> clip = AssetManager::GetAsset<AnimationClip>(animator.CurrentAnimationHandle);
//            if (!clip || clip->GetFrames().empty())
//                continue;
//
//            // 1. 更新时间
//            animator.CurrentTime += ts;
//
//            // 2. 计算当前帧
//            const auto& frames = clip->GetFrames();
//            const auto& durations = clip->GetFrameDurations();
//
//            float accumulatedTime = 0.0f;
//            uint32_t newFrameIndex = 0;
//
//            for (size_t i = 0; i < durations.size(); ++i)
//            {
//                accumulatedTime += durations[i];
//                if (animator.CurrentTime <= accumulatedTime)
//                {
//                    newFrameIndex = i;
//                    break;
//                }
//            }
//
//            // 3. 处理循环
//            if (animator.CurrentTime > clip->GetTotalDuration())
//            {
//                if (clip->IsLooping())
//                    animator.CurrentTime = 0.0f;
//                else
//                    newFrameIndex = frames.size() - 1; // 停留在最后一帧
//            }
//
//            // 4. 更新 SpriteRenderer (核心)
//            if (newFrameIndex != animator.CurrentFrameIndex)
//            {
//                animator.CurrentFrameIndex = newFrameIndex;
//                const SpriteFrame& frame = frames[newFrameIndex];
//
//                // 更新纹理和 UV
//                sprite.TextureHandle = frame.TextureHandle;
//                sprite.UVOffset = frame.UVOffset;
//                sprite.UVSize = frame.UVSize;
//            }
//        }
//    }
//
//    void UpdateSkeletal2D(Scene* scene, Timestep ts)
//    {
//        // 1. 遍历所有骨骼实体
//        auto boneView = scene->GetRegistry().view<Bone2DComponent>();
//        for (auto boneEntity : boneView)
//        {
//            auto& bone = boneView.get<Bone2DComponent>(boneEntity);
//
//            // 2. 根据动画剪辑计算当前骨骼的 Local Transform
//            glm::vec3 targetPos = CalculateBonePosition(bone, animator.CurrentTime);
//            float targetRot = CalculateBoneRotation(bone, animator.CurrentTime);
//
//            // 3. 应用变换 (使用 Lerp 平滑过渡)
//            auto& transform = scene->GetComponent<TransformComponent>(boneEntity);
//            transform.Translation = glm::lerp(transform.Translation, targetPos, ts * 10.0f);
//            transform.Rotation.z = glm::lerp(transform.Rotation.z, targetRot, ts * 10.0f);
//        }
//
//        // 4. 更新层级 (父节点移动，子节点跟随)
//        // 这部分通常由 Scene Graph 自动处理，无需动画系统干预
//    }
//
//}