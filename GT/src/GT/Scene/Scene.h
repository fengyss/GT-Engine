#pragma once

#include "GT/Core/Timestep.h"
#include "GT/Renderer/EditorCamera.h"
#include "GT/Core/UUID.h"

#include "entt/entt.hpp"

#include "box2d/id.h"

namespace GT
{
	class Entity;
	class ParticleSystem;

	class Scene 
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		void OnUpdateRuntime(Timestep ts);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateSimulation(Timestep ts,EditorCamera& camera);



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

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void OnPhysics2DStart();
		void OnPhysics2DStop();

		void RenderScene(Timestep ts, EditorCamera& camera);
		void RenderScene2D();
		void RenderScene3D();



		void SetName(const std::string& newName) { name = newName; }
		void SetFilePath(const std::filesystem::path& path) { filepath = path; }


		const std::string& GetName() const { return name; }
		const std::filesystem::path& GetFilePath() { return filepath; }
		int substepcount = 4;
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		template<typename T>
		void OnComponentRemoved(Entity entity, T& component);

	private:
		std::string name = "New Scene";
		std::filesystem::path filepath;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		b2WorldId m_WorldID = b2_nullWorldId;
		

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
    
}