#pragma once

#include "GT/Core/Base.h"
#include "GT/Core/Timestep.h"
#include "GT/Renderer/EditorCamera.h"
#include "GT/Core/UUID.h"

#include "entt/entt.hpp"

#include "box2d/id.h"
#include "GT/Core/Asset/Asset.h"

namespace GT
{
	class Entity;
	class ParticleSystem;
	class Framebuffer;

	class Scene : public Asset
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> MakeCopy(Ref<Scene> other);
		void Copy(Ref<Scene> other);


		static AssetType GetStaticType() { return AssetType::Scene; }

		virtual AssetType GetType() const { return GetStaticType(); }


		virtual uint32_t GetMemorySize() const override { return 0; }


		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateSimulation(Timestep ts,EditorCamera& camera);

		void SceneUpdate(Timestep ts);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void OnPhysics2DStart();
		void OnPhysics2DUpdate(Timestep ts);
		void OnPhysics2DStop();

		void RenderScene(Camera& camera);
		void RenderScene2D();
		void RenderScene3D();

		Entity GetEntityByUUID(UUID uuid);

		Ref<Framebuffer> GetFramebuffer() { return m_Framebuffer; }

		void SetFramebuffer(Ref<Framebuffer> framebuffer) { m_Framebuffer = framebuffer; }

		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);

		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
		//
		Entity CreateEntity(const char* name = "");
		void DestroyEntity(Entity e);
		entt::registry& Reg() { return m_Registry; }

		Entity GetPrimaryCameraEntity();

		bool IsRunning() const { return m_IsRunning; }

		void SetName(const std::string& newName) { Name = newName; }
		void SetFilePath(const std::filesystem::path& path) { filepath = path; }


		virtual const std::string& GetName() const override { return Name; }
		const std::filesystem::path& GetFilePath() { return filepath; }
		int substepcount = 4;

		Ref<Framebuffer> m_Framebuffer;

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		template<typename T>
		void OnComponentRemoved(Entity entity, T& component);

	private:

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		std::filesystem::path filepath;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		bool m_IsRunning = false;

		b2WorldId m_WorldID = b2_nullWorldId;
		

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
    
}