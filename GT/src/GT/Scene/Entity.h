#pragma once

#include "GT/Core/Log.h"
#include "GT/Core/UUID.h"
#include "Components.h"
#include "entt/entt.hpp"
#include "Scene.h"

namespace GT
{
	class Entity
	{

	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(uint32_t entity, Scene* scene)
			: m_EntityHandle(static_cast<entt::entity>(entity)), m_Scene(scene) {
		}
		Entity(const Entity& other) = default;
		~Entity() = default;

		template<typename T,typename... Args>
		T& AddComponent(Args&&... args)
		{
			GT_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component!");

			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);

			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			GT_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			GT_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this component!");

			T& component = this->GetComponent<T>();
			m_Scene->OnComponentRemoved<T>(*this, component);

			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }
		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}
		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene;
	};

	// 特化 std::hash 给 GT::Entity
	
}


// 现在可以特化 std::hash
//namespace std {
//	template<>
//	struct hash<GT::Entity> {
//		size_t operator()(const GT::Entity& entity) const {
//			return hash<entt::entity>()(static_cast<entt::entity>(entity));
//		}
//	};
//}


