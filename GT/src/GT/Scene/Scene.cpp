#include "gtpch.h"
#include "Scene.h"

#include "glm/glm.hpp"
#include "Components.h"
#include "GT/Renderer/Camera.h"
#include "GT/Renderer/Framebuffer.h"
#include "GT/Renderer/RenderCommand.h"
#include "GT/Renderer/Renderer2D.h"
#include "GT/Renderer/Renderer3D.h"
#include "Entity.h"
#include "GT/Utils/PlatformUtils.h"

#include "ScriptableEntity.h"

#include "GT/Scripting/ScriptEngine.h"

#include "GT/Particle/ParticleSystem.h"
#include "GT/Core/Animation/AnimationSystem.h"

#include "GT/Core/Asset/AssetManager.h"

// Box2D
#include "box2d/box2d.h"

#include "GT/Project/Project.h"

namespace GT
{
    static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
    {
        switch (bodyType)
        {
        case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
        case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
        case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
        }

        GT_CORE_ASSERT(false, "Unknown body type");
        return b2_staticBody;
    }

    Scene::Scene()
    {
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormate::RGBA8,FramebufferTextureFormate::RED_INTEGER,FramebufferTextureFormate::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;

        m_Framebuffer = Framebuffer::Create(fbSpec);

        //AssetMetadata meta;
        //meta.FilePath = Project::GetAssetDirectory() / "Scene.hazel";
        //meta.Name = "Scene";
        //meta.ID = UUID();
        //meta.Type = AssetType::Scene;

        //AssetManager::RegisterMetadata(meta);

    }
    Scene::~Scene()
    {

        //void OnRuntimeStop();

        //void OnSimulationStop();

        //void OnPhysics2DStop();
    }

    template<typename Component>
    static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
    {
        auto view = src.view<Component>();
        for (auto e : view)
        {
            UUID uuid = src.get<IDComponent>(e).ID;
            GT_CORE_ASSERT(enttMap.find(uuid) != enttMap.end(),"Can't find entity!");
            entt::entity dstEnttID = enttMap.at(uuid);

            auto& component = src.get<Component>(e);
            dst.emplace_or_replace<Component>(dstEnttID, component);
        }
    }

    template<typename Component>
    static void CopyComponentIfExists(Entity dst, Entity src)
    {
        if (src.HasComponent<Component>())
            dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
    }

    void Scene::Copy(Ref<Scene> other)
    {

        m_ViewportWidth = other->m_ViewportWidth;
        m_ViewportHeight = other->m_ViewportHeight;
        Name = other->Name;
        filepath = other->filepath;

        auto& srcSceneRegistry = other->m_Registry;
        auto& dstSceneRegistry = m_Registry;
        std::unordered_map<UUID, entt::entity> enttMap;

        // Create entities in new scene
        auto idView = srcSceneRegistry.view<IDComponent>();
        for (auto e : idView)
        {
            UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
            const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
            Entity newEntity = CreateEntityWithUUID(uuid, name);
            enttMap[uuid] = (entt::entity)newEntity;
        }


        // Copy components (except IDComponent and TagComponent)
        CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CircleRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CircleCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<ParticleComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<LightRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<ModelComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<Animator2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<ScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

    }

    Ref<Scene> Scene::MakeCopy(Ref<Scene> other)
    {
        Ref<Scene> newScene = CreateRef<Scene>();

        newScene->m_ViewportWidth = other->m_ViewportWidth;
        newScene->m_ViewportHeight = other->m_ViewportHeight;
		newScene->Name = other->Name;
		newScene->filepath = other->filepath;

        auto& srcSceneRegistry = other->m_Registry;
        auto& dstSceneRegistry = newScene->m_Registry;
        std::unordered_map<UUID, entt::entity> enttMap;

        // Create entities in new scene
        auto idView = srcSceneRegistry.view<IDComponent>();
        for (auto e : idView)
        {
            UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
            const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
            Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
            enttMap[uuid] = (entt::entity)newEntity;
        }


        // Copy components (except IDComponent and TagComponent)
        CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CircleRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CircleCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<ParticleComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<LightRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<ModelComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<Animator2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<ScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

        return newScene;
    }
    void Scene::DuplicateEntity(Entity entity)
    {
        std::string name = entity.GetName();
        Entity newEntity = CreateEntity(name.c_str());

        CopyComponentIfExists<TransformComponent>(newEntity, entity);
        CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
        CopyComponentIfExists<CircleRendererComponent>(newEntity, entity);
        CopyComponentIfExists<CameraComponent>(newEntity, entity);
        CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
        CopyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
        CopyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
        CopyComponentIfExists<CircleCollider2DComponent>(newEntity, entity);
        CopyComponentIfExists<ParticleComponent>(newEntity, entity);
        CopyComponentIfExists<LightRendererComponent>(newEntity, entity);
        CopyComponentIfExists<ModelComponent>(newEntity, entity);
        CopyComponentIfExists<Animator2DComponent>(newEntity, entity);
        CopyComponentIfExists<ScriptComponent>(newEntity, entity);
    }

