#include "EditorLayer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "GT/Scene/SceneSerializer.h"
#include "GT/Utils/PlatformUtils.h"
#include "GT/Math/Math.h"
#include "GT/Renderer/Renderer3D.h"
#include "GT/Particle/ParticleSystem.h"

#include "Panels/SpriteSheetPanel.h"
#include "GT/Renderer/ShadowMap.h"

namespace GT
{


	SceneSerializer serializer;

	glm::vec4 color = { 0.0f,0.0f,0.5f,0.0f };

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(16.0f / 9.0f, true)
	{
		GT_CORE_INFO("EditorLayer Layer Constructed!");

	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		GT_CORE_INFO("EditorLayer Layer Attached!");


		m_IconPlay.Reset(AssetManager::GetAssetHandle("PlayButton"));
		m_IconStop.Reset(AssetManager::GetAssetHandle("StopButton"));
		m_IconSimulate.Reset(AssetManager::GetAssetHandle("SimulateButton"));

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormate::RGBA8,FramebufferTextureFormate::RED_INTEGER,FramebufferTextureFormate::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;

		//m_Framebuffer = Framebuffer::Create(fbSpec);

		
		m_EditorCamera = EditorCamera(45.0f,16.0f/9.0f, 0.1f, 1000.0f);

		m_ViewportSize = { fbSpec.Width,fbSpec.Height };


		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>();

		CreateNewEditorScene();

		SetActiveScene(m_EditorScene);

		m_SceneHierarchyPanel->SetContexts(m_SceneHistory);

		OpenProject("projects\\FirstProject\\FirstProject.hproj");


		m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
		m_AssetsPanel = CreateScope<AssetsPanel>();

		Renderer2D::SetLineWidth(4.0f);


		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			m_ActiveScene = serializer.Deserialize(std::filesystem::path(sceneFilePath));
		}

