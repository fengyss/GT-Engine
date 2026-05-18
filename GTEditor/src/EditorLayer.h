#include "GT.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SpriteSheetPanel.h"
#include "GT/Project/Project.h"
#include "GT/Renderer/Model.h"
#include "imgui/imgui.h"
#include "imguizmo/ImGuizmo.h"


namespace GT
{
	class EditorLayer : public GT::Layer
	{
	public:
		EditorLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(GT::Timestep ts) override;
		void OnEvent(GT::Event& event) override;

		void OnImGuiRender() override;

		void OnMenuBarRender();
		void OnStatusBarRender();
		void OnToolbarRender();
		void OnViewportRender();
		void OnImGuizmoRender();

		void OnSceneHierarchyPanelRender();
		void OnSpriteSheetPanelRender();

		void OnScenePlay();
		void OnSceneSimulate();
		void OnSceneStop();

		const char* GetImGuizmoTypeName()
		{
			switch (m_GizmoType)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
				return "Translate";
			case ImGuizmo::OPERATION::ROTATE:
				return "Rotate";
			case ImGuizmo::OPERATION::SCALE:
				return "Scale";
			default:
				return "Unknown";
			}
		}

	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		
		void OnOverlayRender();

		void CreateNewEditorScene();
		void SetActiveScene(Ref<Scene> scene);

		void OpenProject();
		void OpenProject(std::filesystem::path filepath);

		void OpenScene();
		void OpenScene(std::filesystem::path filepath);
		void NewScene();
		void SaveScene();
		void SaveSceneAs();

	private:
		OrthographicCameraController m_CameraController;

		EditorCamera m_EditorCamera;

		RefHandle<Texture2D> m_IconPlay, m_IconSimulate, m_IconStop;


		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::vector<Ref<Scene>> m_SceneHistory;

		Entity squareentity;
		Entity m_CameraEntity;
		Entity m_HoveredEntity;

		bool IsResized = false;

		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportBounds[2];
		glm::vec2 m_ViewportPos;
		glm::vec2 m_ViewportPosWithoutBar;

		bool m_ViewFocused = false;
		bool m_ViewHovered = false;
		bool m_ViewportActived = false;
		bool m_ShowPhysicsColliders = false;

		int m_GizmoType = ImGuizmo::OPERATION::TRANSLATE; 

		Scope<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;
		Scope<SpriteSheetPanel> m_SpriteSheetPanel;

		Ref<Project> m_Project;

		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};
		SceneState m_SceneState = SceneState::Edit;
	};
}