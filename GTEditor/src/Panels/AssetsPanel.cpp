#include "AssetsPanel.h"
#include "ImGui/imgui.h"
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
	void DisplayAssetSlot(const Ref<Asset> asset, const uint64_t& RendererID)
	{
		UUID id = asset->ID;
		auto info = AssetManager::GetAssetInfoFromUUID(id);

		ImGui::Text("%s  %d", info->metadata.Name.c_str(), info->Refcount);
		ImGui::SameLine();
		ImGui::Image((ImTextureID)RendererID, ImVec2(40, 40), { 0, 1 }, { 1, 0 });
		if (ImGui::IsItemHovered())
		{
			ImGui::SameLine();
			ImGui::Text("%s %d", info->metadata.FilePath.string().c_str(), info->Refcount);
		}
	}
	void AssetsPanel::OnImGuiRender()
	{
		ImGui::Begin("Assets Panel");
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
				DisplayAssetSlot(slot.asset, m_SceneIcon->GetRendererID());
				break;
			case GT::AssetType::Texture2D:
				//DisplayAssetSlot(slot.asset, m_TextureIcon->GetRendererID());
				DisplayAssetSlot(slot.asset, std::dynamic_pointer_cast<Texture2DAsset>(slot.asset)->GetRendererID());
				break;
			case GT::AssetType::Texture3D:
				DisplayAssetSlot(slot.asset, m_TextureIcon->GetRendererID());
				break;
			case GT::AssetType::Shader:
				DisplayAssetSlot(slot.asset, m_ShaderIcon->GetRendererID());
				break;
			case GT::AssetType::ComputeShader:
				DisplayAssetSlot(slot.asset, m_ShaderIcon->GetRendererID());
				break;
			case GT::AssetType::GeometryShader:
				DisplayAssetSlot(slot.asset, m_ShaderIcon->GetRendererID());
				break;
			case GT::AssetType::Model:
				DisplayAssetSlot(slot.asset, m_ModelIcon->GetRendererID());
				break;
			case GT::AssetType::Mesh:
				DisplayAssetSlot(slot.asset, m_SceneIcon->GetRendererID());
				break;
			default:
				break;
			}
		}
		ImGui::End();
	}
}