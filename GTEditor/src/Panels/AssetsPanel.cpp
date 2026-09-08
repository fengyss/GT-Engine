#include "AssetsPanel.h"
#include "imgui.h"
#include "GT/Core/Asset/AssetManager.h"
namespace GT
{
	AssetsPanel::AssetsPanel()
		: Panel("AssetsPanel")
	{
		m_ModelIcon.Reset(AssetManager::GetAssetHandle("ModelIcon"));
		m_ShaderIcon.Reset(AssetManager::GetAssetHandle("ShaderIcon"));

		m_SceneIcon.Reset(AssetManager::GetAssetHandle("SceneIcon"));
		m_TextureIcon.Reset(AssetManager::GetAssetHandle("TextureIcon"));
	}
	bool DisplayAssetSlot(const Ref<Asset> asset, const uint64_t& RendererID)
	{
		UUID id = asset->ID;
		auto info = AssetManager::GetAssetInfoFromUUID(id);

		ImGui::Text("%s  %d", info->metadata.Name.c_str(), info->Refcount);
		if (ImGui::IsItemHovered())
		{
			ImGui::SameLine();
			ImGui::Text("%s", info->metadata.FilePath.string().c_str());
		}
		ImGui::SameLine();
		ImGui::Image((ImTextureID)RendererID, ImVec2(40, 40), { 0, 1 }, { 1, 0 });
		return ImGui::IsItemClicked();
	}
	void AssetsPanel::OnImGuiRender()
	{
		ImGui::Begin("Assets Panel");
		tex = scn = mat = mesh = mod = sha = 0;
		auto assetslots = AssetManager::GetAssets();
		for(auto& slot : assetslots)
		{
			if (slot.asset == nullptr) continue;
			auto type = slot.asset->GetType();
			switch (type)
			{
			case GT::AssetType::None:
				break;
			case GT::AssetType::Scene:
				scn++;
				break;
			case GT::AssetType::Texture2D:
				tex++;
				break;
			case GT::AssetType::Texture3D:
				tex++;
				break;
			case GT::AssetType::Shader:
				sha++;
				break;
			case GT::AssetType::ComputeShader:
				sha++;
				break;
			case GT::AssetType::GeometryShader:
				sha++;
				break;
			case GT::AssetType::Model:
				mod++;
				break;
			case GT::AssetType::Mesh:
				mesh++;
				break;
			default:
				break;
			}
		}


		OnTextureDisplay();
		OnModelDisplay();
		OnShaderDisplay();
		OnSceneDisplay();
		OnMaterialDisplay();
		OnMeshDisplay();


		ImGui::End();
	}
	void AssetsPanel::OnTextureDisplay()
	{
		if(ImGui::TreeNode("Texture Assets "))
		{
			ImGui::SameLine();
			ImGui::Text("Size: %d", tex);

			auto assetslots = AssetManager::GetAssets();
			for (auto& slot : assetslots)
			{
				if (slot.asset == nullptr) continue;
				auto type = slot.asset->GetType();
				if (type == GT::AssetType::Texture2D)
				{
					DisplayAssetSlot(slot.asset, std::dynamic_pointer_cast<Texture2DAsset>(slot.asset)->GetRendererID());
				}
				else if (type == GT::AssetType::Texture3D)
				{
					DisplayAssetSlot(slot.asset, m_TextureIcon->GetRendererID());
				}
			}
			ImGui::TreePop();
		}
	}
	void AssetsPanel::OnModelDisplay()
	{
		if (ImGui::TreeNode("Model Assets "))
		{
			ImGui::SameLine();
			ImGui::Text("Size: %d", mod);
			auto assetslots = AssetManager::GetAssets();
			for (auto& slot : assetslots)
			{
				if (slot.asset == nullptr) continue;
				auto type = slot.asset->GetType();
				if (type == GT::AssetType::Model)
				{
					DisplayAssetSlot(slot.asset, m_ModelIcon->GetRendererID());
				}
			}
			ImGui::TreePop();
		}
	}
	void AssetsPanel::OnShaderDisplay()
	{
		if (ImGui::TreeNode("Shader Assets "))
		{
			ImGui::SameLine();
			ImGui::Text("Size: %d", sha);
			auto assetslots = AssetManager::GetAssets();
			for (auto& slot : assetslots)
			{
				if (slot.asset == nullptr) continue;
				auto type = slot.asset->GetType();
				if (type == GT::AssetType::Shader || type == GT::AssetType::ComputeShader || type == GT::AssetType::GeometryShader)
				{
					DisplayAssetSlot(slot.asset, m_ShaderIcon->GetRendererID());
				}
			}
			ImGui::TreePop();
		}
	}
	void AssetsPanel::OnSceneDisplay()
	{
		if (ImGui::TreeNode("Scene Assets "))
		{
			ImGui::SameLine();
			ImGui::Text("Size: %d", scn);
			auto assetslots = AssetManager::GetAssets();
			for (auto& slot : assetslots)
			{
				if (slot.asset == nullptr) continue;
				auto type = slot.asset->GetType();
				if (type == GT::AssetType::Scene)
				{
					DisplayAssetSlot(slot.asset, m_SceneIcon->GetRendererID());
				}
			}
			ImGui::TreePop();
		}
	}
	void AssetsPanel::OnMaterialDisplay()
	{
		if (ImGui::TreeNode("Material Assets "))
		{
			ImGui::SameLine();
			ImGui::Text("Size: %d", mat);
			auto assetslots = AssetManager::GetAssets();
			for (auto& slot : assetslots)
			{
				if (slot.asset == nullptr) continue;
				auto type = slot.asset->GetType();
				if (type == GT::AssetType::Material)
				{
					DisplayAssetSlot(slot.asset, m_ModelIcon->GetRendererID());
				}
			}
			ImGui::TreePop();
		}
	}
	void AssetsPanel::OnMeshDisplay()
	{
		if (ImGui::TreeNode("Mesh Assets "))
		{
			ImGui::SameLine();
			ImGui::Text("Size: %d", mesh);
			auto assetslots = AssetManager::GetAssets();
			for (auto& slot : assetslots)
			{
				if (slot.asset == nullptr) continue;
				auto type = slot.asset->GetType();
				if (type == GT::AssetType::Mesh)
				{
					if(DisplayAssetSlot(slot.asset, m_ModelIcon->GetRendererID()))
					{
						auto info = AssetManager::GetAssetInfoFromUUID(slot.asset->ID);
						m_SelectedMeshIndex = info->Index;
					}
				}
			}
			ImGui::TreePop();
		}
	}
}