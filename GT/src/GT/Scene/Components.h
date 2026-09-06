#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"


#include "GT/Renderer/Model.h"
#include "GT/Renderer/Texture.h"
#include "GT/Renderer/Shader.h"
#include "SceneCamera.h"

#include "box2d/id.h"
#include "GT/Core/UUID.h"

#include "GT/Particle/ParticleTypes.h"

#include "GT/Core/Animation/AnimationTypes.h"

#include "GT/Core/Asset/Asset.h"

namespace GT
{
    

    struct IDComponent
    {
        UUID ID;

        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
    };

    struct TagComponent
    {
        std::string Tag;
        std::string Type = "None";

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const char* tag)
            :Tag(tag) {}
        TagComponent(std::string tag)
            :Tag(tag) {
        }
        operator const char* () const { return Tag.c_str(); }
        operator const std::string () const { return Tag; }
    };

    struct TransformComponent
    {
		const char* name = "Transform";
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);  //radians or 弧度制
		glm::vec3 Scale = glm::vec3(1.0f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3 translation)
            :Translation(translation) {
        }

        // void DoMaths(const glm::mat4& transform);
        // TransformComponent transform;
        // now we can use DoMaths((glm::mat4)transform) or DoMaths(transform)
        // instand of using DoMaths(transform.Tranform)
        glm::mat4 GetTransform () 
        { 
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Translation) * 
                   rotation *
				glm::scale(glm::mat4(1.0f), Scale);
        }

    };

    struct SpriteRendererComponent
    {
        const char* name = "Sprite Renderer";
        glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        Texture2D texture;
		int TilingFactor = 1;

        glm::vec2 UVOffset = { 0.0f, 0.0f };
        glm::vec2 UVSize = { 1.0f, 1.0f };
        float SortingOrder = 0.0f; // 2D 层级

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4 color)
            :Color(color) {
        }

        operator glm::vec4 () { return Color; }
        operator const glm::vec4& () const { return Color; }
    };
    class ParticleEmitter;
    struct ParticleComponent {
		bool IsEmitting = true;
        bool IsRegen = false;


		ParticleEmitterConfig Config;
        Ref<ParticleEmitter> Emitter;

        // 待用
        std::function<void(Particle&)> init_func;
        std::function<void(Particle&)> update_func;

        ParticleComponent() = default;
		ParticleComponent(const ParticleComponent&) = default;
        ParticleComponent(const ParticleEmitterConfig& config)
            : Config(config) {
        };
    };

    struct LightRendererComponent
    {
        const char* name = "Light Renderer";
        Texture2D texture;

        Light light;


        float orthoSize = 50.0f;
        float nearPlane = 0.2f;
        float farPlane = 100.0f;


        float perspectiveFov = glm::radians(90.0f);
        float perspectiveNear = 1.0f;
        float perspectiveFar = 500.0f;
        float aspectRatio = 0.7f;

        LightRendererComponent() = default;
        LightRendererComponent(const LightRendererComponent&) = default;
        LightRendererComponent(Light _light)
            :light(_light) {
        }

        glm::mat4 GetLightSpaceMatrix();
    };

    struct ModelComponent
    {
        const char* name = "Model Renderer";
        Model model;

        ModelComponent() = default;
        ModelComponent(const ModelComponent&) = default;

    };

    struct CircleRendererComponent
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
        float Thickness = 1.0f;
        float Fade = 0.005f;

        Texture2D texture;

        CircleRendererComponent() = default;
        CircleRendererComponent(const CircleRendererComponent&) = default;
    };

    struct CameraComponent
    {
        const char* name = "Camera";
        SceneCamera Camera;
        glm::vec3 Offset = { 0.0f, 0.0f, 0.0f };
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    class ScriptableEntity;
    struct NativeScriptComponent
    {
        const char* name = "Native Script";
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity* (*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        //std::function<void()> InstantiateFunction;
        //std::function<void()> DestroyInstanceFunction;

        //std::function<void(ScriptableEntity*)> OnCreateFunction;
        //std::function<void(ScriptableEntity*)> OnDestroyFunction;
        //std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

        template<typename T>
        void Bind()
        {
            InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T());};
            DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr;};

            //InstantiateFunction = [&]() {Instance = new T();};
            //DestroyInstanceFunction = [&]() {delete (T*)Instance; Instance = nullptr;};

            //OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
            //OnDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
            //OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(ts); };
        }
    };

    // Physics

    struct Rigidbody2DComponent
    {
        enum class BodyType { Static = 0, Dynamic, Kinematic };
        BodyType Type = BodyType::Static;
        bool FixedRotation = false;

        // Storage for runtime
        b2BodyId RuntimeBody = b2_nullBodyId;

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
    };

    struct BoxCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 0.5f, 0.5f };

        // TODO(Yan): move into physics material in the future maybe
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        // Storage for runtime
        b2ShapeId RuntimeShape = b2_nullShapeId;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
    };


    struct CircleCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        float Radius = 0.5f;

        // TODO(Yan): move into physics material in the future maybe
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        // Storage for runtime
        b2ShapeId RuntimeShape = b2_nullShapeId;

        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
    };


    struct Animator2DComponent
    {
        // 当前播放的动画
        Ref<AnimationClip> CurrentAnimation;

        std::string name = "None";
        float duration = 10.0f;
        bool isEnable = false;
        // 运行时状态
        float CurrentTime = 0.0f;
        uint32_t CurrentFrameIndex = 0;

        // 动画状态机参数 (用于逻辑切换)
        std::unordered_map<std::string, float> FloatParameters;
        std::unordered_map<std::string, bool>  BoolParameters;

        // 构造函数
        Animator2DComponent() = default;
        Animator2DComponent(Ref<AnimationClip> anim)
            : CurrentAnimation(anim) {
        }
    };

    struct ScriptComponent
    {
        std::string ClassName;

        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent&) = default;
    };

    template<typename... Component>
    struct ComponentGroup
    {
    };

    using AllComponents =
        ComponentGroup<IDComponent,TransformComponent, SpriteRendererComponent,
        CircleRendererComponent, CameraComponent, NativeScriptComponent,
        Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent,
        Animator2DComponent,LightRendererComponent,ModelComponent,
        ParticleComponent,ScriptComponent>;

}