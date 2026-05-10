#pragma once
#include "GT/Scene/Scene.h"
#include "GT/Scene/Entity.h"
#include "GT/Core/Base.h"


namespace GT
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
        void SetContexts(std::vector<Ref<Scene>>& context) { m_SceneHistory = &context; }

		void SetSelectedEntity(Entity entity);

		Entity GetSelectedEntity() const { return m_SelectedEntity; }
		Ref<Scene> GetActiveContext() const { return m_ActiveContext; }
		Ref<Scene> GetDeletedContext() const { return m_DeletedContext; }

		bool IsSetNewActiveContext() const { return m_NewSetContext != nullptr; }
		bool IsDeletedActiveContext() const { return m_ActiveContext == nullptr; }

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity e);
		void DrawComponents(Entity& e);

	private:
		Ref<Scene> m_ActiveContext;
		Ref<Scene> m_NewSetContext;
		Ref<Scene> m_DeletedContext;

		std::vector<Ref<Scene>>* m_SceneHistory;
		Entity m_SelectedEntity;
		friend class Scene;
	};

}

