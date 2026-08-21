#include "AssetsPanel.h"
#include "GT/Renderer/Texture.h"
#include "ImGui/imgui.h"

namespace GT
{
	AssetsPanel::AssetsPanel()
		: Panel("AssetsPanel")
	{
		m_ModelIcon = CreateHandle<Texture2D>("ModelIcon");
		m_ShaderIcon = CreateHandle<Texture2D>("ShaderIcon");

		m_SceneIcon = CreateHandle<Texture2D>("SceneIcon");
		m_TextureIcon = CreateHandle<Texture2D>("TextureIcon");
	}
	void DisplayAssetSlot(const Ref<Asset> asset, const uint64_t& RendererID)
	{
		ImGui::Text("%s", asset->metadata.Name.c_str());
		ImGui::SameLine();
		ImGui::Image((ImTextureID)RendererID, ImVec2(40, 40), { 0, 1 }, { 1, 0 });
		if (ImGui::IsItemHovered())
		{
			ImGui::SameLine();
			ImGui::Text("%s", asset->metadata.FilePath.string().c_str());
		}
	}
	void AssetsPanel::OnImGuiRender()
	{
		ImGui::Begin("Assets Panel");
		auto assetslots = AssetManager::GetAssetSlots();
		for(auto& slot : assetslots)
		{
			auto type = slot.second->GetType();
			switch (type)
			{
			case GT::AssetType::None:
				break;
			case GT::AssetType::Scene:
				DisplayAssetSlot(slot.second, m_SceneIcon->Get()->GetRendererID());
				break;
			case GT::AssetType::Texture2D:
				//DisplayAssetSlot(slot.second, m_TextureIcon->Get()->GetRendererID());
				DisplayAssetSlot(slot.second, std::dynamic_pointer_cast<Texture2D>(slot.second)->GetRendererID());
				break;
			case GT::AssetType::Texture3D:
				DisplayAssetSlot(slot.second, m_TextureIcon->Get()->GetRendererID());
				break;
			case GT::AssetType::Shader:
				DisplayAssetSlot(slot.second, m_ShaderIcon->Get()->GetRendererID());
				break;
			case GT::AssetType::ComputeShader:
				DisplayAssetSlot(slot.second, m_ShaderIcon->Get()->GetRendererID());
				break;
			case GT::AssetType::GeometryShader:
				DisplayAssetSlot(slot.second, m_ShaderIcon->Get()->GetRendererID());
				break;
			case GT::AssetType::Model:
				DisplayAssetSlot(slot.second, m_ModelIcon->Get()->GetRendererID());
				break;
			default:
				break;
			}
		}
		ImGui::End();
	}
}