    void Scene::OnRuntimeStart()
    {
        m_IsRunning = true;
        OnPhysics2DStart();


        // Scripting
        {
            ScriptEngine::OnRuntimeStart(this);
            // Instantiate all script entities

            auto view = m_Registry.view<ScriptComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                ScriptEngine::OnCreateEntity(entity);
            }
        }
    }


    void Scene::OnRuntimeStop()
    {

        m_IsRunning = false;
        ScriptEngine::OnRuntimeStop();
        OnPhysics2DStop();
    }

    void Scene::OnSimulationStart()
    {

        m_IsRunning = true;
        OnPhysics2DStart();
        // Scripting
        {
            ScriptEngine::OnRuntimeStart(this);
            // Instantiate all script entities

            auto view = m_Registry.view<ScriptComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                ScriptEngine::OnCreateEntity(entity);
            }
        }
    }

    void Scene::OnSimulationStop()
    {
        m_IsRunning = false;
        OnPhysics2DStop();
        ScriptEngine::OnRuntimeStop();
    }

    void Scene::OnPhysics2DStart()
    {
        b2Vec2 gravity = { 0.0f, -9.8f };
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = gravity;
        m_WorldID = b2CreateWorld(&worldDef);

        auto view = m_Registry.view<Rigidbody2DComponent>();
        for (auto e : view)
        {
            Entity entity = { e, this };
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();


            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
            bodyDef.position = { transform.Translation.x, transform.Translation.y };
            bodyDef.rotation = b2MakeRot(transform.Rotation.z);

            b2BodyId bodyId = b2CreateBody(m_WorldID, &bodyDef);


            rb2d.RuntimeBody = bodyId;

            if (entity.HasComponent<BoxCollider2DComponent>())
            {
                auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

                b2Polygon box = b2MakeBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);
                ;

                b2ShapeDef shapeDef = b2DefaultShapeDef();
                shapeDef.density = bc2d.Density;
                shapeDef.material.restitution = bc2d.Restitution;
                shapeDef.material.friction = bc2d.Friction;
                b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
                bc2d.RuntimeShape = shapeId;

            }

            if (entity.HasComponent<CircleCollider2DComponent>())
            {
                auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

                b2Circle circleShape;
                circleShape.center = { cc2d.Offset.x, cc2d.Offset.y };
                circleShape.radius = transform.Scale.x * cc2d.Radius;

                b2ShapeDef shapeDef = b2DefaultShapeDef();
                shapeDef.density = cc2d.Density;
                shapeDef.material.restitution = cc2d.Restitution;
                shapeDef.material.friction = cc2d.Friction;

                b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circleShape);
                cc2d.RuntimeShape = shapeId;

            }
        }
    }

    void Scene::OnPhysics2DUpdate(Timestep ts)
    {
        b2World_Step(m_WorldID, ts, substepcount);


        // Retrieve transform from Box2D
        auto view = m_Registry.view<Rigidbody2DComponent>();
        for (auto e : view)
        {
            Entity entity = { e, this };
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

            b2BodyId bodyid = rb2d.RuntimeBody;
            const auto& position = b2Body_GetPosition(bodyid);

            transform.Translation.x = position.x;
            transform.Translation.y = position.y;

            transform.Rotation.z = b2Rot_GetAngle(b2Body_GetRotation(bodyid));
        }
    }

    void Scene::OnPhysics2DStop()
    {
        b2DestroyWorld(m_WorldID);
        m_WorldID = b2_nullWorldId;
    }

    void Scene::RenderScene(Camera& camera)
    {

        float time = Time::GetTime();

        m_Framebuffer->Bind();

        //RenderCommand::SetClearColor({ 0.2,0.5,0.7,1.0 });
        RenderCommand::Clear();
        m_Framebuffer->ClearAttachment(1,-1);

        Renderer2D::BeginScene(camera);
        Renderer3D::BeginScene(camera);
        ParticleRenderer::BeginScene(camera);

        RenderScene2D();
        RenderScene3D();
        ParticleSystem::OnRender(this);

        //Renderer3D::RenderShadowMap(Renderer3D::GetShadowMap());


        ParticleRenderer::EndScene();

        ParticleRenderer::Flush(BlendMode::Alpha);


        Renderer3D::EndScene();


        Renderer2D::EndScene();


        m_Framebuffer->Unbind();


        //time = Time::GetTime() - time;
        //GT_CORE_CRITICAL("GPU time4 : {0} ms!", time);
    }

    void Scene::RenderScene2D()
    {
         //Render all quads
        {
            auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
            for (auto entity : group)
            {
                Renderer2D::SetCurrentEntityID(int(entity));
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.GetTransform(), sprite);
                Renderer2D::SetCurrentEntityID(-1);
            }
        }
        // Render all circles
        {
            auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
            for (auto entity : view)
            {
                Renderer2D::SetCurrentEntityID(int(entity));
                auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
                if(circle.texture) 
                    Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.texture, circle.Thickness, circle.Fade);
                else
                    Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade);
                Renderer2D::SetCurrentEntityID(-1);
            }
        }
        // Render all cubes
        {
            auto view = m_Registry.view<TransformComponent, LightRendererComponent>();
            for (auto entity : view)
            {
                Renderer2D::SetCurrentEntityID(int(entity));
                auto [transform, light] = view.get<TransformComponent, LightRendererComponent>(entity);

                if (light.texture) Renderer2D::DrawCube(transform.GetTransform(), glm::vec4(1.0f), light.texture);
                else Renderer2D::DrawCube(transform.GetTransform(), glm::vec4(1.0f));
                Renderer2D::SetCurrentEntityID(-1);

                Renderer3D::AddLight(light.light,light.GetLightSpaceMatrix());

                switch (light.light.type)
                {
                case LightType::Ambient:
                    break;
                case LightType::Point:
                    break;
                case LightType::Directional:
                    Renderer2D::DrawLine(glm::vec3(0.0f), transform.Translation,glm::vec4(1.0f));
                    break;
                case LightType::Spot:
                    Renderer2D::DrawLine(transform.Translation, transform.Translation+light.light.direction, glm::vec4(1.0f));
                    break;
                }
            }
        }
        // Render all particles
        {
            
        }

       //Renderer2D::DrawLine(glm::vec3(2.0f), glm::vec3(5.0f), glm::vec4(1.0f));
       //Renderer2D::DrawRect(glm::vec3(0.0f), glm::vec2(2.0f), glm::vec4(1.0f));

    }
    void Scene::RenderScene3D()
    {
        // Render all models
        {
            auto view = m_Registry.view<TransformComponent, ModelComponent>();
            for (auto entity : view)
            {

                auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);
                if (!model.model) continue;

                Renderer3D::SetCurrentEntityID(int(entity));
				Renderer3D::DrawModel(transform.GetTransform(), model.model);
                Renderer3D::SetCurrentEntityID(-1);

            }
        }
    }

    void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
    {
        // check is regenerate particle emitter
        {
            auto view = m_Registry.view<ParticleComponent>();
            for (auto entity : view)
            {

                auto& particle = view.get<ParticleComponent>(entity);
                if (particle.IsEmitting == false) continue;
                if (particle.IsRegen)
                {
                    particle.IsRegen = false;
                    particle.Emitter->SetConfig(particle.Config);
                }

            }
        }

        

        SceneUpdate(ts);

        RenderScene(camera);
    }

    void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
    {

        // Update Scripts
        {
            // C# Entity OnUpdate
            auto view = m_Registry.view<ScriptComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                ScriptEngine::OnUpdateEntity(entity, ts);
            }

            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                {
                    //TODO: Move to Scene::OnScenePlay
                    if (!nsc.Instance)
                    {
                        nsc.Instance = nsc.InstantiateScript();
                        nsc.Instance->m_Entity = Entity{ entity,this };
                        nsc.Instance->OnCreate();
                    }
                    nsc.Instance->OnUpdate(ts);
                });
        }

        OnPhysics2DUpdate(ts);

        SceneUpdate(ts);

        // Render
		RenderScene(camera);

    }

    void Scene::SceneUpdate(Timestep ts)
    {
        Animation2DSystem::OnUpdate(this, ts);
        ParticleSystem::OnUpdate(this, ts);

        // update light properties
        {
            auto view = m_Registry.view<TransformComponent, LightRendererComponent>();
            for (auto entity : view)
            {
                auto& [transform, light] = view.get<TransformComponent, LightRendererComponent>(entity);

                light.light.pos = transform.Translation;
                if (light.light.type == LightType::Directional)
                    light.light.direction = transform.Translation;
            }
        }
    }



    void Scene::OnUpdateRuntime(Timestep ts)
    {
        // Update Scripts
        {
            // C# Entity OnUpdate
            auto view = m_Registry.view<ScriptComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                ScriptEngine::OnUpdateEntity(entity, ts);
            }

            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                {
                    //TODO: Move to Scene::OnScenePlay
                    if (!nsc.Instance)
                    {
                        nsc.Instance = nsc.InstantiateScript();
                        nsc.Instance->m_Entity = Entity{ entity,this };
                        nsc.Instance->OnCreate();
                    }
                    nsc.Instance->OnUpdate(ts);
                });
        }


        OnPhysics2DUpdate(ts);

        SceneUpdate(ts);

        // Renderer
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto  view = m_Registry.view<TransformComponent, CameraComponent>();

            view.each([&](auto entity, auto& transform, auto& camera) {
                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
					mainCamera->SetViewMatrix(transform.GetTransform());
                }
                });
        }
        if (mainCamera)
        {
            RenderScene(*mainCamera);
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto  view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }

        m_Framebuffer->Resize(width, height);

    }
    Entity Scene::CreateEntity(const char* name)
    {
        UUID uuid = UUID();
        return CreateEntityWithUUID(uuid, name);
    }
    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };

        entity.AddComponent<IDComponent>(uuid);
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        if (name == "") tag.Tag = "Entity";
        else tag.Tag = name;


        m_EntityMap[uuid] = entt::entity(entity);

        return entity;
    }
    void Scene::DestroyEntity(Entity e)
    {
        if (e.HasComponent<ParticleComponent>()) e.RemoveComponent<ParticleComponent>();
       
        m_EntityMap.erase(e.GetUUID());
        m_Registry.destroy(e);

    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_Registry.view<CameraComponent>();
        for (auto e : view)
        {
            auto& camera = view.get<CameraComponent>(e);
            if (camera.Primary)
            {
                Entity entity = Entity{ e, this };
				camera.Camera.SetViewMatrix(entity.GetComponent<TransformComponent>().GetTransform());
                return entity;
            }
        }
        return Entity{};
    }

    Entity Scene::GetEntityByUUID(UUID uuid)
    {
        // TODO(Yan): Maybe should be assert
        if (m_EntityMap.find(uuid) != m_EntityMap.end())
            return { m_EntityMap.at(uuid), this };

        return {};
    }

    template<typename T>
    inline void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(sizeof(T) == 0, "Unknown component type added to entity!");
        //static_assert(false, "Unknown component type added to entity!");
    }

    template<typename T>
    inline void Scene::OnComponentRemoved(Entity entity, T& component)
    {
        static_assert(sizeof(T) == 0, "Unknown component type added to entity!");
        //static_assert(false, "Unknown component type added to entity!");
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {

    }


    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
        
    }

    template<>
    void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);

		auto view = m_Registry.view<CameraComponent>();
        for (auto e : view)
        {
            if(e != entity && view.get<CameraComponent>(e).Primary)
            {
                view.get<CameraComponent>(e).Primary = false;
			}
        }
    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<LightRendererComponent>(Entity entity, LightRendererComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<ModelComponent>(Entity entity, ModelComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<ParticleComponent>(Entity entity, ParticleComponent& component)
    {
        component.Emitter = CreateRef<ParticleEmitter>(component.Config);
    }

    template<>
    void Scene::OnComponentAdded<Animator2DComponent>(Entity entity, Animator2DComponent& component)
    {
        
        component.CurrentAnimation = CreateRef<AnimationClip>(component.name, component.duration);
        auto tex = entity.GetComponent<SpriteRendererComponent>().texture;
        if(tex) component.isEnable = component.CurrentAnimation->ImportSpriteSheet(tex->GetPath());
		else GT_CORE_ERROR("Animator2DComponent: SpriteRendererComponent texture path is empty!");
    }
    
    template<>
    void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
    {
    }



    template<>
    void Scene::OnComponentRemoved<TagComponent>(Entity entity, TagComponent& component)
    {

    }

    template<>
    void Scene::OnComponentRemoved<TransformComponent>(Entity entity, TransformComponent& component)
    {

    }


    template<>
    void Scene::OnComponentRemoved<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {

    }

    template<>
    void Scene::OnComponentRemoved<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<CameraComponent>(Entity entity, CameraComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {

    }

    template<>
    void Scene::OnComponentRemoved<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<IDComponent>(Entity entity, IDComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<LightRendererComponent>(Entity entity, LightRendererComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<ModelComponent>(Entity entity, ModelComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<ParticleComponent>(Entity entity, ParticleComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<Animator2DComponent>(Entity entity, Animator2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<ScriptComponent>(Entity entity, ScriptComponent& component)
    {
    }
}