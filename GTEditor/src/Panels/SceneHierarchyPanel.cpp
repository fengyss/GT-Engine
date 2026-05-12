#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <glm/glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>
#include "GT/Utils/PlatformUtils.h"

namespace GT
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
		:m_ActiveContext(context)
	{
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_ActiveContext = context;
		m_SelectedEntity = {};
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectedEntity = entity;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		m_DeletedContext = nullptr;
		m_NewSetContext = nullptr;


		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;

		ImGui::Begin("Scene Hierarchy");


		for (auto& scene : *m_SceneHistory)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx(scene->name.c_str(), treeNodeFlags, scene->name.c_str());
			ImGui::PopStyleVar();

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Set As ActiveScene") && scene != m_ActiveContext)
				{
					m_NewSetContext = scene;
				}
				if (ImGui::MenuItem("Delete Scene"))
				{
					m_DeletedContext = scene;
				}
				ImGui::EndPopup();
			}

			if (open)
			{
				auto  view = scene->m_Registry.view<TagComponent>();
				for (auto entityID : view)
				{

					Entity e{ entityID,scene.get() };
					DrawEntityNode(e);

				}
				ImGui::TreePop();
			}
		}

		// right click on blank space
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if(ImGui::MenuItem("Create Empty Entity"))
			{
				m_ActiveContext->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();


		ImGui::Begin("Properties");

		if (m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);
		}

		ImGui::End();



		if(m_NewSetContext)
		{
			m_ActiveContext = m_NewSetContext;
			m_SelectedEntity = {};
		}

		if (m_DeletedContext)
		{
			if (m_ActiveContext == m_DeletedContext)
			{
				m_SelectedEntity = {};
				m_ActiveContext = nullptr;
			}
		}
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity e)
	{
		auto& tag = e.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0)
									| ImGuiTreeNodeFlags_OpenOnArrow;

		// now can slelct from blank og this node
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint32_t)e,flags,tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = e;
		}

		bool entityDeleted = false;


		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Duplicate  Entity"))
			{
				m_ActiveContext->DuplicateEntity(e);

			}
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}



		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			if (m_SelectedEntity == e)
				m_SelectedEntity = {};
			m_ActiveContext->DestroyEntity(e);
		}
	}

	static void DrawVec3Control(const char* label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];


		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });


		float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		//X
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,1.0f,1.0f,0.3f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Y
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.7f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,1.0f,1.0f,0.3f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.7f,0.2f,1.0f });


		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f,0.0f,0.0f,"%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Y
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.1f,0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f,1.0f,1.0f,0.3f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.1f,0.8f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		//ImGui::SameLine();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	template<typename T,typename UIFunction>
	static void DrawComponent(const char* name, Entity e, UIFunction func)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed 
								| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
		if(e.HasComponent<T>())
		{

			auto& component = e.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name);
			ImGui::PopStyleVar();

			float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+",ImVec2{lineHeight,lineHeight}))
			{
				//context menu for transform component
				ImGui::OpenPopup("ComponentSettings");
			}


			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open)
			{

				func(component);

				ImGui::TreePop();
			}

			if (removeComponent)
			{
				e.RemoveComponent<T>();
			}
		}
	}

	template<typename T>
	static void AddComponentButton(const char* name, Entity& e)
	{
		if (!e.HasComponent<T>())
		{
			if (ImGui::MenuItem(name))
			{
				e.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity& e)
	{


		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		if (e.HasComponent<TagComponent>())
		{
			auto& tag = e.GetComponent<TagComponent>();
			auto& type = tag.Type;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy(buffer, tag.Tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag.Tag = std::string(buffer);
			}
			ImGui::PopItemWidth();

			ImGui::SameLine();

			//static int type = 0;
			//ImGui::RadioButton("None", &type, 0); ImGui::SameLine();
			//ImGui::RadioButton("Sprite", &type, 1); ImGui::SameLine();
			//ImGui::RadioButton("Cube", &type, 2); ImGui::SameLine();
			//ImGui::RadioButton("Model", &type, 3); ImGui::SameLine();

			const char* Types[] = { "None","Sprite","Cube","Model" };
			static int currentType = 0;

			if (ImGui::BeginCombo("##Type", type.data()))
			{
				for (int i = 0; i < 4; i++)
				{
					bool isSelected = currentType == i;
					if (ImGui::Selectable(Types[i], isSelected))
					{
						currentType = i;
						type = Types[i];
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}
		


		ImGui::SameLine();
		//ImGui::PushItemWidth(-1);
		if (ImGui::Button("+")) ImGui::OpenPopup("AddComponent");
		if(ImGui::IsItemHovered())
			ImGui::SetTooltip("Add Component");

		if (ImGui::BeginPopup("AddComponent"))
		{
			AddComponentButton<TransformComponent>("Transform",e);
			AddComponentButton<SpriteRendererComponent>("Sprite Renderer",e);
			AddComponentButton<CircleRendererComponent>("Circle Renderer", e);
			AddComponentButton<CameraComponent>("Camera",e);
			AddComponentButton<Rigidbody2DComponent>("Rigidbody 2D", e);
			AddComponentButton<BoxCollider2DComponent>("Box Collider 2D", e);
			AddComponentButton<CircleCollider2DComponent>("Circle Collider 2D", e);
			AddComponentButton<LightRendererComponent>("Light Renderer", e);
			AddComponentButton<ModelComponent>("Model Renderer", e);
			AddComponentButton<ParticleComponent>("Particle Renderer", e);
			AddComponentButton<Animator2DComponent>("Animator 2D", e);

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>("Transform", e, [](auto& component)
			{
				auto& transform = component;
				DrawVec3Control("Translation", transform.Translation);

				glm::vec3 rotation = glm::degrees(transform.Rotation);
				DrawVec3Control("Rotation", rotation);
				transform.Rotation = glm::radians(rotation);

				DrawVec3Control("Scale", transform.Scale, 1.0f);
				
			});



		DrawComponent<CameraComponent>("Camera", e, [](auto& component)
			{
				auto& cameraComponent = component;
				auto& camera = cameraComponent.Camera;

				const char* projectionTypeStrings[] = { "Perspective","Orthographic" };
				const char* currentProjectionTypeString =
					projectionTypeStrings[(int)camera.GetProjectionType()];

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType(
								(SceneCamera::ProjectionType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				switch (camera.GetProjectionType())
				{
				case GT::SceneCamera::ProjectionType::Perspective:
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFov());
					float nearClip = camera.GetPerspectiveNearClip();
					float farClip = camera.GetPerspectiveFarClip();

					DrawVec3Control("Offset", cameraComponent.Offset);

					if (ImGui::DragFloat("Vertical FOV", &verticalFov))
						camera.SetPerspectiveVerticalFov(glm::radians(verticalFov));
					if (ImGui::DragFloat("Near Clip", &nearClip))
						camera.SetPerspectiveNearClip(nearClip);
					if (ImGui::DragFloat("Far Clip", &farClip))
						camera.SetPerspectiveFarClip(farClip);
					break;
				}
				case GT::SceneCamera::ProjectionType::Orthographic:
				{
					float orthoSize = camera.GetOrthographicSize();
					float nearClip = camera.GetOrthographicNearClip();
					float farClip = camera.GetOrthographicFarClip();

					DrawVec3Control("Offset", cameraComponent.Offset);

					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					if (ImGui::DragFloat("Near Clip", &nearClip))
						camera.SetOrthographicNearClip(nearClip);

					if (ImGui::DragFloat("Far Clip", &farClip))
						camera.SetOrthographicFarClip(farClip);
					break;
				}
				default:
					break;
				}
			});

		
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", e, [](auto& component)
			{
				ImGui::ColorEdit4("Render Color", glm::value_ptr(component.Color));
				const char* name = "None";
				if (component.texture) name = component.texture->Get()->GetName().c_str();
				ImGui::Text("Texture: %s", name);
				ImGui::SameLine();
				ImGui::Columns(1);
				//ImGui::PushItemWidth(-1);

				ImGui::Button("Texture");  //Target for drag and drop
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filepath(path);
						RefHandle<Texture2D> texture = CreateHandle<Texture2D>(filepath);

						if (texture->Get()) component.texture = texture;
						else GT_CORE_WARN("Could not load texture {0}", filepath.string());
					}
					ImGui::EndDragDropTarget();
				}

				if (component.texture)
				{
					ImGui::DragFloat2("Offset", glm::value_ptr(component.UVOffset), 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat2("Size", glm::value_ptr(component.UVSize), 0.1f, 0.0f, 1.0f);
				}

				ImGui::SliderInt("Tiling Factor", &component.TilingFactor, 1, 10);
			});

		DrawComponent<CircleRendererComponent>("Circle Renderer", e, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
			});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", e, [](auto& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.Type = (Rigidbody2DComponent::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
			});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", e, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.Offset));
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", e, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGui::DragFloat("Radius", &component.Radius);
				ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});

		DrawComponent<LightRendererComponent>("Light Renderer", e, [](auto& component)
			{
				ImGui::ColorEdit4("Render Color", glm::value_ptr(component.Color));
				const char* name = "None";
				if (component.texture) name = component.texture->Get()->GetName().c_str();
				ImGui::Text("Texture: %s", name);
				ImGui::SameLine();
				ImGui::Columns(1);
				//ImGui::PushItemWidth(-1);

				ImGui::Button("Texture");  //Target for drag and drop
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filepath(path);
						RefHandle<Texture2D> texture = CreateHandle<Texture2D>(filepath);

						if (texture->Get()) component.texture = texture;
						else GT_CORE_WARN("Could not load texture {0}", filepath.string());
					}
					ImGui::EndDragDropTarget();
				}
			});

		DrawComponent<ModelComponent>("Model Renderer", e, [](auto& component)
			{
				if (!component.model) ImGui::Text("Model: %s", component.name);
				else ImGui::Text("Model: %s", component.model->Get()->name.c_str());
				ImGui::SameLine();
				ImGui::Columns(1);
				//ImGui::PushItemWidth(-1);

				ImGui::Button("Model");  //Target for drag and drop
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path filepath(path);
						RefHandle<Model> model = CreateHandle<Model>(filepath);

						component.model = model;
						//GT_CORE_WARN("Could not load Model {0}", filepath.string());
					}
					ImGui::EndDragDropTarget();
				}
			});

			DrawComponent<Animator2DComponent>("Animator 2D", e, [](auto& component)
				{
					ImGui::Text("Name: %s", &component.name);
					ImGui::DragFloat("Duration", &component.duration, 0.5f, 1.0f, 20.0f);
					//ImGui::PushItemWidth(-1);

				});

		DrawComponent<ParticleComponent>("Particle Renderer", e, [](auto& component)
			{
				auto& config = component.Config;

				ImGui::Checkbox("Emitting", &component.IsEmitting);


				const char* Types[] = { "Point","Box","Sphere","Ring","Cone"};
				const char* type = Types[int(config.shape)];
				static int currentType = 0;

				if (ImGui::BeginCombo("##Type", type))
				{
					for (int i = 0; i < int(EmitterShape::Count); i++)
					{
						bool isSelected = currentType == i;
						if (ImGui::Selectable(Types[i], isSelected))
						{
							currentType = i;
							type = Types[i];
							config.shape = EmitterShape(i);
							component.IsRegen = true;
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				const char* Modes[] = { "None","Alpha","Additive","Multiply" };
				const char* mode = Modes[int(config.blendMode)];
				static int currentMode = 0;

				if (ImGui::BeginCombo("##BlendMode", mode))
				{
					for (int i = 0; i < int(BlendMode::Count); i++)
					{
						bool isSelected = currentMode == i;
						if (ImGui::Selectable(Modes[i], isSelected))
						{
							currentMode = i;
							mode = Modes[i];
							config.blendMode = BlendMode(i);
							component.IsRegen = true;
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}


				ImGui::Checkbox("ReGenerate Particle", &component.IsRegen);

				//ImGui::Button("Texture");  //Target for drag and drop
				//if (ImGui::BeginDragDropTarget())
				//{
				//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				//	{
				//		const wchar_t* path = (const wchar_t*)payload->Data;
				//		std::filesystem::path filepath(path);
				//		RefHandle<Texture2D> texture = CreateHandle<Texture2D>(filepath);

				//		if (texture->Get()) component.texture = texture;
				//		else GT_CORE_WARN("Could not load texture {0}", filepath.string());
				//	}
				//	ImGui::EndDragDropTarget();
				//}


				switch (config.shape)
				{
				case EmitterShape::Point:
					break;
				case EmitterShape::Box:
					break;
				case EmitterShape::Sphere:
					ImGui::Checkbox("InUnitSphere", &config.InUnitSphere);
					break;
				case EmitterShape::Ring:
					ImGui::DragFloat("Inner Circle", &config.innerRadius, 0.0f, 0.0f, 100.0f);
					ImGui::DragFloat("Outer Circle", &config.outerRadius, 1.0f, 1.0f, 100.0f);
					break;
				case EmitterShape::Cone:
					ImGui::DragFloat("Cone Angle", &config.coneAngle, 30.0f, 1.0f, 100.0f);
					break;
				}
				
				ImGui::DragFloat("Lifetime", &config.lifetime, 1.0f, 0.0f, 100.0f);
				ImGui::DragFloat("Velocity", &config.velocity, 1.0f, 1.0f, 100.0f);
				ImGui::DragFloat("Emission Rate", &config.spawnRate, 10.0f, 1.0f, 100.0f);
				ImGui::DragFloat("Size", &config.sizeStart, 10.0f, 1.0f, 100.0f);

				ImGui::DragFloat("Size Variance", &config.sizeVariance, 0.0f, 0.1f, 0.5f);

				DrawVec3Control("Position Variance", config.positionVariance,1.0f);
				DrawVec3Control("Velocity Variance", config.velocityVariance,1.0f);
				DrawVec3Control("Rotation Variance", config.rotationVariance, 1.0f);
				ImGui::ColorEdit4("Color Variance", glm::value_ptr(config.colorVariance));


				ImGui::Separator();
				ImGui::Text("Burst:");
				{
					ImGui::PushID(0);
					static ParticleBurst burst;
					ImGui::DragFloat("Start Time", &burst.time, 1.0f, 0.0f, 100.0f);
					ImGui::DragInt("Count", &(int)burst.count, 5, 20, 100);
					ImGui::DragInt("Cycles", &(int)burst.cycles, 1, 0, 30);
					ImGui::DragFloat("Interval", &burst.interval, 1.0f, 1.0f, 10.0f);
					if (ImGui::Button("Add burst"))
					{
						config.bursts.push_back(burst);
					}
					ImGui::PopID();
				}
				ImGui::Separator();
				ImGui::Text("Bursts:");
				int index = 1;
				for (auto& burst : config.bursts)
				{
					ImGui::PushID(index++);
					ImGui::DragFloat("Start Time", &burst.time, 1.0f, 0.0f, 100.0f);
					ImGui::DragInt("Count", &(int)burst.count, 5, 20, 100);
					ImGui::DragInt("Cycles", &(int)burst.cycles, 1, 0, 30);
					ImGui::DragFloat("Interval", &burst.interval, 1.0f, 1.0f, 10.0f);
					ImGui::Separator();
					ImGui::PopID();
				}

			});
	}
}