		m_SpriteSheetPanel = CreateScope<SpriteSheetPanel>();


	}

	void EditorLayer::OnDetach()
	{
		if(m_ActiveScene->IsRunning())
			OnSceneStop();


		m_ContentBrowserPanel.reset();
		m_AssetsPanel.reset();
		m_SceneHierarchyPanel.reset();
		m_SpriteSheetPanel.reset();

		m_ActiveScene.reset();
		m_EditorScene.reset();
		m_SceneHistory.clear();


	}



	bool synced = true;

	bool dockspace = true;

	bool IsEnableTransparentAndMousePassthrough = false;

	int lastx, lasty, lastp;
	int pixelData;
	glm::vec2 pos(0.5f), size(0.2f);
	glm::vec4 _color(1.0f, 0.0f, 0.0f, 1.0f);
	void EditorLayer::OnUpdate(Timestep ts)
	{
		//for(auto& scene : m_SceneHistory)
		//{
		//	if (scene->IsRunning())
		//		scene->OnUpdateRuntime(ts);
		//}

		if (IsResized)
		{
			IsResized = false;
			//m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			float aspecratio = m_ViewportSize.x / m_ViewportSize.y;
			auto view = m_ActiveScene->Reg().view<LightRendererComponent>();
			for (auto e : view)
			{
				auto& light = view.get<LightRendererComponent>(e);
				light.aspectRatio = aspecratio;
			}
		}


		{
			GT_PROFILE_SCOPE("Renderer::Draw");

			Renderer2D::ResetStats();
			Renderer3D::ResetStats();


			//m_ActiveScene->m_Framebuffer->Bind();
			RenderCommand::SetClearColor(color);
			//RenderCommand::Clear();
			//m_ActiveScene->m_Framebuffer->ClearAttachment(1, -1);

			switch (m_SceneState)
			{
				case SceneState::Edit:
				{
					if(m_ViewportActived || !dockspace)
						m_EditorCamera.OnUpdate(ts);

					m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
					break;
				}
				case SceneState::Simulate:
				{
					if (m_ViewportActived)
						m_EditorCamera.OnUpdate(ts);

					m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
					break;
				}
				case SceneState::Play:
				{
					m_ActiveScene->OnUpdateRuntime(ts);
					break;
				}
			}

			m_ActiveScene->m_Framebuffer->Bind();

			Renderer2D::BeginScene(m_EditorCamera);
			switch (m_EditorCamera.GetProjectionType())
			{
			case Camera::ProjectionType::Perspective:
			{
				Renderer2D::DrawLine(glm::vec3(-1000.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				Renderer2D::DrawLine(glm::vec3(0.0f, -1000.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				Renderer2D::DrawLine(glm::vec3(0.0f, 0.0f, -1000.0f), glm::vec3(0.0f, 0.0f, 1000.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			}
			break;
			case Camera::ProjectionType::Orthographic:
			{
				Renderer2D::DrawLine(glm::vec3(-1000.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				Renderer2D::DrawLine(glm::vec3(0.0f, -1000.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			}
			break;

			}
			Renderer2D::EndScene();

			//m_ActiveScene->m_Framebuffer->Unbind();

			{
				// read entity ID
				auto [mx, my] = ImGui::GetMousePos();
				if (mx != lastx || my != lasty)
				{
					lastx = mx;
					lasty = my;
					mx = mx - m_ViewportPosWithoutBar.x;
					my = my - m_ViewportPosWithoutBar.y;

					my = m_ViewportSize.y - my;
					int mouseX = (int)mx;
					int mouseY = (int)my;
					if (mouseX >= 0 && mouseY >= 0 && mouseX < m_ViewportSize.x && mouseY < m_ViewportSize.y)
					{
						pixelData = m_ActiveScene->m_Framebuffer->ReadPixel(1, mouseX, mouseY);
						m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
					}
				}
				OnOverlayRender();

			}
			m_ActiveScene->m_Framebuffer->Unbind();
		}
	}


	void EditorLayer::OnImGuiRender()
	{
		GT_PROFILE_FUNCTION();

		//static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;

		if (dockspace)
		{

			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace", &dockspace, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			style.WindowMinSize.x = minWinSizeX;

			OnMenuBarRender();

			ImGui::End();

			OnViewportRender();
		}


		OnSceneHierarchyPanelRender();

		m_ContentBrowserPanel->OnImGuiRender();
		m_AssetsPanel->OnImGuiRender();

		OnStatusBarRender();

		OnSpriteSheetPanelRender();

		OnToolbarRender();

	}

	void EditorLayer::OnMenuBarRender()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+P"))
					OpenProject();

				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
					OpenScene();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R")) {}
				//ScriptEngine::ReloadAssembly();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				// µã»÷²Ëµ¥ÏîÇÐ»»ÏÔÊ¾×´Ì¬
				if (ImGui::MenuItem("Sprite Sheet Cropper", "..."))
				{
					m_SpriteSheetPanel->Open();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Setting"))
			{
				// µã»÷²Ëµ¥ÏîÇÐ»»ÏÔÊ¾×´Ì¬
				if (ImGui::MenuItem("Unreal Theme", ""))
				{
					Application::Get().GetImGuiLayer()->SetTheme(ImGuiTheme::Unreal);
				}
				if (ImGui::MenuItem("Vscode Theme", ""))
				{
					Application::Get().GetImGuiLayer()->SetTheme(ImGuiTheme::VSCode);
				}
				if (ImGui::MenuItem("Dark Theme", ""))
				{
					Application::Get().GetImGuiLayer()->SetTheme(ImGuiTheme::Dark);
				}
				if (ImGui::MenuItem("SoftLight Theme", ""))
				{
					Application::Get().GetImGuiLayer()->SetTheme(ImGuiTheme::SoftLight);
				}
				if (ImGui::MenuItem("Cyberpunk Theme", ""))
				{
					Application::Get().GetImGuiLayer()->SetTheme(ImGuiTheme::Cyberpunk);
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
	float dis = 10.0f;
	bool isregen = false;
	bool ismousefollow = false;
	bool isshowaabb = false;
	void EditorLayer::OnStatusBarRender()
	{

		ImGui::Begin("Status");

		ImGui::Separator();
		ImGui::ColorEdit4("Background Color", (float*)&color);

		ImGui::Separator();

		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
		if (ImGui::Checkbox("Show 3D AABB", &isshowaabb))
		{
			Renderer3D::ShowAABB(isshowaabb);
		}
		if (ImGui::Checkbox("Enbale MouseFollow", &ismousefollow))
		{
			m_EditorCamera.FlipMouseFollow();
		}
		if (ImGui::Checkbox("Sync", &synced))
		{
			Application::Get().GetWindow().SetVSync(synced);
		}

		if (ImGui::Checkbox("Enable Docking Space", &dockspace))
		{
		}
		if (ImGui::Checkbox("Enable Window Transparent", &IsEnableTransparentAndMousePassthrough))
		{
			Application::Get().GetWindow().SetWindowTransparentAndMousePassthrough(IsEnableTransparentAndMousePassthrough);
		}

		ImGui::SliderInt("Physics Substeps", &m_ActiveScene->substepcount, 4, 10);
		dis = m_EditorCamera.GetDistance();
		if (ImGui::SliderFloat("Distance", &dis, 1.0f, 100.0f))
		{
			m_EditorCamera.SetDistance(dis);
		}

		ImGui::SliderFloat2("UI Pos", (float*)&pos, -1.0f, 1.0f);
		ImGui::SliderFloat2("UI Size", (float*)&size, -1.0f, 1.0f);
		ImGui::ColorEdit4("UI Color", (float*)&_color);
		

		ImGui::Separator();

		uint64_t m_TotalMemory , m_UsedMemory;
		RenderCommand::GetMemoryUsage(m_TotalMemory, m_UsedMemory);
		ImGui::Text("GPU Memory Usage :");
		ImGui::Text("total memory : %d Mb", m_TotalMemory);
		ImGui::Text("used memory : %d Mb", m_UsedMemory);


		ImGui::Separator();

		std::string name = "None";
		if (m_HoveredEntity && m_HoveredEntity.HasComponent<TagComponent>()) name = m_HoveredEntity.GetComponent<TagComponent>().Tag;

		ImGui::Text("Mouse Hovered Entity: ");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		ImGui::Text("%s", name.c_str());
		ImGui::PopStyleColor();


		ImGui::Text("Mouse Hovered EntityID: %d", pixelData);


		ImGui::Text("ImGuizmo Operation: ");
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		ImGui::Text("%s", GetImGuizmoTypeName());
		ImGui::PopStyleColor();

		ImGui::Text("SceneHistory Size: %d", m_SceneHistory.size());

		ImGui::Separator();

		{

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
			bool open = ImGui::TreeNodeEx("Renderer2D stats");
			ImGui::PopStyleColor();

			auto stats = Renderer2D::GetStats();

			if(open)
			{
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
				ImGui::Text("Quad Count: %d", stats.QuadCount);
				ImGui::Text("Circle Count: %d", stats.CircleCount);
				ImGui::Text("Line Count: %d", stats.LineCount);
				ImGui::TreePop();
			}
		}

		ImGui::Separator();

		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
			bool open = ImGui::TreeNodeEx("Renderer3D stats");
			ImGui::PopStyleColor();

			auto stats = Renderer3D::GetStats();
			
			if (open)
			{
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Model Meshes: %d", stats.Meshes);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::TreePop();
			}

		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		auto& map = Renderer3D::GetShadowMap();
		
		ImGui::Image((void*)(uint64_t)map.GetDepthTextureID(), ImVec2(100.0f,100.0f));

		ImGui::End();
	}
	void EditorLayer::OnToolbarRender()
	{

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_ActiveScene;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		if (hasPlayButton)
		{
			Texture2D& icon = ((m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop);
			if (ImGui::ImageButton("playbutton",(ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size)))
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();

				m_SceneHierarchyPanel->SetSelectedEntity({});
			}
		}

		if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine();

			Texture2D& icon = ((m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop);
			if (ImGui::ImageButton("StimulateButton",(ImTextureID)(uint64_t)icon->GetRendererID(), 
				ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}


		//if (hasPauseButton)
		//{
		//	bool isPaused = m_ActiveScene->IsPaused();
		//	ImGui::SameLine();
		//	{
		//		Texture2D& icon = m_IconPause;
		//		if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
		//		{
		//			m_ActiveScene->SetPaused(!isPaused);
		//		}
		//	}

		//	// Step button
		//	if (isPaused)
		//	{
		//		ImGui::SameLine();
		//		{
		//			Texture2D& icon = m_IconStep;
		//			bool isPaused = m_ActiveScene->IsPaused();
		//			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
		//			{
		//				m_ActiveScene->Step();
		//			}
		//		}
		//	}
		//}

		ImGui::SameLine();
		if (ImGui::Button("2D", ImVec2(size, size)))
		{
			m_EditorCamera.SetProjectionType(Camera::ProjectionType::Orthographic);
		}

		ImGui::SameLine();

		if (ImGui::Button("3D", ImVec2(size, size)))
		{
			m_EditorCamera.SetProjectionType(Camera::ProjectionType::Perspective);
		}


		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
	void EditorLayer::OnViewportRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		// suto hide title bar
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
		ImGui::SetNextWindowClass(&window_class);

		ImGui::Begin("Viewport", NULL);


		m_ViewportActived = ImGui::IsWindowFocused() || ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportActived);

		ImVec2 windowPos = ImGui::GetWindowPos();
		m_ViewportPos = { windowPos.x,windowPos.y };

		// check if viewport size changed
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			IsResized = true;

			m_ViewportSize = { viewportPanelSize.x,viewportPanelSize.y };
		}
		

		// Target for drag and drop
		ImGui::Image((void*)(uint64_t)m_ActiveScene->m_Framebuffer->GetColorAttachmentRendererID(), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path filepath = path;
				OpenScene(filepath);
			}
			ImGui::EndDragDropTarget();
		}


		// Get viewport bounds (not include menubar)
		
		{
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			m_ViewportPosWithoutBar = { m_ViewportPos.x + viewportMinRegion.x, m_ViewportPos.y + viewportMinRegion.y };
			if (IsResized)
			{
				auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
				m_ViewportBounds[0] = { viewportMinRegion.x, viewportMinRegion.y };
				m_ViewportBounds[1] = { viewportMaxRegion.x, viewportMaxRegion.y };
			}
		}


		if(m_SceneState != SceneState::Play)
			OnImGuizmoRender();


		ImGui::End();
		ImGui::PopStyleVar();
	}
	void EditorLayer::OnImGuizmoRender()
	{
		//m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			// Gizmo
		Entity selectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();
		if (selectedEntity)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			//ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			ImGuizmo::SetRect(m_ViewportPosWithoutBar.x, m_ViewportPosWithoutBar.y, m_ViewportSize.x, m_ViewportSize.y);


			// camera
				// Runtime camera 
				/*auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
				const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				const glm::mat4& cameraProjection = camera.GetProjection();
				glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

				// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;

			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue,snapValue,snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				m_EditorCamera.CancelDraging();
				tc.Translation = translation;
				tc.Scale = scale;
				tc.Rotation = rotation;
			}

		}
	}

	void EditorLayer::OnSceneHierarchyPanelRender()
	{
		m_SceneHierarchyPanel->OnImGuiRender();
		if (m_SceneHierarchyPanel->IsDeletedActiveContext())
		{
			m_SceneHistory.erase(std::remove(m_SceneHistory.begin(), m_SceneHistory.end(), m_ActiveScene), m_SceneHistory.end());

			if (m_SceneHistory.size() != 0)
			{
				m_EditorScene = m_SceneHistory.back();
				SetActiveScene(m_EditorScene);
			}
			else
			{
				CreateNewEditorScene();
				SetActiveScene(m_EditorScene);
			}
		}
		else m_SceneHistory.erase(std::remove(m_SceneHistory.begin(), m_SceneHistory.end(), m_SceneHierarchyPanel->GetDeletedContext()), m_SceneHistory.end());

		if (m_SceneHierarchyPanel->IsSetNewActiveContext())
		{
			m_EditorScene = m_SceneHierarchyPanel->GetActiveContext();
			SetActiveScene(m_EditorScene);
		}
	}

	void EditorLayer::OnSpriteSheetPanelRender()
	{
		if (m_SpriteSheetPanel->IsOpen())
		{
			m_SpriteSheetPanel->OnImGuiRender();
		}
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			if (!camera) return;

			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera);
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		// Draw selected entity outline 
		Entity selectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();
		if (selectedEntity
			&&!(selectedEntity.HasComponent<ModelComponent>() ||
				selectedEntity.HasComponent<LightRendererComponent>()))
		{
			
			TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
			Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		}


		if (m_ShowPhysicsColliders)
		{
			// Box Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					Renderer2D::DrawRect(tc.GetTransform(), glm::vec4(0, 1, 0, 1));
				}
			}

			// Circle Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.06f);
				}
			}

		}

		

		Renderer2D::EndScene();
	}

	void EditorLayer::CreateNewEditorScene()
	{
		m_EditorScene = CreateRef<Scene>();
		m_SceneHistory.push_back(m_EditorScene);
	}

	void EditorLayer::SetActiveScene(Ref<Scene> scene)
	{
		m_ActiveScene = scene;
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		//m_ActiveScene->SetFramebuffer(m_Framebuffer);
		m_HoveredEntity = Entity();
		m_SceneHierarchyPanel->SetContext(m_ActiveScene);
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();

		m_SceneState = SceneState::Play;


		SetActiveScene(Scene::MakeCopy(m_EditorScene));

		m_ActiveScene->OnRuntimeStart();
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;

		SetActiveScene(Scene::MakeCopy(m_EditorScene));

		m_ActiveScene->OnSimulationStart();
	}

	void EditorLayer::OnSceneStop()
	{
		GT_CORE_ASSERT(m_SceneState == SceneState::Play 
			|| m_SceneState == SceneState::Simulate,"Progess Error");

		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulate)
			m_ActiveScene->OnSimulationStop();

		m_SceneState = SceneState::Edit;

		SetActiveScene(m_EditorScene);
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);
		if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
			m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(GT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(GT_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.IsRepeat())
			return false;
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (event.GetKeyCode())
		{

		case Key::Q:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case Key::W:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case Key::E:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;

		case Key::N:
			if (control)
			{
				NewScene();
				return true;
			}
			break;
		case Key::O:
			if (control)
			{
				OpenScene();
				return true;
			}
			break;
		case Key::S:
			if (control && shift)
			{
				SaveSceneAs();
				return true;
			}
			else if (control)
			{
				SaveScene();
				return true;
			}
			break;
		}
		return false;
	}
	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		switch (event.GetMouseButton())
		{
			case Mouse::ButtonLeft:
				
				if (Input::IsKeyPressed(Key::LeftControl))
				{
					if (!m_HoveredEntity) m_SceneHierarchyPanel->SetSelectedEntity(Entity());
					else m_SceneHierarchyPanel->SetSelectedEntity(m_HoveredEntity);
				}
				//GT_CORE_INFO("Mouse Button Left Pressed");
				break;
			case Mouse::ButtonRight:
				//GT_CORE_INFO("Mouse Button Right Pressed");
				break;
		default:
			break;
		}
		return false;
	}



	void EditorLayer::OpenProject()
	{
		std::filesystem::path path = FileDialogs::OpenFile("GT Project (*.hproj)\0*.hproj\0");
		OpenProject(path);
	}
	void EditorLayer::OpenProject(std::filesystem::path filepath)
	{
		m_Project = Project::Load(filepath);
		m_SceneHistory.clear();
		OpenScene(m_Project->GetAssetDirectory() / m_Project->GetConfig().StartScene);
		m_SceneHierarchyPanel->SetContexts(m_SceneHistory);

	}


	void EditorLayer::OpenScene()
	{
		std::filesystem::path filepath = FileDialogs::OpenFile("GT Scene (*.hazel)\0*.hazel\0");
		OpenScene(filepath);
	}
	void EditorLayer::OpenScene(std::filesystem::path filepath)
	{
		if (!filepath.string().empty())
		{
			if (filepath.extension().string() != ".hazel")
			{
				GT_WARN("Could not load {0} - not a scene file", filepath.filename().string());
				return;
			}

			CreateNewEditorScene();

			SetActiveScene(m_EditorScene);

			m_ActiveScene->Copy(serializer.Deserialize(filepath));
		}
		else GT_CORE_WARN("Filepath {0} is empty!",filepath.string());
	}
	void EditorLayer::NewScene()
	{
		
		CreateNewEditorScene();

		SetActiveScene(m_EditorScene);
		

	}
	void EditorLayer::SaveScene()
	{
		std::filesystem::path path = m_ActiveScene->GetFilePath();
		if (!std::filesystem::exists(path))
		{
			SaveSceneAs();
		}
		else serializer.Serialize(m_ActiveScene, path);
	}
	void EditorLayer::SaveSceneAs()
	{
		std::filesystem::path filepath = FileDialogs::SaveFile("GT Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			m_ActiveScene->SetFilePath(filepath);
			serializer.Serialize(m_ActiveScene, filepath);
		}
	}
}